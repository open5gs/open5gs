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

/****** ROUTING *********/

struct rt_data {
};

%extend rt_data {
	rt_data() {
		struct rt_data * r = NULL;
		int ret = fd_rtd_init(&r);
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
			return NULL;
		}
		return r;
	}
	~rt_data() {
		struct rt_data *r = self;
		fd_rtd_free(&r);
	}
	%apply (char *STRING, int LENGTH) { (char * peerid, size_t peeridlen) };
	%apply (char *STRING, int LENGTH) { (char * realm, size_t realmlen) };
	void add(char * peerid, size_t peeridlen, char * realm, size_t realmlen) {
		int ret = fd_rtd_candidate_add($self, peerid, peeridlen, realm, realmlen);
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
		}
	}
	void remove(char * STRING, size_t LENGTH) {
		fd_rtd_candidate_del($self, (os0_t)STRING, LENGTH);
	}
	int error(char * peerid, size_t peeridlen, char * STRING, size_t LENGTH, uint32_t rcode) {
		int n;
		int ret =  fd_rtd_error_add($self, peerid, peeridlen, (os0_t)STRING, LENGTH, rcode, NULL, &n);
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
		}
		return n;
	}
	struct fd_list * extract(int score = 0) {
		struct fd_list * li = NULL;
		fd_rtd_candidate_extract($self, &li, score);
		return li;
	}
}



%extend rtd_candidate {
	void dump() {
		fd_log_debug("candidate %p", $self);
		fd_log_debug("  id : %s",  $self->diamid);
		fd_log_debug("  rlm: %s", $self->realm);
		fd_log_debug("  sc : %d", $self->score);
	}
}


%{
/* call it (will be called from a different thread than the interpreter, when message arrives) */
static int call_the_python_rt_fwd_callback(void * pycb, struct msg **msg) {
	PyObject *PyMsg;
	PyObject *cb, *result = NULL;
	int ret = 0;
	
	if (!pycb) {
		fd_log_debug("Internal error: missing the callback!");
		return ENOTSUP;
	}
	cb = pycb;
	
	SWIG_PYTHON_THREAD_BEGIN_BLOCK;
	/* Convert the arguments */
	PyMsg  = SWIG_NewPointerObj((void *)*msg,     SWIGTYPE_p_msg,     0 );
	
	/* Call the function */
	result = PyObject_CallFunction(cb, "(O)", PyMsg);
	
	/* The result is supposedly composed of: [ ret, *msg ] */
	if ((result == NULL) || (!PyList_Check(result)) || (PyList_Size(result) != 2)) {
		fd_log_debug("Error: The Python callback did not return [ ret, msg ].");
		ret = EINVAL;
		goto out;
	}
	
	/* Convert the return values */
	if (!SWIG_IsOK(SWIG_AsVal_int(PyList_GetItem(result, 0), &ret))) {
		fd_log_debug("Error: Cannot convert the first return value to integer.");
		ret = EINVAL;
		goto out;
	}
	if (ret) {
		TRACE_DEBUG(INFO, "The Python callback returned the error code %d (%s)", ret, strerror(ret));
		goto out;
	}
	
	if (!SWIG_IsOK(SWIG_ConvertPtr(PyList_GetItem(result, 1), (void *)msg, SWIGTYPE_p_msg, SWIG_POINTER_DISOWN))) {
		fd_log_debug("Error: Cannot convert the second return value to message.");
		ret = EINVAL;
		goto out;
	}
	
out:	
	Py_XDECREF(result);
	
	SWIG_PYTHON_THREAD_END_BLOCK;
	return ret;
}
%}


struct fd_rt_fwd_hdl {
};

%extend fd_rt_fwd_hdl{
	fd_rt_fwd_hdl(PyObject * PyCb, enum fd_rt_fwd_dir dir) {
		struct fd_rt_fwd_hdl * r = NULL;
		int ret;
		
		Py_XINCREF(PyCb);

		ret = fd_rt_fwd_register( call_the_python_rt_fwd_callback, PyCb, dir, &r );
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
			return NULL;
		}
		return r;
	}
	
	~fd_rt_fwd_hdl() {
		PyObject * func;
		int ret = fd_rt_fwd_unregister ( $self, (void *) &func );
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
			return;
		}
		Py_XDECREF(func);
		return;
	}
}


%{
/* call it (will be called from a different thread than the interpreter, when message arrives) */
static int call_the_python_rt_out_callback(void * pycb, struct msg **msg, struct fd_list * candidates) {
	PyObject *PyMsg, *PyCands;
	PyObject *cb, *result = NULL;
	int ret = 0;
	
	if (!pycb) {
		fd_log_debug("Internal error: missing the callback!");
		return ENOTSUP;
	}
	cb = pycb;
	
	SWIG_PYTHON_THREAD_BEGIN_BLOCK;
	/* Convert the arguments */
	PyMsg   = SWIG_NewPointerObj((void *)*msg,       SWIGTYPE_p_msg,     0 );
	PyCands = SWIG_NewPointerObj((void *)candidates, SWIGTYPE_p_fd_list, 0 );
	
	/* Call the function */
	result = PyObject_CallFunction(cb, "(OO)", PyMsg, PyCands);
	
	/* The result is supposedly composed of: [ ret, *msg ] */
	if (result == NULL){
		fd_log_debug("Error: The Python callback raised an exception.");
		ret = EINVAL;
		goto out;
	}
	
	/* Convert the return values */
	if (!SWIG_IsOK(SWIG_AsVal_int(result, &ret))) {
		fd_log_debug("Error: Cannot convert the return value to integer.");
		ret = EINVAL;
		goto out;
	}
out:	
	Py_XDECREF(result);
	
	SWIG_PYTHON_THREAD_END_BLOCK;
	return ret;
}
%}


struct fd_rt_out_hdl {
};

%extend fd_rt_out_hdl{
	fd_rt_out_hdl(PyObject * PyCb, int priority = 0) {
		struct fd_rt_out_hdl * r = NULL;
		int ret;
		
		Py_XINCREF(PyCb);

		ret = fd_rt_out_register( call_the_python_rt_out_callback, PyCb, priority, &r );
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
			return NULL;
		}
		return r;
	}
	
	~fd_rt_out_hdl() {
		PyObject * func;
		int ret = fd_rt_out_unregister ( $self, (void *) &func );
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
			return;
		}
		Py_XDECREF(func);
		return;
	}
}

