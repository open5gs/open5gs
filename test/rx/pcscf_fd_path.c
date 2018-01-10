#define TRACE_MODULE _pcscf_fd_path

#include "core_debug.h"
#include "core_pool.h"
#include "core_lib.h"
#include "core_network.h"
#include "3gpp_types.h"

#include "gtp/gtp_xact.h"

#include "fd/fd_lib.h"
#include "fd/rx/rx_dict.h"

#include "pcscf_fd_path.h"

#define MAX_NUM_SESSION_STATE 32

static struct session_handler *pcscf_rx_reg = NULL;
static fd_config_t fd_config;

struct sess_state {
    struct timespec ts; /* Time of sending the message */
};

pool_declare(pcscf_rx_sess_pool, struct sess_state, MAX_NUM_SESSION_STATE);

static void pcscf_rx_aaa_cb(void *data, struct msg **msg);

void pcscf_rx_sess_cleanup(
        struct sess_state *sess_data, os0_t sid, void * opaque)
{
    pool_free_node(&pcscf_rx_sess_pool, sess_data);
}

void pcscf_rx_send_aar(const char *ip)
{
    status_t rv;

    struct msg *req = NULL;
    struct avp *avp;
#if 0
    struct avp *avpch1, *avpch2;
#else
    struct avp *avpch1;
#endif
    union avp_value val;
    struct sess_state *mi = NULL, *svg;
    struct session *session = NULL;

    paa_t paa;
    ipsubnet_t ipsub;

    d_assert(ip, return,);
    rv = core_ipsubnet(&ipsub, ip, NULL);
    d_assert(rv == CORE_OK, return,);

    /* Create the random value to store with the session */
    pool_alloc_node(&pcscf_rx_sess_pool, &mi);
    d_assert(mi, return, "malloc failed: %s", strerror(errno));
    
    /* Create the request */
    CHECK_FCT_DO( fd_msg_new(rx_cmd_aar, MSGFL_ALLOC_ETEID, &req), goto out );
    {
        struct msg_hdr * h;
        CHECK_FCT_DO( fd_msg_hdr( req, &h ), goto out );
        h->msg_appl = RX_APPLICATION_ID;
    }
    
    /* Create a new session */
    #define RX_APP_SID_OPT  "app_rx"
    CHECK_FCT_DO( fd_msg_new_session(req, (os0_t)RX_APP_SID_OPT, 
            CONSTSTRLEN(RX_APP_SID_OPT)), goto out );
    CHECK_FCT_DO( fd_msg_sess_get(fd_g_config->cnf_dict, req, &session, NULL),
            goto out );

    /* Set Origin-Host & Origin-Realm */
    CHECK_FCT_DO( fd_msg_add_origin(req, 0), goto out );
    
    /* Set the Destination-Realm AVP */
    CHECK_FCT_DO( fd_msg_avp_new(fd_destination_realm, 0, &avp), goto out );
    val.os.data = (unsigned char *)(fd_g_config->cnf_diamrlm);
    val.os.len  = strlen(fd_g_config->cnf_diamrlm);
    CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp), goto out );

    /* Set the Auth-Application-Id AVP */
    CHECK_FCT_DO( fd_msg_avp_new(fd_auth_application_id, 0, &avp), goto out );
    val.i32 = RX_APPLICATION_ID;
    CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp), goto out );

    /* Set Subscription-Id */
    CHECK_FCT_DO( fd_msg_avp_new(rx_subscription_id, 0, &avp),
            goto out );

    CHECK_FCT_DO( fd_msg_avp_new(rx_subscription_id_type, 0, &avpch1),
            goto out );
    val.i32 = RX_SUBSCRIPTION_ID_TYPE_END_USER_IMSI;
    CHECK_FCT_DO( fd_msg_avp_setvalue (avpch1, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1), goto out );

    #define RX_APP_IMSI_BCD  "0123456789012345"
    CHECK_FCT_DO( fd_msg_avp_new(rx_subscription_id_data, 0, &avpch1),
            goto out );
    val.os.data = (c_uint8_t *)RX_APP_IMSI_BCD;
    val.os.len  = strlen(RX_APP_IMSI_BCD);
    CHECK_FCT_DO( fd_msg_avp_setvalue (avpch1, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1), goto out );

    CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp), goto out );

    if (ipsub.family == AF_INET)
    {
        /* Set Framed-IP-Address */
        CHECK_FCT_DO( fd_msg_avp_new(rx_framed_ip_address, 0, &avp),
                goto out );
        val.os.data = (c_uint8_t*)ipsub.sub;
        val.os.len = IPV4_LEN;
        CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
        CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp),
                goto out );
    }
    else if (ipsub.family == AF_INET6)
    {
        /* Set Framed-IPv6-Prefix */
        CHECK_FCT_DO( fd_msg_avp_new(rx_framed_ipv6_prefix, 0, &avp),
                goto out );
        memset(&paa, 0, sizeof(paa_t));

        memcpy(paa.addr6, ipsub.sub, IPV6_LEN);
        paa.pdn_type = 0x03;
#define FRAMED_IPV6_PREFIX_LENGTH 128  /* from spec document */
        paa.len = FRAMED_IPV6_PREFIX_LENGTH; 
        val.os.data = (c_uint8_t*)&paa;
        val.os.len = PAA_IPV6_LEN;
        CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
        CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp),
                goto out );
    }

    CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &mi->ts), goto out );
    
    /* Keep a pointer to the session data for debug purpose, 
     * in real life we would not need it */
    svg = mi;
    
    /* Store this value in the session */
    CHECK_FCT_DO( fd_sess_state_store(pcscf_rx_reg, session, &mi), goto out );
    
    /* Send the request */
    CHECK_FCT_DO( fd_msg_send(&req, pcscf_rx_aaa_cb, svg), goto out );

    /* Increment the counter */
    CHECK_POSIX_DO( pthread_mutex_lock(&fd_logger_self()->stats_lock), );
    fd_logger_self()->stats.nb_sent++;
    CHECK_POSIX_DO( pthread_mutex_unlock(&fd_logger_self()->stats_lock), );

