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


int app_sip_SAR_cb( struct msg ** msg, struct avp * paramavp, struct session * sess, void * opaque, enum disp_action * act)
{
	TRACE_ENTRY("%p %p %p %p", msg, paramavp, sess, act);
	
	struct msg *ans, *qry;
	struct avp *avp;
	struct avp_hdr *avphdr, *sipaorhdr, *usernamehdr, *sipuserdataalreadyavailable;
	union avp_value value;
	int ret=0, assignment_type=0, got_datatype=1;
	
	
	struct listdatatype
	{
		struct fd_list datatype;
		char * type;
		size_t typelen;
	};
	
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
		if(avp!=NULL)
		{
			CHECK_FCT( fd_msg_avp_hdr( avp, &sipaorhdr )  );
		}
		else
			sipaorhdr=NULL;
		
	}
	
	//We check if we have a username AVP
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
				
				if(sipaorhdr!=NULL)
				{
					//We must check that this user can use this SIP-AOR
					ret=check_sipaor(usernamehdr->avp_value->os.data, usernamehdr->avp_value->os.len,(const char *) sipaorhdr->avp_value->os.data,sipaorhdr->avp_value->os.len);
					
					if(ret==0)
					{
						//The SIP-AOR and Username are ok!
						ret=clear_pending_flag(usernamehdr->avp_value->os.data, usernamehdr->avp_value->os.len);
						if(ret==2)
						{
							TRACE_DEBUG(INFO,"ERROR: We couldn't clear the flag of pending authentication.'");
							strcpy(result,"DIAMETER_UNABLE_TO_COMPLY");
							goto out;
						}
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
				else
				{
					TRACE_DEBUG(INFO,"ERROR: There is no SIP-AOR AVP!'");
					strcpy(result,"DIAMETER_ERROR_IDENTITIES_DONT_MATCH");
					goto out;
				}
			}
		}
		else
		{
			if(sipaorhdr!=NULL)
			{//If we have a SIP-AOR, we want the user to check it.
				strcpy(result,"DIAMETER_USER_NAME_REQUIRED");
				goto out;
			}
			usernamehdr=NULL;
		}
			
	}
	
	//We get the SIP_Server_Assignment_Type
	{
		CHECK_FCT( fd_msg_search_avp ( qry, sip_dict.SIP_Server_Assignment_Type, &avp) );
		CHECK_FCT( fd_msg_avp_hdr( avp, &avphdr )  );
		
		assignment_type=avphdr->avp_value->i32;
	}
	
	
	
	
	
	//We get SIP_User_Data_Already_Available AVP
	{
		CHECK_FCT( fd_msg_search_avp ( qry, sip_dict.SIP_User_Data_Already_Available, &avp) );
		CHECK_FCT( fd_msg_avp_hdr( avp, &sipuserdataalreadyavailable )  );
	}
	
	if(assignment_type==1 || assignment_type==2)
	{//registration & re-registration
		if(count_avp(qry,CODE_SIP_AOR,0)==1)
		{
			
			if(sipuserdataalreadyavailable->avp_value->i32==0)
			{//Data not available, we must provide it
				ret=add_user_datatype(sipaorhdr->avp_value->os.data, sipaorhdr->avp_value->os.len,ans);
				
				if(ret==0)
				{
					//We found and added datatype
					got_datatype=1;
				}
				else if(ret==1)
				{
					//No data type was found
					got_datatype=0;
				}
				else
				{//error
				//We couldn't make the request, we must stop process!
				strcpy(result,"DIAMETER_UNABLE_TO_COMPLY");
				goto out;
				}
			}
			strcpy(result,"DIAMETER_SUCCESS");
			goto out;
		}
		else
		{//There is more than 1 SIP-AOR AVP
			strcpy(result,"DIAMETER_AVP_OCCURS_TOO_MANY_TIMES");
			goto out;
		}
	}
	else if(assignment_type==3)
	{//Unregistered user
		
		//TODO:place user unknown here!
		if(count_avp(qry,CODE_SIP_AOR,0)==1)
		{
			if(sipuserdataalreadyavailable->avp_value->i32==0)
			{//Data not available, we must provide it
				if(got_datatype==1)
				{
					ret=add_user_datatype(sipaorhdr->avp_value->os.data, sipaorhdr->avp_value->os.len,ans);
					
					if(ret==0)
					{
						//We found and added datatype
						got_datatype=1;
					}
					else if(ret==1)
					{
						//No data type was found
						got_datatype=0;
					}
					else
					{//error
						//We couldn't make the request, we must stop process!
						strcpy(result,"DIAMETER_UNABLE_TO_COMPLY");
						goto out;
					}
				}
			}
			strcpy(result,"DIAMETER_SUCCESS");
			goto out;
		}
		else
		{//There is more than 1 SIP-AOR AVP
			strcpy(result,"DIAMETER_AVP_OCCURS_TOO_MANY_TIMES");
			goto out;
		}
		
		if(sipuserdataalreadyavailable->avp_value->i32==0)
		{//Data not available, we must provide it
			if(got_datatype==1)
			{
				ret=add_user_datatype(sipaorhdr->avp_value->os.data, sipaorhdr->avp_value->os.len,ans);
				
				if(ret==0)
				{
					//We found and added datatype
					got_datatype=1;
				}
				else if(ret==1)
				{
					//No data type was found
					got_datatype=0;
				}
				else
				{//error
					//We couldn't make the request, we must stop process!
					strcpy(result,"DIAMETER_UNABLE_TO_COMPLY");
					goto out;
				}
			}
		}
		
		CHECK_FCT( fd_msg_search_avp ( qry, sip_dict.SIP_Server_URI, &avp) );
		if(avp!=NULL)
		{
			CHECK_FCT( fd_msg_avp_hdr( avp, &avphdr ));
			
			set_real_sipserver_uri(usernamehdr->avp_value->os.data, usernamehdr->avp_value->os.len, avphdr->avp_value->os.data,avphdr->avp_value->os.len);
			strcpy(result,"DIAMETER_SUCCESS");
			goto out;
		}
		else
		{
			TRACE_DEBUG(INFO,"There is no SIP_Server_URI AVP in this Unregistered User Request!");
			strcpy(result,"DIAMETER_UNABLE_TO_COMPLY");
			goto out;
		}
	}
	else if(assignment_type==4 || assignment_type==5 || assignment_type==11 || assignment_type==8)
	{//Unregistered user
		
		if(got_datatype==1)
		{
			if(sipuserdataalreadyavailable->avp_value->i32==0)
			{//Data not available, we must provide it
				
				ret=add_user_datatype(sipaorhdr->avp_value->os.data, sipaorhdr->avp_value->os.len,ans);
				
				if(ret==0)
				{
					//We found and added datatype
					got_datatype=1;
				}
				else if(ret==1)
				{
					//No data type was found
					got_datatype=0;
				}
				else
				{//error
					//We couldn't make the request, we must stop process!
					strcpy(result,"DIAMETER_UNABLE_TO_COMPLY");
					goto out;
				}
			}
		}
		
		if(sipaorhdr==NULL)
		{
			//We don't have any SIP-AOR to unregister, this is strange!'
			TRACE_DEBUG(INFO, "There was no SIP-AOR in this request, we can't proceed request!'");
			strcpy(result,"DIAMETER_UNABLE_TO_COMPLY");
			goto out;
		}
		else
		{
			//TODO: unregister SIP-Server-URI for all SIP-AOR
		}
		strcpy(result,"DIAMETER_SUCCESS");
		goto out;
	}
	else if(assignment_type==6 || assignment_type==7)
	{
		
		
		CHECK_FCT( fd_msg_search_avp ( qry, sip_dict.SIP_Server_URI, &avp) );
		if(avp!=NULL)
		{
			CHECK_FCT( fd_msg_avp_hdr( avp, &avphdr ));
			
			//TODO: set SIP server URI for each AOR
			
			//TODO: unregister all SIP-AOR provided
			
			//set_real_sipserver_uri(usernamehdr->avp_value->os.data, usernamehdr->avp_value->os.len, avphdr->avp_value->os.data,avphdr->avp_value->os.len);
			strcpy(result,"DIAMETER_SUCCESS");
			goto out;
		}
		else
		{
			
			
			//TODO: unregister all SIP-AOR provided
			
			//TODO: clear sip server uri in database for the sip-aor
			
			
			TRACE_DEBUG(INFO,"There is no SIP_Server_URI AVP in this Deregistration User Request! We just unregister SIP-AOR");
			strcpy(result,"DIAMETER_SUCCESS_SERVER_NAME_NOT_STORED");
			goto out;
		}
	}
	else if(assignment_type==0)
	{
		
		
		CHECK_FCT( fd_msg_search_avp ( qry, sip_dict.SIP_Server_URI, &avp) );
		if(avp!=NULL)
		{
			unsigned char * sipserver_uri;
			size_t sipserverurilen;
			
			
			
			
			ret=get_sipserver_uri(sipaorhdr->avp_value->os.data, sipaorhdr->avp_value->os.len, &sipserver_uri, &sipserverurilen);
			
			if(ret==0)
			{//found
				CHECK_FCT( fd_msg_avp_hdr( avp, &avphdr ));
				
				
				
				
				if(strncmp((char *)avphdr->avp_value->os.data,(char *)sipserver_uri,sipserverurilen))
				{
					if(got_datatype==1)
					{
						if(sipuserdataalreadyavailable->avp_value->i32==0)
						{//Data not available, we must provide it
							
							ret=add_user_datatype(sipaorhdr->avp_value->os.data, sipaorhdr->avp_value->os.len,ans);
							
							if(ret==0)
							{
								//We found and added datatype
								got_datatype=1;
							}
							else if(ret==1)
							{
								//No data type was found
								got_datatype=0;
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
				else
				{//error
					TRACE_DEBUG(FULL,"SIP_Server_URI is different from the one in database");
					strcpy(result,"DIAMETER_UNABLE_TO_COMPLY");
					goto out;
				}
			}
			else
			{
				TRACE_DEBUG(FULL,"SIP_Server_URI is different from the one in database");
				strcpy(result,"DIAMETER_UNABLE_TO_COMPLY");
				goto out;
			}
			
			
			
			
			
			
			
			//set_real_sipserver_uri(usernamehdr->avp_value->os.data, usernamehdr->avp_value->os.len, avphdr->avp_value->os.data,avphdr->avp_value->os.len);
			strcpy(result,"DIAMETER_SUCCESS");
			goto out;
		}
		else
		{
			TRACE_DEBUG(INFO, "There was no SIP_Server_URI in this request, we can't proceed request!'");
			strcpy(result,"DIAMETER_UNABLE_TO_COMPLY");
			goto out;
		}
	}
	else if(assignment_type==9 || assignment_type==10)
	{
		if(count_avp(qry, CODE_SIP_AOR,0)==1)
		{
			//TODO: remove SIP-server URI for sip_aor
			//TODO: unregister it
			strcpy(result,"DIAMETER_SUCCESS");
			goto out;
		}
		else
		{//There is more than 1 SIP-AOR AVP
			strcpy(result,"DIAMETER_AVP_OCCURS_TOO_MANY_TIMES");
			goto out;
		}
	}
	out:
	CHECK_FCT( fd_msg_rescode_set( ans, result, NULL, NULL, 1 ) );
	
	
	//fd_msg_dump_walk(INFO,ans);
	
	CHECK_FCT( fd_msg_send( msg, NULL, NULL ));
	
	
	
	return 0;
}
