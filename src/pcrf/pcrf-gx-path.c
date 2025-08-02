/* Gx Interface, 3GPP TS 29.212 section 4
 * Copyright (C) 2019-2025 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "pcrf-context.h"
#include "pcrf-fd-path.h"

struct rx_sess_state {
    ogs_lnode_t         node;

    os0_t               sid;                            /* Rx Session-Id */

    ogs_pcc_rule_t      pcc_rule[OGS_MAX_NUM_OF_PCC_RULE];
    int                 num_of_pcc_rule;

    struct sess_state   *gx;
};

struct sess_state {
    os0_t       sid;                /* Gx Session-Id */

    os0_t       peer_host;          /* Peer Host */

    char        *imsi_bcd;
    char        *apn;

ED3(uint8_t     ipv4:1;,
    uint8_t     ipv6:1;,
    uint8_t     reserved:6;)
    uint32_t    addr;                   /* Framed-IPv4-Address */
    uint8_t     addr6[OGS_IPV6_LEN];    /* Framed-IPv6-Prefix */

    ogs_list_t  rx_list;

    struct timespec ts;             /* Time of sending the message */
};

static OGS_POOL(sess_state_pool, struct sess_state);
static OGS_POOL(rx_sess_state_pool, struct rx_sess_state);
static ogs_thread_mutex_t sess_state_mutex;

static struct session_handler *pcrf_gx_reg = NULL;
static struct disp_hdl *hdl_gx_fb = NULL;
static struct disp_hdl *hdl_gx_ccr = NULL;

static void pcrf_gx_raa_cb(void *data, struct msg **msg);

static int encode_pcc_rule_definition(
        struct avp *avp, ogs_pcc_rule_t *pcc_rule, int flow_presence);

static __inline__ struct sess_state *new_state(os0_t sid)
{
    struct sess_state *new = NULL;

    ogs_assert(sid);

    ogs_thread_mutex_lock(&sess_state_mutex);
    ogs_pool_alloc(&sess_state_pool, &new);
    if (!new) {
        ogs_error("ogs_pool_alloc() failed");
        ogs_thread_mutex_unlock(&sess_state_mutex);
        return NULL;
    }
    memset(new, 0, sizeof(*new));

    new->sid = (os0_t)ogs_strdup((char *)sid);
    if (!new->sid) {
        ogs_error("ogs_strdup() failed");
        ogs_pool_free(&sess_state_pool, new);
        ogs_thread_mutex_unlock(&sess_state_mutex);
        return NULL;
    }

    ogs_list_init(&new->rx_list);

    ogs_thread_mutex_unlock(&sess_state_mutex);

    return new;
}

static struct rx_sess_state *add_rx_state(struct sess_state *gx, os0_t sid)
{
    struct rx_sess_state *new = NULL;

    ogs_assert(gx);
    ogs_assert(sid);

    ogs_thread_mutex_lock(&sess_state_mutex);
    ogs_pool_alloc(&rx_sess_state_pool, &new);
    if (!new) {
        ogs_error("ogs_pool_alloc() failed");
        ogs_thread_mutex_unlock(&sess_state_mutex);
        return NULL;
    }
    memset(new, 0, sizeof(*new));

    new->sid = (os0_t)ogs_strdup((char *)sid);
    if (!new->sid) {
        ogs_error("ogs_strdup() failed");
        ogs_pool_free(&rx_sess_state_pool, new);
        ogs_thread_mutex_unlock(&sess_state_mutex);
        return NULL;
    }

    new->gx = gx;

    ogs_list_add(&gx->rx_list, new);

    ogs_thread_mutex_unlock(&sess_state_mutex);

    return new;
}

static int remove_rx_state(struct rx_sess_state *rx_sess_data)
{
    struct sess_state *gx = NULL;
    int i;

    ogs_assert(rx_sess_data);
    gx = rx_sess_data->gx;
    ogs_assert(gx);

    ogs_list_remove(&gx->rx_list, rx_sess_data);

    for (i = 0; i < rx_sess_data->num_of_pcc_rule; i++) {
        OGS_PCC_RULE_FREE(&rx_sess_data->pcc_rule[i]);
    }

    if (rx_sess_data->sid)
        ogs_free(rx_sess_data->sid);

    ogs_thread_mutex_lock(&sess_state_mutex);
    ogs_pool_free(&rx_sess_state_pool, rx_sess_data);
    ogs_thread_mutex_unlock(&sess_state_mutex);

    return OGS_OK;
}

static int remove_rx_state_all(struct sess_state *gx)
{
    struct rx_sess_state *rx_sess_data = NULL, *next_rx_sess_data = NULL;

    ogs_assert(gx);

    ogs_list_for_each_safe(&gx->rx_list, next_rx_sess_data, rx_sess_data)
        remove_rx_state(rx_sess_data);

    return OGS_OK;
}

static struct rx_sess_state *find_rx_state(struct sess_state *gx, os0_t sid)
{
    struct rx_sess_state *rx_sess_data = NULL;

    ogs_assert(gx);
    ogs_assert(sid);

    ogs_list_for_each(&gx->rx_list, rx_sess_data) {
        if (!strcmp((char *)rx_sess_data->sid, (char *)sid))
            return rx_sess_data;
    }

    return NULL;
}

static void state_cleanup(struct sess_state *sess_data, os0_t sid, void *opaque)
{
    if (!sess_data) {
        ogs_error("No session state");
        return;
    }

    if (sess_data->peer_host)
        ogs_free(sess_data->peer_host);

    if (sess_data->imsi_bcd)
        ogs_free(sess_data->imsi_bcd);
    if (sess_data->apn)
        ogs_free(sess_data->apn);

    if (sess_data->ipv4)
        pcrf_sess_set_ipv4(&sess_data->addr, NULL);
    if (sess_data->ipv6)
        pcrf_sess_set_ipv6(sess_data->addr6, NULL);

    if (sess_data->sid)
        ogs_free(sess_data->sid);

    remove_rx_state_all(sess_data);

    ogs_thread_mutex_lock(&sess_state_mutex);
    ogs_pool_free(&sess_state_pool, sess_data);
    ogs_thread_mutex_unlock(&sess_state_mutex);
}

static int pcrf_gx_fb_cb(struct msg **msg, struct avp *avp,
        struct session *sess, void *opaque, enum disp_action *act)
{
    /* This CB should never be called */
    ogs_warn("Unexpected message received!");

    OGS_DIAM_STATS_MTX(
        PCRF_DIAM_PRIV_STATS_INC(gx.rx_unknown);
    )

    return ENOTSUP;
}

static int pcrf_gx_ccr_cb(struct msg **msg, struct avp *avp,
        struct session *sess, void *opaque, enum disp_action *act)
{
    int rv;
    int ret = 0, i;
    struct msg *ans = NULL, *qry = NULL;
    struct avp *avpch1 = NULL, *avpch2 = NULL;
    struct avp_hdr *hdr = NULL;
    union avp_value val;
    struct sess_state *sess_data = NULL;
    ogs_diam_gx_message_t gx_message;
    uint32_t cc_request_type = OGS_DIAM_GX_CC_REQUEST_TYPE_INITIAL_REQUEST;
    uint32_t cc_request_number = 0;
    uint32_t result_code = OGS_DIAM_MISSING_AVP;
    int error_occurred = 0;
    int charging_rule = 0;

    ogs_debug("Rx Credit-Control-Request");

    /* Validate input parameters */
    if (!msg || !*msg || !sess) {
        ogs_error("Invalid input parameters");
        return EINVAL;
    }

    /* Initialize Message */
    memset(&gx_message, 0, sizeof(ogs_diam_gx_message_t));

    /* Create answer header */
    qry = *msg;
    ret = fd_msg_new_answer_from_req(fd_g_config->cnf_dict, msg, 0);
    if (ret != 0) {
        ogs_error("Failed to create answer message");
        error_occurred = 1;
        goto out;
    }
    ans = *msg;

