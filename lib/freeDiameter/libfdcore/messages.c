/*********************************************************************************************************
* Software License Agreement (BSD License)                                                               *
* Author: Sebastien Decugis <sdecugis@freediameter.net>							 *
*													 *
* Copyright (c) 2013, WIDE Project and NICT								 *
* All rights reserved.											 *
* 													 *
* Redistribution and use of this software in source and binary forms, with or without modification, are  *
* permitted provided that the following conditions are met:						 *
* 													 *
* * Redistributions of source code must retain the above 						 *
*   copyright notice, this list of conditions and the 							 *
*   following disclaimer.										 *
*    													 *
* * Redistributions in binary form must reproduce the above 						 *
*   copyright notice, this list of conditions and the 							 *
*   following disclaimer in the documentation and/or other						 *
*   materials provided with the distribution.								 *
* 													 *
* * Neither the name of the WIDE Project or NICT nor the 						 *
*   names of its contributors may be used to endorse or 						 *
*   promote products derived from this software without 						 *
*   specific prior written permission of WIDE Project and 						 *
*   NICT.												 *
* 													 *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED *
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A *
* PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR *
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 	 *
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 	 *
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR *
* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF   *
* ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.								 *
*********************************************************************************************************/

#include "fdcore-internal.h"

static struct dict_object * dict_avp_SI  = NULL; /* Session-Id */
static struct dict_object * dict_avp_OH  = NULL; /* Origin-Host */
static struct dict_object * dict_avp_OR  = NULL; /* Origin-Realm */
static struct dict_object * dict_avp_EM  = NULL; /* Error-Message */
static struct dict_object * dict_avp_ERH = NULL; /* Error-Reporting-Host */
static struct dict_object * dict_avp_FAVP= NULL; /* Failed-AVP */
static struct dict_object * dict_avp_RC  = NULL; /* Result-Code */
struct dict_object * fd_dict_avp_OSI = NULL; /* Origin-State-Id */
struct dict_object * fd_dict_cmd_CER = NULL; /* Capabilities-Exchange-Request */
struct dict_object * fd_dict_cmd_DWR = NULL; /* Device-Watchdog-Request */
struct dict_object * fd_dict_avp_DC  = NULL; /* Disconnect-Cause */
struct dict_object * fd_dict_cmd_DPR = NULL; /* Disconnect-Peer-Request */

