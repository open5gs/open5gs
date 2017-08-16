#define TRACE_MODULE _mme_s6a_handler

#include "core_debug.h"
#include "core_pool.h"

#include "fd_lib.h"
#include "s6a_lib.h"

#include "mme_event.h"
#include "mme_s6a_handler.h"

#define MAX_NUM_SESSION_STATE 32

static struct session_handler *mme_s6a_reg = NULL;

struct sess_state {
    mme_ue_t *mme_ue;
    struct timespec ts; /* Time of sending the message */
};

pool_declare(mme_s6a_sess_pool, struct sess_state, MAX_NUM_SESSION_STATE);

/* MME received Authentication Information Answer from HSS */
static void mme_s6a_aia_cb(void *data, struct msg **msg)
{
    struct sess_state *mi = NULL;
    struct timespec ts;
    struct session *sess;
    struct avp *avp, *avpch;
    struct avp *avp_e_utran_vector, *avp_xres, *avp_kasme, *avp_rand, *avp_autn;
    struct avp_hdr *hdr;
    unsigned long dur;
    int error = 0;
    c_uint32_t result_code = 0;
    int new;

    event_t e;
    mme_ue_t *mme_ue = NULL;
    
    CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &ts), return );

    /* Search the session, retrieve its data */
    CHECK_FCT_DO( fd_msg_sess_get(fd_g_config->cnf_dict, *msg, &sess, &new),
            return );
    d_assert(new == 0, return, );
    
    CHECK_FCT_DO( fd_sess_state_retrieve(mme_s6a_reg, sess, &mi), return );
    d_assert(mi && (void *)mi == data, return, );

    mme_ue = mi->mme_ue;
    d_assert(mme_ue, return, );

    d_trace(3, "[S6A] Authentication-Information-Response : UE[%s] <-- HSS\n", 
            mme_ue->imsi_bcd);
    
    /* Value of Result Code */
    CHECK_FCT_DO( fd_msg_search_avp(*msg, fd_result_code, &avp), return );
    if (avp)
    {
        CHECK_FCT_DO( fd_msg_avp_hdr(avp, &hdr), return);
        result_code = hdr->avp_value->i32;
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
                result_code = hdr->avp_value->i32;
                d_trace(3, "Experimental Result Code: %d\n", result_code);
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

    if (result_code != ER_DIAMETER_SUCCESS)
    {
        d_warn("ERROR DIAMETER Result Code(%d)", result_code);
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
        memcpy(mme_ue->xres, hdr->avp_value->os.data, hdr->avp_value->os.len);
        mme_ue->xres_len = hdr->avp_value->os.len;
    }
    else
        error++;

    CHECK_FCT_DO(
        fd_avp_search_avp(avp_e_utran_vector, s6a_kasme, &avp_kasme), return );
    if (avp)
    {
        CHECK_FCT_DO( fd_msg_avp_hdr(avp_kasme, &hdr), return );
        memcpy(mme_ue->kasme, hdr->avp_value->os.data, hdr->avp_value->os.len);
    }
    else
        error++;


    CHECK_FCT_DO(
        fd_avp_search_avp(avp_e_utran_vector, s6a_rand, &avp_rand), return );
    if (avp)
    {
        CHECK_FCT_DO( fd_msg_avp_hdr(avp_rand, &hdr), return );
        memcpy(mme_ue->rand, hdr->avp_value->os.data, hdr->avp_value->os.len);
    }
    else
        error++;

    CHECK_FCT_DO(
        fd_avp_search_avp(avp_e_utran_vector, s6a_autn, &avp_autn), return );
    if (avp)
    {
        CHECK_FCT_DO( fd_msg_avp_hdr(avp_autn, &hdr), return );
        memcpy(mme_ue->autn, hdr->avp_value->os.data, hdr->avp_value->os.len);
    }
    else
        error++;

out:
    event_set(&e, MME_EVT_EMM_UE_FROM_S6A);
    event_set_param1(&e, (c_uintptr_t)mme_ue->index);
    event_set_param2(&e, (c_uintptr_t)S6A_CMD_AUTHENTICATION_INFORMATION);
    event_set_param3(&e, (c_uintptr_t)result_code);
    mme_event_send(&e);

    /* Free the message */
    CHECK_POSIX_DO( pthread_mutex_lock(&fd_self()->stats_lock), );
    dur = ((ts.tv_sec - mi->ts.tv_sec) * 1000000) + 
        ((ts.tv_nsec - mi->ts.tv_nsec) / 1000);
    if (fd_self()->stats.nb_recv)
    {
        /* Ponderate in the avg */
        fd_self()->stats.avg = (fd_self()->stats.avg * 
            fd_self()->stats.nb_recv + dur) / (fd_self()->stats.nb_recv + 1);
        /* Min, max */
        if (dur < fd_self()->stats.shortest)
            fd_self()->stats.shortest = dur;
        if (dur > fd_self()->stats.longest)
            fd_self()->stats.longest = dur;
    }
    else
    {
        fd_self()->stats.shortest = dur;
        fd_self()->stats.longest = dur;
        fd_self()->stats.avg = dur;
    }
    if (error)
        fd_self()->stats.nb_errs++;
    else 
        fd_self()->stats.nb_recv++;

    CHECK_POSIX_DO( pthread_mutex_unlock(&fd_self()->stats_lock), );
    
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

/* MME Sends Authentication Information Request to HSS */
void mme_s6a_send_air(mme_ue_t *mme_ue)
{
    struct msg *req = NULL;
    struct avp *avp;
    struct avp *avpch;
    union avp_value val;
    struct sess_state *mi = NULL, *svg;
    struct session *sess = NULL;
    enb_ue_t *enb_ue = NULL;

    d_assert(mme_ue, return, "Null Param");
    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return, "Null Param");

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
    CHECK_FCT_DO( fd_msg_sess_get(fd_g_config->cnf_dict, req, &sess, NULL),
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
    
    CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &mi->ts), goto out );
    
    /* Keep a pointer to the session data for debug purpose, 
     * in real life we would not need it */
    svg = mi;
    
    /* Store this value in the session */
    CHECK_FCT_DO( fd_sess_state_store(mme_s6a_reg, sess, &mi), goto out );
    
    /* Send the request */
    CHECK_FCT_DO( fd_msg_send(&req, mme_s6a_aia_cb, svg), goto out );

    /* Increment the counter */
    CHECK_POSIX_DO( pthread_mutex_lock(&fd_self()->stats_lock), );
    fd_self()->stats.nb_sent++;
    CHECK_POSIX_DO( pthread_mutex_unlock(&fd_self()->stats_lock), );

    d_trace(3, "[S6A] Authentication-Information-Request : UE[%s] --> HSS\n", 
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
    struct session *sess;
    struct avp *avp, *avpch;
    struct avp *avpch1, *avpch2, *avpch3, *avpch4, *avpch5;
    struct avp_hdr *hdr;
    unsigned long dur;
    int error = 0;
    c_uint32_t result_code = 0;
    int new;

    event_t e;
    mme_ue_t *mme_ue = NULL;
    pdn_t *pdn = NULL;

    CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &ts), return );

    /* Search the session, retrieve its data */
    CHECK_FCT_DO( fd_msg_sess_get(fd_g_config->cnf_dict, *msg, &sess, &new),
            return );
    d_assert(new == 0, return, );
    
    CHECK_FCT_DO( fd_sess_state_retrieve(mme_s6a_reg, sess, &mi), return );
    d_assert(mi && (void *)mi == data, return, );

    mme_ue = mi->mme_ue;
    d_assert(mme_ue, return, );

    d_trace(3, "[S6A] Update-Location-Response : UE[%s] <-- HSS\n", 
            mme_ue->imsi_bcd);
    
    /* Value of Result Code */
    CHECK_FCT_DO( fd_msg_search_avp(*msg, fd_result_code, &avp), return );
    if (avp)
    {
        CHECK_FCT_DO( fd_msg_avp_hdr(avp, &hdr), return);
        result_code = hdr->avp_value->i32;
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
                result_code = hdr->avp_value->i32;
                d_trace(3, "Experimental Result Code: %d\n", result_code);
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
        mme_ue->ula_flags = hdr->avp_value->i32;
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
                mme_ue->max_bandwidth_ul = hdr->avp_value->i32;
            }
            else
                error++;

            CHECK_FCT_DO(fd_avp_search_avp(
                    avpch1, s6a_max_bandwidth_dl, &avpch2), return );
            if (avpch2)
            {
                CHECK_FCT_DO( fd_msg_avp_hdr(avpch2, &hdr), return );
                mme_ue->max_bandwidth_dl = hdr->avp_value->i32;
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
                        c_int8_t apn[MAX_APN_LEN];
                        CHECK_FCT_DO( fd_avp_search_avp(
                            avpch2, s6a_service_selection, &avpch3), return );
                        if (avpch3)
                        {
                            CHECK_FCT_DO( 
                                fd_msg_avp_hdr(avpch3, &hdr), return );
                            memcpy(apn, hdr->avp_value->os.data, 
                                    hdr->avp_value->os.len);
                            apn[hdr->avp_value->os.len] = 0;

                            pdn = mme_pdn_find_by_apn(mme_ue, apn);
                            if (!pdn)
                                pdn = mme_pdn_add(mme_ue, apn);
                            else
                                d_warn("Duplicate APN:[%s]", apn);
                        }
                        else
                            error++;

                        d_assert(pdn, return, );

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
                                    pdn->qos.priority_level = 
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
                                    pdn->qos.pre_emption_capability =
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
                                    pdn->qos.pre_emption_vulnerability =
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
                                pdn->qos.max_bandwidth_ul =
                                    hdr->avp_value->i32 / 1024; /* Kbps */
                            }
                            else
                                error++;

                            CHECK_FCT_DO( fd_avp_search_avp(avpch3,
                                s6a_max_bandwidth_dl, &avpch4), return );
                            if (avpch4)
                            {
                                CHECK_FCT_DO(
                                    fd_msg_avp_hdr(avpch4, &hdr), return );
                                pdn->qos.max_bandwidth_dl =
                                    hdr->avp_value->i32 / 1024; /* Kbps */
                            }
                            else
                                error++;
                        }
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
        mme_ue->subscribed_rau_tau_timer = hdr->avp_value->i32;
    }
    else
        error++;
    
    event_set(&e, MME_EVT_EMM_UE_FROM_S6A);
    event_set_param1(&e, (c_uintptr_t)mme_ue->index);
    event_set_param2(&e, (c_uintptr_t)S6A_CMD_UPDATE_LOCATION);
    event_set_param3(&e, (c_uintptr_t)result_code);
    mme_event_send(&e);

    /* Free the message */
    CHECK_POSIX_DO( pthread_mutex_lock(&fd_self()->stats_lock), );
    dur = ((ts.tv_sec - mi->ts.tv_sec) * 1000000) + 
        ((ts.tv_nsec - mi->ts.tv_nsec) / 1000);
    if (fd_self()->stats.nb_recv)
    {
        /* Ponderate in the avg */
        fd_self()->stats.avg = (fd_self()->stats.avg * 
            fd_self()->stats.nb_recv + dur) / (fd_self()->stats.nb_recv + 1);
        /* Min, max */
        if (dur < fd_self()->stats.shortest)
            fd_self()->stats.shortest = dur;
        if (dur > fd_self()->stats.longest)
            fd_self()->stats.longest = dur;
    }
    else
    {
        fd_self()->stats.shortest = dur;
        fd_self()->stats.longest = dur;
        fd_self()->stats.avg = dur;
    }
    if (error)
        fd_self()->stats.nb_errs++;
    else 
        fd_self()->stats.nb_recv++;

    CHECK_POSIX_DO( pthread_mutex_unlock(&fd_self()->stats_lock), );
    
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
    struct session *sess = NULL;

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
    CHECK_FCT_DO( fd_msg_sess_get(fd_g_config->cnf_dict, req, &sess, NULL), 
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

    CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &mi->ts), goto out );
    
    /* Keep a pointer to the session data for debug purpose, 
     * in real life we would not need it */
    svg = mi;
    
    /* Store this value in the session */
    CHECK_FCT_DO( fd_sess_state_store(mme_s6a_reg, sess, &mi), goto out ); 
    
    /* Send the request */
    CHECK_FCT_DO( fd_msg_send(&req, mme_s6a_ula_cb, svg), goto out );

    /* Increment the counter */
    CHECK_POSIX_DO( pthread_mutex_lock(&fd_self()->stats_lock), );
    fd_self()->stats.nb_sent++;
    CHECK_POSIX_DO( pthread_mutex_unlock(&fd_self()->stats_lock), );

    d_trace(3, "[S6A] Update-Location-Request : UE[%s] --> HSS\n", 
            mme_ue->imsi_bcd);

