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

/* Do not include this directly, use dbg_interactive.i instead */

/****** DICTIONARY *********/

struct dictionary {
};

%extend dictionary {
	dictionary() {
		struct dictionary * r = NULL;
		int ret = fd_dict_init(&r);
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
			return NULL;
		}
		return r;
	}
	~dictionary() {
		struct dictionary *d = self;
		int ret = fd_dict_fini(&d);
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
		}
		return;
	}
	void dump() {
		char * buf = NULL;
		size_t len;
		printf("%s", fd_dict_dump(&buf, &len, NULL, $self));
		free(buf);
	}
	PyObject * vendors_list() {
		uint32_t *list = NULL, *li;
		PyObject * ret;
		SWIG_PYTHON_THREAD_BEGIN_BLOCK;
		ret = PyList_New(0);
		list = fd_dict_get_vendorid_list($self);
		for (li = list; *li != 0; li++) {
			PyList_Append(ret, PyInt_FromLong((long)*li));
		}
		free(list);
		SWIG_PYTHON_THREAD_END_BLOCK;
		return ret;
	}
	struct dict_object * new_obj(enum dict_object_type type, void * data, struct dict_object * parent = NULL) {
		struct dict_object * obj = NULL;
		int ret = fd_dict_new($self, type, data, parent, &obj);
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
			return NULL;
		}
		return obj;
	}
	struct dict_object * search(enum dict_object_type type, int criteria, int what_by_val) {
		struct dict_object * obj = NULL;
		int ret = fd_dict_search ( $self, type, criteria, &what_by_val, &obj, ENOENT );
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
			return NULL;
		}
		return obj;
	}
	struct dict_object * search(enum dict_object_type type, int criteria, char * what_by_string) {
		struct dict_object * obj = NULL;
		int ret = fd_dict_search ( $self, type, criteria, what_by_string, &obj, ENOENT );
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
			return NULL;
		}
		return obj;
	}
	struct dict_object * search(enum dict_object_type type, int criteria, void * what) {
		struct dict_object * obj = NULL;
		int ret = fd_dict_search ( $self, type, criteria, what, &obj, ENOENT );
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
			return NULL;
		}
		return obj;
	}
	struct dict_object * error_cmd() {
		struct dict_object * obj = NULL;
		int ret = fd_dict_get_error_cmd ( $self, &obj );
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
			return NULL;
		}
		return obj;
	}
}

%nodefaultctor dict_object;
struct dict_object {
};

%extend dict_object {
	void dump() {
		char * buf = NULL;
		size_t len;
		printf("%s", fd_dict_dump_object(&buf, &len, NULL, $self));
		free(buf);
	}
	enum dict_object_type gettype() {
		enum dict_object_type t;
		int ret = fd_dict_gettype ( $self, &t);
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
			return 0;
		}
		return t;
	}
	struct dictionary * getdict() {
		struct dictionary *d;
		int ret = fd_dict_getdict ( $self, &d );
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
			return NULL;
		}
		return d;
	}
	/* Since casting the pointer requires intelligence, we do it here instead of giving it to SWIG */
	PyObject * getval() {
		/* first, get the type */
		enum dict_object_type t;
		int ret = fd_dict_gettype ( $self, &t);
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
			return NULL;
		}
		switch (t) {
%define %GETVAL_CASE(TYPE,STRUCT)
			case TYPE: {
				PyObject * v = NULL;
				struct STRUCT * data = NULL;
				data = malloc(sizeof(struct STRUCT));
				if (!data) {
					DI_ERROR_MALLOC;
					return NULL;
				}
				ret = fd_dict_getval($self, data);
				if (ret != 0) {
					DI_ERROR(ret, NULL, NULL);
					free(data);
					return NULL;
				}
				SWIG_PYTHON_THREAD_BEGIN_BLOCK;
				v = SWIG_NewPointerObj((void *)data, SWIGTYPE_p_##STRUCT, SWIG_POINTER_OWN );
				Py_XINCREF(v);
				SWIG_PYTHON_THREAD_END_BLOCK;
				return v;
			} break
%enddef
			%GETVAL_CASE( DICT_VENDOR, 	dict_vendor_data );
			%GETVAL_CASE( DICT_APPLICATION, dict_application_data );
			%GETVAL_CASE( DICT_TYPE, 	dict_type_data );
			%GETVAL_CASE( DICT_ENUMVAL, 	dict_enumval_data );
			%GETVAL_CASE( DICT_AVP, 	dict_avp_data );
			%GETVAL_CASE( DICT_COMMAND, 	dict_cmd_data );
			%GETVAL_CASE( DICT_RULE, 	dict_rule_data );
			default:
				DI_ERROR(EINVAL, PyExc_SystemError, "Internal error: Got invalid object type");
		}
		return NULL;
	}
}


