/*********************************************************************************************************
* Software License Agreement (BSD License)                                                               *
* Author: Sebastien Decugis <sdecugis@freediameter.net>							 *
*													 *
* Copyright (c) 2013, WIDE Project and NICT								 *
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
* * Neither the name of the WIDE Project or NICT nor the 						 *
*   names of its contributors may be used to endorse or 						 *
*   promote products derived from this software without 						 *
*   specific prior written permission of WIDE Project and 						 *
*   NICT.												 *
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

/* Database interface module */

/* There is one connection to the db per thread. 
The connection is stored in the pthread_key_t variable */


#include "app_acct.h"
#include <libpq-fe.h>

const char * diam2db_types_mapping[AVP_TYPE_MAX + 1] = {
	"" 		/* AVP_TYPE_GROUPED */,
	"bytea" 	/* AVP_TYPE_OCTETSTRING */,
	"integer" 	/* AVP_TYPE_INTEGER32 */,
	"bigint" 	/* AVP_TYPE_INTEGER64 */,
	"integer" 	/* AVP_TYPE_UNSIGNED32 + cast */,
	"bigint" 	/* AVP_TYPE_UNSIGNED64 + cast */,
	"real" 		/* AVP_TYPE_FLOAT32 */,
	"double precision" /* AVP_TYPE_FLOAT64 */
};

static const char * stmt = "acct_db_stmt";
#ifndef TEST_DEBUG
static 
#endif /* TEST_DEBUG */
pthread_key_t connk;
static char * sql = NULL;   /* The buffer that will contain the SQL query */
static int nbrecords = 0;


