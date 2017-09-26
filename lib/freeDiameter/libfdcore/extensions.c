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

#include "fdcore-internal.h"

#include <dlfcn.h>	/* We may use libtool's <ltdl.h> later for better portability.... */
#include <libgen.h>	/* for "basename" */

/* plugins management */

/* List of extensions to load, from the configuration parsing */
struct fd_ext_info {
	struct fd_list	chain;		/* link in the list */
	char 		*filename;	/* extension filename. must be a dynamic library with fd_ext_init symbol. */
	char 		*conffile;	/* optional configuration file name for the extension */
	void 		*handler;	/* object returned by dlopen() */
	const char 	**depends;	/* names of the other extensions this one depends on (if provided) */
	char		*ext_name;	/* points to the extension name, either inside depends, or basename(filename) */
	int		free_ext_name;	/* must be freed if it was malloc'd */
	void		(*fini)(void);	/* optional address of the fd_ext_fini callback */
};

/* list of extensions */
static struct fd_list ext_list = FD_LIST_INITIALIZER(ext_list);

/* Add new extension */
int fd_ext_add( char * filename, char * conffile )
{
	struct fd_ext_info * new;
	
	TRACE_ENTRY("%p %p", filename, conffile);
	
	/* Check the filename is valid */
	CHECK_PARAMS( filename );
	
	/* Create a new object in the list */
	CHECK_MALLOC(  new = malloc( sizeof(struct fd_ext_info) )  );
	memset(new, 0, sizeof(struct fd_ext_info));
	fd_list_init(&new->chain, NULL);
	new->filename = filename;
	new->conffile = conffile;
	fd_list_insert_before( &ext_list, &new->chain );
	TRACE_DEBUG (FULL, "Extension %s added to the list.", filename);
	return 0;
}

/* Dump the list */
DECLARE_FD_DUMP_PROTOTYPE(fd_ext_dump)
{
	struct fd_list * li;
	FD_DUMP_HANDLE_OFFSET();
	
	if (FD_IS_LIST_EMPTY(&ext_list)) {
		CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "-none-"), return NULL);
	} else {
		for (li = ext_list.next; li != &ext_list; li = li->next)
		{
			struct fd_ext_info * ext = (struct fd_ext_info *)li;
			CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "'%s'[%s], %sloaded%s",
						ext->filename, 
						ext->conffile?:"(no config file)", 
						ext->handler ? "" : "not ", (li->next == &ext_list) ? "":"\n"), return NULL);
		}
	}
	return *buf;
}

/* Check the dependencies. The object must have been dlopened already. */
static int check_dependencies(struct fd_ext_info * ext) 
{
	int i = 1;
	TRACE_ENTRY( "%p", ext );
	
	/* Attempt to resolve the dependency array */
	ext->depends = dlsym( ext->handler, "fd_ext_depends" );
	if (!ext->depends) {
		/* Duplicate the filename */
		char * tmp = strdup(ext->filename);
		ext->ext_name = strdup(basename(tmp));
		free(tmp);
		ext->free_ext_name = 1;
		TRACE_DEBUG(FULL, "Old extension's [%s] API: missing dependencies (ignored)", ext->ext_name);
		return 0;
	}
	
	ext->ext_name = (char *)ext->depends[0];
	
	TRACE_DEBUG(FULL, "Checking dependencies for '%s'...", ext->ext_name);
	
	while (ext->depends[i]) {
		struct fd_list * li;
		for (li = ext_list.next; li != &ext->chain; li = li->next)
		{
			struct fd_ext_info * e = (struct fd_ext_info *)li;
			if (!strcasecmp(e->ext_name, ext->depends[i])) {
				/* the dependency was already loaded */
				break;
			}
		}
		
		if (li == &ext->chain) {
			/* the dependency was not found */
			LOG_F("Error: extension [%s] depends on [%s] which was not loaded first. Please fix your configuration file.",
				ext->ext_name, ext->depends[i]);
			return ESRCH;
		}
		
		i++;
	}

	/* All dependencies resolved successfully */
	return 0;
}

