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

#include "dict_lxml.h"

/*
The internal freeDiameter dictionary has strong dependency relations between
the different objects, as follow:

           vendor
           /    \
   application   \
     /    \      |
 command   \     |
  |       type   |
  |       /   \  |
   \  enumval  \ | 
    \          avp
     \    _____/    
      \  /
      rule

It means an AVP cannot be defined unless the parent TYPE has already been defined, 
in turn depending on parent APPLICATION, etc. (top-to-bottom dependencies on the graph)

On the other hand, the hierarchy of the XML format described in draft-frascone-xml-dictionary-00 
does not enforce most of these dependencies, the structure is as follows:

 vendor     application
	   /     |     \
      command    |     avp 
	 /      type     \
       rule	        enumval

(in addition if DTD validation was performed, command and avp refer to vendor, avp refers to type, 
but we do not do it for larger compatibility -- we just report when errors are found)

As a consequence of this difference, it is impossible to parse the XML tree and create the dictionary objects in freeDiameter
in only 1 pass. To avoid parsing the tree several times, we use a temporary structure in memory to contain all the data
from the XML file, and when the parsing is complete we store all the objects in the dictionary. 
*/

/* We use the SAX interface of libxml2 (from GNOME) to parse the XML file. */ 
#include <libxml/parser.h>

/*******************************************/
 /* Helper functions */
static int xmltoint(xmlChar * xmlinteger, uint32_t * conv) {
	TRACE_ENTRY("%p %p", xmlinteger, conv);
	
	/* Attempt at converting the string to an integer */
	if (sscanf((char *)xmlinteger, "%u", conv) != 1) {
		TRACE_DEBUG(INFO, "Unable to convert '%s' to integer.", (char *)xmlinteger)
		return EINVAL;
	}
	
	return 0;
}


/*******************************************
 The temporary structure that is being built when the XML file is parsed 
 *******************************************/

/* VENDOR */
struct t_vend {
	struct fd_list 	chain; /* link in the t_dictionary->vendors */
	uint32_t	id;
	uint8_t *	name;
};

static int new_vendor(struct fd_list * parent, xmlChar * xmlid, xmlChar * xmlname) {
	struct t_vend * new;
	uint32_t id = 0;
	
	TRACE_ENTRY("%p %p %p", parent, xmlid, xmlname);
	CHECK_PARAMS( parent && xmlid && xmlname );
	
	CHECK_FCT( xmltoint(xmlid, &id) );
	
	CHECK_MALLOC( new = malloc(sizeof(struct t_vend)) );
	memset(new, 0, sizeof(struct t_vend));
	fd_list_init(&new->chain, NULL);
	new->id = id;
	CHECK_MALLOC( new->name = (uint8_t *)strdup((char *)xmlname) );
	
	fd_list_insert_before(parent, &new->chain);
	
	return 0;
}

static void dump_vendor(struct t_vend * v) {
	fd_log_debug(" Vendor %d:'%s'", v->id, (char *)v->name);
}

static void del_vendor_contents(struct t_vend * v) {
	TRACE_ENTRY("%p", v);
	free(v->name);
}


/* RULE */
struct t_rule {
	struct fd_list  chain;	/* link in either t_cmd or t_avp */
	uint8_t *	avpname;
	int		max;
	int		min;
};

static int new_rule(struct fd_list * parent, xmlChar * xmlname, /* position is never used */ xmlChar * xmlmaximum, xmlChar * xmlminimum) {
	struct t_rule * new;
	uint32_t min, max;
	
	TRACE_ENTRY("%p %p %p %p", parent, xmlname, xmlmaximum, xmlminimum);
	CHECK_PARAMS( parent && xmlname );
	
	CHECK_MALLOC( new = malloc(sizeof(struct t_rule)) );
	memset(new, 0, sizeof(struct t_rule));
	fd_list_init(&new->chain, NULL);
	if (xmlminimum) {
		CHECK_FCT( xmltoint(xmlminimum, &min) );
		new->min = (int) min;
	} else {
		new->min = -1;
	}
	if (xmlmaximum) {
		CHECK_FCT( xmltoint(xmlmaximum, &max) );
		new->max = (int) max;
	} else {
		new->max = -1;
	}
	CHECK_MALLOC( new->avpname = (uint8_t *)strdup((char *)xmlname) );
	
	fd_list_insert_before(parent, &new->chain);
	
	return 0;
}

static void dump_rule(struct t_rule * r, char * prefix) {
	fd_log_debug("%s ", prefix);
	if (r->min != -1)
		fd_log_debug("m:%d ", r->min);
	if (r->max != -1)
		fd_log_debug("M:%d ", r->max);
	fd_log_debug("%s", (char *)r->avpname);
}

static void del_rule_contents(struct t_rule * r) {
	TRACE_ENTRY("%p",r);
	free(r->avpname);
}


/* COMMAND */
struct t_cmd {
	struct fd_list  chain;    /* link in t_appl->commands */
	uint32_t	code;
	uint8_t *	name;
	uint8_t		flags;
	uint8_t		fmask;
	struct fd_list  reqrules_fixed;     /* list of t_rule */
	struct fd_list  reqrules_required;  /* list of t_rule */
	struct fd_list  reqrules_optional;  /* list of t_rule */
	struct fd_list  ansrules_fixed;     /* list of t_rule */
	struct fd_list  ansrules_required;  /* list of t_rule */
	struct fd_list  ansrules_optional;  /* list of t_rule */
};

static int new_cmd(struct fd_list * parent, xmlChar * xmlcode, xmlChar * xmlname /*, ignore the vendor id because we don't use it */, xmlChar * xmlpbit, struct t_cmd **ret) {
	struct t_cmd * new;
	uint32_t code;
	uint32_t flag = 0;
	uint32_t fmask = 0;
	
	TRACE_ENTRY("%p %p %p %p", parent, xmlcode, xmlname, xmlpbit);
	CHECK_PARAMS( parent && xmlcode && xmlname );
	
	CHECK_FCT( xmltoint(xmlcode, &code) );
	
	if (xmlpbit) {
		uint32_t val;
		CHECK_FCT( xmltoint(xmlpbit, &val) );
		fmask |= CMD_FLAG_PROXIABLE;
		if (val)
			flag |= CMD_FLAG_PROXIABLE;
	}
	
	CHECK_MALLOC( new = malloc(sizeof(struct t_cmd)) );
	memset(new, 0, sizeof(struct t_cmd));
	fd_list_init(&new->chain, NULL);
	new->code = code;
	CHECK_MALLOC( new->name = (uint8_t *)strdup((char *)xmlname) );
	new->flags = flag;
	new->fmask = fmask;
	fd_list_init(&new->reqrules_fixed, NULL);
	fd_list_init(&new->reqrules_required, NULL);
	fd_list_init(&new->reqrules_optional, NULL);
	fd_list_init(&new->ansrules_fixed, NULL);
	fd_list_init(&new->ansrules_required, NULL);
	fd_list_init(&new->ansrules_optional, NULL);
	
	fd_list_insert_before(parent, &new->chain);
	
	*ret = new;
	
	return 0;
}

static void dump_cmd(struct t_cmd * c) {
	struct fd_list * li;
	fd_log_debug("  Command %d %s: %s", c->code, 
		c->fmask ? ( c->flags ? "[P=1]" : "[P=0]") : "", c->name);
	for (li = c->reqrules_fixed.next; li != &c->reqrules_fixed; li = li->next)
		dump_rule((struct t_rule *)li, "    Request fixed    AVP:");
	for (li = c->reqrules_required.next; li != &c->reqrules_required; li = li->next)
		dump_rule((struct t_rule *)li, "    Request required AVP:");
	for (li = c->reqrules_optional.next; li != &c->reqrules_optional; li = li->next)
		dump_rule((struct t_rule *)li, "    Request optional AVP:");
	for (li = c->ansrules_fixed.next; li != &c->ansrules_fixed; li = li->next)
		dump_rule((struct t_rule *)li, "    Answer fixed    AVP:");
	for (li = c->ansrules_required.next; li != &c->ansrules_required; li = li->next)
		dump_rule((struct t_rule *)li, "    Answer required AVP:");
	for (li = c->ansrules_optional.next; li != &c->ansrules_optional; li = li->next)
		dump_rule((struct t_rule *)li, "    Answer optional AVP:");
}

