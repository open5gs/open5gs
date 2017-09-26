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

struct sess_state
{
	char *nonce;
};


int app_sip_MAR_cb( struct msg ** msg, struct avp * paramavp, struct session * sess, void * opaque, enum disp_action * act)
{
	struct msg *ans, *qry;
	struct avp *avp, *a2, *authdataitem;
	//struct msg_hdr * header = NULL;
	struct avp_hdr * avphdr=NULL, *avpheader=NULL, *avpheader_auth=NULL,*digestheader=NULL;
	union avp_value val;
	int found_cnonce=0;
	struct avp * tempavp=NULL,*sipAuthentication=NULL,*sipAuthenticate=NULL;
	char * result;
	char password[51];
	int idx=0, number_of_auth_items=0,i=0, ret=0;
	//Flags and variables for Database
	int sipurinotstored=0, authenticationpending=0; 
	size_t querylen=0, usernamelen=0;
	char *query=NULL;
	unsigned char *username=NULL;
	
	//The nonce we will store and retrieve in session
	struct sess_state *storednonce=NULL;
	
	
	TRACE_ENTRY("%p %p %p %p", msg, paramavp, sess, act);
	
	if (msg == NULL)
		return EINVAL;

	
	/* Create answer header */
	qry = *msg;
	CHECK_FCT( fd_msg_new_answer_from_req ( fd_g_config->cnf_dict, msg, 0 ) );
	ans = *msg;
	


	/* Add the appropriate command code & Auth-Application-Id 
	{
		
		CHECK_FCT( fd_msg_hdr ( *msg, &header ) );
		header->msg_flags = CMD_FLAG_PROXIABLE;
		header->msg_code = 286;
		header->msg_appl = 6;
		
		
		// Add the Auth-Application-Id 
		{
			CHECK_FCT( fd_msg_avp_new ( sip_dict.Auth_Application_Id, 0, &avp ) );
			ASSERT(avp);
			val.i32 = header->msg_appl;
			CHECK_FCT( fd_msg_avp_setvalue ( avp, &val ) );
			CHECK_FCT( fd_msg_avp_add ( ans, MSG_BRW_LAST_CHILD, avp) );
		}
	}*/
	
	
	/* Add the Auth-Session-State AVP */
	{
		
		CHECK_FCT( fd_msg_search_avp ( qry, sip_dict.Auth_Session_State, &avp) );
		CHECK_FCT( fd_msg_avp_hdr( avp, &avphdr )  );
		
		CHECK_FCT( fd_msg_avp_new ( sip_dict.Auth_Session_State, 0, &avp ) );
		CHECK_FCT( fd_msg_avp_setvalue( avp, avphdr->avp_value ) );
		CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
	}

	
	
	/* Check if method is REGISTER then User-Name must be present */
	{
		
		CHECK_FCT( fd_msg_search_avp ( qry, sip_dict.SIP_Method, &avp) );
		CHECK_FCT( fd_msg_avp_hdr( avp, &avpheader ));
		

		char *method=NULL;
		
		CHECK_FCT( fd_msg_search_avp ( qry, sip_dict.User_Name, &avp) );
		if(avp!=NULL)
		{
			
			int not_found=1;
			MYSQL_RES *res=NULL;
			MYSQL_ROW row;
			
			CHECK_FCT( fd_msg_avp_hdr( avp, &avphdr )  );
			
			
			
			
			if((strncmp((const char *)avpheader->avp_value->os.data,"REGISTER",avpheader->avp_value->os.len)==0))
			{
				not_found=1;
				
				//TODO TODO TODO TODO TODO TODO TODO: maybe doesn't work!!'
				ret=get_password(avphdr->avp_value->os.data, avphdr->avp_value->os.len, (char *)&password);
				
				if(ret>1)
				{
					//We couldn't make the request
					result="DIAMETER_UNABLE_TO_COMPLY";
					goto out;
				}
				not_found=ret;
				
				if(not_found)
				{
					TRACE_DEBUG(FULL,"The user %s doesn't exist!",username);
					result="DIAMETER_ERROR_USER_UNKNOWN";
					free(username);	
					goto out;
				}
			
			
				//We allocate the double size of username because at worst it can be all quotes
				username=malloc(avphdr->avp_value->os.len*2+1);
				//We purify username not to have forbidden characters
				usernamelen=mysql_real_escape_string(conn, (char *)username, (const char *)avphdr->avp_value->os.data, avphdr->avp_value->os.len);
				
				//Now that we know the user exist, we get the list of AOR owned by this user
				querylen=SQL_GETSIPAOR_LEN + usernamelen;
				query = malloc(querylen+2);
				snprintf(query, querylen+1, SQL_GETSIPAOR, username);
			
				//We make the query
				request_mysql(query);
				res=mysql_use_result(conn);
				if(res==NULL)
				{
					//We couldn't make the request
					result="DIAMETER_UNABLE_TO_COMPLY";
					goto out;
				}
			
				//We retrieve the SIP-AOR from AVP to check if the user can us it
				CHECK_FCT( fd_msg_search_avp ( qry, sip_dict.SIP_AOR, &avp) );
				CHECK_FCT( fd_msg_avp_hdr( avp, &avphdr )  );
			
				not_found=1;
				while ((row = mysql_fetch_row(res)) != NULL)
				{
					if(strncmp((const char *)avphdr->avp_value->os.data,row[0],avphdr->avp_value->os.len)==0)
					{
						not_found=0;
						break;
					}
				}
				mysql_free_result(res);
				free(query);
      				
				if(not_found)
				{
					TRACE_DEBUG(FULL,"The user %s can't use this SIP-AOR!",username);
					result="DIAMETER_ERROR_IDENTITIES_DONT_MATCH";
					free(username);	
					goto out;
				}
				
			}

			CHECK_FCT( fd_msg_search_avp ( qry, sip_dict.SIP_Server_URI, &avp) );
			CHECK_FCT( fd_msg_avp_hdr( avp, &avphdr ));
			
			if(avphdr!=NULL)
			{
				unsigned char *sipuri=NULL;
				int sipurilen=0;

				//We allocate the double size of SIP-URI because at worst it can be all quotes
				CHECK_MALLOC(sipuri=malloc(avphdr->avp_value->os.len*2+1));
				//We purify SIP-URI not to have forbidden characters
				sipurilen=mysql_real_escape_string(conn, (char *)sipuri, (const char *)avphdr->avp_value->os.data, avphdr->avp_value->os.len);
				
				
				//We get the SIP-URI assignated to the user
				querylen=SQL_GETSIPURI_LEN + usernamelen;
				CHECK_MALLOC(query = malloc(querylen+2));
				snprintf(query, querylen+1, SQL_GETSIPURI, username);

				//We make the query
				request_mysql(query);
				res=mysql_use_result(conn);
				if(res==NULL)
				{
					//We couldn't make the request
					result="DIAMETER_UNABLE_TO_COMPLY";
					goto out;
				}
				not_found=1;
				while ((row = mysql_fetch_row(res)) != NULL)
				{
					if(strncmp((const char *)avphdr->avp_value->os.data,row[0],avphdr->avp_value->os.len)==0)
					{
						not_found=0;
						break;
					}
				}
				mysql_free_result(res);
				free(query);

				if(not_found)
				{
					//Temporary
					set_sipserver_uri(username, usernamelen, sipuri,sipurilen);
					
					
					set_pending_flag(username, usernamelen);
					
					
					authenticationpending=1;
				}
				free(sipuri);
				
			}
			else
				sipurinotstored=1;

		}
		else
		{
			result="DIAMETER_USER_NAME_REQUIRED";
			goto out;
		}

			
		free(method);

	}

	
	//TODO: remove loop for authdataitem because RFC say only one (wait for answer from Garcia)
	// How many Auth Data Items?
	CHECK_FCT( fd_msg_search_avp ( qry, sip_dict.SIP_Number_Auth_Items, &avp) );
	CHECK_FCT( fd_msg_avp_hdr( avp, &avpheader )  );
	
	
	if(avp!=NULL)
	{
		CHECK_FCT(fd_msg_search_avp ( qry, sip_dict.SIP_Auth_Data_Item, &avp));
		CHECK_FCT( fd_msg_avp_hdr( avp, &avphdr )  );
		
		if(avp!=NULL)
		{
			//First is Authentication Scheme
			CHECK_FCT(fd_msg_browse ( avp, MSG_BRW_FIRST_CHILD, &avp, NULL) );
			CHECK_FCT( fd_msg_avp_hdr( avp, &avphdr )  );
			
			//Digest-Authentication?
			if(avphdr->avp_value->i32==0)
			{
				
				for(idx=0;idx<avpheader->avp_value->i32;idx++)
				{
					//We look for SIP Auth items
					CHECK_FCT(fd_msg_browse ( avp, MSG_BRW_WALK, &avp, NULL) );
					
					if(avp!=NULL)
					{
						
						CHECK_FCT( fd_msg_avp_hdr( avp,&avphdr ));
		
						if(avphdr->avp_code==380) //We only create Auth-Data-Item to answer Auth-Data-Item
						{
							/* Add the Auth-Data-Item AVP */
							CHECK_FCT( fd_msg_avp_new ( sip_dict.SIP_Auth_Data_Item, 0, &authdataitem ) );
					
							/* Add the Authentication Scheme AVP */
							{
								CHECK_FCT( fd_msg_avp_new ( sip_dict.SIP_Authentication_Scheme, 0, &a2 ) );
								val.i32=0; //We only know Digest Authentication
								CHECK_FCT( fd_msg_avp_setvalue( a2, &val ) );
								CHECK_FCT( fd_msg_avp_add( authdataitem, MSG_BRW_LAST_CHILD, a2 ) );
							}
							
							//We need to know if there is a Cnonce attribute (only in the second MAR request)
							
							//CHECK_FCT(fd_msg_browse ( avp, MSG_BRW_WALK, &avp, NULL) );
							
							
							CHECK_FCT(fd_avp_search_avp (avp, sip_dict.Digest_CNonce, &a2 ));
							
							if(a2!=NULL)
								found_cnonce=1;
							else
								found_cnonce=0;
								
							if(!found_cnonce)
							{
								/*
								We are in the case of first access request so we need to challenge the user.
								*/
								TRACE_DEBUG(FULL,"First Authorization in progress...");

								/* Create a new session */ //this create a new session Id !!!
								//CHECK_FCT_DO( fd_sess_new( &sess, fd_g_config->cnf_diamid, "diamsip", 7), goto out );
								

								/* Create the SIP-Authenticate AVP */
								{
									CHECK_FCT( fd_msg_avp_new ( sip_dict.SIP_Authenticate, 0, &sipAuthenticate ) );
								}						

								/* Add the Digest QOP AVP */
								{
									CHECK_FCT( fd_msg_avp_new ( sip_dict.Digest_QOP, 0, &a2 ) );
									val.os.data=(unsigned char *)"auth";
									val.os.len=strlen((const char *)val.os.data);
									CHECK_FCT( fd_msg_avp_setvalue( a2, &val ) );
									CHECK_FCT( fd_msg_avp_add( sipAuthenticate, MSG_BRW_LAST_CHILD, a2 ) );
								}
								/* Add the Digest Nonce AVP */
								{
									uint8_t buffer[NONCE_SIZE];
									char nonce[NONCE_SIZE * 2 + 1];
							
								
									gcry_create_nonce ((uint8_t *)buffer, sizeof(buffer));
							
									for(i=0;i<NONCE_SIZE;i++)
										sprintf(&nonce[2 * i], "%2.2hhx", buffer[i]);
								
									CHECK_FCT( fd_msg_avp_new ( sip_dict.Digest_Nonce, 0, &a2 ) );
									
									
									//We store the nonce (storednonce structure) inside the session
									storednonce=malloc(sizeof(struct sess_state));
									memset(storednonce,0,sizeof(struct sess_state));
									CHECK_MALLOC(storednonce->nonce=malloc(NONCE_SIZE*2+1));
									memcpy(storednonce->nonce,(char *)nonce,NONCE_SIZE*2+1);
									CHECK_FCT( fd_sess_state_store ( ds_sess_hdl, sess, &storednonce ));  
									
									val.os.data=(unsigned char *)nonce;
									val.os.len=NONCE_SIZE * 2;
									
									CHECK_FCT( fd_msg_avp_setvalue( a2, &val ) );
									CHECK_FCT( fd_msg_avp_add( sipAuthenticate, MSG_BRW_LAST_CHILD, a2 ) );
								}
								/* Add the Digest Algorithm AVP */
								{
									CHECK_FCT( fd_msg_avp_new ( sip_dict.Digest_Algorithm, 0, &a2 ) );
									val.os.data=(unsigned char *)"MD5";
									val.os.len=strlen((const char *)val.os.data);
									CHECK_FCT( fd_msg_avp_setvalue( a2, &val ) );
									CHECK_FCT( fd_msg_avp_add( sipAuthenticate, MSG_BRW_LAST_CHILD, a2 ) );
							
								}
								/* Add the Digest Realm AVP */
								{
									tempavp=avp;
								
									avpheader_auth=walk_digest(tempavp, 104);
									if(avpheader_auth!=NULL)
									{
										CHECK_FCT( fd_msg_avp_new ( sip_dict.Digest_Realm, 0, &a2 ) );
										CHECK_FCT( fd_msg_avp_setvalue( a2, avpheader_auth->avp_value ) );
										CHECK_FCT( fd_msg_avp_add( sipAuthenticate, MSG_BRW_LAST_CHILD, a2 ) );
								
									}
								}
						
						
								//We add SIP Authenticate to Auth Data Item
								CHECK_FCT( fd_msg_avp_add( authdataitem, MSG_BRW_LAST_CHILD, sipAuthenticate ) );
								//We add Auth Data Item to Answer
								CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, authdataitem ) );
						
								number_of_auth_items++;
								if(sipurinotstored)
									result="DIAMETER_SUCCESS_AUTH_SENT_SERVER_NOT_STORED";
								else
									result="DIAMETER_MULTI_ROUND_AUTH";
								found_cnonce=0;
							}
							else
							{
								/*
								We are in the case of access request after challenge so we need to check credentials.
								*/
								TRACE_DEBUG(FULL,"Authentication after challenge");
								
								/* Search the session, retrieve its data */
								{
									//int new=0;
									
									//TRACE_DEBUG(FULL,"new: *%d*",new);
									//ASSERT( new == 0 );
									CHECK_FCT( fd_sess_state_retrieve( ds_sess_hdl, sess, &storednonce ));
									if(storednonce ==NULL)
									{
										result="DIAMETER_UNABLE_TO_COMPLY";
												
										if(username!=NULL)
											free(username);
										goto out;
									}
								}
								
								/* Create the SIP-Authentication-Info AVP */
								{
									CHECK_FCT( fd_msg_avp_new ( sip_dict.SIP_Authentication_Info, 0, &sipAuthentication ) );
								}
								
								
							
								/* Add the Digest response Auth AVP */
								{
									//uint8_t bufferresp[DIGEST_LEN];
									//char response[DIGEST_LEN*2+1];
									
									
									//We extract all the data we need
									tempavp=avp;
									
									char * digest_username=NULL, *digest_uri=NULL, *digest_response=NULL, *digest_realm=NULL, *digest_nonce=NULL, *digest_method=NULL, *digest_qop=NULL, *digest_algorithm=NULL, *digest_cnonce=NULL, *digest_noncecount=NULL;
									
									
									CHECK_FCT(fd_avp_search_avp (avp, sip_dict.Digest_Nonce, &a2 ));
									if(a2!=NULL)
									{
										CHECK_FCT( fd_msg_avp_hdr( a2, &digestheader )  );
										if(digestheader!=NULL)
										{
											
											digest_nonce = malloc(digestheader->avp_value->os.len + 1);
											memcpy(digest_nonce, digestheader->avp_value->os.data,
											digestheader->avp_value->os.len);
											digest_nonce[digestheader->avp_value->os.len]='\0';
											TRACE_DEBUG(FULL,"Element:*%s*",digest_nonce);	
											TRACE_DEBUG(FULL,"Stored Nonce:*%s*",storednonce->nonce);
											
											if(strcmp(digest_nonce,storednonce->nonce)!=0)
											{
												free(digest_nonce);
												free(storednonce->nonce);
												free(storednonce);
												result="DIAMETER_UNABLE_TO_COMPLY";
												
												if(username!=NULL)
													free(username);
												goto out;
											}
											
										}
											
									}
									CHECK_FCT(fd_avp_search_avp (avp, sip_dict.Digest_Response, &a2 ));
									if(a2!=NULL)
									{
										CHECK_FCT( fd_msg_avp_hdr( a2, &digestheader )  );
										if(digestheader!=NULL)
										{
											digest_response = malloc(digestheader->avp_value->os.len + 1);
											memcpy(digest_response, digestheader->avp_value->os.data,
											digestheader->avp_value->os.len);
											digest_response[digestheader->avp_value->os.len]='\0';
											TRACE_DEBUG(FULL,"Element:*%s*",digest_response);	
										}
										
									}
									CHECK_FCT(fd_avp_search_avp (avp, sip_dict.Digest_Realm, &a2 ));
									if(a2!=NULL)
									{
										CHECK_FCT( fd_msg_avp_hdr( a2, &digestheader )  );
										if(digestheader!=NULL)
										{
											digest_realm = malloc(digestheader->avp_value->os.len + 1);
											memcpy(digest_realm, digestheader->avp_value->os.data,
											digestheader->avp_value->os.len);
											digest_realm[digestheader->avp_value->os.len]='\0';
											TRACE_DEBUG(FULL,"Element:*%s*",digest_realm);	
										}
									}
									
									CHECK_FCT(fd_avp_search_avp (avp, sip_dict.Digest_Method, &a2 ));
									if(a2!=NULL)
									{
										CHECK_FCT( fd_msg_avp_hdr( a2, &digestheader )  );
										if(digestheader!=NULL)
										{
											digest_method = malloc(digestheader->avp_value->os.len + 1);
											memcpy(digest_method, digestheader->avp_value->os.data,
											digestheader->avp_value->os.len);
											digest_method[digestheader->avp_value->os.len]='\0';
											TRACE_DEBUG(FULL,"Element:*%s*",digest_method);	
										}
									}
									else
										digest_method="";
									
									CHECK_FCT(fd_avp_search_avp (avp, sip_dict.Digest_URI, &a2 ));
									if(a2!=NULL)
									{
										CHECK_FCT( fd_msg_avp_hdr( a2, &digestheader )  );
										if(digestheader!=NULL)
										{
											digest_uri = malloc(digestheader->avp_value->os.len + 1);
											memcpy(digest_uri, digestheader->avp_value->os.data,
											digestheader->avp_value->os.len);
											digest_uri[digestheader->avp_value->os.len]='\0';
											TRACE_DEBUG(FULL,"Element:*%s*",digest_uri);	
										}
									}
									
									CHECK_FCT(fd_avp_search_avp (avp, sip_dict.Digest_QOP, &a2 ));
									if(a2!=NULL)
									{
										CHECK_FCT( fd_msg_avp_hdr( a2, &digestheader )  );
										if(digestheader!=NULL)
										{
											digest_qop = malloc(digestheader->avp_value->os.len + 1);
											memcpy(digest_qop, digestheader->avp_value->os.data,
											digestheader->avp_value->os.len);
											digest_qop[digestheader->avp_value->os.len]='\0';
											TRACE_DEBUG(FULL,"Element:*%s*",digest_qop);	
										}
									}
									else
										digest_qop=NULL;
									CHECK_FCT(fd_avp_search_avp (avp, sip_dict.Digest_Algorithm, &a2 ));
									if(a2!=NULL)
									{
										CHECK_FCT( fd_msg_avp_hdr( a2, &digestheader )  );
										if(digestheader!=NULL)
										{
											digest_algorithm = malloc(digestheader->avp_value->os.len + 1);
											memcpy(digest_algorithm, digestheader->avp_value->os.data,
											digestheader->avp_value->os.len);
											digest_algorithm[digestheader->avp_value->os.len]='\0';
											TRACE_DEBUG(FULL,"Element:*%s*",digest_algorithm);	
										}
									}
									else
										digest_algorithm=NULL;
									CHECK_FCT(fd_avp_search_avp (avp, sip_dict.Digest_CNonce, &a2 ));
									if(a2!=NULL)
									{
										CHECK_FCT( fd_msg_avp_hdr( a2, &digestheader )  );
										if(digestheader!=NULL)
										{
											digest_cnonce = malloc(digestheader->avp_value->os.len + 1);
											memcpy(digest_cnonce, digestheader->avp_value->os.data,
											digestheader->avp_value->os.len);
											digest_cnonce[digestheader->avp_value->os.len]='\0';
											TRACE_DEBUG(FULL,"Element:*%s*",digest_cnonce);	
										}
									}
									else
										digest_cnonce="";
									CHECK_FCT(fd_avp_search_avp (avp, sip_dict.Digest_Nonce_Count, &a2 ));
									if(a2!=NULL)
									{
										CHECK_FCT( fd_msg_avp_hdr( a2, &digestheader )  );
										if(digestheader!=NULL)
										{
											digest_noncecount = malloc(digestheader->avp_value->os.len + 1);
											memcpy(digest_noncecount, digestheader->avp_value->os.data,
											digestheader->avp_value->os.len);
											digest_noncecount[digestheader->avp_value->os.len]='\0';
											TRACE_DEBUG(FULL,"Element:*%s*",digest_noncecount);	
										}
									}
									else
										digest_noncecount="";
									CHECK_FCT(fd_avp_search_avp (avp, sip_dict.Digest_Username, &a2 ));
									if(a2!=NULL)
									{
										CHECK_FCT( fd_msg_avp_hdr( a2, &digestheader )  );
										if(digestheader!=NULL)
										{
											digest_username = malloc(digestheader->avp_value->os.len + 1);
											memcpy(digest_username, digestheader->avp_value->os.data,
											digestheader->avp_value->os.len);
											digest_username[digestheader->avp_value->os.len]='\0';
											TRACE_DEBUG(FULL,"Element:*%s*",digest_username);	
										}
									}
									//TODO: replace by authentication function
									
									HASHHEX HA1;
									HASHHEX HA2 = "";
									HASHHEX response, responseauth;

									
									DigestCalcHA1(digest_algorithm, digest_username, digest_realm, password, digest_nonce,digest_cnonce, HA1);
      								DigestCalcResponse(HA1, digest_nonce, digest_noncecount, digest_cnonce, digest_qop,digest_method, digest_uri, HA2, response);
      									
		
									// We check that the Digest-Response is the same (UA, Diameter)
									if(strcmp(response,digest_response)!=0)
									{
										TRACE_DEBUG(FULL,"Response calculated by Diameter server:%s",response);
										TRACE_DEBUG(FULL,"Response calculated by UA:%s",digest_response);
										TRACE_DEBUG(INFO,"Digest-Response does not match!");
										result="DIAMETER_UNABLE_TO_COMPLY";
										free(digest_algorithm);
										free(digest_cnonce);
										free(digest_nonce);
										free(digest_noncecount);
										free(digest_method);
										free(digest_username);
										free(digest_uri);
										free(digest_qop);
										free(digest_response);
										free(digest_realm);
										free(storednonce->nonce);
										free(storednonce);
										if(username!=NULL)
											free(username);
										goto out;
										
									}
									//We calculate Digest_Response_Auth
									DigestCalcResponseAuth(HA1, digest_nonce, digest_noncecount, digest_cnonce, digest_qop,digest_method, digest_uri, HA2, responseauth);
									
									TRACE_DEBUG(FULL,"Response calculated by Diameter server:%s",response);
										TRACE_DEBUG(FULL,"Response calculated by UA:%s",digest_response);
									if(strcmp(digest_qop,"auth-int")==0)
									{
										//Digest-HA1 MUST be used instead of Digest-Response-Auth if Digest-Qop is 'auth-int'.
										CHECK_FCT( fd_msg_avp_new ( sip_dict.Digest_HA1, 0, &a2 ) );
										val.os.data=(unsigned char *)HA1;
										val.os.len=HASHHEXLEN+1;
										CHECK_FCT( fd_msg_avp_setvalue( a2, &val ) );
										CHECK_FCT( fd_msg_avp_add( sipAuthentication, MSG_BRW_LAST_CHILD, a2 ) );
									}
									else
									{
										//Digest-Response-Auth MUST be used instead of Digest-HA1 if Digest-Qop is 'auth'.
										CHECK_FCT( fd_msg_avp_new ( sip_dict.Digest_Response_Auth, 0, &a2 ) );
										val.os.data=(unsigned char *)responseauth;
										val.os.len=DIGEST_LEN*2;
										CHECK_FCT( fd_msg_avp_setvalue( a2, &val ) );
										CHECK_FCT( fd_msg_avp_add( sipAuthentication, MSG_BRW_LAST_CHILD, a2 ) );
									}
									free(digest_algorithm);
									free(digest_cnonce);
									free(digest_nonce);
									free(digest_noncecount);
									free(digest_method);
									free(digest_username);
									free(digest_uri);
									free(digest_qop);
									free(digest_response);
									free(digest_realm);
									free(storednonce->nonce);
									free(storednonce);
								
									number_of_auth_items++;
								}	
								
								
								//We add SIP Authentication-Info to Auth Data Item
								CHECK_FCT( fd_msg_avp_add( authdataitem, MSG_BRW_LAST_CHILD, sipAuthentication ) );
								//We add Auth Data Item to Answer
								CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, authdataitem ) );
								
								
								if(username!=NULL && authenticationpending)
								{
									//We clear the pending flag
									clear_pending_flag(username, usernamelen);
								}
								
								if(sipurinotstored)
									result="DIAMETER_SUCCESS_SERVER_NAME_NOT_STORED";
								else
									result="DIAMETER_SUCCESS";
								found_cnonce=0;
							}
						}
					}
					else
						TRACE_DEBUG(INFO,"No auth data items!");
				}
				/*Add SIP_Number_Auth_Items  AVP */
				{
					CHECK_FCT( fd_msg_avp_new ( sip_dict.SIP_Number_Auth_Items, 0, &avp ) );
					val.i32 = number_of_auth_items;
					CHECK_FCT( fd_msg_avp_setvalue ( avp, &val ) );
					CHECK_FCT( fd_msg_avp_add ( ans, MSG_BRW_LAST_CHILD, avp) );
				}
				
				
			}
			else
			{
				TRACE_DEBUG(INFO,"We only support DIGEST for now, unable to comply");
				result="DIAMETER_ERROR_AUTH_SCHEME_NOT_SUPPORTED";
				if(username!=NULL)
					free(username);
				goto out;
			}
		}
	}
	else
	{
		//TODO: remove this because Number_Auth_Items is not compulsory
		TRACE_DEBUG(FULL,"Number-Auth-Items is not included.");
		result="DIAMETER_UNABLE_TO_COMPLY";
		if(username!=NULL)
			free(username);
		goto out;
	}
	
	if(username!=NULL)
		free(username);
	

out:
	/* Set the Origin-Host, Origin-Realm, Result-Code AVPs */
	CHECK_FCT( fd_msg_rescode_set( ans, result, NULL, NULL, 1 ) );
	
	
	/* Send the answer */
	CHECK_FCT( fd_msg_send( msg, NULL, NULL ) );
	
	
	return 0;
}