    /* Set the Auth-Application-Id AVP */
    ret = fd_msg_avp_new(ogs_diam_auth_application_id, 0, &avp);
    if (ret != 0) {
        ogs_error("Failed to create Auth-Application-Id AVP");
        error_occurred = 1;
        goto out;
    }
    val.i32 = OGS_DIAM_GX_APPLICATION_ID;
    ret = fd_msg_avp_setvalue(avp, &val);
    if (ret != 0) {
        ogs_error("Failed to set Auth-Application-Id value");
        error_occurred = 1;
        goto out;
    }
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    if (ret != 0) {
        ogs_error("Failed to add Auth-Application-Id AVP");
        error_occurred = 1;
        goto out;
    }

    /* Get CC-Request-Type */
    ret = fd_msg_search_avp(qry, ogs_diam_gx_cc_request_type, &avp);
    if (ret != 0) {
        ogs_error("Failed to search CC-Request-Type AVP");
        error_occurred = 1;
        goto out;
    }

    if (avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        if (ret != 0) {
            ogs_error("Failed to get CC-Request-Type AVP header");
            error_occurred = 1;
            goto out;
        }
        cc_request_type = hdr->avp_value->i32;
    } else {
        ogs_error("No CC-Request-Type found");
        result_code = OGS_DIAM_MISSING_AVP;
        error_occurred = 1;
        goto out;
    }

    /* Get CC-Request-Number */
    ret = fd_msg_search_avp(qry, ogs_diam_gx_cc_request_number, &avp);
    if (ret != 0) {
        ogs_error("Failed to search CC-Request-Number AVP");
        error_occurred = 1;
        goto out;
    }

    if (avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        if (ret != 0) {
            ogs_error("Failed to get CC-Request-Number AVP header");
            error_occurred = 1;
            goto out;
        }
        cc_request_number = hdr->avp_value->i32;
    } else {
        ogs_error("No CC-Request-Number found");
        result_code = OGS_DIAM_MISSING_AVP;
        error_occurred = 1;
        goto out;
    }

    ogs_debug("    CC-Request-Type[%d] Number[%d]",
        cc_request_type, cc_request_number);

    /* Set CC-Request-Type */
    ret = fd_msg_avp_new(ogs_diam_gx_cc_request_type, 0, &avp);
    if (ret != 0) {
        ogs_error("Failed to create CC-Request-Type AVP");
        error_occurred = 1;
        goto out;
    }
    val.i32 = cc_request_type;
    ret = fd_msg_avp_setvalue(avp, &val);
    if (ret != 0) {
        ogs_error("Failed to set CC-Request-Type value");
        error_occurred = 1;
        goto out;
    }
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    if (ret != 0) {
        ogs_error("Failed to add CC-Request-Type AVP");
        error_occurred = 1;
        goto out;
    }

    /* Set CC-Request-Number */
    ret = fd_msg_avp_new(ogs_diam_gx_cc_request_number, 0, &avp);
    if (ret != 0) {
        ogs_error("Failed to create CC-Request-Number AVP");
        error_occurred = 1;
        goto out;
    }
    val.i32 = cc_request_number;
    ret = fd_msg_avp_setvalue(avp, &val);
    if (ret != 0) {
        ogs_error("Failed to set CC-Request-Number value");
        error_occurred = 1;
        goto out;
    }
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    if (ret != 0) {
        ogs_error("Failed to add CC-Request-Number AVP");
        error_occurred = 1;
        goto out;
    }

    /* Find Session */
    ret = fd_sess_state_retrieve(pcrf_gx_reg, sess, &sess_data);
    if (ret != 0) {
        ogs_error("Failed to retrieve session state");
        error_occurred = 1;
        goto out;
    }

    /* Check Session */
    if (!sess_data &&
        (cc_request_type == OGS_DIAM_GX_CC_REQUEST_TYPE_UPDATE_REQUEST ||
        cc_request_type == OGS_DIAM_GX_CC_REQUEST_TYPE_TERMINATION_REQUEST)) {
        ogs_error("No Session for CC-Request-Type: [%d]", cc_request_type);
        result_code = OGS_DIAM_UNKNOWN_SESSION_ID;
        error_occurred = 1;
        goto out;
    }

    if (!sess_data) {
        os0_t sid = NULL;
        size_t sidlen;

        ret = fd_sess_getsid(sess, &sid, &sidlen);
        if (ret != 0) {
            ogs_error("Failed to get session ID");
            error_occurred = 1;
            goto out;
        }

        sess_data = new_state(sid);
        if (!sess_data) {
            ogs_error("Failed to create new session state");
            error_occurred = 1;
            goto out;
        }
    }

    /* Get Origin-Host */
    ret = fd_msg_search_avp(qry, ogs_diam_origin_host, &avp);
    if (ret != 0) {
        ogs_error("Failed to search Origin-Host AVP");
        error_occurred = 1;
        goto out;
    }

    if (avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        if (ret != 0) {
            ogs_error("Failed to get Origin-Host AVP header");
            error_occurred = 1;
            goto out;
        }

        if (sess_data->peer_host) {
            ogs_free(sess_data->peer_host);
            sess_data->peer_host = NULL;
        }
        sess_data->peer_host =
            (os0_t)ogs_strdup((char *)hdr->avp_value->os.data);
        if (!sess_data->peer_host) {
            ogs_error("Failed to duplicate peer host");
            error_occurred = 1;
            goto out;
        }
    } else {
        ogs_error("No Origin-Host found");
        result_code = OGS_DIAM_MISSING_AVP;
        error_occurred = 1;
        goto out;
    }

    /* Get Framed-IP-Address */
    ret = fd_msg_search_avp(qry, ogs_diam_gx_framed_ip_address, &avp);
    if (ret == 0 && avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        if (ret == 0 && hdr &&
            hdr->avp_value->os.len == sizeof(sess_data->addr)) {
            memcpy(&sess_data->addr,
                    hdr->avp_value->os.data, hdr->avp_value->os.len);
            pcrf_sess_set_ipv4(&sess_data->addr, sess_data->sid);
            sess_data->ipv4 = 1;
        } else if (ret != 0) {
            ogs_warn("Failed to get Framed-IP-Address AVP header");
        }
    }

    /* Get Framed-IPv6-Prefix */
    ret = fd_msg_search_avp(qry, ogs_diam_gx_framed_ipv6_prefix, &avp);
    if (ret == 0 && avp) {
        ogs_paa_t *paa = NULL;

        ret = fd_msg_avp_hdr(avp, &hdr);
        if (ret == 0 && hdr) {
            paa = (ogs_paa_t *)hdr->avp_value->os.data;
            if (paa && paa->len == OGS_IPV6_DEFAULT_PREFIX_LEN) {
                memcpy(sess_data->addr6, paa->addr6, paa->len >> 3);
                pcrf_sess_set_ipv6(sess_data->addr6, sess_data->sid);
                sess_data->ipv6 = 1;
            }
        } else if (ret != 0) {
            ogs_warn("Failed to get Framed-IPv6-Prefix AVP header");
        }
    }

    /* Get IMSI + APN */
    ret = fd_msg_search_avp(qry, ogs_diam_subscription_id, &avp);
    if (ret != 0) {
        ogs_error("Failed to search Subscription-Id AVP");
        error_occurred = 1;
        goto out;
    }

