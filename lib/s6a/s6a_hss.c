#define TRACE_MODULE _s6a_hss

#include "core_debug.h"

#include "s6a_app.h"

static struct disp_hdl *s6a_hdl_fb = NULL; /* handler for fallback cb */
static struct disp_hdl *s6a_hdl_tr = NULL; /* handler for Test-Request req cb */

/* Default callback for the application. */
static int s6a_fb_cb(struct msg **msg, struct avp *avp, 
        struct session *sess, void *opaque, enum disp_action *act)
{
	/* This CB should never be called */
	d_warn("Unexpected message received!");
	
	return ENOTSUP;
}

/* Callback for incoming Test-Request messages */
static int s6a_air_cb( struct msg **msg, struct avp *avp, 
        struct session *sess, void *opaque, enum disp_action *act)
{
	struct msg *ans, *qry;
	struct avp * a;
	
	if (msg == NULL)
		return EINVAL;
	
	/* Create answer header */
	qry = *msg;
	d_assert(fd_msg_new_answer_from_req(fd_g_config->cnf_dict, msg, 0) == 0, 
            return -1,);
	ans = *msg;
	
	/* Set the Origin-Host, Origin-Realm, Result-Code AVPs */
	d_assert(fd_msg_rescode_set(ans, "DIAMETER_SUCCESS", NULL, NULL, 1) == 0,
            goto out,);
	
	/* Send the answer */
	d_assert(fd_msg_send(msg, NULL, NULL) == 0, goto out,);
	
	/* Add this value to the stats */
	d_assert(pthread_mutex_lock(&s6a_config->stats_lock) == 0,,) ;
	s6a_config->stats.nb_echoed++;
	d_assert(pthread_mutex_unlock(&s6a_config->stats_lock) == 0,,);

    d_assert(fd_msg_free(qry) == 0,,);

	return 0;
out:
    d_assert(fd_msg_free(ans) == 0,,);
	
    return -1;
}

int s6a_hss_init(void)
{
	struct disp_when data;
	
	memset(&data, 0, sizeof(data));
	data.app = s6a_appli;
	data.command = s6a_cmd_air;
	
	/* fallback CB if command != unexpected message received */
	d_assert(fd_disp_register(s6a_fb_cb, DISP_HOW_APPID, &data, NULL, 
                &s6a_hdl_fb) == 0, return -1,);
	
	/* Now specific handler for Authentication-Information-Request */
	d_assert(fd_disp_register(s6a_air_cb, DISP_HOW_CC, &data, NULL, 
                &s6a_hdl_tr) == 0, return -1,);
	
	return 0;
}

void s6a_hss_final(void)
{
	if (s6a_hdl_fb) {
		(void) fd_disp_unregister(&s6a_hdl_fb, NULL);
	}
	if (s6a_hdl_tr) {
		(void) fd_disp_unregister(&s6a_hdl_tr, NULL);
	}
	
	return;
}
