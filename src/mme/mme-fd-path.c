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

#include "mme-event.h"
#include "mme-fd-path.h"

static struct session_handler *mme_s6a_reg = NULL;

struct sess_state {
    mme_ue_t *mme_ue;
    struct timespec ts; /* Time of sending the message */
};

static void mme_s6a_aia_cb(void *data, struct msg **msg);
static void mme_s6a_ula_cb(void *data, struct msg **msg);

static void state_cleanup(struct sess_state *sess_data, os0_t sid, void *opaque)
{
    ogs_free(sess_data);
}

/* MME Sends Authentication Information Request to HSS */
void mme_s6a_send_air(mme_ue_t *mme_ue,
    ogs_nas_authentication_failure_parameter_t
        *authentication_failure_parameter)
{
    int ret;

    struct msg *req = NULL;
    struct avp *avp;
    struct avp *avpch;
    union avp_value val;
    struct sess_state *sess_data = NULL, *svg;
    struct session *session = NULL;
    ogs_nas_plmn_id_t nas_plmn_id;

    uint8_t resync[OGS_AUTS_LEN + OGS_RAND_LEN];

    ogs_assert(mme_ue);

    ogs_debug("[MME] Authentication-Information-Request");

    /* Clear Security Context */
    CLEAR_SECURITY_CONTEXT(mme_ue);
    
    /* Create the random value to store with the session */
    sess_data = ogs_calloc(1, sizeof (*sess_data));
    ogs_assert(sess_data);
    
    sess_data->mme_ue = mme_ue;
    
    /* Create the request */
    ret = fd_msg_new(ogs_diam_s6a_cmd_air, MSGFL_ALLOC_ETEID, &req);
    ogs_assert(ret == 0);
    
    /* Create a new session */
    #define OGS_DIAM_S6A_APP_SID_OPT  "app_s6a"
    ret = fd_msg_new_session(req, (os0_t)OGS_DIAM_S6A_APP_SID_OPT, 
            CONSTSTRLEN(OGS_DIAM_S6A_APP_SID_OPT));
    ogs_assert(ret == 0);
    ret = fd_msg_sess_get(fd_g_config->cnf_dict, req, &session, NULL);
    ogs_assert(ret == 0);

    /* Set the Auth-Session-State AVP */
    ret = fd_msg_avp_new(ogs_diam_auth_session_state, 0, &avp);
    ogs_assert(ret == 0);
    val.i32 = 1;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

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
    
    /* Set the User-Name AVP */
    ret = fd_msg_avp_new(ogs_diam_user_name, 0, &avp);
    ogs_assert(ret == 0);
    val.os.data = (uint8_t *)mme_ue->imsi_bcd;
    val.os.len  = strlen(mme_ue->imsi_bcd);
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Add the Authentication-Info */
    ret = fd_msg_avp_new(ogs_diam_s6a_req_eutran_auth_info, 0, &avp);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_new(ogs_diam_s6a_number_of_requested_vectors, 0, &avpch);
    ogs_assert(ret == 0);
    val.u32 = 1;
    ret = fd_msg_avp_setvalue (avpch, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch);
    ogs_assert(ret == 0);

    ret = fd_msg_avp_new(ogs_diam_s6a_immediate_response_preferred, 0, &avpch);
    ogs_assert(ret == 0);
    val.u32 = 1;
    ret = fd_msg_avp_setvalue(avpch, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avpch);
    ogs_assert(ret == 0);

    if (authentication_failure_parameter) {
        ret = fd_msg_avp_new(ogs_diam_s6a_re_synchronization_info, 0, &avpch);
        ogs_assert(ret == 0);
        memcpy(resync, mme_ue->rand, OGS_RAND_LEN);
        memcpy(resync+OGS_RAND_LEN,
                authentication_failure_parameter->auts, OGS_AUTS_LEN);
        val.os.len = OGS_RAND_LEN+OGS_AUTS_LEN;
        val.os.data = resync;
        ret = fd_msg_avp_setvalue(avpch, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avpch);
        ogs_assert(ret == 0);
    }

    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Set the Visited-PLMN-Id AVP */
    ret = fd_msg_avp_new(ogs_diam_s6a_visited_plmn_id, 0, &avp);
    ogs_assert(ret == 0);
    val.os.data = ogs_nas_from_plmn_id(&nas_plmn_id, &mme_ue->tai.plmn_id);
    val.os.len  = OGS_PLMN_ID_LEN;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Set Vendor-Specific-Application-Id AVP */
    ret = ogs_diam_message_vendor_specific_appid_set(
            req, OGS_DIAM_S6A_APPLICATION_ID);
    ogs_assert(ret == 0);
    
    ret = clock_gettime(CLOCK_REALTIME, &sess_data->ts);
    ogs_assert(ret == 0);
    
    /* Keep a pointer to the session data for debug purpose, 
     * in real life we would not need it */
    svg = sess_data;
    
    /* Store this value in the session */
    ret = fd_sess_state_store(mme_s6a_reg, session, &sess_data);
    ogs_assert(ret == 0);
    ogs_assert(sess_data == 0);
    
    /* Send the request */
    ret = fd_msg_send(&req, mme_s6a_aia_cb, svg);
    ogs_assert(ret == 0);

    /* Increment the counter */
    ogs_assert(pthread_mutex_lock(&ogs_diam_logger_self()->stats_lock) == 0);
    ogs_diam_logger_self()->stats.nb_sent++;
    ogs_assert(pthread_mutex_unlock(&ogs_diam_logger_self()->stats_lock) == 0);
}