out:
    return;
}

static void pcscf_rx_aaa_cb(void *data, struct msg **msg)
{
    struct sess_state *mi = NULL;
    struct timespec ts;
    struct session *session;
#if 0
    struct avp *avp, *avpch1, *avpch2, *avpch3, *avpch4;
#else
    struct avp *avp, *avpch1;
#endif
    struct avp_hdr *hdr;
    unsigned long dur;
    int error = 0;
    int new;
    c_int32_t result_code = 0;

    CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &ts), return );

    /* Search the session, retrieve its data */
    CHECK_FCT_DO( fd_msg_sess_get(fd_g_config->cnf_dict, *msg, &session, &new),
            return );
    d_assert(new == 0, return, );
    
    CHECK_FCT_DO( fd_sess_state_retrieve(pcscf_rx_reg, session, &mi), return );
    d_assert(mi && (void *)mi == data, return, );

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
                    fd_experimental_result_code, &avpch1), return );
            if (avpch1)
            {
                CHECK_FCT_DO( fd_msg_avp_hdr(avpch1, &hdr), return);
                result_code = hdr->avp_value->i32;
                d_trace(3, "Experimental Result Code: %d\n",
                        result_code);
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

out:
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

    pcscf_rx_sess_cleanup(mi, NULL, NULL);
    return;
}

void pcscf_fd_config()
{
    memset(&fd_config, 0, sizeof(fd_config_t));

    fd_config.cnf_diamid = "pcscf.open-ims.test";
    fd_config.cnf_diamrlm = "open-ims.test";
    fd_config.cnf_port = DIAMETER_PORT;
    fd_config.cnf_port_tls = DIAMETER_SECURE_PORT;
    fd_config.cnf_flags.no_sctp = 1;
    fd_config.cnf_addr = "127.0.0.1";

    fd_config.ext[fd_config.num_of_ext].module = "dbg_msg_dumps.so";
    fd_config.ext[fd_config.num_of_ext].conf = "0x8888";
    fd_config.num_of_ext++;
    fd_config.ext[fd_config.num_of_ext].module = "dict_rfc5777.so";
    fd_config.num_of_ext++;
    fd_config.ext[fd_config.num_of_ext].module = "dict_mip6i.so";
    fd_config.num_of_ext++;
    fd_config.ext[fd_config.num_of_ext].module = "dict_nasreq.so";
    fd_config.num_of_ext++;
    fd_config.ext[fd_config.num_of_ext].module = "dict_nas_mipv6.so";
    fd_config.num_of_ext++;
    fd_config.ext[fd_config.num_of_ext].module = "dict_dcca.so";
    fd_config.num_of_ext++;
    fd_config.ext[fd_config.num_of_ext].module = "dict_dcca_3gpp.so";
    fd_config.num_of_ext++;

    fd_config.conn[fd_config.num_of_conn].identity = "pcrf.open-ims.test";
    fd_config.conn[fd_config.num_of_conn].addr = "127.0.0.5";
    fd_config.num_of_conn++;
}

int pcscf_fd_init(void)
{
    pool_init(&pcscf_rx_sess_pool, MAX_NUM_SESSION_STATE);

    pcscf_fd_config();

    CHECK_FCT( fd_init(FD_MODE_CLIENT, NULL, &fd_config) );

	CHECK_FCT( rx_dict_init() );

	CHECK_FCT( fd_sess_handler_create(&pcscf_rx_reg, pcscf_rx_sess_cleanup,
                NULL, NULL) );

	/* Advertise the support for the application in the peer */
	CHECK_FCT( fd_disp_app_support(rx_application, fd_vendor, 1, 0) );
	
	return 0;
}

void pcscf_fd_final(void)
{
	CHECK_FCT_DO( fd_sess_handler_destroy(&pcscf_rx_reg, NULL), );

    fd_final();

    if (pool_used(&pcscf_rx_sess_pool))
        d_error("%d not freed in pcscf_rx_sess_pool[%d] of S6A-SM",
                pool_used(&pcscf_rx_sess_pool), pool_size(&pcscf_rx_sess_pool));
    d_trace(3, "%d not freed in pcscf_rx_sess_pool[%d] of S6A-SM\n",
            pool_used(&pcscf_rx_sess_pool), pool_size(&pcscf_rx_sess_pool));

    pool_final(&pcscf_rx_sess_pool);
}
