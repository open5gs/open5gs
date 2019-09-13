/****************
 Contributed by: Krishnan Srinivasan <hsirk_6@yahoo.com>
 License: to be specified.
 TODO:
 
 ****************/


#include <freeDiameter/extension.h>
#include <signal.h>
#include <time.h>
#define AUTH_APP_ID				    16777238
#define VENDOR_ID_3GPP  			    10415
/* The content of this file follows the same structure as dict_base_proto.c */

#if 0
#define CHECK_dict_new( _type, _data, _parent, _ref )  \
       CHECK_FCT(  fd_dict_new( fd_g_config->cnf_dict, (_type), (_data), (_parent), (_ref))  );
#endif

void dump_sess_eyec(struct session *sess, const char *);

static int ccr_cb( struct msg ** msg, struct avp * avp, struct session * sess, void * opaque, enum disp_action * act);
static int reauth_cb( struct msg ** msg, struct avp * avp, struct session * sess, void * opaque, enum disp_action * act);
static int cca_cb( struct msg ** msg, struct avp * avp, struct session * sess, void * opaque, enum disp_action * act);
static struct disp_hdl * ccr_cb_hdl = NULL; /* handler for ccr req cb */
static struct disp_hdl * cca_cb_hdl = NULL; /* handler for cca req cb */
static struct disp_hdl * reauth_cb_hdl = NULL; /* handler for cca req cb */

struct dict_object *ccr_cmd = NULL;
struct dict_object *cca_cmd = NULL;
struct dict_object *dataobj_re_auth_request_type = NULL;
struct dict_object * origin_host = NULL;
struct dict_object * origin_realm = NULL;
struct dict_object * dest_host = NULL;
struct dict_object * dest_realm = NULL;
struct dict_object *reauth_cmd = NULL;
struct dict_object * auth_app_id    = NULL; 
struct dict_object * service_cxt_id = NULL ; 
struct dict_object * cc_req_type    = NULL; 
struct dict_object * cc_req_num    = NULL; 
struct dict_object * bearer_usage    = NULL; 
struct dict_object * pflt_oper    = NULL; 
struct dict_object * pflt_info    = NULL; 
struct dict_object * pflt_id	= NULL; 
struct dict_object * gx_inf;
struct dict_object * term_cause = NULL;

struct session *g_sess = NULL;
struct session_handler *g_hdlr = NULL;

enum gx_state {
    STATE_INIT = 0,
    STATE_INTERMEDIATE ,
    STATE_FINAL
};
struct gx_sm_t {
    enum gx_state state;
    pthread_t tid;
    struct fifo *events;
    pthread_mutex_t p_sm_mtx;
    int req_num;
    struct session *sess;  
} g_gx_sm;

int snd_ccr_msg(struct gx_sm_t **gx_sm  , struct dict_object *cmd_r ) ;

void sig_hdlr(void);

void *gx_sm_th(void *sm);
#define press_key_continue() { printf("%s %d\n", __FUNCTION__, __LINE__);}
static int app_gx_entry(char * conffile)
{
//     TRACE_ENTRY("%p", conffile);	       
       {
	       application_id_t dcca_id = AUTH_APP_ID;
	       application_id_t ccr_id = 272;
	       application_id_t cca_id = 272;
	       application_id_t reauth_id = 258;
	       CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_APPLICATION, APPLICATION_BY_ID, &dcca_id, &gx_inf, ENOENT));
       CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_CODE_R, &ccr_id, &ccr_cmd, ENOENT));
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_CODE_A, &cca_id, &cca_cmd, ENOENT));
  //   CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_CODE_R, &reauth_id, &reauth_cmd, ENOENT));

       }

       /* Applications section */
#if 0
     {
	// Gx interface
	       {
	struct dict_application_data data = { AUTH_APP_ID, "3GPP-Gx Application" };
	 CHECK_dict_new( DICT_APPLICATION, &data, NULL, &gx_inf);

	       }				
  
	}
