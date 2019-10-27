/*********************************************************************************************************
* Software License Agreement (BSD License)                                                               *
* Author: Alexandre Westfahl <awestfahl@freediameter.net>						 *
*													 *
* Copyright (c) 2010, Alexandre Westfahl, Teraoka Laboratory (Keio University), and the WIDE Project. 	 *		
*													 *
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
* * Neither the name of the Teraoka Laboratory nor the 							 *
*   names of its contributors may be used to endorse or 						 *
*   promote products derived from this software without 						 *
*   specific prior written permission of Teraoka Laboratory 						 *
*   													 *
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
#include "app_sip.h"

//This callback is specific to SUSCRIBER LOCATOR. We must look for the "serving" SIP server
int app_sip_SL_LIR_cb( struct msg ** msg, struct avp * paramavp, struct session * sess, void * opaque, enum disp_action * act)
{
	TRACE_ENTRY("%p %p %p %p", msg, paramavp, sess, act);
	
	struct msg *ans, *qry;
	struct avp *avp;
	struct avp_hdr *avphdr;
	union avp_value value;
	
	//Result_Code to return in the answer
	char result[55];
	int ret=0;
	
	
	if (msg == NULL)
		return EINVAL;

	
	
	// Create answer header 
	qry = *msg;
	CHECK_FCT( fd_msg_new_answer_from_req ( fd_g_config->cnf_dict, msg, 0 ) );
	ans = *msg;
	
	
	//Add the Auth-Application-Id 
	{
		CHECK_FCT( fd_msg_avp_new ( sip_dict.Auth_Application_Id, 0, &avp ) );
		value.i32 = 6;
		CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
		CHECK_FCT( fd_msg_avp_add ( ans, MSG_BRW_LAST_CHILD, avp) );
	}
	
	// Add the Auth-Session-State AVP 
	{
		
		CHECK_FCT( fd_msg_search_avp ( qry, sip_dict.Auth_Session_State, &avp) );
		CHECK_FCT( fd_msg_avp_hdr( avp, &avphdr )  );
		
		CHECK_FCT( fd_msg_avp_new ( sip_dict.Auth_Session_State, 0, &avp ) );
		CHECK_FCT( fd_msg_avp_setvalue( avp, avphdr->avp_value ) );
		CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
	}
	
	
	// Add the Redirect Host AVP 
	{
		CHECK_FCT( fd_msg_search_avp ( qry, sip_dict.SIP_AOR, &avp) );
		CHECK_FCT( fd_msg_avp_hdr( avp, &avphdr )  );
		size_t diameterurilen;
		char * diameter_uri=NULL;
		
		
		
		ret=get_diameter_uri(avphdr->avp_value->os.data, avphdr->avp_value->os.len, &diameter_uri, &diameterurilen);
		
		//found
		if(ret==0)
		{
		  if(diameter_uri==NULL)
		  {
			  //There is a problem because we must get a Diameter_URI here
			  strcpy(result,"DIAMETER_UNABLE_TO_COMPLY");
			  goto out;
		  }
		  else
		  {
		    CHECK_FCT( fd_msg_avp_new ( sip_dict.Redirect_Host, 0, &avp ) );
		    value.os.data=(unsigned char *)diameter_uri;
		    value.os.len=diameterurilen;
		    CHECK_FCT( fd_msg_avp_setvalue( avp, &value ) );
		    CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
		    
		    CHECK_FCT( fd_msg_avp_new ( sip_dict.Redirect_Host_Usage, 0, &avp ) );
		    value.i32=ALL_USER; //All the request about the same user must be sent to this server
		    CHECK_FCT( fd_msg_avp_setvalue( avp, &value ) );
		    CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
		    strcpy(result,"DIAMETER_SUCCESS");
		  }
		}
		else if(ret==1)
		{//not found
			//We don't know this SIP_AOR in SL
			strcpy(result,"DIAMETER_ERROR_USER_UNKNOWN");
			goto out;
		}
		else
		{// returned 2, impossible to make request
			//We couldn't make the request, we must stop process!
			strcpy(result,"DIAMETER_UNABLE_TO_COMPLY");
			goto out;
		}
	}
	
	
out:
	CHECK_FCT( fd_msg_rescode_set( ans, result, NULL, NULL, 1 ) );
	
	
	
	CHECK_FCT( fd_msg_send( msg, NULL, NULL ));
	
	
	
	return 0;
}
