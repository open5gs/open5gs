/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
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

#include "fd-path.h"

static struct session_handler *smf_s6b_reg = NULL;
static struct disp_hdl *hdl_s6b_fb = NULL; 

struct sess_state {
    smf_sess_t *sess;
    os0_t       s6b_sid;             /* S6B Session-Id */

    ogs_gtp_xact_t *xact;

    struct timespec ts; /* Time of sending the message */
};

static OGS_POOL(sess_state_pool, struct sess_state);
static ogs_thread_mutex_t sess_state_mutex;

static void smf_s6b_aaa_cb(void *data, struct msg **msg);
static void smf_s6b_sta_cb(void *data, struct msg **msg);

static __inline__ struct sess_state *new_state(os0_t sid)
{
    struct sess_state *new = NULL;

    ogs_thread_mutex_lock(&sess_state_mutex);
    ogs_pool_alloc(&sess_state_pool, &new);
    ogs_expect_or_return_val(new, NULL);
    ogs_thread_mutex_unlock(&sess_state_mutex);

    new->s6b_sid = (os0_t)ogs_strdup((char *)sid);
    ogs_expect_or_return_val(new->s6b_sid, NULL);

    return new;
}

static void state_cleanup(struct sess_state *sess_data, os0_t sid, void *opaque)
{
    if (sess_data->s6b_sid)
        ogs_free(sess_data->s6b_sid);

    ogs_thread_mutex_lock(&sess_state_mutex);
    ogs_pool_free(&sess_state_pool, sess_data);
    ogs_thread_mutex_unlock(&sess_state_mutex);
}

static int smf_s6b_fb_cb(struct msg **msg, struct avp *avp, 
        struct session *sess, void *opaque, enum disp_action *act)
{
	/* This CB should never be called */
	ogs_warn("Unexpected message received!");
	
	return ENOTSUP;
}