    if (avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        if (ret != 0) {
            ogs_error("Failed to get Subscription-Id AVP header");
            error_occurred = 1;
            goto out;
        }

        ret = fd_avp_search_avp(avp, ogs_diam_subscription_id_type, &avpch1);
        if (ret != 0) {
            ogs_error("Failed to search Subscription-Id-Type AVP");
            error_occurred = 1;
            goto out;
        }

        if (avpch1) {
            ret = fd_msg_avp_hdr(avpch1, &hdr);
            if (ret != 0) {
                ogs_error("Failed to get Subscription-Id-Type AVP header");
                error_occurred = 1;
                goto out;
            }
            if (hdr->avp_value->i32 !=
                    OGS_DIAM_SUBSCRIPTION_ID_TYPE_END_USER_IMSI) {
                ogs_error("Not implemented Subscription-Id-Type(%d)",
                        hdr->avp_value->i32);
                result_code = OGS_DIAM_AVP_UNSUPPORTED;
                error_occurred = 1;
                goto out;
            }
        } else {
            ogs_error("No Subscription-Id-Type found");
            result_code = OGS_DIAM_MISSING_AVP;
            error_occurred = 1;
            goto out;
        }

        ret = fd_avp_search_avp(avp, ogs_diam_subscription_id_data, &avpch1);
        if (ret != 0) {
            ogs_error("Failed to search Subscription-Id-Data AVP");
            error_occurred = 1;
            goto out;
        }

        if (avpch1) {
            ret = fd_msg_avp_hdr(avpch1, &hdr);
            if (ret != 0) {
                ogs_error("Failed to get Subscription-Id-Data AVP header");
                error_occurred = 1;
                goto out;
            }
            if (sess_data->imsi_bcd) {
                ogs_free(sess_data->imsi_bcd);
                sess_data->imsi_bcd = NULL;
            }
            sess_data->imsi_bcd = ogs_strdup((char *)hdr->avp_value->os.data);
            if (!sess_data->imsi_bcd) {
                ogs_error("Failed to duplicate IMSI");
                error_occurred = 1;
                goto out;
            }
        } else {
            ogs_error("No Subscription-Id-Data found");
            result_code = OGS_DIAM_MISSING_AVP;
            error_occurred = 1;
            goto out;
        }
    }

    if (!sess_data->imsi_bcd) {
        ogs_error("No Subscription-Id");
        result_code = OGS_DIAM_MISSING_AVP;
        error_occurred = 1;
        goto out;
    }

    ret = fd_msg_search_avp(qry, ogs_diam_gx_called_station_id, &avp);
    if (ret != 0) {
        ogs_error("Failed to search Called-Station-Id AVP");
        error_occurred = 1;
        goto out;
    }

    if (avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        if (ret != 0) {
            ogs_error("Failed to get Called-Station-Id AVP header");
            error_occurred = 1;
            goto out;
        }
        if (sess_data->apn) {
            ogs_free(sess_data->apn);
            sess_data->apn = NULL;
        }
        sess_data->apn = ogs_strdup((char *)hdr->avp_value->os.data);
        if (!sess_data->apn) {
            ogs_error("Failed to duplicate APN");
            error_occurred = 1;
            goto out;
        }
    }

    if (!sess_data->apn) {
        ogs_error("No Called-Station-Id");
        result_code = OGS_DIAM_MISSING_AVP;
        error_occurred = 1;
        goto out;
    }

    /* Retrieve QoS Data from Database */
    rv = pcrf_db_qos_data(
            sess_data->imsi_bcd, sess_data->apn, &gx_message.session_data);
    if (rv != OGS_OK) {
        ogs_error("Cannot get data for IMSI(%s)+APN(%s)",
                sess_data->imsi_bcd, sess_data->apn);
        result_code = OGS_DIAM_UNKNOWN_SESSION_ID;
        error_occurred = 1;
        goto out;
    }