static void del_cmd_contents(struct t_cmd * c) {
	TRACE_ENTRY("%p", c);
	free(c->name);
	while (!FD_IS_LIST_EMPTY(&c->reqrules_fixed)) {
		struct fd_list * li = c->reqrules_fixed.next;
		fd_list_unlink(li);
		del_rule_contents((struct t_rule *)li);
		free(li);
	}
	while (!FD_IS_LIST_EMPTY(&c->reqrules_required)) {
		struct fd_list * li = c->reqrules_required.next;
		fd_list_unlink(li);
		del_rule_contents((struct t_rule *)li);
		free(li);
	}
	while (!FD_IS_LIST_EMPTY(&c->reqrules_optional)) {
		struct fd_list * li = c->reqrules_optional.next;
		fd_list_unlink(li);
		del_rule_contents((struct t_rule *)li);
		free(li);
	}
	while (!FD_IS_LIST_EMPTY(&c->ansrules_fixed)) {
		struct fd_list * li = c->ansrules_fixed.next;
		fd_list_unlink(li);
		del_rule_contents((struct t_rule *)li);
		free(li);
	}
	while (!FD_IS_LIST_EMPTY(&c->ansrules_required)) {
		struct fd_list * li = c->ansrules_required.next;
		fd_list_unlink(li);
		del_rule_contents((struct t_rule *)li);
		free(li);
	}
	while (!FD_IS_LIST_EMPTY(&c->ansrules_optional)) {
		struct fd_list * li = c->ansrules_optional.next;
		fd_list_unlink(li);
		del_rule_contents((struct t_rule *)li);
		free(li);
	}
}

/* TYPE */
struct t_typedefn {
	struct fd_list  chain;	/* link in t_appl->types */
	uint8_t *	name;
	uint8_t *	parent_name;
};

static int new_type(struct fd_list * parent, xmlChar * xmlname, xmlChar * xmlparent /*, xmlChar * xmldescription -- ignore */) {
	struct t_typedefn * new;
	
	TRACE_ENTRY("%p %p %p", parent, xmlname, xmlparent);
	CHECK_PARAMS( parent && xmlname );
	
	CHECK_MALLOC( new = malloc(sizeof(struct t_typedefn)) );
	memset(new, 0, sizeof(struct t_typedefn));
	fd_list_init(&new->chain, NULL);
	CHECK_MALLOC( new->name = (uint8_t *)strdup((char *)xmlname) );
	if (xmlparent) {
		CHECK_MALLOC( new->parent_name = (uint8_t *)strdup((char *)xmlparent) );
	}
	
	fd_list_insert_before(parent, &new->chain);
	
	return 0;
}

static void dump_type(struct t_typedefn * t) {
	fd_log_debug("  Type %s%s%s%s", (char *)t->name,
		     t->parent_name ? "(parent: " : "", 
		     t->parent_name ? (char *)t->parent_name : "", 
		     t->parent_name ? ")" : "");
}

static void del_type_contents(struct t_typedefn * t) {
	TRACE_ENTRY("%p", t);
	free(t->name);
	free(t->parent_name);
}
	

/* TYPE INSIDE AVP */
struct t_avptype {
	struct fd_list  chain;  /* link in t_avp->type */
	uint8_t *	type_name;
};

static int new_avptype(struct fd_list * parent, xmlChar * xmlname) {
	struct t_avptype * new;
	
	TRACE_ENTRY("%p %p", parent, xmlname);
	CHECK_PARAMS( parent && xmlname );
	
	CHECK_MALLOC( new = malloc(sizeof(struct t_avptype)) );
	memset(new, 0, sizeof(struct t_avptype));
	fd_list_init(&new->chain, NULL);
	CHECK_MALLOC( new->type_name = (uint8_t *)strdup((char *)xmlname) );
	
	fd_list_insert_before(parent, &new->chain);
	
	return 0;
}

static void dump_avptype(struct t_avptype * t) {
	fd_log_debug("    data type: %s", t->type_name);
}

static void del_avptype_contents(struct t_avptype * t) {
	TRACE_ENTRY("%p", t);
	free(t->type_name);
}
	

/* ENUM */
struct t_enum {
	struct fd_list  chain;  /* link in t_avp->enums */
	uint32_t	code;
	uint8_t *	name;
};

static int new_enum(struct fd_list * parent, xmlChar * xmlcode, xmlChar * xmlname) {
	struct t_enum * new;
	uint32_t code = 0;
	
	TRACE_ENTRY("%p %p %p", parent, xmlcode, xmlname);
	CHECK_PARAMS( parent && xmlcode && xmlname );
	
	CHECK_FCT( xmltoint(xmlcode, &code) );
	
	CHECK_MALLOC( new = malloc(sizeof(struct t_enum)) );
	memset(new, 0, sizeof(struct t_enum));
	fd_list_init(&new->chain, NULL);
	new->code = code;
	CHECK_MALLOC( new->name = (uint8_t *)strdup((char *)xmlname) );
	
	fd_list_insert_before(parent, &new->chain);
	
	return 0;
}

static void dump_enum(struct t_enum * e) {
	fd_log_debug("    Value: %d == %s", e->code, e->name);
}	

static void del_enum_contents(struct t_enum * e) {
	TRACE_ENTRY("%p", e);
	free(e->name);
}

/* AVP */
struct t_avp {
	struct fd_list  chain;  /* link in t_appl->avps */
	uint32_t	code;
	uint8_t *	name;
	uint8_t		flags;
	uint8_t		fmask;
	uint32_t	vendor;
	struct fd_list  type;             /* list of t_avptype -- there must be at max 1 item in the list */
	struct fd_list  enums;            /* list of t_enum */
	struct fd_list  grouped_fixed;    /* list of t_rule */
	struct fd_list  grouped_required; /* list of t_rule */
	struct fd_list  grouped_optional; /* list of t_rule */
};

static int new_avp(struct fd_list * parent, xmlChar * xmlcode, xmlChar * xmlname, xmlChar * xmlmandatory, xmlChar * xmlvendor, struct t_avp **ret) {
	/* we ignore description, may-encrypt, protected, ... */
	struct t_avp * new;
	uint32_t code;
	uint32_t vendor = 0;
	uint32_t flag = 0;
	uint32_t fmask = 0;
	
	TRACE_ENTRY("%p %p %p %p %p", parent, xmlcode, xmlname, xmlmandatory, xmlvendor);
	CHECK_PARAMS( parent && xmlcode && xmlname );
	
	CHECK_FCT( xmltoint(xmlcode, &code) );
	
	if (xmlmandatory && !strcasecmp((char *)xmlmandatory, "must")) {
		flag |= AVP_FLAG_MANDATORY;
		fmask |= AVP_FLAG_MANDATORY;
	}
	
	if (xmlvendor) {
		CHECK_FCT( xmltoint(xmlvendor, &vendor) );
		if (vendor)
			flag |= AVP_FLAG_VENDOR;
		fmask |= AVP_FLAG_VENDOR;
	}
	
	CHECK_MALLOC( new = malloc(sizeof(struct t_avp)) );
	memset(new, 0, sizeof(struct t_avp));
	fd_list_init(&new->chain, NULL);
	new->code = code;
	CHECK_MALLOC( new->name = (uint8_t *)strdup((char *)xmlname) );
	new->flags = flag;
	new->fmask = fmask;
	new->vendor= vendor; 
	fd_list_init(&new->type, NULL);
	fd_list_init(&new->enums, NULL);
	fd_list_init(&new->grouped_fixed, NULL);
	fd_list_init(&new->grouped_required, NULL);
	fd_list_init(&new->grouped_optional, NULL);
	
	fd_list_insert_before(parent, &new->chain);
	
	*ret = new;
	
	return 0;
}

static void dump_avp(struct t_avp * a) {
	struct fd_list * li;
	fd_log_debug("  AVP %d %s%s: %s", a->code, 
		a->fmask & AVP_FLAG_MANDATORY ? ( a->flags & AVP_FLAG_MANDATORY ? "[M=1]" : "[M=0]") : "", 
		a->fmask & AVP_FLAG_VENDOR ? ( a->flags & AVP_FLAG_VENDOR ? "[V=1]" : "[V=0]") : "", 
		a->name);
	if (a->fmask & AVP_FLAG_VENDOR)
		fd_log_debug("    vendor: %d", a->vendor);
	for (li = a->type.next; li != &a->type; li = li->next)
		dump_avptype((struct t_avptype *)li);
	for (li = a->enums.next; li != &a->enums; li = li->next)
		dump_enum((struct t_enum *)li);
	for (li = a->grouped_fixed.next; li != &a->grouped_fixed; li = li->next)
		dump_rule((struct t_rule *)li, "    Grouped, fixed    AVP:");
	for (li = a->grouped_required.next; li != &a->grouped_required; li = li->next)
		dump_rule((struct t_rule *)li, "    Grouped, required AVP:");
	for (li = a->grouped_optional.next; li != &a->grouped_optional; li = li->next)
		dump_rule((struct t_rule *)li, "    Grouped, optional AVP:");
}