/* Resolve the dictionary objects */
int fd_msg_init(void)
{
	TRACE_ENTRY("");
	
	/* Initialize the dictionary objects that we may use frequently */
	CHECK_FCT(  fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Session-Id", 	&dict_avp_SI , ENOENT)  );
	CHECK_FCT(  fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Origin-Host",     	&dict_avp_OH  , ENOENT)  );
	CHECK_FCT(  fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Origin-Realm",    	&dict_avp_OR  , ENOENT)  );
	CHECK_FCT(  fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Origin-State-Id", 	&fd_dict_avp_OSI , ENOENT)  );
	
	CHECK_FCT(  fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Result-Code",     	&dict_avp_RC  , ENOENT)  );
	CHECK_FCT(  fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Error-Message",   	&dict_avp_EM  , ENOENT)  );
	CHECK_FCT(  fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Error-Reporting-Host", &dict_avp_ERH , ENOENT)  );
	CHECK_FCT(  fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Failed-AVP",      	&dict_avp_FAVP, ENOENT)  );
	
	CHECK_FCT(  fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Disconnect-Cause", 	&fd_dict_avp_DC , ENOENT)  );
	
	CHECK_FCT( fd_dict_search ( fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME, "Capabilities-Exchange-Request", &fd_dict_cmd_CER, ENOENT ) );
	CHECK_FCT( fd_dict_search ( fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME, "Device-Watchdog-Request", &fd_dict_cmd_DWR, ENOENT ) );
	CHECK_FCT( fd_dict_search ( fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME, "Disconnect-Peer-Request", &fd_dict_cmd_DPR, ENOENT ) );
	
	
	return 0;
}

/* Add Origin-Host, Origin-Realm, Origin-State-Id AVPS at the end of the message */
int fd_msg_add_origin ( struct msg * msg, int osi )
{
	union avp_value val;
	struct avp * avp_OH  = NULL;
	struct avp * avp_OR  = NULL;
	struct avp * avp_OSI = NULL;
	
	TRACE_ENTRY("%p", msg);
	CHECK_PARAMS(  msg  );
	
	/* Create the Origin-Host AVP */
	CHECK_FCT( fd_msg_avp_new( dict_avp_OH, 0, &avp_OH ) );
	
	/* Set its value */
	memset(&val, 0, sizeof(val));
	val.os.data = (os0_t)fd_g_config->cnf_diamid;
	val.os.len  = fd_g_config->cnf_diamid_len;
	CHECK_FCT( fd_msg_avp_setvalue( avp_OH, &val ) );
	
	/* Add it to the message */
	CHECK_FCT( fd_msg_avp_add( msg, MSG_BRW_LAST_CHILD, avp_OH ) );
	
	
	/* Create the Origin-Realm AVP */
	CHECK_FCT( fd_msg_avp_new( dict_avp_OR, 0, &avp_OR ) );
	
	/* Set its value */
	memset(&val, 0, sizeof(val));
	val.os.data = (os0_t)fd_g_config->cnf_diamrlm;
	val.os.len  = fd_g_config->cnf_diamrlm_len;
	CHECK_FCT( fd_msg_avp_setvalue( avp_OR, &val ) );
	
	/* Add it to the message */
	CHECK_FCT( fd_msg_avp_add( msg, MSG_BRW_LAST_CHILD, avp_OR ) );
	
	if (osi) {
		/* Create the Origin-State-Id AVP */
		CHECK_FCT( fd_msg_avp_new( fd_dict_avp_OSI, 0, &avp_OSI ) );

		/* Set its value */
		memset(&val, 0, sizeof(val));
		val.u32 = fd_g_config->cnf_orstateid;
		CHECK_FCT( fd_msg_avp_setvalue( avp_OSI, &val ) );

		/* Add it to the message */
		CHECK_FCT( fd_msg_avp_add( msg, MSG_BRW_LAST_CHILD, avp_OSI ) );
	}
	
	return 0;
}

/* Create a new Session-Id and add at the beginning of the message. */
int fd_msg_new_session( struct msg * msg, os0_t opt, size_t optlen )
{
	union avp_value val;
	struct avp * avp  = NULL;
	struct session * sess = NULL;
	os0_t sid;
	size_t sidlen;
	
	TRACE_ENTRY("%p %p %zd", msg, opt, optlen);
	CHECK_PARAMS(  msg  );
	
	/* Check there is not already a session in the message */
	CHECK_FCT( fd_msg_sess_get(fd_g_config->cnf_dict, msg, &sess, NULL) );
	CHECK_PARAMS( sess == NULL );
	
	/* Ok, now create the session */
	CHECK_FCT( fd_sess_new ( &sess, fd_g_config->cnf_diamid, fd_g_config->cnf_diamid_len, opt, optlen ) );
	CHECK_FCT( fd_sess_getsid( sess, &sid, &sidlen) );
	
	/* Create an AVP to hold it */
	CHECK_FCT( fd_msg_avp_new( dict_avp_SI, 0, &avp ) );
	
	/* Set its value */
	memset(&val, 0, sizeof(val));
	val.os.data = sid;
	val.os.len  = sidlen;
	CHECK_FCT( fd_msg_avp_setvalue( avp, &val ) );
	
	/* Add it to the message */
	CHECK_FCT( fd_msg_avp_add( msg, MSG_BRW_FIRST_CHILD, avp ) );
	
	/* Save the session associated with the message */
	CHECK_FCT( fd_msg_sess_set( msg, sess) );
	
	/* Done! */
	return 0;
}


/* Add Result-Code and eventually Failed-AVP, Error-Message and Error-Reporting-Host AVPs */
int fd_msg_rescode_set( struct msg * msg, char * rescode, char * errormsg, struct avp * optavp, int type_id )
{
	union avp_value val;
	struct avp * avp_RC  = NULL;
	struct avp * avp_EM  = NULL;
	struct avp * avp_ERH = NULL;
	struct avp * avp_FAVP= NULL;
	uint32_t rc_val = 0;
	int set_e_bit=0;
	int std_err_msg=0;
	
	TRACE_ENTRY("%p %s %p %p %d", msg, rescode, errormsg, optavp, type_id);
		
	CHECK_PARAMS(  msg && rescode  );
	
	/* Find the enum value corresponding to the rescode string, this will give the class of error */
	{
		struct dict_object * enum_obj = NULL;
		struct dict_enumval_request req;
		memset(&req, 0, sizeof(struct dict_enumval_request));
		
		/* First, get the enumerated type of the Result-Code AVP (this is fast, no need to cache the object) */
		CHECK_FCT(  fd_dict_search( fd_g_config->cnf_dict, DICT_TYPE, TYPE_OF_AVP, dict_avp_RC, &(req.type_obj), ENOENT  )  );
		
		/* Now search for the value given as parameter */
		req.search.enum_name = rescode;
		CHECK_FCT(  fd_dict_search( fd_g_config->cnf_dict, DICT_ENUMVAL, ENUMVAL_BY_STRUCT, &req, &enum_obj, ENOTSUP)  );
		
		/* finally retrieve its data */
		CHECK_FCT_DO(  fd_dict_getval( enum_obj, &(req.search) ), return EINVAL );
		
		/* copy the found value, we're done */
		rc_val = req.search.enum_value.u32;
	}
	
	if (type_id == 1) {
		/* Add the Origin-Host and Origin-Realm AVP */
		CHECK_FCT( fd_msg_add_origin ( msg, 0 ) );
	}
	
	/* Create the Result-Code AVP */
	CHECK_FCT( fd_msg_avp_new( dict_avp_RC, 0, &avp_RC ) );
	
	/* Set its value */
	memset(&val, 0, sizeof(val));
	val.u32  = rc_val;
	CHECK_FCT( fd_msg_avp_setvalue( avp_RC, &val ) );
	
	/* Add it to the message */
	CHECK_FCT( fd_msg_avp_add( msg, MSG_BRW_LAST_CHILD, avp_RC ) );
	
	if (type_id == 2) {
		/* Add the Error-Reporting-Host AVP */
		
		CHECK_FCT( fd_msg_avp_new( dict_avp_ERH, 0, &avp_ERH ) );

		/* Set its value */
		memset(&val, 0, sizeof(val));
		val.os.data = (uint8_t *)fd_g_config->cnf_diamid;
		val.os.len  = fd_g_config->cnf_diamid_len;
		CHECK_FCT( fd_msg_avp_setvalue( avp_ERH, &val ) );

		/* Add it to the message */
		CHECK_FCT( fd_msg_avp_add( msg, MSG_BRW_LAST_CHILD, avp_ERH ) );
	
	}
	
	/* Now add the optavp in a FailedAVP if provided */
	if (optavp) {
		struct avp * optavp_cpy = NULL;
		struct avp_hdr *opt_hdr, *optcpy_hdr;
		
		/* Create the Failed-AVP AVP */
		CHECK_FCT( fd_msg_avp_new( dict_avp_FAVP, 0, &avp_FAVP ) );
		
		/* Create a new AVP with a copy of the data of the invalid or missing AVP */
		optavp_cpy = optavp;
		CHECK_FCT( fd_msg_avp_new( NULL, AVPFL_SET_BLANK_VALUE | AVPFL_SET_RAWDATA_FROM_AVP, &optavp_cpy) );
		
		CHECK_FCT( fd_msg_avp_hdr(optavp, &opt_hdr) );
		CHECK_FCT( fd_msg_avp_hdr(optavp_cpy, &optcpy_hdr) );
		memcpy(optcpy_hdr, opt_hdr, sizeof(struct avp_hdr));
		
		/* Add the passed AVP inside it */
		CHECK_FCT( fd_msg_avp_add( avp_FAVP, MSG_BRW_LAST_CHILD, optavp_cpy ) );
		
		/* And add to the message */
		CHECK_FCT( fd_msg_avp_add( msg, MSG_BRW_LAST_CHILD, avp_FAVP ) );
	}
	
	
	/* Deal with the 'E' bit and the error message */
	switch (rc_val / 1000) {
		case 1:	/* Informational */
		case 2: /* Success */
			/* Nothing special here: no E bit, no error message unless one is specified */
			break;
			
		case 3: /* Protocol Errors */
			set_e_bit = 1;
			std_err_msg = 1;
			break;
			
		case 4: /* Transcient Failure */
		case 5: /* Permanent Failure */
		default:
			std_err_msg = 1;
			break;
			
	}
	
	{
		struct msg_hdr * hdr = NULL;
		
		CHECK_FCT(  fd_msg_hdr( msg, &hdr )  );
		
		if (set_e_bit)
			hdr->msg_flags |= CMD_FLAG_ERROR;
		else
			hdr->msg_flags &= ~ CMD_FLAG_ERROR;
	}
	
	if (std_err_msg || errormsg) {
		/* Add the Error-Message AVP */
		
		CHECK_FCT( fd_msg_avp_new( dict_avp_EM, 0, &avp_EM ) );

		/* Set its value */
		memset(&val, 0, sizeof(val));
		
		if (errormsg) {
			val.os.data = (uint8_t *)errormsg;
			val.os.len  = strlen(errormsg);
		} else {
			val.os.data = (uint8_t *)rescode;
			val.os.len  = strlen(rescode);
		}
		CHECK_FCT( fd_msg_avp_setvalue( avp_EM, &val ) );

		/* Add it to the message */
		CHECK_FCT( fd_msg_avp_add( msg, MSG_BRW_LAST_CHILD, avp_EM ) );
	}
	
	return 0;
}

static int fd_msg_send_int( struct msg ** pmsg, void (*anscb)(void *, struct msg **), void * data, void (*expirecb)(void *, DiamId_t, size_t, struct msg **), const struct timespec *timeout )
{
	struct msg_hdr *hdr;
	DiamId_t diamid;
	
	/* Save the callback in the message, with the timeout */
	CHECK_FCT(  fd_msg_anscb_associate( *pmsg, anscb, data, expirecb, timeout )  );
	
	/* If this is a new request, call the HOOK_MESSAGE_LOCAL hook */
	if ( (fd_msg_hdr(*pmsg, &hdr) == 0)
	 &&  (hdr->msg_flags & CMD_FLAG_REQUEST)
	 &&  (fd_msg_source_get(*pmsg, &diamid, NULL) == 0)
	 &&  (diamid == NULL)) {
		fd_hook_call(HOOK_MESSAGE_LOCAL, *pmsg, NULL, NULL, fd_msg_pmdl_get(*pmsg));
	}
		
	/* Post the message in the outgoing queue */
	CHECK_FCT( fd_fifo_post(fd_g_outgoing, pmsg) );
	
	return 0;
}

/* Send a message and optionally register a callback for an answer */
int fd_msg_send ( struct msg ** pmsg, void (*anscb)(void *, struct msg **), void * data )
{
	TRACE_ENTRY("%p %p %p", pmsg, anscb, data);
	CHECK_PARAMS( pmsg );
	
	return fd_msg_send_int(pmsg, anscb, data, NULL, NULL);
}

/* The variation of the same function with a timeout callback */
int fd_msg_send_timeout ( struct msg ** pmsg, void (*anscb)(void *, struct msg **), void * data, void (*expirecb)(void *, DiamId_t, size_t, struct msg **), const struct timespec *timeout )
{
	TRACE_ENTRY("%p %p %p %p %p", pmsg, anscb, data, expirecb, timeout);
	CHECK_PARAMS( pmsg && expirecb && timeout );
	
	return fd_msg_send_int(pmsg, anscb, data, expirecb, timeout);
}


/* Parse a message against our dictionary, and in case of error log and eventually build the error reply -- returns the parsing status */
int fd_msg_parse_or_error( struct msg ** msg, struct msg **error)
{
	int ret = 0;
	struct msg * m;
	struct msg_hdr * hdr = NULL;
	struct fd_pei	pei;
	
	TRACE_ENTRY("%p", msg);
	
	CHECK_PARAMS(msg && *msg && error);
	m = *msg;
	*error = NULL;
	
	/* Parse the message against our dictionary */
	ret = fd_msg_parse_rules ( m, fd_g_config->cnf_dict, &pei);
	if 	((ret != EBADMSG) 	/* Parsing grouped AVP failed / Conflicting rule found */
		&& (ret != ENOTSUP))	/* Command is not supported / Mandatory AVP is not supported */
		return ret; /* 0 or another error */
	
	/* Log */
	fd_hook_call(HOOK_MESSAGE_PARSING_ERROR, m, NULL, pei.pei_message ?: pei.pei_errcode, fd_msg_pmdl_get(m));
	
	CHECK_FCT( fd_msg_hdr(m, &hdr) );
	
	/* Now create an answer error if the message is a query */
	if (hdr->msg_flags & CMD_FLAG_REQUEST) {
		
		/* Create the error message */
		CHECK_FCT( fd_msg_new_answer_from_req ( fd_g_config->cnf_dict, &m, pei.pei_protoerr ? MSGFL_ANSW_ERROR : 0 ) );
		
		/* Set the error code */
		CHECK_FCT( fd_msg_rescode_set(m, pei.pei_errcode, pei.pei_message, pei.pei_avp, 1 ) );
		
		/* free the pei AVP to avoid memory leak */
		if (pei.pei_avp_free) {
			fd_msg_free(pei.pei_avp);
		}
		
		*msg = NULL;
		*error = m;
		
	} else {
		do { /* Rescue error messages */
			struct avp * avp;
			union avp_value * rc = NULL;
			
			/* Search the Result-Code AVP */
			CHECK_FCT_DO(  fd_msg_browse(*msg, MSG_BRW_FIRST_CHILD, &avp, NULL), break  );
			while (avp) {
				struct avp_hdr * ahdr;
				CHECK_FCT_DO(  fd_msg_avp_hdr( avp, &ahdr ), break  );
				
				if ((ahdr->avp_code == AC_RESULT_CODE) && (! (ahdr->avp_flags & AVP_FLAG_VENDOR)) ) {
					/* Parse this AVP */
					ASSERT( ahdr->avp_value );
					rc = ahdr->avp_value;
					break;
				}
				
				/* Go to next AVP */
				CHECK_FCT_DO(  fd_msg_browse(avp, MSG_BRW_NEXT, &avp, NULL), break  );
			}
			
			if (rc) {
				switch (rc->u32 / 1000) {
					case 1:	/* 1xxx : Informational */
					case 2:	/* 2xxx : Sucess */
						/* In these cases, we want the message to validate the ABNF, so we will discard the bad message */
						break;
						
					default: /* Other errors */
						/* We let the application decide what to do with the message, we rescue it */
						*error = m;
				}
			}
		} while (0);
	}
	
	return EBADMSG; /* We convert ENOTSUP to EBADMSG as well */
}