    if (cc_request_type == OGS_DIAM_GX_CC_REQUEST_TYPE_INITIAL_REQUEST ||
        cc_request_type == OGS_DIAM_GX_CC_REQUEST_TYPE_UPDATE_REQUEST) {

        for (i = 0; i < gx_message.session_data.num_of_pcc_rule; i++) {
            ogs_pcc_rule_t *pcc_rule = &gx_message.session_data.pcc_rule[i];
            if (pcc_rule->num_of_flow) {
                if (charging_rule == 0) {
                    ret = fd_msg_avp_new(
                            ogs_diam_gx_charging_rule_install, 0, &avp);
                    if (ret != 0) {
                        ogs_error("Failed to create Charging-Rule-Install AVP");
                        error_occurred = 1;
                        goto out;
                    }
                    charging_rule = 1;
                }

                rv = encode_pcc_rule_definition(avp, pcc_rule, 1);
                if (rv != OGS_OK) {
                    ogs_error("Failed to encode PCC rule definition");
                    error_occurred = 1;
                    goto out;
                }
            }
        }

        if (charging_rule) {
            ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
            if (ret != 0) {
                ogs_error("Failed to add Charging-Rule-Install AVP");
                error_occurred = 1;
                goto out;
            }
        }

        /* Set QoS-Information */
        if (gx_message.session_data.session.ambr.downlink ||
                gx_message.session_data.session.ambr.uplink) {
            ret = fd_msg_avp_new(ogs_diam_gx_qos_information, 0, &avp);
            if (ret != 0) {
                ogs_error("Failed to create QoS-Information AVP");
                error_occurred = 1;
                goto out;
            }

            if (gx_message.session_data.session.ambr.uplink) {
                ret = fd_msg_avp_new(
                        ogs_diam_gx_apn_aggregate_max_bitrate_ul, 0, &avpch1);
                if (ret != 0) {
                    ogs_error("Failed to create APN-AMBR-UL AVP");
                    error_occurred = 1;
                    goto out;
                }
                val.u32 = ogs_uint64_to_uint32(
                            gx_message.session_data.session.ambr.uplink);
                ret = fd_msg_avp_setvalue(avpch1, &val);
                if (ret != 0) {
                    ogs_error("Failed to set APN-AMBR-UL value");
                    error_occurred = 1;
                    goto out;
                }
                ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avpch1);
                if (ret != 0) {
                    ogs_error("Failed to add APN-AMBR-UL AVP");
                    error_occurred = 1;
                    goto out;
                }
            }

            if (gx_message.session_data.session.ambr.downlink) {
                ret = fd_msg_avp_new(
                        ogs_diam_gx_apn_aggregate_max_bitrate_dl, 0, &avpch1);
                if (ret != 0) {
                    ogs_error("Failed to create APN-AMBR-DL AVP");
                    error_occurred = 1;
                    goto out;
                }
                val.u32 = ogs_uint64_to_uint32(
                            gx_message.session_data.session.ambr.downlink);
                ret = fd_msg_avp_setvalue(avpch1, &val);
                if (ret != 0) {
                    ogs_error("Failed to set APN-AMBR-DL value");
                    error_occurred = 1;
                    goto out;
                }
                ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avpch1);
                if (ret != 0) {
                    ogs_error("Failed to add APN-AMBR-DL AVP");
                    error_occurred = 1;
                    goto out;
                }
            }

            ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
            if (ret != 0) {
                ogs_error("Failed to add QoS-Information AVP");
                error_occurred = 1;
                goto out;
            }
        }

        /* Set Default-EPS-Bearer-QoS */
        ret = fd_msg_avp_new(ogs_diam_gx_default_eps_bearer_qos, 0, &avp);
        if (ret != 0) {
            ogs_error("Failed to create Default-EPS-Bearer-QoS AVP");
            error_occurred = 1;
            goto out;
        }

        ret = fd_msg_avp_new(ogs_diam_gx_qos_class_identifier, 0, &avpch1);
        if (ret != 0) {
            ogs_error("Failed to create QoS-Class-Identifier AVP");
            error_occurred = 1;
            goto out;
        }
        val.u32 = gx_message.session_data.session.qos.index;
        ret = fd_msg_avp_setvalue(avpch1, &val);
        if (ret != 0) {
            ogs_error("Failed to set QoS-Class-Identifier value");
            error_occurred = 1;
            goto out;
        }
        ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avpch1);
        if (ret != 0) {
            ogs_error("Failed to add QoS-Class-Identifier AVP");
            error_occurred = 1;
            goto out;
        }

        ret = fd_msg_avp_new(
                ogs_diam_gx_allocation_retention_priority, 0, &avpch1);
        if (ret != 0) {
            ogs_error("Failed to create ARP AVP");
            error_occurred = 1;
            goto out;
        }

        ret = fd_msg_avp_new(ogs_diam_gx_priority_level, 0, &avpch2);
        if (ret != 0) {
            ogs_error("Failed to create Priority-Level AVP");
            error_occurred = 1;
            goto out;
        }
        val.u32 = gx_message.session_data.session.qos.arp.priority_level;
        ret = fd_msg_avp_setvalue(avpch2, &val);
        if (ret != 0) {
            ogs_error("Failed to set Priority-Level value");
            error_occurred = 1;
            goto out;
        }
        ret = fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2);
        if (ret != 0) {
            ogs_error("Failed to add Priority-Level AVP");
            error_occurred = 1;
            goto out;
        }

        ret = fd_msg_avp_new(ogs_diam_gx_pre_emption_capability, 0, &avpch2);
        if (ret != 0) {
            ogs_error("Failed to create Pre-emption-Capability AVP");
            error_occurred = 1;
            goto out;
        }
        val.u32 = OGS_EPC_PRE_EMPTION_DISABLED;
        if (gx_message.session_data.session.qos.arp.pre_emption_capability ==
                OGS_5GC_PRE_EMPTION_ENABLED)
            val.u32 = OGS_EPC_PRE_EMPTION_ENABLED;
        ret = fd_msg_avp_setvalue(avpch2, &val);
        if (ret != 0) {
            ogs_error("Failed to set Pre-emption-Capability value");
            error_occurred = 1;
            goto out;
        }
        ret = fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2);
        if (ret != 0) {
            ogs_error("Failed to add Pre-emption-Capability AVP");
            error_occurred = 1;
            goto out;
        }

        ret = fd_msg_avp_new(ogs_diam_gx_pre_emption_vulnerability, 0, &avpch2);
        if (ret != 0) {
            ogs_error("Failed to create Pre-emption-Vulnerability AVP");
            error_occurred = 1;
            goto out;
        }
        val.u32 = OGS_EPC_PRE_EMPTION_DISABLED;
        if (gx_message.session_data.session.qos.arp.pre_emption_vulnerability ==
                OGS_5GC_PRE_EMPTION_ENABLED)
            val.u32 = OGS_EPC_PRE_EMPTION_ENABLED;
        ret = fd_msg_avp_setvalue(avpch2, &val);
        if (ret != 0) {
            ogs_error("Failed to set Pre-emption-Vulnerability value");
            error_occurred = 1;
            goto out;
        }
        ret = fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2);
        if (ret != 0) {
            ogs_error("Failed to add Pre-emption-Vulnerability AVP");
            error_occurred = 1;
            goto out;
        }

        ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avpch1);
        if (ret != 0) {
            ogs_error("Failed to add ARP AVP");
            error_occurred = 1;
            goto out;
        }

        ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
        if (ret != 0) {
            ogs_error("Failed to add Default-EPS-Bearer-QoS AVP");
            error_occurred = 1;
            goto out;
        }

        /* Set Supported Features */
        ret = fd_msg_avp_new(ogs_diam_gx_supported_features, 0, &avp);
        if (ret != 0) {
            ogs_error("Failed to create Supported-Features AVP");
            error_occurred = 1;
            goto out;
        }

        ret = fd_msg_avp_new(ogs_diam_vendor_id, 0, &avpch1);
        if (ret != 0) {
            ogs_error("Failed to create Vendor-Id AVP");
            error_occurred = 1;
            goto out;
        }
        val.i32 = OGS_3GPP_VENDOR_ID;
        ret = fd_msg_avp_setvalue(avpch1, &val);
        if (ret != 0) {
            ogs_error("Failed to set Vendor-Id value");
            error_occurred = 1;
            goto out;
        }
        ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avpch1);
        if (ret != 0) {
            ogs_error("Failed to add Vendor-Id AVP");
            error_occurred = 1;
            goto out;
        }

        ret = fd_msg_avp_new(ogs_diam_gx_feature_list_id, 0, &avpch1);
        if (ret != 0) {
            ogs_error("Failed to create Feature-List-ID AVP");
            error_occurred = 1;
            goto out;
        }
        val.i32 = 1;
        ret = fd_msg_avp_setvalue(avpch1, &val);
        if (ret != 0) {
            ogs_error("Failed to set Feature-List-ID value");
            error_occurred = 1;
            goto out;
        }
        ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avpch1);
        if (ret != 0) {
            ogs_error("Failed to add Feature-List-ID AVP");
            error_occurred = 1;
            goto out;
        }

        ret = fd_msg_avp_new(ogs_diam_gx_feature_list, 0, &avpch1);
        if (ret != 0) {
            ogs_error("Failed to create Feature-List AVP");
            error_occurred = 1;
            goto out;
        }
        val.u32 = 0x0000000b;
        ret = fd_msg_avp_setvalue(avpch1, &val);
        if (ret != 0) {
            ogs_error("Failed to set Feature-List value");
            error_occurred = 1;
            goto out;
        }
        ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avpch1);
        if (ret != 0) {
            ogs_error("Failed to add Feature-List AVP");
            error_occurred = 1;
            goto out;
        }

        ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
        if (ret != 0) {
            ogs_error("Failed to add Supported-Features AVP");
            error_occurred = 1;
            goto out;
        }

    } else if (cc_request_type ==
            OGS_DIAM_GX_CC_REQUEST_TYPE_TERMINATION_REQUEST) {
        struct rx_sess_state *rx_sess_data = NULL, *next_rx_sess_data = NULL;
        ogs_list_for_each_safe(&sess_data->rx_list,
                next_rx_sess_data, rx_sess_data) {
            rv = pcrf_rx_send_asr(
                    rx_sess_data->sid, OGS_DIAM_RX_ABORT_CAUSE_BEARER_RELEASED);
            if (rv != OGS_OK) {
                ogs_warn("Failed to send ASR for RX session");
                /* Continue processing other sessions */
            }

            remove_rx_state(rx_sess_data);
        }
    }

    if (!error_occurred) {
        /* Set success result code */
        ret = fd_msg_rescode_set(ans, (char *)"DIAMETER_SUCCESS", NULL, NULL, 1);
        if (ret != 0) {
            ogs_error("Failed to set success result code");
            error_occurred = 1;
            goto out;
        }

        /* Store or cleanup session based on request type */
        if (cc_request_type != OGS_DIAM_GX_CC_REQUEST_TYPE_TERMINATION_REQUEST) {
            /* Store this value in the session */
            ret = fd_sess_state_store(pcrf_gx_reg, sess, &sess_data);
            if (ret != 0) {
                ogs_error("Failed to store session state");
                error_occurred = 1;
                goto out;
            }
            sess_data = NULL; /* Ownership transferred */
        } else {
            state_cleanup(sess_data, NULL, NULL);
            sess_data = NULL;
        }

        /* Send the answer */
        ret = fd_msg_send(msg, NULL, NULL);
        if (ret != 0) {
            ogs_error("Failed to send message");
            error_occurred = 1;
            goto out;
        }

        ogs_debug("Tx Credit-Control-Answer");

        /* Add to stats */
        OGS_DIAM_STATS_MTX(
            OGS_DIAM_STATS_INC(nb_echoed);
            PCRF_DIAM_PRIV_STATS_INC(gx.rx_ccr);
            PCRF_DIAM_PRIV_STATS_INC(gx.tx_cca);
        )

        OGS_SESSION_DATA_FREE(&gx_message.session_data);
        return 0;
    }

