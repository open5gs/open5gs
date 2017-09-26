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

#include "fdproto-internal.h"

/* Initialize a list element */
void fd_list_init ( struct fd_list * list, void * obj )
{
	memset(list, 0, sizeof(struct fd_list));
	list->next = list;
	list->prev = list;
	list->head = list;
	list->o    = obj;
}

#define CHECK_SINGLE( li ) {			\
	ASSERT( ((struct fd_list *)(li))->next == (li) );	\
	ASSERT( ((struct fd_list *)(li))->prev == (li) );	\
	ASSERT( ((struct fd_list *)(li))->head == (li) );	\
}

/* insert after a reference, checks done */
static void list_insert_after( struct fd_list * ref, struct fd_list * item )
{
	item->prev = ref;
	item->next = ref->next;
	item->head = ref->head;
	ref->next->prev = item;
	ref->next = item;
}

/* insert after a reference */
void fd_list_insert_after  ( struct fd_list * ref, struct fd_list * item )
{
	ASSERT(item != NULL);
	ASSERT(ref != NULL);
	CHECK_SINGLE ( item );
	ASSERT(ref->head != item);
	list_insert_after(ref, item);
}

/* Move all elements of list senti at the end of list ref */
void fd_list_move_end(struct fd_list * ref, struct fd_list * senti)
{
	struct fd_list * li;
	ASSERT(ref->head == ref);
	ASSERT(senti->head == senti);
	
	if (senti->next == senti)
		return;
	
	for (li = senti->next; li != senti; li = li->next)
		li->head = ref;
	
	senti->next->prev = ref->prev;
	ref->prev->next   = senti->next;
	senti->prev->next = ref;
	ref->prev         = senti->prev;
	senti->prev = senti;
	senti->next = senti;
}

/* insert before a reference, checks done */
static void list_insert_before ( struct fd_list * ref, struct fd_list * item )
{
	item->prev = ref->prev;
	item->next = ref;
	item->head = ref->head;
	ref->prev->next = item;
	ref->prev = item;
}

/* insert before a reference */
void fd_list_insert_before ( struct fd_list * ref, struct fd_list * item )
{
	ASSERT(item != NULL);
	ASSERT(ref != NULL);
	CHECK_SINGLE ( item );
	ASSERT(ref->head != item);
	list_insert_before(ref, item);
}

/* Insert an item in an ordered list -- ordering function provided. If duplicate object found, it is returned in ref_duplicate */
int fd_list_insert_ordered( struct fd_list * head, struct fd_list * item, int (*cmp_fct)(void *, void *), void ** ref_duplicate)
{
	struct fd_list * ptr = head;
	int cmp;
	
	/* Some debug sanity checks */
	ASSERT(head != NULL);
	ASSERT(item != NULL);
	ASSERT(cmp_fct != NULL);
	ASSERT(head->head == head);
	CHECK_SINGLE ( item );
	
	/* loop in the list */
	while (ptr->next != head)
	{
		/* Compare the object to insert with the next object in list */
		cmp = cmp_fct( item->o, ptr->next->o );
		if (!cmp) {
			/* An element with the same key already exists */
			if (ref_duplicate != NULL)
				*ref_duplicate = ptr->next->o;
			return EEXIST;
		}
		
		if (cmp < 0)
			break; /* We must insert the element here */
		
		ptr = ptr->next;
	}
	
	/* Now insert the element between ptr and ptr->next */
	list_insert_after( ptr, item );
	
	/* Ok */
	return 0;
}
	
/* Unlink an object */
void fd_list_unlink ( struct fd_list * item )
{
	ASSERT(item != NULL);
	if (item->head == item)
		return;
	/* unlink */
	item->next->prev = item->prev;
	item->prev->next = item->next;
	/* sanitize */
	item->next = item;
	item->prev = item;
	item->head = item;
}


