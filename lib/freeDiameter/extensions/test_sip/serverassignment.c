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
#include "test_sip.h"

//Called to send a UAR
int test_sip_SAR_cb()
{
	struct dict_object * sar_model=NULL;
	struct msg * message=NULL;
	struct avp *avp=NULL;
	union avp_value value;
	
	//Fake values START
	char *sip_aor="sip:aw-lappy@tera.ics.keio.ac.jp";
	size_t aor_len=strlen(sip_aor); 
	char *destination_realm="tera.ics.keio.ac.jp";
	size_t destination_realmlen=strlen(destination_realm);
	char *destination_host="suika.tera.ics.keio.ac.jp";
	size_t destination_hostlen=strlen(destination_host);
	char *username="aw-lappy";
	size_t usernamelen=strlen(username);
	char *sipserveruri="sip:ichigo@tera.ics.keio.ac.jp";
	size_t sipserverurilen=strlen(sipserveruri);
	// char *visitednetwork="Pink";
	//size_t visitednetworklen=strlen(visitednetwork);
	//int registrationtype = 2;
	int data_already_available=0;
	int assignment_type=0;
	//Fake values STOP
	
	//Create the base message for an RTR
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME, "Server-Assignment-Request", &sar_model, ENOENT) );
	CHECK_FCT( fd_msg_new (sar_model, 0, &message));
	
	
	
	// Create a new session 
	{
		CHECK_FCT( fd_msg_new_session( message, (os0_t)"appsip", CONSTSTRLEN("appsip") ) );
	}
	
	//Add the Auth-Application-Id 
	{
		CHECK_FCT( fd_msg_avp_new ( sip_dict.Auth_Application_Id, 0, &avp ) );
		value.i32 = 6;
		CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
		CHECK_FCT( fd_msg_avp_add ( message, MSG_BRW_LAST_CHILD, avp) );
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
		value.os.data=(unsigned char *)destination_host;
		value.os.len=destination_hostlen;
		CHECK_FCT( fd_msg_avp_setvalue( avp, &value ) );
		CHECK_FCT( fd_msg_avp_add( message, MSG_BRW_LAST_CHILD, avp ) );
	}
	//Destination_Realm
	{
		CHECK_FCT( fd_msg_avp_new ( sip_dict.Destination_Realm, 0, &avp ) );
		value.os.data=(unsigned char *)destination_realm;
		value.os.len=destination_realmlen;
		CHECK_FCT( fd_msg_avp_setvalue( avp, &value ) );
		CHECK_FCT( fd_msg_avp_add( message, MSG_BRW_LAST_CHILD, avp ) );
	}
	
	//SIP_AOR
	{
		
		CHECK_FCT( fd_msg_avp_new ( sip_dict.SIP_AOR, 0, &avp ) );
		value.os.data=(unsigned char *)sip_aor;
		value.os.len=aor_len;
		CHECK_FCT( fd_msg_avp_setvalue( avp, &value ) );
		CHECK_FCT( fd_msg_avp_add( message, MSG_BRW_LAST_CHILD, avp ) );
		
	}
	//Username
	{
		
		CHECK_FCT( fd_msg_avp_new ( sip_dict.User_Name, 0, &avp ) );
		value.os.data=(unsigned char *)username;
		value.os.len=usernamelen;
		CHECK_FCT( fd_msg_avp_setvalue( avp, &value ) );
		CHECK_FCT( fd_msg_avp_add( message, MSG_BRW_LAST_CHILD, avp ) );
		
	}
	//SIP_User_Data_Already_Available
	{
		CHECK_FCT( fd_msg_avp_new ( sip_dict.SIP_User_Data_Already_Available, 0, &avp ) );
		value.i32=data_already_available;
		CHECK_FCT( fd_msg_avp_setvalue( avp, &value ) );
		CHECK_FCT( fd_msg_avp_add( message, MSG_BRW_LAST_CHILD, avp ) );
	}
	
	//SIP_Server_Assignment_Type;
	{
		CHECK_FCT( fd_msg_avp_new ( sip_dict.SIP_Server_Assignment_Type, 0, &avp ) );
		value.i32=assignment_type;
		CHECK_FCT( fd_msg_avp_setvalue( avp, &value ) );
		CHECK_FCT( fd_msg_avp_add( message, MSG_BRW_LAST_CHILD, avp ) );
	}
	//SIP_server_uri
	{
		
		CHECK_FCT( fd_msg_avp_new ( sip_dict.SIP_Server_URI, 0, &avp ) );
		value.os.data=(unsigned char *)sipserveruri;
		value.os.len=sipserverurilen;
		CHECK_FCT( fd_msg_avp_setvalue( avp, &value ) );
		CHECK_FCT( fd_msg_avp_add( message, MSG_BRW_LAST_CHILD, avp ) );
		
	}
	
	CHECK_FCT( fd_msg_send( &message, NULL, NULL ));
	
	return 0;
}

int test_sip_SAA_cb( struct msg ** msg, struct avp * paramavp, struct session * sess, void * opaque, enum disp_action * act)
{
	
	return 0;
}
