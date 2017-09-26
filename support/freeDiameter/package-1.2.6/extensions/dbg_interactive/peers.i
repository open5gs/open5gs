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

/****** PEERS *********/

%{
static void fd_add_cb(struct peer_info *peer, void *data) {
	/* Callback called when the peer connection completes (or fails) */
	PyObject *PyPeer, *PyFunc;
	PyObject *result = NULL;
	
	if (!data) {
		TRACE_DEBUG(INFO, "Internal error: missing callback");
		return;
	}
	PyFunc = data;
	
	SWIG_PYTHON_THREAD_BEGIN_BLOCK;
	
	/* Convert the argument */
	PyPeer  = SWIG_NewPointerObj((void *)peer,     SWIGTYPE_p_peer_info,     0 );
	
	/* Call the function */
	result = PyObject_CallFunction(PyFunc, "(O)", PyPeer);
	
	Py_XDECREF(result);
	Py_XDECREF(PyFunc);
	
	SWIG_PYTHON_THREAD_END_BLOCK;
	return;
}
%}

%extend peer_info {
	peer_info () {
		struct peer_info *np = (struct peer_info *)calloc(1, sizeof(struct peer_info));
		if (!np) {
			DI_ERROR_MALLOC;
			return NULL;
		}
		
		fd_list_init(&np->pi_endpoints, NULL);
		
		return np;
	}

	/* Wrapper around fd_peer_add to allow calling the python callback */
	%delobject add;
	void add(PyObject * PyCb=NULL) {
		int ret;
		
		if (PyCb) {
			Py_XINCREF(PyCb);
			ret = fd_peer_add ( $self, "dbg_interactive", fd_add_cb, PyCb );
		} else {
			ret = fd_peer_add ( $self, "dbg_interactive", NULL, NULL );
		}
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
		}
	}
}

%inline %{
static struct peer_hdr * peer_search(char *STRING, size_t LENGTH) {
	struct peer_hdr *r = NULL;
	int ret = fd_peer_getbyid( STRING, LENGTH, 0, &r );
	if (ret) {
		DI_ERROR(ret, NULL, NULL);
		return NULL;
	}
	return r;
}
%}

%{
static PyObject * validate_cb_py = NULL;
static PyObject * validate_cb2_py = NULL;

/* C wrapper that calls validate_cb2_py */
int call_the_python_validate_callback2(struct peer_info * info) {
	PyObject *PyInfo;
	PyObject *result = NULL;
	int ret = 0;
	
	if (!validate_cb2_py) {
		fd_log_debug("Internal error: missing the callback2!");
		return ENOTSUP;
	}
	
	SWIG_PYTHON_THREAD_BEGIN_BLOCK;
	/* Convert the arguments */
	PyInfo  = SWIG_NewPointerObj((void *)info,     SWIGTYPE_p_peer_info,     0 );
	
	/* Call the function */
	result = PyObject_CallFunction(validate_cb2_py, "(O)", PyInfo);
	
	/* The result is an integer */
	if ((result == NULL) || !SWIG_IsOK(SWIG_AsVal_int(result, &ret))) {
		fd_log_debug("Error: The Python callback did not return an integer.");
		ret = EINVAL;
		goto out;
	}
	
out:	
	Py_XDECREF(result);
	SWIG_PYTHON_THREAD_END_BLOCK;
	return ret;
}

/* C wrapper that calls validate_cb_py */
int call_the_python_validate_callback(struct peer_info * info, int * auth, int (**cb2)(struct peer_info *)) {
	PyObject *PyInfo;
	PyObject *result = NULL;
	int ret = 0;
	
	if (!validate_cb_py) {
		fd_log_debug("Internal error: missing the callback!");
		return ENOTSUP;
	}
	
	SWIG_PYTHON_THREAD_BEGIN_BLOCK;
	/* Convert the arguments */
	PyInfo  = SWIG_NewPointerObj((void *)info,     SWIGTYPE_p_peer_info,     0 );
	
	/* Call the function */
	result = PyObject_CallFunction(validate_cb_py, "(O)", PyInfo);
	
	/* The result is supposedly -1, 1, or a cb2 */
	if (result == NULL) {
		fd_log_debug("Error: The Python callback did not return a value.");
		ret = EINVAL;
		goto out;
	}
	
	if (PyCallable_Check(result)) {
		if (cb2) {
			if (validate_cb2_py && (validate_cb2_py != result)) {
				fd_log_debug("Only 1 register callback2 is supported currently");
				ret = ENOTSUP;
				goto out;
			}
			validate_cb2_py = result;
			*cb2 = call_the_python_validate_callback2;
			*auth = 1;
			goto out_nodec;
		} else {
			*auth = 1;
			goto out; /* ignore the callback since it won't be used */
		}
	} else { /* In this case, the return value must be -1, 0, or 1 */
		if (!SWIG_IsOK(SWIG_AsVal_int(result, auth))) {
			fd_log_debug("Error: Cannot convert the return value to integer.");
			ret = EINVAL;
			goto out;
		}
	}
	
out:	
	Py_XDECREF(result);
out_nodec:	
	SWIG_PYTHON_THREAD_END_BLOCK;
	TRACE_DEBUG(FULL, "ret=%d, *auth=%d, cb2=%p, *cb2=%p", ret, *auth, cb2, cb2 ? *cb2 : NULL);
	return ret;
}

%}

%inline %{
static void peer_validate_register(PyObject * PyCb) {
	int ret ;
	
	if (!PyCb) {
		DI_ERROR(EINVAL, NULL, "The callback must be provided");
		return;
	}
	
	if (validate_cb_py) {
		if (PyCb != validate_cb_py) {
			DI_ERROR(ENOTSUP, PyExc_RuntimeError, "Only 1 register callback is supported currently");
			return;
		}
	} else {
		validate_cb_py = PyCb;
		Py_XINCREF(PyCb);
	}
	
	ret = fd_peer_validate_register ( call_the_python_validate_callback );
	if (ret) {
		DI_ERROR(ret, NULL, NULL);
	}
}
%}