static void del_avp_contents(struct t_avp * a) {
	TRACE_ENTRY("%p", a);
	free(a->name);
	while (!FD_IS_LIST_EMPTY(&a->type)) {
		struct fd_list * li = a->type.next;
		fd_list_unlink(li);
		del_avptype_contents((struct t_avptype *)li);
		free(li);
	}
	while (!FD_IS_LIST_EMPTY(&a->enums)) {
		struct fd_list * li = a->enums.next;
		fd_list_unlink(li);
		del_enum_contents((struct t_enum *)li);
		free(li);
	}
	while (!FD_IS_LIST_EMPTY(&a->grouped_fixed)) {
		struct fd_list * li = a->grouped_fixed.next;
		fd_list_unlink(li);
		del_rule_contents((struct t_rule *)li);
		free(li);
	}
	while (!FD_IS_LIST_EMPTY(&a->grouped_required)) {
		struct fd_list * li = a->grouped_required.next;
		fd_list_unlink(li);
		del_rule_contents((struct t_rule *)li);
		free(li);
	}
	while (!FD_IS_LIST_EMPTY(&a->grouped_optional)) {
		struct fd_list * li = a->grouped_optional.next;
		fd_list_unlink(li);
		del_rule_contents((struct t_rule *)li);
		free(li);
	}
}


/* APPLICATION */
struct t_appl {
	struct fd_list 	chain; /* link in the t_dictionary->base_and_applications, the sentinel corresponds to "base" */
	uint32_t	id;
	uint8_t *	name;
	struct fd_list  commands; /* list of t_cmd */
	struct fd_list	types;    /* list of t_typedefn */
	struct fd_list  avps;     /* list of t_avp */
};

static int new_appl(struct fd_list * parent, xmlChar * xmlid, xmlChar * xmlname /* We ignore the URI */, struct t_appl **ret) {
	struct t_appl * new;
	uint32_t id = 0;
	
	TRACE_ENTRY("%p %p %p", parent, xmlid, xmlname);
	CHECK_PARAMS( parent && xmlid && xmlname );
	
	CHECK_FCT( xmltoint(xmlid, &id) );
	
	CHECK_MALLOC( new = malloc(sizeof(struct t_appl)) );
	memset(new, 0, sizeof(struct t_appl));
	fd_list_init(&new->chain, NULL);
	new->id = id;
	CHECK_MALLOC( new->name = (uint8_t *)strdup((char *)xmlname) );
	
	fd_list_init(&new->commands, NULL);
	fd_list_init(&new->types, NULL);
	fd_list_init(&new->avps, NULL);
	
	fd_list_insert_before(parent, &new->chain);
	
	*ret = new;
	
	return 0;
}

static void dump_appl(struct t_appl * a) {
	struct fd_list * li;
	fd_log_debug(" Application %d: %s", a->id, a->name);
	for (li = a->commands.next; li != &a->commands; li = li->next)
		dump_cmd((struct t_cmd *)li);
	for (li = a->types.next; li != &a->types; li = li->next)
		dump_type((struct t_typedefn *)li);
	for (li = a->avps.next; li != &a->avps; li = li->next)
		dump_avp((struct t_avp *)li);
}

static void del_appl_contents(struct t_appl * a) {
	TRACE_ENTRY("%p", a);
	free(a->name);
	while (!FD_IS_LIST_EMPTY(&a->commands)) {
		struct fd_list * li = a->commands.next;
		fd_list_unlink(li);
		del_cmd_contents((struct t_cmd *)li);
		free(li);
	}
	while (!FD_IS_LIST_EMPTY(&a->types)) {
		struct fd_list * li = a->types.next;
		fd_list_unlink(li);
		del_type_contents((struct t_typedefn *)li);
		free(li);
	}
	while (!FD_IS_LIST_EMPTY(&a->avps)) {
		struct fd_list * li = a->avps.next;
		fd_list_unlink(li);
		del_avp_contents((struct t_avp *)li);
		free(li);
	}
}

/* DICTIONARY */	
struct t_dictionary {
	struct fd_list  vendors;
	struct t_appl   base_and_applications;
};

static void dump_dict(struct t_dictionary * d) {
	struct fd_list * li;
	for (li = d->vendors.next; li != &d->vendors; li = li->next)
		dump_vendor((struct t_vend *)li);
	dump_appl(&d->base_and_applications);
	for (li = d->base_and_applications.chain.next; li != &d->base_and_applications.chain; li = li->next)
		dump_appl((struct t_appl *)li);
}

static void del_dict_contents(struct t_dictionary * d) {
	TRACE_ENTRY("%p", d);
	while (!FD_IS_LIST_EMPTY(&d->vendors)) {
		struct fd_list * li = d->vendors.next;
		fd_list_unlink(li);
		del_vendor_contents((struct t_vend *)li);
		free(li);
	}
	while (!FD_IS_LIST_EMPTY(&d->base_and_applications.chain)) {
		struct fd_list * li = d->base_and_applications.chain.next;
		fd_list_unlink(li);
		del_appl_contents((struct t_appl *)li);
		free(li);
	}
	d->base_and_applications.name = NULL;
	del_appl_contents(&d->base_and_applications);
}

/*********************************************/
	
/* The states for the SAX parser, corresponding roughly to the expected structure of the XML file. 
We use the states mostly to validate the XML file. */
enum state {
	INIT = 0,
	START, /* In "dictionary" */
	 IN_VENDOR,
	 IN_APPLICATION,        /* note that "base" is equivalent to "application" for our state machine */
	  IN_COMMAND,
	   IN_REQRULES,
	    IN_REQRULES_FIXED,
	    IN_REQRULES_REQUIRED,
	    IN_REQRULES_OPTIONAL,
	   IN_ANSRULES,
	    IN_ANSRULES_FIXED,
	    IN_ANSRULES_REQUIRED,
	    IN_ANSRULES_OPTIONAL,
	  IN_TYPEDEFN,
	  IN_AVP,
	   IN_AVP_TYPE,
	   IN_AVP_ENUM,
	   IN_AVP_GROUPED,
	    IN_AVP_GROUPED_FIXED,
	    IN_AVP_GROUPED_REQUIRED,
	    IN_AVP_GROUPED_OPTIONAL
};


/* The context passed to the SAX parser */
struct parser_ctx {
	enum state 		state; 	     /* the current state */
	int        		error_depth; /* if non 0, we are in an unexpected element, wait until the count goes back to 0 to resume normal parsing. */
	struct t_dictionary 	dict;        /* The dictionary being built */
	struct t_appl *		cur_app;
	struct t_cmd  *		cur_cmd;
	struct t_avp  *		cur_avp;
	char * 			xmlfilename; /* Name of the file, for error messages */
};

/* Find an attribute with given name in the list */
static void get_attr(const xmlChar ** atts_array, const char * attr_name, xmlChar ** attr_val) {
	int i;
	*attr_val = NULL;
	if (atts_array == NULL)
		return;
	for (i=0; atts_array[i] != NULL; i+=2) {
		if (!strcasecmp((char *)atts_array[i], attr_name)) {
			/* found */
			*attr_val = (xmlChar *)atts_array[i+1];
			return;
		}
	}
	/* not found */
	return;
}

