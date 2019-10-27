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
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

//Procedure which always wait for data on socket 
void *ppr_socket(void *arg)
{
	
	SOCKET sock;
    SOCKADDR_IN sin, csin;
    struct pprsipaor pprsip;
    int rcvbytes=0;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	sin.sin_addr.s_addr = inet_addr("127.0.0.1");
    sin.sin_family = AF_INET;
	sin.sin_port = htons(as_conf->ppr_port);
    socklen_t sinsize = sizeof(csin);
	int accepted=0;

    
	if(!bind(sock, (SOCKADDR*)&sin, sizeof(sin)))
	{
		if(listen(sock,1))
		{
			TRACE_DEBUG(INFO,"ERROR on listen!");
		}
		
		while(1)
		{
			accepted=accept(sock, (struct sockaddr *)&csin,&sinsize);
			if(accepted>-1)
			{
				rcvbytes=recv(accepted, &pprsip, sizeof(struct pprsipaor),0);
				
				if(rcvbytes>-1)
				{
					//We received something, we can send an PPR
					app_sip_PPR_cb(&pprsip);
				}
			}
		}
		
		
	}
	else
		TRACE_DEBUG(INFO,"Can't create socket!");

	
	pthread_exit(NULL);
	
}
//Called to send a PPR
int app_sip_PPR_cb(struct pprsipaor *structure)
{
	/*
	TRACE_ENTRY("%p", structure);
	
	int got_username=0;
	int got_streason=0;
	int num_aor=0;//How many SIP-AOR?
	struct dict_object * rtr_model=NULL;
	struct msg * message=NULL;
	struct avp *groupedavp=NULL, *avp=NULL;
	struct session *sess=NULL;
	union avp_value value;
	
	//We must check that we have all needed value in structure
	if(structure.username[0]!='\0')
		got_username=1;
	
	if(structure.sip_aor1[0]!='\0')
	{	
		num_aor++;
		if(structure.sip_aor2[0]!='\0')
		{
			num_aor++;
			if(structure.sip_aor3[0]!='\0')
				num_aor++;
		}
	}
	
	if(structure.strreason!='\0')
		got_streason=1;
	
	
	TRACE_DEBUG(INFO,"We have %d SIP_AOR",num_aor);
	
	if((got_username + num_aor)==0)
	{
		//We must have a least a SIP_AOR or a Username
		TRACE_DEBUG(INFO,"Can not proceed because there is no SIP_AOR or Username");
		return EINVAL;
	}
	if(structure.reason<0)
	{
		//We must have a least a SIP_AOR or a Username
		TRACE_DEBUG(INFO,"Incorrect Reason-Code");
		return EINVAL;
	}
	
	if(structure.desthost[0]=='\0')
	{
		//We must have a least a SIP_AOR or a Username
		TRACE_DEBUG(INFO,"No Destination_Host was provided!");
		return EINVAL;
	}
	//Create the base message for an RTR
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME, "Registration-Termination-Request", &rtr_model, ENOENT) );
	CHECK_FCT( fd_msg_new (rtr_model, 0, &message));
	
	// Create a new session 
	{
		CHECK_FCT( fd_sess_new( &sess, fd_g_config->cnf_diamid, "app_sip", 7 ));
		char * sid;
		CHECK_FCT( fd_sess_getsid ( sess, &sid ));
		CHECK_FCT( fd_msg_avp_new ( sip_dict.Session_Id, 0, &avp ));
		value.os.data = (uint8_t *)sid;
		value.os.len  = strlen(sid);
		CHECK_FCT( fd_msg_avp_setvalue( avp, &value ));
		CHECK_FCT( fd_msg_avp_add( message, MSG_BRW_FIRST_CHILD, avp ));
	}
	
	
	//Auth_Session_State
	{
		CHECK_FCT( fd_msg_avp_new ( sip_dict.Auth_Session_State, 0, &avp ) );
		value.i32=1;
		CHECK_FCT( fd_msg_avp_setvalue( avp, &value ) );
		CHECK_FCT( fd_msg_avp_add( message, MSG_BRW_LAST_CHILD, avp ) );
	}
	
	//Origin_Host & Origin_Realm
	CHECK_FCT( fd_msg_add_origin ( message, 0 ));
	
	//Destination_Host
	{
		CHECK_FCT( fd_msg_avp_new ( sip_dict.Destination_Host, 0, &avp ) );
		value.os.data=(unsigned char *)structure.desthost;
		value.os.len=(size_t)strlen(structure.desthost);
		CHECK_FCT( fd_msg_avp_setvalue( avp, &value ) );
		CHECK_FCT( fd_msg_avp_add( message, MSG_BRW_LAST_CHILD, avp ) );
	}
	
	
	//SIP Deregistration Reason (Grouped AVP)
	{
		CHECK_FCT( fd_msg_avp_new ( sip_dict.SIP_Deregistration_Reason, 0, &groupedavp ) );
		
		//Reason Code
		CHECK_FCT( fd_msg_avp_new ( sip_dict.SIP_Reason_Code, 0, &avp ) );
		value.i32=structure.reason;
		CHECK_FCT( fd_msg_avp_setvalue( avp, &value ) );
		CHECK_FCT( fd_msg_avp_add( groupedavp, MSG_BRW_LAST_CHILD, avp ) );
		
		if(got_streason)
		{
			//Reason Info
			CHECK_FCT( fd_msg_avp_new ( sip_dict.SIP_Reason_Info, 0, &avp ) );
			value.os.data=(unsigned char *)structure.strreason;
			value.os.len=(size_t)strlen(structure.strreason);
			CHECK_FCT( fd_msg_avp_setvalue( avp, &value ) );
			CHECK_FCT( fd_msg_avp_add( groupedavp, MSG_BRW_LAST_CHILD, avp ) );
		}
		
		//We add the grouped AVP to the message
		CHECK_FCT( fd_msg_avp_add( message, MSG_BRW_LAST_CHILD, groupedavp ) );
	}
	
	//Username
	{
		if(got_username)
		{
			CHECK_FCT( fd_msg_avp_new ( sip_dict.User_Name, 0, &avp ) );
			value.os.data=(unsigned char *)structure.username;
			value.os.len=(size_t)strlen(structure.username);
			CHECK_FCT( fd_msg_avp_setvalue( avp, &value ) );
			CHECK_FCT( fd_msg_avp_add( message, MSG_BRW_LAST_CHILD, avp ) );
		}
	}
	
	//SIP_AOR
	{
		if(num_aor>0)
		{
			CHECK_FCT( fd_msg_avp_new ( sip_dict.SIP_AOR, 0, &avp ) );
			value.os.data=(unsigned char *)structure.sip_aor1;
			value.os.len=(size_t)strlen(structure.sip_aor1);
			CHECK_FCT( fd_msg_avp_setvalue( avp, &value ) );
			CHECK_FCT( fd_msg_avp_add( message, MSG_BRW_LAST_CHILD, avp ) );
			if(num_aor>1)
			{
				CHECK_FCT( fd_msg_avp_new ( sip_dict.SIP_AOR, 0, &avp ) );
				value.os.data=(unsigned char *)structure.sip_aor2;
				value.os.len=(size_t)strlen(structure.sip_aor2);
				CHECK_FCT( fd_msg_avp_setvalue( avp, &value ) );
				CHECK_FCT( fd_msg_avp_add( message, MSG_BRW_LAST_CHILD, avp ) );
				if(num_aor>2)
				{
					CHECK_FCT( fd_msg_avp_new ( sip_dict.SIP_AOR, 0, &avp ) );
					value.os.data=(unsigned char *)structure.sip_aor3;
					value.os.len=(size_t)strlen(structure.sip_aor3);
					CHECK_FCT( fd_msg_avp_setvalue( avp, &value ) );
					CHECK_FCT( fd_msg_avp_add( message, MSG_BRW_LAST_CHILD, avp ) );
				}
			}
		}
	}
	
	fd_msg_dump_walk(INFO,message);
	CHECK_FCT( fd_msg_send( &message, NULL, NULL ));
	*/
	return 0;
}

