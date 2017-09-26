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

#include "acl_wl.h"

/* The configuration simply contains the allowed fqdn and/or domains (*.example.net)
 * It is represented similarly to the DNS tree:
 *              (root)--___
 *              /    \     \
 *            tld1  tld2   (tld3...)
 *             /      |
 *          label1   label2
 *                    /   \
 *                 lbl21 lbl22
 *                  /       \
 *               lbl211      *
 *
 * This tree would whitelist:
 *   - label1.tld1
 *   - lbl211.lbl21.label2.tld2
 *   - *.lbl22.label2.tld2
 *
 * The functions to add and search the tree are in aw_tree.c.
 *
 */
 
/* Maximum depth of the tree. We set a static size to avoid dynamic allocations. We report an error if this is not sufficient. */
#define AW_TREE_MAXDEPTH 10

/* An element of the tree */
struct tree_item {
	struct fd_list	chain;		/* Link to elements at the same level. Ordered alphabetically. */
	struct fd_list	children;	/* Sentinel for the subtree. */
	char *		str;	/* the \0 terminated label, or NULL if it is a generic container ("*") */
	int		flags;	/* PI_SEC_* flags */
	int		leaf;	/* true if this item can be a leaf of the tree */
};

/* The root of the tree */
static struct fd_list tree_root = FD_LIST_INITIALIZER(tree_root);

/* Note: we don't need to lock, since we add only when parsing the conf, and then read only */


/* The parsed name */
struct split_name {
	struct {
		char * str;	/* start of this label */
		size_t len;	/* length of this label. It does not include the final "." or "\0" */
	}   label[AW_TREE_MAXDEPTH];
	int last_lbl;	/* idx of last label defined */
};

/* The following function explodes a name into a split_name structure */
static int parse_name(char * name, struct split_name * result)
{
	int i, l, prev_offset;
	
	TRACE_ENTRY("%p %p", name, result);
	
	/* First, initialize the result array */
	memset(result, 0, sizeof(struct split_name));
	result->label[0].str = name;
	l = 0; prev_offset = 0;
	
	for (i=0; name[i] != '\0'; i++) {
		if (name[i]=='.') {
			l++;
			CHECK_PARAMS( l < AW_TREE_MAXDEPTH );
			
			/* The previous label is complete, write its size */
			result->label[l - 1].len = i - prev_offset;
			prev_offset = i + 1;
			
			/* Write the start of the new label */
			result->label[l].str = name + i + 1;
		}
	}
	
	/* Finally, write the size of the last label */
	result->label[l].len = i - prev_offset;
	
	result->last_lbl = l;
	
#if 0
	fd_log_debug("Parsed name %s as:", name);
	for (i=0; i<=l; i++)
		fd_log_debug("  str[%d] len: %d, v:%.*s", i, result->label[i].len, result->label[i].len, result->label[i].str);
#endif /* 0 */
	return 0;
}

/* Create a new tree_item structure */
static struct tree_item * new_ti(char * str, size_t len, int flags, int leaf)
{
	struct tree_item * ti;
	char * s = NULL;
	
	TRACE_ENTRY("%p %zd %x", str, len, flags);
	
	if (str) {
		CHECK_MALLOC_DO(s = malloc(len + 1), return NULL);
		memcpy(s, str, len);
		s[len] = '\0';
	}
	
	CHECK_MALLOC_DO( ti = malloc(sizeof(struct tree_item)), {free(s); return NULL; } );
	memset(ti, 0, sizeof(struct tree_item));
	
	fd_list_init(&ti->chain, ti);
	fd_list_init(&ti->children, ti);
	ti->str = s;
	ti->flags = flags;
	ti->leaf = leaf;
	
	return ti;
}

/* Recursively delete a subtree */
static void delete_tree(struct fd_list * senti)
{
	while (!FD_IS_LIST_EMPTY(senti)) {
		struct tree_item * ti = (struct tree_item *)(senti->next);
		
		/* Delete recursively its children first */
		delete_tree(&ti->children);
		
		/* Now, unlink from the sentinel list */
		fd_list_unlink(&ti->chain);
		
		/* destroy this tree item */
		free(ti->str);
		free(ti);
	}
}

