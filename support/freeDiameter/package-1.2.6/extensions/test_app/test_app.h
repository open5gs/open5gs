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

/* Header file for the app_test extension. 
 *
 *  This extension provides a way to send configurable messages on the network
 *
 *  See the app_test.conf.sample file for the format of the configuration file.
 */
 
#include <freeDiameter/extension.h>
#include <signal.h>

#ifndef TEST_APP_DEFAULT_SIGNAL
#define TEST_APP_DEFAULT_SIGNAL	SIGUSR1
#endif /* TEST_APP_DEFAULT_SIGNAL */


/* Mode for the extension */
#define MODE_SERV	0x1
#define	MODE_CLI	0x2
#define	MODE_BENCH	0x4

/* The module configuration */
struct ta_conf {
	uint32_t	vendor_id;	/* default 999999 */
	uint32_t	appli_id;	/* default 123456 */
	uint32_t	cmd_id;		/* default 234567 */
	uint32_t	avp_id;		/* default 345678 */
	uint32_t	long_avp_id;	/* default 0 */
	size_t		long_avp_len;	/* default 5000 */
	int		mode;		/* default MODE_SERV | MODE_CLI */
	char 	*	dest_realm;	/* default local realm */
	char 	*	dest_host;	/* default NULL */
	char 	*	user_name;	/* default NULL */
	int 		signal;		/* default TEST_APP_DEFAULT_SIGNAL */
	int		bench_concur;	/* default 100 */
	int		bench_duration; /* default 10 */
	struct ta_stats {
		unsigned long long	nb_echoed; /* server */
		unsigned long long	nb_sent;   /* client */
		unsigned long long	nb_recv;   /* client */
		unsigned long long	nb_errs;   /* client */
		unsigned long		shortest;  /* fastest answer, in microseconds */
		unsigned long		longest;   /* slowest answer, in microseconds */
		unsigned long		avg;       /* average answer time, in microseconds */
	} stats;
	pthread_mutex_t		stats_lock;
};
extern struct ta_conf * ta_conf;

/* Parse the configuration file */
int ta_conf_handle(char * conffile);

/* Handle incoming messages (server) */
int ta_serv_init(void);
void ta_serv_fini(void);

/* Create outgoing message (client) */
int ta_cli_init(void);
void ta_cli_fini(void);

/* Benchmark flavour */
int ta_bench_init(void);
void ta_bench_fini(void);

/* Initialize dictionary definitions */
int ta_dict_init(void);


/* Some global variables for dictionary */
extern struct dict_object * ta_vendor;
extern struct dict_object * ta_appli;
extern struct dict_object * ta_cmd_r;
extern struct dict_object * ta_cmd_a;
extern struct dict_object * ta_avp;
extern struct dict_object * ta_avp_long;

extern struct dict_object * ta_sess_id;
extern struct dict_object * ta_origin_host;
extern struct dict_object * ta_origin_realm;
extern struct dict_object * ta_dest_host;
extern struct dict_object * ta_dest_realm;
extern struct dict_object * ta_user_name;
extern struct dict_object * ta_res_code;
