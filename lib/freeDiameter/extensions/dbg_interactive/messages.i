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

/****** MESSAGES *********/

%{
struct anscb_py_layer {
	PyObject * cb;
	PyObject * expcb;
	PyObject * data;
};

/* If a python callback was provided, it is received in cbdata */
static void anscb_python(void *cbdata, struct msg ** msg) {
	/* The python callback is received in cbdata */
	PyObject * result, *PyMsg;
	struct anscb_py_layer * l = cbdata;
	
	if (!l) {
		fd_log_debug("Internal error! Python callback disappeared...");
		return;
	}
	
	if (l->cb) {
	
		SWIG_PYTHON_THREAD_BEGIN_BLOCK;

		if (!msg || !*msg) {
			PyMsg = Py_None;
		} else {
			PyMsg = SWIG_NewPointerObj((void *)*msg,     SWIGTYPE_p_msg,     0 );
		}

		result = PyObject_CallFunction(l->cb, "(OO)", PyMsg, l->data);
		Py_XDECREF(l->cb);
		Py_XDECREF(l->expcb);
		Py_XDECREF(l->data);
		free(l);

		/* The callback is supposed to return a message or NULL */
		if (!SWIG_IsOK(SWIG_ConvertPtr(result, (void *)msg, SWIGTYPE_p_msg, SWIG_POINTER_DISOWN))) {
			fd_log_debug("Error: Cannot convert the return value to message.");
			*msg = NULL;
		}

		Py_XDECREF(result);

		SWIG_PYTHON_THREAD_END_BLOCK;
		
	}
	/* else */
		/* Only the timeout was specified, without a callback */
		/* in this case, just delete the message */
		/* it actually happens automatically when we do nothing. */
}

static void expcb_python(void *cbdata, DiamId_t sentto, size_t senttolen, struct msg ** msg) {
	/* The python callback is received in cbdata */
	PyObject * result, *PyMsg;
	struct anscb_py_layer * l = cbdata;
	
	if (!l) {
		fd_log_debug("Internal error! Python callback disappeared...");
		return;
	}
	
	SWIG_PYTHON_THREAD_BEGIN_BLOCK;

	if (!msg || !*msg) {
		PyMsg = Py_None;
	} else {
		PyMsg = SWIG_NewPointerObj((void *)*msg,     SWIGTYPE_p_msg,     0 );
	}

	result = PyObject_CallFunction(l->expcb, "(Os#O)", PyMsg, sentto, senttolen, l->data);
	Py_XDECREF(l->cb);
	Py_XDECREF(l->expcb);
	Py_XDECREF(l->data);
	free(l);

	/* The callback is supposed to return a message or NULL */
	if (!SWIG_IsOK(SWIG_ConvertPtr(result, (void *)msg, SWIGTYPE_p_msg, SWIG_POINTER_DISOWN))) {
		fd_log_debug("Error: Cannot convert the return value to message.");
		*msg = NULL;
	}

	Py_XDECREF(result);

	SWIG_PYTHON_THREAD_END_BLOCK;
		
}



%}

struct msg {
};