/* Top-level destroy function */
void aw_tree_destroy(void)
{
	delete_tree(&tree_root);
}

/* Display the content of a subtree */
static void tree_dump(struct fd_list * sub, int indent)
{
	struct fd_list * li;
	for (li = sub->next; li != sub; li = li->next) {
		struct tree_item * ti = (struct tree_item *)li;
		char buf[1024];
		snprintf(buf, sizeof(buf), "%*s%s", indent * 2, "", ti->str?:"*");
		if (ti->leaf)
			snprintf(buf+strlen(buf), sizeof(buf)-strlen(buf), " (flag:%x)", ti->flags);
		fd_log_debug("%s", buf);
		tree_dump(&ti->children, indent + 1);
	}
}

/* Top-level function */
void aw_tree_dump(void)
{
	fd_log_debug("[acl_wl] tree dump:");
	fd_log_debug("(root)");
	tree_dump(&tree_root, 1);
	fd_log_debug("[acl_wl] end of dump");
}

/* Function to add a new entry in the tree */
int aw_tree_add(char * name, int flags)
{
	struct split_name sn;
	struct tree_item * ti;
	struct fd_list * li, *senti;
	int lbl, found;
	
	TRACE_ENTRY("%p %x", name, flags);
	CHECK_PARAMS(name && *name);
	
	CHECK_FCT_DO( parse_name(name, &sn), 
		{ 
			fd_log_debug("The name '%s' contains too many labels, try a generic (*) or recompile with bigger AW_TREE_MAXDEPTH value (cur: %d)", name, AW_TREE_MAXDEPTH); 
			return EINVAL; 
		} );
		
	senti = &tree_root;
	
	for (lbl = sn.last_lbl; lbl > 0; lbl--) {
		/* Check if the list is empty, we can directly create the new entry */
		if (FD_IS_LIST_EMPTY(senti)) {
			CHECK_MALLOC( ti = new_ti(sn.label[lbl].str, sn.label[lbl].len, 0, 0 /* flags are only set in the terminals */) );
			/* Insert this label in the sentinel sublist */
			fd_list_insert_after(senti, &ti->chain);
			/* Update the sentinel */
			senti = &ti->children;
			/* loop to the next label */
			continue;
		}
		
		/* Check if we have a '*' element already that overlapses */
		ti = (struct tree_item *)(senti->next);
		if (ti->str == NULL) {
			fd_log_debug("[acl_wl] Warning: entry '%s' is superseeded by a generic entry at label %d, ignoring.", name, lbl + 1);
			return 0;
		}
		
		/* Search this label in the ordered list */
		found = 0;
		for (li = senti->next; li != senti; li=li->next) {
			int cmp, len;
			ti = (struct tree_item *)li;
			
			cmp = strncasecmp(ti->str, sn.label[lbl].str, sn.label[lbl].len);
			if (cmp > 0)
				break;	/* the new label must be inserted before li */
			if (cmp < 0)
				continue;
			
			/* Check the lengths */
			len = strlen(ti->str);
			if (len > sn.label[lbl].len)
				break;	/* the new label must be inserted before li */
			if (len < sn.label[lbl].len)
				continue;
			
			/* We already had this label */
			found = 1;
			senti = &ti->children;
			break;
		}
		
		if (found)
			continue;
		
		/* Otherwise, we have to create a new ti, and add it before li */
		CHECK_MALLOC( ti = new_ti(sn.label[lbl].str, sn.label[lbl].len, 0, 0 /* flags are only set in the terminals */) );
		/* Insert this label in the sentinel sublist */
		fd_list_insert_before(li, &ti->chain);
		/* Update the sentinel */
		senti = &ti->children;
	}
	
	ti = NULL;
	li = senti;
	
	/* At this point, senti points to the list where we are supposed to insert our last label. */
	if (sn.label[0].str[0] == '*') {
		if (!FD_IS_LIST_EMPTY(senti)) {
			fd_log_debug("[acl_wl] Warning: entry '%s' overwrites previous more detailed entries, these are deleted.", name);
			delete_tree(senti);
		}
		
		/* Create the new entry */
		CHECK_MALLOC( ti = new_ti(NULL, 0, flags, 1) );
	} else {
		if (!FD_IS_LIST_EMPTY(senti)) {
			/* Check we don't have a '*' entry already */
			ti = (struct tree_item *)(senti->next);
			if (ti->str == NULL) {
				fd_log_debug("[acl_wl] Warning: entry '%s' is superseeded by a generic entry at label 1, ignoring.", name);
				return 0;
			}
			
			/* Search the place for the new label */
			for (li = senti->next; li != senti; li=li->next) {
				int cmp, len;
				ti = (struct tree_item *)li;

				cmp = strncasecmp(ti->str, sn.label[0].str, sn.label[0].len);
				if (cmp > 0)
					break;	/* the new label must be inserted before li */
				if (cmp < 0)
					continue;

				/* Check the lengths */
				len = strlen(ti->str);
				if (len > sn.label[0].len)
					break;	/* the new label must be inserted before li */
				if (len < sn.label[0].len)
					continue;

				/* We already had this label */
				if (ti->leaf) {
					fd_log_debug("[acl_wl] Warning: entry '%s' is duplicated, merging the flags.", name);
					ti->flags |= flags;
					return 0;
				} else {
					/* Just mark this entry as a valid leaf also */
					ti->leaf = 1;
					ti->flags = flags;
					return 0;
				}
			}
		}
		
		/* Create the new entry */
		CHECK_MALLOC( ti = new_ti(sn.label[0].str, sn.label[0].len, flags, 1) );
	}
	
	/* The new label is "ti", it is inserted before "li" */
	fd_list_insert_before(li, &ti->chain);
	
	/* Done! */
	return 0;
}