/* The following macro avoids duplicating a lot of code in the state machine */
#define ADD_RULE( _parent_list ) { 				\
	xmlChar *xname, *xmin, *xmax;				\
	/* We are expecting an <avprule> tag at this point */	\
	if (strcasecmp((char *)name, "avprule"))		\
		goto xml_tree_error;				\
	/* Search the expected attributes */			\
	get_attr(atts, "name", &xname);				\
	get_attr(atts, "maximum", &xmax);			\
	get_attr(atts, "minimum", &xmin);			\
	/* Check the mandatory name is here */			\
	CHECK_PARAMS_DO(xname, 					\
		{ TRACE_DEBUG(INFO, "Invalid 'avprule' tag found without 'name' attribute."); goto xml_tree_error; } );	\
	/* Create the rule and add into the parent list */	\
	CHECK_FCT_DO( new_rule((_parent_list), xname, xmax, xmin),\
		{ TRACE_DEBUG(INFO, "An error occurred while parsing an avprule tag. Entry ignored."); goto xml_tree_error; } ); \
	/* Done. we don't change the state */			\
}
	
	
/* The function called on each XML element start tag (startElementSAXFunc) */
static void SAXstartelem (void * ctx, const xmlChar * name, const xmlChar ** atts)
{
	struct parser_ctx * data = ctx;
	TRACE_ENTRY("%p %p %p", ctx, name, atts);
	CHECK_PARAMS_DO( ctx && name, { return; } );
	
	TRACE_DEBUG(CALL, "Tag: <%s>", (char *)name);
	
	if (data->error_depth) /* we are in an unknown element, just skip until it is closed */
		goto xml_tree_error;
	
	switch (data->state) {
		case INIT: /* we are just starting. We only expect a <dictionary> tag, reject anything else. */
			if (strcasecmp((char *)name, "dictionary"))
				goto xml_tree_error;
			
			data->state = START;
			break;
			
		case START: 
			/* We are in <dictionary> 
				Valid tags are: <vendor>, <base>, <application> */
			if (!strcasecmp((char *)name, "vendor")) {
				xmlChar *xid, *xname;
				
				get_attr(atts, "id", &xid);
				get_attr(atts, "name", &xname);
				
				/* id and name are required */
				CHECK_PARAMS_DO(xid && xname, 
					{ TRACE_DEBUG(INFO, "Invalid 'vendor' tag found without 'id' or 'name' attribute."); goto xml_tree_error; } );
				
				
				CHECK_FCT_DO( new_vendor(&data->dict.vendors, xid, xname),
					{ TRACE_DEBUG(INFO, "An error occurred while parsing a vendor tag. Entry ignored."); goto xml_tree_error; } )
				
				data->state = IN_VENDOR;
				break;
			}
			
			if (!strcasecmp((char *)name, "base")) {
				/* we don't care for the 'uri' attribute */
				data->cur_app = &data->dict.base_and_applications;
				data->state = IN_APPLICATION;
				break;
			}
			
			if (!strcasecmp((char *)name, "application")) {
				/* we don't care for the 'uri' attribute */
				xmlChar *xid, *xname;
				char buf[50];
				
				get_attr(atts, "id", &xid);
				get_attr(atts, "name", &xname);
				
				CHECK_PARAMS_DO(xid, 
					{ TRACE_DEBUG(INFO, "Invalid 'application' tag found without 'id' attribute."); goto xml_tree_error; } );
				
				/* Name is optional, if not provided we create a name */
				if (!xname) {
					snprintf(buf, sizeof(buf), "Application %s", xid);
					xname = (xmlChar *)buf;
				}
				
				CHECK_FCT_DO( new_appl(&data->dict.base_and_applications.chain, xid, xname, &data->cur_app),
					{ TRACE_DEBUG(INFO, "An error occurred while parsing an application tag. Entry ignored."); goto xml_tree_error; } )
				
				data->state = IN_APPLICATION;
				break;
			}
			
			/* Other tags are errors */
			goto xml_tree_error;
			
			
		case IN_VENDOR: /* nothing is allowed inside <vendor> */
			goto xml_tree_error;
				
		case IN_APPLICATION: 
			/* We are in <base> or <application>
				Valid tags are: <command>, <typedefn>, <avp> */
			if (!strcasecmp((char *)name, "command")) {
				/* we don't care for the 'vendor-id' attribute. */
				xmlChar *xcode, *xname, *xpbit;
				
				get_attr(atts, "code", &xcode);
				get_attr(atts, "name", &xname);
				get_attr(atts, "pbit", &xpbit);
				
				/* code and name are required */
				CHECK_PARAMS_DO(xcode && xname, 
					{ TRACE_DEBUG(INFO, "Invalid 'command' tag found without 'code' or 'name' attribute."); goto xml_tree_error; } );
				
				CHECK_FCT_DO( new_cmd( &data->cur_app->commands, xcode, xname, xpbit, &data->cur_cmd),
					{ TRACE_DEBUG(INFO, "An error occurred while parsing a command tag. Entry ignored."); goto xml_tree_error; } )
				
				data->state = IN_COMMAND;
				break;
			}
			
			if (!strcasecmp((char *)name, "typedefn")) {
				/* we don't care for the 'description' attribute. */
				xmlChar *xname, *xparent;
				
				get_attr(atts, "type-name", &xname);
				get_attr(atts, "type-parent", &xparent);
				
				/* name is required */
				CHECK_PARAMS_DO(xname, 
					{ TRACE_DEBUG(INFO, "Invalid 'typedefn' tag found without 'name' attribute."); goto xml_tree_error; } );
				
				CHECK_FCT_DO( new_type( &data->cur_app->types, xname, xparent),
					{ TRACE_DEBUG(INFO, "An error occurred while parsing a typedefn tag. Entry ignored."); goto xml_tree_error; } )
				
				data->state = IN_TYPEDEFN;
				break;
			}
			
			if (!strcasecmp((char *)name, "avp")) {
				/* we don't care for the description, may-encrypt, and protected attributes */
				xmlChar *xname, *xcode, *xmandatory, *xvendor;
				
				get_attr(atts, "name", &xname);
				get_attr(atts, "code", &xcode);
				get_attr(atts, "mandatory", &xmandatory);
				get_attr(atts, "vendor-id", &xvendor);
				
				/* code and name are required */
				CHECK_PARAMS_DO(xcode && xname, 
					{ TRACE_DEBUG(INFO, "Invalid 'avp' tag found without 'code' or 'name' attribute."); goto xml_tree_error; } );
				
				CHECK_FCT_DO( new_avp(&data->cur_app->avps, xcode, xname, xmandatory, xvendor, &data->cur_avp),
					{ TRACE_DEBUG(INFO, "An error occurred while parsing an avp tag. Entry ignored."); goto xml_tree_error; } )
				
				data->state = IN_AVP;
				break;
			}
			/* Other tags are errors */
			goto xml_tree_error;
		
			
		case IN_COMMAND: 
			/* We are in <command>
				Valid tags are: <requestrules>, <answerrules> */
			if (!strcasecmp((char *)name, "requestrules")) {
				data->state = IN_REQRULES;
				break;
			}
			if (!strcasecmp((char *)name, "answerrules")) {
				data->state = IN_ANSRULES;
				break;
			}
			/* Other tags are errors */
			goto xml_tree_error;
		
		case IN_REQRULES: 
			/* We are in <requestrules>
				Valid tags are: <fixed>, <required>, <optional> */
			if (!strcasecmp((char *)name, "fixed")) {
				data->state = IN_REQRULES_FIXED;
				break;
			}
			if (!strcasecmp((char *)name, "required")) {
				data->state = IN_REQRULES_REQUIRED;
				break;
			}
			if (!strcasecmp((char *)name, "optional")) {
				data->state = IN_REQRULES_OPTIONAL;
				break;
			}
			/* Other tags are errors */
			goto xml_tree_error;
		
		case IN_ANSRULES: 
			/* We are in <answerrules>
				Valid tags are: <fixed>, <required>, <optional> */
			if (!strcasecmp((char *)name, "fixed")) {
				data->state = IN_ANSRULES_FIXED;
				break;
			}
			if (!strcasecmp((char *)name, "required")) {
				data->state = IN_ANSRULES_REQUIRED;
				break;
			}
			if (!strcasecmp((char *)name, "optional")) {
				data->state = IN_ANSRULES_OPTIONAL;
				break;
			}
			/* Other tags are errors */
			goto xml_tree_error;
			
		case IN_REQRULES_FIXED:
			/* We are in <command><answerrules><fixed>
				Valid tags are: <avprule> */
			ADD_RULE( &data->cur_cmd->reqrules_fixed );
			break;
		case IN_REQRULES_REQUIRED:
			ADD_RULE( &data->cur_cmd->reqrules_required );
			break;
		case IN_REQRULES_OPTIONAL:
			ADD_RULE( &data->cur_cmd->reqrules_optional );
			break;
		case IN_ANSRULES_FIXED:
			ADD_RULE( &data->cur_cmd->ansrules_fixed );
			break;
		case IN_ANSRULES_REQUIRED:
			ADD_RULE( &data->cur_cmd->ansrules_required );
			break;
		case IN_ANSRULES_OPTIONAL:
			ADD_RULE( &data->cur_cmd->ansrules_optional );
			break;
			
		
		case IN_TYPEDEFN: /* nothing is allowed inside <typedefn> */
			goto xml_tree_error;
				
		
		case IN_AVP: 
			/* We are in <avp>
				Valid tags are: <type>, <enum>, <grouped> */
			if (!strcasecmp((char *)name, "type")) {
				xmlChar *xname;
				
				get_attr(atts, "type-name", &xname);
				
				/* name is required */
				CHECK_PARAMS_DO(xname, 
					{ TRACE_DEBUG(INFO, "Invalid 'type' tag found without 'name' attribute."); goto xml_tree_error; } );
				
				/* Check there is only 1 type */
				if (!FD_IS_LIST_EMPTY(&data->cur_avp->type)) {
					TRACE_DEBUG(INFO, "Multiple 'type' tags found for AVP.");
					goto xml_tree_error;
				}
				
				/* Add the new type */
				CHECK_FCT_DO( new_avptype(&data->cur_avp->type, xname),
					{ TRACE_DEBUG(INFO, "An error occurred while parsing a type tag. Entry ignored."); goto xml_tree_error; } )
				
				data->state = IN_AVP_TYPE;
				break;
			}
			if (!strcasecmp((char *)name, "enum")) {
				xmlChar *xcode, *xname;
				
				get_attr(atts, "code", &xcode);
				get_attr(atts, "name", &xname);
				
				/* code and name are required */
				CHECK_PARAMS_DO(xcode && xname, 
					{ TRACE_DEBUG(INFO, "Invalid 'enum' tag found without 'code' or 'name' attribute."); goto xml_tree_error; } );
				
				CHECK_FCT_DO( new_enum(&data->cur_avp->enums, xcode, xname),
					{ TRACE_DEBUG(INFO, "An error occurred while parsing a command tag. Entry ignored."); goto xml_tree_error; } )
			
				data->state = IN_AVP_ENUM;
				break;
			}
			if (!strcasecmp((char *)name, "grouped")) {
				/* no attribute for this one */
				data->state = IN_AVP_GROUPED;
				break;
			}
			/* Other tags are errors */
			goto xml_tree_error;
		
		case IN_AVP_TYPE: /* nothing is allowed inside <type> */
			goto xml_tree_error;
			
		case IN_AVP_ENUM: /* nothing is allowed inside <enum> */
			goto xml_tree_error;
			
		case IN_AVP_GROUPED: 
			/* We are in <avp><grouped>
				Valid tags are: <fixed>, <required>, <optional> */
			if (!strcasecmp((char *)name, "fixed")) {
				data->state = IN_AVP_GROUPED_FIXED;
				break;
			}
			if (!strcasecmp((char *)name, "required")) {
				data->state = IN_AVP_GROUPED_REQUIRED;
				break;
			}
			if (!strcasecmp((char *)name, "optional")) {
				data->state = IN_AVP_GROUPED_OPTIONAL;
				break;
			}
			/* Other tags are errors */
			goto xml_tree_error;
			
		case IN_AVP_GROUPED_FIXED:
			/* We are in <avp><grouped><fixed>
				Valid tags are: <avprule> */
			ADD_RULE( &data->cur_avp->grouped_fixed );
			break;
		case IN_AVP_GROUPED_REQUIRED:
			ADD_RULE( &data->cur_avp->grouped_required );
			break;
		case IN_AVP_GROUPED_OPTIONAL:
			ADD_RULE( &data->cur_avp->grouped_optional );
			break;
		
			
		default:
			TRACE_DEBUG(INFO, "Internal parsing error, unexpected state %d.", data->state);
	}
	
	return;
	
xml_tree_error:
	if (!data->error_depth) {
		TRACE_DEBUG(INFO, "Unexpected XML element found: '%s'. Ignoring...", name);
	}
	data->error_depth += 1;
	if (data->cur_app || data->cur_cmd || data->cur_avp) {
		TRACE_DEBUG(INFO, "Error encountered while parsing tag of:");
		if (data->cur_app)
			fd_log_debug("  Application: '%s'", data->cur_app->name);
		if (data->cur_cmd)
			fd_log_debug("  Command    : '%s'", data->cur_cmd->name);
		if (data->cur_avp)
			fd_log_debug("  AVP        : '%s'", data->cur_avp->name);
	}
	return;
}