out:
    /* Error handling */
    if (ans) {
        /* Set the Result-Code */
        if (result_code == OGS_DIAM_AVP_UNSUPPORTED) {
            ret = fd_msg_rescode_set(ans,
                        (char *)"DIAMETER_AVP_UNSUPPORTED", NULL, NULL, 1);
        } else if (result_code == OGS_DIAM_UNKNOWN_SESSION_ID) {
            ret = fd_msg_rescode_set(ans,
                        (char *)"DIAMETER_UNKNOWN_SESSION_ID", NULL, NULL, 1);
        } else if (result_code == OGS_DIAM_MISSING_AVP) {
            ret = fd_msg_rescode_set(ans,
                        (char *)"DIAMETER_MISSING_AVP", NULL, NULL, 1);
        } else {
            ret = ogs_diam_message_experimental_rescode_set(ans, result_code);
        }

        if (ret != 0) {
            ogs_error("Failed to set error result code");
        }

        /* Handle session state for error cases */
        if (sess_data) {
            if (cc_request_type !=
                    OGS_DIAM_GX_CC_REQUEST_TYPE_TERMINATION_REQUEST) {
                /* Store this value in the session */
                ret = fd_sess_state_store(pcrf_gx_reg, sess, &sess_data);
                if (ret != 0) {
                    ogs_error("Failed to store session state in error path");
                }
                sess_data = NULL; /* Ownership transferred */
            } else {
                state_cleanup(sess_data, NULL, NULL);
                sess_data = NULL;
            }
        }

        ret = fd_msg_send(msg, NULL, NULL);
        if (ret != 0) {
            ogs_error("Failed to send error response");
        }
    }

    /* Update error stats */
    OGS_DIAM_STATS_MTX(
        PCRF_DIAM_PRIV_STATS_INC(gx.rx_ccr);
        PCRF_DIAM_PRIV_STATS_INC(gx.rx_ccr_error);
    )

    /* Always free session data */
    OGS_SESSION_DATA_FREE(&gx_message.session_data);

    return 0;
}

