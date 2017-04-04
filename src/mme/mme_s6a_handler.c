#define TRACE_MODULE _mme_s6a_handler

#include "core_debug.h"
#include "core_pool.h"

#include "nas_message.h"
#include "s6a_lib.h"

#include "mme_event.h"

#include "mme_s6a_handler.h"

#define SIZE_OF_SESS_STATE_POOL 32

static struct session_handler *mme_s6a_reg = NULL;

struct sess_state {
    ue_ctx_t *ue;
    struct timespec ts; /* Time of sending the message */
};

pool_declare(sess_state_pool, struct sess_state, SIZE_OF_SESS_STATE_POOL);

static void mme_s6a_aia_cb(void *data, struct msg **msg);

/* Cb called when an answer is received */
int mme_s6a_send_auth_info_req(ue_ctx_t *ue, plmn_id_t *plmn_id)
{
    struct msg *req = NULL;
    struct avp *avp;
    struct avp *avpch;
    union avp_value val;
    struct sess_state *mi = NULL, *svg;
    struct session *sess = NULL;

    d_assert(ue, return -1, "Null Param");
    
    /* Create the random value to store with the session */
    pool_alloc_node(&sess_state_pool, &mi);
    d_assert(mi, return -1, "malloc failed: %s", strerror(errno));
    
    mi->ue = ue;
    
    /* Create the request */
    d_assert(fd_msg_new(s6a_cmd_air, MSGFL_ALLOC_ETEID, &req) == 0, 
            pool_free_node(&sess_state_pool, mi); return -1,);
    
    /* Create a new session */
    #define S6A_APP_SID_OPT  "app_s6a"
    d_assert(fd_msg_new_session(req, (os0_t)S6A_APP_SID_OPT, 
            CONSTSTRLEN(S6A_APP_SID_OPT)) == 0, goto out,);
    d_assert(fd_msg_sess_get(fd_g_config->cnf_dict, req, &sess, NULL) == 0, 
            goto out, );

    /* Set the Auth-Session-Statee AVP */
    d_assert(fd_msg_avp_new(s6a_auth_session_state, 0, &avp) == 0, goto out,);
    val.i32 = 1;
    d_assert(fd_msg_avp_setvalue(avp, &val) == 0, goto out,);
    d_assert(fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp) == 0, goto out,);

    /* Set Origin-Host & Origin-Realm */
    d_assert(fd_msg_add_origin(req, 0) == 0, goto out, );
    
    /* Set the Destination-Realm AVP */
    d_assert(fd_msg_avp_new(s6a_destination_realm, 0, &avp) == 0, goto out,);
    val.os.data = (unsigned char *)(fd_g_config->cnf_diamrlm);
    val.os.len  = strlen(fd_g_config->cnf_diamrlm);
    d_assert(fd_msg_avp_setvalue(avp, &val) == 0, goto out, );
    d_assert(fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp) == 0, goto out, );
    
    /* Set the User-Name AVP if needed*/
    d_assert(fd_msg_avp_new(s6a_user_name, 0, &avp) == 0, goto out,);
    val.os.data = ue->imsi;
    val.os.len  = ue->imsi_len;
    d_assert(fd_msg_avp_setvalue(avp, &val) == 0, goto out, );
    d_assert(fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp) == 0, goto out,);

    /* Add the Authentication-Info */
    d_assert(fd_msg_avp_new(s6a_req_eutran_auth_info, 0, &avp) == 0, goto out,);
    d_assert(fd_msg_avp_new(s6a_number_of_requested_vectors, 0, 
                &avpch) == 0, goto out,);
    val.u32 = 1;
    d_assert(fd_msg_avp_setvalue (avpch, &val) == 0, goto out,);
    d_assert(fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch) == 0, goto out,);

    d_assert(fd_msg_avp_new(s6a_immediate_response_preferred, 0, 
                &avpch) == 0, goto out,);
    val.u32 = 1;
    d_assert(fd_msg_avp_setvalue(avpch, &val) == 0, goto out,);
    d_assert(fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avpch) == 0, goto out,);

    d_assert(fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp) == 0, goto out,);

    /* Set the Visited-PLMN-Id AVP if needed*/
    d_assert(fd_msg_avp_new(s6a_visited_plmn_id, 0, &avp) == 0, goto out,);
    val.os.data = (c_uint8_t *)plmn_id;
    val.os.len  = PLMN_ID_LEN;
    d_assert(fd_msg_avp_setvalue(avp, &val) == 0, goto out,);
    d_assert(fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp) == 0, goto out,);
    
    d_assert(clock_gettime(CLOCK_REALTIME, &mi->ts) == 0, goto out,);
    
    /* Keep a pointer to the session data for debug purpose, 
     * in real life we would not need it */
    svg = mi;
    
    /* Store this value in the session */
    d_assert(fd_sess_state_store(mme_s6a_reg, sess, &mi) == 0, goto out,); 
    
    /* Send the request */
    d_assert(fd_msg_send(&req, mme_s6a_aia_cb, svg) == 0, goto out,);

    /* Increment the counter */
    d_assert(pthread_mutex_lock(&s6a_config->stats_lock) == 0,,);
    s6a_config->stats.nb_sent++;
    d_assert(pthread_mutex_unlock(&s6a_config->stats_lock) == 0,, );

    d_assert(ue->imsi, return 0,);
    d_info("[S6A] Authentication-Information-Request : UE[%s] --> HSS", 
            ue->imsi);

    return 0;