#endif


	// Do registeration and init stuff
      {
	struct disp_when data;
       
       TRACE_DEBUG(FULL, "Initializing dispatch callbacks for Gx interface");
	   
       memset(&data, 0, sizeof(data));
       data.app = gx_inf;
	   data.command = ccr_cmd;
       /* Now specific handler for CCR-CMD */
	   CHECK_FCT( fd_disp_register( ccr_cb, DISP_HOW_CC, &data, NULL, &ccr_cb_hdl ) );

 
       memset(&data, 0, sizeof(data));
       data.app = gx_inf;
	   data.command = cca_cmd;

	   CHECK_FCT( fd_disp_register( cca_cb, DISP_HOW_CC, &data, NULL, &cca_cb_hdl ) );
       
#ifdef REAUTH
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME,
				       "Re-Auth-Request", &reauth_cmd, ENOENT));
	memset(&data, 0, sizeof(data));
       data.app = gx_inf;
	   data.command = reauth_cmd;
	printf("register REAUTH\n");
	   CHECK_FCT( fd_disp_register( reauth_cb, DISP_HOW_CC, &data, NULL, &reauth_cb_hdl ) );
#endif


     }

	
       CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, 
			       DICT_AVP, AVP_BY_NAME, 
			       "Origin-Host", 
				&origin_host, 
				ENOENT) );

       CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, 
			       DICT_AVP, 
			       AVP_BY_NAME, 
			       "Origin-Realm", 
			       &origin_realm, 
			       ENOENT) );

       CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, 
			       DICT_AVP, 
			       AVP_BY_NAME, 
			       "Destination-Host", 
				&dest_host, 
				ENOENT) );

       CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, 
			       DICT_AVP, 
			       AVP_BY_NAME, 
			       "Destination-Realm", 
			       &dest_realm, 
			       ENOENT) );


       CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, 
			       DICT_AVP, 
			       AVP_BY_NAME, 
			       "Auth-Application-Id", 
			       &auth_app_id, 
				ENOENT) );


       CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, 
			       DICT_AVP, 
			       AVP_BY_NAME, 
			       "Service-Context-Id", 
			       &service_cxt_id, 
				ENOENT) );

    CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, 
			       DICT_AVP, 
			       AVP_BY_NAME, 
			       "CC-Request-Type", 
			       &cc_req_type, 
				ENOENT) );

    CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, 
			       DICT_AVP, 
			       AVP_BY_NAME, 
			       "Termination-Cause", 
			       &term_cause, 
				ENOENT) );

    CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, 
			       DICT_AVP, 
			       AVP_BY_NAME, 
			       "CC-Request-Number", 
			       &cc_req_num, 
				ENOENT) );
    {
	struct dict_avp_request req = { VENDOR_ID_3GPP, 0, "Bearer-Usage"}; 

	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, 
				   DICT_AVP, 
				   AVP_BY_NAME_AND_VENDOR, 
				   &req, 
				   &bearer_usage, 
				    ENOENT) );
    }
    {
	struct dict_avp_request req = { VENDOR_ID_3GPP, 0, "Packet-Filter-Operation"}; 

	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, 
				   DICT_AVP, 
				   AVP_BY_NAME_AND_VENDOR, 
				   &req, 
				   &pflt_oper, 
				    ENOENT) );
    }
    {
	struct dict_avp_request req = { VENDOR_ID_3GPP, 0, "Packet-Filter-Information"}; 

	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, 
				   DICT_AVP, 
				   AVP_BY_NAME_AND_VENDOR, 
				   &req, 
				   &pflt_info, 
				    ENOENT) );
    }
    {
	struct dict_avp_request req = { VENDOR_ID_3GPP, 0, "Packet-Filter-Identifier"}; 

	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, 
				   DICT_AVP, 
				   AVP_BY_NAME_AND_VENDOR, 
				   &req, 
				   &pflt_id, 
				    ENOENT) );
    }

    CHECK_FCT(fd_sess_handler_create( &g_hdlr, free, NULL));
    CHECK_FCT( fd_fifo_new(&g_gx_sm.events));  
    CHECK_FCT( fd_disp_app_support( gx_inf, NULL, 1 , 0));

    CHECK_FCT( fd_event_trig_regcb(SIGUSR1, "app_gx", sig_hdlr ) );


       TRACE_DEBUG(INFO, "Extension 'Gx' initialized");
       return 0;
}

