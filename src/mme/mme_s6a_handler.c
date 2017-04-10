#define TRACE_MODULE _mme_s6a_handler

#include "core_debug.h"
#include "core_pool.h"

#include "nas_message.h"
#include "s6a_lib.h"

#include "mme_event.h"

#include "mme_s6a_handler.h"

#define MAX_NUM_SESSION_STATE 32

static struct session_handler *mme_s6a_reg = NULL;

struct sess_state {
    mme_ue_t *ue;
    struct timespec ts; /* Time of sending the message */
};

pool_declare(sess_state_pool, struct sess_state, MAX_NUM_SESSION_STATE);

/* MME received Authentication Information Answer from HSS */
static void mme_s6a_aia_cb(void *data, struct msg **msg)
{
    struct sess_state *mi = NULL;
    struct timespec ts;
    struct session *sess;
    struct avp *avp;
    struct avp *avp_e_utran_vector, *avp_xres, *avp_kasme, *avp_rand, *avp_autn;
    struct avp_hdr *hdr;
    unsigned long dur;
    int error = 0;
    int new;

    mme_ue_t *ue = NULL;
    nas_message_t message;
    pkbuf_t *sendbuf = NULL;
    event_t e;
    nas_authentication_request_t *authentication_request = 
        &message.emm.authentication_request;
    
    CHECK_SYS_DO(clock_gettime(CLOCK_REALTIME, &ts), return);

    /* Search the session, retrieve its data */
    d_assert(fd_msg_sess_get(fd_g_config->cnf_dict, *msg, &sess, &new) == 0 && 
            new == 0, return,);
    
    d_assert(fd_sess_state_retrieve(mme_s6a_reg, sess, &mi) == 0 &&
            mi && (void *)mi == data, fd_msg_free(*msg); *msg = NULL; return,);

    ue = mi->ue;
    d_assert(ue, error++; goto out,);

    d_info("[S6A] Authentication-Information-Response : UE[%s] <-- HSS", 
            ue->imsi_bcd);
    
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
    message.emm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = NAS_AUTHENTICATION_REQUEST;

    d_assert(fd_msg_search_avp(*msg, s6a_authentication_info, &avp) == 0 && 
            avp, error++; goto out,);
    d_assert(fd_msg_avp_hdr(avp, &hdr) == 0 && hdr, error++; goto out,);
    d_assert(fd_avp_search_avp(avp, s6a_e_utran_vector, &avp_e_utran_vector) 
            == 0 && avp_e_utran_vector, error++; goto out,);
    d_assert(fd_msg_avp_hdr(avp_e_utran_vector, &hdr) == 0 && hdr, error++; 
            goto out,);

    d_assert(fd_avp_search_avp(avp_e_utran_vector, s6a_xres, &avp_xres) == 0 && 
            avp_xres, error++; goto out,);
    d_assert(fd_msg_avp_hdr(avp_xres, &hdr) == 0 && hdr, error++; goto out,);
    memcpy(ue->xres, hdr->avp_value->os.data, hdr->avp_value->os.len);
    ue->xres_len = hdr->avp_value->os.len;

    d_assert(
        fd_avp_search_avp(avp_e_utran_vector, s6a_kasme, &avp_kasme) == 0 && 
        avp_kasme, error++; goto out,);
    d_assert(fd_msg_avp_hdr(avp_kasme, &hdr) == 0 && hdr, error++; goto out,);
    memcpy(ue->kasme, hdr->avp_value->os.data, hdr->avp_value->os.len);

    d_assert(fd_avp_search_avp(avp_e_utran_vector, s6a_rand, &avp_rand) == 0 && 
            avp_rand, error++; goto out,);
    d_assert(fd_msg_avp_hdr(avp_rand, &hdr) == 0 && hdr, error++; goto out,);
    memcpy(authentication_request->authentication_parameter_rand.rand,
            hdr->avp_value->os.data, hdr->avp_value->os.len);

    d_assert(fd_avp_search_avp(avp_e_utran_vector, s6a_autn, &avp_autn) == 0 && 
            avp_autn, error++; goto out,);
    d_assert(fd_msg_avp_hdr(avp_autn, &hdr) == 0 && hdr, error++; goto out,);
    authentication_request->authentication_parameter_autn.length = 
        hdr->avp_value->os.len;
    memcpy(authentication_request->authentication_parameter_autn.autn,
            hdr->avp_value->os.data, hdr->avp_value->os.len);

    d_assert(nas_plain_encode(&sendbuf, &message) == CORE_OK && sendbuf, 
            error++; goto out,);

    event_set(&e, EVT_MSG_MME_EMM);
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

/* MME Sends Authentication Information Request to HSS */
int mme_s6a_send_air(mme_ue_t *ue)
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

    /* Set the Auth-Session-State AVP */
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
    
    /* Set the User-Name AVP */
    d_assert(fd_msg_avp_new(s6a_user_name, 0, &avp) == 0, goto out,);
    val.os.data = (c_uint8_t *)ue->imsi_bcd;
    val.os.len  = strlen(ue->imsi_bcd);
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

    /* Set the Visited-PLMN-Id AVP */
    d_assert(fd_msg_avp_new(s6a_visited_plmn_id, 0, &avp) == 0, goto out,);
    val.os.data = (c_uint8_t *)&ue->visited_plmn_id;
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

    d_info("[S6A] Authentication-Information-Request : UE[%s] --> HSS", 
            ue->imsi_bcd);

    return 0;

out:
    d_assert(fd_msg_free(req) == 0,,);
    pool_free_node(&sess_state_pool, mi);

    return -1;
}