/* The function called on each XML element end tag (endElementSAXFunc) */
static void SAXendelem (void * ctx, const xmlChar * name)
{
	struct parser_ctx * data = ctx;
	TRACE_ENTRY("%p %p", ctx, name);
	CHECK_PARAMS_DO( ctx && name, { return; } );
	
	TRACE_DEBUG(CALL, "Tag: </%s>", (char *)name);
	
	if (data->error_depth) {
		/* we are recovering from an erroneous element */
		data->error_depth -= 1;
		return;
	}
	
	switch (data->state) {
		case INIT: 
			goto state_machine_error;
			
		case START:
			if (strcasecmp((char *)name, "dictionary"))
				goto state_machine_error;
			
			data->state = 0;
			break;
		
		case IN_VENDOR:
			if (strcasecmp((char *)name, "vendor"))
				goto state_machine_error;
			
			data->state = START;
			break;
		
		case IN_APPLICATION:
			if (strcasecmp((char *)name, "base") && strcasecmp((char *)name, "application"))
				goto state_machine_error;
			
			data->cur_app = NULL;
			data->state = START;
			break;
		
		case IN_COMMAND:
			if (strcasecmp((char *)name, "command"))
				goto state_machine_error;
			
			data->cur_cmd = NULL;
			data->state = IN_APPLICATION;
			break;
			
		case IN_REQRULES:
			if (strcasecmp((char *)name, "requestrules"))
				goto state_machine_error;
			
			data->state = IN_COMMAND;
			break;
		
		case IN_REQRULES_FIXED:
			if (!strcasecmp((char *)name, "avprule"))
				/* we don't have a special state for these, just ignore */
				return;
			if (strcasecmp((char *)name, "fixed"))
				goto state_machine_error;
			data->state = IN_REQRULES;
			break;
		case IN_REQRULES_REQUIRED:
			if (!strcasecmp((char *)name, "avprule"))
				/* we don't have a special state for these, just ignore */
				return;
			if (strcasecmp((char *)name, "required"))
				goto state_machine_error;
			data->state = IN_REQRULES;
			break;
		case IN_REQRULES_OPTIONAL:
			if (!strcasecmp((char *)name, "avprule"))
				/* we don't have a special state for these, just ignore */
				return;
			if (strcasecmp((char *)name, "optional"))
				goto state_machine_error;
			data->state = IN_REQRULES;
			break;
			
		case IN_ANSRULES: 
			if (strcasecmp((char *)name, "answerrules"))
				goto state_machine_error;
			
			data->state = IN_COMMAND;
			break;			
		case IN_ANSRULES_FIXED:
			if (!strcasecmp((char *)name, "avprule"))
				/* we don't have a special state for these, just ignore */
				return;
			if (strcasecmp((char *)name, "fixed"))
				goto state_machine_error;
			data->state = IN_ANSRULES;
			break;
		case IN_ANSRULES_REQUIRED:
			if (!strcasecmp((char *)name, "avprule"))
				/* we don't have a special state for these, just ignore */
				return;
			if (strcasecmp((char *)name, "required"))
				goto state_machine_error;
			data->state = IN_ANSRULES;
			break;
		case IN_ANSRULES_OPTIONAL:
			if (!strcasecmp((char *)name, "avprule"))
				/* we don't have a special state for these, just ignore */
				return;
			if (strcasecmp((char *)name, "optional"))
				goto state_machine_error;
			data->state = IN_ANSRULES;
			break;
		
		
		case IN_TYPEDEFN:
			if (strcasecmp((char *)name, "typedefn"))
				goto state_machine_error;
			
			data->state = IN_APPLICATION;
			break;
		
		case IN_AVP: 
			if (strcasecmp((char *)name, "avp"))
				goto state_machine_error;
			
			data->cur_avp = NULL;
			data->state = IN_APPLICATION;
			break;
		
		case IN_AVP_TYPE:
			if (strcasecmp((char *)name, "type"))
				goto state_machine_error;
			
			data->state = IN_AVP;
			break;
		
		case IN_AVP_ENUM:
			if (strcasecmp((char *)name, "enum"))
				goto state_machine_error;
			
			data->state = IN_AVP;
			break;
		
		case IN_AVP_GROUPED:
			if (strcasecmp((char *)name, "grouped"))
				goto state_machine_error;
			
			data->state = IN_AVP;
			break;
		
		case IN_AVP_GROUPED_FIXED:
			if (!strcasecmp((char *)name, "avprule"))
				/* we don't have a special state for these, just ignore */
				return;
			if (strcasecmp((char *)name, "fixed"))
				goto state_machine_error;
			data->state = IN_AVP_GROUPED;
			break;
		case IN_AVP_GROUPED_REQUIRED:
			if (!strcasecmp((char *)name, "avprule"))
				return;
			if (strcasecmp((char *)name, "required"))
				goto state_machine_error;
			data->state = IN_AVP_GROUPED;
			break;
		case IN_AVP_GROUPED_OPTIONAL:
			if (!strcasecmp((char *)name, "avprule"))
				return;
			if (strcasecmp((char *)name, "optional"))
				goto state_machine_error;
			data->state = IN_AVP_GROUPED;
			break;
			
		default:
			TRACE_DEBUG(INFO, "Internal parsing error, unexpected state %d.", data->state);
	}
	
	return;
	
state_machine_error:
	TRACE_DEBUG(INFO, "Internal parsing error, ignored [state %d, closing tag '%s'].", data->state, name);
	return;
}