void * gx_sm_th(void *sm)
{
   struct gx_sm_t *gx_sm = (struct gx_sm_t *) sm;
    struct timespec tout;
    int evt_code;

	    CHECK_SYS_DO( clock_gettime( CLOCK_REALTIME, &tout), goto out);
	    tout.tv_sec =+ 60 ;
   
    while(1) {
 
	fd_event_timedget(gx_sm->events, &tout , ETIMEDOUT, &evt_code, NULL, NULL );
	 CHECK_SYS_DO( clock_gettime( CLOCK_REALTIME, &tout), goto out);
	printf("in tout sec %d\n", tout.tv_sec); 
	if(evt_code == ETIMEDOUT) {
	    
	    snd_ccr_msg(&gx_sm, ccr_cmd);
	    gx_sm->req_num++ ;
	    gx_sm->state = STATE_INTERMEDIATE;
	    CHECK_SYS_DO( clock_gettime( CLOCK_REALTIME, &tout), goto out);
	    tout.tv_sec += 30 ;

	}
       // printf("press enter\n");
       // getchar();
    }
     
 out: 
 return NULL;
}
//TBD
int init_gx_sm(struct gx_sm_t *sm)
{
    sm->state = STATE_INIT;
    sm->tid = 0;//
    sm->events = NULL;
    pthread_mutex_t p_sm_mtx;
    sm->req_num = 0;
    sm->sess = NULL;

return 0;
}
int free_gx_sm(struct gx_sm_t *sm)
{
    
  free(sm);
}
struct gx_sm_t *gl_gx_sm = NULL;

void sig_hdlr()
{
struct gx_sm_t *gx_sm = gl_gx_sm;

    if( gx_sm) {

	fd_sess_dump( 0 , g_gx_sm.sess);

     } else {

	if(gx_sm= (struct gx_sm_t *)malloc(sizeof(struct gx_sm_t))) {
	    init_gx_sm(gx_sm);
	   }
	   gl_gx_sm = gx_sm;
	}

      snd_ccr_msg( &gx_sm, ccr_cmd);
    return;
}

static void cr_cb_ans(void *data, struct msg  **msg)
{
    printf("call back \n");
   return;
}

/*				     < Session-Id >
				   { Origin-Host }
				   { Origin-Realm }
				   { Destination-Realm }
				   { Auth-Application-Id }
				   { Service-Context-Id }
				   { CC-Request-Type }
				   { CC-Request-Number }
*/

