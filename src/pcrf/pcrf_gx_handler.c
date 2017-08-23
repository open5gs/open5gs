#define TRACE_MODULE _pcrf_gx_handler

#include "core_debug.h"

#include "fd_lib.h"
#include "gx_lib.h"

#include "pcrf_context.h"

/* handler for fallback cb */
static struct disp_hdl *hdl_fb = NULL; 
/* handler for Credit-Control-Request cb */
static struct disp_hdl *hdl_ccr = NULL; 

/* Default callback for the application. */
static int pcrf_fb_cb(struct msg **msg, struct avp *avp, 
        struct session *sess, void *opaque, enum disp_action *act)
{
	/* This CB should never be called */
	d_warn("Unexpected message received!");
	
	return ENOTSUP;
}

/* Callback for incoming Credit-Control-Request messages */
static int pcrf_ccr_cb( struct msg **msg, struct avp *avp, 
        struct session *sess, void *opaque, enum disp_action *act)
{
	struct msg *ans, *qry;
    struct avp_hdr *hdr;
    union avp_value val;

    c_uint32_t cc_request_type = 0;
    c_uint32_t result_code = 0;
	
    d_assert(msg, return EINVAL,);

	/* Create answer header */
	qry = *msg;
	CHECK_FCT( fd_msg_new_answer_from_req(fd_g_config->cnf_dict, msg, 0) );
    ans = *msg;

    /* Get CC-Request-Type */
    CHECK_FCT( fd_msg_search_avp(qry, gx_cc_request_type, &avp) );
    CHECK_FCT( fd_msg_avp_hdr(avp, &hdr) );
    cc_request_type = hdr->avp_value->i32;

	/* Set the Origin-Host, Origin-Realm, andResult-Code AVPs */
	CHECK_FCT( fd_msg_rescode_set(ans, "DIAMETER_SUCCESS", NULL, NULL, 1) );

    /* Set the Auth-Application-Id AVP */
    CHECK_FCT_DO( fd_msg_avp_new(fd_auth_application_id, 0, &avp), goto out );
    val.i32 = GX_APPLICATION_ID;
    CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp), goto out );

    /* Set CC-Request-Type, CC-Request-Number */
    CHECK_FCT_DO( fd_msg_avp_new(gx_cc_request_type, 0, &avp), goto out );
    val.i32 = cc_request_type;
    CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp), goto out );

    CHECK_FCT_DO( fd_msg_avp_new(gx_cc_request_number, 0, &avp), goto out );
    val.i32 = 0;
    CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp), goto out );

	/* Send the answer */
	CHECK_FCT( fd_msg_send(msg, NULL, NULL) );
	
	/* Add this value to the stats */
	CHECK_POSIX_DO( pthread_mutex_lock(&fd_logger_self()->stats_lock), );
	fd_logger_self()->stats.nb_echoed++;
	CHECK_POSIX_DO( pthread_mutex_unlock(&fd_logger_self()->stats_lock), );

	return 0;

out:
    CHECK_FCT( fd_message_experimental_rescode_set(ans, result_code) );

    /* Set the Auth-Application-Id AVP */
    CHECK_FCT_DO( fd_msg_avp_new(fd_auth_application_id, 0, &avp), goto out );
    val.i32 = GX_APPLICATION_ID;
    CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp), goto out );

	CHECK_FCT( fd_msg_send(msg, NULL, NULL) );

    return 0;
}

int pcrf_gx_init(void)
{
	struct disp_when data;

    CHECK_FCT( fd_init(FD_MODE_SERVER, pcrf_self()->fd_conf_path) );

	/* Install objects definitions for this application */
	CHECK_FCT( gx_dict_init() );

	memset(&data, 0, sizeof(data));
	data.app = gx_application;
	
	/* fallback CB if command != unexpected message received */
	CHECK_FCT( fd_disp_register(pcrf_fb_cb, DISP_HOW_APPID, &data, NULL,
                &hdl_fb) );
	
	/* specific handler for Credit-Control-Request */
	data.command = gx_cmd_ccr;
	CHECK_FCT( fd_disp_register(pcrf_ccr_cb, DISP_HOW_CC, &data, NULL,
                &hdl_ccr) );

	/* Advertise the support for the application in the peer */
	CHECK_FCT( fd_disp_app_support(gx_application, fd_vendor, 1, 0) );

	return 0;
}

void pcrf_gx_final(void)
{
	if (hdl_fb) {
		(void) fd_disp_unregister(&hdl_fb, NULL);
	}
	if (hdl_ccr) {
		(void) fd_disp_unregister(&hdl_ccr, NULL);
	}

    fd_final();
}
