#include "core_debug.h"

#include "s6a_app.h"

static struct disp_hdl * s6a_hdl_fb = NULL; /* handler for fallback cb */
static struct disp_hdl * s6a_hdl_tr = NULL; /* handler for Test-Request req cb */

/* Default callback for the application. */
static int s6a_fb_cb( struct msg ** msg, struct avp * avp, 
        struct session * sess, void * opaque, enum disp_action * act)
{
	/* This CB should never be called */
	d_warn("Unexpected message received!");
	
	return ENOTSUP;
}

/* Callback for incoming Test-Request messages */
static int s6a_air_cb( struct msg ** msg, struct avp * avp, 
        struct session * sess, void * opaque, enum disp_action * act)
{
	struct msg *ans, *qry;
	struct avp * a;
	
	if (msg == NULL)
		return EINVAL;
	
	/* Create answer header */
	qry = *msg;
	CHECK_FCT( fd_msg_new_answer_from_req ( fd_g_config->cnf_dict, msg, 0 ) );
	ans = *msg;
	
#if 0
	/* Set the Test-AVP AVP */
	{
		struct avp * src = NULL;
		struct avp_hdr * hdr = NULL;
		
		CHECK_FCT( fd_msg_search_avp ( qry, s6a_avp, &src) );
		CHECK_FCT( fd_msg_avp_hdr( src, &hdr )  );
		
		CHECK_FCT( fd_msg_avp_new ( s6a_avp, 0, &avp ) );
		CHECK_FCT( fd_msg_avp_setvalue( avp, hdr->avp_value ) );
		CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
	}
	
	/* Set the Test-Payload-AVP AVP */
	if (s6a_conf->long_avp_id) {
		struct avp * src = NULL;
		struct avp_hdr * hdr = NULL;
		
		CHECK_FCT( fd_msg_search_avp ( qry, s6a_avp_long, &src) );
		CHECK_FCT( fd_msg_avp_hdr( src, &hdr )  );
		
		CHECK_FCT( fd_msg_avp_new ( s6a_avp_long, 0, &avp ) );
		CHECK_FCT( fd_msg_avp_setvalue( avp, hdr->avp_value ) );
		CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
	}
#endif
	
	
	/* Set the Origin-Host, Origin-Realm, Result-Code AVPs */
	CHECK_FCT( fd_msg_rescode_set( ans, "DIAMETER_SUCCESS", NULL, NULL, 1 ) );
	
	/* Send the answer */
	CHECK_FCT( fd_msg_send( msg, NULL, NULL ) );
	
	/* Add this value to the stats */
	CHECK_POSIX_DO( pthread_mutex_lock(&s6a_conf->stats_lock), );
	s6a_conf->stats.nb_echoed++;
	CHECK_POSIX_DO( pthread_mutex_unlock(&s6a_conf->stats_lock), );
	
	return 0;
}

int s6a_serv_init(void)
{
	struct disp_when data;
	
	memset(&data, 0, sizeof(data));
	data.app = s6a_appli;
	data.command = s6a_cmd_air;
	
	/* fallback CB if command != unexpected message received */
	CHECK_FCT( fd_disp_register( s6a_fb_cb, DISP_HOW_APPID, &data, NULL, 
                &s6a_hdl_fb ) );
	
	/* Now specific handler for Authentication-Information-Request */
	CHECK_FCT( fd_disp_register( s6a_air_cb, DISP_HOW_CC, &data, NULL, 
                &s6a_hdl_tr ) );
	
	return 0;
}

void s6a_serv_fini(void)
{
	if (s6a_hdl_fb) {
		(void) fd_disp_unregister(&s6a_hdl_fb, NULL);
	}
	if (s6a_hdl_tr) {
		(void) fd_disp_unregister(&s6a_hdl_tr, NULL);
	}
	
	return;
}
