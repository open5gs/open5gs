/*********************************************************************************************************
* Software License Agreement (BSD License)                                                               *
* Author: Sebastien Decugis <sdecugis@freediameter.net>							 *
*													 *
* Copyright (c) 2011, WIDE Project and NICT								 *
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

/* Header file for the app_acct extension. 
 *
 *  This extension is a simple Diameter Accounting server.
 *
 * It receives the Diameter Accounting-Request message, and sends its content in a 
 * "buffer" postgreSQL database (see configuration sample file app_acct.conf.sample).
 *
 * The intent is that another application will then pick the records from this "buffer" database
 * and process it accordingly to the requirements of the end application.
 */
 
#include <freeDiameter/extension.h>

/* The structure corresponding to an AVP entry in the configuration file */
struct acct_conf_avp {
	/* Chain */
	struct fd_list	 	 chain;		/* link in the global list */
	
	/* Raw information from the configuration file */
	char 			*avpname;	/* the name of the AVP, as appear in the configuration file */
	char			*field;		/* the field name in the database, or NULL if it is the same as avpname */
	int			 required;	/* set to true if this AVP has to be in the message */
	unsigned 		 multi;		/* the number of occurrences of this AVP we convert, or 0 if it was not specified */
	
	/* Parsed information */
	struct dict_object 	*avpobj;	/* the dictionary object corresponding to this AVP */
	enum dict_avp_basetype	 avptype;	/* this info is extracted from avpobj. GROUPED avps are not allowed yet */
};

/* This is described only inside acct_db.c */
struct acct_db;

/* The complete configuration */
struct acct_conf {
	/* AVPs */
	struct fd_list	 avps;		/* the list of acct_conf_avp entries */
	
	/* Raw information */
	char		*conninfo;	/* the connection string to the database, that is passed as is to the database library */
	char 		*tablename;	/* the name of the table we are working with */
	char 		*tsfield;	/* the name of the timestamp field, or NULL if not required */
	char 		*srvnfield;	/* the name of the server name field, or NULL if not required */
};

/* A successfully parsed Accounting-Request produces a list of these: */
struct acct_record_item {
	struct fd_list		 chain;	/* link with all others */
	struct fd_list		 unmapd;/* link with only unmap'd records */
	struct acct_conf_avp 	*param;	/* the AVP entry this refers to. */
	unsigned		 index;	/* in case of multi */
	union avp_value		*value; /* If the AVP was found in the message, this points to its value. Otherwise, NULL */
	union {
		uint32_t v32	/* Storage area for network byte-order copy of the AVP value */;
		uint64_t v64;
		char	 c;	/* pointer that is passed to the database */
	} 			 scalar;/* for scalar AVP (all types except OCTETSTRING) we copy in this area the value in network byte order */
};

/* The sentinel for a list of acct_record_items */
struct acct_record_list {
	struct fd_list	all;	/* The list of records */
	int		nball;	/* The number of records in all */
	struct fd_list	unmaped;/* Only the records without a value */
	int		nbunmap;/* The number of unmap'd records */
};

/* Mapping of the data types between Diameter AVP and PQ types: */
extern const char * diam2db_types_mapping[];

/* In acct_conf.y */
extern struct acct_conf * acct_config;	/* the global configuration */
int acct_conf_init(void);
int acct_conf_parse(char * conffile);
int acct_conf_check(char * conffile);
void acct_conf_free(void);

/* In acct_db.c */
int acct_db_init(void);
int acct_db_insert(struct acct_record_list * records);
void acct_db_free(void);

/* In acct_records.c */
int acct_rec_prepare(struct acct_record_list * records);
int acct_rec_map(struct acct_record_list * records, struct msg * msg);
int acct_rec_validate(struct acct_record_list * records);
void acct_rec_empty(struct acct_record_list * records);