out:
    d_assert(fd_msg_free(req) == 0,,);
    pool_free_node(&sess_state_pool, mi);

    return -1;
}

static void mme_s6a_aia_cb(void *data, struct msg **msg)
{
    struct sess_state *mi = NULL;
    struct timespec ts;
    struct session *sess;
    struct avp *avp;
    struct avp *avpch1, *avpch2;
    struct avp_hdr *hdr;
    unsigned long dur;
    int error = 0;
    int new;

    ue_ctx_t *ue = NULL;
    nas_message_t message;
    pkbuf_t *sendbuf = NULL;
    event_t e;
    nas_authentication_request_t *authentication_request = 
        &message.authentication_request;
    
    CHECK_SYS_DO(clock_gettime(CLOCK_REALTIME, &ts), return);

    /* Search the session, retrieve its data */
    d_assert(fd_msg_sess_get(fd_g_config->cnf_dict, *msg, &sess, &new) == 0 && 
            new == 0, return,);
    
    d_assert(fd_sess_state_retrieve(mme_s6a_reg, sess, &mi) == 0 &&
            mi && (void *)mi == data, fd_msg_free(*msg); *msg = NULL; return,);

    ue = mi->ue;
    d_assert(ue, error++; goto out,);

    d_assert(ue->imsi, error++; goto out,);
    d_info("[S6A] Authentication-Information-Response : UE[%s] <-- HSS", 
            ue->imsi);
    
    /* Value of Result Code */
    d_assert(fd_msg_search_avp(*msg, s6a_result_code, &avp) == 0 && avp, 
            error++; goto out,);
    d_assert(fd_msg_avp_hdr(avp, &hdr) == 0 && hdr, error++; goto out,);
    if (hdr->avp_value->i32 != ER_DIAMETER_SUCCESS)
    {
        d_error("ERROR DIAMETER Result Code(%d)", hdr->avp_value->i32);
        error++;
        goto out;
    }
    
    memset(&message, 0, sizeof(message));
    message.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.h.message_type = NAS_AUTHENTICATION_REQUEST;

    d_assert(fd_msg_search_avp(*msg, s6a_authentication_info, &avp) == 0 && avp, 
            error++; goto out,);
    d_assert(fd_msg_avp_hdr(avp, &hdr) == 0 && hdr, error++; goto out,);
    d_assert(fd_avp_search_avp(avp, s6a_e_utran_vector, &avpch1) == 0 && avp, 
            error++; goto out,);
    d_assert(fd_msg_avp_hdr(avpch1, &hdr) == 0 && hdr, error++; goto out,);

    d_assert(fd_avp_search_avp(avpch1, s6a_xres, &avpch2) == 0 && avp, 
            error++; goto out,);
    d_assert(fd_msg_avp_hdr(avpch2, &hdr) == 0 && hdr, error++; goto out,);
    memcpy(ue->xres, hdr->avp_value->os.data, hdr->avp_value->os.len);
    ue->xres_len = hdr->avp_value->os.len;

    d_assert(fd_avp_search_avp(avpch1, s6a_kasme, &avpch2) == 0 && avp, 
            error++; goto out,);
    d_assert(fd_msg_avp_hdr(avpch2, &hdr) == 0 && hdr, error++; goto out,);
    memcpy(ue->kasme, hdr->avp_value->os.data, hdr->avp_value->os.len);

    d_assert(fd_avp_search_avp(avpch1, s6a_rand, &avpch2) == 0 && avp, 
            error++; goto out,);
    d_assert(fd_msg_avp_hdr(avpch2, &hdr) == 0 && hdr, error++; goto out,);
    memcpy(authentication_request->authentication_parameter_rand.rand,
            hdr->avp_value->os.data, hdr->avp_value->os.len);

    d_assert(fd_avp_search_avp(avpch1, s6a_autn, &avpch2) == 0 && avp, 
            error++; goto out,);
    d_assert(fd_msg_avp_hdr(avpch2, &hdr) == 0 && hdr, error++; goto out,);
    authentication_request->authentication_parameter_autn.length = 
        hdr->avp_value->os.len;
    memcpy(authentication_request->authentication_parameter_autn.autn,
            hdr->avp_value->os.data, hdr->avp_value->os.len);

    d_assert(nas_plain_encode(&sendbuf, &message) == CORE_OK && sendbuf, 
            error++; goto out,);

    event_set(&e, EVT_MSG_UE_EMM);
    event_set_param1(&e, (c_uintptr_t)ue);
    event_set_param2(&e, (c_uintptr_t)sendbuf);
    if (mme_event_send(&e) != CORE_OK)
    {
        d_error("mme_event_send failed");
        pkbuf_free(sendbuf);
        error++;
    }
out:
    /* Free the message */
    d_assert(pthread_mutex_lock(&s6a_config->stats_lock) == 0,,);
    dur = ((ts.tv_sec - mi->ts.tv_sec) * 1000000) + 
        ((ts.tv_nsec - mi->ts.tv_nsec) / 1000);
    if (s6a_config->stats.nb_recv)
    {
        /* Ponderate in the avg */
        s6a_config->stats.avg = (s6a_config->stats.avg * 
            s6a_config->stats.nb_recv + dur) / (s6a_config->stats.nb_recv + 1);
        /* Min, max */
        if (dur < s6a_config->stats.shortest)
            s6a_config->stats.shortest = dur;
        if (dur > s6a_config->stats.longest)
            s6a_config->stats.longest = dur;
    }
    else
    {
        s6a_config->stats.shortest = dur;
        s6a_config->stats.longest = dur;
        s6a_config->stats.avg = dur;
    }
    if (error)
        s6a_config->stats.nb_errs++;
    else 
        s6a_config->stats.nb_recv++;
    d_assert(pthread_mutex_unlock(&s6a_config->stats_lock) == 0,,);
    
    /* Display how long it took */
    if (ts.tv_nsec > mi->ts.tv_nsec)
        d_trace(1, "in %d.%06ld sec\n", 
                (int)(ts.tv_sec - mi->ts.tv_sec),
                (long)(ts.tv_nsec - mi->ts.tv_nsec) / 1000);
    else
        d_trace(1, "in %d.%06ld sec\n", 
                (int)(ts.tv_sec + 1 - mi->ts.tv_sec),
                (long)(1000000000 + ts.tv_nsec - mi->ts.tv_nsec) / 1000);
    
    d_assert(fd_msg_free(*msg) == 0,,);
    *msg = NULL;
    pool_free_node(&sess_state_pool, mi);
}

status_t mme_s6a_init(void)
{
    status_t rv;

    rv = s6a_init(MODE_MME);
    if (rv != CORE_OK) return rv;

    pool_init(&sess_state_pool, SIZE_OF_SESS_STATE_POOL);

	d_assert(fd_sess_handler_create(&mme_s6a_reg, 
            (void *)free, NULL, NULL) == 0, return -1,);
	
	return CORE_OK;
}

void mme_s6a_final(void)
{
	d_assert(fd_sess_handler_destroy(&mme_s6a_reg, NULL) == 0,,);

    d_print("%d not freed in sess_state_pool[%d] of S6A-SM\n",
            pool_size(&sess_state_pool) - pool_avail(&sess_state_pool),
            pool_size(&sess_state_pool));

    pool_final(&sess_state_pool);

    s6a_final();
}