/* The SAX parser sends a warning, error, fatalerror -- do we need these ?
static void SAXwarning (void * ctx, const char * msg, ...)
{

}
static void SAXerror (void * ctx, const char * msg, ...)
{

}
static void SAXfatal (void * ctx, const char * msg, ...)
{

}
*/




/*********************************************/
 /* 2nd pass: from memory to fD dictionary */
/*********************************************/

/* Find or create a vendor */
static int vend_to_fD(struct t_vend * v, struct dictionary * fD_dict, struct dict_object ** fd_v, int * nb_added)
{
	int ret;
	struct dict_object * prev = NULL;
	struct dict_vendor_data vd;

	TRACE_ENTRY("%p %p %p %p", v, fD_dict, fd_v, nb_added);
	
	CHECK_PARAMS(v && fD_dict);
	
	/* Prepare the data in fD's format */
	memset(&vd, 0, sizeof(vd));
	vd.vendor_id   = v->id;
	vd.vendor_name = (char *)v->name;
	
	/* Create or search in the dictionary */
	ret = fd_dict_new ( fD_dict, DICT_VENDOR, &vd, NULL, &prev );
	if (fd_v)
		*fd_v = prev;
	if (ret == EEXIST) {
		/* Conflict with existing entry */
		CHECK_FCT( fd_dict_getval(prev, &vd) );
		TRACE_DEBUG(INFO, "[dict_legacy_xml] Warning: Conflicting entry.");
		TRACE_DEBUG(INFO, "[dict_legacy_xml]   New entry (ignored): %u - '%s'", v->id, (char *)v->name);
		TRACE_DEBUG(INFO, "[dict_legacy_xml]   Old entry          : %u - '%s'", vd.vendor_id, vd.vendor_name);
		return 0;
	} else {
		/* other errors are stoppers */
		CHECK_FCT(ret);
	}
	
	/* Update count */
	if (nb_added)
		*nb_added += 1;
	
	/* Done */
	return 0;
}

/* Find the base fD type from a type name */
static int resolve_base_type(struct dictionary * fD_dict, uint8_t * type_name, enum dict_avp_basetype * basetype, struct dict_object **type)
{
	int ret;
	struct dict_type_data td;
	struct dict_object *t;
	
	TRACE_ENTRY("%p, %p %p", fD_dict, type_name, basetype);
	CHECK_PARAMS( fD_dict && type_name && basetype );
	
	/* First, check if the type is already in the dictionary */
	ret = fd_dict_search ( fD_dict, DICT_TYPE, TYPE_BY_NAME, type_name, &t, ENOENT);
	switch (ret) {
		case 0: /* the type is already in the dictionary */
			CHECK_FCT( fd_dict_getval(t, &td) );
			*basetype = td.type_base;
			if (type)
				*type = t;
			return 0;
		
		case ENOENT: /* We did not find it, it is maybe normal */
			break;
			
		default:
			/* An unexpected error occurred */
			CHECK_FCT(ret);
	}
	
	/* at this point we did not find the type in the dictionary */
#define PREDEF_TYPES( _typename_, _basetype_ )			\
	if (!strcasecmp((char *)type_name, (_typename_))) {	\
		*basetype = (_basetype_);			\
		return 0;					\
	}
	
	PREDEF_TYPES( "OctetString", AVP_TYPE_OCTETSTRING );
	PREDEF_TYPES( "Integer32",   AVP_TYPE_INTEGER32   );
	PREDEF_TYPES( "Integer64",   AVP_TYPE_INTEGER64   );
	PREDEF_TYPES( "Unsigned32",  AVP_TYPE_UNSIGNED32  );
	PREDEF_TYPES( "Enumerated",  AVP_TYPE_UNSIGNED32  );
	PREDEF_TYPES( "Unsigned64",  AVP_TYPE_UNSIGNED64  );
	PREDEF_TYPES( "Float32",     AVP_TYPE_FLOAT32     );
	PREDEF_TYPES( "Float64",     AVP_TYPE_FLOAT64     );
	
	/* When we reach this point, we have not yet found this type anywhere. */
	TODO("Type not found. Maybe search in whole xmldictionary if it is defined later?");
	TRACE_DEBUG(INFO, "The type '%s' could not be resolved. Please check it is defined before use.", type_name);
	return ENOENT;
}

/* Find or create a type. */
static int typdefn_to_fD(struct t_typedefn * t, struct dictionary * fD_dict, struct dict_object * fd_appl, struct dict_object ** fd_t, int * nb_added)
{
	int ret;
	struct dict_object * prev = NULL;
	struct dict_type_data td;

	TRACE_ENTRY("%p %p %p %p %p", t, fD_dict, fd_appl, fd_t, nb_added);
	
	CHECK_PARAMS(t && fD_dict);
	
	/* Prepare the data in fD's format */
	memset(&td, 0, sizeof(td));
	td.type_name = (char *)t->name;
	
	/* infer td.type_base from t->parent_name */
	CHECK_FCT( resolve_base_type(fD_dict, t->parent_name, &td.type_base, NULL) );
	
	/* Create or search in the dictionary */
	ret = fd_dict_new ( fD_dict, DICT_TYPE, &td, fd_appl, &prev );
	if (fd_t)
		*fd_t = prev;
	if (ret == EEXIST) {
		/* Conflict with existing entry */
		enum dict_avp_basetype xmlbt = td.type_base;
		extern const char * type_base_name[]; /* in libfreeDiameter/dictionary.c */
		CHECK_FCT( fd_dict_getval(prev, &td) );
		TRACE_DEBUG(INFO, "[dict_legacy_xml] Warning: Conflicting entry.");
		TRACE_DEBUG(INFO, "[dict_legacy_xml]   New entry (ignored): '%s' (%d - %s)", t->name, xmlbt, type_base_name[xmlbt] );
		TRACE_DEBUG(INFO, "[dict_legacy_xml]   Old entry          : '%s' (%d - %s)", td.type_name, td.type_base, type_base_name[td.type_base]);
		return 0;
	} else {
		/* other errors are stoppers */
		CHECK_FCT(ret);
	}
	
	/* Update count */
	if (nb_added)
		*nb_added += 1;
	
	/* Done */
	return 0;
}

/* Process one list of rules */
static int rules_to_fD_onelist(struct dictionary * fD_dict, struct dict_object * parent, enum rule_position position, struct fd_list * list, int * nb_added)
{
	struct dict_rule_data rd;
	struct fd_list * li;
	int order = 0;
	int ret;

	TRACE_ENTRY("%p %p %d %p %p", fD_dict, parent, position, list, nb_added);
	
	CHECK_PARAMS(fD_dict && parent && position && list);
	
	for (li = list->next; li != list; li = li->next) {
		struct t_rule * r = (struct t_rule *)li;
		
		/* The [AVP] rule in all ABNF definitions is implicit in freeDiameter, skip it */
		if (!strcmp((char *)r->avpname, "AVP"))
			continue;
		
		/* Prepare rule data */
		memset(&rd, 0, sizeof(rd));
		rd.rule_position = position;
		rd.rule_order = ++order; /* actually only used for fixed rules, but no harm for others */
		rd.rule_min = r->min;
		rd.rule_max = r->max;
		
		/* Resolve the AVP */
		ret = fd_dict_search(fD_dict, DICT_AVP, AVP_BY_NAME_ALL_VENDORS, r->avpname, &rd.rule_avp, ENOENT);
		if (ret == ENOENT) {
			TRACE_DEBUG(INFO, "[dict_legacy_xml] Error: AVP '%s' used in a rule before being defined.", r->avpname);
		}
		CHECK_FCT(ret);
		
		/* Now create the new rule */
		CHECK_FCT_DO( ret = fd_dict_new ( fD_dict, DICT_RULE, &rd, parent, NULL ),
			{ TRACE_DEBUG(INFO, "Error creating rule for sub-AVP '%s'", r->avpname); return ret; } );
		if (nb_added)
			*nb_added += 1;
	}
	
	return 0;
}

/* Process lists of rules */
static int rules_to_fD(struct dictionary * fD_dict, struct dict_object * parent, struct fd_list * fixed, struct fd_list * required, struct fd_list * optional, int * nb_added)
{
	int ret;
	
	TRACE_ENTRY("%p %p %p %p %p %p", fD_dict, parent, fixed, required, optional, nb_added);
	
	/* Process the rules */
	CHECK_FCT_DO( ret = rules_to_fD_onelist(fD_dict, parent, RULE_FIXED_HEAD, fixed, nb_added),
		{ TRACE_DEBUG(INFO, "Error processing FIXED rules"); return ret; } );
	CHECK_FCT_DO( ret = rules_to_fD_onelist(fD_dict, parent, RULE_REQUIRED, required, nb_added),
		{ TRACE_DEBUG(INFO, "Error processing REQUIRED rules"); return ret; } );
	CHECK_FCT_DO( ret = rules_to_fD_onelist(fD_dict, parent, RULE_OPTIONAL, optional, nb_added),
		{ TRACE_DEBUG(INFO, "Error processing OPTIONAL rules"); return ret; } );
	
	return 0;
}

