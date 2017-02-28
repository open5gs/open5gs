#define TRACE_MODULE _s6a_auth

#include "core_debug.h"

#include "s6a_app.h"
#include "s6a_auth.h"

struct sess_state {
	c_int32_t	randval; /* a random value to store in Test-AVP */
	struct timespec ts;	/* Time of sending the message */
} ;

/* Cb called when an answer is received */
static void s6a_aia_cb(void * data, struct msg ** msg)
{
	struct sess_state * mi = NULL;
	struct timespec ts;
	struct session * sess;
	struct avp * avp;
	struct avp_hdr * hdr;
	unsigned long dur;
	int error = 0;
	
	CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &ts), return );

	/* Search the session, retrieve its data */
	{
		int new;
		CHECK_FCT_DO( fd_msg_sess_get(fd_g_config->cnf_dict, *msg, &sess, &new), 
                return );
        d_assert(new == 0, return, "fd_msg_sess_get() failed");
		
		CHECK_FCT_DO( fd_sess_state_retrieve( s6a_mme_reg, sess, &mi ), 
                return );
        d_assert((void *)mi == data, return, "fd_sess_state_retrieve() failed");
	}
	
	/* Now log content of the answer */
	fprintf(stderr, "RECV ");
	
#if 0
	/* Value of Test-AVP */
	CHECK_FCT_DO( fd_msg_search_avp ( *msg, s6a_avp, &avp), return );
	if (avp) {
		CHECK_FCT_DO( fd_msg_avp_hdr( avp, &hdr ), return );
		if (hdr->avp_value->i32 == mi->randval) {
			fprintf(stderr, "%x (%s) ", hdr->avp_value->i32, "Ok");
		} else {
			fprintf(stderr, "%x (%s) ", hdr->avp_value->i32, "PROBLEM");
			error++;
		}
	} else {
		fprintf(stderr, "no_Test-AVP ");
		error++;
	}