int pcrf_gx_send_rar(
        uint8_t *gx_sid, uint8_t *rx_sid, ogs_diam_rx_message_t *rx_message)
{
    int rv;
    int ret = 0, i, j;

    struct msg *req = NULL;
    struct avp *avp, *avpch1;
    union avp_value val;
    struct sess_state *sess_data = NULL, *svg;
    struct rx_sess_state *rx_sess_data = NULL;
    struct session *session = NULL;
    int new;
    size_t sidlen;

    ogs_diam_gx_message_t gx_message;
    int charging_rule = 0;

    ogs_assert(gx_sid);
    ogs_assert(rx_sid);
    ogs_assert(rx_message);

    ogs_debug("[PCRF] Tx Re-Auth-Request");

    /* Initialize Message */
    memset(&gx_message, 0, sizeof(ogs_diam_gx_message_t));

    /* Set default error result code */
    rx_message->result_code = OGS_DIAM_UNKNOWN_SESSION_ID;

    /* Create the request */
    ret = fd_msg_new(ogs_diam_gx_cmd_rar, MSGFL_ALLOC_ETEID, &req);
    ogs_assert(ret == 0);
    {
        struct msg_hdr * h;
        ret = fd_msg_hdr( req, &h );
        ogs_assert(ret == 0);
        h->msg_appl = OGS_DIAM_GX_APPLICATION_ID;
    }

    /* Retrieve session by Session-Id */
    sidlen = strlen((char *)gx_sid);
    ret = fd_sess_fromsid_msg((os0_t)gx_sid, sidlen, &session, &new);
    ogs_assert(ret == 0);
    if (new) {
        ogs_error("No session data");
        ret = fd_msg_free(req);
        ogs_assert(ret == 0);
        rx_message->result_code = OGS_DIAM_UNKNOWN_PEER;
        return OGS_ERROR;
    }

    /* Add Session-Id to the message */
    ret = ogs_diam_message_session_id_set(req, (os0_t)gx_sid, sidlen);
    ogs_assert(ret == 0);

    /* Save the session associated with the message */
    ret = fd_msg_sess_set(req, session);
    ogs_assert(ret == 0);

    /* Retrieve session state in this session */
    ret = fd_sess_state_retrieve(pcrf_gx_reg, session, &sess_data);
    ogs_assert(ret == 0);
    if (sess_data == NULL) {
        ogs_error("No Session Data");
        ret = fd_msg_free(req);
        ogs_assert(ret == 0);
        rx_message->result_code = OGS_DIAM_UNKNOWN_SESSION_ID;
        return OGS_ERROR;
    }

    /* Find RX session state */
    rx_sess_data = find_rx_state(sess_data, rx_sid);
    if (rx_message->cmd_code == OGS_DIAM_RX_CMD_CODE_AA) {
        if (!rx_sess_data) {
            rx_sess_data = add_rx_state(sess_data, rx_sid);
            ogs_assert(rx_sess_data);
        }

        /* Retrieve QoS Data from Database */
        rv = pcrf_db_qos_data(
                sess_data->imsi_bcd, sess_data->apn, &gx_message.session_data);
        if (rv != OGS_OK) {
            ogs_error("Cannot get data for IMSI(%s)+APN(%s)'",
                    sess_data->imsi_bcd, sess_data->apn);
            rx_message->result_code =
                OGS_DIAM_RX_DIAMETER_IP_CAN_SESSION_NOT_AVAILABLE;
            goto out;
        }

        /* Match Media-Component with PCC Rule */
        for (i = 0; i < rx_message->ims_data.num_of_media_component; i++) {
            int flow_presence = 0;
            ogs_pcc_rule_t *pcc_rule = NULL;
            ogs_pcc_rule_t *db_pcc_rule = NULL;
            uint8_t qos_index = 0;
            ogs_media_component_t *media_component =
                &rx_message->ims_data.media_component[i];

            switch(media_component->media_type) {
            case OGS_DIAM_RX_MEDIA_TYPE_AUDIO:
                qos_index = OGS_QOS_INDEX_1;
                break;
            case OGS_DIAM_RX_MEDIA_TYPE_VIDEO:
                qos_index = OGS_QOS_INDEX_2;
                break;
            case OGS_DIAM_RX_MEDIA_TYPE_CONTROL:
                qos_index = OGS_QOS_INDEX_5;
                break;
            default:
                ogs_error("Not implemented : [Media-Type:%d]",
                        media_component->media_type);
                rx_message->result_code = OGS_DIAM_INVALID_AVP_VALUE;
                goto out;
            }

            for (j = 0; j < gx_message.session_data.num_of_pcc_rule; j++) {
                if (gx_message.session_data.pcc_rule[j].qos.index ==
                        qos_index) {
                    db_pcc_rule = &gx_message.session_data.pcc_rule[j];
                    break;
                }
            }

            if (!db_pcc_rule && (media_component->media_type
                    == OGS_DIAM_RX_MEDIA_TYPE_CONTROL)) {
                /*
                 * Check for default bearer for IMS signalling
                 * QCI 5 and ARP 1
                 */
                if (gx_message.session_data.
                        session.qos.index != OGS_QOS_INDEX_5 ||
                    gx_message.session_data.
                        session.qos.arp.priority_level != 1) {
                    ogs_error("CHECK WEBUI : Even the Default "
                        "Bearer(QCI:%d,ARP:%d) cannot support IMS signalling.",
                        gx_message.session_data.session.qos.index,
                        gx_message.session_data.session.qos.arp.priority_level);
                    rx_message->result_code =
                        OGS_DIAM_RX_DIAMETER_REQUESTED_SERVICE_NOT_AUTHORIZED;
                    goto out;
                } else {
                    continue;
                }
            }

            if (!db_pcc_rule) {
                ogs_error("CHECK WEBUI : No PCC Rule in DB [QoS Index:%d]",
                        qos_index);
                ogs_error("Please add PCC Rule using WEBUI");
                rx_message->result_code =
                    OGS_DIAM_RX_DIAMETER_REQUESTED_SERVICE_NOT_AUTHORIZED;
                goto out;
            }

            for (j = 0; j < rx_sess_data->num_of_pcc_rule; j++) {
                if (rx_sess_data->pcc_rule[j].qos.index == qos_index) {
                    pcc_rule = &rx_sess_data->pcc_rule[j];
                    break;
                }
            }

            if (!pcc_rule) {
                pcc_rule =
                    &rx_sess_data->pcc_rule[rx_sess_data->num_of_pcc_rule];

                /* Device PCC Rule Info from DB Profile */
                pcc_rule->name = ogs_msprintf("%s-r%d", db_pcc_rule->name,
                    (int)ogs_pool_index(&rx_sess_state_pool, rx_sess_data));
                ogs_assert(pcc_rule->name);

                memcpy(&pcc_rule->qos, &db_pcc_rule->qos, sizeof(ogs_qos_t));

                pcc_rule->flow_status = db_pcc_rule->flow_status;
                pcc_rule->precedence = db_pcc_rule->precedence;

                /* Install Flow */
                flow_presence = 1;
                rv = ogs_pcc_rule_install_flow_from_media(
                        pcc_rule, media_component);
                if (rv != OGS_OK) {
                    rx_message->result_code =
                        OGS_DIAM_RX_DIAMETER_FILTER_RESTRICTIONS;
                    ogs_error("install_flow() failed");
                    goto out;
                }

                rx_sess_data->num_of_pcc_rule++;
            } else {
                int count = 0;

                /* Check Flow */
                count = ogs_pcc_rule_num_of_flow_equal_to_media(
                        pcc_rule, media_component);
                if (count == -1) {
                    rx_message->result_code =
                        OGS_DIAM_RX_DIAMETER_FILTER_RESTRICTIONS;
                    ogs_error("matched_flow() failed");
                    goto out;
                }

                if (pcc_rule->num_of_flow != count) {
                    /* Re-install Flow */
                    flow_presence = 1;
                    rv = ogs_pcc_rule_install_flow_from_media(
                            pcc_rule, media_component);
                    if (rv != OGS_OK) {
                        rx_message->result_code =
                            OGS_DIAM_RX_DIAMETER_FILTER_RESTRICTIONS;
                        ogs_error("install_flow() failed");
                        goto out;
                    }
                }
            }

            /* Update QoS */
            rv = ogs_pcc_rule_update_qos_from_media(pcc_rule, media_component);
            if (rv != OGS_OK) {
                rx_message->result_code =
                    OGS_DIAM_RX_DIAMETER_REQUESTED_SERVICE_NOT_AUTHORIZED;
                ogs_error("update_qos() failed");
                goto out;
            }

            /* if we failed to get QoS from IMS, apply WEBUI QoS */
            if (pcc_rule->qos.mbr.downlink == 0)
                pcc_rule->qos.mbr.downlink = db_pcc_rule->qos.mbr.downlink;
            if (pcc_rule->qos.mbr.uplink == 0)
                pcc_rule->qos.mbr.uplink = db_pcc_rule->qos.mbr.uplink;
            if (pcc_rule->qos.gbr.downlink == 0)
                pcc_rule->qos.gbr.downlink = db_pcc_rule->qos.gbr.downlink;
            if (pcc_rule->qos.gbr.uplink == 0)
                pcc_rule->qos.gbr.uplink = db_pcc_rule->qos.gbr.uplink;

            if (charging_rule == 0) {
                ret = fd_msg_avp_new(
                        ogs_diam_gx_charging_rule_install, 0, &avp);
                ogs_assert(ret == 0);
                charging_rule = 1;
            }

            rv = encode_pcc_rule_definition(avp, pcc_rule, flow_presence);
            ogs_assert(rv == OGS_OK);
        }

        if (charging_rule == 1) {
            ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
            ogs_assert(ret == 0);
        }

    } else if (rx_message->cmd_code ==
            OGS_DIAM_RX_CMD_CODE_SESSION_TERMINATION) {
        ogs_assert(rx_sess_data);

        for (i = 0; i < rx_sess_data->num_of_pcc_rule; i++) {
            ogs_assert(rx_sess_data->pcc_rule[i].name);

            if (charging_rule == 0) {
                ret = fd_msg_avp_new(ogs_diam_gx_charging_rule_remove, 0, &avp);
                ogs_assert(ret == 0);
                charging_rule = 1;
            }

            ret = fd_msg_avp_new(ogs_diam_gx_charging_rule_name, 0, &avpch1);
            ogs_assert(ret == 0);
            val.os.data = (uint8_t *)rx_sess_data->pcc_rule[i].name;
            val.os.len = strlen(rx_sess_data->pcc_rule[i].name);
            ret = fd_msg_avp_setvalue(avpch1, &val);
            ogs_assert(ret == 0);
            ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avpch1);
            ogs_assert(ret == 0);
        }

        if (charging_rule == 1) {
            ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
            ogs_assert(ret == 0);
        }

        remove_rx_state(rx_sess_data);
    } else
        ogs_assert_if_reached();

    if (charging_rule == 0) {
        rx_message->result_code = ER_DIAMETER_SUCCESS;
        goto out;
    }

    /* Set Origin-Host & Origin-Realm */
    ret = fd_msg_add_origin(req, 0);
    ogs_assert(ret == 0);

    /* Set the Destination-Host AVP */
    ret = fd_msg_avp_new(ogs_diam_destination_host, 0, &avp);
    ogs_assert(ret == 0);
    val.os.data = sess_data->peer_host;
    val.os.len  = strlen((char *)sess_data->peer_host);
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Set the Destination-Realm AVP */
    ret = fd_msg_avp_new(ogs_diam_destination_realm, 0, &avp);
    ogs_assert(ret == 0);
    val.os.data = (unsigned char *)(fd_g_config->cnf_diamrlm);
    val.os.len  = strlen(fd_g_config->cnf_diamrlm);
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Set the Auth-Application-Id AVP */
    ret = fd_msg_avp_new(ogs_diam_auth_application_id, 0, &avp);
    ogs_assert(ret == 0);
    val.i32 = OGS_DIAM_GX_APPLICATION_ID;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Set the Re-Auth-Request-Type */
    ret = fd_msg_avp_new(ogs_diam_re_auth_request_type, 0, &avp);
    ogs_assert(ret == 0);
    val.i32 = OGS_DIAM_RE_AUTH_REQUEST_TYPE_AUTHORIZE_ONLY;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    ret = clock_gettime(CLOCK_REALTIME, &sess_data->ts);
    ogs_assert(ret == 0);

    /* Keep a pointer to the session data for debug purpose,
     * in real life we would not need it */
    svg = sess_data;

    /* Store this value in the session */
    ret = fd_sess_state_store(pcrf_gx_reg, session, &sess_data);
    ogs_assert(ret == 0);
    ogs_assert(sess_data == NULL);

    /* Send the request */
    ret = fd_msg_send(&req, pcrf_gx_raa_cb, svg);
    ogs_assert(ret == 0);

    /* Increment the counter */
    OGS_DIAM_STATS_MTX(
        OGS_DIAM_STATS_INC(nb_sent);
        PCRF_DIAM_PRIV_STATS_INC(gx.tx_rar);
    )

    /* Set no error */
    rx_message->result_code = ER_DIAMETER_SUCCESS;

    OGS_SESSION_DATA_FREE(&gx_message.session_data);

    return OGS_OK;

out:
    /* Store this value in the session */
    ret = fd_sess_state_store(pcrf_gx_reg, session, &sess_data);
    ogs_assert(sess_data == NULL);

    OGS_DIAM_STATS_MTX(
        OGS_DIAM_STATS_INC(nb_sent);
        PCRF_DIAM_PRIV_STATS_INC(gx.tx_rar_error);
    )

    OGS_SESSION_DATA_FREE(&gx_message.session_data);

    return OGS_ERROR;
}

