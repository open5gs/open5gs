#define TRACE_MODULE _mme_fd_path

#include "core_debug.h"
#include "core_pool.h"
#include "core_lib.h"

#include "fd_lib.h"
#include "s6a_dict.h"
#include "s6a_message.h"

#include "mme_event.h"
#include "mme_fd_path.h"

#define MAX_NUM_SESSION_STATE 32

static struct session_handler *mme_s6a_reg = NULL;

struct sess_state {
    mme_ue_t *mme_ue;
    struct timespec ts; /* Time of sending the message */
};

pool_declare(mme_s6a_sess_pool, struct sess_state, MAX_NUM_SESSION_STATE);

static void mme_s6a_aia_cb(void *data, struct msg **msg);
static void mme_s6a_ula_cb(void *data, struct msg **msg);

/* MME Sends Authentication Information Request to HSS */
void mme_s6a_send_air(mme_ue_t *mme_ue)
{
    struct msg *req = NULL;
    struct avp *avp;
    struct avp *avpch;
    union avp_value val;
    struct sess_state *mi = NULL, *svg;
    struct session *session = NULL;

    d_assert(mme_ue, return, "Null param");

    /* Clear Security Context */
    CLEAR_SECURITY_CONTEXT(mme_ue);
    
    /* Create the random value to store with the session */
    pool_alloc_node(&mme_s6a_sess_pool, &mi);
    d_assert(mi, return, "malloc failed: %s", strerror(errno));
    
    mi->mme_ue = mme_ue;
    
    /* Create the request */
    CHECK_FCT_DO( fd_msg_new(s6a_cmd_air, MSGFL_ALLOC_ETEID, &req), goto out );
    
    /* Create a new session */
    #define S6A_APP_SID_OPT  "app_s6a"
    CHECK_FCT_DO( fd_msg_new_session(req, (os0_t)S6A_APP_SID_OPT, 
            CONSTSTRLEN(S6A_APP_SID_OPT)), goto out );
    CHECK_FCT_DO( fd_msg_sess_get(fd_g_config->cnf_dict, req, &session, NULL),
            goto out );

    /* Set the Auth-Session-State AVP */
    CHECK_FCT_DO( fd_msg_avp_new(fd_auth_session_state, 0, &avp), goto out );
    val.i32 = 1;
    CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp), goto out );

    /* Set Origin-Host & Origin-Realm */
    CHECK_FCT_DO( fd_msg_add_origin(req, 0), goto out );
    
    /* Set the Destination-Realm AVP */
    CHECK_FCT_DO( fd_msg_avp_new(fd_destination_realm, 0, &avp), goto out );
    val.os.data = (unsigned char *)(fd_g_config->cnf_diamrlm);
    val.os.len  = strlen(fd_g_config->cnf_diamrlm);
    CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp), goto out );
    
    /* Set the User-Name AVP */
    CHECK_FCT_DO( fd_msg_avp_new(fd_user_name, 0, &avp), goto out );
    val.os.data = (c_uint8_t *)mme_ue->imsi_bcd;
    val.os.len  = strlen(mme_ue->imsi_bcd);
    CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp), goto out );

    /* Add the Authentication-Info */
    CHECK_FCT_DO( fd_msg_avp_new(s6a_req_eutran_auth_info, 0, &avp), goto out );
    CHECK_FCT_DO( fd_msg_avp_new(s6a_number_of_requested_vectors, 0, &avpch),
            goto out );
    val.u32 = 1;
    CHECK_FCT_DO( fd_msg_avp_setvalue (avpch, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch), goto out );

    CHECK_FCT_DO( fd_msg_avp_new(s6a_immediate_response_preferred, 0, &avpch),
            goto out );
    val.u32 = 1;
    CHECK_FCT_DO( fd_msg_avp_setvalue(avpch, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avpch), goto out );

    CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp), goto out );

    /* Set the Visited-PLMN-Id AVP */
    CHECK_FCT_DO( fd_msg_avp_new(s6a_visited_plmn_id, 0, &avp), goto out );
    val.os.data = (c_uint8_t *)&mme_ue->visited_plmn_id;
    val.os.len  = PLMN_ID_LEN;
    CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp), goto out );

    /* Set Vendor-Specific-Application-Id AVP */
    CHECK_FCT_DO( fd_message_vendor_specific_appid_set(
                req, S6A_APPLICATION_ID), goto out );
    
    CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &mi->ts), goto out );
    
    /* Keep a pointer to the session data for debug purpose, 
     * in real life we would not need it */
    svg = mi;
    
    /* Store this value in the session */
    CHECK_FCT_DO( fd_sess_state_store(mme_s6a_reg, session, &mi), goto out );
    
    /* Send the request */
    CHECK_FCT_DO( fd_msg_send(&req, mme_s6a_aia_cb, svg), goto out );

    /* Increment the counter */
    CHECK_POSIX_DO( pthread_mutex_lock(&fd_logger_self()->stats_lock), );
    fd_logger_self()->stats.nb_sent++;
    CHECK_POSIX_DO( pthread_mutex_unlock(&fd_logger_self()->stats_lock), );

    d_trace(3, "[S6A] Authentication-Information-Request : UE[%s] --> HSS\n", 
            mme_ue->imsi_bcd);

