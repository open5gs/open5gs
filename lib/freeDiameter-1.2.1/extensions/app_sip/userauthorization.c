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


int app_sip_UAR_cb( struct msg ** msg, struct avp * paramavp, struct session * sess, void * opaque, enum disp_action * act)
{
	TRACE_ENTRY("%p %p %p %p", msg, paramavp, sess, act);
	
	struct msg *ans, *qry;
	struct avp *avp, *groupedavp=NULL;
	struct avp_hdr *avphdr, *sipaorhdr, *visitednethdr, *usernamehdr;
	union avp_value value;
	int ret=0, hascap=0;
	
	//Result_Code to return in the answer
	char result[55];
	
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
	
	//Retrieve SIP-AOR
	{
		CHECK_FCT( fd_msg_search_avp ( qry, sip_dict.SIP_AOR, &avp) );
		CHECK_FCT( fd_msg_avp_hdr( avp, &sipaorhdr )  );
	}
	
	//We check if we have a User-Name AVP
	{
		CHECK_FCT( fd_msg_search_avp ( qry, sip_dict.User_Name, &avp) );
		if(avp!=NULL)
		{
			CHECK_FCT( fd_msg_avp_hdr( avp, &usernamehdr )  );
			
			ret=get_password(usernamehdr->avp_value->os.data, usernamehdr->avp_value->os.len, NULL);
			
			
			if(ret==1)
			{//not found
				strcpy(result,"DIAMETER_ERROR_USER_UNKNOWN");
				goto out;
			}
			else if(ret==2)
			{//error
				//We couldn't make the request, we must stop process!
				strcpy(result,"DIAMETER_UNABLE_TO_COMPLY");
				goto out;
			}
			else if(ret==0)
			{//found
				
				//We must check that this user can use this SIP-AOR
				ret=check_sipaor(usernamehdr->avp_value->os.data, usernamehdr->avp_value->os.len, (const char *)sipaorhdr->avp_value->os.data,sipaorhdr->avp_value->os.len);
				
				if(ret==0)
				{
					//The SIP-AOR and Username are ok!
				}
				else if(ret==1)
				{//not found
					strcpy(result,"DIAMETER_ERROR_IDENTITIES_DONT_MATCH");
					goto out;
				}
				else
				{//error
					//We couldn't make the request, we must stop process!
					strcpy(result,"DIAMETER_UNABLE_TO_COMPLY");
					goto out;
				}
			}
		}
	}
	
	//We check if we have a SIP-Visited-Network-Id AVP
	{
		CHECK_FCT( fd_msg_search_avp ( qry, sip_dict.SIP_Visited_Network_Id, &avp) );
		
		if(avp!=NULL)
		{
			CHECK_FCT( fd_msg_avp_hdr( avp, &visitednethdr )  );
		}
		else
			visitednethdr=NULL;
	}
	
	//We check if we have a SIP_User_Authorization_Type
	{
		int auth_type;
		
		CHECK_FCT( fd_msg_search_avp ( qry, sip_dict.SIP_User_Authorization_Type, &avp) );
		
		if(avp!=NULL)
		{
			CHECK_FCT( fd_msg_avp_hdr( avp, &avphdr )  );
			auth_type=avphdr->avp_value->i32;
			
			if(auth_type==0 || auth_type==2)
			{
				if(visitednethdr!=NULL)
				{
					/*
					If there is a SIP-Visited-Network-Id AVP in the Diameter UAR message,
					and the SIP-User-Authorization-Type AVP value received in the
					Diameter UAR message is set to REGISTRATION or REGISTRATION&
					CAPABILITIES, then the Diameter server SHOULD verify whether the user
					is allowed to roam into the network specified in the
					SIP-Visited-Network-Id AVP in the Diameter UAR message.  If the user
					is not allowed to roam into that network, the Diameter AAA server
					MUST set the Result-Code AVP value in the Diameter UAA message to
					DIAMETER_ERROR_ROAMING_NOT_ALLOWED.
					*/
					ret=allow_roaming(usernamehdr->avp_value->os.data, usernamehdr->avp_value->os.len, (const char *)visitednethdr->avp_value->os.data,visitednethdr->avp_value->os.len);
					
					if(ret==0)
					{
						//This user can come in this network
					}
					else if(ret==1)
					{
						strcpy(result,"DIAMETER_ERROR_ROAMING_NOT_ALLOWED");
						goto out;
					}
					else
					{
						strcpy(result,"DIAMETER_UNABLE_TO_COMPLY");
						goto out;
					}
					
				}
				/*
				If the SIP-User-Authorization-Type AVP value received in the Diameter
				UAR message is set to REGISTRATION or REGISTRATION&CAPABILITIES, then
				the Diameter server SHOULD verify whether the SIP-AOR AVP value is
				authorized to register in the Home Realm.  Where the SIP AOR is not
				authorized to register in the Home Realm, the Diameter server MUST
				set the Result-Code AVP to DIAMETER_AUTHORIZATION_REJECTED and send
				it in a Diameter UAA message.
				*/
				ret=exist_username(sipaorhdr->avp_value->os.data, sipaorhdr->avp_value->os.len);
				if(ret==0)
				{
					//The SIP_AOR exists in this realm
				}
				else if(ret==1)
				{
					strcpy(result,"DIAMETER_AUTHORIZATION_REJECTED");
					goto out;
				}
				else
				{
					strcpy(result,"DIAMETER_UNABLE_TO_COMPLY");
					goto out;
				}
			}
		}
		else
			auth_type=0; //Registration only (default value if absent)
		
		
		//Let's look for the SIP_Server_URI'
		unsigned char *sipserver_uri;
		size_t sipserverurilen;
		
		ret=get_sipserver_uri(sipaorhdr->avp_value->os.data, sipaorhdr->avp_value->os.len, &sipserver_uri, &sipserverurilen);
		
		if(ret==0)
		{//We know a SIP Server so we must provide it
			//We only add this AVP when we have registration or deregistration
			if(auth_type==0 ||auth_type==1)
			{
				CHECK_FCT( fd_msg_avp_new ( sip_dict.SIP_Server_URI, 0, &avp ) );
				value.os.data=sipserver_uri;
				value.os.len=sipserverurilen;
				CHECK_FCT( fd_msg_avp_setvalue( avp, &value ) );
				CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
			}
		}
		else if(ret==1)
		{
			sipserver_uri=NULL;
			sipserverurilen=0;
			
		}
		else
		{
			strcpy(result,"DIAMETER_UNABLE_TO_COMPLY");
			goto out;
		}
		
		//If we have a REGISTRATION or REGISTRATION & CAPABILITIES
		if(auth_type==0 ||auth_type==2)
		{
			//Adding SIP-Server-Capabilities
			CHECK_FCT( fd_msg_avp_new ( sip_dict.SIP_Server_Capabilities, 0, &groupedavp ) );
			
			ret=get_sipserver_cap(sipaorhdr->avp_value->os.data, sipaorhdr->avp_value->os.len,&groupedavp);
			
			if(ret==0)
			{
				CHECK_FCT( fd_msg_avp_add ( ans, MSG_BRW_LAST_CHILD, groupedavp) );
				hascap=1;
			}
			else if(ret==1)
			{
				
				hascap=0;
			}
			else
			{
				CHECK_FCT( fd_msg_free( groupedavp ) );
				strcpy(result,"DIAMETER_UNABLE_TO_COMPLY");
				goto out;
			}
		}
		
		
		
		if(auth_type==0)
		{//Registration
			
			if(sipserver_uri==NULL)
			{
				strcpy(result,"DIAMETER_FIRST_REGISTRATION");
			}
			else
			{
				if(hascap==1)
				{
					strcpy(result,"DIAMETER_SERVER_SELECTION");
				}
				else
				{
					//We free the Capabilities AVP because we didn't found any'
					if(groupedavp!=NULL)
						CHECK_FCT( fd_msg_free( groupedavp ) );
					strcpy(result,"DIAMETER_SUBSEQUENT_REGISTRATION");
				}
			}
		}
		else if(auth_type==2)
		{//Registration & Capabilities
			//We let the groupedavp in the message even if there is no capabilities
			
			if(hascap==0)
			{
				CHECK_FCT( fd_msg_avp_add ( ans, MSG_BRW_LAST_CHILD, groupedavp) );
			}
			strcpy(result,"DIAMETER_SUCCESS");	
		}
		else if(auth_type==1)
		{//Deregistration
			if(sipserver_uri==NULL)
			{
				strcpy(result,"DIAMETER_ERROR_IDENTITY_NOT_REGISTERED");
			}
			else
			{
				strcpy(result,"DIAMETER_SUCCESS");	
			}
		}
		else
		{//We should never be here!!
			strcpy(result,"DIAMETER_UNABLE_TO_COMPLY");
			goto out;
		}
	}
	
	
out:
	CHECK_FCT( fd_msg_rescode_set( ans, result, NULL, NULL, 1 ) );
	
	
	//fd_msg_dump_walk(INFO,ans);
	
	CHECK_FCT( fd_msg_send( msg, NULL, NULL ));
	
	
	
	return 0;
}