/* Find or create an AVP (and dependent objects) */
static int avp_to_fD(struct t_avp * a, struct dictionary * fD_dict, struct dict_object * fd_appl, struct dict_object ** fd_a, int * nb_added)
{
	int ret;
	struct dict_object * prev = NULL, *type = NULL;
	struct dict_avp_data ad;
	struct fd_list * li;

	TRACE_ENTRY("%p %p %p %p %p", a, fD_dict, fd_appl, fd_a, nb_added);
	
	CHECK_PARAMS(a && fD_dict);
	
	/* Prepare the data in fD's format */
	memset(&ad, 0, sizeof(ad));
	ad.avp_code   = a->code;
	ad.avp_vendor = a->vendor;
	ad.avp_name   = (char *)a->name;
	ad.avp_flag_mask = a->fmask | AVP_FLAG_VENDOR;
	ad.avp_flag_val  = a->flags;
	
	if (!FD_IS_LIST_EMPTY(&a->type)) {
		/* special exception: we use per-AVP enumerated types in fD */
		if (!strcasecmp("Enumerated", (char *)((struct t_avptype *)a->type.next)->type_name))
			goto enumerated;
		/* Let's allow "Unsigned32" instead of "Enumerated" also... */
		if ((!FD_IS_LIST_EMPTY(&a->enums)) && (!strcasecmp("Unsigned32", (char *)((struct t_avptype *)a->type.next)->type_name)))
			goto enumerated;
		
		/* The type was explicitly specified, resolve it */
		CHECK_FCT( resolve_base_type(fD_dict, ((struct t_avptype *)a->type.next)->type_name, &ad.avp_basetype, &type) );
	} else {
		/* The type was not specified, try to infer it from provided data */
		if (       !FD_IS_LIST_EMPTY(&a->grouped_optional)
			|| !FD_IS_LIST_EMPTY(&a->grouped_required)
			|| !FD_IS_LIST_EMPTY(&a->grouped_fixed) ) {
			/* The AVP has rules, it is a grouped AVP */
			CHECK_PARAMS_DO( FD_IS_LIST_EMPTY(&a->enums), 
				{ TRACE_DEBUG(INFO, "Conflict: The AVP '%s' has both enum values and rules.", ad.avp_name); return EINVAL; } );
			ad.avp_basetype = AVP_TYPE_GROUPED;
		} else {
			/* It should be an enumerated AVP... */
			if (FD_IS_LIST_EMPTY(&a->enums)) {
				TRACE_DEBUG(INFO, "Error: Missing type information for AVP '%s'", ad.avp_name);
				return EINVAL;
			} else {
				/* We create a new type to hold the enumerated values -- fD specifics */
				char typename[256];
				struct dict_type_data 	tdata;
				
enumerated:
				snprintf(typename, sizeof(typename), "Enumerated*(%s)", ad.avp_name);
				memset(&tdata, 0, sizeof(tdata));
				tdata.type_base = AVP_TYPE_UNSIGNED32;
				tdata.type_name = &typename[0];
				CHECK_FCT( fd_dict_new ( fD_dict, DICT_TYPE, &tdata, fd_appl, &type ) );
				if (nb_added)
					*nb_added += 1;
				
				ad.avp_basetype = AVP_TYPE_UNSIGNED32;
			}
		}
	}
	
	/* At this point, ad.avp_basetype is defined and type might also be */
	
	/* Create or search in the dictionary */
	ret = fd_dict_new ( fD_dict, DICT_AVP, &ad, type, &prev );
	if (fd_a)
		*fd_a = prev;
	if (ret == EEXIST) {
		/* Conflict with existing entry */
		CHECK_FCT( fd_dict_getval(prev, &ad) );
		TRACE_DEBUG(INFO, "[dict_legacy_xml] Warning: Conflicting entry.");
		TRACE_DEBUG(INFO, "[dict_legacy_xml]   New entry (ignored): %u - '%s'", a->code, (char *)a->name);
		TRACE_DEBUG(INFO, "[dict_legacy_xml]   Old entry          : %u - '%s'", ad.avp_code, ad.avp_name);
		goto inside;
	} else {
		/* other errors are stoppers */
		CHECK_FCT(ret);
	}
	
	/* Update count */
	if (nb_added)
		*nb_added += 1;
	
inside:
	/* Now, the inner elements, if any */
	
	if ( (!FD_IS_LIST_EMPTY(&a->enums)) && (ad.avp_basetype != AVP_TYPE_UNSIGNED32)) {
		TRACE_DEBUG(INFO, "AVP '%s' type is not an Unsigned32 but it has enum values (invalid in this extension).", ad.avp_name);
		return EINVAL;
	}

	/* In case of enumeration, define the enum values */
	for (li = a->enums.next; li != &a->enums; li = li->next) {
		struct t_enum * e = (struct t_enum *)li;
		struct dict_enumval_data ed;
		
		memset(&ed, 0, sizeof(ed));
		ed.enum_name = (char *)e->name;
		ed.enum_value.u32 = e->code;
		
		CHECK_FCT_DO( ret = fd_dict_new ( fD_dict, DICT_ENUMVAL, &ed, type, NULL ),
			{
				TRACE_DEBUG(INFO, "Error defining constant value '%s' for AVP '%s': %s", ed.enum_name, ad.avp_name, strerror(ret));
				return ret;
			} );
		if (nb_added)
			*nb_added += 1;
	}
	
	/* In case of grouped AVP, check the type is really grouped */
	if ( !FD_IS_LIST_EMPTY(&a->grouped_optional)
	  || !FD_IS_LIST_EMPTY(&a->grouped_required)
	  || !FD_IS_LIST_EMPTY(&a->grouped_fixed) ) {
		CHECK_PARAMS_DO( ad.avp_basetype == AVP_TYPE_GROUPED, 
			{ TRACE_DEBUG(INFO, "Got rules for non-grouped AVP '%s'", ad.avp_name); return EINVAL;} );
		CHECK_FCT_DO( ret = rules_to_fD(fD_dict, prev, &a->grouped_fixed, &a->grouped_required, &a->grouped_optional, nb_added),
			{ TRACE_DEBUG(INFO, "Error processing rules for AVP '%s': %s", ad.avp_name, strerror(ret)); return ret; } );
	}
	
	/* done! */
	return 0;
}

/* Find or create a command. */
static int cmd_to_fD(struct t_cmd * c, struct dictionary * fD_dict, struct dict_object * fd_appl, struct dict_object ** fd_req, int * nb_added)
{
	int ret;
	struct dict_object * req = NULL, *ans = NULL;
	struct dict_cmd_data cd;
	char cmdname[512];

	TRACE_ENTRY("%p %p %p %p %p", c, fD_dict, fd_appl, fd_req, nb_added);
	
	CHECK_PARAMS(c && fD_dict);
	
	/* Prepare the request data in fD's format */
	memset(&cd, 0, sizeof(cd));
	cd.cmd_code = c->code;
	snprintf(cmdname, sizeof(cmdname), "%s-Request", (char *)c->name);
	cd.cmd_name = &cmdname[0];
	cd.cmd_flag_mask = c->fmask | CMD_FLAG_REQUEST | CMD_FLAG_ERROR;
	cd.cmd_flag_val  = c->flags | CMD_FLAG_REQUEST;
	
	/* Create or search in the dictionary */
	ret = fd_dict_new ( fD_dict, DICT_COMMAND, &cd, fd_appl, &req );
	if (fd_req)
		*fd_req = req;
	if (ret == EEXIST) {
		struct dict_cmd_data prevcd;
		/* Conflict with existing entry */
		CHECK_FCT( fd_dict_getval(req, &prevcd) );
		TRACE_DEBUG(INFO, "[dict_legacy_xml] Warning: Conflicting entry.");
		TRACE_DEBUG(INFO, "[dict_legacy_xml]   New entry (ignored): %u - '%s'", cd.cmd_code, cd.cmd_name);
		TRACE_DEBUG(INFO, "[dict_legacy_xml]   Old entry          : %u - '%s'", prevcd.cmd_code, prevcd.cmd_name);
		goto answer;
	} else {
		/* other errors are stoppers */
		CHECK_FCT(ret);
	}
	
	/* Update count */
	if (nb_added)
		*nb_added += 1;
	
answer:
	/* update data for the answer */
	snprintf(cmdname, sizeof(cmdname), "%s-Answer", (char *)c->name);
	cd.cmd_flag_val &= ~CMD_FLAG_REQUEST;
	cd.cmd_flag_mask &= ~CMD_FLAG_ERROR;
	
	ret = fd_dict_new ( fD_dict, DICT_COMMAND, &cd, fd_appl, &ans );
	if (ret == EEXIST) {
		struct dict_cmd_data prevcd;
		/* Conflict with existing entry */
		CHECK_FCT( fd_dict_getval(ans, &prevcd) );
		TRACE_DEBUG(INFO, "[dict_legacy_xml] Warning: Conflicting entry.");
		TRACE_DEBUG(INFO, "[dict_legacy_xml]   New entry (ignored): %u - '%s'", cd.cmd_code, cd.cmd_name);
		TRACE_DEBUG(INFO, "[dict_legacy_xml]   Old entry          : %u - '%s'", prevcd.cmd_code, prevcd.cmd_name);
		goto rules;
	} else {
		/* other errors are stoppers */
		CHECK_FCT(ret);
	}
	
	/* Update count */
	if (nb_added)
		*nb_added += 1;
	
rules:
	/* Now process the rules inside the command */
	CHECK_FCT_DO( ret = rules_to_fD(fD_dict, req, &c->reqrules_fixed, &c->reqrules_required, &c->reqrules_optional, nb_added),
			{
				TRACE_DEBUG(INFO, "Error converting data from request rules: %s", strerror(ret));
				return ret;
			}   );
	CHECK_FCT_DO( ret = rules_to_fD(fD_dict, ans, &c->ansrules_fixed, &c->ansrules_required, &c->ansrules_optional, nb_added),
			{
				TRACE_DEBUG(INFO, "Error converting data from answer rules: %s", strerror(ret));
				return ret;
			}   );
	
	/* Done */
	return 0;
}

