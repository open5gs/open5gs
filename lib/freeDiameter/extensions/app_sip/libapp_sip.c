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
//#include <mysql.h>
#include "app_sip.h"

MYSQL *conn;

void clear_digest(uint8_t * digest, char * readable_digest, int digestlength)
{
	int i=0;
	for(i=0;i<digestlength * 2;i++)
		sprintf(&readable_digest[2 * i], "%2.2hhx", digest[i]);
	readable_digest[2 * digestlength]='\0';
	
	return;
}

// You must create a table like this "char  clearDigest[DIGEST_LEN*2+1];"
void calc_md5(char *clearDigest, char * data)
{
	gcry_md_hd_t md5;
	uint8_t * binDigest=NULL;
	
	CHECK_MALLOC_DO(binDigest=malloc(DIGEST_LEN),return);
	
	gcry_md_open(&md5,GCRY_MD_MD5, 0); 
	gcry_md_write(md5, (char *)data, sizeof(data));
	memcpy(binDigest, gcry_md_read(md5,  GCRY_MD_MD5),gcry_md_get_algo_dlen(GCRY_MD_MD5));
	gcry_md_close(md5);
	
	clear_digest(binDigest, clearDigest, DIGEST_LEN);
	free(binDigest);
	return;
}


/* Search a given AVP model in an AVP (extracted from libfreediameter/message.c ) */
int fd_avp_search_avp ( struct avp * groupedavp, struct dict_object * what, struct avp ** avp )
{
	struct avp * nextavp;
	struct avp_hdr * nextavphdr;
	struct dict_avp_data 	dictdata;
	
	
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
struct avp_hdr *walk_digest(struct avp *avp, int avp_code)
{
	struct avp_hdr *temphdr=NULL;
	CHECK_FCT_DO(fd_msg_browse ( avp, MSG_BRW_WALK, &avp, NULL),return NULL);
	
	while(avp!=NULL)
	{
		
		CHECK_FCT_DO( fd_msg_avp_hdr( avp,&temphdr ),return NULL);

		if(temphdr->avp_code==avp_code)
		{
			//We found the AVP so we set avp to NULL to exit the loop
			avp=NULL;
			return temphdr;
			
		}
		else if(temphdr->avp_code==380)//SIP-Authorization AVP
		{
			//We didn't found the AVP but we finished browsing the Authentication AVP
			avp=NULL;
			temphdr=NULL;
			
			return temphdr;
		}
		else
		{
			CHECK_FCT_DO(fd_msg_browse ( avp, MSG_BRW_WALK, &avp, NULL),return NULL);
			temphdr=NULL;
			
		}
	}
	
	return temphdr;
}

int start_mysql_connection()
{
	conn = mysql_init(NULL);
	
	mysql_options(conn, MYSQL_OPT_RECONNECT, "true");
	
	if (!mysql_real_connect(conn, as_conf->mysql_server,as_conf->mysql_login, as_conf->mysql_password, as_conf->mysql_database, as_conf->mysql_port, NULL, 0)) 
	{//TODO: display error from mysql
		TRACE_DEBUG(INFO,"Unable to connect to database (%s) with login:%s",as_conf->mysql_database,as_conf->mysql_login);
		return 1;
	}
	return 0;
	
}


void request_mysql(char *query)
{
	//We check if the connection is still up
	mysql_ping(conn);
	
	if (mysql_query(conn, query)) 
	{
		TRACE_DEBUG(INFO,"Query %s failed", query);
		
	}
	
}

void close_mysql_connection()
{
	mysql_close(conn);
	
}

//If password is null, we just verify this user exist
//We don't need the password length because it is a table'
int get_password(const unsigned char *username, const size_t usernamelen, char *password)
{
	CHECK_PARAMS(username && usernamelen);
	
	int not_found=2;
	size_t querylen, usernamepurelen;
	char *query, *username_pure;
	
	switch(as_conf->datasource)
	{
		//MySQL
		case ASMYSQL:
			//We allocate the double size of username because at worst it can be all quotes
			username_pure=malloc(usernamelen*2+1);
			//We purify username not to have forbidden characters
			usernamepurelen=mysql_real_escape_string(conn, username_pure, (const char *)username, usernamelen);
			
			//We copy username in query
			querylen=SQL_GETPASSWORD_LEN + usernamepurelen;
			query = malloc(querylen+2);
			snprintf(query, querylen+1, SQL_GETPASSWORD, username_pure);
			
			MYSQL_RES *res;
			MYSQL_ROW row;
			
			//We make the query	
			request_mysql(query);
			res=mysql_use_result(conn);
			if(res==NULL)
			{
				if(password!=NULL)
					password[0]='\0';
				free(query);
				return 2;
			}
			
			
			
			while ((row = mysql_fetch_row(res)) != NULL)
			{
				if(strlen(row[0])>0)
				{
					if(password!=NULL)
						strcpy(password,row[0]);
					
					not_found=0;
					break;
				}
			}
			mysql_free_result(res);
			free(query);
		break;
	}
	return not_found;
}

int check_sipaor(const unsigned char  *username, const size_t usernamelen, const char * sip_aor,const size_t sipaorlen)
{
	CHECK_PARAMS(username && usernamelen && sip_aor && sipaorlen);
	
	int not_found=2;
	size_t querylen, usernamepurelen;
	char *query, *username_pure;
	
	switch(as_conf->datasource)
	{
		//MySQL
		case ASMYSQL:
			//We allocate the double size of username because at worst it can be all quotes
			username_pure=malloc(usernamelen*2+1);
			//We purify username not to have forbidden characters
			usernamepurelen=mysql_real_escape_string(conn, username_pure, (const char *)username, usernamelen);
	
			//We get the list of AOR owned by this user
			querylen=SQL_GETSIPAOR_LEN + usernamepurelen;
			query = malloc(querylen+2);
			snprintf(query, querylen+1, SQL_GETSIPAOR, username_pure);
			
			MYSQL_RES *res;
			MYSQL_ROW row;
			
			//We make the query
			request_mysql(query);
			res=mysql_use_result(conn);
			if(res==NULL)
			{
				free(query);
				return 2;
			}
			
			
			not_found=1;
			while ((row = mysql_fetch_row(res)) != NULL)
			{
				if(strncmp((const char *)sip_aor,row[0],sipaorlen)==0)
				{
					not_found=0;
					break;
				}
			}
			mysql_free_result(res);
			free(query);
		break;
	}
	return not_found;
}

int get_diameter_uri(const unsigned char *sip_aor, const size_t sipaorlen, char ** diameter_uri, size_t *diameterurilen)
{
	CHECK_PARAMS(sip_aor && sipaorlen);

	size_t querylen, sipaorpurelen;
	char *query, *sipaor_pure;
	int not_found=2;




	//a sip aor must begin by "sip:" or "sips:" so it must at least be longer than 4 chars
	if(sipaorlen<5)
		return 2;

	//NOTE: each method has to purify sip_aor itself. You must remove quotes or special chars for security
	switch(as_conf->datasource)
	{
		//MySQL
		case ASMYSQL:

			querylen=SQL_GETDIAMURI_LEN + sipaorlen;

			
			//We allocate the double size of SIP-URI because at worst it can be all quotes
			CHECK_MALLOC(sipaor_pure=malloc(sipaorlen*2+1));
			//We purify SIP-URI not to have forbidden characters
			sipaorpurelen=mysql_real_escape_string(conn, sipaor_pure, (const char *)sip_aor, sipaorlen);
			
			
			query = malloc(querylen+sipaorpurelen+ 2);
			snprintf(query, querylen+1, SQL_GETDIAMURI, sipaor_pure);
			
			MYSQL_RES *res;
			MYSQL_ROW row;
			

			//We make the query	
			request_mysql(query);
			res=mysql_use_result(conn);
			if(res==NULL)
			{
				//We couldn't make the request
				diameter_uri=NULL;
				return 2;
			}
			
			while ((row = mysql_fetch_row(res)) != NULL)
			{
				*diameterurilen=strlen(row[0]);
				if(*diameterurilen>0)
				{
					CHECK_MALLOC(*diameter_uri=malloc(*diameterurilen+1));
					strcpy(*diameter_uri,row[0]);
					not_found=0;
					break;
				}
			}
			mysql_free_result(res);
			free(query);
			free(sipaor_pure);
		break;
      
    default:
      
      //We must never go here, if so, we must stop diameter_sip
      diameter_uri=NULL;
      return 2;
      
      break;
  }
  
  //0 if it was found
  return not_found;
  
}


int exist_username(const unsigned char *sip_aor, const size_t sipaorlen)
{
	CHECK_PARAMS(sip_aor && sipaorlen);
	
	size_t querylen, sipaorpurelen;
	char *query, *sipaor_pure;
	int not_found=1;
	
	//a sip aor must begin by "sip:" or "sips:" so it must at least be longer than 4 chars
	if(sipaorlen<5)
		return 2;
	
	//NOTE: each method has to purify sip_aor itself. You must remove quotes or special chars for security
	
	switch(as_conf->datasource)
	{
		//MySQL
		case ASMYSQL:
			
			querylen=SQL_GETUSERNAME_LEN + sipaorlen;
			
			
			//We allocate the double size of SIP-URI because at worst it can be all quotes
			CHECK_MALLOC(sipaor_pure=malloc(sipaorlen*2+1));
			//We purify SIP-URI not to have forbidden characters
			sipaorpurelen=mysql_real_escape_string(conn, sipaor_pure, (const char *)sip_aor, sipaorlen);
			
			
			query = malloc(querylen+sipaorpurelen+ 2);
			snprintf(query, querylen+1, SQL_GETUSERNAME, sipaor_pure);
			
			MYSQL_RES *res;
			MYSQL_ROW row;
			
			
			//We make the query	
			request_mysql(query);
			res=mysql_use_result(conn);
			if(res==NULL)
			{
				//We couldn't make the request
				return 2;
			}
			
			while ((row = mysql_fetch_row(res)) != NULL)
			{
				if(strlen(row[0])>0)
				{
					not_found=0;
					break;
				}
			}
			mysql_free_result(res);
			free(query);
			free(sipaor_pure);
			break;
			
		default:
			
			//We must never go here, if so, we must stop diameter_sip
			TRACE_DEBUG(INFO,"FATAL ERROR: the datasource is unknown, please check your config file!");
			return 2;
			
			break;
	}
	
	//0 if it was found
	return not_found;
	
}

//We check if this user can go in the given network
int allow_roaming(const unsigned char  *username, const size_t usernamelen, const char * network,const size_t networklen)
{
	CHECK_PARAMS(username && usernamelen && network && networklen);
	
	int not_found=2;
	size_t querylen, usernamepurelen;
	char *query, *username_pure;
	
	switch(as_conf->datasource)
	{
		//MySQL
		case ASMYSQL:
			//We allocate the double size of username because at worst it can be all quotes
			username_pure=malloc(usernamelen*2+1);
			//We purify username not to have forbidden characters
			usernamepurelen=mysql_real_escape_string(conn, username_pure, (const char *)username, usernamelen);
			
			//We get the list of AOR owned by this user
			querylen=SQL_GETUSERNET_LEN + usernamepurelen;
			query = malloc(querylen+2);
			snprintf(query, querylen+1, SQL_GETUSERNET, username_pure);
			
			
			MYSQL_RES *res;
			MYSQL_ROW row;
			
			
			//We make the query
			request_mysql(query);
			res=mysql_use_result(conn);
			if(res==NULL)
			{
				free(query);
				return 2;
			}
			
			
			not_found=1;
			while ((row = mysql_fetch_row(res)) != NULL)
			{
				if(strncmp((const char *)network,row[0],networklen)==0)
				{
					not_found=0;
					break;
				}
			}
			mysql_free_result(res);
			free(query);
			break;
	}
	return not_found;
}

//SIP-Server-Capabilities for the SIP-AOR
int get_sipserver_cap(const unsigned char *sip_aor, const size_t sipaorlen, struct avp **capabilities)
{
	CHECK_PARAMS(sip_aor && sipaorlen && capabilities);
	
	size_t querylen, sipaorpurelen;
	char *query, *sipaor_pure;
	int not_found=2;
	union avp_value value;
	struct avp *avp;
	
	//a sip aor must begin by "sip:" or "sips:" so it must at least be longer than 4 chars
	if(sipaorlen<5)
		return 2;
	
	//NOTE: each method has to purify sip_aor itself. You must remove quotes or special chars for security
	switch(as_conf->datasource)
	{
		//MySQL
		case ASMYSQL:
			
			querylen=SQL_GETSIPSERCAP_LEN + sipaorlen;
			
			
			//We allocate the double size of SIP-URI because at worst it can be all quotes
			CHECK_MALLOC(sipaor_pure=malloc(sipaorlen*2+1));
			//We purify SIP-URI not to have forbidden characters
			sipaorpurelen=mysql_real_escape_string(conn, sipaor_pure, (const char *)sip_aor, sipaorlen);
			
			
			query = malloc(querylen+sipaorpurelen+ 2);
			snprintf(query, querylen+1, SQL_GETSIPSERCAP, sipaor_pure);
			
			MYSQL_RES *res;
			MYSQL_ROW row;
			
			//We make the query	
			request_mysql(query);
			res=mysql_use_result(conn);
			if(res==NULL)
			{
				//We couldn't make the request
				return 2;
			}
			not_found=1;
			while ((row = mysql_fetch_row(res)) != NULL)
			{
				if(atoi(row[0])==1)
				{//mandatory
					CHECK_FCT( fd_msg_avp_new ( sip_dict.SIP_Mandatory_Capability, 0, &avp ) );
					value.i32=(uint32_t)atoi(row[1]);
					CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
					CHECK_FCT( fd_msg_avp_add ( *capabilities, MSG_BRW_LAST_CHILD, avp) );
					
				}
				else
				{//optional
					CHECK_FCT( fd_msg_avp_new ( sip_dict.SIP_Optional_Capability, 0, &avp ) );
					value.i32=(uint32_t)atoi(row[1]);
					CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
					CHECK_FCT( fd_msg_avp_add ( *capabilities, MSG_BRW_LAST_CHILD, avp) );
				}
				not_found=0;
			}
			
			mysql_free_result(res);
			free(query);
			free(sipaor_pure);
			break;
			
		default:
			
			//We must never go here, if so, we must stop diameter_sip
			TRACE_DEBUG(INFO,"FATAL ERROR: the datasource is unknown, please check your config file!");
			return 2;
			
			break;
	}
	
	//0 if it was found
	return not_found;
	
}


//We retrieve datatype
int add_user_datatype(const unsigned char  *sip_aor, const size_t sipaorlen,struct msg *message)
{
	CHECK_PARAMS(sip_aor && sipaorlen && message );
	
	size_t querylen, sipaorpurelen;
	char *query, *sipaor_pure;
	int not_found=2;
	union avp_value value;
	struct avp *avp, *rootavp;
	unsigned long *length;
	
	//a sip aor must begin by "sip:" or "sips:" so it must at least be longer than 4 chars
	if(sipaorlen<5)
		return 2;
	
	//NOTE: each method has to purify sip_aor itself. You must remove quotes or special chars for security
	switch(as_conf->datasource)
	{
		//MySQL
		case ASMYSQL:
			
			querylen=SQL_GETSIPDATA_LEN + sipaorlen;
			
			
			//We allocate the double size of SIP-URI because at worst it can be all quotes
			CHECK_MALLOC(sipaor_pure=malloc(sipaorlen*2+1));
			//We purify SIP-URI not to have forbidden characters
			sipaorpurelen=mysql_real_escape_string(conn, sipaor_pure, (const char *)sip_aor, sipaorlen);
			
			
			query = malloc(querylen+sipaorpurelen+ 2);
			snprintf(query, querylen+1, SQL_GETSIPDATA, sipaor_pure);
			
			MYSQL_RES *res;
			MYSQL_ROW row;
			
			//We make the query	
			request_mysql(query);
			res=mysql_use_result(conn);
			if(res==NULL)
			{
				//We couldn't make the request
				return 2;
			}
			not_found=1;
			while ((row = mysql_fetch_row(res)) != NULL)
			{
				length=mysql_fetch_lengths(res);
				
				if(strlen(row[0])>1)
				{
					CHECK_FCT( fd_msg_avp_new ( sip_dict.SIP_User_Data, 0, &rootavp ) );
					
					CHECK_FCT( fd_msg_avp_new ( sip_dict.SIP_User_Data_Type, 0, &avp ) );
					CHECK_MALLOC(value.os.data=malloc(length[0]*sizeof(unsigned char)));
					strncpy((char *)value.os.data,(char *)row[0],length[0]);
					value.os.len=(size_t)length[0];
					CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
					CHECK_FCT( fd_msg_avp_add ( rootavp, MSG_BRW_LAST_CHILD, avp) );
					
					
					
					CHECK_FCT( fd_msg_avp_new ( sip_dict.SIP_User_Data_Contents, 0, &avp ) );
					CHECK_MALLOC(value.os.data=malloc(length[1]*sizeof(unsigned char)));
					memcpy(value.os.data,row[1],length[1]);
					value.os.len=(size_t)length[1];
					CHECK_FCT( fd_msg_avp_setvalue ( avp, &value ) );
					CHECK_FCT( fd_msg_avp_add ( rootavp, MSG_BRW_LAST_CHILD, avp) );
					
					//We add SIP_User_Data to message
					CHECK_FCT( fd_msg_avp_add ( message, MSG_BRW_LAST_CHILD, rootavp) );
					not_found=0;
				}
				
			}
			
			mysql_free_result(res);
			free(query);
			free(sipaor_pure);
			break;
			
			default:
				
				//We must never go here, if so, we must stop diameter_sip
				TRACE_DEBUG(INFO,"FATAL ERROR: the datasource is unknown, please check your config file!");
				return 2;
				
				break;
	}
	
	//0 if it was found
	return not_found;
	
	
	
}

int set_pending_flag(const unsigned char  *username, const size_t usernamelen)
{
	CHECK_PARAMS(username && usernamelen);
	
	
	size_t querylen, usernamepurelen;
	char *query, *username_pure;
	
	switch(as_conf->datasource)
	{
		//MySQL
		case ASMYSQL:
			//We allocate the double size of username because at worst it can be all quotes
			username_pure=malloc(usernamelen*2+1);
			//We purify username not to have forbidden characters
			usernamepurelen=mysql_real_escape_string(conn, username_pure, (const char *)username, usernamelen);
			
			
			
			//We clear the flag "authentication pending"
			querylen=SQL_SETFLAG_LEN + usernamepurelen;
			query = malloc(querylen+2);
			snprintf(query, querylen+1, SQL_SETFLAG, username_pure);
			
			if (mysql_query(conn, query)) 
			{
				TRACE_DEBUG(INFO,"Query %s failed", query);
				free(query);
				return 2;
			}
			
			free(query);
			break;
	}	
	return 0;
}
int clear_pending_flag(const unsigned char  *username, const size_t usernamelen)
{
	CHECK_PARAMS(username && usernamelen);
	
	
	size_t querylen, usernamepurelen;
	char *query, *username_pure;
	
	switch(as_conf->datasource)
	{
		//MySQL
		case ASMYSQL:
			//We allocate the double size of username because at worst it can be all quotes
			username_pure=malloc(usernamelen*2+1);
			//We purify username not to have forbidden characters
			usernamepurelen=mysql_real_escape_string(conn, username_pure, (const char *)username, usernamelen);
			
			
			
			//We clear the flag "authentication pending"
			querylen=SQL_CLEARFLAG_LEN + usernamepurelen;
			query = malloc(querylen+2);
			snprintf(query, querylen+1, SQL_CLEARFLAG, username_pure);
			
			if (mysql_query(conn, query)) 
			{
				TRACE_DEBUG(INFO,"Query %s failed", query);
				free(query);
				return 2;
			}
			
			free(query);
		break;
	}	
	return 0;
}



int set_sipserver_uri(const unsigned char  *username, const size_t usernamelen, const unsigned char *sipserver_uri,const size_t sipserverurilen)
{
	CHECK_PARAMS(username && usernamelen && sipserver_uri && sipserverurilen);
	
	
	size_t querylen, usernamepurelen, sipserveruripurelen;
	char *query, *username_pure, *sipserveruri_pure;
	
	switch(as_conf->datasource)
	{
		//MySQL
		case ASMYSQL:
			//We allocate the double size of username because at worst it can be all quotes
			username_pure=malloc(usernamelen*2+1);
			//We purify username not to have forbidden characters
			usernamepurelen=mysql_real_escape_string(conn, username_pure, (const char *)username, usernamelen);
			
			//We allocate the double size of username because at worst it can be all quotes
			sipserveruri_pure=malloc(sipserverurilen*2+1);
			//We purify username not to have forbidden characters
			sipserveruripurelen=mysql_real_escape_string(conn, sipserveruri_pure, (const char *)sipserver_uri, sipserverurilen);
			
			//We clear the flag "authentication pending"
			querylen=SQL_SETSIPURI_LEN + usernamepurelen + sipserveruripurelen;
			query = malloc(querylen+2);
			snprintf(query, querylen+1, SQL_SETSIPURI, sipserveruri_pure,username_pure);
			
			if (mysql_query(conn, query)) 
			{
				TRACE_DEBUG(INFO,"Query %s failed", query);
				free(query);
				return 2;
			}
			
			free(query);
			break;
	}	
	return 0;
}
int remove_sipserver_uri(const unsigned char *sipserver_uri,const size_t sipserverurilen)
{
	CHECK_PARAMS(sipserver_uri && sipserverurilen);
	

	size_t querylen, sipserveruripurelen;
	char *query, *sipserveruri_pure;
	
	switch(as_conf->datasource)
	{
		//MySQL
		case ASMYSQL:
			//We allocate the double size of username because at worst it can be all quotes
			sipserveruri_pure=malloc(sipserverurilen*2+1);
			//We purify username not to have forbidden characters
			sipserveruripurelen=mysql_real_escape_string(conn, sipserveruri_pure, (const char *)sipserver_uri, sipserverurilen);
			
			//We clear the flag "authentication pending"
			querylen=SQL_RMSIPURI_LEN + sipserveruripurelen;
			query = malloc(querylen+2);
			snprintf(query, querylen+1, SQL_RMSIPURI, sipserveruri_pure);
			
			if (mysql_query(conn, query)) 
			{
				TRACE_DEBUG(INFO,"Query %s failed", query);
				free(query);
				return 2;
			}
			
			free(query);
			break;
	}	
	return 0;
}
int set_real_sipserver_uri(const unsigned char  *username, const size_t usernamelen, const unsigned char *sipserver_uri,const size_t sipserverurilen)
{
	CHECK_PARAMS(username && usernamelen && sipserver_uri && sipserverurilen);
	
	
	size_t querylen, usernamepurelen, sipserveruripurelen;
	char *query, *username_pure, *sipserveruri_pure;
	
	switch(as_conf->datasource)
	{
		//MySQL
		case ASMYSQL:
			//We allocate the double size of username because at worst it can be all quotes
			username_pure=malloc(usernamelen*2+1);
			//We purify username not to have forbidden characters
			usernamepurelen=mysql_real_escape_string(conn, username_pure, (const char *)username, usernamelen);
			
			//We allocate the double size of username because at worst it can be all quotes
			sipserveruri_pure=malloc(sipserverurilen*2+1);
			//We purify username not to have forbidden characters
			sipserveruripurelen=mysql_real_escape_string(conn, sipserveruri_pure, (const char *)sipserver_uri, sipserverurilen);
			
			//We clear the flag "authentication pending"
			querylen=SQL_SETREALSIPURI_LEN + usernamepurelen + sipserveruripurelen;
			query = malloc(querylen+2);
			snprintf(query, querylen+1, SQL_SETREALSIPURI, sipserveruri_pure,username_pure);
			
			if (mysql_query(conn, query)) 
			{
				TRACE_DEBUG(INFO,"Query %s failed", query);
				free(query);
				return 2;
			}
			
			free(query);
			break;
	}	
	return 0;
}

int get_sipserver_uri(const unsigned char *sip_aor, const size_t sipaorlen, unsigned char ** sipserver_uri, size_t *sipserverurilen)
{
	CHECK_PARAMS(sip_aor && sipaorlen && sipserver_uri && sipserverurilen );
	
	size_t querylen, sipaorpurelen;
	char *query, *sipaor_pure;
	int not_found=2;
	
	
	
	
	//a sip aor must begin by "sip:" or "sips:" so it must at least be longer than 4 chars
	if(sipaorlen<5)
		return 2;
	
	//NOTE: each method has to purify sip_aor itself. You must remove quotes or special chars for security
	
	switch(as_conf->datasource)
	{
		//MySQL
		case ASMYSQL:
			
			querylen=SQL_GETSIPSERURI_LEN + sipaorlen;
			
			//We allocate the double size of SIP-URI because at worst it can be all quotes
			CHECK_MALLOC(sipaor_pure=malloc(sipaorlen*2+1));
			//We purify SIP-URI not to have forbidden characters
			sipaorpurelen=mysql_real_escape_string(conn, sipaor_pure, (const char *)sip_aor, sipaorlen);
			
			
			query = malloc(querylen+sipaorpurelen+ 2);
			snprintf(query, querylen+1, SQL_GETSIPSERURI, sipaor_pure);
			
			MYSQL_RES *res;
			MYSQL_ROW row;
			
			//We make the query	
			request_mysql(query);
			res=mysql_use_result(conn);
			if(res==NULL)
			{
				//We couldn't make the request
				sipserver_uri=NULL;
				return 2;
			}
			
			not_found=1;
			while ((row = mysql_fetch_row(res)) != NULL)
			{
				*sipserverurilen=strlen(row[0]);
				if(*sipserverurilen>4)
				{
					CHECK_MALLOC(*sipserver_uri=malloc(*sipserverurilen+1));
					strcpy((char *)*sipserver_uri,row[0]);
					not_found=0;
					break;
				}
			}
			mysql_free_result(res);
			free(query);
			free(sipaor_pure);
			break;
			
		default:
			
			//We must never go here, if so, we must stop diameter_sip
			TRACE_DEBUG(INFO,"FATAL ERROR: the datasource is unknown, please check your config file!");
			sipserver_uri=NULL;
			return 2;
			
			break;
	}
	
	//0 if it was found
	return not_found;
	
}


int count_avp(struct msg * message, int code, int vendor)
{
	CHECK_PARAMS(message);
	
	struct avp_hdr *temphdr;
	struct avp *avp;
	int counter=0;
	
	CHECK_FCT(fd_msg_browse (message, MSG_BRW_WALK, &avp, NULL));
	
	while(avp!=NULL)
	{
		
		CHECK_FCT( fd_msg_avp_hdr( avp,&temphdr ));
		
		if(temphdr->avp_code==code && temphdr->avp_vendor==vendor)
		{
			counter++;
		}
		
		CHECK_FCT(fd_msg_browse (avp, MSG_BRW_WALK, &avp, NULL));
	}
	return counter;
}
/* 
void nonce_add_element(char * nonce)
{
	noncechain *newelt=malloc(sizeof(noncechain));
	
	newelt->nonce=nonce;
	
	newelt->timestamp=(int)time(NULL);
	newelt->next=NULL;
	
	if(listnonce==NULL)
	{
		listnonce=newelt;
	}
	else
	{
		noncechain* temp=listnonce;
		
		while(temp->next != NULL)
		{
			if(temp->timestamp < ((int)time(NULL)-300))
			{
				listnonce=temp->next;
				free(temp);
				temp=listnonce;
			}
			temp = temp->next;
		}
		temp->next = newelt;
	}
	
}
void nonce_del_element(char * nonce)
{
	if(listnonce!=NULL)
	{
		noncechain *temp=listnonce, *tempbefore=NULL;
		
		if(listnonce->next==NULL && strcmp(listnonce->nonce,nonce)==0)
		{
			free(listnonce);
			listnonce=NULL;
			return;
		}
		while(temp->next != NULL)
		{
			if(strcmp(temp->nonce,nonce)==0)
			{
				if(tempbefore==NULL)
				{
					listnonce=temp->next;
					free(temp);
					return;
				}
				tempbefore->next=temp->next;
				free(temp);
				break;
			}
			tempbefore=temp;
			temp = temp->next;
		}
		
	}
	
}
int nonce_check_element(char * nonce)
{
	if(listnonce==NULL)
	{
		//Not found
		return 0;
	}
	else
	{
		noncechain* temp=listnonce;
		
		while(temp->next != NULL)
		{
			if(strcmp(temp->nonce,nonce)==0)
				return 1;
			else
				temp = temp->next;
		}
	}
	return 0;
}

void nonce_deletelistnonce()
{
	if(listnonce !=NULL)
	{
		noncechain* temp=listnonce;
	
		while(listnonce->next != NULL)
		{
			temp = listnonce->next;
		
			free(listnonce);
		
			listnonce=temp;
		}
		free(listnonce);
	}
}
*/
