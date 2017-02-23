#define TRACE_MODULE _s6a_serv

#include "core_debug.h"

#include "freeDiameter/freeDiameter-host.h"
#include "freeDiameter/libfdcore.h"
#include "freeDiameter/extension.h"

#include "s6a_message.h"

/* handler for fallback cb */
static struct disp_hdl *s6a_hdl_fb = NULL; 
/* handler for Authentication-Information-Request cb */
static struct disp_hdl *s6a_hdl_air = NULL; 

/* Default callback for the application. */
static int s6a_fb_cb( struct msg ** msg, struct avp * avp, 
        struct session * sess, void * opaque, enum disp_action * act)
{
	/* This CB should never be called */
	d_warn("Unexpected message received!");
	
	return ENOTSUP;
}

/* Callback for incoming Authentication-Information-Request messages */
static int s6a_air_cb( struct msg ** msg, struct avp * avp, 
        struct session * sess, void * opaque, enum disp_action * act)
{
#if 0
	struct msg *ans, *qry;
	struct avp * a;
#endif

    d_info("Authentication-Information-Request received!");
	
	if (msg == NULL)
		return EINVAL;
	
	return 0;
}

status_t s6a_server_start()
{
	struct disp_when data;
	
	memset(&data, 0, sizeof(data));
	data.app = s6a_appli;
	data.command = s6a_air;
	
	/* fallback CB if Unexpected message received */
	CHECK_FCT(fd_disp_register(
                s6a_fb_cb, DISP_HOW_APPID, &data, NULL, &s6a_hdl_fb));
	
	/* Now specific handler for Authentication-Information-Request */
	CHECK_FCT(fd_disp_register(
                s6a_air_cb, DISP_HOW_CC, &data, NULL, &s6a_hdl_air));

    return CORE_OK;
}

void s6a_server_stop()
{
	if (s6a_hdl_fb)
		(void)fd_disp_unregister(&s6a_hdl_fb, NULL);

	if (s6a_hdl_air)
		(void) fd_disp_unregister(&s6a_hdl_air, NULL);
}
