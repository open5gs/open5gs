#define TRACE_MODULE _pcrf_rx_path

#include "core_debug.h"

#include "fd/fd_lib.h"
#include "fd/rx/rx_dict.h"
#include "fd/rx/rx_message.h"

#include "pcrf_context.h"

static struct session_handler *pcrf_rx_reg = NULL;
static struct disp_hdl *hdl_rx_fb = NULL; 
static struct disp_hdl *hdl_rx_aar = NULL; 

void pcrf_rx_sess_cleanup(
        struct sess_state *sess_data, os0_t sid, void *opaque)
{
//    pool_free_node(&pgw_gx_sess_pool, sess_data);
}

static int pcrf_rx_fb_cb(struct msg **msg, struct avp *avp, 
        struct session *sess, void *opaque, enum disp_action *act)
{
	/* This CB should never be called */
	d_warn("Unexpected message received!");
	
	return ENOTSUP;
}

static int pcrf_rx_aar_cb( struct msg **msg, struct avp *avp, 
        struct session *sess, void *opaque, enum disp_action *act)
{
	struct msg *ans;
#if 0
	struct msg *ans, *qry;
    struct avp *avpch1, *avpch2, *avpch3, *avpch4;
    struct avp_hdr *hdr;
#endif
    union avp_value val;

#if 0
    status_t rv;
    gx_cca_message_t cca_message;
    c_int8_t imsi_bcd[MAX_IMSI_BCD_LEN+1];
    c_int8_t apn[MAX_APN_LEN+1];
    int i, j;

    c_uint32_t cc_request_type = 0;
    c_uint32_t result_code = RX_DIAMETER_ERROR_USER_UNKNOWN;
#endif
	
    d_assert(msg, return EINVAL,);

#if 0
    /* Initialize Message */
    memset(&cca_message, 0, sizeof(gx_cca_message_t));
#endif

	/* Create answer header */
#if 0
	qry = *msg;
#endif
	CHECK_FCT( fd_msg_new_answer_from_req(fd_g_config->cnf_dict, msg, 0) );
    ans = *msg;

    /* Set the Auth-Application-Id AVP */
    CHECK_FCT_DO( fd_msg_avp_new(fd_auth_application_id, 0, &avp), goto out );
    val.i32 = RX_APPLICATION_ID;
    CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp), goto out );

    /* Set the Auth-Request-Type AVP */
    CHECK_FCT_DO( fd_msg_avp_new(fd_auth_request_type, 0, &avp), goto out );
    val.i32 = 1;
    CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp), goto out );

	/* Set the Origin-Host, Origin-Realm, andResult-Code AVPs */
	CHECK_FCT( fd_msg_rescode_set(ans, "DIAMETER_SUCCESS", NULL, NULL, 1) );

	/* Send the answer */
	CHECK_FCT( fd_msg_send(msg, NULL, NULL) );

	/* Add this value to the stats */
	CHECK_POSIX_DO( pthread_mutex_lock(&fd_logger_self()->stats_lock), );
	fd_logger_self()->stats.nb_echoed++;
	CHECK_POSIX_DO( pthread_mutex_unlock(&fd_logger_self()->stats_lock), );

#if 0
    gx_cca_message_free(&cca_message);
#endif

    return 0;

out:
#if 0
    if (result_code == RX_DIAMETER_ERROR_USER_UNKNOWN)
    {
        CHECK_FCT( fd_msg_rescode_set(ans,
                    "DIAMETER_ERROR_USER_UNKNOWN", NULL, NULL, 1) );
    }
    else
    {
        CHECK_FCT( fd_message_experimental_rescode_set(ans, result_code) );
    }
#endif

	CHECK_FCT( fd_msg_send(msg, NULL, NULL) );

#if 0
    gx_cca_message_free(&cca_message);
#endif

    return 0;
}

int pcrf_rx_init(void)
{
	struct disp_when data;

	/* RX Interface */
	CHECK_FCT( rx_dict_init() );

	CHECK_FCT( fd_sess_handler_create(&pcrf_rx_reg, pcrf_rx_sess_cleanup,
                NULL, NULL) );

	memset(&data, 0, sizeof(data));
	data.app = rx_application;
	
	CHECK_FCT( fd_disp_register(pcrf_rx_fb_cb, DISP_HOW_APPID, &data, NULL,
                &hdl_rx_fb) );
	
	data.command = rx_cmd_aar;
	CHECK_FCT( fd_disp_register(pcrf_rx_aar_cb, DISP_HOW_CC, &data, NULL,
                &hdl_rx_aar) );

	CHECK_FCT( fd_disp_app_support(rx_application, fd_vendor, 1, 0) );

	return 0;
}

void pcrf_rx_final(void)
{
	CHECK_FCT_DO( fd_sess_handler_destroy(&pcrf_rx_reg, NULL), );
	if (hdl_rx_fb)
		(void) fd_disp_unregister(&hdl_rx_fb, NULL);
	if (hdl_rx_aar)
		(void) fd_disp_unregister(&hdl_rx_aar, NULL);
}