int snd_ccr_msg(struct gx_sm_t **sm , struct dict_object *cmd_r ) 
{
       struct msg * req = NULL;
       struct avp * avp = NULL;
       union avp_value val;
       struct ta_mess_info * mi = NULL, *svg;
       struct session *sess = NULL;
    struct gx_sm_t *gx_sm = NULL;
    struct gx_sm_t *ptr = NULL;

       TRACE_DEBUG(FULL, "Creating a new CCR message for sending. %p", gx_sm);
       
       /* Create the request from template */
       CHECK_FCT_DO( fd_msg_new( cmd_r, MSGFL_ALLOC_ETEID, &req ), goto out );
       
    gx_sm = *sm;
       /* Create a new session */
    if(!gx_sm->sess) {

	   CHECK_FCT_DO( fd_sess_new( &sess, 
				   fd_g_config->cnf_diamid, 
				   fd_g_config->cnf_diamid_len, 
				    "CCR_SESSION", strlen("CCR_SESSION") ), goto out );
	
	printf("statemachine: %p %p %p\n", *(&gx_sm), gx_sm , *sm);
	gx_sm->sess = sess;
	printf("new session %p \n", sess);
	//Hold the session till terminate happens
	       CHECK_FCT( fd_sess_ref_msg(sess) );

      } else {

	sess = gx_sm->sess;
	printf("use previous session %p \n", sess);

      }
     
     fd_sess_dump( 0 , sess);
    
       
//     dump_sess_eyec( sess, __FUNCTION__);
       /* Now set all AVPs values */
       
       /* Session-Id */
       {
	       os0_t sid;
	       size_t sidlen;
	struct dict_object *sess_id = NULL;

       CHECK_FCT( fd_dict_search(   fd_g_config->cnf_dict
				   , DICT_AVP, AVP_BY_NAME
				   , "Session-Id"
				   , &sess_id, ENOENT) );

	       CHECK_FCT_DO( fd_sess_getsid ( sess, &sid, &sidlen ), goto out );
	       CHECK_FCT_DO( fd_msg_avp_new ( sess_id, 0, &avp ), goto out );
	       val.os.data = sid;
	       val.os.len  = sidlen;
	       CHECK_FCT_DO( fd_msg_avp_setvalue( avp, &val ), goto out );
	       CHECK_FCT_DO( fd_msg_avp_add( req, MSG_BRW_FIRST_CHILD, avp ), goto out );
       }

       /* Set the Destination-Realm AVP */
       {
	       CHECK_FCT_DO( fd_msg_avp_new ( dest_realm, 0, &avp ), goto out  );
	       val.os.data = (unsigned char *)("vm");
	       val.os.len  = strlen("vm");
	       CHECK_FCT_DO( fd_msg_avp_setvalue( avp, &val ), goto out  );
	       CHECK_FCT_DO( fd_msg_avp_add( req, MSG_BRW_LAST_CHILD, avp ), goto out  );
       }
       

       /* Set the Destination-Host AVP if needed*/
    {
	       CHECK_FCT_DO( fd_msg_avp_new ( dest_host, 0, &avp ), goto out  );
	       val.os.data = (unsigned char *)("192.168.101.3");
	       val.os.len  = strlen("192.168.101.3");
	       CHECK_FCT_DO( fd_msg_avp_setvalue( avp, &val ), goto out  );
	       CHECK_FCT_DO( fd_msg_avp_add( req, MSG_BRW_LAST_CHILD, avp ), goto out  );
    }
       

       /* Set Origin-Host & Origin-Realm */
       CHECK_FCT_DO( fd_msg_add_origin ( req, 0 ), goto out  );


   /*  Set Auth-Application ID */
    {
       CHECK_FCT_DO( fd_msg_avp_new ( auth_app_id, 0, &avp ), goto out  );
	       val.i32 =  AUTH_APP_ID; // Auth-App id is 4 for CCR 
	       CHECK_FCT_DO( fd_msg_avp_setvalue( avp, &val ), goto out  );
	       CHECK_FCT_DO( fd_msg_avp_add( req, MSG_BRW_LAST_CHILD, avp ), goto out  );
    }

    /*     Set Service Context ID     */
    {

       CHECK_FCT_DO( fd_msg_avp_new ( service_cxt_id, 0, &avp ), goto out  );
	val.os.data = (unsigned char *)("test@tst");
	       CHECK_FCT_DO( fd_msg_avp_setvalue( avp, &val ), goto out  );
	       CHECK_FCT_DO( fd_msg_avp_add( req, MSG_BRW_LAST_CHILD, avp ), goto out  );

    }

    /*     Set Request Type	      */
    {
#define CCR_INIT_REQUEST		1
#define CCR_UPDATE_REQUEST		2
#define CCR_TERMINATION_REQUEST 	3
#define CCR_EVENT_REQUEST		4
//TODO Change this to use  enum object
       CHECK_FCT_DO( fd_msg_avp_new ( cc_req_type, 0, &avp ), goto out  );
	if(gx_sm->state == STATE_INIT)
	    val.i32 = CCR_INIT_REQUEST;
	else if(gx_sm->state == STATE_FINAL)
	    val.i32 = CCR_TERMINATION_REQUEST;
	 else 
	    val.i32 = CCR_UPDATE_REQUEST;

	       CHECK_FCT_DO( fd_msg_avp_setvalue( avp, &val ), goto out  );
	       CHECK_FCT_DO( fd_msg_avp_add( req, MSG_BRW_LAST_CHILD, avp ), goto out  );

    }

    /*     Set Request Number	      */
    {
       CHECK_FCT_DO( fd_msg_avp_new ( cc_req_num, 0, &avp ), goto out  );
	val.i32 = gx_sm->req_num;
	       CHECK_FCT_DO( fd_msg_avp_setvalue( avp, &val ), goto out  );
	       CHECK_FCT_DO( fd_msg_avp_add( req, MSG_BRW_LAST_CHILD, avp ), goto out  );
	gx_sm->req_num++; 
    }

    switch(gx_sm->state) {

     case STATE_INIT:

	{
	//Set Bearer-Usage
    //TODO Change this to use  enum object
       CHECK_FCT_DO( fd_msg_avp_new ( bearer_usage, 0, &avp ), goto out  );
	val.i32 = 1;//IMS
	       CHECK_FCT_DO( fd_msg_avp_setvalue( avp, &val ), goto out  );
	       CHECK_FCT_DO( fd_msg_avp_add( req, MSG_BRW_LAST_CHILD, avp ), goto out  );
	} 
       {
	//Set Packet Filter Operation 
	//TODO Change this to use  enum object
       CHECK_FCT_DO( fd_msg_avp_new ( pflt_oper, 0, &avp ), goto out  );
	val.i32 = 1;//ADDITION
	       CHECK_FCT_DO( fd_msg_avp_setvalue( avp, &val ), goto out  );
	       CHECK_FCT_DO( fd_msg_avp_add( req, MSG_BRW_LAST_CHILD, avp ), goto out  );
	 } 
	struct avp *flt_info = NULL; 
	{
	    //Set Packet Filter Information 
	   CHECK_FCT_DO( fd_msg_avp_new ( pflt_info, 0, &flt_info ), goto out  );

   
	       CHECK_FCT_DO( fd_msg_avp_add( req, MSG_BRW_LAST_CHILD, flt_info ), goto out  );
	} 
	// Set Packet Filter Identity
	{

	 CHECK_FCT_DO( fd_msg_avp_new ( pflt_id, 0, &avp ), goto out  );
	  val.os.data = (unsigned char *)("ID");
		 val.os.len  = strlen("ID");
	  CHECK_FCT_DO( fd_msg_avp_setvalue( avp, &val ), goto out  );
	  if(flt_info) {
		   CHECK_FCT_DO( fd_msg_avp_add( flt_info, MSG_BRW_LAST_CHILD, avp ), goto out  );
	   }else {
		printf("flt_info NULL\n");
	    }

	 }
	CHECK_FCT(fd_sess_state_store(g_hdlr, sess, &gx_sm));

	break;
     case STATE_FINAL:
	 {
	//Set Packet Filter Operation 
	//TODO Change this to use  enum object
       CHECK_FCT_DO( fd_msg_avp_new ( term_cause, 0, &avp ), goto out  );
	val.i32 = 1;//Diameter logout
	       CHECK_FCT_DO( fd_msg_avp_setvalue( avp, &val ), goto out  );
	       CHECK_FCT_DO( fd_msg_avp_add( req, MSG_BRW_LAST_CHILD, avp ), goto out  );
	 } 
	break;
     default: 
	printf("State mismatch \n");
    }
       fflush(stderr);
       
       /* Send the request */
    printf("CCA %p\n",req);
// Everthing Done Store the state: reply should retreive it 
       CHECK_FCT_DO( fd_msg_send( &req, NULL, NULL ), goto out );

out:
       return;
}