static void pcrf_gx_raa_cb(void *data, struct msg **msg)
{
    int ret;
    struct sess_state *sess_data = NULL;
    struct timespec ts;
    struct session *session = NULL;
    struct avp *avp = NULL, *avpch1 = NULL;
    struct avp_hdr *hdr = NULL;
    unsigned long dur;
    int error = 0;
    int new = 0;
    uint32_t result_code = 0;

    ogs_debug("[PCRF] Rx Re-Auth-Answer");

    /* Validate input parameters */
    if (!msg || !*msg) {
        ogs_error("Invalid message pointer");
        return;
    }

    /* Get current timestamp */
    ret = clock_gettime(CLOCK_REALTIME, &ts);
    if (ret != 0) {
        ogs_error("Failed to get current time");
        goto cleanup;
    }

    /* Search the session, retrieve its data */
    ret = fd_msg_sess_get(fd_g_config->cnf_dict, *msg, &session, &new);
    if (ret != 0) {
        ogs_error("Failed to get session from message");
        error++;
        goto cleanup;
    }

    if (new != 0) {
        ogs_error("Session should already exist");
        error++;
        goto cleanup;
    }

    /* Retrieve session state */
    ret = fd_sess_state_retrieve(pcrf_gx_reg, session, &sess_data);
    if (ret != 0) {
        ogs_error("Failed to retrieve session state");
        error++;
        goto cleanup;
    }

    if (!sess_data) {
        ogs_error("No Session Data");
        error++;
        goto cleanup;
    }

    /* Validate data pointer consistency */
    if ((void *)sess_data != data) {
        ogs_error("Session data pointer mismatch");
        error++;
        goto cleanup;
    }

    /* Value of Result Code */
    ret = fd_msg_search_avp(*msg, ogs_diam_result_code, &avp);
    if (ret != 0) {
        ogs_error("Failed to search Result-Code AVP");
        error++;
        goto parse_experimental_result;
    }

    if (avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        if (ret != 0) {
            ogs_error("Failed to get Result-Code AVP header");
            error++;
            goto parse_experimental_result;
        }
        result_code = hdr->avp_value->i32;
        ogs_debug("    Result Code: %d", result_code);
    } else {
parse_experimental_result:
        /* Try experimental result code */
        ret = fd_msg_search_avp(*msg, ogs_diam_experimental_result, &avp);
        if (ret != 0) {
            ogs_error("Failed to search Experimental-Result AVP");
            error++;
            goto parse_origin_host;
        }

        if (avp) {
            ret = fd_avp_search_avp(avp, ogs_diam_experimental_result_code,
                                    &avpch1);
            if (ret != 0) {
                ogs_error("Failed to search Experimental-Result-Code AVP");
                error++;
                goto parse_origin_host;
            }

            if (avpch1) {
                ret = fd_msg_avp_hdr(avpch1, &hdr);
                if (ret != 0) {
                    ogs_error("Failed to get Experimental-Result-Code header");
                    error++;
                    goto parse_origin_host;
                }
                result_code = hdr->avp_value->i32;
                ogs_debug("    Experimental Result Code: %d", result_code);
            }
        } else {
            ogs_error("No Result-Code or Experimental-Result found");
            error++;
        }
    }

parse_origin_host:
    /* Value of Origin-Host */
    ret = fd_msg_search_avp(*msg, ogs_diam_origin_host, &avp);
    if (ret == 0 && avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        if (ret == 0 && hdr) {
            ogs_debug("    From '%.*s'",
                    (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
        } else {
            ogs_warn("Failed to get Origin-Host AVP header");
        }
    } else {
        ogs_warn("No Origin-Host found");
    }

    /* Value of Origin-Realm */
    ret = fd_msg_search_avp(*msg, ogs_diam_origin_realm, &avp);
    if (ret == 0 && avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        if (ret == 0 && hdr) {
            ogs_debug("         ('%.*s')",
                    (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
        } else {
            ogs_warn("Failed to get Origin-Realm AVP header");
        }
    } else {
        ogs_warn("No Origin-Realm found");
    }

    /* Update statistics and calculate duration if we have valid session data */
    if (sess_data) {
        OGS_DIAM_STATS_MTX(
            dur = ((ts.tv_sec - sess_data->ts.tv_sec) * 1000000) +
                ((ts.tv_nsec - sess_data->ts.tv_nsec) / 1000);

            if (ogs_diam_stats_self()->stats.nb_recv) {
                /* Ponderate in the avg */
                ogs_diam_stats_self()->stats.avg =
                    (ogs_diam_stats_self()->stats.avg *
                     ogs_diam_stats_self()->stats.nb_recv + dur) /
                    (ogs_diam_stats_self()->stats.nb_recv + 1);
                /* Min, max */
                if (dur < ogs_diam_stats_self()->stats.shortest)
                    ogs_diam_stats_self()->stats.shortest = dur;
                if (dur > ogs_diam_stats_self()->stats.longest)
                    ogs_diam_stats_self()->stats.longest = dur;
            } else {
                ogs_diam_stats_self()->stats.shortest = dur;
                ogs_diam_stats_self()->stats.longest = dur;
                ogs_diam_stats_self()->stats.avg = dur;
            }

            if (error)
                ogs_diam_stats_self()->stats.nb_errs++;
            else
                ogs_diam_stats_self()->stats.nb_recv++;

            PCRF_DIAM_PRIV_STATS_INC(gx.rx_raa);
        )

        /* Display processing duration */
        if (ts.tv_nsec > sess_data->ts.tv_nsec) {
            ogs_trace("Processed in %d.%06ld sec",
                    (int)(ts.tv_sec - sess_data->ts.tv_sec),
                    (long)(ts.tv_nsec - sess_data->ts.tv_nsec) / 1000);
        } else {
            ogs_trace("Processed in %d.%06ld sec",
                    (int)(ts.tv_sec + 1 - sess_data->ts.tv_sec),
                    (long)(1000000000 + ts.tv_nsec - sess_data->ts.tv_nsec) /
                    1000);
        }

        /* Store session state back */
        ret = fd_sess_state_store(pcrf_gx_reg, session, &sess_data);
        if (ret != 0) {
            ogs_error("Failed to store session state");
        } else {
            sess_data = NULL; /* Ownership transferred */
        }
    }

cleanup:
    /* Always free the message */
    if (msg && *msg) {
        ret = fd_msg_free(*msg);
        if (ret != 0) {
            ogs_error("Failed to free message");
        }
        *msg = NULL;
    }

    return;
}

int pcrf_gx_init(void)
{
    int ret;
    struct disp_when data;

    ogs_thread_mutex_init(&sess_state_mutex);
    ogs_pool_init(&sess_state_pool, ogs_app()->pool.sess);
    ogs_pool_init(&rx_sess_state_pool, ogs_app()->pool.sess);

    /* Install objects definitions for this application */
    ret = ogs_diam_gx_init();
    ogs_assert(ret == 0);

    /* Create handler for sessions */
    ret = fd_sess_handler_create(&pcrf_gx_reg, state_cleanup, NULL, NULL);
    ogs_assert(ret == 0);

    memset(&data, 0, sizeof(data));
    data.app = ogs_diam_gx_application;

    ret = fd_disp_register(pcrf_gx_fb_cb, DISP_HOW_APPID, &data, NULL,
                &hdl_gx_fb);
    ogs_assert(ret == 0);

    data.command = ogs_diam_gx_cmd_ccr;
    ret = fd_disp_register(pcrf_gx_ccr_cb, DISP_HOW_CC, &data, NULL,
                &hdl_gx_ccr);
    ogs_assert(ret == 0);

    /* Advertise the support for the application in the peer */
    ret = fd_disp_app_support(ogs_diam_gx_application, ogs_diam_vendor, 1, 0);
    ogs_assert(ret == 0);

    return OGS_OK;
}

void pcrf_gx_final(void)
{
    int ret;

    ret = fd_sess_handler_destroy(&pcrf_gx_reg, NULL);
    ogs_assert(ret == 0);

    if (hdl_gx_fb)
        (void) fd_disp_unregister(&hdl_gx_fb, NULL);
    if (hdl_gx_ccr)
        (void) fd_disp_unregister(&hdl_gx_ccr, NULL);

    ogs_pool_final(&sess_state_pool);
    ogs_pool_final(&rx_sess_state_pool);
    ogs_thread_mutex_destroy(&sess_state_mutex);
}

static int encode_pcc_rule_definition(
        struct avp *avp, ogs_pcc_rule_t *pcc_rule, int flow_presence)
{
    struct avp *avpch1, *avpch2, *avpch3, *avpch4;
    union avp_value val;
    int ret = 0, i;

    ogs_assert(avp);
    ogs_assert(pcc_rule);

    ret = fd_msg_avp_new(ogs_diam_gx_charging_rule_definition, 0, &avpch1);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_new(ogs_diam_gx_charging_rule_name, 0, &avpch2);
    ogs_assert(ret == 0);
    val.os.data = (uint8_t *)pcc_rule->name;
    val.os.len = strlen(pcc_rule->name);
    ret = fd_msg_avp_setvalue(avpch2, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2);
    ogs_assert(ret == 0);

    if (flow_presence == 1) {
        for (i = 0; i < pcc_rule->num_of_flow; i++) {
            ogs_flow_t *flow = &pcc_rule->flow[i];

            ret = fd_msg_avp_new(ogs_diam_gx_flow_information, 0, &avpch2);
            ogs_assert(ret == 0);

            ret = fd_msg_avp_new(ogs_diam_gx_flow_direction, 0, &avpch3);
            ogs_assert(ret == 0);
            val.i32 = flow->direction;
            ret = fd_msg_avp_setvalue(avpch3, &val);
            ogs_assert(ret == 0);
            ret = fd_msg_avp_add(avpch2, MSG_BRW_LAST_CHILD, avpch3);
            ogs_assert(ret == 0);

            ret = fd_msg_avp_new(ogs_diam_gx_flow_description, 0, &avpch3);
            ogs_assert(ret == 0);
            val.os.data = (uint8_t *)flow->description;
            val.os.len = strlen(flow->description);
            ret = fd_msg_avp_setvalue(avpch3, &val);
            ogs_assert(ret == 0);
            ret = fd_msg_avp_add(avpch2, MSG_BRW_LAST_CHILD, avpch3);
            ogs_assert(ret == 0);

            ret = fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2);
            ogs_assert(ret == 0);
        }
    }

    ret = fd_msg_avp_new(ogs_diam_gx_flow_status, 0, &avpch2);
    ogs_assert(ret == 0);
    val.i32 = pcc_rule->flow_status;
    ret = fd_msg_avp_setvalue(avpch2, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2);
    ogs_assert(ret == 0);

    ret = fd_msg_avp_new(ogs_diam_gx_qos_information, 0, &avpch2);
    ogs_assert(ret == 0);

    ret = fd_msg_avp_new(ogs_diam_gx_qos_class_identifier, 0, &avpch3);
    ogs_assert(ret == 0);
    val.u32 = pcc_rule->qos.index;
    ret = fd_msg_avp_setvalue (avpch3, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add (avpch2, MSG_BRW_LAST_CHILD, avpch3);
    ogs_assert(ret == 0);

    ret = fd_msg_avp_new(ogs_diam_gx_allocation_retention_priority, 0, &avpch3);
    ogs_assert(ret == 0);

    ret = fd_msg_avp_new(ogs_diam_gx_priority_level, 0, &avpch4);
    ogs_assert(ret == 0);
    val.u32 = pcc_rule->qos.arp.priority_level;
    ret = fd_msg_avp_setvalue (avpch4, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add (avpch3, MSG_BRW_LAST_CHILD, avpch4);
    ogs_assert(ret == 0);

    ret = fd_msg_avp_new(ogs_diam_gx_pre_emption_capability, 0, &avpch4);
    ogs_assert(ret == 0);
    val.u32 = OGS_EPC_PRE_EMPTION_DISABLED;
    if (pcc_rule->qos.arp.pre_emption_capability == OGS_5GC_PRE_EMPTION_ENABLED)
        val.u32 = OGS_EPC_PRE_EMPTION_ENABLED;
    ret = fd_msg_avp_setvalue (avpch4, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add (avpch3, MSG_BRW_LAST_CHILD, avpch4);
    ogs_assert(ret == 0);

    ret = fd_msg_avp_new(ogs_diam_gx_pre_emption_vulnerability, 0, &avpch4);
    ogs_assert(ret == 0);
    val.u32 = OGS_EPC_PRE_EMPTION_DISABLED;
    if (pcc_rule->qos.arp.pre_emption_vulnerability ==
            OGS_5GC_PRE_EMPTION_ENABLED)
        val.u32 = OGS_EPC_PRE_EMPTION_ENABLED;
    ret = fd_msg_avp_setvalue (avpch4, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add (avpch3, MSG_BRW_LAST_CHILD, avpch4);
    ogs_assert(ret == 0);

    ret = fd_msg_avp_add (avpch2, MSG_BRW_LAST_CHILD, avpch3);
    ogs_assert(ret == 0);

    if (pcc_rule->qos.mbr.uplink) {
        ret = fd_msg_avp_new(
                ogs_diam_gx_max_requested_bandwidth_ul, 0, &avpch3);
        ogs_assert(ret == 0);
        val.u32 = ogs_uint64_to_uint32(pcc_rule->qos.mbr.uplink);
        ret = fd_msg_avp_setvalue (avpch3, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add (avpch2, MSG_BRW_LAST_CHILD, avpch3);
        ogs_assert(ret == 0);
    }

    if (pcc_rule->qos.mbr.downlink) {
        ret = fd_msg_avp_new(
                ogs_diam_gx_max_requested_bandwidth_dl, 0, &avpch3);
        ogs_assert(ret == 0);
        val.u32 = ogs_uint64_to_uint32(pcc_rule->qos.mbr.downlink);
        ret = fd_msg_avp_setvalue (avpch3, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add (avpch2, MSG_BRW_LAST_CHILD, avpch3);
        ogs_assert(ret == 0);
    }

    if (pcc_rule->qos.gbr.uplink) {
        ret = fd_msg_avp_new(ogs_diam_gx_guaranteed_bitrate_ul, 0, &avpch3);
        ogs_assert(ret == 0);
        val.u32 = ogs_uint64_to_uint32(pcc_rule->qos.gbr.uplink);
        ret = fd_msg_avp_setvalue (avpch3, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add (avpch2, MSG_BRW_LAST_CHILD, avpch3);
        ogs_assert(ret == 0);
    }

    if (pcc_rule->qos.gbr.downlink) {
        ret = fd_msg_avp_new(ogs_diam_gx_guaranteed_bitrate_dl, 0, &avpch3);
        ogs_assert(ret == 0);
        val.u32 = ogs_uint64_to_uint32(pcc_rule->qos.gbr.downlink);
        ret = fd_msg_avp_setvalue (avpch3, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add (avpch2, MSG_BRW_LAST_CHILD, avpch3);
        ogs_assert(ret == 0);
    }

    ret = fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2);
    ogs_assert(ret == 0);

    ret = fd_msg_avp_new(ogs_diam_gx_precedence, 0, &avpch2);
    ogs_assert(ret == 0);
    val.u32 = pcc_rule->precedence;
    ret = fd_msg_avp_setvalue (avpch2, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2);
    ogs_assert(ret == 0);

    ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avpch1);
    ogs_assert(ret == 0);

    return OGS_OK;
}