/* Find or create an application (and dependent objects) */
static int appl_to_fD(struct t_appl * a, struct dictionary * fD_dict, struct dict_object ** fd_a, int * nb_added)
{
	int ret;
	struct dict_object * prev = NULL;
	struct dict_application_data ad;
	struct fd_list * li;

	TRACE_ENTRY("%p %p %p %p", a, fD_dict, fd_a, nb_added);
	
	CHECK_PARAMS(a && fD_dict);
	
	if (a->id) { /* skip app 0 */
	
		/* Prepare the data in fD's format */
		memset(&ad, 0, sizeof(ad));
		ad.application_id   = a->id;
		ad.application_name = (char *)a->name;

		/* Create or search in the dictionary */
		ret = fd_dict_new ( fD_dict, 
					DICT_APPLICATION, 
					&ad, 
					NULL /* we don't have a parent vendor in XML files, so currently everything links to no vendor */, 
					&prev );
		if (fd_a)
			*fd_a = prev;
		if (ret == EEXIST) {
			/* Conflict with existing entry */
			CHECK_FCT( fd_dict_getval(prev, &ad) );
			TRACE_DEBUG(INFO, "[dict_legacy_xml] Warning: Conflicting entry.");
			TRACE_DEBUG(INFO, "[dict_legacy_xml]   New entry (ignored): %u - '%s'", a->id, (char *)a->name);
			TRACE_DEBUG(INFO, "[dict_legacy_xml]   Old entry          : %u - '%s'", ad.application_id, ad.application_name);
			goto inside;
		} else {
			/* other errors are stoppers */
			CHECK_FCT(ret);
		}

		/* Update count */
		if (nb_added)
			*nb_added += 1;
	}
	
inside:
	/* Now, the inner elements */
	
	/* First, define all the types */
	for (li = a->types.next; li != &a->types; li = li->next) {
		CHECK_FCT_DO( ret = typdefn_to_fD((struct t_typedefn *)li, fD_dict, prev, NULL, nb_added),
			{
				TRACE_DEBUG(INFO, "Error converting data from typedefn '%s': %s", ((struct t_typedefn *)li)->name, strerror(ret));
				return ret;
			}  );
	}
	
	/* Then, AVPs, enums, and grouped AVP rules */
	for (li = a->avps.next; li != &a->avps; li = li->next) {
		CHECK_FCT_DO( ret = avp_to_fD((struct t_avp *)li, fD_dict, prev, NULL, nb_added),
			{
				TRACE_DEBUG(INFO, "Error converting data from AVP '%s': %s", ((struct t_avp *)li)->name, strerror(ret));
				return ret;
			}  );
	}
	
	/* Finally, the commands and rules */
	for (li = a->commands.next; li != &a->commands; li = li->next) {
		CHECK_FCT_DO( ret = cmd_to_fD((struct t_cmd *)li, fD_dict, prev, NULL, nb_added),
			{
				TRACE_DEBUG(INFO, "Error converting data from command '%s': %s", ((struct t_cmd *)li)->name, strerror(ret));
				return ret;
			}  );
	}
	
	/* done! */
	return 0;
}


static int dict_to_fD(struct dictionary * fD_dict, struct t_dictionary * xmldict, int * nb_added)
{
	struct fd_list * li;
	int ret;
	
	TRACE_ENTRY("%p %p %p", fD_dict, xmldict, nb_added);
	
	CHECK_PARAMS(fD_dict && xmldict && nb_added);
	
	*nb_added = 0;
	
	/* Create all the vendors */
	for (li = xmldict->vendors.next; li != &xmldict->vendors; li = li->next) {
		CHECK_FCT_DO( ret = vend_to_fD((struct t_vend *)li, fD_dict, NULL, nb_added),
			{
				TRACE_DEBUG(INFO, "Error converting data from vendor '%s': %s", ((struct t_vend *)li)->name, strerror(ret));
				return ret;
			} );
	}
	
	/* Now, process each application */
	CHECK_FCT_DO( ret = appl_to_fD(&xmldict->base_and_applications, fD_dict, NULL, nb_added),
			{
				TRACE_DEBUG(INFO, "Error converting data from Base application: %s", strerror(ret));
				return ret;
			} );
	for (li = xmldict->base_and_applications.chain.next; li != &xmldict->base_and_applications.chain; li = li->next) {
		CHECK_FCT_DO( ret = appl_to_fD((struct t_appl *) li, fD_dict, NULL, nb_added),
			{
				TRACE_DEBUG(INFO, "Error converting data from application '%s': %s", ((struct t_appl *)li)->name, strerror(ret));
				return ret;
			}  );
	}
	
	/* Complete! */
	return 0;
}





/*********************************************/

int dict_lxml_parse(char * xmlfilename)
{
	xmlSAXHandler handler;
	struct parser_ctx data;
	int ret;

	TRACE_ENTRY("%p", xmlfilename);
	
	CHECK_PARAMS_DO(xmlfilename, { return -1; } );
	
	TRACE_DEBUG(FULL, "Parsing next XML file: %s...", xmlfilename);
	
	/* Initialize the parser */
	memset(&handler, 0, sizeof(handler));
	handler.startElement = SAXstartelem;
	handler.endElement   = SAXendelem;
	
	/* Initialize the data */
	memset(&data, 0, sizeof(data));
	fd_list_init( &data.dict.vendors, NULL );
	fd_list_init( &data.dict.base_and_applications.chain, NULL );
	data.dict.base_and_applications.name = (uint8_t *)"[Diameter Base Protocol]";
	fd_list_init( &data.dict.base_and_applications.commands, NULL );
	fd_list_init( &data.dict.base_and_applications.types, NULL );
	fd_list_init( &data.dict.base_and_applications.avps, NULL );
	data.xmlfilename = xmlfilename;
	
	/* Parse the file */
	ret = xmlSAXUserParseFile(&handler, &data, xmlfilename);
	if (ret < 0) {
		TRACE_DEBUG(INFO, "An error occurred while parsing %s, aborting.", xmlfilename);
		del_dict_contents(&data.dict);
		return -1;
	}
	
	TRACE_DEBUG(FULL, "XML file parsing, 1st pass completed.");
	if (TRACE_BOOL(ANNOYING)) {
		dump_dict(&data.dict);
	}
	
	/* Now, convert all the objects from the temporary tree into the freeDiameter dictionary */
	CHECK_FCT_DO( dict_to_fD(fd_g_config->cnf_dict, &data.dict, &ret), 
		{ 
			TRACE_DEBUG(INFO, "Error while converting data read from file '%s'", xmlfilename);
			del_dict_contents(&data.dict); 
			return -1; 
		} );
	
	TRACE_DEBUG(FULL, "Conversion from '%s' to freeDiameter internal format complete.", xmlfilename);
	
	/* Done */
	del_dict_contents(&data.dict);
	
	return ret;
}