/* Initialize the database context: connection to the DB, prepared statement to insert new records */
int acct_db_init(void)
{
	struct acct_record_list emptyrecords;
	struct fd_list * li;
	size_t sql_allocd = 0; /* The malloc'd size of the buffer */
	size_t sql_offset = 0; /* The actual data already written in this buffer */
	int idx = 0;
	PGresult * res;
	PGconn *conn;
	#define REALLOC_SIZE	1024	/* We extend the buffer by this amount */
	
	TRACE_ENTRY();
	CHECK_PARAMS( acct_config && acct_config->conninfo && acct_config->tablename ); 
	
	CHECK_PARAMS_DO( PQisthreadsafe() == 1, {
		fd_log_debug("You PostGreSQL installation is not thread-safe!");
		return EINVAL;
	} );			
	
	/* Use the information from acct_config to create the connection and prepare the query */
	conn = PQconnectdb(acct_config->conninfo);
	
	/* Check to see that the backend connection was successfully made */
	if (PQstatus(conn) != CONNECTION_OK) {
		fd_log_debug("Connection to database failed: %s", PQerrorMessage(conn));
		acct_db_free();
		return EINVAL;
	}
	if (PQprotocolVersion(conn) < 3) {
		fd_log_debug("Database protocol version is too old, version 3 is required for prepared statements.");
		acct_db_free();
		return EINVAL;
	}
	
	TRACE_DEBUG(FULL, "Connection to database successful, server version %d.", PQserverVersion(conn));
	
	/* Now, prepare the request object */
	
	/* First, we build the list of AVP we will insert in the database */
	CHECK_FCT( acct_rec_prepare(&emptyrecords) );
	
	/* Now, prepare the text of the request */
	CHECK_MALLOC(sql = malloc(REALLOC_SIZE));
	sql_allocd = REALLOC_SIZE;
	
	/* This macro hides the details of extending the buffer on each sprintf... */
	#define ADD_EXTEND(args...) {									\
		size_t p;										\
		int loop = 0;										\
		do {											\
			p = snprintf(sql + sql_offset, sql_allocd - sql_offset, ##args);		\
			if (p >= sql_allocd - sql_offset) {						\
				/* Too short, extend the buffer and start again */			\
				CHECK_MALLOC( sql = realloc(sql, sql_allocd + REALLOC_SIZE) );		\
				sql_allocd += REALLOC_SIZE;						\
				loop++;									\
				ASSERT(loop < 100); /* detect infinite loops */				\
				continue;								\
			}										\
			sql_offset += p;								\
			break;										\
		} while (1);										\
	}
	
	/* This macro allows to add a value in the SQL buffer while escaping in properly */
	#define ADD_ESCAPE(str) {									\
		char * __s = (char *)str;								\
		/* Check we have at least twice the size available +1 */				\
		size_t p = strlen(__s);									\
													\
		while (sql_allocd - sql_offset < 2 * p + 1) {						\
			/* Too short, extend the buffer */						\
			CHECK_MALLOC( sql = realloc(sql, sql_allocd + REALLOC_SIZE) );			\
			sql_allocd += REALLOC_SIZE;							\
		}											\
													\
		/* Now add the escaped string */							\
		p = PQescapeStringConn(conn, sql+sql_offset, __s, p, NULL);				\
		sql_offset += p;									\
	}
	
	/* INSERT INTO table (tsfield, field1, field2, ...) VALUES (now, $1::bytea, $2::integer, ...) */
	ADD_EXTEND("INSERT INTO %s (", acct_config->tablename);
	
	if (acct_config->tsfield) {
		ADD_EXTEND("\"");
		ADD_ESCAPE(acct_config->tsfield);
		ADD_EXTEND("\", ");
	}
	
	if (acct_config->srvnfield) {
		ADD_EXTEND("\"");
		ADD_ESCAPE(acct_config->srvnfield);
		ADD_EXTEND("\", ");
	}
	
	for (li = emptyrecords.all.next; li != &emptyrecords.all; li = li->next) {
		struct acct_record_item * i = (struct acct_record_item *)(li->o);
		ADD_EXTEND("\"");
		ADD_ESCAPE(i->param->field?:i->param->avpname);
		if (i->index) {
			ADD_EXTEND("%d", i->index);
		}
		if (li->next != &emptyrecords.all) {
			ADD_EXTEND("\", ");
		}
	}
	
	ADD_EXTEND("\") VALUES (");
	
	if (acct_config->tsfield) {
		++idx;
		ADD_EXTEND("$%d, ", idx);
	}
	if (acct_config->srvnfield) {
		ADD_EXTEND("'");
		ADD_ESCAPE(fd_g_config->cnf_diamid);
		ADD_EXTEND("', ");
	}
	
	for (li = emptyrecords.all.next; li != &emptyrecords.all; li = li->next) {
		struct acct_record_item * i = (struct acct_record_item *)(li->o);
		++idx;
		ADD_EXTEND("$%d::%s", idx, diam2db_types_mapping[i->param->avptype]);
		
		if (li->next != &emptyrecords.all) {
			ADD_EXTEND(", ");
		}
	}
	
	ADD_EXTEND(");");
	
	TRACE_DEBUG(FULL, "Preparing the following SQL statement: '%s'", sql);
	res = PQprepare(conn, stmt, sql, emptyrecords.nball, NULL);
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
		TRACE_DEBUG(INFO, "Preparing statement '%s' failed: %s",
			sql, PQerrorMessage(conn));
		PQclear(res);
		return EINVAL;
        }
	PQclear(res);
	nbrecords = emptyrecords.nball;
	
	acct_rec_empty(&emptyrecords);
	
	CHECK_POSIX( pthread_key_create(&connk, (void (*)(void*))PQfinish) );
	CHECK_POSIX( pthread_setspecific(connk, conn) );
	
	/* Ok, ready */
	return 0;
}

/* Terminate the connection to the DB */
void acct_db_free(void)
{	
	CHECK_POSIX_DO(pthread_key_delete(connk) , );
	free(sql);
}

