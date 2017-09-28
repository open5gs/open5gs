/*********************************************************************************************************
* Software License Agreement (BSD License)                                                               *
* Author: Alexandre Westfahl <awestfahl@freesipserver.net>						 *
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
#include "md5.h"


#define NONCE_SIZE 16
#define DIGEST_LEN 16


/* Mode for the extension */
#define MODE_DSSERVER	0x1
#define	MODE_SL	0x2

//Redirect_Host_Usage
#define ALL_USER	6

/* The module configuration */
struct as_conf {
	int		mode;		/* default MODE_DSSERVER | MODE_SL */
	enum {ASMYSQL} datasource; 
	char * mysql_login;
	char * mysql_password;
	char * mysql_database;
	char * mysql_server;
	char mysql_prefix[10]; //default: as_
	uint16_t  mysql_port; //if 0, default port will be used
	uint16_t rtr_port;
	uint16_t ppr_port;
};
extern struct as_conf * as_conf;

/* Parse the configuration file */
int as_conf_handle(char * conffile);


extern MYSQL *conn;



void calc_md5(char *buffer, char * data);
void clear_digest(uint8_t * digest, char * readable_digest, int digestlength);
struct avp_hdr * walk_digest(struct avp *avp, int avp_code);


//MySQL part
int start_mysql_connection();
void request_mysql(char *query);
void close_mysql_connection();
int get_sipserver_uri(const unsigned char *sip_aor, const size_t sipaorlen, unsigned char ** sipserver_uri, size_t *sipserverurilen);
int exist_username(const unsigned char *sip_aor, const size_t sipaorlen);
int get_sipserver_cap(const unsigned char *sip_aor, const size_t sipaorlen, struct avp **capabilities);
int get_password(const unsigned char *username, const size_t usernamelen, char *password);
int check_sipaor(const unsigned char  *username, const size_t usernamelen, const char * sip_aor,const size_t sipaorlen);
int add_user_datatype(const unsigned char  *sip_aor, const size_t sipaorlen,struct msg *message);
int set_pending_flag(const unsigned char  *username, const size_t usernamelen);
int clear_pending_flag(const unsigned char  *username, const size_t usernamelen);
int set_real_sipserver_uri(const unsigned char  *username, const size_t usernamelen, const unsigned char *sipserver_uri,const size_t sipserverurilen);
int set_sipserver_uri(const unsigned char  *username, const size_t usernamelen, const unsigned char *sipserver_uri,const size_t sipserverurilen);
//int get_sipserver_uri(const unsigned char *sip_aor, const size_t sipaorlen, unsigned char ** sipserver_uri, size_t *sipserverurilen);
int allow_roaming(const unsigned char  *username, const size_t usernamelen, const char * network,const size_t networklen);
int get_diameter_uri(const unsigned char *sip_aor, const size_t sipaorlen, char ** diameter_uri, size_t *diameterurilen);
//count functions
int count_avp(struct msg * message, int code, int vendor);


void DigestCalcHA1(char * pszAlg,char * pszUserName,char * pszRealm,char * pszPassword,char * pszNonce,char * pszCNonce,HASHHEX SessionKey);
void DigestCalcResponse(HASHHEX HA1,char * pszNonce,char * pszNonceCount,char * pszCNonce,char * pszQop,char * pszMethod,char * pszDigestUri,HASHHEX HEntity,HASHHEX Response);
void DigestCalcResponseAuth(HASHHEX HA1,char * pszNonce,char * pszNonceCount,char * pszCNonce,char * pszQop,char * pszMethod,char * pszDigestUri,HASHHEX HEntity,HASHHEX Response);

int fd_avp_search_avp ( struct avp * groupedavp, struct dict_object * what, struct avp ** avp );

//thread procedure
void *rtr_socket(void *);
void *ppr_socket(void *);

struct rtrsipaor
{
	char username[200];
	char sip_aor1[200];
	char sip_aor2[200];
	char sip_aor3[200];
	char strreason[200];
	char desthost[200];
	int reason;
};
struct pprsipaor
{
	char username[200];
	char datatype1[255];
	char datatype2[255];
	char datatype3[255];
	int accounting;
};

int app_sip_RTR_cb(struct rtrsipaor *structure);
int app_sip_PPR_cb(struct pprsipaor *structure);


int ds_entry();
void fd_ext_fini(void);
int app_sip_default_cb( struct msg ** msg, struct avp * avp, struct session * sess, void * opaque, enum disp_action * act);
int app_sip_MAR_cb( struct msg ** msg, struct avp * avp, struct session * sess, void * opaque, enum disp_action * act);
int app_sip_RTA_cb( struct msg ** msg, struct avp * avp, struct session * sess, void * opaque, enum disp_action * act);
int app_sip_PPA_cb( struct msg ** msg, struct avp * avp, struct session * sess, void * opaque, enum disp_action * act);
int app_sip_LIR_cb( struct msg ** msg, struct avp * avp, struct session * sess, void * opaque, enum disp_action * act);
int app_sip_UAR_cb( struct msg ** msg, struct avp * avp, struct session * sess, void * opaque, enum disp_action * act);
int app_sip_SAR_cb( struct msg ** msg, struct avp * avp, struct session * sess, void * opaque, enum disp_action * act);

