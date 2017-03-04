#define TRACE_MODULE _s6a_auth

#include "core_debug.h"

#include "s6a_message.h"
#include "s6a_lib.h"

struct sess_state {
    c_int32_t randval; /* a random value to store in Test-AVP */
    struct timespec ts; /* Time of sending the message */
};

static void s6a_aia_cb(void *data, struct msg **msg);

void s6a_test_send()
{
    s6a_auth_info_req_t air;
    memset(&air, 0, sizeof(s6a_auth_info_req_t));

    #define TEST_IMSI "001010123456800"
    air.imsi_len = strlen(TEST_IMSI);
    strcpy((char*)air.imsi, TEST_IMSI);
    air.visited_plmn_id.mcc = 1;
    air.visited_plmn_id.mnc = 1;
    air.visited_plmn_id.mnc_len = 2;
    air.auth_info.num_of_eutran_vector = 1;
    air.auth_info.immediate_response_preferred = 1;
    s6a_send_auth_info_req(&air);
}

/* Cb called when an answer is received */
int s6a_send_auth_info_req(s6a_auth_info_req_t *air)
{
    struct msg *req = NULL;
    struct avp *avp;
    struct avp *avpch;
    union avp_value val;
    struct sess_state *mi = NULL, *svg;
    struct session *sess = NULL;
    c_uint8_t plmn_id[PLMN_ID_LEN] = {0, };
    
    /* Create the random value to store with the session */
    mi = malloc(sizeof(struct sess_state));
    d_assert(mi, return -1, "malloc failed: %s", strerror(errno));
    
    mi->randval = (int32_t)random();
    
    /* Create the request */
    d_assert(fd_msg_new( s6a_cmd_air, MSGFL_ALLOC_ETEID, &req ) == 0, 
            free(mi); return -1,);
    
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
    val.os.data = air->imsi;
    val.os.len  = air->imsi_len;
    d_assert(fd_msg_avp_setvalue(avp, &val) == 0, goto out, );
    d_assert(fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp) == 0, goto out,);

    /* Add the Authentication-Info */
    d_assert(fd_msg_avp_new(s6a_req_eutran_auth_info, 0, &avp) == 0, goto out,);
    d_assert(fd_msg_avp_new(s6a_number_of_requested_vectors, 0, 
                &avpch) == 0, goto out,);
    val.u32 = air->auth_info.num_of_eutran_vector;
    d_assert(fd_msg_avp_setvalue (avpch, &val) == 0, goto out,);
    d_assert(fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch) == 0, goto out,);

    d_assert(fd_msg_avp_new(s6a_immediate_response_preferred, 0, 
                &avpch) == 0, goto out,);
    val.u32 = air->auth_info.immediate_response_preferred;
    d_assert(fd_msg_avp_setvalue(avpch, &val) == 0, goto out,);
    d_assert(fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avpch) == 0, goto out,);

    d_assert(fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp) == 0, goto out,);

    /* Set the Visited-PLMN-Id AVP if needed*/
    d_assert(fd_msg_avp_new(s6a_visited_plmn_id, 0, &avp) == 0, goto out,);
    val.os.data = plmn_id_to_buffer(&air->visited_plmn_id, plmn_id);
    val.os.len  = PLMN_ID_LEN;
    d_assert(fd_msg_avp_setvalue(avp, &val) == 0, goto out,);
    d_assert(fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp) == 0, goto out,);
    
    d_assert(clock_gettime(CLOCK_REALTIME, &mi->ts) == 0, goto out,);
    
    /* Keep a pointer to the session data for debug purpose, 
     * in real life we would not need it */
    svg = mi;
    
    /* Store this value in the session */
    d_assert(fd_sess_state_store(s6a_mme_reg, sess, &mi) == 0, goto out,); 
    
    /* Send the request */
    d_assert(fd_msg_send(&req, s6a_aia_cb, svg) == 0, goto out,);

    /* Increment the counter */
    d_assert(pthread_mutex_lock(&s6a_config->stats_lock) == 0,,);
    s6a_config->stats.nb_sent++;
    d_assert(pthread_mutex_unlock(&s6a_config->stats_lock) == 0,, );

    return 0;

out:
    d_assert(fd_msg_free(req) == 0,,);
    free(mi);

    return -1;
}

static void s6a_aia_cb(void *data, struct msg **msg)
{
    struct sess_state *mi = NULL;
    struct timespec ts;
    struct session *sess;
    struct avp *avp;
    struct avp_hdr *hdr;
    unsigned long dur;
    int error = 0;
    int new;
    
    CHECK_SYS_DO(clock_gettime(CLOCK_REALTIME, &ts), return);

    /* Search the session, retrieve its data */
    d_assert(fd_msg_sess_get(fd_g_config->cnf_dict, *msg, &sess, &new) == 0 && 
            new == 0, return,);
    
    d_assert(fd_sess_state_retrieve(s6a_mme_reg, sess, &mi) == 0 &&
            (void *)mi == data, fd_msg_free(*msg); *msg = NULL; return,);
    
    /* Value of Result Code */
    d_assert(fd_msg_search_avp(*msg, s6a_result_code, &avp) == 0, goto out,);
    if (avp) 
    {
        d_assert(fd_msg_avp_hdr(avp, &hdr) == 0, goto out,);
        if (hdr->avp_value->i32 != ER_DIAMETER_SUCCESS)
            error++;
    }
    else 
    {
        d_error("No 'Result-Code'");
        error++;
    }
    
    /* Value of Origin-Host */
    d_assert(fd_msg_search_avp(*msg, s6a_origin_host, &avp) == 0, goto out,);
    if (avp)
    {
        d_assert(fd_msg_avp_hdr(avp, &hdr) == 0, goto out,);
        d_info("From '%.*s' ", 
            (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
    } 
    else 
    {
        d_error("No 'Origin-Host'");
        error++;
    }
    
    /* Value of Origin-Realm */
    d_assert(fd_msg_search_avp(*msg, s6a_origin_realm, &avp) == 0, goto out,);
    if (avp) 
    {
        d_assert(fd_msg_avp_hdr(avp, &hdr) == 0, goto out,);
        d_info("('%.*s') ", 
            (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
    }
    else
    {
        d_error("No 'Origin-Realm'");
        error++;
    }
    
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
        d_info("in %d.%06ld sec", 
                (int)(ts.tv_sec - mi->ts.tv_sec),
                (long)(ts.tv_nsec - mi->ts.tv_nsec) / 1000);
    else
        d_info("in %d.%06ld sec", 
                (int)(ts.tv_sec + 1 - mi->ts.tv_sec),
                (long)(1000000000 + ts.tv_nsec - mi->ts.tv_nsec) / 1000);
    
out:
    /* Free the message */
    d_assert(fd_msg_free(*msg) == 0,,);
    *msg = NULL;
    
    free(mi);
    
    return;
}

