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

/* Functions to create a list of AVPs according to the configuration file */

#include "app_acct.h"

/* Prepare a list of acct_record entries from the configuration, without mapping any value at this time */
int acct_rec_prepare(struct acct_record_list * records)
{
	struct fd_list * li;
	TRACE_ENTRY("%p", records);
	CHECK_PARAMS( records && acct_config );
	
	/* Prepare the records structure */
	memset(records, 0, sizeof(struct acct_record_list));
	fd_list_init(&records->all, records);
	fd_list_init(&records->unmaped, records);
	
	/* for each entry in the configuration */
	for (li = acct_config->avps.next; li != &acct_config->avps; li = li->next) {
		struct acct_conf_avp * a = (struct acct_conf_avp *)li;
		struct acct_record_item * new;
		int i = a->multi ? 1 : 0;
		/* Create as many records as the 'multi' parameter requires */
		do {
			CHECK_MALLOC( new = malloc(sizeof(struct acct_record_item)) );
			memset(new, 0, sizeof(struct acct_record_item));
			fd_list_init(&new->chain, new);
			fd_list_init(&new->unmapd, new);
			new->param = a;
			new->index = i;
			fd_list_insert_before(&records->all, &new->chain);
			fd_list_insert_before(&records->unmaped, &new->unmapd);
			records->nball++;
			records->nbunmap++;
			i++;
		} while (i <= a->multi);
	}
	
	return 0;
}

/* Find the AVPs from configuration inside a received message */
int acct_rec_map(struct acct_record_list * records, struct msg * msg)
{
	struct avp * avp;
	
	TRACE_ENTRY("%p %p", records, msg);
	
	/* For each AVP in the message, search if we have a corresponding unmap'd record */
	CHECK_FCT(  fd_msg_browse(msg, MSG_BRW_FIRST_CHILD, &avp, NULL)  );
	while (avp) {
		struct fd_list * li;
		struct dict_object * model;
		
		CHECK_FCT( fd_msg_model(avp, &model) );
		if (model != NULL) {	/* we ignore the AVPs we don't recognize */
		
			/* Search this model in the list */
			for (li = records->unmaped.next; li != &records->unmaped; li = li->next) {
				struct acct_record_item * r = (struct acct_record_item *)(li->o);
				if (r->param->avpobj == model) {
					/* It matches: save the AVP value and unlink this record from the unmap'd list */
					struct avp_hdr * h;
					CHECK_FCT( fd_msg_avp_hdr( avp, &h ) );
					r->value = h->avp_value;
					fd_list_unlink(&r->unmapd);
					records->nbunmap -= 1;
					break;
				}
			}

			/* Continue only while there are some AVPs to map */
			if (FD_IS_LIST_EMPTY(&records->unmaped))
				break;
		}
		
		/* Go to next AVP in the message */
		CHECK_FCT( fd_msg_browse(avp, MSG_BRW_NEXT, &avp, NULL) );
	}
	
	/* Done */
	return 0;
}

/* Check that a mapped list is not empty and no required AVP is missing. Free the record list in case of error */
int acct_rec_validate(struct acct_record_list * records)
{
	struct fd_list * li;
	TRACE_ENTRY("%p", records);
	CHECK_PARAMS( records );
	
	/* Check at least one AVP was mapped */
	if (records->nball == records->nbunmap) {
		fd_log_debug("The received ACR does not contain any AVP from the configuration file."
				" This is an invalid situation. Please fix your configuration file."
				" One way to ensure this does not happen is to include Session-Id in the database.");
		acct_rec_empty(records);
		return EINVAL;
	}
	
	/* Now, check there is no required AVP unmap'd */
	for (li = records->unmaped.next; li != &records->unmaped; li = li->next) {
		struct acct_record_item * r = (struct acct_record_item *)(li->o);
		if (r->param->required && (r->index <= 1)) {
			fd_log_debug("The received ACR does not contain the required AVP '%s'.", r->param->avpname);
			acct_rec_empty(records);
			return EINVAL;
		}
	}
	
	/* The record list is OK */
	return 0;
}

/* Free all the items in an acct_record_list returned by acct_rec_prepare */
void acct_rec_empty(struct acct_record_list * records)
{
	TRACE_ENTRY("%p", records);
	CHECK_PARAMS_DO( records, return );
	
	while (!FD_IS_LIST_EMPTY(&records->all)) {
		struct acct_record_item * r = (struct acct_record_item *)(records->all.next);
		fd_list_unlink( &r->chain );
		fd_list_unlink( &r->unmapd );
		free(r);
	}
}