/* MME received Update Location Answer from HSS */
static void mme_s6a_ula_cb(void *data, struct msg **msg)
{
    struct sess_state *mi = NULL;
    struct timespec ts;
    struct session *sess;
    struct avp *avp;
    struct avp *avpch1, *avpch2, *avpch3, *avpch4, *avpch5;
    struct avp_hdr *hdr;
    unsigned long dur;
    int error = 0;
    int new;

    mme_ue_t *ue = NULL;
    pdn_t *pdn = NULL;
    c_uint8_t pdn_added = 0;

    nas_message_t message;
#if 0
    pkbuf_t *sendbuf = NULL;
    event_t e;
    nas_authentication_request_t *authentication_request = 
        &message.emm.authentication_request;
#endif
    
    CHECK_SYS_DO(clock_gettime(CLOCK_REALTIME, &ts), return);

    /* Search the session, retrieve its data */
    d_assert(fd_msg_sess_get(fd_g_config->cnf_dict, *msg, &sess, &new) == 0 && 
            new == 0, return,);
    
    d_assert(fd_sess_state_retrieve(mme_s6a_reg, sess, &mi) == 0 &&
            mi && (void *)mi == data, fd_msg_free(*msg); *msg = NULL; return,);

    ue = mi->ue;
    d_assert(ue, error++; goto out,);

    d_info("[S6A] Authentication-Information-Response : UE[%s] <-- HSS", 
            ue->imsi_bcd);
    
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

    d_assert(fd_msg_search_avp(*msg, s6a_ula_flags, &avp) == 0 
            && avp, error++; goto out,);
    d_assert(fd_msg_avp_hdr(avp, &hdr) == 0 && hdr, error++; goto out,);
    ue->ula_flags = hdr->avp_value->i32;

    d_assert(fd_msg_search_avp(*msg, s6a_subscription_data, &avp) == 0 && avp, 
            error++; goto out,);

    d_assert(fd_avp_search_avp(avp, s6a_msisdn, &avpch1) == 0 && avpch1, 
            error++; goto out,);
    d_assert(fd_msg_avp_hdr(avpch1, &hdr) == 0 && hdr, error++; goto out,);
    ue->msisdn_len = hdr->avp_value->os.len;
    memcpy(ue->msisdn, hdr->avp_value->os.data, ue->msisdn_len);

    d_assert(fd_avp_search_avp(avp, s6a_ambr, &avpch1) == 0 && avpch1, 
            error++; goto out,);
    d_assert(fd_avp_search_avp(avpch1, s6a_max_bandwidth_ul, &avpch2) == 0 && 
            avpch2, error++; goto out,);
    d_assert(fd_msg_avp_hdr(avpch2, &hdr) == 0 && hdr, error++; goto out,);
    ue->max_bandwidth_ul = hdr->avp_value->i32;
    d_assert(fd_avp_search_avp(avpch1, s6a_max_bandwidth_dl, &avpch2) == 0 && 
            avpch2, error++; goto out,);
    d_assert(fd_msg_avp_hdr(avpch2, &hdr) == 0 && hdr, error++; goto out,);
    ue->max_bandwidth_dl = hdr->avp_value->i32;

    d_assert(fd_avp_search_avp(avp, s6a_apn_configuration_profile, &avpch1) 
            == 0 && avpch1, error++; goto out,);

    d_assert(fd_msg_browse(avpch1, MSG_BRW_FIRST_CHILD, &avpch2, NULL) == 0 &&
            avpch2, error++;; goto out,);
    while(avpch2) 
    {
        d_assert(fd_msg_avp_hdr(avpch2, &hdr) == 0 && hdr, error++; goto out,);
        switch(hdr->avp_code)
        {
            case AVP_CODE_CONTEXT_IDENTIFIER:
                break;
            case AVP_CODE_ALL_APN_CONFIG_INC_IND:
                break;
            case AVP_CODE_APN_CONFIGURATION:
                d_assert(fd_avp_search_avp(avpch2, s6a_context_identifier, 
                        &avpch3) == 0 && avpch3, error++; goto out,);
                d_assert(fd_msg_avp_hdr(avpch3, &hdr) == 0 && hdr, 
                        error++; goto out,);
                pdn = mme_pdn_find_by_id(hdr->avp_value->i32);
                if (!pdn)
                {
                    pdn = mme_pdn_add(hdr->avp_value->i32);
                    pdn_added = 1;
                }
                d_assert(pdn, error++; goto out,);

                d_assert(fd_avp_search_avp(avpch2, s6a_pdn_type, 
                        &avpch3) == 0 && avpch3, error++; goto out,);
                d_assert(fd_msg_avp_hdr(avpch3, &hdr) == 0 && hdr,
                        error++; goto out,);
                pdn->type = hdr->avp_value->i32;

                d_assert(fd_avp_search_avp(avpch2, s6a_service_selection, 
                        &avpch3) == 0 && avpch3, error++; goto out,);
                d_assert(fd_msg_avp_hdr(avpch3, &hdr) == 0 && hdr,
                        error++; goto out,);

                memcpy(pdn->apn, 
                        hdr->avp_value->os.data, hdr->avp_value->os.len);
                pdn->apn[hdr->avp_value->os.len] = 0;

                if (pdn_added)
                    d_info("PDN(id:%d, type:%s, APN:%s) Added", 
                        pdn->id, 
                        pdn->type == PDN_TYPE_IPV4 ? "IPv4" :
                        pdn->type == PDN_TYPE_IPV6 ? "IPv6" :
                        pdn->type == PDN_TYPE_IPV4_AND_IPV6 ? "IPv4_AND_IPv6" :
                        pdn->type == PDN_TYPE_IPV4_OR_IPV6 ? "IPv4_OR_IPv6" :
                            "Unknown Type", 
                        pdn->apn);

                d_assert(fd_avp_search_avp(avpch2, 
                        s6a_eps_subscribed_qos_profile, &avpch3) == 0 && 
                        avpch3, error++; goto out,);

                d_assert(fd_avp_search_avp(avpch3, s6a_qos_class_identifier, 
                        &avpch4) == 0 && avpch4, error++; goto out,);
                d_assert(fd_msg_avp_hdr(avpch4, &hdr) == 0 && hdr,
                        error++; goto out,);
                pdn->qci = hdr->avp_value->i32;

                d_assert(fd_avp_search_avp(avpch3, 
                        s6a_allocation_retention_priority, &avpch4) == 0 && 
                        avpch4, error++; goto out,);

                d_assert(fd_avp_search_avp(avpch4, s6a_priority_level, 
                        &avpch5) == 0 && avpch5, error++; goto out,);
                d_assert(fd_msg_avp_hdr(avpch5, &hdr) == 0 && hdr,
                        error++; goto out,);
                pdn->priority_level = hdr->avp_value->i32;

                d_assert(fd_avp_search_avp(avpch4, s6a_pre_emption_capability, 
                        &avpch5) == 0 && avpch5, error++; goto out,);
                d_assert(fd_msg_avp_hdr(avpch5, &hdr) == 0 && hdr,
                        error++; goto out,);
                pdn->pre_emption_capability = hdr->avp_value->i32;

                d_assert(fd_avp_search_avp(avpch4,
                        s6a_pre_emption_vulnerability, &avpch5) == 0 && 
                        avpch5, error++; goto out,);
                d_assert(fd_msg_avp_hdr(avpch5, &hdr) == 0 && hdr,
                        error++; goto out,);
                pdn->pre_emption_vulnerability = hdr->avp_value->i32;

                d_assert(fd_avp_search_avp(avpch2, s6a_ambr, 
                        &avpch3) == 0 && avpch3, error++; goto out,);
                d_assert(fd_avp_search_avp(avpch3, s6a_max_bandwidth_ul,
                        &avpch4) == 0 && avpch4, error++; goto out,);
                d_assert(fd_msg_avp_hdr(avpch4, &hdr) == 0 && hdr,
                        error++; goto out,);
                pdn->max_bandwidth_ul = hdr->avp_value->i32;
                d_assert(fd_avp_search_avp(avpch3,
                        s6a_max_bandwidth_dl, &avpch4) == 0 && avpch4,
                        error++; goto out,);
                d_assert(fd_msg_avp_hdr(avpch4, &hdr) == 0 && hdr,
                        error++; goto out,);
                pdn->max_bandwidth_dl = hdr->avp_value->i32;
                break;
        }

        fd_msg_browse(avpch2, MSG_BRW_NEXT, &avpch2, NULL);
    }

    d_assert(fd_msg_search_avp(*msg, s6a_subscribed_rau_tau_timer, &avp) == 0 
            && avp, error++; goto out,);
    d_assert(fd_msg_avp_hdr(avp, &hdr) == 0 && hdr, error++; goto out,);
    ue->subscribed_rau_tau_timer = hdr->avp_value->i32;
    
    memset(&message, 0, sizeof(message));
    message.h.security_header_type = 
       NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;

#if 0
    message.esm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_ESM;
    message.esm.h.procedure_transaction_identity = 33;
    message.esm.h.message_type = NAS_ESM_INFORMATION_REQUEST;
#endif

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

/* MME Sends Update Location Request to HSS */
int mme_s6a_send_ulr(mme_ue_t *ue)
{
    struct msg *req = NULL;
    struct avp *avp;
    union avp_value val;
    struct sess_state *mi = NULL, *svg;
    struct session *sess = NULL;

    d_assert(ue, return -1, "Null Param");
    
    /* Create the random value to store with the session */
    pool_alloc_node(&sess_state_pool, &mi);
    d_assert(mi, return -1, "malloc failed: %s", strerror(errno));
    
    mi->ue = ue;
    
    /* Create the request */
    d_assert(fd_msg_new(s6a_cmd_ulr, MSGFL_ALLOC_ETEID, &req) == 0, 
            pool_free_node(&sess_state_pool, mi); return -1,);
    
    /* Create a new session */
    #define S6A_APP_SID_OPT  "app_s6a"
    d_assert(fd_msg_new_session(req, (os0_t)S6A_APP_SID_OPT, 
            CONSTSTRLEN(S6A_APP_SID_OPT)) == 0, goto out,);
    d_assert(fd_msg_sess_get(fd_g_config->cnf_dict, req, &sess, NULL) == 0, 
            goto out, );

    /* Set the Auth-Session-State AVP */
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
    
    /* Set the User-Name AVP */
    d_assert(fd_msg_avp_new(s6a_user_name, 0, &avp) == 0, goto out,);
    val.os.data = (c_uint8_t *)ue->imsi_bcd;
    val.os.len  = strlen(ue->imsi_bcd);
    d_assert(fd_msg_avp_setvalue(avp, &val) == 0, goto out, );
    d_assert(fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp) == 0, goto out,);

    /* Set the RAT-Type */
    d_assert(fd_msg_avp_new(s6a_rat_type, 0, &avp) == 0, goto out,);
    val.u32 = S6A_RAT_TYPE_EUTRAN;
    d_assert(fd_msg_avp_setvalue(avp, &val) == 0, goto out,);
    d_assert(fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp) == 0, goto out,);

    /* Set the ULR-Flags */
    d_assert(fd_msg_avp_new(s6a_ulr_flags, 0, &avp) == 0, goto out,);
    val.u32 = S6A_ULR_S6A_S6D_INDICATOR;
    d_assert(fd_msg_avp_setvalue(avp, &val) == 0, goto out,);
    d_assert(fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp) == 0, goto out,);

    /* Set the Visited-PLMN-Id */
    d_assert(fd_msg_avp_new(s6a_visited_plmn_id, 0, &avp) == 0, goto out,);
    val.os.data = (c_uint8_t *)&ue->visited_plmn_id;
    val.os.len  = PLMN_ID_LEN;
    d_assert(fd_msg_avp_setvalue(avp, &val) == 0, goto out,);
    d_assert(fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp) == 0, goto out,);

    /* Set the UE-SRVCC Capability */
    d_assert(fd_msg_avp_new(s6a_ue_srvcc_cap, 0, &avp) == 0, goto out,);
    val.u32 = S6A_UE_SRVCC_NOT_SUPPORTED;
    d_assert(fd_msg_avp_setvalue(avp, &val) == 0, goto out,);
    d_assert(fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp) == 0, goto out,);

    d_assert(clock_gettime(CLOCK_REALTIME, &mi->ts) == 0, goto out,);
    
    /* Keep a pointer to the session data for debug purpose, 
     * in real life we would not need it */
    svg = mi;
    
    /* Store this value in the session */
    d_assert(fd_sess_state_store(mme_s6a_reg, sess, &mi) == 0, goto out,); 
    
    /* Send the request */
    d_assert(fd_msg_send(&req, mme_s6a_ula_cb, svg) == 0, goto out,);

    /* Increment the counter */
    d_assert(pthread_mutex_lock(&s6a_config->stats_lock) == 0,,);
    s6a_config->stats.nb_sent++;
    d_assert(pthread_mutex_unlock(&s6a_config->stats_lock) == 0,, );

    d_info("[S6A] Update-Location-Request : UE[%s] --> HSS", 
            ue->imsi_bcd);

    return 0;

out:
    d_assert(fd_msg_free(req) == 0,,);
    pool_free_node(&sess_state_pool, mi);

    return -1;
}

status_t mme_s6a_init(void)
{
    status_t rv;

    rv = s6a_init(MODE_MME);
    if (rv != CORE_OK) return rv;

    pool_init(&sess_state_pool, MAX_NUM_SESSION_STATE);

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