void smf_s6b_send_aar(smf_sess_t *sess, ogs_gtp_xact_t *xact)
{
    int ret;

    struct msg *req = NULL;
    struct avp *avp;
    union avp_value val;
    struct sess_state *sess_data = NULL, *svg;
    struct session *session = NULL;
    int new;

    smf_ue_t *smf_ue = NULL;
    char *user_name = NULL;
    char *visited_network_identifier = NULL;

    struct avp *mip6_agent_info, *mip_home_agent_address;
    struct sockaddr_in sin;
    struct sockaddr_in6 sin6;

    ogs_assert(xact);
    ogs_assert(sess);
    smf_ue = sess->smf_ue;
    ogs_assert(smf_ue);

    ogs_debug("[AA-Request]");

    /* Create the request */
    ret = fd_msg_new(ogs_diam_rx_cmd_aar, MSGFL_ALLOC_ETEID, &req);
    ogs_assert(ret == 0);
    {
        struct msg_hdr * h;
        ret = fd_msg_hdr( req, &h );
        ogs_assert(ret == 0);
        h->msg_appl = OGS_DIAM_S6B_APPLICATION_ID;
    }

    /* Find Diameter S6b Session */
    if (sess->s6b_sid) {
        /* Retrieve session by Session-Id */
        size_t sidlen = strlen(sess->s6b_sid);
		ret = fd_sess_fromsid_msg((os0_t)sess->s6b_sid, sidlen, &session, &new);
        ogs_assert(ret == 0);
        ogs_assert(new == 0);

        ogs_debug("    Found S6b Session-Id: [%s]", sess->s6b_sid);

        /* Add Session-Id to the message */
        ret = ogs_diam_message_session_id_set(
                req, (os0_t)sess->s6b_sid, sidlen);
        ogs_assert(ret == 0);
        /* Save the session associated with the message */
        ret = fd_msg_sess_set(req, session);
    } else {
        /* Create a new session */
        #define OGS_DIAM_S6B_APP_SID_OPT  "app_s6b"
        ret = fd_msg_new_session(req, (os0_t)OGS_DIAM_S6B_APP_SID_OPT,
                CONSTSTRLEN(OGS_DIAM_S6B_APP_SID_OPT));
        ogs_assert(ret == 0);
        ret = fd_msg_sess_get(fd_g_config->cnf_dict, req, &session, NULL);
        ogs_assert(ret == 0);
    }

    /* Retrieve session state in this session */
    ret = fd_sess_state_retrieve(smf_s6b_reg, session, &sess_data);
    if (!sess_data) {
        os0_t sid;
        size_t sidlen;

        ret = fd_sess_getsid(session, &sid, &sidlen);
        ogs_assert(ret == 0);

        /* Allocate new session state memory */
        sess_data = new_state(sid);
        ogs_assert(sess_data);

        ogs_debug("    Allocate new session: [%s]", sess_data->s6b_sid);

        /* Save Session-Id to SMF Session Context */
        sess->s6b_sid = (char *)sess_data->s6b_sid;
    } else
        ogs_debug("    Retrieve session: [%s]", sess_data->s6b_sid);

    /* Update session state */
    sess_data->sess = sess;
    sess_data->xact = xact;

    /* Set Origin-Host & Origin-Realm */
    ret = fd_msg_add_origin(req, 0);
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
    val.i32 = OGS_DIAM_S6B_APPLICATION_ID;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Set the Auth-Request-Type AVP */
    ret = fd_msg_avp_new(ogs_diam_auth_request_type, 0, &avp);
    ogs_assert(ret == 0);
    val.i32 = OGS_DIAM_AUTH_REQUEST_TYPE_AUTHORIZE_ONLY;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Set RAT-Type */
    ret = fd_msg_avp_new(ogs_diam_rat_type, 0, &avp);
    ogs_assert(ret == 0);

    switch (sess->gtp_rat_type) {
    case OGS_GTP2_RAT_TYPE_EUTRAN:
        val.i32 = OGS_DIAM_RAT_TYPE_EUTRAN;
        break;
    case OGS_GTP2_RAT_TYPE_WLAN:
        val.i32 = OGS_DIAM_RAT_TYPE_WLAN;
        break;
    default:
        ogs_error("Unknown RAT Type [%d]", sess->gtp_rat_type);
        ogs_assert_if_reached();
    }

    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Set the User-Name AVP */
    user_name = ogs_msprintf("%s@nai.epc.mnc%03d.mcc%03d.3gppnetwork.org",
                    smf_ue->imsi_bcd,
                    ogs_plmn_id_mnc(&sess->plmn_id),
                    ogs_plmn_id_mcc(&sess->plmn_id));
    ogs_assert(user_name);

    ret = fd_msg_avp_new(ogs_diam_user_name, 0, &avp);
    ogs_assert(ret == 0);
    val.os.data = (uint8_t *)user_name;
    val.os.len = strlen(user_name);
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Set MIP6-Feature-Vector */
    ret = fd_msg_avp_new(ogs_diam_s6b_mip6_feature_vector, 0, &avp);
    ogs_assert(ret == 0);
    val.u64 = 0x0000400000000000LL; /* GTPv2_SUPPORTED: Set */
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Set MIP6-Agent-Info */
    ret = fd_msg_avp_new(ogs_diam_mip6_agent_info, 0, &mip6_agent_info);
    ogs_assert(ret == 0);

    if (ogs_gtp_self()->gtpc_addr) {
        ret = fd_msg_avp_new(ogs_diam_mip_home_agent_address, 0,
                    &mip_home_agent_address);
        ogs_assert(ret == 0);
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr =
            ogs_gtp_self()->gtpc_addr->sin.sin_addr.s_addr;
        ret = fd_msg_avp_value_encode (
                    &sin, mip_home_agent_address );
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(mip6_agent_info,
                MSG_BRW_LAST_CHILD, mip_home_agent_address);
        ogs_assert(ret == 0);
    }

    if (ogs_gtp_self()->gtpc_addr6) {
        ret = fd_msg_avp_new(ogs_diam_mip_home_agent_address, 0,
                    &mip_home_agent_address);
        ogs_assert(ret == 0);
        sin6.sin6_family = AF_INET6;
        memcpy(sin6.sin6_addr.s6_addr,
                ogs_gtp_self()->gtpc_addr6->sin6.sin6_addr.s6_addr,
                OGS_IPV6_LEN);
        ret = fd_msg_avp_value_encode (
                    &sin6, mip_home_agent_address );
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(mip6_agent_info,
                MSG_BRW_LAST_CHILD, mip_home_agent_address);
        ogs_assert(ret == 0);
    }

    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, mip6_agent_info);
    ogs_assert(ret == 0);

    /* Set the Visited-Network-Identifier AVP */
    visited_network_identifier =
                ogs_msprintf("mnc%03d.mcc%03d.3gppnetwork.org",
                    ogs_plmn_id_mnc(&sess->plmn_id),
                    ogs_plmn_id_mcc(&sess->plmn_id));
    ogs_assert(visited_network_identifier);

    ret = fd_msg_avp_new(ogs_diam_visited_network_identifier, 0, &avp);
    ogs_assert(ret == 0);
    val.os.data = (unsigned char *)(visited_network_identifier);
    val.os.len  = strlen(visited_network_identifier);
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Set Service-Selection */
    ret = fd_msg_avp_new(ogs_diam_service_selection, 0, &avp);
    ogs_assert(ret == 0);
    ogs_assert(sess->session.name);
    val.os.data = (uint8_t*)sess->session.name;
    val.os.len = strlen(sess->session.name);
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Keep a pointer to the session data for debug purpose,
     * in real life we would not need it */
    svg = sess_data;

    /* Store this value in the session */
    ret = fd_sess_state_store(smf_s6b_reg, session, &sess_data);
    ogs_assert(ret == 0);
    ogs_assert(sess_data == NULL);

    /* Send the request */
    ret = fd_msg_send(&req, smf_s6b_aaa_cb, svg);
    ogs_assert(ret == 0);

    /* Increment the counter */
    ogs_assert(pthread_mutex_lock(&ogs_diam_logger_self()->stats_lock) == 0);
    ogs_diam_logger_self()->stats.nb_sent++;
    ogs_assert(pthread_mutex_unlock(&ogs_diam_logger_self()->stats_lock) == 0);

    ogs_free(user_name);
    ogs_free(visited_network_identifier);
}