out:
    pool_free_node(&mme_s6a_sess_pool, mi);
    return;
}

/* MME received Authentication Information Answer from HSS */
static void mme_s6a_aia_cb(void *data, struct msg **msg)
{
    struct sess_state *mi = NULL;
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
    
    CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &ts), return );

    /* Search the session, retrieve its data */
    CHECK_FCT_DO( fd_msg_sess_get(fd_g_config->cnf_dict, *msg, &session, &new),
            return );
    d_assert(new == 0, return, );
    
    CHECK_FCT_DO( fd_sess_state_retrieve(mme_s6a_reg, session, &mi), return );
    d_assert(mi && (void *)mi == data, return, );

    mme_ue = mi->mme_ue;
    d_assert(mme_ue, return, );

    s6abuf_len = sizeof(s6a_message_t);
    d_assert(s6abuf_len < 8192, return, "Not supported size:%d", s6abuf_len);
    s6abuf = pkbuf_alloc(0, s6abuf_len);
    d_assert(s6abuf, return, "Null param");
    s6a_message = s6abuf->payload;
    d_assert(s6a_message, return, "Null param");

    d_trace(3, "[S6A] Authentication-Information-Answer : UE <-- HSS\n");

    /* Set Authentication-Information Command */
    memset(s6a_message, 0, s6abuf_len);
    s6a_message->cmd_code = S6A_CMD_CODE_AUTHENTICATION_INFORMATION;
    aia_message = &s6a_message->aia_message;
    d_assert(aia_message, return, "Null param");
    e_utran_vector = &aia_message->e_utran_vector;
    d_assert(e_utran_vector, return, "Null param");
    
    /* Value of Result Code */
    CHECK_FCT_DO( fd_msg_search_avp(*msg, fd_result_code, &avp), return );
    if (avp)
    {
        CHECK_FCT_DO( fd_msg_avp_hdr(avp, &hdr), return);
        s6a_message->result_code = hdr->avp_value->i32;
        d_trace(3, "Result Code: %d\n", hdr->avp_value->i32);
    }
    else
    {
        CHECK_FCT_DO( fd_msg_search_avp(*msg, 
                fd_experimental_result, &avp), return );
        if (avp)
        {
            CHECK_FCT_DO( fd_avp_search_avp(avp, 
                    fd_experimental_result_code, &avpch), return );
            if (avpch)
            {
                CHECK_FCT_DO( fd_msg_avp_hdr(avpch, &hdr), return);
                s6a_message->result_code = hdr->avp_value->i32;
                d_trace(3, "Experimental Result Code: %d\n",
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
    CHECK_FCT_DO( fd_msg_search_avp(*msg, fd_origin_host, &avp), return );
    if (avp)
    {
        CHECK_FCT_DO( fd_msg_avp_hdr(avp, &hdr), return );
        d_trace(3, "From '%.*s' ",
                (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
    }
    else
    {
        d_error("no_Origin-Host ");
        error++;
    }

    /* Value of Origin-Realm */
    CHECK_FCT_DO( fd_msg_search_avp(*msg, fd_origin_realm, &avp), return );
    if (avp)
    {
        CHECK_FCT_DO( fd_msg_avp_hdr(avp, &hdr), return );
        d_trace(3, "('%.*s') ",
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
        error++;
        goto out;
    }

    CHECK_FCT_DO(
        fd_msg_search_avp(*msg, s6a_authentication_info, &avp), return );
    if (avp)
    {
        CHECK_FCT_DO( fd_msg_avp_hdr(avp, &hdr), return );
    }
    else
        error++;

    CHECK_FCT_DO( fd_avp_search_avp(avp, s6a_e_utran_vector, 
        &avp_e_utran_vector), return ); 
    if (avp)
    {
        CHECK_FCT_DO( fd_msg_avp_hdr(avp_e_utran_vector, &hdr), return );
    }
    else
        error++;

    CHECK_FCT_DO(
        fd_avp_search_avp(avp_e_utran_vector, s6a_xres, &avp_xres), return );
    if (avp)
    {
        CHECK_FCT_DO( fd_msg_avp_hdr(avp_xres, &hdr), return );
        memcpy(e_utran_vector->xres,
                hdr->avp_value->os.data, hdr->avp_value->os.len);
        e_utran_vector->xres_len = hdr->avp_value->os.len;
    }
    else
        error++;

    CHECK_FCT_DO(
        fd_avp_search_avp(avp_e_utran_vector, s6a_kasme, &avp_kasme), return );
    if (avp)
    {
        CHECK_FCT_DO( fd_msg_avp_hdr(avp_kasme, &hdr), return );
        memcpy(e_utran_vector->kasme,
                hdr->avp_value->os.data, hdr->avp_value->os.len);
    }
    else
        error++;


    CHECK_FCT_DO(
        fd_avp_search_avp(avp_e_utran_vector, s6a_rand, &avp_rand), return );
    if (avp)
    {
        CHECK_FCT_DO( fd_msg_avp_hdr(avp_rand, &hdr), return );
        memcpy(e_utran_vector->rand,
                hdr->avp_value->os.data, hdr->avp_value->os.len);
    }
    else
        error++;

    CHECK_FCT_DO(
        fd_avp_search_avp(avp_e_utran_vector, s6a_autn, &avp_autn), return );
    if (avp)
    {
        CHECK_FCT_DO( fd_msg_avp_hdr(avp_autn, &hdr), return );
        memcpy(e_utran_vector->autn,
                hdr->avp_value->os.data, hdr->avp_value->os.len);
    }
    else
        error++;

out:
    event_set(&e, MME_EVT_S6A_MESSAGE);
    event_set_param1(&e, (c_uintptr_t)mme_ue->index);
    event_set_param2(&e, (c_uintptr_t)s6abuf);
    mme_event_send(&e);

    /* Free the message */
    CHECK_POSIX_DO( pthread_mutex_lock(&fd_logger_self()->stats_lock), );
    dur = ((ts.tv_sec - mi->ts.tv_sec) * 1000000) + 
        ((ts.tv_nsec - mi->ts.tv_nsec) / 1000);
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

    CHECK_POSIX_DO( pthread_mutex_unlock(&fd_logger_self()->stats_lock), );
    
    /* Display how long it took */
    if (ts.tv_nsec > mi->ts.tv_nsec)
        d_trace(3, "in %d.%06ld sec\n", 
                (int)(ts.tv_sec - mi->ts.tv_sec),
                (long)(ts.tv_nsec - mi->ts.tv_nsec) / 1000);
    else
        d_trace(3, "in %d.%06ld sec\n", 
                (int)(ts.tv_sec + 1 - mi->ts.tv_sec),
                (long)(1000000000 + ts.tv_nsec - mi->ts.tv_nsec) / 1000);
    
    CHECK_FCT_DO( fd_msg_free(*msg), return );
    *msg = NULL;

    pool_free_node(&mme_s6a_sess_pool, mi);

    return;
}

/* MME Sends Update Location Request to HSS */
void mme_s6a_send_ulr(mme_ue_t *mme_ue)
{
    struct msg *req = NULL;
    struct avp *avp;
    union avp_value val;
    struct sess_state *mi = NULL, *svg;
    struct session *session = NULL;

    d_assert(mme_ue, return, "Null Param");
    
    /* Create the random value to store with the session */
    pool_alloc_node(&mme_s6a_sess_pool, &mi);
    d_assert(mi, return, "malloc failed: %s", strerror(errno));
    
    mi->mme_ue = mme_ue;
    
    /* Create the request */
    CHECK_FCT_DO( fd_msg_new(s6a_cmd_ulr, MSGFL_ALLOC_ETEID, &req), goto out );
    
    /* Create a new session */
    #define S6A_APP_SID_OPT  "app_s6a"
    CHECK_FCT_DO( fd_msg_new_session(req, (os0_t)S6A_APP_SID_OPT, 
            CONSTSTRLEN(S6A_APP_SID_OPT)), goto out );
    CHECK_FCT_DO( fd_msg_sess_get(fd_g_config->cnf_dict, req, &session, NULL), 
            goto out );

    /* Set the Auth-Session-State AVP */
    CHECK_FCT_DO( fd_msg_avp_new(fd_auth_session_state, 0, &avp), goto out );
    val.i32 = 1;
    CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp), goto out );

    /* Set Origin-Host & Origin-Realm */
    CHECK_FCT_DO( fd_msg_add_origin(req, 0), goto out  );
    
    /* Set the Destination-Realm AVP */
    CHECK_FCT_DO( fd_msg_avp_new(fd_destination_realm, 0, &avp), goto out );
    val.os.data = (unsigned char *)(fd_g_config->cnf_diamrlm);
    val.os.len  = strlen(fd_g_config->cnf_diamrlm);
    CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out  );
    CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp), goto out  );
    
    /* Set the User-Name AVP */
    CHECK_FCT_DO( fd_msg_avp_new(fd_user_name, 0, &avp), goto out );
    val.os.data = (c_uint8_t *)mme_ue->imsi_bcd;
    val.os.len  = strlen(mme_ue->imsi_bcd);
    CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out  );
    CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp), goto out );

    /* Set the RAT-Type */
    CHECK_FCT_DO( fd_msg_avp_new(s6a_rat_type, 0, &avp), goto out );
    val.u32 = S6A_RAT_TYPE_EUTRAN;
    CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp), goto out );

    /* Set the ULR-Flags */
    CHECK_FCT_DO( fd_msg_avp_new(s6a_ulr_flags, 0, &avp), goto out );
    val.u32 = S6A_ULR_S6A_S6D_INDICATOR;
    CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp), goto out );

    /* Set the Visited-PLMN-Id */
    CHECK_FCT_DO( fd_msg_avp_new(s6a_visited_plmn_id, 0, &avp), goto out );
    val.os.data = (c_uint8_t *)&mme_ue->visited_plmn_id;
    val.os.len  = PLMN_ID_LEN;
    CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp), goto out );

    /* Set the UE-SRVCC Capability */
    CHECK_FCT_DO( fd_msg_avp_new(s6a_ue_srvcc_capability, 0, &avp), goto out );
    val.u32 = S6A_UE_SRVCC_NOT_SUPPORTED;
    CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp), goto out );

    /* Set Vendor-Specific-Application-Id AVP */
    CHECK_FCT_DO( fd_message_vendor_specific_appid_set(
                req, S6A_APPLICATION_ID), goto out );

    CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &mi->ts), goto out );
    
    /* Keep a pointer to the session data for debug purpose, 
     * in real life we would not need it */
    svg = mi;
    
    /* Store this value in the session */
    CHECK_FCT_DO( fd_sess_state_store(mme_s6a_reg, session, &mi), goto out ); 
    
    /* Send the request */
    CHECK_FCT_DO( fd_msg_send(&req, mme_s6a_ula_cb, svg), goto out );

    /* Increment the counter */
    CHECK_POSIX_DO( pthread_mutex_lock(&fd_logger_self()->stats_lock), );
    fd_logger_self()->stats.nb_sent++;
    CHECK_POSIX_DO( pthread_mutex_unlock(&fd_logger_self()->stats_lock), );

    d_trace(3, "[S6A] Update-Location-Request : UE[%s] --> HSS\n", 
            mme_ue->imsi_bcd);