/* Load all extensions in the list */
int fd_ext_load()
{
	int ret;
	int (*fd_ext_init)(int, int, char *) = NULL;
	struct fd_list * li;
	
	TRACE_ENTRY();
	
	/* Loop on all extensions */
	for (li = ext_list.next; li != &ext_list; li = li->next)
	{
		struct fd_ext_info * ext = (struct fd_ext_info *)li;
		LOG_D( "Loading : %s", ext->filename);
		
		/* Load the extension */
#ifndef DEBUG
		ext->handler = dlopen(ext->filename, RTLD_LAZY | RTLD_GLOBAL);
#else /* DEBUG */
		/* We resolve symbols immediatly so it's easier to find problems in ABI */
		ext->handler = dlopen(ext->filename, RTLD_NOW | RTLD_GLOBAL);
#endif /* DEBUG */
		if (ext->handler == NULL) {
			/* An error occured */
			LOG_F("Loading of extension %s failed: %s", ext->filename, dlerror());
			ext->handler = dlopen(ext->filename, RTLD_LAZY | RTLD_GLOBAL);
			if (ext->handler) {
				if (!check_dependencies(ext)) {
					LOG_F("In addition, not all declared dependencies are satisfied (Internal Error!)");
				}
			}
			return EINVAL;
		}
		
		/* Check if declared dependencies are satisfied. */
		CHECK_FCT( check_dependencies(ext) );
		
		/* Resolve the entry point of the extension */
		fd_ext_init = ( int (*) (int, int, char *) )dlsym( ext->handler, "fd_ext_init" );
		
		if (fd_ext_init == NULL) {
			/* An error occured */
			TRACE_ERROR("Unable to resolve symbol 'fd_ext_init' for extension %s: %s", ext->filename, dlerror());
			return EINVAL;
		}
		
		/* Resolve the exit point of the extension, which is optional for extensions */
		ext->fini = ( void (*) (void) )dlsym( ext->handler, "fd_ext_fini" );
		
		if (ext->fini == NULL) {
			/* Not provided */
			TRACE_DEBUG (FULL, "Extension [%s] has no fd_ext_fini function.", ext->filename);
		} else {
			/* Provided */
			TRACE_DEBUG (FULL, "Extension [%s] fd_ext_fini has been resolved successfully.", ext->filename);
		}
		
		/* Now call the entry point to initialize the extension */
		ret = (*fd_ext_init)( FD_PROJECT_VERSION_MAJOR, FD_PROJECT_VERSION_MINOR, ext->conffile );
		if (ret != 0) {
			/* The extension was unable to load cleanly */
			TRACE_ERROR("Extension %s returned an error during initialization: %s", ext->filename, strerror(ret));
			return ret;
		}
		
		/* Proceed to the next extension */
	}

	LOG_N("All extensions loaded.");
	
	/* We have finished. */
	return 0;
}

/* Now unload the extensions and free the memory */
int fd_ext_term( void ) 
{
	TRACE_ENTRY();
	
	/* Loop on all extensions, in FIFO order */
	while (!FD_IS_LIST_EMPTY(&ext_list))
	{
		struct fd_list * li = ext_list.next;
		struct fd_ext_info * ext = (struct fd_ext_info *)li;
	
		/* Unlink this element from the list */
		fd_list_unlink(li);
		
		/* Call the exit point of the extension, if it was resolved */
		if (ext->fini != NULL) {
			TRACE_DEBUG (FULL, "Calling [%s]->fd_ext_fini function.", ext->ext_name ?: ext->filename);
			(*ext->fini)();
		}
		
#ifndef SKIP_DLCLOSE
		/* Now unload the extension */
		if (ext->handler) {
			TRACE_DEBUG (FULL, "Unloading %s", ext->ext_name ?: ext->filename);
			if ( dlclose(ext->handler) != 0 ) {
				TRACE_DEBUG (INFO, "Unloading [%s] failed : %s", ext->ext_name ?: ext->filename, dlerror());
			}
		}
#endif /* SKIP_DLCLOSE */
		
		/* Free the object and continue */
		if (ext->free_ext_name)
			free(ext->ext_name);
		free(ext->filename);
		free(ext->conffile);
		free(ext);
	}
	
	/* We always return 0 since we would not handle an error anyway... */
	return 0;
}