static int reauth_cb( struct msg ** msg, struct avp * avp, struct session * sess, void * opaque, enum disp_action * act)
{
       struct msg *ans, *qry;
       struct avp * a;

       TRACE_ENTRY("%p %p %p %p", msg, avp, sess, act);
       
       if (msg == NULL)
	       return EINVAL;
       
       
       /* Create answer header */
       qry = *msg;
       CHECK_FCT( fd_msg_new_answer_from_req ( fd_g_config->cnf_dict, msg, 0 ) );
       ans = *msg;
       
       /* Set the Origin-Host, Origin-Realm, Result-Code AVPs */
       CHECK_FCT( fd_msg_rescode_set( ans, "DIAMETER_SUCCESS", NULL, NULL, 1 ) );
       
       /* Send the answer */
       CHECK_FCT( fd_msg_send( msg, NULL, NULL ) );
       
       

}
static int dcca_ans_from_req( struct dict_object * obj, struct msg *qry, struct msg **msg)
{
       struct avp *avp  = NULL;
       struct avp_hdr * avpdata;
    int  rc = -1;

    CHECK_FCT(fd_msg_search_avp( qry, obj, &avp));
    CHECK_FCT(fd_msg_avp_hdr(avp, &avpdata));
    avp = NULL;

       CHECK_FCT_DO( fd_msg_avp_new ( obj, 0, &avp ), goto out  );
       CHECK_FCT_DO( fd_msg_avp_setvalue( avp, avpdata->avp_value ), goto out  );
       CHECK_FCT_DO( fd_msg_avp_add( *msg, MSG_BRW_LAST_CHILD, avp ), goto out  );

 rc = 0;
out:
 return rc;

}
/* Dummy ccr which :
   Read the cc-req-type && cc-req-number from the msg and stick it back
  in the reply

*/
static int ccr_cb( struct msg ** msg, struct avp * r_avp, struct session * sess, void * opaque, enum disp_action * act)
{
       struct msg *ans, *qry;
       struct avp *avp  = NULL;
       struct avp_hdr * avp_data;
    int rc  = - 1; 

       TRACE_ENTRY("%p %p %p %p", msg, r_avp, sess, act);
       
       if (msg == NULL)
	       return EINVAL;
       
       
       /* Create answer header */
       qry = *msg;
       CHECK_FCT_DO( fd_msg_new_answer_from_req ( fd_g_config->cnf_dict, msg, 0 ) , goto out);
       ans = *msg;
       
       /* Set the Origin-Host, Origin-Realm, Result-Code AVPs */
       CHECK_FCT_DO( fd_msg_rescode_set( ans, "DIAMETER_SUCCESS", NULL, NULL, 1 ), goto out );

// Get the auth_app_id and from the reply and set it in the reply 
    CHECK_FCT_DO(dcca_ans_from_req( auth_app_id, qry, msg), goto out);
    CHECK_FCT_DO(dcca_ans_from_req( cc_req_type, qry, msg), goto out);
    CHECK_FCT_DO(dcca_ans_from_req( cc_req_num, qry, msg), goto out);

       /* Send the answer */
       CHECK_FCT_DO( fd_msg_send( msg, NULL, NULL ), goto out );
    rc  = 0;
  out:
    //Free up the memory
    return rc ;

}
int send_reauth_req()