/* Search in the tree. On return, *result =  -1: not found; >=0: found with PI_SEC_* flags */
int aw_tree_lookup(char * name, int * result)
{
	struct split_name sn;
	int lbl, found;
	struct tree_item * ti;
	struct fd_list * senti, *li;
	
	TRACE_ENTRY("%p %p", name, result);
	CHECK_PARAMS(name && result);
	
	/* Initialize */
	*result = -1;
	
	/* Parse the name into labels */
	CHECK_FCT_DO( parse_name(name, &sn), 
		{ 
			TRACE_DEBUG(INFO, "Too many labels in this name, it cannot be found in the tree, skipping."); 
			return 0;
		} );
	
	senti = &tree_root;
	
	for (lbl = sn.last_lbl; lbl >= 0; lbl--) {
		/* Check if the list is empty, we can directly return */
		if (FD_IS_LIST_EMPTY(senti)) {
			/* The item is not found */
			return 0;
		}
		
		/* Check if we have a '*' element */
		ti = (struct tree_item *)(senti->next);
		if (ti->str == NULL) {
			TRACE_DEBUG(ANNOYING, "[acl_wl] %s matched at label %d with a generic entry.", name, lbl + 1);
			*result = ti->flags;
			return 0;
		}

		/* Search this label in the ordered list */
		found = 0;
		for (li = senti->next; li != senti; li=li->next) {
			int cmp, len;
			ti = (struct tree_item *)li;
			
			cmp = strncasecmp(ti->str, sn.label[lbl].str, sn.label[lbl].len);
			if (cmp > 0)
				return 0;	/* the label was not found */
			if (cmp < 0)
				continue;
			
			/* Check the lengths */
			len = strlen(ti->str);
			if (len > sn.label[lbl].len)
				return 0;	/* the label was not found */
			if (len < sn.label[lbl].len)
				continue;
			
			/* We found the label */
			found = 1;
			senti = &ti->children;
			break;
		}
		
		if (!found)
			return 0; /* label not found */
		
		/* otherwise, continue, sentinel has been updated */
	}
	
	/* At the end, ti points to the correct leaf */
	if (!ti->leaf)
		return 0;
	
	TRACE_DEBUG(ANNOYING, "[acl_wl] %s matched exactly.", name);
	*result = ti->flags;
	return 0;
}