/* MME received Authentication Information Answer from HSS */
static void mme_s6a_aia_cb(void *data, struct msg **msg)
{
    int ret;
    
    struct sess_state *sess_data = NULL;
    struct timespec ts;
    struct session *session;
    struct avp *avp, *avpch;
    struct avp *avp_e_utran_vector, *avp_xres, *avp_kasme, *avp_rand, *avp_autn;
    struct avp_hdr *hdr;
    unsigned long dur;
    int error = 0;
    int new;

    mme_event_t *e = NULL;
    mme_ue_t *mme_ue = NULL;
    ogs_pkbuf_t *s6abuf = NULL;
    ogs_diam_s6a_message_t *s6a_message = NULL;
    ogs_diam_s6a_aia_message_t *aia_message = NULL;
    uint16_t s6abuf_len = 0;
    ogs_diam_e_utran_vector_t *e_utran_vector = NULL;

    ogs_debug("[MME] Authentication-Information-Answer");
    
    ret = clock_gettime(CLOCK_REALTIME, &ts);
    ogs_assert(ret == 0);

    /* Search the session, retrieve its data */
    ret = fd_msg_sess_get(fd_g_config->cnf_dict, *msg, &session, &new);
    ogs_expect_or_return(ret == 0);
    ogs_expect_or_return(new == 0);
    
    ret = fd_sess_state_retrieve(mme_s6a_reg, session, &sess_data);
    ogs_expect_or_return(ret == 0);
    ogs_expect_or_return(sess_data);
    ogs_expect_or_return((void *)sess_data == data);

    mme_ue = sess_data->mme_ue;
    ogs_assert(mme_ue);

    s6abuf_len = sizeof(ogs_diam_s6a_message_t);
    ogs_assert(s6abuf_len < 8192);
    s6abuf = ogs_pkbuf_alloc(NULL, s6abuf_len);
    ogs_assert(s6abuf);
    ogs_pkbuf_put(s6abuf, s6abuf_len);
    s6a_message = (ogs_diam_s6a_message_t *)s6abuf->data;
    ogs_assert(s6a_message);

    /* Set Authentication-Information Command */
    memset(s6a_message, 0, s6abuf_len);
    s6a_message->cmd_code = OGS_DIAM_S6A_CMD_CODE_AUTHENTICATION_INFORMATION;
    aia_message = &s6a_message->aia_message;
    ogs_assert(aia_message);
    e_utran_vector = &aia_message->e_utran_vector;
    ogs_assert(e_utran_vector);
    
    /* Value of Result Code */
    ret = fd_msg_search_avp(*msg, ogs_diam_result_code, &avp);
    ogs_assert(ret == 0);
    if (avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        ogs_assert(ret == 0);
        s6a_message->result_code = hdr->avp_value->i32;
        s6a_message->err = &s6a_message->result_code;
        ogs_debug("    Result Code: %d", hdr->avp_value->i32);
    } else {
        ret = fd_msg_search_avp(*msg, ogs_diam_experimental_result, &avp);
        ogs_assert(ret == 0);
        if (avp) {
            ret = fd_avp_search_avp(
                    avp, ogs_diam_experimental_result_code, &avpch);
            ogs_assert(ret == 0);
            if (avpch) {
                ret = fd_msg_avp_hdr(avpch, &hdr);
                ogs_assert(ret == 0);
                s6a_message->result_code = hdr->avp_value->i32;
                s6a_message->exp_err = &s6a_message->result_code;
                ogs_debug("    Experimental Result Code: %d",
                        s6a_message->result_code);
            }
        } else {
            ogs_error("no Result-Code");
            error++;
        }
    }

    /* Value of Origin-Host */
    ret = fd_msg_search_avp(*msg, ogs_diam_origin_host, &avp);
    ogs_assert(ret == 0);
    if (avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        ogs_assert(ret == 0);
        ogs_debug("    From '%.*s'",
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
        ogs_debug("         ('%.*s')",
                (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
    } else {
        ogs_error("no_Origin-Realm ");
        error++;
    }

    if (s6a_message->result_code != ER_DIAMETER_SUCCESS) {
        if (s6a_message->err)
            ogs_info("    Result Code: %d", s6a_message->result_code);
        else if (s6a_message->exp_err)
            ogs_info("    Experimental Result Code: %d",
                    s6a_message->result_code);
        else {
            ogs_fatal("ERROR DIAMETER Result Code(%d)",
                    s6a_message->result_code);
            ogs_assert_if_reached();
        }
        goto out;
    }

    ret = fd_msg_search_avp(*msg, ogs_diam_s6a_authentication_info, &avp);
    ogs_assert(ret == 0);
    if (avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        ogs_assert(ret == 0);
    } else {
        ogs_error("no_Authentication-Info ");
        error++;
    }

    ret = fd_avp_search_avp(
            avp, ogs_diam_s6a_e_utran_vector, &avp_e_utran_vector);
    ogs_assert(ret == 0);
    if (avp) {
        ret = fd_msg_avp_hdr(avp_e_utran_vector, &hdr);
        ogs_assert(ret == 0);
    } else {
        ogs_error("no_E-UTRAN-Vector-Info ");
        error++;
    }

    ret = fd_avp_search_avp(avp_e_utran_vector, ogs_diam_s6a_xres, &avp_xres);
    ogs_assert(ret == 0);
    if (avp) {
        ret = fd_msg_avp_hdr(avp_xres, &hdr);
        ogs_assert(ret == 0);
        memcpy(e_utran_vector->xres,
                hdr->avp_value->os.data, hdr->avp_value->os.len);
        e_utran_vector->xres_len = hdr->avp_value->os.len;
    } else {
        ogs_error("no_XRES");
        error++;
    }

    ret = fd_avp_search_avp(avp_e_utran_vector, ogs_diam_s6a_kasme, &avp_kasme);
    ogs_assert(ret == 0);
    if (avp) {
        ret = fd_msg_avp_hdr(avp_kasme, &hdr);
        ogs_assert(ret == 0);
        memcpy(e_utran_vector->kasme,
                hdr->avp_value->os.data, hdr->avp_value->os.len);
    } else {
        ogs_error("no_KASME");
        error++;
    }


    ret = fd_avp_search_avp(avp_e_utran_vector, ogs_diam_s6a_rand, &avp_rand);
    if (avp) {
        ret = fd_msg_avp_hdr(avp_rand, &hdr);
        memcpy(e_utran_vector->rand,
                hdr->avp_value->os.data, hdr->avp_value->os.len);
    } else {
        ogs_error("no_RAND");
        error++;
    }

    ret = fd_avp_search_avp(avp_e_utran_vector, ogs_diam_s6a_autn, &avp_autn);
    ogs_assert(ret == 0);
    if (avp) {
        ret = fd_msg_avp_hdr(avp_autn, &hdr);
        ogs_assert(ret == 0);
        memcpy(e_utran_vector->autn,
                hdr->avp_value->os.data, hdr->avp_value->os.len);
    } else {
        ogs_error("no_AUTN");
        error++;
    }

out:
    if (!error) {
        int rv;
        e = mme_event_new(MME_EVT_S6A_MESSAGE);
        ogs_assert(e);
        e->mme_ue = mme_ue;
        e->pkbuf = s6abuf;
        rv = ogs_queue_push(ogs_app()->queue, e);
        if (rv != OGS_OK) {
            ogs_error("ogs_queue_push() failed:%d", (int)rv);
            ogs_pkbuf_free(e->pkbuf);
            mme_event_free(e);
        } else {
            ogs_pollset_notify(ogs_app()->pollset);
        }
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
        ogs_trace("in %d.%06ld sec", 
                (int)(ts.tv_sec - sess_data->ts.tv_sec),
                (long)(ts.tv_nsec - sess_data->ts.tv_nsec) / 1000);
    else
        ogs_trace("in %d.%06ld sec", 
                (int)(ts.tv_sec + 1 - sess_data->ts.tv_sec),
                (long)(1000000000 + ts.tv_nsec - sess_data->ts.tv_nsec) / 1000);
    
    ret = fd_msg_free(*msg);
    ogs_assert(ret == 0);
    *msg = NULL;

    state_cleanup(sess_data, NULL, NULL);
    return;
}

/* MME Sends Update Location Request to HSS */
void mme_s6a_send_ulr(mme_ue_t *mme_ue)
{
    int ret;

    struct msg *req = NULL;
    struct avp *avp, *avpch;
    union avp_value val;
    struct sess_state *sess_data = NULL, *svg;
    struct session *session = NULL;
    ogs_nas_plmn_id_t nas_plmn_id;

    ogs_assert(mme_ue);

    ogs_debug("[MME] Update-Location-Request");
    
    /* Create the random value to store with the session */
    sess_data = ogs_calloc(1, sizeof(*sess_data));
    sess_data->mme_ue = mme_ue;
    
    /* Create the request */
    ret = fd_msg_new(ogs_diam_s6a_cmd_ulr, MSGFL_ALLOC_ETEID, &req);
    ogs_assert(ret == 0);
    
    /* Create a new session */
    #define OGS_DIAM_S6A_APP_SID_OPT  "app_s6a"
    ret = fd_msg_new_session(req, (os0_t)OGS_DIAM_S6A_APP_SID_OPT, 
            CONSTSTRLEN(OGS_DIAM_S6A_APP_SID_OPT));
    ogs_assert(ret == 0);
    ret = fd_msg_sess_get(fd_g_config->cnf_dict, req, &session, NULL);
    ogs_assert(ret == 0);

    /* Set the Auth-Session-State AVP */
    ret = fd_msg_avp_new(ogs_diam_auth_session_state, 0, &avp);
    ogs_assert(ret == 0);
    val.i32 = 1;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

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
    
    /* Set the User-Name AVP */
    ret = fd_msg_avp_new(ogs_diam_user_name, 0, &avp);
    ogs_assert(ret == 0);
    val.os.data = (uint8_t *)mme_ue->imsi_bcd;
    val.os.len  = strlen(mme_ue->imsi_bcd);
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Set the Terminal-Information AVP */
    if (mme_ue->imeisv_len) {
        ret = fd_msg_avp_new(ogs_diam_s6a_terminal_information, 0, &avp);
        ogs_assert(ret == 0);

        ret = fd_msg_avp_new(ogs_diam_s6a_imei, 0, &avpch);
        ogs_assert(ret == 0);
        val.os.data = (uint8_t *)mme_ue->imeisv_bcd;
        val.os.len  = 14;
        ret = fd_msg_avp_setvalue(avpch, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avpch);
        ogs_assert(ret == 0);

        ret = fd_msg_avp_new(ogs_diam_s6a_software_version, 0, &avpch);
        ogs_assert(ret == 0);
        val.os.data = (uint8_t *)mme_ue->imeisv_bcd+14;
        val.os.len  = 2;
        ret = fd_msg_avp_setvalue(avpch, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avpch);
        ogs_assert(ret == 0);

        ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
        ogs_assert(ret == 0);
    }

    /* Set the RAT-Type */
    ret = fd_msg_avp_new(ogs_diam_s6a_rat_type, 0, &avp);
    ogs_assert(ret == 0);
    val.u32 = OGS_DIAM_S6A_RAT_TYPE_EUTRAN;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Set the ULR-Flags */
    ret = fd_msg_avp_new(ogs_diam_s6a_ulr_flags, 0, &avp);
    ogs_assert(ret == 0);
    val.u32 = OGS_DIAM_S6A_ULR_S6A_S6D_INDICATOR;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Set the Visited-PLMN-Id */
    ret = fd_msg_avp_new(ogs_diam_s6a_visited_plmn_id, 0, &avp);
    ogs_assert(ret == 0);
    val.os.data = ogs_nas_from_plmn_id(&nas_plmn_id, &mme_ue->tai.plmn_id);
    val.os.len  = OGS_PLMN_ID_LEN;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Set the UE-SRVCC Capability */
    ret = fd_msg_avp_new(ogs_diam_s6a_ue_srvcc_capability, 0, &avp);
    ogs_assert(ret == 0);
    val.u32 = OGS_DIAM_S6A_UE_SRVCC_NOT_SUPPORTED;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Set Vendor-Specific-Application-Id AVP */
    ret = ogs_diam_message_vendor_specific_appid_set(
            req, OGS_DIAM_S6A_APPLICATION_ID);
    ogs_assert(ret == 0);

    ret = clock_gettime(CLOCK_REALTIME, &sess_data->ts);
    ogs_assert(ret == 0);
    
    /* Keep a pointer to the session data for debug purpose, 
     * in real life we would not need it */
    svg = sess_data;
    
    /* Store this value in the session */
    ret = fd_sess_state_store(mme_s6a_reg, session, &sess_data); 
    ogs_assert(ret == 0);
    ogs_assert(sess_data == 0);
    
    /* Send the request */
    ret = fd_msg_send(&req, mme_s6a_ula_cb, svg);
    ogs_assert(ret == 0);

    /* Increment the counter */
    ogs_assert(pthread_mutex_lock(&ogs_diam_logger_self()->stats_lock) == 0);
    ogs_diam_logger_self()->stats.nb_sent++;
    ogs_assert(pthread_mutex_unlock(&ogs_diam_logger_self()->stats_lock) == 0);
}

/* MME received Update Location Answer from HSS */
static void mme_s6a_ula_cb(void *data, struct msg **msg)
{
    int ret;

    struct sess_state *sess_data = NULL;
    struct timespec ts;
    struct session *session;
    struct avp *avp, *avpch;
    struct avp *avpch1, *avpch2, *avpch3, *avpch4, *avpch5;
    struct avp_hdr *hdr;
    unsigned long dur;
    int error = 0;
    int new;
    ogs_sockaddr_t addr;

    mme_event_t *e = NULL;
    mme_ue_t *mme_ue = NULL;
    ogs_pkbuf_t *s6abuf = NULL;
    ogs_diam_s6a_message_t *s6a_message = NULL;
    ogs_diam_s6a_ula_message_t *ula_message = NULL;
    ogs_subscription_data_t *subscription_data = NULL;
    uint16_t s6abuf_len = 0;

    ogs_debug("[MME] Update-Location-Answer");

    ret = clock_gettime(CLOCK_REALTIME, &ts);
    ogs_assert(ret == 0);

    /* Search the session, retrieve its data */
    ret = fd_msg_sess_get(fd_g_config->cnf_dict, *msg, &session, &new);
    ogs_expect_or_return(ret == 0);
    ogs_expect_or_return(new == 0);
    
    ret = fd_sess_state_retrieve(mme_s6a_reg, session, &sess_data);
    ogs_expect_or_return(ret == 0);
    ogs_expect_or_return(sess_data);
    ogs_expect_or_return((void *)sess_data == data);

    mme_ue = sess_data->mme_ue;
    ogs_assert(mme_ue);

    s6abuf_len = sizeof(ogs_diam_s6a_message_t);
    ogs_assert(s6abuf_len < 8192);
    s6abuf = ogs_pkbuf_alloc(NULL, s6abuf_len);
    ogs_assert(s6abuf);
    ogs_pkbuf_put(s6abuf, s6abuf_len);
    s6a_message = (ogs_diam_s6a_message_t *)s6abuf->data;
    ogs_assert(s6a_message);

    /* Set Authentication-Information Command */
    memset(s6a_message, 0, s6abuf_len);
    s6a_message->cmd_code = OGS_DIAM_S6A_CMD_CODE_UPDATE_LOCATION;
    ula_message = &s6a_message->ula_message;
    ogs_assert(ula_message);
    subscription_data = &ula_message->subscription_data;
    ogs_assert(subscription_data);
    
    /* Value of Result Code */
    ret = fd_msg_search_avp(*msg, ogs_diam_result_code, &avp);
    ogs_assert(ret == 0);
    if (avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        ogs_assert(ret == 0);
        s6a_message->result_code = hdr->avp_value->i32;
        s6a_message->err = &s6a_message->result_code;
        ogs_debug("    Result Code: %d", hdr->avp_value->i32);
    } else {
        ret = fd_msg_search_avp(*msg, ogs_diam_experimental_result, &avp);
        ogs_assert(ret == 0);
        if (avp) {
            ret = fd_avp_search_avp(avp,
                    ogs_diam_experimental_result_code, &avpch);
            ogs_assert(ret == 0);
            if (avpch) {
                ret = fd_msg_avp_hdr(avpch, &hdr);
                ogs_assert(ret == 0);
                s6a_message->result_code = hdr->avp_value->i32;
                s6a_message->exp_err = &s6a_message->result_code;
                ogs_debug("    Experimental Result Code: %d",
                        s6a_message->result_code);
            }
        } else {
            ogs_error("no Result-Code");
            error++;
        }
    }

    /* Value of Origin-Host */
    ret = fd_msg_search_avp(*msg, ogs_diam_origin_host, &avp);
    ogs_assert(ret == 0);
    if (avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        ogs_assert(ret == 0);
        ogs_debug("    From '%.*s'",
                (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
    } else {
        ogs_error("no_Origin-Host");
        error++;
    }

    /* Value of Origin-Realm */
    ret = fd_msg_search_avp(*msg, ogs_diam_origin_realm, &avp);
    ogs_assert(ret == 0);
    if (avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        ogs_assert(ret == 0);
        ogs_debug("         ('%.*s')",
                (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
    } else {
        ogs_error("no_Origin-Realm");
        error++;
    }

    ret = fd_msg_search_avp(*msg, ogs_diam_s6a_ula_flags, &avp);
    ogs_assert(ret == 0);
    if (avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        ogs_assert(ret == 0);
        ula_message->ula_flags = hdr->avp_value->i32;
    } else {
        ogs_error("no_ULA-Flags");
        error++;
    }


    ret = fd_msg_search_avp(*msg, ogs_diam_s6a_subscription_data, &avp);
    ogs_assert(ret == 0);
    if (avp) {

        /*
         * TS29.328
         * 6.3.2 MSISDN AVP
         *
         * The MSISDN AVP is of type OctetString.
         * This AVP contains an MSISDN, in international number format
         * as described in ITU-T Rec E.164 [8], encoded as a TBCD-string,
         * i.e. digits from 0 through 9 are encoded 0000 to 1001;
         * 1111 is used as a filler when there is an odd number of digits;
         * bits 8 to 5 of octet n encode digit 2n;
         * bits 4 to 1 of octet n encode digit 2(n-1)+1.
         */
        ret = fd_avp_search_avp(avp, ogs_diam_s6a_msisdn, &avpch1);
        ogs_assert(ret == 0);
        if (avpch1) {
            ret = fd_msg_avp_hdr(avpch1, &hdr);
            ogs_assert(ret == 0);
            if (hdr->avp_value->os.len) {
                mme_ue->msisdn_len = hdr->avp_value->os.len;
                memcpy(mme_ue->msisdn,
                    hdr->avp_value->os.data, mme_ue->msisdn_len);
                ogs_buffer_to_bcd(mme_ue->msisdn,
                        mme_ue->msisdn_len, mme_ue->msisdn_bcd);
            }
        }

        ret = fd_avp_search_avp(avp, ogs_diam_s6a_a_msisdn, &avpch1);
        ogs_assert(ret == 0);
        if (avpch1) {
            ret = fd_msg_avp_hdr(avpch1, &hdr);
            ogs_assert(ret == 0);
            if (hdr->avp_value->os.len) {
                mme_ue->a_msisdn_len = hdr->avp_value->os.len;
                memcpy(mme_ue->a_msisdn,
                    hdr->avp_value->os.data, mme_ue->a_msisdn_len);
                ogs_buffer_to_bcd(mme_ue->a_msisdn,
                        mme_ue->a_msisdn_len, mme_ue->a_msisdn_bcd);
            }
        }

        ret = fd_avp_search_avp(avp, ogs_diam_s6a_ambr, &avpch1);
        ogs_assert(ret == 0);
        if (avpch1) {
            ret = fd_avp_search_avp(avpch1,
                    ogs_diam_s6a_max_bandwidth_ul, &avpch2);
            ogs_assert(ret == 0);
            if (avpch2) {
                ret = fd_msg_avp_hdr(avpch2, &hdr);
                ogs_assert(ret == 0);
                subscription_data->ambr.uplink = hdr->avp_value->u32;
            } else {
                ogs_error("no_Max-Bandwidth-UL");
                error++;
            }

            ret = fd_avp_search_avp(avpch1,
                    ogs_diam_s6a_max_bandwidth_dl, &avpch2);
            ogs_assert(ret == 0);
            if (avpch2) {
                ret = fd_msg_avp_hdr(avpch2, &hdr);
                ogs_assert(ret == 0);
                subscription_data->ambr.downlink = hdr->avp_value->u32;
            } else {
                ogs_error("no_Max-Bandwidth-DL");
                error++;
            }

        } else {
            ogs_error("no_AMBR");
            error++;
        }

        ret = fd_avp_search_avp(avp,
                ogs_diam_s6a_subscribed_rau_tau_timer, &avpch1);
        ogs_assert(ret == 0);
        if (avpch1) {
            ret = fd_msg_avp_hdr(avpch1, &hdr);
            ogs_assert(ret == 0);
            subscription_data->subscribed_rau_tau_timer = hdr->avp_value->i32;
        } else {
            subscription_data->subscribed_rau_tau_timer =
                OGS_RAU_TAU_DEFAULT_TIME;
        }

        ret = fd_avp_search_avp(avp,
                ogs_diam_s6a_apn_configuration_profile, &avpch1);
        ogs_assert(ret == 0);
        if (avpch1) {
            ret = fd_msg_browse(avpch1, MSG_BRW_FIRST_CHILD, &avpch2, NULL);
            ogs_assert(ret == 0);
            while (avpch2) {
                ret = fd_msg_avp_hdr(avpch2, &hdr);
                ogs_assert(ret == 0);
                switch(hdr->avp_code) {
                case OGS_DIAM_S6A_AVP_CODE_CONTEXT_IDENTIFIER:
                    subscription_data->context_identifier = 
                            hdr->avp_value->i32;
                    break;
                case OGS_DIAM_S6A_AVP_CODE_ALL_APN_CONFIG_INC_IND:
                    break;
                case OGS_DIAM_S6A_AVP_CODE_APN_CONFIGURATION:
                {
                    ogs_pdn_t *pdn = &subscription_data->pdn[
                                    subscription_data->num_of_pdn];
                    ogs_assert(pdn);
                    ret = fd_avp_search_avp(
                        avpch2, ogs_diam_s6a_service_selection, &avpch3);
                    ogs_assert(ret == 0);
                    if (avpch3) {
                        ret = fd_msg_avp_hdr(avpch3, &hdr);
                        ogs_cpystrn(pdn->apn,
                            (char*)hdr->avp_value->os.data,
                            ogs_min(hdr->avp_value->os.len, OGS_MAX_APN_LEN)+1);
                    } else {
                        ogs_error("no_Service-Selection");
                        error++;
                    }

                    ret = fd_avp_search_avp(avpch2,
                        ogs_diam_s6a_context_identifier, &avpch3);
                    ogs_assert(ret == 0);
                    if (avpch3) {
                        ret = fd_msg_avp_hdr(avpch3, &hdr);
                        pdn->context_identifier = hdr->avp_value->i32;
                    } else {
                        ogs_error("no_Context-Identifier");
                        error++;
                    }

                    ret = fd_avp_search_avp(avpch2, ogs_diam_s6a_pdn_type,
                            &avpch3);
                    ogs_assert(ret == 0);
                    if (avpch3) {
                        ret = fd_msg_avp_hdr(avpch3, &hdr);
                        pdn->pdn_type = hdr->avp_value->i32;
                    } else {
                        ogs_error("no_PDN-Type");
                        error++;
                    }

                    /* Served-Party-IP-Address parsing for any static IPs */
                    ret = fd_msg_browse(avpch2, MSG_BRW_FIRST_CHILD,
                            &avpch3, NULL);
                    ogs_assert(ret == 0);
                    while (avpch3) {
                        ret = fd_msg_avp_hdr(avpch3, &hdr);
                        ogs_assert(ret == 0);
                        switch(hdr->avp_code) {
                        case OGS_DIAM_S6A_AVP_CODE_SERVED_PARTY_IP_ADDRESS:
                            ret = fd_msg_avp_value_interpret(avpch3, &addr.sa);
                            ogs_assert(ret == 0);

                            if (addr.ogs_sa_family == AF_INET) {
                                if (pdn->pdn_type == OGS_DIAM_PDN_TYPE_IPV4) {
                                    pdn->paa.addr = addr.sin.sin_addr.s_addr;
                                } else if (pdn->pdn_type ==
                                        OGS_DIAM_PDN_TYPE_IPV4V6) {
                                    pdn->paa.both.addr =
                                        addr.sin.sin_addr.s_addr;
                                } else {
                                    ogs_error("Warning: Received a static IPv4 "
                                        "address but PDN-Type does not include "
                                        "IPv4. Ignoring...");
                                }
                            } else if (addr.ogs_sa_family == AF_INET6) {
                                if (pdn->pdn_type == OGS_DIAM_PDN_TYPE_IPV6) {
                                    memcpy(pdn->paa.addr6,
                                        addr.sin6.sin6_addr.s6_addr,
                                        OGS_IPV6_LEN);
                                } else if (pdn->pdn_type ==
                                        OGS_DIAM_PDN_TYPE_IPV4V6) {
                                    memcpy(pdn->paa.both.addr6,
                                        addr.sin6.sin6_addr.s6_addr,
                                        OGS_IPV6_LEN);
                                } else {
                                    ogs_error("Warning: Received a static IPv6 "
                                        "address but PDN-Type does not include "
                                        "IPv6. Ignoring...");
                                }
                            } else {
                                ogs_error("Invalid family[%d]",
                                        addr.ogs_sa_family);
                            }
                            break;
                        default:
                            break;
                        }
                        fd_msg_browse(avpch3, MSG_BRW_NEXT, &avpch3, NULL);
                    }

                    ret = fd_avp_search_avp(avpch2,
                        ogs_diam_s6a_eps_subscribed_qos_profile, &avpch3);
                    ogs_assert(ret == 0);
                    if (avpch3) {
                        ret = fd_avp_search_avp(avpch3,
                            ogs_diam_s6a_qos_class_identifier, &avpch4);
                        ogs_assert(ret == 0);
                        if (avpch4) {
                            ret = fd_msg_avp_hdr(avpch4, &hdr);
                            ogs_assert(ret == 0);
                            pdn->qos.qci = hdr->avp_value->i32;
                        } else {
                            ogs_error("no_QoS-Class-Identifier");
                            error++;
                        }

                        ret = fd_avp_search_avp(avpch3,
                            ogs_diam_s6a_allocation_retention_priority, &avpch4);
                        ogs_assert(ret == 0);
                        if (avpch4) {
                            ret = fd_avp_search_avp(avpch4,
                                ogs_diam_s6a_priority_level, &avpch5);
                            ogs_assert(ret == 0);
                            if (avpch5) {
                                ret = fd_msg_avp_hdr(avpch5, &hdr);
                                ogs_assert(ret == 0);
                                pdn->qos.arp.priority_level = 
                                    hdr->avp_value->i32;

                            } else {
                                ogs_error("no_ARP");
                                error++;
                            }

                            ret = fd_avp_search_avp(avpch4,
                                ogs_diam_s6a_pre_emption_capability, &avpch5);
                            ogs_assert(ret == 0);
                            if (avpch5) {
                                ret = fd_msg_avp_hdr(avpch5, &hdr);
                                ogs_assert(ret == 0);
                                pdn->qos.arp.pre_emption_capability =
                                    hdr->avp_value->i32;
                            } else {
                                pdn->qos.arp.pre_emption_capability =
                                    OGS_PDN_PRE_EMPTION_CAPABILITY_DISABLED;
                            }

                            ret = fd_avp_search_avp(avpch4,
                                ogs_diam_s6a_pre_emption_vulnerability, &avpch5);
                            ogs_assert(ret == 0);
                            if (avpch5) {
                                ret = fd_msg_avp_hdr(avpch5, &hdr);
                                ogs_assert(ret == 0);
                                pdn->qos.arp.pre_emption_vulnerability =
                                    hdr->avp_value->i32;
                            } else {
                                pdn->qos.arp.pre_emption_vulnerability =
                                    OGS_PDN_PRE_EMPTION_VULNERABILITY_ENABLED;
                            }

                        } else {
                            ogs_error("no_QCI");
                            error++;
                        }
                    } else {
                        ogs_error("no_EPS-Subscribed-QoS-Profile");
                        error++;
                    }

                    ret = fd_avp_search_avp(avpch2,
                            ogs_diam_mip6_agent_info, &avpch3);
                    ogs_assert(ret == 0);
                    if (avpch3) {
                        ret = fd_msg_browse(avpch3,
                            MSG_BRW_FIRST_CHILD, &avpch4, NULL);
                        ogs_assert(ret == 0);
                        while (avpch4) {
                            ret = fd_msg_avp_hdr(avpch4, &hdr);
                            switch(hdr->avp_code) {
                            case OGS_DIAM_S6A_AVP_CODE_MIP_HOME_AGENT_ADDRESS:
                                ret = fd_msg_avp_value_interpret(avpch4,
                                        &addr.sa);
                                ogs_assert(ret == 0);
                                if (addr.ogs_sa_family == AF_INET)
                                {
                                    pdn->pgw_ip.ipv4 = 1;
                                    pdn->pgw_ip.addr =
                                        addr.sin.sin_addr.s_addr;
                                }
                                else if (addr.ogs_sa_family == AF_INET6)
                                {
                                    pdn->pgw_ip.ipv6 = 1;
                                    memcpy(pdn->pgw_ip.addr6,
                                        addr.sin6.sin6_addr.s6_addr,
                                        OGS_IPV6_LEN);
                                }
                                else
                                {
                                    ogs_error("Invald family:%d",
                                            addr.ogs_sa_family);
                                    error++;
                                }
                                break;
                            default:
                                ogs_error("Unknown AVP-Code:%d",
                                        hdr->avp_code);
                                error++;
                                break; 
                            }
                            fd_msg_browse(avpch4, MSG_BRW_NEXT,
                                    &avpch4, NULL);
                        }
                    }

                    ret = fd_avp_search_avp(avpch2, ogs_diam_s6a_ambr, &avpch3);
                    ogs_assert(ret == 0);
                    if (avpch3) {
                        ret = fd_avp_search_avp(avpch3,
                            ogs_diam_s6a_max_bandwidth_ul, &avpch4);
                        ogs_assert(ret == 0);
                        if (avpch4) {
                            ret = fd_msg_avp_hdr(avpch4, &hdr);
                            ogs_assert(ret == 0);
                            pdn->ambr.uplink = hdr->avp_value->u32;
                        } else {
                            ogs_error("no_Max-Bandwidth-UL");
                            error++;
                        }

                        ret = fd_avp_search_avp(avpch3,
                            ogs_diam_s6a_max_bandwidth_dl, &avpch4);
                        ogs_assert(ret == 0);
                        if (avpch4) {
                            ret = fd_msg_avp_hdr(avpch4, &hdr);
                            ogs_assert(ret == 0);
                            pdn->ambr.downlink = hdr->avp_value->u32;
                        } else {
                            ogs_error("no_Max-Bandwidth-DL");
                            error++;
                        }
                    }

                    subscription_data->num_of_pdn++;
                    break;
                }
                default:
                    ogs_warn("Unknown AVP-code:%d", hdr->avp_code);
                    break;
                }

                fd_msg_browse(avpch2, MSG_BRW_NEXT, &avpch2, NULL);
            }
        } else {
            ogs_error("no_APN-Configuration-Profile");
            error++;
        }
    } else {
        ogs_error("no_Subscription-Data");
        error++;
    }

    if (!error) {
        int rv;
        e = mme_event_new(MME_EVT_S6A_MESSAGE);
        ogs_assert(e);
        e->mme_ue = mme_ue;
        e->pkbuf = s6abuf;
        rv = ogs_queue_push(ogs_app()->queue, e);
        if (rv != OGS_OK) {
            ogs_error("ogs_queue_push() failed:%d", (int)rv);
            ogs_pkbuf_free(e->pkbuf);
            mme_event_free(e);
        } else {
            ogs_pollset_notify(ogs_app()->pollset);
        }
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
        ogs_trace("in %d.%06ld sec", 
                (int)(ts.tv_sec - sess_data->ts.tv_sec),
                (long)(ts.tv_nsec - sess_data->ts.tv_nsec) / 1000);
    else
        ogs_trace("in %d.%06ld sec", 
                (int)(ts.tv_sec + 1 - sess_data->ts.tv_sec),
                (long)(1000000000 + ts.tv_nsec - sess_data->ts.tv_nsec) / 1000);
    
    ret = fd_msg_free(*msg);
    ogs_assert(ret == 0);
    *msg = NULL;

    state_cleanup(sess_data, NULL, NULL);
    return;
}


int mme_fd_init(void)
{
    int ret;
    struct dict_object *s6a_app, *vnd;
    struct dict_vendor_data vnd_data;
    struct dict_application_data s6a_app_data;

    ret = ogs_diam_init(FD_MODE_CLIENT,
                mme_self()->diam_conf_path, mme_self()->diam_config);
    ogs_assert(ret == OGS_OK);

    vnd_data.vendor_id = 10415;
    vnd_data.vendor_name = (char *) "3GPP";

    ret = fd_dict_new(fd_g_config->cnf_dict,
            DICT_VENDOR, &vnd_data, NULL, &vnd);
    ogs_assert(ret == 0);

    s6a_app_data.application_id = 16777251;
    s6a_app_data.application_name = (char *) "S6A";

    ret = fd_dict_new(fd_g_config->cnf_dict, DICT_APPLICATION,
            &s6a_app_data, NULL, &s6a_app);
    ogs_assert(ret == 0);

    ret = fd_disp_app_support(s6a_app, vnd, 1, 0);
    ogs_assert(ret == 0);

	/* Install objects definitions for this application */
	ret = ogs_diam_s6a_init();
    ogs_assert(ret == OGS_OK);

    /* Create handler for sessions */
	ret = fd_sess_handler_create(&mme_s6a_reg, &state_cleanup,
                NULL, NULL);
    ogs_assert(ret == OGS_OK);

	/* Advertise the support for the application in the peer */
	ret = fd_disp_app_support(ogs_diam_s6a_application, ogs_diam_vendor, 1, 0);
    ogs_assert(ret == OGS_OK);
	
	return 0;
}

void mme_fd_final(void)
{
    int ret;

	ret = fd_sess_handler_destroy(&mme_s6a_reg, NULL);
    ogs_assert(ret == OGS_OK);

    ogs_diam_final();
}