//Suscriber Locator
int app_sip_SL_LIR_cb( struct msg ** msg, struct avp * paramavp, struct session * sess, void * opaque, enum disp_action * act);
//int app_sip_SL_SAR_cb( struct msg ** msg, struct avp * paramavp, struct session * sess, void * opaque, enum disp_action * act);

#define SQL_GETPASSWORD "SELECT `password` FROM ds_users WHERE `username` ='%s'"
#define SQL_GETPASSWORD_LEN sizeof(SQL_GETPASSWORD)-2

//username by SIP-AOR
#define SQL_GETUSERNAME  "SELECT `username` FROM ds_users, ds_sip_aor WHERE `sip_aor` ='%s' AND `ds_sip_aor`.`id_user` = `ds_users`.`id_user`"
#define SQL_GETUSERNAME_LEN sizeof(SQL_GETUSERNAME)-2

//sip server uri by username
#define SQL_GETSIPURI  "SELECT `sip_server_uri` FROM ds_users WHERE `username` ='%s'"
#define SQL_GETSIPURI_LEN sizeof(SQL_GETSIPURI)-2

//sip server uri by SIP-AOR
#define SQL_GETSIPSERURI  "SELECT `ds_sip_aor`.`sip_server_uri` FROM ds_users, ds_sip_aor WHERE `sip_aor` ='%s' AND `ds_sip_aor`.`id_user` = `ds_users`.`id_user`"
#define SQL_GETSIPSERURI_LEN sizeof(SQL_GETSIPSERURI)-2

//sip capabilities for a SIP-AOR
#define SQL_GETSIPSERCAP  "SELECT `compulsory`,`id_service` FROM ds_user_services, ds_sip_aor WHERE `sip_aor` ='%s' AND `ds_sip_aor`.`id_user` = `ds_user_services`.`id_user`"
#define SQL_GETSIPSERCAP_LEN sizeof(SQL_GETSIPSERCAP)-2

//user data for a user data supported
#define SQL_GETSIPDATA  "SELECT `label_type`,`data` FROM ds_sip_aor, ds_user_data, ds_data_types WHERE `sip_aor` ='%s' AND `ds_sip_aor`.`id_sip_aor` = `ds_user_data`.`id_sip_aor` AND `ds_data_types`.`id_data_type`=`ds_user_data`.`id_data_type`"
#define SQL_GETSIPDATA_LEN sizeof(SQL_GETSIPDATA)-2

#define SQL_GETDIAMURI "SELECT `sipserver_uri` FROM ds_sip_aor_map WHERE `sip_aor` ='%s'"
#define SQL_GETDIAMURI_LEN sizeof(SQL_GETDIAMURI)-2

//networks for this user
#define SQL_GETUSERNET "SELECT `label_network` FROM ds_users, ds_user_networks, ds_networks WHERE `ds_users`.`username` ='%s' AND `ds_user_networks`.`id_user` = `ds_users`.`id_user` AND `ds_user_networks`.`id_network` = `ds_networks`.`id_network`"
#define SQL_GETUSERNET_LEN sizeof(SQL_GETUSERNET)-2

#define SQL_SETSIPURI "UPDATE ds_users SET `temp_sip_server_uri`='%s' WHERE `username` ='%s'"
#define SQL_SETSIPURI_LEN sizeof(SQL_SETSIPURI)-4

//TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO 
#define SQL_RMSIPURI "UPDATE ds_users SET `temp_sip_server_uri`='', `sip_server_uri`='' WHERE `id_user` ='%s'"
#define SQL_RMSIPURI_LEN sizeof(SQL_RMSIPURI)-2
//TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO 

#define SQL_SETREALSIPURI "UPDATE ds_users SET `sip_server_uri`='%s' WHERE `username` ='%s'"
#define SQL_SETREALSIPURI_LEN sizeof(SQL_SETREALSIPURI)-4

#define SQL_SETFLAG "UPDATE ds_users SET `authentication_pending`=1 WHERE `username` ='%s'"
#define SQL_SETFLAG_LEN sizeof(SQL_SETFLAG)-2

#define SQL_CLEARFLAG "UPDATE ds_users SET `authentication_pending`=0, `registrated`=1 WHERE `username` ='%s'"
#define SQL_CLEARFLAG_LEN sizeof(SQL_CLEARFLAG)-2

#define SQL_GETSIPAOR "SELECT `sip_aor` FROM `ds_sip_aor`, `ds_users` WHERE `ds_sip_aor`.`id_user` = `ds_users`.`id_user` AND `ds_users`.`username` = '%s'"
#define SQL_GETSIPAOR_LEN sizeof(SQL_GETSIPAOR)-2

//#define SQL_CLEARFLAG "UPDATE ds_users SET `authentication_pending`=0 WHERE `username` ='%s'"
//#define SQL_CLEARFLAG_LEN 67

extern struct session_handler * ds_sess_hdl;

//AVP code
#define CODE_SIP_USER_DATA_TYPE	388
#define CODE_SIP_AOR	122

//Storage for some useful AVPs
struct app_sip_dict{
	struct dict_object * Auth_Session_State;
	struct dict_object * Auth_Application_Id;
	struct dict_object * Destination_Host;
	struct dict_object * Destination_Realm;
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
};

extern  struct app_sip_dict  sip_dict;
