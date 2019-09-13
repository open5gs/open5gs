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
#include <freeDiameter/extension.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
GCC_DIAG_OFF("-Wdeprecated-declarations")
#include <gcrypt.h>
GCC_DIAG_ON("-Wdeprecated-declarations")
#include <string.h>
#include <mysql.h>

/* The module configuration */
struct ts_conf {
	char * destination_sip; 
	char * destination_realm;
	char * username;
	char * password;
	char * sip_aor;
};
extern struct ts_conf * ts_conf;

//Storage for some useful AVPs
struct sip_dict{
	struct dict_object * Auth_Session_State;
	struct dict_object * Auth_Application_Id;
	struct dict_object * Destination_Host;
	struct dict_object * User_Name;
	struct dict_object * Session_Id;
	struct dict_object * Redirect_Host;
	struct dict_object * Redirect_Host_Usage;
	struct dict_object * SIP_Auth_Data_Item;
	struct dict_object * SIP_Accounting_Information;
	struct dict_object * SIP_Accounting_Server_URI;
	struct dict_object * SIP_Credit_Control_Server_URI;
	struct dict_object * SIP_Server_Assignment_Type;
	struct dict_object * SIP_Item_Number;
	struct dict_object * SIP_User_Authorization_Type;
	struct dict_object * SIP_Supported_User_Data_Type;
	struct dict_object * SIP_User_Data;
	struct dict_object * SIP_User_Data_Type;
	struct dict_object * SIP_User_Data_Contents;
	struct dict_object * SIP_User_Data_Already_Available;
	struct dict_object * SIP_Visited_Network_Id;
	struct dict_object * SIP_Authorization;
	struct dict_object * SIP_Authenticate;
	struct dict_object * SIP_Number_Auth_Items;	
	struct dict_object * SIP_Authentication_Scheme;
	struct dict_object * SIP_Authentication_Info;	
	struct dict_object * SIP_Server_URI;
	struct dict_object * SIP_Server_Capabilities;
	struct dict_object * SIP_Mandatory_Capability;
	struct dict_object * SIP_Optional_Capability;
	struct dict_object * SIP_Method;
	struct dict_object * SIP_AOR;
	struct dict_object * SIP_Deregistration_Reason;
	struct dict_object * SIP_Reason_Code;
	struct dict_object * SIP_Reason_Info;
	struct dict_object * Digest_URI;		
	struct dict_object * Digest_Nonce;
	struct dict_object * Digest_Nonce_Count;
	struct dict_object * Digest_CNonce;		
	struct dict_object * Digest_Realm;		
	struct dict_object * Digest_Response;	
	struct dict_object * Digest_Response_Auth;	
	struct dict_object * Digest_Username;	
	struct dict_object * Digest_Method;
	struct dict_object * Digest_QOP;	
	struct dict_object * Digest_Algorithm;
	struct dict_object * Digest_HA1;
	struct dict_object * Destination_Realm;
};

extern  struct sip_dict  sip_dict;
extern struct session_handler * ts_sess_hdl;

int ts_entry();
void fd_ext_fini(void);

int test_sip_LIR_cb();
int test_sip_UAR_cb();
int test_sip_SAR_cb();
int test_sipSL_LIR_cb();

int test_sip_default_cb( struct msg ** msg, struct avp * avp, struct session * sess, void * opaque, enum disp_action * act);
int test_sip_MAA_cb( struct msg ** msg, struct avp * avp, struct session * sess, void * opaque, enum disp_action * act);
int test_sip_RTR_cb( struct msg ** msg, struct avp * avp, struct session * sess, void * opaque, enum disp_action * act);
int test_sip_UAA_cb( struct msg ** msg, struct avp * avp, struct session * sess, void * opaque, enum disp_action * act);
int test_sip_LIA_cb( struct msg ** msg, struct avp * avp, struct session * sess, void * opaque, enum disp_action * act);
int test_sip_SAA_cb( struct msg ** msg, struct avp * avp, struct session * sess, void * opaque, enum disp_action * act);
int test_sipSL_LIA_cb( struct msg ** msg, struct avp * avp, struct session * sess, void * opaque, enum disp_action * act);
