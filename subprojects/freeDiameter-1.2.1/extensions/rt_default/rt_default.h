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

/* Header file for the rt_default extension. 
 *
 *  See the rt_default.conf.sample file for the format of the configuration file.
 */
 
/* FreeDiameter's common include file */
#include <freeDiameter/extension.h>

/* Host configuration for this specific extension */
#include <rt_default-host.h>

/* Parse the configuration file */
int rtd_conf_handle(char * conffile);

/* Initialize the rules repository */
int rtd_init(void);

/* Destroy the rules repository */
void rtd_fini(void);

/* Some constants definitions */
enum rtd_crit_type {
	RTD_CRI_ALL	= 0,
	RTD_CRI_OH,
	RTD_CRI_OR,
	RTD_CRI_DH,
	RTD_CRI_DR,
	RTD_CRI_UN,
	RTD_CRI_SI,
	
	RTD_CRI_MAX
};

enum rtd_targ_type {
	RTD_TAR_ID     = 0,
	RTD_TAR_REALM,
	
	RTD_TAR_MAX
};

#define RTD_CRIT_REG	0x1
#define RTD_TARG_REG	0x2

/* Add a rule */
int rtd_add(enum rtd_crit_type ct, char * criteria, enum rtd_targ_type tt, char * target, int score, int flags);

/* Process a message & peer list through the rules repository, updating the scores */
int rtd_process( struct msg * msg, struct fd_list * candidates );

/* For debug: dump the rule repository */
void rtd_dump(void);