out:
    pool_free_node(&mme_s6a_sess_pool, mi);
    return;
}

int mme_s6a_init(void)
{
    pool_init(&mme_s6a_sess_pool, MAX_NUM_SESSION_STATE);

	/* Install objects definitions for this application */
	CHECK_FCT( s6a_dict_init() );

	CHECK_FCT( fd_sess_handler_create(&mme_s6a_reg, (void *)free, NULL, NULL) );

	/* Advertise the support for the application in the peer */
	CHECK_FCT( fd_disp_app_support(s6a_application, fd_vendor, 1, 0) );
	
	return 0;
}

void mme_s6a_final(void)
{
	CHECK_FCT_DO( fd_sess_handler_destroy(&mme_s6a_reg, NULL), );

    if (pool_size(&mme_s6a_sess_pool) != pool_avail(&mme_s6a_sess_pool))
        d_error("%d not freed in mme_s6a_sess_pool[%d] of S6A-SM",
            pool_size(&mme_s6a_sess_pool) - pool_avail(&mme_s6a_sess_pool),
            pool_size(&mme_s6a_sess_pool));

    d_trace(3, "%d not freed in mme_s6a_sess_pool[%d] of S6A-SM\n",
            pool_size(&mme_s6a_sess_pool) - pool_avail(&mme_s6a_sess_pool),
            pool_size(&mme_s6a_sess_pool));

    pool_final(&mme_s6a_sess_pool);
}
