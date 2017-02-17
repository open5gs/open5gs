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

/****** HOOKS *********/


/* Functions to handle the PMD */
%{

struct fd_hook_permsgdata {
	PyObject *PyPML;
};

static void init_permsgdata (struct fd_hook_permsgdata * pmd) {
	/* The PMD is None by default */
	Py_INCREF(Py_None);
	pmd->PyPML = Py_None;
}

static void fini_permsgdata (struct fd_hook_permsgdata * pmd) {
	Py_DECREF(pmd->PyPML);
}

%}

struct fd_hook_data_hdl {
};

%nodefaultctor fd_hook_data_hdl;
%extend fd_hook_data_hdl {
	fd_hook_data_hdl() {
		struct fd_hook_data_hdl * hdl = NULL;
		int ret;
		
		ret = fd_hook_data_register ( sizeof(struct fd_hook_permsgdata), init_permsgdata, fini_permsgdata, &hdl );
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
			return NULL;
		}
		return hdl;
	}
}


/* Now the hook itself */
%{
static void call_the_python_hook_callback(enum fd_hook_type type, struct msg * msg, struct peer_hdr * peer, void * other, struct fd_hook_permsgdata *pmd, void * regdata) {
	PyObject *cb, *result = NULL;
	PyObject *PyType, *PyMsg, *PyPeer, *PyOther, *PyOldPmd;
	
	if (!regdata) {
		LOG_E("Internal error: missing the callback!");
		return;
	}
	cb = regdata;

	SWIG_PYTHON_THREAD_BEGIN_BLOCK;
	/* Convert the arguments */
	PyType = PyLong_FromLong(type);
	Py_INCREF(PyType);
	
	PyMsg  = SWIG_NewPointerObj((void *) msg,     SWIGTYPE_p_msg,     0 );
	
	PyPeer = SWIG_NewPointerObj((void *) peer,    SWIGTYPE_p_peer_hdr,     0 );
	if (other == NULL) {
		PyOther=Py_None;
		Py_INCREF(Py_None);
	} else {
		switch (type) {
		case HOOK_DATA_RECEIVED:
			PyOther= SWIG_NewPointerObj( other, SWIGTYPE_p_fd_cnx_rcvdata, 0 );
			break;
			
		case HOOK_MESSAGE_RECEIVED:
		case HOOK_MESSAGE_ROUTING_ERROR:
		case HOOK_MESSAGE_DROPPED:
		case HOOK_PEER_CONNECT_FAILED:
			PyOther= SWIG_NewPointerObj( other, SWIGTYPE_p_char, 0 );
			break;
			
		case HOOK_MESSAGE_PARSING_ERROR:
			if (msg) {
				PyOther= SWIG_NewPointerObj( other, SWIGTYPE_p_char, 0 );
			} else {
				PyOther= SWIG_NewPointerObj( other, SWIGTYPE_p_fd_cnx_rcvdata, 0 );
			}
			break;
		default:
			/* In other cases, other should be NULL */
			LOG_E("Internal error: got a value of *other");
		}
		
	}
	
	if (pmd == NULL) {
		Py_INCREF(Py_None);
		PyOldPmd=Py_None;
	} else {
		PyOldPmd=pmd->PyPML;
	}
	
	/* Call the function */
	result = PyObject_CallFunction(cb, "(OOOOO)", PyType, PyMsg, PyPeer, PyOther, PyOldPmd);
	
	SWIG_PYTHON_THREAD_END_BLOCK;
	if (pmd == NULL)
		return;
	
	Py_DECREF(pmd->PyPML);
	Py_INCREF(result);
	pmd->PyPML = result;
}
%}



struct fd_hook_hdl {
};

%nodefaultctor fd_hook_hdl;
%extend fd_hook_hdl {
	fd_hook_hdl(uint32_t type_mask, PyObject * PyCb) {
		struct fd_hook_hdl *hdl;
		int ret;
		
		Py_XINCREF(PyCb);
		
		ret = fd_hook_register ( type_mask, call_the_python_hook_callback, PyCb, NULL, &hdl );
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
			return NULL;
		}
		return hdl;
	}
	fd_hook_hdl(uint32_t type_mask, PyObject * PyCb, struct fd_hook_data_hdl *datahdl) {
		struct fd_hook_hdl *hdl;
		int ret;
		
		Py_XINCREF(PyCb);
		
		ret = fd_hook_register ( type_mask, call_the_python_hook_callback, PyCb, datahdl, &hdl );
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
			return NULL;
		}
		return hdl;
	}
	~fd_hook_hdl() {
		struct fd_hook_hdl * hdl = self;
		int ret = fd_hook_unregister(hdl);
		if (ret != 0) {
			DI_ERROR(ret, NULL, NULL);
		}
		return;
	}
}
