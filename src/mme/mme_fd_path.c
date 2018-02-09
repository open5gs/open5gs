#define TRACE_MODULE _mme_fd_path

#include "core_debug.h"
#include "core_pool.h"
#include "core_lib.h"

#include "fd/fd_lib.h"
#include "fd/s6a/s6a_dict.h"
#include "fd/s6a/s6a_message.h"

#include "mme_event.h"
#include "mme_fd_path.h"

static struct session_handler *mme_s6a_reg = NULL;

struct sess_state {
    mme_ue_t *mme_ue;
    struct timespec ts; /* Time of sending the message */
};

pool_declare(mme_s6a_sess_pool, struct sess_state, MAX_POOL_OF_DIAMETER_SESS);

static void mme_s6a_aia_cb(void *data, struct msg **msg);
static void mme_s6a_ula_cb(void *data, struct msg **msg);

static void state_cleanup(struct sess_state *sess_data, os0_t sid, void *opaque)
{
    pool_free_node(&mme_s6a_sess_pool, sess_data);
}

/* MME Sends Authentication Information Request to HSS */
void mme_s6a_send_air(mme_ue_t *mme_ue,
    nas_authentication_failure_parameter_t *authentication_failure_parameter)
{
    int ret;

    struct msg *req = NULL;
    struct avp *avp;
    struct avp *avpch;
    union avp_value val;
    struct sess_state *sess_data = NULL, *svg;
    struct session *session = NULL;

    c_uint8_t resync[AUTS_LEN + RAND_LEN];

    d_assert(mme_ue, return, "Null param");

    d_trace(3, "[MME] Authentication-Information-Request\n");

    /* Clear Security Context */
    CLEAR_SECURITY_CONTEXT(mme_ue);
    
    /* Create the random value to store with the session */
    pool_alloc_node(&mme_s6a_sess_pool, &sess_data);
    d_assert(sess_data, return,);
    
    sess_data->mme_ue = mme_ue;
    
    /* Create the request */
    ret = fd_msg_new(s6a_cmd_air, MSGFL_ALLOC_ETEID, &req);
    d_assert(ret == 0, return,);
    
    /* Create a new session */
    #define S6A_APP_SID_OPT  "app_s6a"
    ret = fd_msg_new_session(req, (os0_t)S6A_APP_SID_OPT, 
            CONSTSTRLEN(S6A_APP_SID_OPT));
    d_assert(ret == 0, return,);
    ret = fd_msg_sess_get(fd_g_config->cnf_dict, req, &session, NULL);
    d_assert(ret == 0, return,);

    /* Set the Auth-Session-State AVP */
    ret = fd_msg_avp_new(fd_auth_session_state, 0, &avp);
    d_assert(ret == 0, return,);
    val.i32 = 1;
    ret = fd_msg_avp_setvalue(avp, &val);
    d_assert(ret == 0, return,);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return,);

    /* Set Origin-Host & Origin-Realm */
    ret = fd_msg_add_origin(req, 0);
    d_assert(ret == 0, return,);
    
    /* Set the Destination-Realm AVP */
    ret = fd_msg_avp_new(fd_destination_realm, 0, &avp);
    d_assert(ret == 0, return,);
    val.os.data = (unsigned char *)(fd_g_config->cnf_diamrlm);
    val.os.len  = strlen(fd_g_config->cnf_diamrlm);
    ret = fd_msg_avp_setvalue(avp, &val);
    d_assert(ret == 0, return,);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return,);
    
    /* Set the User-Name AVP */
    ret = fd_msg_avp_new(fd_user_name, 0, &avp);
    d_assert(ret == 0, return,);
    val.os.data = (c_uint8_t *)mme_ue->imsi_bcd;
    val.os.len  = strlen(mme_ue->imsi_bcd);
    ret = fd_msg_avp_setvalue(avp, &val);
    d_assert(ret == 0, return,);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return,);

    /* Add the Authentication-Info */
    ret = fd_msg_avp_new(s6a_req_eutran_auth_info, 0, &avp);
    d_assert(ret == 0, return,);
    ret = fd_msg_avp_new(s6a_number_of_requested_vectors, 0, &avpch);
    d_assert(ret == 0, return,);
    val.u32 = 1;
    ret = fd_msg_avp_setvalue (avpch, &val);
    d_assert(ret == 0, return,);
    ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch);
    d_assert(ret == 0, return,);

    ret = fd_msg_avp_new(s6a_immediate_response_preferred, 0, &avpch);
    d_assert(ret == 0, return,);
    val.u32 = 1;
    ret = fd_msg_avp_setvalue(avpch, &val);
    d_assert(ret == 0, return,);
    ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avpch);
    d_assert(ret == 0, return,);

    if (authentication_failure_parameter)
    {
        ret = fd_msg_avp_new(s6a_re_synchronization_info, 0, &avpch);
        d_assert(ret == 0, return,);
        memcpy(resync, mme_ue->rand, RAND_LEN);
        memcpy(resync+RAND_LEN,
                authentication_failure_parameter->auts, AUTS_LEN);
        val.os.len = RAND_LEN+AUTS_LEN;
        val.os.data = resync;
        ret = fd_msg_avp_setvalue(avpch, &val);
        d_assert(ret == 0, return,);
        ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avpch);
        d_assert(ret == 0, return,);
    }

    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return,);

    /* Set the Visited-PLMN-Id AVP */
    ret = fd_msg_avp_new(s6a_visited_plmn_id, 0, &avp);
    d_assert(ret == 0, return,);
    val.os.data = (c_uint8_t *)&mme_ue->visited_plmn_id;
    val.os.len  = PLMN_ID_LEN;
    ret = fd_msg_avp_setvalue(avp, &val);
    d_assert(ret == 0, return,);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return,);

    /* Set Vendor-Specific-Application-Id AVP */
    ret = fd_message_vendor_specific_appid_set(req, S6A_APPLICATION_ID);
    d_assert(ret == 0, return,);
    
    ret = clock_gettime(CLOCK_REALTIME, &sess_data->ts);
    d_assert(ret == 0, return,);
    
    /* Keep a pointer to the session data for debug purpose, 
     * in real life we would not need it */
    svg = sess_data;
    
    /* Store this value in the session */
    ret = fd_sess_state_store(mme_s6a_reg, session, &sess_data);
    d_assert(ret == 0, return,);
    d_assert(sess_data == 0, return,);
    
    /* Send the request */
    ret = fd_msg_send(&req, mme_s6a_aia_cb, svg);
    d_assert(ret == 0,,);

    /* Increment the counter */
    d_assert(pthread_mutex_lock(&fd_logger_self()->stats_lock) == 0,, );
    fd_logger_self()->stats.nb_sent++;
    d_assert(pthread_mutex_unlock(&fd_logger_self()->stats_lock) == 0,, );
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

    event_t e;
    mme_ue_t *mme_ue = NULL;
    pkbuf_t *s6abuf = NULL;
    s6a_message_t *s6a_message = NULL;
    s6a_aia_message_t *aia_message = NULL;
    c_uint16_t s6abuf_len = 0;
    e_utran_vector_t *e_utran_vector = NULL;

    d_trace(3, "[MME] Authentication-Information-Answer\n");
    
    ret = clock_gettime(CLOCK_REALTIME, &ts);
    d_assert(ret == 0, return,);

    /* Search the session, retrieve its data */
    ret = fd_msg_sess_get(fd_g_config->cnf_dict, *msg, &session, &new);
    d_assert(ret == 0, return,);
    d_assert(new == 0, return, );
    
    ret = fd_sess_state_retrieve(mme_s6a_reg, session, &sess_data);
    d_assert(ret == 0, return,);
    d_assert(sess_data && (void *)sess_data == data, return, );

    mme_ue = sess_data->mme_ue;
    d_assert(mme_ue, return, );

    s6abuf_len = sizeof(s6a_message_t);
    d_assert(s6abuf_len < 8192, return, "Not supported size:%d", s6abuf_len);
    s6abuf = pkbuf_alloc(0, s6abuf_len);
    d_assert(s6abuf, return, "Null param");
    s6a_message = s6abuf->payload;
    d_assert(s6a_message, return, "Null param");

    /* Set Authentication-Information Command */
    memset(s6a_message, 0, s6abuf_len);
    s6a_message->cmd_code = S6A_CMD_CODE_AUTHENTICATION_INFORMATION;
    aia_message = &s6a_message->aia_message;
    d_assert(aia_message, return, "Null param");
    e_utran_vector = &aia_message->e_utran_vector;
    d_assert(e_utran_vector, return, "Null param");
    
    /* Value of Result Code */
    ret = fd_msg_search_avp(*msg, fd_result_code, &avp);
    d_assert(ret == 0, return,);
    if (avp)
    {
        ret = fd_msg_avp_hdr(avp, &hdr);
        d_assert(ret == 0, return,);
        s6a_message->result_code = hdr->avp_value->i32;
        d_trace(5, "    Result Code: %d\n", hdr->avp_value->i32);
    }
    else
    {
        ret = fd_msg_search_avp(*msg, fd_experimental_result, &avp);
        d_assert(ret == 0, return,);
        if (avp)
        {
            ret = fd_avp_search_avp(avp, fd_experimental_result_code, &avpch);
            d_assert(ret == 0, return,);
            if (avpch)
            {
                ret = fd_msg_avp_hdr(avpch, &hdr);
                d_assert(ret == 0, return,);
                s6a_message->result_code = hdr->avp_value->i32;
                d_trace(5, "    Experimental Result Code: %d\n",
                        s6a_message->result_code);
            }
        }
        else
        {
            d_error("no Result-Code");
            error++;
        }
    }

    /* Value of Origin-Host */
    ret = fd_msg_search_avp(*msg, fd_origin_host, &avp);
    d_assert(ret == 0, return,);
    if (avp)
    {
        ret = fd_msg_avp_hdr(avp, &hdr);
        d_assert(ret == 0, return,);
        d_trace(5, "    From '%.*s'\n",
                (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
    }
    else
    {
        d_error("no_Origin-Host ");
        error++;
    }

    /* Value of Origin-Realm */
    ret = fd_msg_search_avp(*msg, fd_origin_realm, &avp);
    d_assert(ret == 0, return,);
    if (avp)
    {
        ret = fd_msg_avp_hdr(avp, &hdr);
        d_assert(ret == 0, return,);
        d_trace(5, "         ('%.*s')\n",
                (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
    }
    else
    {
        d_error("no_Origin-Realm ");
        error++;
    }

    if (s6a_message->result_code != ER_DIAMETER_SUCCESS)
    {
        d_warn("ERROR DIAMETER Result Code(%d)", s6a_message->result_code);
        goto out;
    }

    ret = fd_msg_search_avp(*msg, s6a_authentication_info, &avp);
    d_assert(ret == 0, return,);
    if (avp)
    {
        ret = fd_msg_avp_hdr(avp, &hdr);
        d_assert(ret == 0, return,);
    }
    else
    {
        d_error("no_Authentication-Info ");
        error++;
    }

    ret = fd_avp_search_avp(avp, s6a_e_utran_vector, &avp_e_utran_vector); 
    d_assert(ret == 0, return,);
    if (avp)
    {
        ret = fd_msg_avp_hdr(avp_e_utran_vector, &hdr);
        d_assert(ret == 0, return,);
    }
    else
    {
        d_error("no_E-UTRAN-Vector-Info ");
        error++;
    }

    ret = fd_avp_search_avp(avp_e_utran_vector, s6a_xres, &avp_xres);
    d_assert(ret == 0, return,);
    if (avp)
    {
        ret = fd_msg_avp_hdr(avp_xres, &hdr);
        d_assert(ret == 0, return,);
        memcpy(e_utran_vector->xres,
                hdr->avp_value->os.data, hdr->avp_value->os.len);
        e_utran_vector->xres_len = hdr->avp_value->os.len;
    }
    else
    {
        d_error("no_XRES");
        error++;
    }

    ret = fd_avp_search_avp(avp_e_utran_vector, s6a_kasme, &avp_kasme);
    d_assert(ret == 0, return,);
    if (avp)
    {
        ret = fd_msg_avp_hdr(avp_kasme, &hdr);
        d_assert(ret == 0, return,);
        memcpy(e_utran_vector->kasme,
                hdr->avp_value->os.data, hdr->avp_value->os.len);
    }
    else
    {
        d_error("no_KASME");
        error++;
    }


    ret = fd_avp_search_avp(avp_e_utran_vector, s6a_rand, &avp_rand);
    if (avp)
    {
        ret = fd_msg_avp_hdr(avp_rand, &hdr);
        memcpy(e_utran_vector->rand,
                hdr->avp_value->os.data, hdr->avp_value->os.len);
    }
    else
    {
        d_error("no_RAND");
        error++;
    }

    ret = fd_avp_search_avp(avp_e_utran_vector, s6a_autn, &avp_autn);
    d_assert(ret == 0, return,);
    if (avp)
    {
        ret = fd_msg_avp_hdr(avp_autn, &hdr);
        d_assert(ret == 0, return,);
        memcpy(e_utran_vector->autn,
                hdr->avp_value->os.data, hdr->avp_value->os.len);
    }
    else
    {
        d_error("no_AUTN");
        error++;
    }

out:
    if (!error)
    {
        event_set(&e, MME_EVT_S6A_MESSAGE);
        event_set_param1(&e, (c_uintptr_t)mme_ue->index);
        event_set_param2(&e, (c_uintptr_t)s6abuf);
        mme_event_send(&e);
    }

    /* Free the message */
    d_assert(pthread_mutex_lock(&fd_logger_self()->stats_lock) == 0,, );
    dur = ((ts.tv_sec - sess_data->ts.tv_sec) * 1000000) + 
        ((ts.tv_nsec - sess_data->ts.tv_nsec) / 1000);
    if (fd_logger_self()->stats.nb_recv)
    {
        /* Ponderate in the avg */
        fd_logger_self()->stats.avg = (fd_logger_self()->stats.avg * 
            fd_logger_self()->stats.nb_recv + dur) /
            (fd_logger_self()->stats.nb_recv + 1);
        /* Min, max */
        if (dur < fd_logger_self()->stats.shortest)
            fd_logger_self()->stats.shortest = dur;
        if (dur > fd_logger_self()->stats.longest)
            fd_logger_self()->stats.longest = dur;
    }
    else
    {
        fd_logger_self()->stats.shortest = dur;
        fd_logger_self()->stats.longest = dur;
        fd_logger_self()->stats.avg = dur;
    }
    if (error)
        fd_logger_self()->stats.nb_errs++;
    else 
        fd_logger_self()->stats.nb_recv++;

    d_assert(pthread_mutex_unlock(&fd_logger_self()->stats_lock) == 0,, );
    
    /* Display how long it took */
    if (ts.tv_nsec > sess_data->ts.tv_nsec)
        d_trace(9, "in %d.%06ld sec\n", 
                (int)(ts.tv_sec - sess_data->ts.tv_sec),
                (long)(ts.tv_nsec - sess_data->ts.tv_nsec) / 1000);
    else
        d_trace(9, "in %d.%06ld sec\n", 
                (int)(ts.tv_sec + 1 - sess_data->ts.tv_sec),
                (long)(1000000000 + ts.tv_nsec - sess_data->ts.tv_nsec) / 1000);
    
    ret = fd_msg_free(*msg);
    d_assert(ret == 0,,);
    *msg = NULL;

    state_cleanup(sess_data, NULL, NULL);
    return;
}

/* MME Sends Update Location Request to HSS */
void mme_s6a_send_ulr(mme_ue_t *mme_ue)
{
    int ret;

    struct msg *req = NULL;
    struct avp *avp;
    union avp_value val;
    struct sess_state *sess_data = NULL, *svg;
    struct session *session = NULL;

    d_assert(mme_ue, return, "Null Param");

    d_trace(3, "[MME] Update-Location-Request\n");
    
    /* Create the random value to store with the session */
    pool_alloc_node(&mme_s6a_sess_pool, &sess_data);
    d_assert(sess_data, return,);
    
    sess_data->mme_ue = mme_ue;
    
    /* Create the request */
    ret = fd_msg_new(s6a_cmd_ulr, MSGFL_ALLOC_ETEID, &req);
    d_assert(ret == 0, return,);
    
    /* Create a new session */
    #define S6A_APP_SID_OPT  "app_s6a"
    ret = fd_msg_new_session(req, (os0_t)S6A_APP_SID_OPT, 
            CONSTSTRLEN(S6A_APP_SID_OPT));
    d_assert(ret == 0, return,);
    ret = fd_msg_sess_get(fd_g_config->cnf_dict, req, &session, NULL);
    d_assert(ret == 0, return,);

    /* Set the Auth-Session-State AVP */
    ret = fd_msg_avp_new(fd_auth_session_state, 0, &avp);
    d_assert(ret == 0, return,);
    val.i32 = 1;
    ret = fd_msg_avp_setvalue(avp, &val);
    d_assert(ret == 0, return,);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return,);

    /* Set Origin-Host & Origin-Realm */
    ret = fd_msg_add_origin(req, 0);
    d_assert(ret == 0, return,);
    
    /* Set the Destination-Realm AVP */
    ret = fd_msg_avp_new(fd_destination_realm, 0, &avp);
    d_assert(ret == 0, return,);
    val.os.data = (unsigned char *)(fd_g_config->cnf_diamrlm);
    val.os.len  = strlen(fd_g_config->cnf_diamrlm);
    ret = fd_msg_avp_setvalue(avp, &val);
    d_assert(ret == 0, return,);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return,);
    
    /* Set the User-Name AVP */
    ret = fd_msg_avp_new(fd_user_name, 0, &avp);
    d_assert(ret == 0, return,);
    val.os.data = (c_uint8_t *)mme_ue->imsi_bcd;
    val.os.len  = strlen(mme_ue->imsi_bcd);
    ret = fd_msg_avp_setvalue(avp, &val);
    d_assert(ret == 0, return,);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return,);

    /* Set the RAT-Type */
    ret = fd_msg_avp_new(s6a_rat_type, 0, &avp);
    d_assert(ret == 0, return,);
    val.u32 = S6A_RAT_TYPE_EUTRAN;
    ret = fd_msg_avp_setvalue(avp, &val);
    d_assert(ret == 0, return,);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return,);

    /* Set the ULR-Flags */
    ret = fd_msg_avp_new(s6a_ulr_flags, 0, &avp);
    d_assert(ret == 0, return,);
    val.u32 = S6A_ULR_S6A_S6D_INDICATOR;
    ret = fd_msg_avp_setvalue(avp, &val);
    d_assert(ret == 0, return,);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return,);

    /* Set the Visited-PLMN-Id */
    ret = fd_msg_avp_new(s6a_visited_plmn_id, 0, &avp);
    d_assert(ret == 0, return,);
    val.os.data = (c_uint8_t *)&mme_ue->visited_plmn_id;
    val.os.len  = PLMN_ID_LEN;
    ret = fd_msg_avp_setvalue(avp, &val);
    d_assert(ret == 0, return,);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return,);

    /* Set the UE-SRVCC Capability */
    ret = fd_msg_avp_new(s6a_ue_srvcc_capability, 0, &avp);
    d_assert(ret == 0, return,);
    val.u32 = S6A_UE_SRVCC_NOT_SUPPORTED;
    ret = fd_msg_avp_setvalue(avp, &val);
    d_assert(ret == 0, return,);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return,);

    /* Set Vendor-Specific-Application-Id AVP */
    ret = fd_message_vendor_specific_appid_set( req, S6A_APPLICATION_ID);
    d_assert(ret == 0, return,);

    ret = clock_gettime(CLOCK_REALTIME, &sess_data->ts);
    d_assert(ret == 0, return,);
    
    /* Keep a pointer to the session data for debug purpose, 
     * in real life we would not need it */
    svg = sess_data;
    
    /* Store this value in the session */
    ret = fd_sess_state_store(mme_s6a_reg, session, &sess_data); 
    d_assert(ret == 0, return,);
    d_assert(sess_data == 0, return,);
    
    /* Send the request */
    ret = fd_msg_send(&req, mme_s6a_ula_cb, svg);
    d_assert(ret == 0,,);

    /* Increment the counter */
    d_assert(pthread_mutex_lock(&fd_logger_self()->stats_lock) == 0,, );
    fd_logger_self()->stats.nb_sent++;
    d_assert(pthread_mutex_unlock(&fd_logger_self()->stats_lock) == 0,, );
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

    event_t e;
    mme_ue_t *mme_ue = NULL;
    pkbuf_t *s6abuf = NULL;
    s6a_message_t *s6a_message = NULL;
    s6a_ula_message_t *ula_message = NULL;
    s6a_subscription_data_t *subscription_data = NULL;
    c_uint16_t s6abuf_len = 0;

    d_trace(3, "[MME] Update-Location-Answer\n");

    ret = clock_gettime(CLOCK_REALTIME, &ts);
    d_assert(ret == 0, return,);

    /* Search the session, retrieve its data */
    ret = fd_msg_sess_get(fd_g_config->cnf_dict, *msg, &session, &new);
    d_assert(ret == 0, return,);
    d_assert(new == 0, return, );
    
    ret = fd_sess_state_retrieve(mme_s6a_reg, session, &sess_data);
    d_assert(ret == 0, return,);
    d_assert(sess_data && (void *)sess_data == data, return, );

    mme_ue = sess_data->mme_ue;
    d_assert(mme_ue, return, );

    s6abuf_len = sizeof(s6a_message_t);
    d_assert(s6abuf_len < 8192, return, "Not supported size:%d", s6abuf_len);
    s6abuf = pkbuf_alloc(0, s6abuf_len);
    d_assert(s6abuf, return, "Null param");
    s6a_message = s6abuf->payload;
    d_assert(s6a_message, return, "Null param");

    /* Set Authentication-Information Command */
    memset(s6a_message, 0, s6abuf_len);
    s6a_message->cmd_code = S6A_CMD_CODE_UPDATE_LOCATION;
    ula_message = &s6a_message->ula_message;
    d_assert(ula_message, return, "Null param");
    subscription_data = &ula_message->subscription_data;
    d_assert(subscription_data, return, "Null param");
    
    /* Value of Result Code */
    ret = fd_msg_search_avp(*msg, fd_result_code, &avp);
    d_assert(ret == 0, return,);
    if (avp)
    {
        ret = fd_msg_avp_hdr(avp, &hdr);
        d_assert(ret == 0, return,);
        s6a_message->result_code = hdr->avp_value->i32;
        d_trace(5, "    Result Code: %d\n", hdr->avp_value->i32);
    }
    else
    {
        ret = fd_msg_search_avp(*msg, fd_experimental_result, &avp);
        d_assert(ret == 0, return,);
        if (avp)
        {
            ret = fd_avp_search_avp(avp, fd_experimental_result_code, &avpch);
            d_assert(ret == 0, return,);
            if (avpch)
            {
                ret = fd_msg_avp_hdr(avpch, &hdr);
                d_assert(ret == 0, return,);
                s6a_message->result_code = hdr->avp_value->i32;
                d_trace(5, "    Experimental Result Code: %d\n",
                        s6a_message->result_code);
            }
        }
        else
        {
            d_error("no Result-Code");
            error++;
        }
    }

    /* Value of Origin-Host */
    ret = fd_msg_search_avp(*msg, fd_origin_host, &avp);
    d_assert(ret == 0, return,);
    if (avp)
    {
        ret = fd_msg_avp_hdr(avp, &hdr);
        d_assert(ret == 0, return,);
        d_trace(5, "    From '%.*s'\n",
                (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
    }
    else
    {
        d_error("no_Origin-Host");
        error++;
    }

    /* Value of Origin-Realm */
    ret = fd_msg_search_avp(*msg, fd_origin_realm, &avp);
    d_assert(ret == 0, return,);
    if (avp)
    {
        ret = fd_msg_avp_hdr(avp, &hdr);
        d_assert(ret == 0, return,);
        d_trace(5, "         ('%.*s')\n",
                (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
    }
    else
    {
        d_error("no_Origin-Realm");
        error++;
    }

    ret = fd_msg_search_avp(*msg, s6a_ula_flags, &avp);
    d_assert(ret == 0, return,);
    if (avp)
    {
        ret = fd_msg_avp_hdr(avp, &hdr);
        d_assert(ret == 0, return,);
        ula_message->ula_flags = hdr->avp_value->i32;
    }
    else
    {
        d_error("no_ULA-Flags");
        error++;
    }


    ret = fd_msg_search_avp(*msg, s6a_subscription_data, &avp);
    d_assert(ret == 0, return,);
    if (avp)
    {
        ret = fd_avp_search_avp(avp, s6a_ambr, &avpch1);
        d_assert(ret == 0, return,);
        if (avpch1)
        {
            ret = fd_avp_search_avp( avpch1, s6a_max_bandwidth_ul, &avpch2);
            d_assert(ret == 0, return,);
            if (avpch2)
            {
                ret = fd_msg_avp_hdr(avpch2, &hdr);
                d_assert(ret == 0, return,);
                subscription_data->ambr.uplink = hdr->avp_value->u32;
            }
            else
            {
                d_error("no_Max-Bandwidth-UL");
                error++;
            }

            ret = fd_avp_search_avp(avpch1, s6a_max_bandwidth_dl, &avpch2);
            d_assert(ret == 0, return,);
            if (avpch2)
            {
                ret = fd_msg_avp_hdr(avpch2, &hdr);
                d_assert(ret == 0, return,);
                subscription_data->ambr.downlink = hdr->avp_value->u32;
            }
            else
            {
                d_error("no_Max-Bandwidth-DL");
                error++;
            }
        }
        else
        {
            d_error("no_AMBR");
            error++;
        }

        ret = fd_avp_search_avp(avp, s6a_apn_configuration_profile, &avpch1);
        d_assert(ret == 0, return,);
        if (avpch1)
        {
            ret = fd_msg_browse(avpch1, MSG_BRW_FIRST_CHILD, &avpch2, NULL);
            d_assert(ret == 0, return,);
            while(avpch2) 
            {
                ret = fd_msg_avp_hdr(avpch2, &hdr);
                d_assert(ret == 0, return,);
                switch(hdr->avp_code)
                {
                    case S6A_AVP_CODE_CONTEXT_IDENTIFIER:
                    {
                        subscription_data->context_identifier = 
                                hdr->avp_value->i32;
                        break;
                    }
                    case S6A_AVP_CODE_ALL_APN_CONFIG_INC_IND:
                        break;
                    case S6A_AVP_CODE_APN_CONFIGURATION:
                    {
                        pdn_t *pdn = &subscription_data->pdn[
                                        subscription_data->num_of_pdn];
                        d_assert(pdn, return, );
                        ret = fd_avp_search_avp(
                            avpch2, s6a_service_selection, &avpch3);
                        d_assert(ret == 0, return,);
                        if (avpch3)
                        {
                            ret = fd_msg_avp_hdr(avpch3, &hdr);
                            core_cpystrn(pdn->apn,
                                (char*)hdr->avp_value->os.data,
                                c_min(hdr->avp_value->os.len, MAX_APN_LEN)+1);
                        }
                        else
                        {
                            d_error("no_Service-Selection");
                            error++;
                        }

                        ret = fd_avp_search_avp(avpch2,
                            s6a_context_identifier, &avpch3);
                        d_assert(ret == 0, return,);
                        if (avpch3)
                        {
                            ret = fd_msg_avp_hdr(avpch3, &hdr);
                            pdn->context_identifier = hdr->avp_value->i32;
                        }
                        else
                        {
                            d_error("no_Context-Identifier");
                            error++;
                        }

                        ret = fd_avp_search_avp(avpch2, s6a_pdn_type, &avpch3);
                        d_assert(ret == 0, return,);
                        if (avpch3)
                        {
                            ret = fd_msg_avp_hdr(avpch3, &hdr);
                            pdn->pdn_type = hdr->avp_value->i32;
                        }
                        else
                        {
                            d_error("no_PDN-Type");
                            error++;
                        }

                        ret = fd_avp_search_avp(avpch2,
                            s6a_eps_subscribed_qos_profile, &avpch3);
                        d_assert(ret == 0, return,);
                        if (avpch3)
                        {
                            ret = fd_avp_search_avp(avpch3,
                                s6a_qos_class_identifier, &avpch4);
                            d_assert(ret == 0, return,);
                            if (avpch4)
                            {
                                ret = fd_msg_avp_hdr(avpch4, &hdr);
                                d_assert(ret == 0, return,);
                                pdn->qos.qci = hdr->avp_value->i32;
                            }
                            else
                            {
                                d_error("no_QoS-Class-Identifier");
                                error++;
                            }

                            ret = fd_avp_search_avp(avpch3,
                                s6a_allocation_retention_priority, &avpch4);
                            d_assert(ret == 0, return,);
                            if (avpch4)
                            {
                                ret = fd_avp_search_avp(avpch4,
                                    s6a_priority_level, &avpch5);
                                d_assert(ret == 0, return,);
                                if (avpch5)
                                {
                                    ret = fd_msg_avp_hdr(avpch5, &hdr);
                                    d_assert(ret == 0, return,);
                                    pdn->qos.arp.priority_level = 
                                        hdr->avp_value->i32;

                                }
                                else
                                {
                                    d_error("no_ARP");
                                    error++;
                                }

                                ret = fd_avp_search_avp(avpch4,
                                    s6a_pre_emption_capability, &avpch5);
                                d_assert(ret == 0, return,);
                                if (avpch5)
                                {
                                    ret = fd_msg_avp_hdr(avpch5, &hdr);
                                    d_assert(ret == 0, return,);
                                    pdn->qos.arp.pre_emption_capability =
                                        hdr->avp_value->i32;
                                }
                                else
                                {
                                    d_error("no_Preemption-Capability");
                                    error++;
                                }

                                ret = fd_avp_search_avp(avpch4,
                                    s6a_pre_emption_vulnerability, &avpch5);
                                d_assert(ret == 0, return,);
                                if (avpch5)
                                {
                                    ret = fd_msg_avp_hdr(avpch5, &hdr);
                                    d_assert(ret == 0, return,);
                                    pdn->qos.arp.pre_emption_vulnerability =
                                        hdr->avp_value->i32;
                                }
                                else
                                {
                                    d_error("no_Preemption-Vulnerability");
                                    error++;
                                }

                            }
                            else
                            {
                                d_error("no_QCI");
                                error++;
                            }
                        }
                        else
                        {
                            d_error("no_EPS-Subscribed-QoS-Profile");
                            error++;
                        }

                        ret = fd_avp_search_avp(avpch2,
                                fd_mip6_agent_info, &avpch3);
                        d_assert(ret == 0, return,);
                        if (avpch3)
                        {
                            ret = fd_msg_browse(avpch3,
                                MSG_BRW_FIRST_CHILD, &avpch4, NULL);
                            d_assert(ret == 0, return,);
                            while(avpch4) 
                            {
                                ret = fd_msg_avp_hdr(avpch4, &hdr);
                                switch(hdr->avp_code)
                                {
                                    case S6A_AVP_CODE_MIP_HOME_AGENT_ADDRESS:
                                    {
                                        c_sockaddr_t addr;

                                        ret = fd_msg_avp_value_interpret(avpch4,
                                                &addr.sa);
                                        d_assert(ret == 0, return,);
                                        if (addr.c_sa_family == AF_INET)
                                        {
                                            pdn->pgw_ip.ipv4 = 1;
                                            pdn->pgw_ip.both.addr = 
                                                addr.sin.sin_addr.s_addr;
                                        }
                                        else if (addr.c_sa_family == AF_INET6)
                                        {
                                            pdn->pgw_ip.ipv6 = 1;
                                            memcpy(pdn->pgw_ip.both.addr6,
                                                addr.sin6.sin6_addr.s6_addr,
                                                IPV6_LEN);
                                        }
                                        else
                                        {
                                            d_error("Invald family:%d",
                                                    addr.c_sa_family);
                                            error++;
                                        }
                                        break;
                                    }
                                    default:
                                    {
                                        d_error("Unknown AVP-Code:%d",
                                                hdr->avp_code);
                                        error++;
                                        break; 
                                    }
                                }
                                fd_msg_browse(avpch4, MSG_BRW_NEXT,
                                        &avpch4, NULL);
                            }
                        }

                        ret = fd_avp_search_avp(avpch2, s6a_ambr, &avpch3);
                        d_assert(ret == 0, return,);
                        if (avpch3)
                        {
                            ret = fd_avp_search_avp(avpch3,
                                s6a_max_bandwidth_ul, &avpch4);
                            d_assert(ret == 0, return,);
                            if (avpch4)
                            {
                                ret = fd_msg_avp_hdr(avpch4, &hdr);
                                d_assert(ret == 0, return,);
                                pdn->ambr.uplink = hdr->avp_value->u32;
                            }
                            else
                            {
                                d_error("no_Max-Bandwidth-UL");
                                error++;
                            }

                            ret = fd_avp_search_avp(avpch3,
                                s6a_max_bandwidth_dl, &avpch4);
                            d_assert(ret == 0, return,);
                            if (avpch4)
                            {
                                ret = fd_msg_avp_hdr(avpch4, &hdr);
                                d_assert(ret == 0, return,);
                                pdn->ambr.downlink = hdr->avp_value->u32;
                            }
                            else
                            {
                                d_error("no_Max-Bandwidth-DL");
                                error++;
                            }
                        }

                        subscription_data->num_of_pdn++;
                        break;
                    }
                    default:
                    {
                        d_warn("Unknown AVP-code:%d", hdr->avp_code);
                        break;
                    }
                }

                fd_msg_browse(avpch2, MSG_BRW_NEXT, &avpch2, NULL);
            }
        }
        else
        {
            d_error("no_APN-Configuration-Profile");
            error++;
        }
    }
    else
    {
        d_error("no_Subscription-Data");
        error++;
    }

    ret = fd_msg_search_avp(*msg, s6a_subscribed_rau_tau_timer, &avp);
    d_assert(ret == 0, return,);
    if (avp)
    {
        ret = fd_msg_avp_hdr(avp, &hdr);
        d_assert(ret == 0, return,);
        subscription_data->subscribed_rau_tau_timer = hdr->avp_value->i32;
    }
    else
    {
        d_error("no_Subscribed_RAU-TAU-Timer");
        error++;
    }
    
    if (!error)
    {
        event_set(&e, MME_EVT_S6A_MESSAGE);
        event_set_param1(&e, (c_uintptr_t)mme_ue->index);
        event_set_param2(&e, (c_uintptr_t)s6abuf);
        mme_event_send(&e);
    }

    /* Free the message */
    d_assert(pthread_mutex_lock(&fd_logger_self()->stats_lock) == 0,, );
    dur = ((ts.tv_sec - sess_data->ts.tv_sec) * 1000000) + 
        ((ts.tv_nsec - sess_data->ts.tv_nsec) / 1000);
    if (fd_logger_self()->stats.nb_recv)
    {
        /* Ponderate in the avg */
        fd_logger_self()->stats.avg = (fd_logger_self()->stats.avg * 
            fd_logger_self()->stats.nb_recv + dur) / 
            (fd_logger_self()->stats.nb_recv + 1);
        /* Min, max */
        if (dur < fd_logger_self()->stats.shortest)
            fd_logger_self()->stats.shortest = dur;
        if (dur > fd_logger_self()->stats.longest)
            fd_logger_self()->stats.longest = dur;
    }
    else
    {
        fd_logger_self()->stats.shortest = dur;
        fd_logger_self()->stats.longest = dur;
        fd_logger_self()->stats.avg = dur;
    }
    if (error)
        fd_logger_self()->stats.nb_errs++;
    else 
        fd_logger_self()->stats.nb_recv++;

    d_assert(pthread_mutex_unlock(&fd_logger_self()->stats_lock) == 0,, );
    
    /* Display how long it took */
    if (ts.tv_nsec > sess_data->ts.tv_nsec)
        d_trace(9, "in %d.%06ld sec\n", 
                (int)(ts.tv_sec - sess_data->ts.tv_sec),
                (long)(ts.tv_nsec - sess_data->ts.tv_nsec) / 1000);
    else
        d_trace(9, "in %d.%06ld sec\n", 
                (int)(ts.tv_sec + 1 - sess_data->ts.tv_sec),
                (long)(1000000000 + ts.tv_nsec - sess_data->ts.tv_nsec) / 1000);
    
    ret = fd_msg_free(*msg);
    d_assert(ret == 0,,);
    *msg = NULL;

    state_cleanup(sess_data, NULL, NULL);
    return;
}


status_t mme_fd_init(void)
{
    int ret;

    pool_init(&mme_s6a_sess_pool, MAX_POOL_OF_DIAMETER_SESS);

    ret = fd_init(FD_MODE_CLIENT,
                mme_self()->fd_conf_path, mme_self()->fd_config);
    d_assert(ret == CORE_OK, return CORE_ERROR,);

	/* Install objects definitions for this application */
	ret = s6a_dict_init();
    d_assert(ret == CORE_OK, return CORE_ERROR,);

    /* Create handler for sessions */
	ret = fd_sess_handler_create(&mme_s6a_reg, &state_cleanup,
                NULL, NULL);
    d_assert(ret == CORE_OK, return CORE_ERROR,);

	/* Advertise the support for the application in the peer */
	ret = fd_disp_app_support(s6a_application, fd_vendor, 1, 0);
    d_assert(ret == CORE_OK, return CORE_ERROR,);
	
	return 0;
}

void mme_fd_final(void)
{
    int ret;

	ret = fd_sess_handler_destroy(&mme_s6a_reg, NULL);
    d_assert(ret == CORE_OK,,);

    fd_final();

    if (pool_used(&mme_s6a_sess_pool))
        d_error("%d not freed in mme_s6a_sess_pool[%d] of GX-SM",
                pool_used(&mme_s6a_sess_pool), pool_size(&mme_s6a_sess_pool));
    d_trace(9, "%d not freed in mme_s6a_sess_pool[%d] of GX-SM\n",
            pool_used(&mme_s6a_sess_pool), pool_size(&mme_s6a_sess_pool));

    pool_final(&mme_s6a_sess_pool);
}