//Called when an PPA arrive
int app_sip_PPA_cb( struct msg ** msg, struct avp * paramavp, struct session * sess, void * opaque, enum disp_action * act)
{
	//TODO: PPA reception
/*
	//TODO:remove unused variables
	struct msg *ans, *qry;
	struct avp *avp, *a2, *authdataitem;
	struct msg_hdr * header = NULL;
	struct avp_hdr * avphdr=NULL, *avpheader=NULL, *avpheader_auth=NULL,*digestheader=NULL;
	union avp_value val;
	int found_cnonce=0;
	struct avp * tempavp=NULL,*sipAuthentication=NULL,*sipAuthenticate=NULL;
	char * result;
	int idx=0, idx2=0, number_of_auth_items=0,i=0;
	//Flags and variables for Database
	int sipurinotstored=0, authenticationpending=0, querylen=0, usernamelen=0;
	char *query=NULL,*username=NULL;
	
	
	
	TRACE_ENTRY("%p %p %p %p", msg, avp, sess, act);
	
	if (msg == NULL)
		return EINVAL;
	
	
	// Create answer header 
	qry = *msg;
	CHECK_FCT( fd_msg_new_answer_from_req ( fd_g_config->cnf_dict, msg, 0 ) );
	ans = *msg;	
	
	
	// Add the Auth-Session-State AVP 
	{
		
		CHECK_FCT( fd_msg_search_avp ( qry, sip_dict.Auth_Session_State, &avp) );
		CHECK_FCT( fd_msg_avp_hdr( avp, &avphdr )  );
		
		CHECK_FCT( fd_msg_avp_new ( sip_dict.Auth_Session_State, 0, &avp ) );
		CHECK_FCT( fd_msg_avp_setvalue( avp, avphdr->avp_value ) );
		CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
	}
	
	CHECK_FCT( fd_msg_search_avp ( qry, sip_dict.SIP_Deregistration_Reason, &avp) );
	CHECK_FCT( fd_msg_avp_hdr( avp, &avphdr )  );
	
	
	
	
	
	
	
	
	
	
	
	
	
	*/
	return 0;
	
}