#endif
	
	/* Value of Result Code */
	CHECK_FCT_DO( fd_msg_search_avp ( *msg, s6a_result_code, &avp), 
            return );
	if (avp) {
		CHECK_FCT_DO( fd_msg_avp_hdr( avp, &hdr ), return );
        d_info("Status: %d ", hdr->avp_value->i32);
		if (hdr->avp_value->i32 != 2001)
			error++;
	} else {
        d_error("no_Result-Code");
		error++;
	}
	
	/* Value of Origin-Host */
	CHECK_FCT_DO( fd_msg_search_avp ( *msg, s6a_origin_host, &avp), return );
	if (avp) {
		CHECK_FCT_DO( fd_msg_avp_hdr( avp, &hdr ), return );
        d_info("From '%.*s' ", 
                (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
	} else {
        d_error("no_Origin-Host ");
		error++;
	}
	
	/* Value of Origin-Realm */
	CHECK_FCT_DO( fd_msg_search_avp ( *msg, s6a_origin_realm, &avp), return );
	if (avp) {
		CHECK_FCT_DO( fd_msg_avp_hdr( avp, &hdr ), return );
        d_info("('%.*s') ", 
                (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
	} else {
        d_error("no_Origin-Realm ");
		error++;
	}
	
	CHECK_POSIX_DO( pthread_mutex_lock(&s6a_config->stats_lock), );
	dur = ((ts.tv_sec - mi->ts.tv_sec) * 1000000) + 
        ((ts.tv_nsec - mi->ts.tv_nsec) / 1000);
	if (s6a_config->stats.nb_recv) {
		/* Ponderate in the avg */
		s6a_config->stats.avg = (s6a_config->stats.avg * 
            s6a_config->stats.nb_recv + dur) / (s6a_config->stats.nb_recv + 1);
		/* Min, max */
		if (dur < s6a_config->stats.shortest)
			s6a_config->stats.shortest = dur;
		if (dur > s6a_config->stats.longest)
			s6a_config->stats.longest = dur;
	} else {
		s6a_config->stats.shortest = dur;
		s6a_config->stats.longest = dur;
		s6a_config->stats.avg = dur;
	}
	if (error)
		s6a_config->stats.nb_errs++;
	else 
		s6a_config->stats.nb_recv++;
	
	
	CHECK_POSIX_DO( pthread_mutex_unlock(&s6a_config->stats_lock), );
	
	/* Display how long it took */
	if (ts.tv_nsec > mi->ts.tv_nsec) {
        d_info("in %d.%06ld sec", 
				(int)(ts.tv_sec - mi->ts.tv_sec),
				(long)(ts.tv_nsec - mi->ts.tv_nsec) / 1000);
	} else {
        d_info("in %d.%06ld sec", 
				(int)(ts.tv_sec + 1 - mi->ts.tv_sec),
				(long)(1000000000 + ts.tv_nsec - mi->ts.tv_nsec) / 1000);
	}
	
	/* Free the message */
	CHECK_FCT_DO(fd_msg_free(*msg), return);
	*msg = NULL;
	
	free(mi);
	
	return;
}

status_t s6_send_auth_req()
{
	struct msg * req = NULL;
	struct avp * avp;
	union avp_value val;
	struct sess_state * mi = NULL, *svg;
	struct session *sess = NULL;
	
	/* Create the request */
	CHECK_FCT_DO( fd_msg_new( s6a_cmd_air, 
                MSGFL_ALLOC_ETEID, &req ), goto out );
	
	/* Create a new session */
	#define TEST_APP_SID_OPT  "app_s6a"
	CHECK_FCT_DO( fd_msg_new_session( req, (os0_t)TEST_APP_SID_OPT, 
            CONSTSTRLEN(TEST_APP_SID_OPT) ), goto out );
	CHECK_FCT_DO( fd_msg_sess_get(fd_g_config->cnf_dict, req, &sess, NULL), 
            goto out );
	
	/* Create the random value to store with the session */
	mi = malloc(sizeof(struct sess_state));
    d_assert(mi, goto out, "malloc failed: %s", strerror(errno));
	
	mi->randval = (int32_t)random();
	
	/* Now set all AVPs values */
	
	/* Set the Destination-Realm AVP */
	{
		CHECK_FCT_DO( fd_msg_avp_new ( s6a_destination_realm, 0, &avp ), 
                goto out  );
		val.os.data = (unsigned char *)(s6a_config->dest_realm);
		val.os.len  = strlen(s6a_config->dest_realm);
		CHECK_FCT_DO( fd_msg_avp_setvalue( avp, &val ), goto out  );
		CHECK_FCT_DO( fd_msg_avp_add( req, MSG_BRW_LAST_CHILD, avp ), 
                goto out  );
	}
	
	/* Set the Destination-Host AVP if needed*/
	if (s6a_config->dest_host) {
		CHECK_FCT_DO( fd_msg_avp_new ( s6a_destination_host, 0, &avp ), 
                goto out  );
		val.os.data = (unsigned char *)(s6a_config->dest_host);
		val.os.len  = strlen(s6a_config->dest_host);
		CHECK_FCT_DO( fd_msg_avp_setvalue( avp, &val ), goto out  );
		CHECK_FCT_DO( fd_msg_avp_add( req, MSG_BRW_LAST_CHILD, avp ), 
                goto out  );
	}
	
	/* Set Origin-Host & Origin-Realm */
	CHECK_FCT_DO( fd_msg_add_origin ( req, 0 ), goto out  );
	
	/* Set the User-Name AVP if needed*/
	if (s6a_config->user_name) {
		CHECK_FCT_DO( fd_msg_avp_new ( s6a_user_name, 0, &avp ), goto out  );
		val.os.data = (unsigned char *)(s6a_config->user_name);
		val.os.len  = strlen(s6a_config->user_name);
		CHECK_FCT_DO( fd_msg_avp_setvalue( avp, &val ), goto out  );
		CHECK_FCT_DO( fd_msg_avp_add( req, MSG_BRW_LAST_CHILD, avp ), 
                goto out  );
	}

	/* Set the Auth-Session-Statee AVP if needed*/
    {
		CHECK_FCT_DO( fd_msg_avp_new ( s6a_auth_session_state, 0, &avp ), 
                goto out  );
        val.i32 = 1;
		CHECK_FCT_DO( fd_msg_avp_setvalue( avp, &val ), goto out  );
		CHECK_FCT_DO( fd_msg_avp_add( req, MSG_BRW_LAST_CHILD, avp ), 
                goto out  );
	}

	/* Set the Visited-PLMN-Id AVP if needed*/
    {
        c_uint8_t plmn[3] = { 0x00, 0xf1, 0x10 };
		CHECK_FCT_DO( fd_msg_avp_new ( s6a_visited_plmn_id, 0, &avp ), 
                goto out  );
		val.os.data = plmn;
		val.os.len  = 3;
		CHECK_FCT_DO( fd_msg_avp_setvalue( avp, &val ), goto out  );
		CHECK_FCT_DO( fd_msg_avp_add( req, MSG_BRW_LAST_CHILD, avp ), 
                goto out  );
	}
	
	CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &mi->ts), goto out );
	
	/* Keep a pointer to the session data for debug purpose, in real life we would not need it */
	svg = mi;
	
	/* Store this value in the session */
	CHECK_FCT_DO( fd_sess_state_store ( s6a_mme_reg, sess, &mi ), goto out ); 
	
	/* Log sending the message */
    d_info("SEND %x to '%s' (%s)\n", svg->randval, 
            s6a_config->dest_realm, s6a_config->dest_host?:"-" );
	
	/* Send the request */
	CHECK_FCT_DO( fd_msg_send( &req, s6a_aia_cb, svg ), goto out );

	/* Increment the counter */
	CHECK_POSIX_DO( pthread_mutex_lock(&s6a_config->stats_lock), );
	s6a_config->stats.nb_sent++;
	CHECK_POSIX_DO( pthread_mutex_unlock(&s6a_config->stats_lock), );

out:
	return CORE_OK;
}
