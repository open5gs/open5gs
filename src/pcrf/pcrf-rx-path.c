/*
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

struct sess_state {
    os0_t   rx_sid;             /* Rx Session-Id */
    os0_t   gx_sid;             /* Gx Session-Id */

    os0_t   peer_host;          /* Peer Host */

#define SESSION_ABORTED         1
    int     state;

    int     abort_cause;
    int     termination_cause;

    struct timespec ts;         /* Time of sending the message */
};

static OGS_POOL(sess_state_pool, struct sess_state);
static ogs_thread_mutex_t sess_state_mutex;

static struct session_handler *pcrf_rx_reg = NULL;
static struct disp_hdl *hdl_rx_fb = NULL;
static struct disp_hdl *hdl_rx_aar = NULL;
static struct disp_hdl *hdl_rx_str = NULL;

static void pcrf_rx_asa_cb(void *data, struct msg **msg);

static __inline__ struct sess_state *new_state(os0_t sid)
{
    struct sess_state *new = NULL;

    ogs_thread_mutex_lock(&sess_state_mutex);
    ogs_pool_alloc(&sess_state_pool, &new);
    if (!new) {
        ogs_error("ogs_pool_alloc() failed");
        ogs_thread_mutex_unlock(&sess_state_mutex);
        return NULL;
    }
    memset(new, 0, sizeof(*new));

    new->rx_sid = (os0_t)ogs_strdup((char *)sid);
    if (!new->rx_sid) {
        ogs_error("ogs_strdup() failed");
        ogs_pool_free(&sess_state_pool, new);
        ogs_thread_mutex_unlock(&sess_state_mutex);
        return NULL;
    }

    ogs_thread_mutex_unlock(&sess_state_mutex);

    return new;
}

static void state_cleanup(struct sess_state *sess_data, os0_t sid, void *opaque)
{
    if (!sess_data) {
        ogs_error("No session state");
        return;
    }

    if (sess_data->rx_sid)
        ogs_free((char *)sess_data->rx_sid);
    if (sess_data->gx_sid)
        ogs_free((char *)sess_data->gx_sid);

    if (sess_data->peer_host)
        ogs_free(sess_data->peer_host);

    ogs_thread_mutex_lock(&sess_state_mutex);
    ogs_pool_free(&sess_state_pool, sess_data);
    ogs_thread_mutex_unlock(&sess_state_mutex);
}

static int pcrf_rx_fb_cb(struct msg **msg, struct avp *avp,
        struct session *sess, void *opaque, enum disp_action *act)
{
    /* This CB should never be called */
    ogs_warn("Unexpected message received!");

    OGS_DIAM_STATS_MTX(
        PCRF_DIAM_PRIV_STATS_INC(rx.rx_unknown);
    )

    return ENOTSUP;
}

static int pcrf_rx_aar_cb(struct msg **msg, struct avp *avp,
        struct session *sess, void *opaque, enum disp_action *act)
{
    int rv;
    int ret;
    struct msg *ans = NULL, *qry = NULL;
    struct avp *avpch1 = NULL, *avpch2 = NULL, *avpch3 = NULL;
    struct avp_hdr *hdr = NULL;
    union avp_value val;
    struct sess_state *sess_data = NULL;
    size_t sidlen;
    ogs_diam_rx_message_t rx_message;
    ogs_media_component_t *media_component = NULL;
    ogs_media_sub_component_t *sub = NULL;
    ogs_flow_t *flow = NULL;
    char buf[OGS_ADDRSTRLEN];
    os0_t gx_sid = NULL;
    uint32_t result_code = OGS_DIAM_RX_DIAMETER_IP_CAN_SESSION_NOT_AVAILABLE;
    int error_occurred = 0;

    ogs_debug("[PCRF] Rx AA-Request");

    /* Validate input parameters */
    if (!msg || !*msg || !sess) {
        ogs_error("Invalid input parameters");
        return EINVAL;
    }