/* The following wrapper leaks memory each time an union avp_value is assigned an octet string.
 TODO: fix this leak by better understanding SWIG... 
   -- the alternative is to uncomment the "free" statements below, but then it is easy to
   create a segmentation fault by assigning first an integer, then an octetstring.
 */
%extend avp_value {
	/* The following hack in the proxy file allows assigning the octet string directly like this:
	avp_value.os = "blabla"
	*/
	%pythoncode
	{
    __swig_setmethods__["os"] = _fDpy.avp_value_os_set
    if _newclass:os = _swig_property(_fDpy.avp_value_os_get, _fDpy.avp_value_os_set)
	}
	void os_set(char *STRING, size_t LENGTH) {
		/* free($self->os.data);  -- do not free, in case the previous value was not an OS */
		$self->os.data = malloc(LENGTH);
		if (!$self->os.data) {
			DI_ERROR_MALLOC;
			return;
		}
		memcpy($self->os.data, STRING, LENGTH);
		$self->os.len = LENGTH;
	}
	void os_set(avp_value_os * os) {
		/* free($self->os.data);  -- do not free, in case the previous value was not an OS */
		$self->os.data = malloc(os->len);
		if (!$self->os.data) {
			DI_ERROR_MALLOC;
			return;
		}
		memcpy($self->os.data, os->data, os->len);
		$self->os.len = os->len;
	}
};

%extend avp_value_os {
	void dump() {
		%#define LEN_MAX 20
		int i, n=LEN_MAX;
		if ($self->len < LEN_MAX)
			n = $self->len;
		fd_log_debug("l:%u, v:[", $self->len);
		for (i=0; i < n; i++)
			fd_log_debug("%02.2X", $self->data[i]);
		fd_log_debug("] '%.*s%s'", n, $self->data, n == LEN_MAX ? "..." : "");
	}
	%cstring_output_allocate_size(char ** outbuffer, size_t * outlen, free(*$1));
	void as_str ( char ** outbuffer, size_t * outlen ) {
		char * b;
		if (!$self->len) {
			*outlen = 0;
			*outbuffer = NULL;
			return;
		}
		b = malloc($self->len);
		if (!b) {
			DI_ERROR_MALLOC;
			return;
		}
		memcpy(b, $self->data, $self->len);
		*outlen = $self->len;
		*outbuffer = b;
	}
}


/* Allow constructors with parameters for the dict_*_data */
%extend dict_vendor_data {
	dict_vendor_data(uint32_t id = 0, char * name = NULL) {
		struct dict_vendor_data * d = (struct dict_vendor_data *)calloc(1, sizeof(struct dict_vendor_data));
		if (!d) {
			DI_ERROR_MALLOC;
			return NULL;
		}
		d->vendor_id = id;
		if (name) {
			d->vendor_name = strdup(name);
			if (!d->vendor_name) {
				DI_ERROR_MALLOC;
				free(d);
				return NULL;
			}
		}
		return d;
	}
}

