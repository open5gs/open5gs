/*****************************************************************************************************
 * Software License Agreement (BSD License)
 * Author : Souheil Ben Ayed <souheil@tera.ics.keio.ac.jp>
 *
 * Copyright (c) 2009-2010, Souheil Ben Ayed, Teraoka Laboratory of Keio University, and the WIDE Project
 * All rights reserved.
 *
 * Redistribution and use of this software in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by Souheil Ben Ayed <souheil@tera.ics.keio.ac.jp>.
 *
 * 4. Neither the name of Souheil Ben Ayed, Teraoka Laboratory of Keio University or the WIDE Project nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************************************/

#include "diameap_mysql.h"

static pthread_mutex_t db_cs_mutex =
PTHREAD_MUTEX_INITIALIZER;

int diameap_get_eap_user(struct eap_user * user, char * username)
{
	TRACE_ENTRY("%p %p",user,username);
	if (db_conn == NULL)
	{
		TRACE_DEBUG(INFO, "%sNot connected to the MySQL Database server.",DIAMEAP_EXTENSION);

		return EINVAL;

	}
	mysql_thread_init();

	MYSQL_RES *res;
	MYSQL_ROW row;
	char * query;
	CHECK_MALLOC(query=(char *)malloc(sizeof(char)*255));

	sprintf(
			query,
			"SELECT id,username,password,eapmethod, vendor FROM users WHERE  users.username='%s' and users.active='Y' ",
			username);

	CHECK_POSIX(pthread_mutex_lock( &db_cs_mutex ));

	if (mysql_query(db_conn, query))
	{
		CHECK_POSIX(pthread_mutex_unlock( &db_cs_mutex ));
		TRACE_DEBUG(INFO, "%sQuery execution fail. %s",DIAMEAP_EXTENSION, mysql_error(db_conn));
		mysql_thread_end();
		free(query);
		query = NULL;
		return EINVAL;
	}

	res = mysql_store_result(db_conn);

	CHECK_POSIX(pthread_mutex_unlock( &db_cs_mutex ));

	if ((row = mysql_fetch_row(res)) != NULL)
	{

		user->id = atoi(row[0]);
		CHECK_MALLOC(user->userid=malloc(strlen(row[1])+1));
		memcpy(user->userid,row[1],strlen(row[1])+1);
		user->useridLength = strlen(row[1]);
		CHECK_MALLOC(user->password=malloc(strlen(row[2])+1));
		memcpy(user->password, row[2],strlen(row[2])+1);
		user->passwordLength = strlen(row[2]);
		user->proposed_eap_method = atoi(row[3]);
		user->proposed_eap_method_vendor = atoi(row[4]);

		mysql_free_result(res);
		mysql_thread_end();
		free(query);
		query = NULL;
		return 0;
	}

	TRACE_DEBUG(INFO, "%sUser unavailable.",DIAMEAP_EXTENSION);
	mysql_free_result(res);
	mysql_thread_end();
	free(query);
	query = NULL;
	return EINVAL;

}

int diameap_authentication_get_attribs(struct eap_user *user,
		struct fd_list * attribute_list)
{

	TRACE_ENTRY("%p %p",user,attribute_list);

	if (db_conn == NULL)
	{
		TRACE_DEBUG(INFO, "%sNot connected to the MySQL Database server.",DIAMEAP_EXTENSION);

		return EINVAL;

	}

	mysql_thread_init();
	MYSQL_RES *res;
	MYSQL_ROW row;
	char * query;
	CHECK_MALLOC(query=malloc(sizeof(char)*255));

	sprintf(
			query,
			"SELECT `authe`.`attribute` ,`authe`.`value` FROM `authe` WHERE `authe`.`grp` IN ( SELECT `user_grp`.`grp` FROM `user_grp` WHERE `user_grp`.`user` = %d ) ",
			user->id);

	CHECK_POSIX(pthread_mutex_lock( &db_cs_mutex ));

	if (mysql_query(db_conn, query))
	{
		CHECK_POSIX(pthread_mutex_unlock( &db_cs_mutex ));
		TRACE_DEBUG(INFO, "%sQuery execution fail. %s",DIAMEAP_EXTENSION, mysql_error(db_conn));
		mysql_thread_end();
		free(query);
		query = NULL;
		return EINVAL;
	}

	res = mysql_store_result(db_conn);

	CHECK_POSIX(pthread_mutex_unlock( &db_cs_mutex ));

	while ((row = mysql_fetch_row(res)))
	{
		struct auth_attribute * attribute;
		CHECK_MALLOC(attribute = malloc(sizeof(struct auth_attribute)));
		memset(attribute, 0, sizeof(struct auth_attribute));
		fd_list_init(&attribute->chain, NULL);
		attribute->attrib = strdup(row[0]);
		attribute->op = NULL;
		attribute->value = strdup(row[1]);
		fd_list_insert_before(attribute_list, &attribute->chain);

	}

	mysql_free_result(res);
	mysql_thread_end();
	free(query);
	query = NULL;
	return 0;
}

int diameap_authorization_get_attribs(struct eap_user *user,
		struct fd_list * attribute_list)
{
	TRACE_ENTRY("%p %p",user,attribute_list);

	if (db_conn == NULL)
	{
		TRACE_DEBUG(INFO, "%sNot connected to the MySQL Database server.",DIAMEAP_EXTENSION);

		return EINVAL;

	}

	mysql_thread_init();

	MYSQL_RES *res;
	MYSQL_ROW row;
	char * query;
	CHECK_MALLOC(query=malloc(sizeof(char)*255));

	sprintf(
			query,
			"SELECT `authz`.`attribute` , `authz`.`op` , `authz`.`value` FROM `authz` WHERE `authz`.`grp` IN ( SELECT `user_grp`.`grp` FROM `user_grp` WHERE `user_grp`.`user` = %d ) ",
			user->id);

	CHECK_POSIX(pthread_mutex_lock( &db_cs_mutex ));

	if (mysql_query(db_conn, query))
	{
		CHECK_POSIX(pthread_mutex_unlock( &db_cs_mutex ));
		TRACE_DEBUG(INFO, "%sQuery execution fail. %s",DIAMEAP_EXTENSION, mysql_error(db_conn));
		mysql_thread_end();
		free(query);
		query = NULL;
		return EINVAL;
	}

	res = mysql_store_result(db_conn);

	CHECK_POSIX(pthread_mutex_unlock( &db_cs_mutex ));

	while ((row = mysql_fetch_row(res)))
	{
		struct auth_attribute * attribute;
		CHECK_MALLOC(attribute = malloc(sizeof(struct auth_attribute)));
		memset(attribute, 0, sizeof(struct auth_attribute));
		fd_list_init(&attribute->chain, NULL);
		attribute->attrib = strdup(row[0]);
		attribute->op = strdup(row[1]);
		attribute->value = strdup(row[2]);
		fd_list_insert_before(attribute_list, &attribute->chain);
	}

	mysql_free_result(res);
	mysql_thread_end();
	free(query);
	query = NULL;
	return 0;
}

void diameap_mysql_disconnect()
{
	mysql_close(db_conn);
}