%extend msg {
	msg(struct dict_object * model=NULL, int flags = MSGFL_ALLOC_ETEID) {
		struct msg * m = NULL;
		int ret = fd_msg_new( model, flags, &m);
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
		}
		return m;
	}
	/* construct also from a binary buffer */
	msg(char * STRING, size_t LENGTH) {
		int ret;
		struct msg * m = NULL;
		/* First, copy the string */
		unsigned char * buf = malloc(LENGTH);
		if (buf == NULL) {
			DI_ERROR_MALLOC;
			return NULL;
		}
		memcpy(buf, STRING, LENGTH);
		ret = fd_msg_parse_buffer(&buf, LENGTH, &m);
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
			free(buf);
			return NULL;
		}
		return m;
	}
	~msg() {
		int ret = fd_msg_free($self);
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
		}
	}
	
	/* SEND THE MESSAGE */
	%delobject send; /* when this has been called, the msg must not be freed anymore */
	void send(PyObject * PyCb = NULL, PyObject * data = NULL, PyObject * PyExpCb = NULL, unsigned int timeout = 0) {
		int ret;
		struct msg * m = $self;
		struct anscb_py_layer * l = NULL;
		
		if (PyCb || timeout) {
			l = malloc(sizeof(struct anscb_py_layer));
			if (!l) {
				DI_ERROR_MALLOC;
				return;
			}

			Py_XINCREF(PyCb);
			Py_XINCREF(data);
			Py_XINCREF(PyExpCb);
			l->expcb = PyExpCb;
			l->cb = PyCb;
			l->data = data;
		}
		
		if (timeout) {
			struct timespec ts;
			(void) clock_gettime(CLOCK_REALTIME, &ts);
			ts.tv_sec += timeout;
			ret = fd_msg_send_timeout(&m, anscb_python, l, expcb_python, &ts);
		} else {
			ret = fd_msg_send(&m, PyCb ? anscb_python : NULL, l);
		}
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
		}
	}
	
	/* Create an answer */
	%delobject create_answer; /* when this has been called, the original msg should not be freed anymore */
	struct msg * create_answer(struct dictionary * dict = NULL, int flags = 0) {
		/* if dict is not provided, attempt to get it from the request model */
		struct dictionary * d = dict;
		struct msg * m = $self;
		int ret;
		if (!d) {
			struct dict_object * mo = NULL;
			ret = fd_msg_model($self, &mo);
			if (ret != 0) {
				DI_ERROR(ret, NULL, "Error retrieving query model.");
				return NULL;
			}
			if (mo == NULL) {
				/* use the fD dictionary by default */
				d = fd_g_config->cnf_dict;
			} else {
				ret = fd_dict_getdict ( mo, &d );
				if (ret != 0) {
					DI_ERROR(ret, NULL, "Error retrieving query's dictionary.");
					return NULL;
				}
			}
		}
		ret = fd_msg_new_answer_from_req(d, &m, flags);
		if (ret != 0) {
			DI_ERROR(ret, NULL, "Cannot guess the dictionary to use, please provide it as parameter.");
			return NULL;
		}
		
		return m;
	}
	/* Return the first child AVP if any */
	struct avp * first_child() {
		struct avp * a = NULL;
		int ret = fd_msg_browse($self, MSG_BRW_FIRST_CHILD, &a, NULL);
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
		}
		return a;
	}
	
	/* Enumerable list of children AVP */
	%newobject children;
	PyObject * children() {
		struct avp * a = NULL;
		PyObject * rl;
		int ret = fd_msg_browse($self, MSG_BRW_FIRST_CHILD, &a, NULL);
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
			return NULL;
		}
		SWIG_PYTHON_THREAD_BEGIN_BLOCK;
		rl = PyList_New(0);
		while (a) {
			PyList_Append(rl, SWIG_NewPointerObj((void *)a, SWIGTYPE_p_avp, 0 /* do not own the AVPs */ ));
			ret = fd_msg_browse(a, MSG_BRW_NEXT, &a, NULL);
			if (ret != 0) {
				DI_ERROR(ret, NULL, NULL);
				return NULL;
			}
		}
		Py_XINCREF(rl);
		SWIG_PYTHON_THREAD_END_BLOCK;
		return rl;
	}
	
	/* Add a new AVP */
	void add_child(struct avp *DISOWN, int begin = 0) {
		int ret = fd_msg_avp_add ( $self, begin ? MSG_BRW_FIRST_CHILD : MSG_BRW_LAST_CHILD, DISOWN);
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
		}
	}
	
	/* Search an AVP */
	struct avp * search(struct dict_object * what) {
		struct avp * a = NULL;
		int ret = fd_msg_search_avp($self, what, &a);
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
			return NULL;
		}
		return a;
	}
	
	/* Dump */
	void dump (int tree = 1) {
		char * buf = NULL;
		size_t len;
		printf("%s", fd_msg_dump_treeview(&buf, &len, NULL, $self, NULL, 0, tree));
		free(buf);
	}
	
	/* Model */
	struct dict_object * model() {
		struct dict_object * m = NULL;
		int ret = fd_msg_model($self, &m);
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
			return NULL;
		}
		return m;
	}
	
	/* Header */
	struct msg_hdr * header() {
		struct msg_hdr * h = NULL;
		int ret = fd_msg_hdr($self, &h);
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
			return NULL;
		}
		return h;
	}
	
	/* Get query if message is an answer */
	struct msg * get_query() {
		struct msg * q = NULL;
		int ret = fd_msg_answ_getq($self, &q);
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
			return NULL;
		}
		return q;
	}
	
	/* Get / Set routing data */
	struct rt_data * get_rtd() {
		struct rt_data * r = NULL;
		int ret = fd_msg_rt_get($self, &r);
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
			return NULL;
		}
		return r;
	}
	void set_rtd(struct rt_data *DISOWN) {
		struct rt_data * r = DISOWN;
		int ret = fd_msg_rt_associate($self, r);
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
		}
		return;
	}
	
	/* Is routable? */
	PyObject * is_routable() {
		PyObject * r;
		if (fd_msg_is_routable($self))
			r = Py_True;
		else
			r = Py_False;
		Py_XINCREF(r);
		return r;
	}
	
	/* Is request? (shortcut) */
	PyObject * is_request() {
		PyObject * r;
		int ret;
		struct msg_hdr * h;
		
		ret = fd_msg_hdr($self, &h);
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
		}
		if (h->msg_flags & CMD_FLAG_REQUEST) 
			r = Py_True;
		else
			r = Py_False;
		Py_XINCREF(r);
		return r;
	}
	
	/* Get the source */
	%cstring_output_allocate_size(char ** outid, size_t * outlen, /* do not free */);
	void source(char ** outid, size_t * outlen) {
		int ret = fd_msg_source_get($self, outid, outlen);
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
			return;
		}
		return;
	}
	
	/* Get the session */
	%newobject get_session; /* it may be created or not, it is harmless because we only reclaim in ~session */
	struct session *get_session(struct dictionary * dict = NULL) {
		struct session *s = NULL;
		struct dictionary * d = dict;
		int ret = 0;
		if (d == NULL)
			d = fd_g_config->cnf_dict; /* default: use daemon's */
		ret = fd_msg_sess_get(d, $self, &s, NULL);
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
			return NULL;
		}
		return s;
	}
	
	/* Bufferize */
	%cstring_output_allocate_size(char ** outbuffer, size_t * outlen, free(*$1));
	void bufferize ( char ** outbuffer, size_t * outlen ) {
		int ret = fd_msg_bufferize ( $self, (void *)outbuffer, outlen );
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
		}
	}
	
	/* Dictionary parsing */
	%newobject parse_dict;
	struct fd_pei * parse_dict(struct dictionary * dict=NULL) {
		int ret;
		struct fd_pei pei, *e = NULL;
		struct dictionary * d = dict;
		memset(&pei, 0, sizeof(struct fd_pei));
		if (d == NULL)
			d = fd_g_config->cnf_dict; /* default: use daemon's */
		
		ret = fd_msg_parse_dict ( $self, d, &pei );
		if (ret != 0) {
			e = malloc(sizeof(struct fd_pei));
			if (!e) {
				DI_ERROR_MALLOC;
				return NULL;
			}
			memcpy(e, &pei, sizeof(struct fd_pei));
		}
		return e; /* returns NULL when everything went OK */
	}
	
	/* Rules parsing */
	%newobject parse_rules;
	struct fd_pei * parse_rules(struct dictionary * dict=NULL) {
		int ret;
		struct fd_pei pei, *e = NULL;
		struct dictionary * d = dict;
		memset(&pei, 0, sizeof(struct fd_pei));
		if (d == NULL)
			d = fd_g_config->cnf_dict; /* default: use daemon's */
		
		ret = fd_msg_parse_rules ( $self, d, &pei );
		if (ret != 0) {
			e = malloc(sizeof(struct fd_pei));
			if (!e) {
				DI_ERROR_MALLOC;
				return NULL;
			}
			memcpy(e, &pei, sizeof(struct fd_pei));
		}
		return e; /* returns NULL when everything went OK */
	}
	
	/* Update the length info in header */
	void update_length() {
		int ret = fd_msg_update_length ( $self );
		if (ret) {
			DI_ERROR(ret, NULL, NULL);
		}
	}
	
	/* Set the result code */
	void rescode_set(char * rescode = "DIAMETER_SUCCESS", char * errormsg = NULL, struct avp * optavp = NULL, int type_id = 0) {
		int ret = fd_msg_rescode_set( $self, rescode, errormsg, optavp, type_id );
		if (ret) {
			DI_ERROR(ret, NULL, NULL);
		}
	}
	
	/* Add the origin */
	void add_origin(int osi = 0) {
		int ret = fd_msg_add_origin( $self, osi );
		if (ret) {
			DI_ERROR(ret, NULL, NULL);
		}
	}
	
}

