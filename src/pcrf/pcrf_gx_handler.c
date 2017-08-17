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