static void smf_s6b_aaa_cb(void *data, struct msg **msg)
{
    int ret;

    struct sess_state *sess_data = NULL;
    struct timespec ts;
    struct session *session;
    struct avp *avp, *avpch1;
    struct avp_hdr *hdr;
    unsigned long dur;
    int error = 0;
    int new;
    int result_code = 0;
    int exp_result_code = 0;

    smf_sess_t *sess = NULL;
    ogs_gtp_xact_t *xact = NULL;

    ogs_debug("[AA-Answer]");

    ret = clock_gettime(CLOCK_REALTIME, &ts);
    ogs_assert(ret == 0);

    /* Search the session, retrieve its data */
    ret = fd_msg_sess_get(fd_g_config->cnf_dict, *msg, &session, &new);
    ogs_assert(ret == 0);
    ogs_assert(new == 0);

    ogs_debug("    Search the session");

    ret = fd_sess_state_retrieve(smf_s6b_reg, session, &sess_data);
    ogs_assert(ret == 0);
    ogs_assert(sess_data);
    ogs_assert((void *)sess_data == data);

    ogs_debug("    Retrieve its data: [%s]", sess_data->s6b_sid);

    sess = sess_data->sess;
    ogs_assert(sess);
    xact = sess_data->xact;
    ogs_assert(xact);

    /* Value of Result Code */
    ret = fd_msg_search_avp(*msg, ogs_diam_result_code, &avp);
    ogs_assert(ret == 0);
    if (avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        ogs_assert(ret == 0);
        result_code = hdr->avp_value->i32;
        if (result_code != ER_DIAMETER_SUCCESS) {
            ogs_error("Result Code: %d", result_code);
            error++;
        }
    } else {
        error++;

        ret = fd_msg_search_avp(*msg, ogs_diam_experimental_result, &avp);
        ogs_assert(ret == 0);
        if (avp) {
            ret = fd_avp_search_avp(
                    avp, ogs_diam_experimental_result_code, &avpch1);
            ogs_assert(ret == 0);
            if (avpch1) {
                ret = fd_msg_avp_hdr(avpch1, &hdr);
                ogs_assert(ret == 0);
                exp_result_code = hdr->avp_value->i32;
                ogs_error("Experimental Result Code: %d", exp_result_code);
            }
        } else {
            ogs_error("no Result-Code");
        }
    }

    /* Value of Origin-Host */
    ret = fd_msg_search_avp(*msg, ogs_diam_origin_host, &avp);
    ogs_assert(ret == 0);
    if (avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        ogs_assert(ret == 0);
        ogs_debug("From '%.*s' ",
                (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
    } else {
        ogs_error("no_Origin-Host ");
        error++;
    }

    /* Value of Origin-Realm */
    ret = fd_msg_search_avp(*msg, ogs_diam_origin_realm, &avp);
    ogs_assert(ret == 0);
    if (avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        ogs_assert(ret == 0);
        ogs_debug("('%.*s') ",
                (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
    } else {
        ogs_error("no_Origin-Realm ");
        error++;
    }

    if (!error) {
        smf_gx_send_ccr(sess, xact,
            OGS_DIAM_GX_CC_REQUEST_TYPE_INITIAL_REQUEST);
    }

    /* Free the message */
    ogs_assert(pthread_mutex_lock(&ogs_diam_logger_self()->stats_lock) == 0);
    dur = ((ts.tv_sec - sess_data->ts.tv_sec) * 1000000) +
        ((ts.tv_nsec - sess_data->ts.tv_nsec) / 1000);
    if (ogs_diam_logger_self()->stats.nb_recv) {
        /* Ponderate in the avg */
        ogs_diam_logger_self()->stats.avg = (ogs_diam_logger_self()->stats.avg *
            ogs_diam_logger_self()->stats.nb_recv + dur) /
            (ogs_diam_logger_self()->stats.nb_recv + 1);
        /* Min, max */
        if (dur < ogs_diam_logger_self()->stats.shortest)
            ogs_diam_logger_self()->stats.shortest = dur;
        if (dur > ogs_diam_logger_self()->stats.longest)
            ogs_diam_logger_self()->stats.longest = dur;
    } else {
        ogs_diam_logger_self()->stats.shortest = dur;
        ogs_diam_logger_self()->stats.longest = dur;
        ogs_diam_logger_self()->stats.avg = dur;
    }
    if (error)
        ogs_diam_logger_self()->stats.nb_errs++;
    else
        ogs_diam_logger_self()->stats.nb_recv++;

    ogs_assert(pthread_mutex_unlock(&ogs_diam_logger_self()->stats_lock) == 0);

    /* Display how long it took */
    if (ts.tv_nsec > sess_data->ts.tv_nsec)
        ogs_debug("in %d.%06ld sec",
                (int)(ts.tv_sec - sess_data->ts.tv_sec),
                (long)(ts.tv_nsec - sess_data->ts.tv_nsec) / 1000);
    else
        ogs_debug("in %d.%06ld sec",
                (int)(ts.tv_sec + 1 - sess_data->ts.tv_sec),
                (long)(1000000000 + ts.tv_nsec - sess_data->ts.tv_nsec) / 1000);

    ret = fd_sess_state_store(smf_s6b_reg, session, &sess_data);
    ogs_assert(ret == 0);
    ogs_assert(sess_data == NULL);

    ret = fd_msg_free(*msg);
    ogs_assert(ret == 0);
    *msg = NULL;

    return;
}

void smf_s6b_send_str(smf_sess_t *sess, ogs_gtp_xact_t *xact, uint32_t cause)
{
    int ret;

    struct msg *req = NULL;
    struct avp *avp;
    union avp_value val;
    struct sess_state *sess_data = NULL, *svg;
    struct session *session = NULL;
    int new;
    size_t sidlen;

    smf_ue_t *smf_ue = NULL;
    char *user_name = NULL;

    ogs_assert(xact);
    ogs_assert(sess);
    smf_ue = sess->smf_ue;
    ogs_assert(smf_ue);

    ogs_debug("[Session-Termination-Request]");

    /* Create the request */
    ret = fd_msg_new(ogs_diam_rx_cmd_str, MSGFL_ALLOC_ETEID, &req);
    ogs_assert(ret == 0);
    {
        struct msg_hdr * h;
        ret = fd_msg_hdr( req, &h );
        ogs_assert(ret == 0);
        h->msg_appl = OGS_DIAM_S6B_APPLICATION_ID;
    }

    ogs_assert(sess->s6b_sid);

    /* Retrieve session by Session-Id */
    sidlen = strlen(sess->s6b_sid);
    ret = fd_sess_fromsid_msg((os0_t)sess->s6b_sid, sidlen, &session, &new);
    ogs_assert(ret == 0);
    ogs_assert(new == 0);

    ogs_debug("    Found S6b Session-Id: [%s]", sess->s6b_sid);

    /* Add Session-Id to the message */
    ret = ogs_diam_message_session_id_set(req, (os0_t)sess->s6b_sid, sidlen);
    ogs_assert(ret == 0);
    /* Save the session associated with the message */
    ret = fd_msg_sess_set(req, session);

    /* Retrieve session state in this session */
    ret = fd_sess_state_retrieve(smf_s6b_reg, session, &sess_data);
    ogs_assert(ret == 0);
    ogs_assert(sess_data);
    ogs_debug("    Retrieve session: [%s]", sess_data->s6b_sid);

    /* Update session state */
    sess_data->sess = sess;
    sess_data->xact = xact;

    /* Set Origin-Host & Origin-Realm */
    ret = fd_msg_add_origin(req, 0);
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
    val.i32 = OGS_DIAM_S6B_APPLICATION_ID;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Set the Termination-Cause AVP */
    ret = fd_msg_avp_new(ogs_diam_termination_cause, 0, &avp);
    ogs_assert(ret == 0);
    val.i32 = cause;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Set the User-Name AVP */
    user_name = ogs_msprintf("%s@nai.epc.mnc%03d.mcc%03d.3gppnetwork.org",
                    smf_ue->imsi_bcd,
                    ogs_plmn_id_mnc(&sess->plmn_id),
                    ogs_plmn_id_mcc(&sess->plmn_id));
    ogs_assert(user_name);

    ret = fd_msg_avp_new(ogs_diam_user_name, 0, &avp);
    ogs_assert(ret == 0);
    val.os.data = (uint8_t *)user_name;
    val.os.len = strlen(user_name);
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Keep a pointer to the session data for debug purpose,
     * in real life we would not need it */
    svg = sess_data;

    /* Store this value in the session */
    ret = fd_sess_state_store(smf_s6b_reg, session, &sess_data);
    ogs_assert(ret == 0);
    ogs_assert(sess_data == NULL);

    /* Send the request */
    ret = fd_msg_send(&req, smf_s6b_sta_cb, svg);
    ogs_assert(ret == 0);

    /* Increment the counter */
    ogs_assert(pthread_mutex_lock(&ogs_diam_logger_self()->stats_lock) == 0);
    ogs_diam_logger_self()->stats.nb_sent++;
    ogs_assert(pthread_mutex_unlock(&ogs_diam_logger_self()->stats_lock) == 0);

    ogs_free(user_name);
}

static void smf_s6b_sta_cb(void *data, struct msg **msg)
{
    int ret;

    struct sess_state *sess_data = NULL;
    struct timespec ts;
    struct session *session;
    struct avp *avp, *avpch1;
    struct avp_hdr *hdr;
    unsigned long dur;
    int error = 0;
    int new;
    int result_code = 0;
    int exp_result_code = 0;

    smf_sess_t *sess = NULL;
    ogs_gtp_xact_t *xact = NULL;

    ogs_debug("[Session-Termination-Answer]");

    ret = clock_gettime(CLOCK_REALTIME, &ts);
    ogs_assert(ret == 0);

    /* Search the session, retrieve its data */
    ret = fd_msg_sess_get(fd_g_config->cnf_dict, *msg, &session, &new);
    ogs_assert(ret == 0);
    ogs_assert(new == 0);

    ogs_debug("    Search the session");

    ret = fd_sess_state_retrieve(smf_s6b_reg, session, &sess_data);
    ogs_assert(ret == 0);
    ogs_assert(sess_data);
    ogs_assert((void *)sess_data == data);

    ogs_debug("    Retrieve its data: [%s]", sess_data->s6b_sid);

    sess = sess_data->sess;
    ogs_assert(sess);
    xact = sess_data->xact;
    ogs_assert(xact);

    /* Value of Result Code */
    ret = fd_msg_search_avp(*msg, ogs_diam_result_code, &avp);
    ogs_assert(ret == 0);
    if (avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        ogs_assert(ret == 0);
        result_code = hdr->avp_value->i32;
        if (result_code != ER_DIAMETER_SUCCESS) {
            ogs_error("Result Code: %d", result_code);
            error++;
        }
    } else {
        error++;

        ret = fd_msg_search_avp(*msg, ogs_diam_experimental_result, &avp);
        ogs_assert(ret == 0);
        if (avp) {
            ret = fd_avp_search_avp(
                    avp, ogs_diam_experimental_result_code, &avpch1);
            ogs_assert(ret == 0);
            if (avpch1) {
                ret = fd_msg_avp_hdr(avpch1, &hdr);
                ogs_assert(ret == 0);
                exp_result_code = hdr->avp_value->i32;
                ogs_error("Experimental Result Code: %d", exp_result_code);
            }
        } else {
            ogs_error("no Result-Code");
        }
    }

    /* Value of Origin-Host */
    ret = fd_msg_search_avp(*msg, ogs_diam_origin_host, &avp);
    ogs_assert(ret == 0);
    if (avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        ogs_assert(ret == 0);
        ogs_debug("From '%.*s' ",
                (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
    } else {
        ogs_error("no_Origin-Host ");
        error++;
    }

    /* Value of Origin-Realm */
    ret = fd_msg_search_avp(*msg, ogs_diam_origin_realm, &avp);
    ogs_assert(ret == 0);
    if (avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        ogs_assert(ret == 0);
        ogs_debug("('%.*s') ",
                (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
    } else {
        ogs_error("no_Origin-Realm ");
        error++;
    }

    if (!error) {
        smf_gx_send_ccr(sess, xact,
            OGS_DIAM_GX_CC_REQUEST_TYPE_TERMINATION_REQUEST);
    }

    /* Free the message */
    ogs_assert(pthread_mutex_lock(&ogs_diam_logger_self()->stats_lock) == 0);
    dur = ((ts.tv_sec - sess_data->ts.tv_sec) * 1000000) +
        ((ts.tv_nsec - sess_data->ts.tv_nsec) / 1000);
    if (ogs_diam_logger_self()->stats.nb_recv) {
        /* Ponderate in the avg */
        ogs_diam_logger_self()->stats.avg = (ogs_diam_logger_self()->stats.avg *
            ogs_diam_logger_self()->stats.nb_recv + dur) /
            (ogs_diam_logger_self()->stats.nb_recv + 1);
        /* Min, max */
        if (dur < ogs_diam_logger_self()->stats.shortest)
            ogs_diam_logger_self()->stats.shortest = dur;
        if (dur > ogs_diam_logger_self()->stats.longest)
            ogs_diam_logger_self()->stats.longest = dur;
    } else {
        ogs_diam_logger_self()->stats.shortest = dur;
        ogs_diam_logger_self()->stats.longest = dur;
        ogs_diam_logger_self()->stats.avg = dur;
    }
    if (error)
        ogs_diam_logger_self()->stats.nb_errs++;
    else
        ogs_diam_logger_self()->stats.nb_recv++;

    ogs_assert(pthread_mutex_unlock(&ogs_diam_logger_self()->stats_lock) == 0);

    /* Display how long it took */
    if (ts.tv_nsec > sess_data->ts.tv_nsec)
        ogs_debug("in %d.%06ld sec",
                (int)(ts.tv_sec - sess_data->ts.tv_sec),
                (long)(ts.tv_nsec - sess_data->ts.tv_nsec) / 1000);
    else
        ogs_debug("in %d.%06ld sec",
                (int)(ts.tv_sec + 1 - sess_data->ts.tv_sec),
                (long)(1000000000 + ts.tv_nsec - sess_data->ts.tv_nsec) / 1000);

    ret = fd_sess_state_store(smf_s6b_reg, session, &sess_data);
    ogs_assert(ret == 0);
    ogs_assert(sess_data == NULL);

    ret = fd_msg_free(*msg);
    ogs_assert(ret == 0);
    *msg = NULL;

    return;
}

int smf_s6b_init(void)
{
    int ret;
	struct disp_when data;

    ogs_thread_mutex_init(&sess_state_mutex);
    ogs_pool_init(&sess_state_pool, ogs_app()->pool.sess);

	/* Install objects definitions for this application */
	ret = ogs_diam_s6b_init();
    ogs_assert(ret == 0);

    /* Create handler for sessions */
	ret = fd_sess_handler_create(&smf_s6b_reg, state_cleanup, NULL, NULL);
    ogs_assert(ret == 0);

	memset(&data, 0, sizeof(data));
	data.app = ogs_diam_s6b_application;

	ret = fd_disp_register(smf_s6b_fb_cb, DISP_HOW_APPID, &data, NULL,
                &hdl_s6b_fb);
    ogs_assert(ret == 0);

	/* Advertise the support for the application in the peer */
	ret = fd_disp_app_support(ogs_diam_s6b_application, ogs_diam_vendor, 1, 0);
    ogs_assert(ret == 0);

	return OGS_OK;
}

void smf_s6b_final(void)
{
    int ret;

	ret = fd_sess_handler_destroy(&smf_s6b_reg, NULL);
    ogs_assert(ret == 0);

	if (hdl_s6b_fb)
		(void) fd_disp_unregister(&hdl_s6b_fb, NULL);

    ogs_pool_final(&sess_state_pool);
    ogs_thread_mutex_destroy(&sess_state_mutex);
}
