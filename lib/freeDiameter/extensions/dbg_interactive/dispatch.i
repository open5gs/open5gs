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

/****** DISPATCH *********/


%{
/* call it (will be called from a different thread than the interpreter, when message arrives) */
static int call_the_python_dispatch_callback(struct msg **msg, struct avp *avp, struct session *session, void * pycb, enum disp_action *action) {
	PyObject *PyMsg, *PyAvp, *PySess;
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
	PyAvp  = SWIG_NewPointerObj((void *) avp,     SWIGTYPE_p_avp,     0 );
	PySess = SWIG_NewPointerObj((void *) session, SWIGTYPE_p_session, 0 );
	
	/* Call the function */
	result = PyObject_CallFunction(cb, "(OOO)", PyMsg, PyAvp, PySess);
	
	/* The result is supposedly composed of: [ ret, *msg, *action ] */
	if ((result == NULL) || (!PyList_Check(result)) || (PyList_Size(result) != 3)) {
		fd_log_debug("Error: The Python callback did not return [ ret, msg, action ].");
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
	
	if (!SWIG_IsOK(SWIG_AsVal_int(PyList_GetItem(result, 2), (int *)action))) {
		fd_log_debug("Error: Cannot convert the third return value to integer.");
		ret = EINVAL;
		goto out;
	}
	
	TRACE_DEBUG(FULL, "Python callback return: *action = %d", *action);
out:	
	Py_XDECREF(result);
	
	SWIG_PYTHON_THREAD_END_BLOCK;
	return ret;
}
%}

struct disp_hdl {
};

%nodefaultctor disp_hdl;
%extend disp_hdl {
	disp_hdl(PyObject * PyCb, enum disp_how how, struct disp_when * when) {
		struct disp_hdl * hdl = NULL;
		int ret;
		
		Py_XINCREF(PyCb);
		
		ret = fd_disp_register ( call_the_python_dispatch_callback, how, when, PyCb, &hdl );
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
			return NULL;
		}
		return hdl;
	}
	~disp_hdl() {
		struct disp_hdl * hdl = self;
		PyObject * cb = NULL;
		int ret = fd_disp_unregister(&hdl, (void *)&cb);
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
		}
		Py_XDECREF(cb);
		return;
	}
}


%extend disp_when {
	disp_when(struct dict_object * app = NULL, struct dict_object * command = NULL, struct dict_object * avp = NULL, struct dict_object * value = NULL) {
      		struct disp_when * w = (struct disp_when *)calloc(1, sizeof(struct disp_when));
		if (!w) {
			DI_ERROR_MALLOC;
			return NULL;
		}
		w->app = app;
		w->command = command;
		w->avp = avp;
		w->value = value;
		return w;
	}
}