out:
    pool_free_node(&mme_s6a_sess_pool, mi);
    return;
}

/* MME received Update Location Answer from HSS */
static void mme_s6a_ula_cb(void *data, struct msg **msg)
{
    struct sess_state *mi = NULL;
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

    CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &ts), return );

    /* Search the session, retrieve its data */
    CHECK_FCT_DO( fd_msg_sess_get(fd_g_config->cnf_dict, *msg, &session, &new),
            return );
    d_assert(new == 0, return, );
    
    CHECK_FCT_DO( fd_sess_state_retrieve(mme_s6a_reg, session, &mi), return );
    d_assert(mi && (void *)mi == data, return, );

    mme_ue = mi->mme_ue;
    d_assert(mme_ue, return, );

    s6abuf_len = sizeof(s6a_message_t);
    d_assert(s6abuf_len < 8192, return, "Not supported size:%d", s6abuf_len);
    s6abuf = pkbuf_alloc(0, s6abuf_len);
    d_assert(s6abuf, return, "Null param");
    s6a_message = s6abuf->payload;
    d_assert(s6a_message, return, "Null param");

    d_trace(3, "[S6A] Update-Location-Answer : UE <-- HSS\n");

    /* Set Authentication-Information Command */
    memset(s6a_message, 0, s6abuf_len);
    s6a_message->cmd_code = S6A_CMD_CODE_UPDATE_LOCATION;
    ula_message = &s6a_message->ula_message;
    d_assert(ula_message, return, "Null param");
    subscription_data = &ula_message->subscription_data;
    d_assert(subscription_data, return, "Null param");
    
    /* Value of Result Code */
    CHECK_FCT_DO( fd_msg_search_avp(*msg, fd_result_code, &avp), return );
    if (avp)
    {
        CHECK_FCT_DO( fd_msg_avp_hdr(avp, &hdr), return);
        s6a_message->result_code = hdr->avp_value->i32;
        d_trace(3, "Result Code: %d\n", hdr->avp_value->i32);
    }
    else
    {
        CHECK_FCT_DO( fd_msg_search_avp(*msg, 
                fd_experimental_result, &avp), return );
        if (avp)
        {
            CHECK_FCT_DO( fd_avp_search_avp(avp, 
                    fd_experimental_result_code, &avpch), return );
            if (avpch)
            {
                CHECK_FCT_DO( fd_msg_avp_hdr(avpch, &hdr), return);
                s6a_message->result_code = hdr->avp_value->i32;
                d_trace(3, "Experimental Result Code: %d\n",
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
    CHECK_FCT_DO( fd_msg_search_avp(*msg, fd_origin_host, &avp), return );
    if (avp)
    {
        CHECK_FCT_DO( fd_msg_avp_hdr(avp, &hdr), return );
        d_trace(3, "From '%.*s' ",
                (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
    }
    else
    {
        d_error("no_Origin-Host ");
        error++;
    }

    /* Value of Origin-Realm */
    CHECK_FCT_DO( fd_msg_search_avp(*msg, fd_origin_realm, &avp), return );
    if (avp)
    {
        CHECK_FCT_DO( fd_msg_avp_hdr(avp, &hdr), return );
        d_trace(3, "('%.*s') ",
                (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
    }
    else
    {
        d_error("no_Origin-Realm ");
        error++;
    }

    CHECK_FCT_DO( fd_msg_search_avp(*msg, s6a_ula_flags, &avp), return);
    if (avp)
    {
        CHECK_FCT_DO( fd_msg_avp_hdr(avp, &hdr), return );
        ula_message->ula_flags = hdr->avp_value->i32;
    }
    else
        error++;


    CHECK_FCT_DO(
        fd_msg_search_avp(*msg, s6a_subscription_data, &avp), return );
    if (avp)
    {
        CHECK_FCT_DO( fd_avp_search_avp(avp, s6a_ambr, &avpch1), return );
        if (avpch1)
        {
            CHECK_FCT_DO( fd_avp_search_avp(
                    avpch1, s6a_max_bandwidth_ul, &avpch2), return);
            if (avpch2)
            {
                CHECK_FCT_DO( fd_msg_avp_hdr(avpch2, &hdr), return );
                subscription_data->ambr.uplink = hdr->avp_value->u32;
            }
            else
                error++;

            CHECK_FCT_DO(fd_avp_search_avp(
                    avpch1, s6a_max_bandwidth_dl, &avpch2), return );
            if (avpch2)
            {
                CHECK_FCT_DO( fd_msg_avp_hdr(avpch2, &hdr), return );
                subscription_data->ambr.downlink = hdr->avp_value->u32;
            }
            else
                error++;
        }
        else
            error++;

        CHECK_FCT_DO( fd_avp_search_avp(
                avp, s6a_apn_configuration_profile, &avpch1), return );
        if (avpch1)
        {
            CHECK_FCT_DO( fd_msg_browse(
                avpch1, MSG_BRW_FIRST_CHILD, &avpch2, NULL), return );
            while(avpch2) 
            {
                CHECK_FCT_DO( fd_msg_avp_hdr(avpch2, &hdr), return );
                switch(hdr->avp_code)
                {
                    case S6A_AVP_CODE_CONTEXT_IDENTIFIER:
                        break;
                    case S6A_AVP_CODE_ALL_APN_CONFIG_INC_IND:
                        break;
                    case S6A_AVP_CODE_APN_CONFIGURATION:
                    {
                        pdn_t *pdn = &subscription_data->pdn[
                                        subscription_data->num_of_pdn];
                        d_assert(pdn, return, );
                        CHECK_FCT_DO( fd_avp_search_avp(
                            avpch2, s6a_service_selection, &avpch3), return );
                        if (avpch3)
                        {
                            CHECK_FCT_DO( 
                                fd_msg_avp_hdr(avpch3, &hdr), return );
                            core_cpystrn(pdn->apn,
                                (char*)hdr->avp_value->os.data,
                                c_min(hdr->avp_value->os.len, MAX_APN_LEN)+1);
                        }
                        else
                            error++;

                        CHECK_FCT_DO( fd_avp_search_avp(avpch2,
                            s6a_context_identifier, &avpch3), return );
                        if (avpch3)
                        {
                            CHECK_FCT_DO( 
                                fd_msg_avp_hdr(avpch3, &hdr), return );
                            pdn->context_identifier = hdr->avp_value->i32;
                        }
                        else
                            error++;

                        CHECK_FCT_DO( fd_avp_search_avp(avpch2,
                            s6a_pdn_type, &avpch3), return );
                        if (avpch3)
                        {
                            CHECK_FCT_DO( 
                                fd_msg_avp_hdr(avpch3, &hdr), return );
                            pdn->pdn_type = hdr->avp_value->i32;
                        }
                        else
                            error++;

                        CHECK_FCT_DO( fd_avp_search_avp(avpch2,
                            s6a_eps_subscribed_qos_profile, &avpch3), return );
                        if (avpch3)
                        {
                            CHECK_FCT_DO(fd_avp_search_avp(avpch3,
                                s6a_qos_class_identifier, &avpch4), return );
                            if (avpch4)
                            {
                                CHECK_FCT_DO( 
                                    fd_msg_avp_hdr(avpch4, &hdr), return );
                                pdn->qos.qci = hdr->avp_value->i32;
                            }
                            else
                                error++;

                            CHECK_FCT_DO(fd_avp_search_avp(avpch3,
                                s6a_allocation_retention_priority, &avpch4),
                                return );
                            if (avpch4)
                            {
                                CHECK_FCT_DO( fd_avp_search_avp(avpch4,
                                    s6a_priority_level, &avpch5), return );
                                if (avpch5)
                                {
                                    CHECK_FCT_DO( fd_msg_avp_hdr(avpch5, &hdr),
                                        return );
                                    pdn->qos.arp.priority_level = 
                                        hdr->avp_value->i32;

                                }
                                else
                                    error++;

                                CHECK_FCT_DO( fd_avp_search_avp(avpch4,
                                    s6a_pre_emption_capability, &avpch5),
                                    return );
                                if (avpch5)
                                {
                                    CHECK_FCT_DO( fd_msg_avp_hdr(avpch5, &hdr),
                                        return );
                                    pdn->qos.arp.pre_emption_capability =
                                        hdr->avp_value->i32;
                                }
                                else
                                    error++;

                                CHECK_FCT_DO( fd_avp_search_avp(avpch4,
                                    s6a_pre_emption_vulnerability, &avpch5),
                                    return );
                                if (avpch5)
                                {
                                    CHECK_FCT_DO( fd_msg_avp_hdr(avpch5, &hdr),
                                        return );
                                    pdn->qos.arp.pre_emption_vulnerability =
                                        hdr->avp_value->i32;
                                }
                                else
                                    error++;

                            }
                            else
                                error++;
                        }
                        else
                            error++;

                        CHECK_FCT_DO( fd_avp_search_avp(avpch2,
                                fd_mip6_agent_info, &avpch3), return );
                        if (avpch3)
                        {
                            CHECK_FCT_DO( fd_avp_search_avp(avpch3,
                                fd_mip_home_agent_address, &avpch4), return );
                            if (avpch4)
                            {
                                struct sockaddr_storage ss;
                                struct sockaddr_in *sin;

                                CHECK_FCT_DO(
                                    fd_msg_avp_hdr(avpch4, &hdr), return );
                                CHECK_FCT_DO(
                                    fd_msg_avp_value_interpret(avpch4, &ss),
                                    return);
                                sin = (struct sockaddr_in *)&ss;
                                d_assert(sin, return, "Null param");
                                if (sin->sin_family == AF_INET)
                                {
                                    pdn->pgw.ipv4_addr = sin->sin_addr.s_addr;
                                }
                                else
                                {
                                    d_error("Not implemented(%d)",
                                            sin->sin_family);
                                }
                            }
                            else
                                error++;

                        }

                        CHECK_FCT_DO( fd_avp_search_avp(avpch2, s6a_ambr, 
                                &avpch3), return );
                        if (avpch3)
                        {
                            CHECK_FCT_DO( fd_avp_search_avp(avpch3,
                                s6a_max_bandwidth_ul, &avpch4), return );
                            if (avpch4)
                            {
                                CHECK_FCT_DO(
                                    fd_msg_avp_hdr(avpch4, &hdr), return );
                                pdn->ambr.uplink = hdr->avp_value->u32;
                            }
                            else
                                error++;

                            CHECK_FCT_DO( fd_avp_search_avp(avpch3,
                                s6a_max_bandwidth_dl, &avpch4), return );
                            if (avpch4)
                            {
                                CHECK_FCT_DO(
                                    fd_msg_avp_hdr(avpch4, &hdr), return );
                                pdn->ambr.downlink = hdr->avp_value->u32;
                            }
                            else
                                error++;
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
            error++;
    }
    else
        error++;

    CHECK_FCT_DO(
        fd_msg_search_avp(*msg, s6a_subscribed_rau_tau_timer, &avp), return );
    if (avp)
    {
        CHECK_FCT_DO( fd_msg_avp_hdr(avp, &hdr), return );
        subscription_data->subscribed_rau_tau_timer = hdr->avp_value->i32;
    }
    else
        error++;
    
    event_set(&e, MME_EVT_S6A_MESSAGE);
    event_set_param1(&e, (c_uintptr_t)mme_ue->index);
    event_set_param2(&e, (c_uintptr_t)s6abuf);
    mme_event_send(&e);

    /* Free the message */
    CHECK_POSIX_DO( pthread_mutex_lock(&fd_logger_self()->stats_lock), );
    dur = ((ts.tv_sec - mi->ts.tv_sec) * 1000000) + 
        ((ts.tv_nsec - mi->ts.tv_nsec) / 1000);
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

    CHECK_POSIX_DO( pthread_mutex_unlock(&fd_logger_self()->stats_lock), );
    
    /* Display how long it took */
    if (ts.tv_nsec > mi->ts.tv_nsec)
        d_trace(3, "in %d.%06ld sec\n", 
                (int)(ts.tv_sec - mi->ts.tv_sec),
                (long)(ts.tv_nsec - mi->ts.tv_nsec) / 1000);
    else
        d_trace(3, "in %d.%06ld sec\n", 
                (int)(ts.tv_sec + 1 - mi->ts.tv_sec),
                (long)(1000000000 + ts.tv_nsec - mi->ts.tv_nsec) / 1000);
    
    CHECK_FCT_DO( fd_msg_free(*msg), return );
    *msg = NULL;

    pool_free_node(&mme_s6a_sess_pool, mi);

    return;
}


int mme_fd_init(void)
{
    pool_init(&mme_s6a_sess_pool, MAX_NUM_SESSION_STATE);

    CHECK_FCT( fd_init(FD_MODE_CLIENT, mme_self()->fd_conf_path) );

	/* Install objects definitions for this application */
	CHECK_FCT( s6a_dict_init() );

	CHECK_FCT( fd_sess_handler_create(&mme_s6a_reg, (void *)free, NULL, NULL) );

	/* Advertise the support for the application in the peer */
	CHECK_FCT( fd_disp_app_support(s6a_application, fd_vendor, 1, 0) );
	
	return 0;
}

void mme_fd_final(void)
{
	CHECK_FCT_DO( fd_sess_handler_destroy(&mme_s6a_reg, NULL), );

    fd_final();

    if (pool_size(&mme_s6a_sess_pool) != pool_avail(&mme_s6a_sess_pool))
        d_error("%d not freed in mme_s6a_sess_pool[%d] of S6A-SM",
            pool_size(&mme_s6a_sess_pool) - pool_avail(&mme_s6a_sess_pool),
            pool_size(&mme_s6a_sess_pool));

    d_trace(3, "%d not freed in mme_s6a_sess_pool[%d] of S6A-SM\n",
            pool_size(&mme_s6a_sess_pool) - pool_avail(&mme_s6a_sess_pool),
            pool_size(&mme_s6a_sess_pool));

    pool_final(&mme_s6a_sess_pool);
}