/* When a new message has been received, insert the content of the parsed mapping into the DB (using prepared statement) */
int acct_db_insert(struct acct_record_list * records)
{
	char 	**val;
	int	 *val_len;
	int 	 *val_isbin;
	int	  idx = 0;
	int	  size = 0;
	PGresult *res;
	struct fd_list *li;
	PGconn *conn;
	int new = 0;
	
	TRACE_ENTRY("%p", records);
	CHECK_PARAMS( records );
	
	conn = pthread_getspecific(connk);
	if (!conn) {
		conn = PQconnectdb(acct_config->conninfo);
		CHECK_POSIX( pthread_setspecific(connk, conn) );
		
		new = 1;
	}
	
	/* First, check if the connection with the DB has not staled, and eventually try to fix it */
	if (PQstatus(conn) != CONNECTION_OK) {
		/* Attempt a reset */
		PQreset(conn);
		if (PQstatus(conn) != CONNECTION_OK) {
			TRACE_DEBUG(INFO, "Lost connection to the database server, and attempt to reestablish it failed");
			TODO("Terminate the freeDiameter instance completely?");
			return ENOTCONN;
		}
	}
	
	if (new) {
		/* Create the prepared statement for this ocnnection, it is not shared */
		res = PQprepare(conn, stmt, sql, nbrecords, NULL);
		if (PQresultStatus(res) != PGRES_COMMAND_OK) {
			TRACE_DEBUG(INFO, "Preparing statement '%s' failed: %s",
				sql, PQerrorMessage(conn));
			PQclear(res);
			return EINVAL;
        	}
		PQclear(res);
	}
	
	size = 	acct_config->tsfield ? records->nball + 1 : records->nball;
	
	/* Alloc the arrays of parameters */
	CHECK_MALLOC( val       = calloc(size, sizeof(const char *)) );
	CHECK_MALLOC( val_len   = calloc(size, sizeof(const int)) );
	CHECK_MALLOC( val_isbin = calloc(size, sizeof(const int)) );
	
	if (acct_config->tsfield) {
		val[idx] = "now";
		val_len[idx] = 3;
		val_isbin[idx] = 0;
		idx++;
	}
	
	/* Now write all the map'd records in these arrays */
	for (li = records->all.next; li != &records->all; li = li->next) {
		struct acct_record_item * r = (struct acct_record_item *)(li->o);
		if (r->value) {
			val_isbin[idx] = 1; /* We always pass binary parameters */
			switch (r->param->avptype) {
				case AVP_TYPE_OCTETSTRING:
					val[idx] = (void *)(r->value->os.data);
					val_len[idx] = r->value->os.len;
					break;
					
				case AVP_TYPE_INTEGER32:
				case AVP_TYPE_UNSIGNED32:
				case AVP_TYPE_FLOAT32:
					r->scalar.v32 = htonl(r->value->u32);
					val[idx] = &r->scalar.c;
					val_len[idx] = sizeof(uint32_t);
					break;
					
				case AVP_TYPE_INTEGER64:
				case AVP_TYPE_UNSIGNED64:
				case AVP_TYPE_FLOAT64:
					r->scalar.v64 = htonll(r->value->u64);
					val[idx] = &r->scalar.c;
					val_len[idx] = sizeof(uint64_t);
					break;
				
				default:
					ASSERT(0); /* detect bugs */
			}
		}
		
		idx++;
	}
	
	/* OK, now execute the SQL statement */
	res = PQexecPrepared(conn, stmt, size, (const char * const *)val, val_len, val_isbin, 1 /* We actually don't care here */);
	
	/* Done with the parameters */
	free(val);
	free(val_len);
	free(val_isbin);
	
	/* Now check the result code */
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
		TRACE_DEBUG(INFO, "An error occurred while INSERTing in the database: %s", PQerrorMessage(conn));
		PQclear(res);
		return EINVAL; /* It was probably a mistake in configuration file... */
        }
	PQclear(res);
	
	/* Ok, we are done */
	return 0;
}


