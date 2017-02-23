#define TRACE_MODULE _s6a_client

#include "core_debug.h"

#include "freeDiameter/freeDiameter-host.h"
#include "freeDiameter/libfdcore.h"
#include "freeDiameter/extension.h"

#include "s6a_message.h"

#if 0
static struct session_handler *s6a_cli_reg = NULL;
#endif

struct sess_state {
	c_int32_t randval; /* a random value to store in Test-AVP */
	struct timespec ts;	/* Time of sending the message */
} ;

/* Cb called when an Authentication-Information-Answe is received */
static void s6a_aia_cb(void *data, struct msg **msg)
{
#if 0
	struct sess_state *mi = NULL;
	struct timespec ts;
	struct session *sess;
	struct avp *avp;
	struct avp_hdr *hdr;
	unsigned long dur;
	int error = 0;
#endif
	
	/* Now log content of the answer */
    d_info("Authentication-Information-Answer received!");
	
	return;
}

/* Create a test message */
void s6a_build_auth_info_req()
{
	struct msg *req = NULL;
	struct avp *avp;
	union avp_value val;
#if 0
	struct sess_state *mi = NULL, *svg;
#endif
	struct session *sess = NULL;
	
	/* Create the request */
	CHECK_FCT_DO(fd_msg_new(s6a_air, MSGFL_ALLOC_ETEID, &req), goto out);
	
	/* Create a new session */
	#define S6A_APP_SID_OPT  "app_s6a"
	CHECK_FCT_DO(fd_msg_new_session(req, (os0_t)S6A_APP_SID_OPT, 
                CONSTSTRLEN(S6A_APP_SID_OPT)), goto out);
	CHECK_FCT_DO(fd_msg_sess_get(
                fd_g_config->cnf_dict, req, &sess, NULL), goto out);
	
	/* Now set all AVPs values */
	
	/* Set the Destination-Realm AVP */
	{
		CHECK_FCT_DO(fd_msg_avp_new(
                    s6a_destination_realm, 0, &avp), goto out);
		val.os.data = (unsigned char *)(fd_g_config->cnf_diamrlm);
		val.os.len  = strlen(fd_g_config->cnf_diamrlm);
		CHECK_FCT_DO(fd_msg_avp_setvalue(avp, &val), goto out);
		CHECK_FCT_DO(fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp), goto out);
	}
	
	/* Set the Destination-Host AVP if needed*/
#if 0
	if (ta_conf->dest_host) {
#endif
    {
		CHECK_FCT_DO(fd_msg_avp_new(
                    s6a_destination_host, 0, &avp), goto out);
		val.os.data = (unsigned char *)(fd_g_config->cnf_diamid);
		val.os.len  = strlen(fd_g_config->cnf_diamid);
		CHECK_FCT_DO(fd_msg_avp_setvalue(avp, &val), goto out);
		CHECK_FCT_DO(fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp), goto out);
	}
	
	/* Set Origin-Host & Origin-Realm */
	CHECK_FCT_DO(fd_msg_add_origin (req, 0), goto out);
	
	/* Set the User-Name AVP if needed*/
#if 0
	if (ta_conf->user_name) {
#endif
    {
		CHECK_FCT_DO(fd_msg_avp_new(s6a_user_name, 0, &avp), goto out);
#define S6A_IMSI "01045238277"
		val.os.data = (unsigned char *)(S6A_IMSI);
		val.os.len  = strlen(S6A_IMSI);
		CHECK_FCT_DO(fd_msg_avp_setvalue(avp, &val), goto out);
		CHECK_FCT_DO(fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp), goto out);
	}
	
	/* Send the request */
	CHECK_FCT_DO(fd_msg_send(&req, s6a_aia_cb, NULL), goto out);

out:
	return;
}

status_t s6a_client_start()
{
#if 0
	CHECK_FCT(fd_sess_handler_create(&s6a_cli_reg, (void *)free, NULL, NULL));
#endif
    s6a_build_auth_info_req();

    return CORE_OK;
}

void s6a_client_stop()
{
#if 0
	CHECK_FCT_DO(fd_sess_handler_destroy(&s6a_cli_reg, NULL), /* continue */ );
#endif
}