struct avp {
};

%extend avp {
	avp(struct dict_object * model = NULL, int flags = 0) {
		struct avp * a = NULL;
		int ret = fd_msg_avp_new( model, flags, &a);
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
		}
		return a;
	}
	~avp() {
		int ret = fd_msg_free($self);
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
		}
	}
	
	/* Return the first child AVP if any */
	struct avp * first_child() {
		struct avp * a = NULL;
		int ret = fd_msg_browse($self, MSG_BRW_FIRST_CHILD, &a, NULL);
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
		}
		return a;
	}
	
	/* Enumerable list of children AVP */
	%newobject children;
	PyObject * children() {
		struct avp * a = NULL;
		PyObject * rl;
		int ret = fd_msg_browse($self, MSG_BRW_FIRST_CHILD, &a, NULL);
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
			return NULL;
		}
		SWIG_PYTHON_THREAD_BEGIN_BLOCK;
		rl = PyList_New(0);
		while (a) {
			PyList_Append(rl, SWIG_NewPointerObj((void *)a, SWIGTYPE_p_avp, 0 /* the AVPs are not owned */ ));
			ret = fd_msg_browse(a, MSG_BRW_NEXT, &a, NULL);
			if (ret != 0) {
				DI_ERROR(ret, NULL, NULL);
				return NULL;
			}
		}
		Py_XINCREF(rl);
		SWIG_PYTHON_THREAD_END_BLOCK;
		return rl;
	}
	
	/* Add a new AVP */
	void add_next(struct avp *avp) {
		int ret = fd_msg_avp_add ( $self, MSG_BRW_NEXT, avp);
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
		}
	}
	void add_prev(struct avp *avp) {
		int ret = fd_msg_avp_add ( $self, MSG_BRW_PREV, avp);
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
		}
	}
	void add_child(struct avp *DISOWN, int begin = 0) {
		int ret = fd_msg_avp_add ( $self, begin ? MSG_BRW_FIRST_CHILD : MSG_BRW_LAST_CHILD, DISOWN);
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
		}
	}
	
	/* Get the next AVP at the same level */
	struct avp * get_next() {
		struct avp * a = NULL;
		int ret = fd_msg_browse($self, MSG_BRW_NEXT, &a, NULL);
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
		}
		return a;
	}
	
	/* Dump */
	void dump (int tree = 1) {
		char * buf = NULL;
		size_t len;
		printf("%s", fd_msg_dump_treeview(&buf, &len, NULL, $self, NULL, 0, tree));
		free(buf);
	}
	
	/* Model */
	struct dict_object * model() {
		struct dict_object * m = NULL;
		int ret = fd_msg_model($self, &m);
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
			return NULL;
		}
		return m;
	}
	
	/* Header */
	struct avp_hdr * header() {
		struct avp_hdr * h = NULL;
		int ret = fd_msg_avp_hdr($self, &h);
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
			return NULL;
		}
		return h;
	}
	
	/* set value */
	void setval(union avp_value * val) {
		int ret = fd_msg_avp_setvalue ( $self, val );
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
		}
	}
	
	/* Update the length info in header */
	void update_length() {
		int ret = fd_msg_update_length ( $self );
		if (ret) {
			DI_ERROR(ret, NULL, NULL);
		}
	}
}