    /* Initialize Message */
    memset(&rx_message, 0, sizeof(ogs_diam_rx_message_t));
    rx_message.cmd_code = OGS_DIAM_RX_CMD_CODE_AA;

    /* Retrieve session state */
    ret = fd_sess_state_retrieve(pcrf_rx_reg, sess, &sess_data);
    if (ret != 0) {
        ogs_error("Failed to retrieve session state");
        error_occurred = 1;
        goto out;
    }

    if (!sess_data) {
        os0_t sid = NULL;
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
    val.i32 = OGS_DIAM_RX_APPLICATION_ID;
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

    /* Set the Auth-Request-Type AVP */
    ret = fd_msg_avp_new(ogs_diam_auth_request_type, 0, &avp);
    if (ret != 0) {
        ogs_error("Failed to create Auth-Request-Type AVP");
        error_occurred = 1;
        goto out;
    }
    val.i32 = OGS_DIAM_AUTH_REQUEST_TYPE_AUTHENTICATE_ONLY;
    ret = fd_msg_avp_setvalue(avp, &val);
    if (ret != 0) {
        ogs_error("Failed to set Auth-Request-Type value");
        error_occurred = 1;
        goto out;
    }
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    if (ret != 0) {
        ogs_error("Failed to add Auth-Request-Type AVP");
        error_occurred = 1;
        goto out;
    }

    /* Get Framed-IP-Address */
    ret = fd_msg_search_avp(qry, ogs_diam_rx_framed_ip_address, &avp);
    if (ret == 0 && avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        if (ret == 0 && hdr) {
            gx_sid = (os0_t)pcrf_sess_find_by_ipv4(hdr->avp_value->os.data);
            if (!gx_sid) {
                ogs_warn("Cannot find Gx Session for IPv4:%s",
                        OGS_INET_NTOP(hdr->avp_value->os.data, buf));
            }
        }
    }

    if (!gx_sid) {
        /* Get Framed-IPv6-Prefix */
        ret = fd_msg_search_avp(qry, ogs_diam_rx_framed_ipv6_prefix, &avp);
        if (ret == 0 && avp) {
            ogs_paa_t *paa = NULL;

            ret = fd_msg_avp_hdr(avp, &hdr);
            if (ret == 0 && hdr) {
                paa = (ogs_paa_t *)hdr->avp_value->os.data;
                if (paa && paa->len == OGS_IPV6_LEN * 8) {
                    gx_sid = (os0_t)pcrf_sess_find_by_ipv6(paa->addr6);
                    if (!gx_sid) {
                        ogs_warn("Cannot find Gx Session for IPv6:%s",
                                OGS_INET6_NTOP(hdr->avp_value->os.data, buf));
                    }
                }
            }
        }
    }

    if (!gx_sid) {
        ogs_error("No Gx Session found");
        result_code = OGS_DIAM_RX_DIAMETER_IP_CAN_SESSION_NOT_AVAILABLE;
        error_occurred = 1;
        goto out;
    }

    /* Parse AVPs - with error checking for each browse operation */
    ret = fd_msg_browse(qry, MSG_BRW_FIRST_CHILD, &avpch1, NULL);
    if (ret != 0) {
        ogs_error("Failed to browse message");
        error_occurred = 1;
        goto out;
    }

    while (avpch1) {
        ret = fd_msg_avp_hdr(avpch1, &hdr);
        if (ret != 0) {
            ogs_error("Failed to get AVP header");
            fd_msg_browse(avpch1, MSG_BRW_NEXT, &avpch1, NULL);
            continue;
        }

        switch(hdr->avp_code) {
        case AC_SESSION_ID:
        case AC_ORIGIN_HOST:
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
            break;

        case OGS_DIAM_RX_AVP_CODE_MEDIA_COMPONENT_DESCRIPTION:
            if (rx_message.ims_data.num_of_media_component >=
                    OGS_ARRAY_SIZE(rx_message.ims_data.media_component)) {
                ogs_error("OVERFLOW rx_message.ims_data.num_of_media_component"
                        " [%d:%d]", rx_message.ims_data.num_of_media_component,
                        (int)OGS_ARRAY_SIZE(
                            rx_message.ims_data.media_component));
                break;
            }

            media_component = &rx_message.ims_data.
                    media_component[rx_message.ims_data.num_of_media_component];

            ret = fd_msg_browse(avpch1, MSG_BRW_FIRST_CHILD, &avpch2, NULL);
            if (ret != 0) {
                ogs_error("Failed to browse media component");
                break;
            }

            while (avpch2) {
                ret = fd_msg_avp_hdr(avpch2, &hdr);
                if (ret != 0) {
                    fd_msg_browse(avpch2, MSG_BRW_NEXT, &avpch2, NULL);
                    continue;
                }

                switch (hdr->avp_code) {
                case OGS_DIAM_RX_AVP_CODE_MEDIA_COMPONENT_NUMBER:
                    media_component->media_component_number =
                        hdr->avp_value->i32;
                    break;
                case OGS_DIAM_RX_AVP_CODE_MEDIA_TYPE:
                    media_component->media_type = hdr->avp_value->i32;
                    break;
                case OGS_DIAM_RX_AVP_CODE_MAX_REQUESTED_BANDWIDTH_DL:
                    media_component->max_requested_bandwidth_dl =
                        hdr->avp_value->i32;
                    break;
                case OGS_DIAM_RX_AVP_CODE_MAX_REQUESTED_BANDWIDTH_UL:
                    media_component->max_requested_bandwidth_ul =
                        hdr->avp_value->i32;
                    break;
                case OGS_DIAM_RX_AVP_CODE_RR_BANDWIDTH:
                    media_component->rr_bandwidth = hdr->avp_value->i32;
                    break;
                case OGS_DIAM_RX_AVP_CODE_RS_BANDWIDTH:
                    media_component->rs_bandwidth = hdr->avp_value->i32;
                    break;
                case OGS_DIAM_RX_AVP_CODE_MIN_REQUESTED_BANDWIDTH_DL:
                    media_component->min_requested_bandwidth_dl =
                        hdr->avp_value->i32;
                    break;
                case OGS_DIAM_RX_AVP_CODE_MIN_REQUESTED_BANDWIDTH_UL:
                    media_component->min_requested_bandwidth_ul =
                        hdr->avp_value->i32;
                    break;
                case OGS_DIAM_RX_AVP_CODE_FLOW_STATUS:
                    media_component->flow_status = hdr->avp_value->i32;
                    break;

                case OGS_DIAM_RX_AVP_CODE_MEDIA_SUB_COMPONENT:
                    if (media_component->num_of_sub >=
                            OGS_ARRAY_SIZE(media_component->sub)) {
                        ogs_error("OVERFLOW media_component->num_of_sub [%d:%d]",
                                media_component->num_of_sub,
                                (int)OGS_ARRAY_SIZE(media_component->sub));
                        break;
                    }

                    sub = &media_component->sub[media_component->num_of_sub];

                    ret = fd_msg_browse(avpch2, MSG_BRW_FIRST_CHILD,
                                        &avpch3, NULL);
                    if (ret != 0) {
                        ogs_error("Failed to browse sub component");
                        break;
                    }

                    while (avpch3) {
                        ret = fd_msg_avp_hdr(avpch3, &hdr);
                        if (ret != 0) {
                            fd_msg_browse(avpch3, MSG_BRW_NEXT, &avpch3, NULL);
                            continue;
                        }

                        switch (hdr->avp_code) {
                        case OGS_DIAM_RX_AVP_CODE_FLOW_NUMBER:
                            sub->flow_number = hdr->avp_value->i32;
                            break;
                        case OGS_DIAM_RX_AVP_CODE_FLOW_USAGE:
                            switch (hdr->avp_value->i32) {
                            case OGS_DIAM_RX_FLOW_USAGE_NO_INFORMATION:
                                sub->flow_usage = OGS_FLOW_USAGE_NO_INFO;
                                break;
                            case OGS_DIAM_RX_FLOW_USAGE_RTCP:
                                sub->flow_usage = OGS_FLOW_USAGE_RTCP;
                                break;
                            case OGS_DIAM_RX_FLOW_USAGE_AF_SIGNALLING:
                                sub->flow_usage = OGS_FLOW_USAGE_AF_SIGNALLING;
                                break;
                            default:
                                ogs_error("Invalid flow usage = %d",
                                    hdr->avp_value->i32);
                            }
                            break;
                        case OGS_DIAM_RX_AVP_CODE_FLOW_DESCRIPTION:
                            if (sub->num_of_flow >=
                                    OGS_ARRAY_SIZE(sub->flow)) {
                                ogs_error("OVERFLOW sub->num_of_flow [%d:%d]",
                                    sub->num_of_flow,
                                    (int)OGS_ARRAY_SIZE(sub->flow));
                                break;
                            }

                            flow = &sub->flow[sub->num_of_flow];
                            flow->description = ogs_strndup(
                                    (char*)hdr->avp_value->os.data,
                                    hdr->avp_value->os.len);
                            if (!flow->description) {
                                ogs_error("Failed to duplicate flow description");
                                break;
                            }
                            sub->num_of_flow++;
                            break;
                        default:
                            ogs_warn("Not supported AVP code(%d)",
                                    hdr->avp_code);
                            break;
                        }
                        fd_msg_browse(avpch3, MSG_BRW_NEXT, &avpch3, NULL);
                    }

                    media_component->num_of_sub++;
                    break;
                default:
                    ogs_warn("Not supported AVP code(%d)", hdr->avp_code);
                    break;
                }

                fd_msg_browse(avpch2, MSG_BRW_NEXT, &avpch2, NULL);
            }

            rx_message.ims_data.num_of_media_component++;
            break;

        case AC_ORIGIN_REALM:
        case AC_DESTINATION_REALM:
        case AC_ROUTE_RECORD:
        case AC_PROXY_INFO:
        case AC_AUTH_APPLICATION_ID:
        case OGS_DIAM_AVP_CODE_FRAME_IP_ADDRESS:
        case OGS_DIAM_AVP_CODE_FRAME_IPV6_PREFIX:
        case OGS_DIAM_RX_AVP_CODE_SUBSCRIPTION_ID:
        case OGS_DIAM_RX_AVP_CODE_SPECIFIC_ACTION:
            break;
        default:
            ogs_warn("Not supported AVP code(%d)", hdr->avp_code);
            break;
        }
        fd_msg_browse(avpch1, MSG_BRW_NEXT, &avpch1, NULL);
    }

    if (!error_occurred) {
        /* Send Re-Auth Request */
        rv = pcrf_gx_send_rar(gx_sid, sess_data->rx_sid, &rx_message);
        if (rv != OGS_OK) {
            result_code = rx_message.result_code;
            if (result_code != ER_DIAMETER_SUCCESS) {
                ogs_error("pcrf_gx_send_rar() failed");
                error_occurred = 1;
                goto out;
            }
        }

        /* Store Gx Session-Id in this session */
        if (!sess_data->gx_sid) {
            sess_data->gx_sid = (os0_t)ogs_strdup((char *)gx_sid);
            if (!sess_data->gx_sid) {
                ogs_error("Failed to duplicate gx_sid");
                error_occurred = 1;
                goto out;
            }
        }

        /* Set IP-Can-Type */
        ret = fd_msg_avp_new(ogs_diam_rx_ip_can_type, 0, &avp);
        if (ret == 0) {
            val.i32 = OGS_DIAM_RX_IP_CAN_TYPE_3GPP_EPS;
            ret = fd_msg_avp_setvalue(avp, &val);
            if (ret == 0) {
                ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
            }
        }
        if (ret != 0) {
            ogs_error("Failed to add IP-Can-Type AVP");
        }

        /* Set RAT-Type */
        ret = fd_msg_avp_new(ogs_diam_rat_type, 0, &avp);
        if (ret == 0) {
            val.i32 = OGS_DIAM_RAT_TYPE_EUTRAN;
            ret = fd_msg_avp_setvalue(avp, &val);
            if (ret == 0) {
                ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
            }
        }
        if (ret != 0) {
            ogs_error("Failed to add RAT-Type AVP");
        }

        /* Set success result code */
        ret = fd_msg_rescode_set(ans, (char *)"DIAMETER_SUCCESS", NULL, NULL, 1);
        if (ret != 0) {
            ogs_error("Failed to set success result code");
            error_occurred = 1;
            goto out;
        }

        /* Store session state */
        ret = fd_sess_state_store(pcrf_rx_reg, sess, &sess_data);
        if (ret != 0) {
            ogs_error("Failed to store session state");
            error_occurred = 1;
            goto out;
        }
        sess_data = NULL; /* Ownership transferred */

        /* Send the answer */
        ret = fd_msg_send(msg, NULL, NULL);
        if (ret != 0) {
            ogs_error("Failed to send message");
            error_occurred = 1;
            goto out;
        }

        ogs_debug("[PCRF] Tx AA-Answer");

        /* Add to stats */
        OGS_DIAM_STATS_MTX(
            OGS_DIAM_STATS_INC(nb_echoed);
            PCRF_DIAM_PRIV_STATS_INC(rx.rx_aar);
            PCRF_DIAM_PRIV_STATS_INC(rx.tx_aaa);
        )

        ogs_ims_data_free(&rx_message.ims_data);
        return 0;
    }

out:
    /* Error handling */
    if (ans) {
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

        ret = fd_msg_send(msg, NULL, NULL);
        if (ret != 0) {
            ogs_error("Failed to send error response");
        }
    }

    /* Update error stats */
    OGS_DIAM_STATS_MTX(
        PCRF_DIAM_PRIV_STATS_INC(rx.rx_aar);
        PCRF_DIAM_PRIV_STATS_INC(rx.rx_aar_error);
    )

    /* Cleanup session state if it wasn't stored */
    if (sess_data) {
        state_cleanup(sess_data, NULL, NULL);
    }

    /* Always free IMS data */
    ogs_ims_data_free(&rx_message.ims_data);

    return 0;
}

int pcrf_rx_send_asr(uint8_t *rx_sid, uint32_t abort_cause)
{
    int ret;

    struct msg *req = NULL;
    struct avp *avp;
    union avp_value val;
    struct sess_state *sess_data = NULL, *svg;
    struct session *session = NULL;
    int new;
    size_t sidlen;

    ogs_assert(rx_sid);

    ogs_debug("[PCRF] Tx Abort-Session-Request");

    /* Retrieve session by Session-Id */
    sidlen = strlen((char *)rx_sid);
    ret = fd_sess_fromsid_msg((os0_t)(rx_sid), sidlen, &session, &new);
    ogs_assert(ret == 0);
    if (new) {
        ogs_error("Cannot find Rx Session [ID:%s Cause:%d]",
                    rx_sid, abort_cause);
        return OGS_ERROR;
    }

    /* Create the request */
    ret = fd_msg_new(ogs_diam_rx_cmd_asr, MSGFL_ALLOC_ETEID, &req);
    ogs_assert(ret == 0);
    {
        struct msg_hdr * h;
        ret = fd_msg_hdr( req, &h );
        ogs_assert(ret == 0);
        h->msg_appl = OGS_DIAM_RX_APPLICATION_ID;
    }

    /* Add Session-Id to the message */
    ret = ogs_diam_message_session_id_set(req, (os0_t)(rx_sid), sidlen);
    ogs_assert(ret == 0);

    /* Save the session associated with the message */
    ret = fd_msg_sess_set(req, session);

    /* Retrieve session state in this session */
    ret = fd_sess_state_retrieve(pcrf_rx_reg, session, &sess_data);
    ogs_assert(ret == 0);
    if (!sess_data) {
        ogs_error("No Session Data");
        ret = fd_msg_free(req);
        ogs_assert(ret == 0);
        return OGS_ERROR;
    }

    /* Update State */
    sess_data->state = SESSION_ABORTED;
    sess_data->abort_cause = abort_cause;

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
    val.i32 = OGS_DIAM_RX_APPLICATION_ID;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Set the Abort-Cause AVP */
    ret = fd_msg_avp_new(ogs_diam_rx_abort_cause, 0, &avp);
    ogs_assert(ret == 0);
    val.i32 = sess_data->abort_cause;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Keep a pointer to the session data for debug purpose,
     * in real life we would not need it */
    svg = sess_data;

    /* Store this value in the session */
    ret = fd_sess_state_store(pcrf_rx_reg, session, &sess_data);
    ogs_assert(ret == 0);
    ogs_assert(sess_data == NULL);

    /* Send the request */
    ret = fd_msg_send(&req, pcrf_rx_asa_cb, svg);
    ogs_assert(ret == 0);

    /* Increment the counter */
    OGS_DIAM_STATS_MTX(
        OGS_DIAM_STATS_INC(nb_sent);
        PCRF_DIAM_PRIV_STATS_INC(rx.tx_sar);
    )

    return OGS_OK;
}

static void pcrf_rx_asa_cb(void *data, struct msg **msg)
{
    int ret;
    struct session *session = NULL;
    struct avp *avp = NULL, *avpch1 = NULL;
    struct avp_hdr *hdr = NULL;
    int new = 0;
    int result_code = 0;

    ogs_debug("[PCRF] Rx Abort-Session-Answer");

    /* Validate input parameters */
    if (!msg || !*msg) {
        ogs_error("Invalid message pointer");
        return;
    }

    /* Search the session, retrieve its data */
    ret = fd_msg_sess_get(fd_g_config->cnf_dict, *msg, &session, &new);
    if (ret != 0) {
        ogs_error("Failed to get session from message");
        goto cleanup;
    }

    if (new != 0) {
        ogs_error("Session should already exist");
        goto cleanup;
    }

    /* Value of Result Code */
    ret = fd_msg_search_avp(*msg, ogs_diam_result_code, &avp);
    if (ret != 0) {
        ogs_error("Failed to search Result-Code AVP");
        goto cleanup;
    }

    if (avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        if (ret != 0) {
            ogs_error("Failed to get Result-Code AVP header");
            goto cleanup;
        }
        result_code = hdr->avp_value->i32;
        ogs_debug("    Result Code: %d", result_code);
    } else {
        /* Try experimental result code */
        ret = fd_msg_search_avp(*msg, ogs_diam_experimental_result, &avp);
        if (ret != 0) {
            ogs_error("Failed to search Experimental-Result AVP");
            goto cleanup;
        }

        if (avp) {
            ret = fd_avp_search_avp(avp, ogs_diam_experimental_result_code,
                                    &avpch1);
            if (ret != 0) {
                ogs_error("Failed to search Experimental-Result-Code AVP");
                goto cleanup;
            }

            if (avpch1) {
                ret = fd_msg_avp_hdr(avpch1, &hdr);
                if (ret != 0) {
                    ogs_error("Failed to get Experimental-Result-Code header");
                    goto cleanup;
                }
                result_code = hdr->avp_value->i32;
                ogs_debug("    Experimental Result Code: %d", result_code);
            }
        } else {
            ogs_error("No Result-Code or Experimental-Result found");
            result_code = -1; /* Set error indicator */
        }
    }

    /* Value of Origin-Host */
    ret = fd_msg_search_avp(*msg, ogs_diam_origin_host, &avp);
    if (ret == 0 && avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        if (ret == 0 && hdr) {
            ogs_debug("    From '%.*s'",
                    (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
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
        }
    } else {
        ogs_warn("No Origin-Realm found");
    }

    if (result_code != ER_DIAMETER_SUCCESS) {
        ogs_error("ERROR DIAMETER Result Code(%d)", result_code);
    }

    /* Update statistics */
    OGS_DIAM_STATS_MTX(
        PCRF_DIAM_PRIV_STATS_INC(rx.rx_asa);
    )

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

static int pcrf_rx_str_cb(struct msg **msg, struct avp *avp,
        struct session *sess, void *opaque, enum disp_action *act)
{
    int rv;
    int ret;
    struct msg *ans = NULL, *qry = NULL;
    struct avp_hdr *hdr = NULL;
    union avp_value val;
    struct sess_state *sess_data = NULL;
    ogs_diam_rx_message_t rx_message;
    uint32_t result_code = OGS_DIAM_RX_DIAMETER_IP_CAN_SESSION_NOT_AVAILABLE;
    int error_occurred = 0;

    ogs_debug("[PCRF] Rx Session-Termination-Request");

    /* Validate input parameters */
    if (!msg || !*msg || !sess) {
        ogs_error("Invalid input parameters");
        return EINVAL;
    }

    /* Initialize Message */
    memset(&rx_message, 0, sizeof(ogs_diam_rx_message_t));
    rx_message.cmd_code = OGS_DIAM_RX_CMD_CODE_SESSION_TERMINATION;

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
    val.i32 = OGS_DIAM_RX_APPLICATION_ID;
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

    /* Set the Auth-Request-Type AVP */
    ret = fd_msg_avp_new(ogs_diam_auth_request_type, 0, &avp);
    if (ret != 0) {
        ogs_error("Failed to create Auth-Request-Type AVP");
        error_occurred = 1;
        goto out;
    }
    val.i32 = OGS_DIAM_AUTH_REQUEST_TYPE_AUTHENTICATE_ONLY;
    ret = fd_msg_avp_setvalue(avp, &val);
    if (ret != 0) {
        ogs_error("Failed to set Auth-Request-Type value");
        error_occurred = 1;
        goto out;
    }
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    if (ret != 0) {
        ogs_error("Failed to add Auth-Request-Type AVP");
        error_occurred = 1;
        goto out;
    }

    /* Retrieve session state */
    ret = fd_sess_state_retrieve(pcrf_rx_reg, sess, &sess_data);
    if (ret != 0) {
        ogs_error("Failed to retrieve session state");
        error_occurred = 1;
        goto out;
    }

    if (!sess_data) {
        ogs_error("Cannot find session in Session-Termination-Request");
        error_occurred = 1;
        goto out;
    }

    if (!sess_data->rx_sid) {
        ogs_error("No Rx Session-Id in session data");
        error_occurred = 1;
        goto out;
    }

    if (!sess_data->gx_sid) {
        ogs_error("No Gx Session-Id in session data");
        error_occurred = 1;
        goto out;
    }

    /* Get Termination-Cause */
    ret = fd_msg_search_avp(qry, ogs_diam_termination_cause, &avp);
    if (ret == 0 && avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        if (ret == 0 && hdr) {
            sess_data->termination_cause = hdr->avp_value->i32;
            switch (sess_data->termination_cause) {
            case OGS_DIAM_TERMINATION_CAUSE_DIAMETER_LOGOUT:
                break;
            default:
                ogs_error("Termination-Cause Error : [%d]",
                        sess_data->termination_cause);
                break;
            }
        } else {
            ogs_error("Failed to get Termination-Cause AVP header");
        }
    } else {
        ogs_warn("No Termination-Cause found");
    }

    if (sess_data->state != SESSION_ABORTED) {
        /* Send Re-Auth Request if Abort-Session-Request is not initiated */
        rv = pcrf_gx_send_rar(
                sess_data->gx_sid, sess_data->rx_sid, &rx_message);
        if (rv != OGS_OK) {
            result_code = rx_message.result_code;
            if (result_code != ER_DIAMETER_SUCCESS) {
                ogs_error("pcrf_gx_send_rar() failed");
                error_occurred = 1;
                goto out;
            }
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

        /* Send the answer */
        ret = fd_msg_send(msg, NULL, NULL);
        if (ret != 0) {
            ogs_error("Failed to send message");
            error_occurred = 1;
            goto out;
        }

        ogs_debug("[PCRF] Tx Session-Termination-Answer");

        /* Add to stats */
        OGS_DIAM_STATS_MTX(
            OGS_DIAM_STATS_INC(nb_echoed);
            PCRF_DIAM_PRIV_STATS_INC(rx.rx_str);
            PCRF_DIAM_PRIV_STATS_INC(rx.tx_sta);
        )

        /* Clean up session state */
        state_cleanup(sess_data, NULL, NULL);
        ogs_ims_data_free(&rx_message.ims_data);

        return 0;
    }

out:
    /* Error handling */
    if (ans) {
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
            ret = fd_msg_rescode_set(ans,
                        (char *)"DIAMETER_UNABLE_TO_COMPLY", NULL, NULL, 1);
        }

        if (ret != 0) {
            ogs_error("Failed to set error result code");
        }

        ret = fd_msg_send(msg, NULL, NULL);
        if (ret != 0) {
            ogs_error("Failed to send error response");
        }

        ogs_debug("[PCRF] Tx Session-Termination-Answer");
    }

    /* Update error stats */
    OGS_DIAM_STATS_MTX(
        PCRF_DIAM_PRIV_STATS_INC(rx.rx_str);
        PCRF_DIAM_PRIV_STATS_INC(rx.rx_str_error);
        PCRF_DIAM_PRIV_STATS_INC(rx.tx_sta);
    )

    /* Clean up session state */
    if (sess_data) {
        state_cleanup(sess_data, NULL, NULL);
    }

    /* Always free IMS data */
    ogs_ims_data_free(&rx_message.ims_data);

    return 0;
}

int pcrf_rx_init(void)
{
    int ret;
    struct disp_when data;

    ogs_thread_mutex_init(&sess_state_mutex);
    ogs_pool_init(&sess_state_pool, ogs_app()->pool.sess);

    /* Install objects definitions for this application */
    ret = ogs_diam_rx_init();
    ogs_assert(ret == 0);

    /* Create handler for sessions */
    ret = fd_sess_handler_create(&pcrf_rx_reg, state_cleanup, NULL, NULL);
    ogs_assert(ret == 0);

    /* Fallback CB if command != unexpected message received */
    memset(&data, 0, sizeof(data));
    data.app = ogs_diam_rx_application;

    ret = fd_disp_register(pcrf_rx_fb_cb, DISP_HOW_APPID, &data, NULL,
                &hdl_rx_fb);
    ogs_assert(ret == 0);

    /* Specific handler for AA-Request */
    data.command = ogs_diam_rx_cmd_aar;
    ret = fd_disp_register(pcrf_rx_aar_cb, DISP_HOW_CC, &data, NULL,
                &hdl_rx_aar);
    ogs_assert(ret == 0);

    /* Specific handler for STR-Request */
    data.command = ogs_diam_rx_cmd_str;
    ret = fd_disp_register(pcrf_rx_str_cb, DISP_HOW_CC, &data, NULL,
                &hdl_rx_str);
    ogs_assert(ret == 0);

    /* Advertise the support for the application in the peer */
    ret = fd_disp_app_support(ogs_diam_rx_application, ogs_diam_vendor, 1, 0);
    ogs_assert(ret == 0);

    return OGS_OK;
}

void pcrf_rx_final(void)
{
    int ret;

    ret = fd_sess_handler_destroy(&pcrf_rx_reg, NULL);
    ogs_assert(ret == 0);

    if (hdl_rx_fb)
        (void) fd_disp_unregister(&hdl_rx_fb, NULL);
    if (hdl_rx_aar)
        (void) fd_disp_unregister(&hdl_rx_aar, NULL);
    if (hdl_rx_str)
        (void) fd_disp_unregister(&hdl_rx_str, NULL);

    ogs_pool_final(&sess_state_pool);
    ogs_thread_mutex_destroy(&sess_state_mutex);
}
