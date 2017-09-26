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

/****** LISTS *********/

%extend fd_list {
	/* allow a parameter in the constructor, and perform the fd_list_init operation */
	fd_list(void * o = NULL) {
		struct fd_list * li;
		li = (struct fd_list *) malloc(sizeof(struct fd_list));
		if (!li) {
			DI_ERROR_MALLOC;
			return NULL;
		}
		fd_list_init(li, o);
		return li;
	}
	/* Unlink before freeing */
	~fd_list() {
		fd_list_unlink($self);
		free($self);
	}
	/* For debug, show the values of the list */
	void dump() {
		fd_log_debug("list: %p", $self);
		fd_log_debug("  - next: %p", $self->next);
		fd_log_debug("  - prev: %p", $self->prev);
		fd_log_debug("  - head: %p", $self->head);
		fd_log_debug("  - o   : %p", $self->o);
	}
	/* Insert before/after wrapper */
	void insert_prev(struct fd_list * li) {
		fd_list_insert_before($self, li);
	}
	void insert_next(struct fd_list * li) {
		fd_list_insert_after($self, li);
	}
	/* Test for emptyness */
	PyObject * isempty() {
		PyObject * ret;
		if (FD_IS_LIST_EMPTY($self))
			ret = Py_True;
		else
			ret = Py_False;
		Py_XINCREF(ret);
		return ret;
	}
	/* Concatenate two lists */
	void concat(struct fd_list * li) {
		fd_list_move_end($self, li);
	}
	/* Unlink without freeing */
	void detach() {
		fd_list_unlink($self);
	}
	
	/* Return the list as python list of elements */
	PyObject * enum_as(char * type = NULL, int dont_use_o = 0) {
		struct fd_list *li;
		swig_type_info * desttype = NULL;
		PyObject * rl;
		
		if ($self->head != $self) {
			DI_ERROR(EINVAL, NULL, "This method can only be called on the list sentinel.");
			return NULL;
		}
		
		if (type) {
			desttype = SWIG_TypeQuery(type);
			if (!desttype) {
				DI_ERROR(EINVAL, NULL, "Unable to resolve this type. Please check the form: 'struct blahbla *'");
				return NULL;
			}
		}
		if (desttype == NULL) {
			/* fallback to fd_list */
			desttype = SWIGTYPE_p_fd_list;
			/* in this case, don't follow the 'o' link */
			dont_use_o = 1;
		}
		
		rl = PyList_New(0);
		SWIG_PYTHON_THREAD_BEGIN_BLOCK;
		for (li = $self->next; li != $self; li = li->next) {
			void * obj = NULL;
			if (dont_use_o || li->o == NULL)
				obj = li;
			else
				obj = li->o;
			PyList_Append(rl, SWIG_NewPointerObj(obj, desttype, 0 ));
		}
		Py_XINCREF(rl);
		SWIG_PYTHON_THREAD_END_BLOCK;
		
		return rl;
	}
};