{
       struct dict_application_data appdata;
    struct avp *avp = NULL;
    union avp_value val ;
    struct msg *qry, *ans = NULL;
    struct msg *req = NULL;
    struct msg *tst = NULL;
       struct dict_object * auth_app_id = NULL, *reauth_req_type = NULL;

//    qry = *msg;


    { // Send new reauth request

	   CHECK_FCT_DO( fd_msg_new( reauth_cmd, MSGFL_ALLOC_ETEID, &req ), goto out );
       CHECK_FCT(  fd_dict_getval(gx_inf, &appdata)  );
    
       struct msg_hdr * header = NULL;
	       CHECK_FCT( fd_msg_hdr ( req, &header ) );
	       header->msg_appl = appdata.application_id;

    }

       /* Session-Id */
       {
	       os0_t sid;
	       size_t sidlen;
	struct dict_object *sess_id = NULL;
       CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Session-Id", &sess_id, ENOENT) );

	       //CHECK_FCT_DO( fd_sess_getsid ( sess, &sid, &sidlen ), goto out );
	       CHECK_FCT_DO( fd_msg_avp_new ( sess_id, 0, &avp ), goto out );
	       val.os.data = sid;
	       val.os.len  = sidlen;
	       CHECK_FCT_DO( fd_msg_avp_setvalue( avp, &val ), goto out );
	       CHECK_FCT_DO( fd_msg_avp_add( req, MSG_BRW_FIRST_CHILD, avp ), goto out );
	       
       }

       /* Set the Destination-Realm AVP */
       {
	       CHECK_FCT_DO( fd_msg_avp_new ( dest_realm, 0, &avp ), goto out  );
	       val.os.data = (unsigned char *)("vm");
	       val.os.len  = strlen("vm");
	       CHECK_FCT_DO( fd_msg_avp_setvalue( avp, &val ), goto out  );
	       CHECK_FCT_DO( fd_msg_avp_add( req, MSG_BRW_LAST_CHILD, avp ), goto out  );
       }
       
       /* Set the Destination-Host AVP if needed*/
//     if (ta_conf->dest_host) {
	       CHECK_FCT_DO( fd_msg_avp_new ( dest_host, 0, &avp ), goto out  );
	       val.os.data = (unsigned char *)("cli.vm");
	       val.os.len  = strlen("cli.vm");
	       CHECK_FCT_DO( fd_msg_avp_setvalue( avp, &val ), goto out  );
	       CHECK_FCT_DO( fd_msg_avp_add( req, MSG_BRW_LAST_CHILD, avp ), goto out  );
//     }
    
       /* Set Origin-Host & Origin-Realm */
       CHECK_FCT_DO( fd_msg_add_origin ( req, 0 ), goto out  );

       /* AUTH_Application-ID */
       CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Auth-Application-Id", &auth_app_id, ENOENT) );
	       CHECK_FCT_DO( fd_msg_avp_new ( auth_app_id, 0, &avp ), goto out  );
	       val.i32 = appdata.application_id;
	       CHECK_FCT_DO( fd_msg_avp_setvalue( avp, &val ), goto out  );
	       CHECK_FCT_DO( fd_msg_avp_add( req, MSG_BRW_LAST_CHILD, avp ), goto out  );

       /* Re-Auth Request Type */
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Re-Auth-Request-Type", &reauth_req_type, ENOENT) );
	   CHECK_FCT_DO( fd_msg_avp_new ( reauth_req_type, 0, &avp ), goto out  );
	val.i32 = 0;
	CHECK_FCT_DO( fd_msg_avp_setvalue( avp, &val ), goto out  );
	CHECK_FCT_DO( fd_msg_avp_add( req, MSG_BRW_LAST_CHILD, avp ), goto out  );


       CHECK_FCT_DO( fd_msg_send( &req, cr_cb_ans, NULL ), goto out );
out:
    return 0 ;


}
/* Search a given AVP model in an AVP (extracted from libfreediameter/message.c ) */
int fd_avp_search_avp ( struct avp * groupedavp, struct dict_object * what, struct avp ** avp )
{
       struct avp * nextavp;
       struct avp_hdr * nextavphdr;
       struct dict_avp_data    dictdata;
       
       
       TRACE_ENTRY("%p %p %p", groupedavp, what, avp);
       
       CHECK_FCT(  fd_dict_getval(what, &dictdata)  );
       
       // Loop only in the group AVP 
       CHECK_FCT(  fd_msg_browse(groupedavp, MSG_BRW_FIRST_CHILD, (void *)&nextavp, NULL)  );
       CHECK_FCT( fd_msg_avp_hdr( nextavp, &nextavphdr )  );
       
       while (nextavphdr) {
	       
	       if ( (nextavphdr->avp_code   == dictdata.avp_code) && (nextavphdr->avp_vendor == dictdata.avp_vendor) ) // always 0 if no Vendor flag
	       {
		       break;
	       }
	       
	       // Otherwise move to next AVP in the grouped AVP 
	       CHECK_FCT( fd_msg_browse(nextavp, MSG_BRW_NEXT, (void *)&nextavp, NULL) );
	       
	       if(nextavp!=NULL)
	       {
		       CHECK_FCT( fd_msg_avp_hdr( nextavp, &nextavphdr )  );
	       }
	       else
		       nextavphdr=NULL;
       }
       if (avp)
	       *avp = nextavp;
       
       if (avp && nextavp) {
	       struct dictionary * dict;
	       CHECK_FCT( fd_dict_getdict( what, &dict) );
	       CHECK_FCT_DO( fd_msg_parse_dict( nextavp, dict, NULL ),  );
       }
       
       if (avp || nextavp)
	       return 0;
       else
	       return ENOENT;
}