%extend dict_application_data {
	dict_application_data(uint32_t id = 0, char * name = NULL) {
		struct dict_application_data * d = (struct dict_application_data *)calloc(1, sizeof(struct dict_application_data));
		if (!d) {
			DI_ERROR_MALLOC;
			return NULL;
		}
		d->application_id = id;
		if (name) {
			d->application_name = strdup(name);
			if (!d->application_name) {
				DI_ERROR_MALLOC;
				free(d);
				return NULL;
			}
		}
		return d;
	}
}

%extend dict_type_data {
	dict_type_data(enum dict_avp_basetype base = 0, char * name = NULL) {
		struct dict_type_data * d = (struct dict_type_data *)calloc(1, sizeof(struct dict_type_data));
		if (!d) {
			DI_ERROR_MALLOC;
			return NULL;
		}
		d->type_base = base;
		if (name) {
			d->type_name = strdup(name);
			if (!d->type_name) {
				DI_ERROR_MALLOC;
				free(d);
				return NULL;
			}
		}
		return d;
	}
}

%extend dict_enumval_data {
	dict_enumval_data(char * name = NULL, uint32_t v = 0) {
		struct dict_enumval_data * d = (struct dict_enumval_data *)calloc(1, sizeof(struct dict_enumval_data));
		if (!d) {
			DI_ERROR_MALLOC;
			return NULL;
		}
		if (name) {
			d->enum_name = strdup(name);
			if (!d->enum_name) {
				DI_ERROR_MALLOC;
				free(d);
				return NULL;
			}
		}
		d->enum_value.u32 = v;
		return d;
	}
}

%extend dict_avp_data {
	dict_avp_data(uint32_t code = 0, char * name = NULL, enum dict_avp_basetype basetype = 0, uint32_t vendor = 0, int mandatory=0) {
		struct dict_avp_data * d = (struct dict_avp_data *)calloc(1, sizeof(struct dict_avp_data));
		if (!d) {
			DI_ERROR_MALLOC;
			return NULL;
		}
		if (name) {
			d->avp_name = strdup(name);
			if (!d->avp_name) {
				DI_ERROR_MALLOC;
				free(d);
				return NULL;
			}
		}
		d->avp_code = code;
		d->avp_basetype = basetype;
		d->avp_vendor = vendor;
		if (vendor) {
			d->avp_flag_val |= AVP_FLAG_VENDOR;
			d->avp_flag_mask |= AVP_FLAG_VENDOR;
		}
		d->avp_flag_mask |= AVP_FLAG_MANDATORY;
		if (mandatory)
			d->avp_flag_val |= AVP_FLAG_MANDATORY;
		return d;
	}
}

%extend dict_cmd_data {
	dict_cmd_data(uint32_t code = 0, char * name = NULL, int request = 1) {
		struct dict_cmd_data * d = (struct dict_cmd_data *)calloc(1, sizeof(struct dict_cmd_data));
		if (!d) {
			DI_ERROR_MALLOC;
			return NULL;
		}
		if (name) {
			d->cmd_name = strdup(name);
			if (!d->cmd_name) {
				DI_ERROR_MALLOC;
				free(d);
				return NULL;
			}
		}
		d->cmd_code = code;
		d->cmd_flag_mask = CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE;
		d->cmd_flag_val = CMD_FLAG_PROXIABLE | ( request ? CMD_FLAG_REQUEST : 0 );
		return d;
	}
}

%extend dict_rule_data {
	dict_rule_data(struct dict_object *avp = NULL, enum rule_position pos = 0, int min = -1, int max = -1 ) {
		struct dict_rule_data * d = (struct dict_rule_data *)calloc(1, sizeof(struct dict_rule_data));
		if (!d) {
			DI_ERROR_MALLOC;
			return NULL;
		}
		d->rule_avp = avp;
		d->rule_position = pos;
		d->rule_order = 1;
		d->rule_min = min;
		d->rule_max = max;
		return d;
	}
}