static int cca_cb( struct msg ** msg, 
		   struct avp * t_avp , 
		   struct session * sess, 
		   void * opaque, 
		   enum disp_action * act)
{
    struct avp *avp = NULL, *g_avp = NULL;
    struct msg *req = *msg;
    struct dict_object *chrg_rule_name = NULL ;
    struct dict_object *chrg_rule_grp = NULL ;
       struct avp_hdr * avpdata = NULL;
    struct dict_avp_request grule_req =   {VENDOR_ID_3GPP, 0,"Charging-Rule-Install"};
    struct dict_avp_request rule_req =   {VENDOR_ID_3GPP, 0,"Charging-Rule-Name"};
    struct timespec sess_timeout;
    struct gx_sm_t *gx_sm = NULL;
  //  struct session *sess = NULL;
    
    
       CHECK_FCT(fd_sess_state_retrieve( g_hdlr, sess, &gx_sm));
    fd_sess_dump( 0, sess);
    if(gx_sm->state != STATE_FINAL) { 

	       CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict
				   , DICT_AVP, AVP_BY_NAME_AND_VENDOR
				   , &grule_req 
				   , &chrg_rule_grp
				   , ENOENT));
    
	   CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict
				   , DICT_AVP, AVP_BY_NAME_AND_VENDOR
				   , &rule_req 
				   , &chrg_rule_name
				   , ENOENT));


	CHECK_FCT(fd_msg_search_avp ( *msg, chrg_rule_grp, &g_avp ));
	CHECK_FCT(fd_avp_search_avp(g_avp, chrg_rule_name, &avp));

	if(avp) {
	       CHECK_FCT(fd_msg_avp_hdr(avp, &avpdata));
	} else {
	    printf("NULL AVP \n");
	}
	printf("charging-rule-name %s\n", avpdata->avp_value->os.data);
	gx_sm->state = STATE_FINAL;
	dump_sess_eyec( sess, __FUNCTION__);
	printf("next dump\n");
	CHECK_FCT(fd_sess_state_store( g_hdlr, sess, &gx_sm));
	fd_msg_free(*msg);
	*msg = NULL;

   } else {
	printf("Session terminated\n");
	free_gx_sm(gx_sm);
	fd_msg_free(*msg);
	fd_sess_reclaim(&sess);
	*msg = NULL;
    }
    fd_sess_dump( 0 , sess);

    return 0;
}

static int gx_entry(char * conffile)
{
 return 0;
}
EXTENSION_ENTRY( "app_gx", app_gx_entry, "dict_dcca_3gpp");
//EXTENSION_ENTRY( "app_gx", gx_entry);
