/*****************************************************************************************************
 * Software License Agreement (BSD License)
 * Author : Souheil Ben Ayed <souheil@tera.ics.keio.ac.jp>
 *
 * Copyright (c) 2009-2010, Souheil Ben Ayed, Teraoka Laboratory of Keio University, and the WIDE Project
 * All rights reserved.
 *
 * Redistribution and use of this software in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by Souheil Ben Ayed <souheil@tera.ics.keio.ac.jp>.
 *
 * 4. Neither the name of Souheil Ben Ayed, Teraoka Laboratory of Keio University or the WIDE Project nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTfORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************************************/

#include "diameap_common.h"

/********************************************/
/*	Loading and initializing plug-ins		*/
/********************************************/

/* Add new plugin */
int diameap_plugin_add(char * name, eap_type methodtype, u32 vendor,
		char * filename, char * conffile)
{
	TRACE_ENTRY("%p %d %d %p %p", name, methodtype, vendor, filename, conffile);

	/* Check the filename is valid */
	CHECK_PARAMS(filename);

	/* add new EAP method  in the list of plugins*/
	{
		struct plugin * plugin;
		CHECK_MALLOC(plugin = malloc(sizeof(struct plugin)));
		memset(plugin, 0, sizeof(struct plugin));
		fd_list_init(&plugin->chain, plugin);
		plugin->methodname = name;
		plugin->methodtype = methodtype;
		plugin->vendor = vendor;
		plugin->pluginfile = filename;
		plugin->conffile = conffile;
		fd_list_insert_before(&plugins_list, &plugin->chain);
	}

	return 0;
}

/* Dump plugins list */
void diameap_plugin_dump(void)
{
	struct fd_list * pl;

	for (pl = plugins_list.next; pl != &plugins_list; pl = pl->next)
	{
		struct plugin * plugin = (struct plugin *) pl;
		fd_log_debug("\t\t - %s plugin \t [Type: %i, Vendor: %i]  %sloaded", plugin->methodname,
				plugin->methodtype,plugin->vendor, plugin->handler ? "" : "not ");
	}
}

int diameap_plugin_get(u32 vendor, eap_type type, struct plugin ** getplugin)
{
	TRACE_ENTRY("%d %d %p",vendor,type,getplugin);
	struct fd_list * pl;

	for (pl = plugins_list.next; pl != &plugins_list; pl = pl->next)
	{
		struct plugin * plugin = (struct plugin *) pl;
		if (plugin->methodtype == type && plugin->vendor == vendor)
		{
			*getplugin = plugin;
			return 0;
		}
	}
	return 1;
}

/* Load all method in the plugins list */
int diameap_plugin_load(void)
{

	int ret;
	struct fd_list * pl;

	/* Loop on all plugins */

	for (pl = plugins_list.next; pl != &plugins_list; pl = pl->next)
	{
		struct plugin * plugin = (struct plugin *) pl;
		struct register_plugin * registerplugin;
		int (*diameap_plugin_register)();

		TRACE_DEBUG(FULL, "%sLoading EAP method plugin: %s",DIAMEAP_EXTENSION, plugin->methodname);

		/* Load the method */
		plugin->handler = dlopen(plugin->pluginfile, RTLD_LAZY | RTLD_GLOBAL);
		if (plugin->handler == NULL)
		{
			TRACE_DEBUG(INFO, "%sLoading of plugin %s failed: %s",DIAMEAP_EXTENSION,
					plugin->methodname, dlerror());
			return EINVAL;
		}

		/* Defined register methods for this EAP method*/
		diameap_plugin_register = (int(*)()) dlsym(plugin->handler,
				"diameap_plugin_register");
		if (!diameap_plugin_register)
		{
			TRACE_DEBUG(INFO,
					"%s[%s plugin] Unable to register EAP method: %s.",DIAMEAP_EXTENSION,
					plugin->methodname, dlerror());
			return EINVAL;
		}

		if ((*diameap_plugin_register)() != 0)
		{
			TRACE_DEBUG(INFO,
					"%s[%s plugin] Unable to register EAP method plugin",DIAMEAP_EXTENSION,plugin->methodname);
		}

		int (*diameap_plugin_objects)(struct register_plugin **);

		diameap_plugin_objects = (int(*)(struct register_plugin **)) dlsym(
				plugin->handler, "diameap_plugin_objects");
		if (!diameap_plugin_objects)
		{
			TRACE_DEBUG(INFO,
					"%s[%s plugin] Unable to resolve symbol of the plugin: %s",DIAMEAP_EXTENSION,
					plugin->methodname, dlerror());
			return EINVAL;
		}

		if ((*diameap_plugin_objects)(&registerplugin) != 0)
		{

			TRACE_DEBUG(
					INFO,
					"%s[%s plugin] Unable to get objects description from the plug-in: %s",DIAMEAP_EXTENSION,
					plugin->methodname, dlerror());
			return EINVAL;
		}

		/* eap_method_configure method */
		if (registerplugin->configure)
		{
			plugin->eap_method_configure = (int(*)(char *)) dlsym(
					plugin->handler, registerplugin->configure);
			if (plugin->eap_method_configure == NULL)
			{
				TRACE_DEBUG(
						INFO,
						"%s[%s plugin] Unable to resolve symbol for 'eap_method_configure': %s",DIAMEAP_EXTENSION,
						plugin->methodname, dlerror());
			}
		}
		else
		{
			plugin->eap_method_configure = NULL;
			TRACE_DEBUG(FULL+1,"%s[%s plugin] Unavailable function 'eap_method_configure'",DIAMEAP_EXTENSION, plugin->methodname);
		}

		/* eap_method_init method */
		if (registerplugin->init)
		{
			plugin->eap_method_init
					= (int(*)(struct eap_state_machine *)) dlsym(
							plugin->handler, registerplugin->init);
			if (plugin->eap_method_init == NULL)
			{
				TRACE_DEBUG(
						INFO,
						"%s[%s plugin] Unable to resolve symbol for 'eap_method_init': %s",DIAMEAP_EXTENSION,
						plugin->methodname, dlerror());
				return EINVAL;
			}
		}
		else
		{
			plugin->eap_method_init = NULL;
			TRACE_DEBUG(INFO,"%s[%s plugin] Unavailable function 'eap_method_init'",DIAMEAP_EXTENSION, plugin->methodname);
			return EINVAL;
		}

		/* eap_method_initPickUp method */
		if (registerplugin->initPickUp)
		{
			plugin->eap_method_initPickUp
					= (int(*)(struct eap_state_machine *)) dlsym(
							plugin->handler, registerplugin->initPickUp);
		}
		else
		{
			plugin->eap_method_initPickUp = NULL;
			TRACE_DEBUG(FULL+1,"%s[%s plugin] Unavailable function 'eap_method_initPickUp'",DIAMEAP_EXTENSION, plugin->methodname);
		}

		/* eap_method_buildReq method */
		if (registerplugin->buildReq)
		{
			plugin->eap_method_buildReq = (int(*)(struct eap_state_machine *,
					u8, struct eap_packet *)) dlsym(plugin->handler,
					registerplugin->buildReq);
			if (plugin->eap_method_buildReq == NULL)
			{
				TRACE_DEBUG(
						INFO,
						"%s[%s plugin] Unable to resolve symbol for 'eap_method_buildReq': %s",DIAMEAP_EXTENSION,
						plugin->methodname, dlerror());
				return EINVAL;
			}
		}
		else
		{
			plugin->eap_method_buildReq = NULL;
			TRACE_DEBUG(INFO,"%s[%s plugin] Unavailable function 'eap_method_buildReq'",DIAMEAP_EXTENSION, plugin->methodname);
			return EINVAL;
		}

		/* eap_method_getTimeout method */
		if (registerplugin->getTimeout)
		{
			plugin->eap_method_getTimeout = (int(*)(struct eap_state_machine *,
					int *)) dlsym(plugin->handler, registerplugin->getTimeout);
			if (plugin->eap_method_getTimeout == NULL)
			{
				TRACE_DEBUG(
						INFO,
						"%s[%s plugin] Unable to resolve symbol for 'eap_method_getTimeout': %s",DIAMEAP_EXTENSION,
						plugin->methodname, dlerror());
				return EINVAL;
			}
		}
		else
		{
			plugin->eap_method_getTimeout = NULL;
			TRACE_DEBUG(FULL+1,"%s[%s plugin] Unavailable function  'eap_method_getTimeout'",DIAMEAP_EXTENSION, plugin->methodname);
		}

		/* eap_method_check method */
		if (registerplugin->check)
		{
			plugin->eap_method_check = (boolean(*)(struct eap_state_machine *,
					struct eap_packet*)) dlsym(plugin->handler,
					registerplugin->check);
			if (plugin->eap_method_check == NULL)
			{
				TRACE_DEBUG(
						INFO,
						"%s[%s plugin] Unable to resolve symbol for 'eap_method_check': %s",DIAMEAP_EXTENSION,
						plugin->methodname, dlerror());
				return EINVAL;
			}
		}
		else
		{
			plugin->eap_method_check = NULL;
			TRACE_DEBUG(INFO,"%s[%s plugin] Unavailable function 'eap_method_check'",DIAMEAP_EXTENSION, plugin->methodname);
			return EINVAL;
		}

		/* eap_method_process method */
		if (registerplugin->process)
		{
			plugin->eap_method_process = (int(*)(struct eap_state_machine *,
					struct eap_packet*)) dlsym(plugin->handler,
					registerplugin->process);
			if (plugin->eap_method_process == NULL)
			{
				TRACE_DEBUG(
						INFO,
						"%s[%s plugin] Unable to resolve symbol for 'eap_method_process': %s",DIAMEAP_EXTENSION,
						plugin->methodname, dlerror());
				return EINVAL;
			}
		}
		else
		{
			plugin->eap_method_process = NULL;
			TRACE_DEBUG(INFO,"%s[%s plugin] Unavailable function 'eap_method_process'",DIAMEAP_EXTENSION, plugin->methodname);
			return EINVAL;
		}

		/* eap_method_isDone method */
		if (registerplugin->isDone)
		{
			plugin->eap_method_isDone
					= (boolean(*)(struct eap_state_machine *)) dlsym(
							plugin->handler, registerplugin->isDone);
			if (plugin->eap_method_isDone == NULL)
			{
				TRACE_DEBUG(
						INFO,
						"%s[%s plugin] Unable to resolve symbol for 'eap_method_isDone': %s",DIAMEAP_EXTENSION,
						plugin->methodname, dlerror());
				return EINVAL;
			}
		}
		else
		{
			plugin->eap_method_isDone = NULL;
			TRACE_DEBUG(INFO,"%s[%s plugin] Unavailable function 'eap_method_isDone'",DIAMEAP_EXTENSION, plugin->methodname);
			return EINVAL;
		}

		/* eap_method_getKey method */
		if (registerplugin->getKey)
		{
			plugin->eap_method_getKey = (int(*)(struct eap_state_machine *,
					u8**, int*,u8**, int*)) dlsym(plugin->handler, registerplugin->getKey);
			if (plugin->eap_method_getKey == NULL)
			{
				TRACE_DEBUG(
						INFO,
						"%s[%s plugin] Unable to resolve symbol for 'eap_method_getKey': %s",DIAMEAP_EXTENSION,
						plugin->methodname, dlerror());
				return EINVAL;
			}
		}
		else
		{
			plugin->eap_method_getKey = NULL;
			TRACE_DEBUG(FULL+1,"%s[%s plugin] Unavailable function 'eap_method_getKey'",DIAMEAP_EXTENSION, plugin->methodname);
		}

		/* eap_method_unregister method */
		if (registerplugin->unregister)
		{
			plugin->eap_method_unregister = (void(*)(void)) dlsym(
					plugin->handler, registerplugin->unregister);
			if (plugin->eap_method_unregister == NULL)
			{
				TRACE_DEBUG(
						INFO,
						"%s[%s plugin] Unable to resolve symbol for 'eap_method_unregister': %s",DIAMEAP_EXTENSION,
						plugin->methodname, dlerror());
				return EINVAL;
			}
		}
		else
		{
			plugin->eap_method_unregister = NULL;
			TRACE_DEBUG(FULL+1,"%s[%s plugin] Unavailable function 'eap_method_unregister'",DIAMEAP_EXTENSION, plugin->methodname);
		}

		/* eap_method_datafree method */
		if (registerplugin->datafree)
		{
			plugin->eap_method_free = (void(*)(void *)) dlsym(plugin->handler,
					registerplugin->datafree);
			if (plugin->eap_method_free == NULL)
			{
				TRACE_DEBUG(
						INFO,
						"%s[%s plugin] Unable to resolve symbol for 'eap_method_datafree': %s",DIAMEAP_EXTENSION,
						plugin->methodname, dlerror());
				return EINVAL;
			}
		}
		else
		{
			plugin->eap_method_free = NULL;
			TRACE_DEBUG(FULL+1,"%s[%s plugin] Unavailable function 'eap_method_datafree'",DIAMEAP_EXTENSION, plugin->methodname);
		}

		if (plugin->eap_method_configure != NULL)
		{
			/* call the configuration method */
			ret = (*plugin->eap_method_configure)(plugin->conffile);
			if (ret != 0)
			{
				TRACE_DEBUG(
						INFO,
						"%s[%s plugin] Unable to configure the plugin",DIAMEAP_EXTENSION,
						plugin->methodname);
				return ret;
			}
		}

		TRACE_DEBUG(FULL, "%s[%s plugin] Loaded successfully.",DIAMEAP_EXTENSION,
				plugin->methodname);

		/* load next method */
	}

	return 0;
}

boolean diameap_plugin_exist(u32 vendor, eap_type type)
{

	TRACE_ENTRY("%d %d",vendor,type);
	struct fd_list * pl;

	for (pl = plugins_list.next; pl != &plugins_list; pl = pl->next)
	{
		struct plugin * plugin = (struct plugin *) pl;
		if (plugin->methodtype == type && plugin->vendor == vendor)
		{
			return TRUE;
		}
	}
	return FALSE;
}

int diameap_plugin_unload(void)
{

	while (!FD_IS_LIST_EMPTY(&plugins_list))
	{
		struct fd_list * plugin = plugins_list.next;
		struct plugin * item = (struct plugin *) plugin;

		fd_list_unlink(plugin);

		if (item->eap_method_unregister != NULL)
		{
			(*item->eap_method_unregister)();
		}

		if (item->handler)
		{
			if (dlclose(item->handler) != 0)
			{
				TRACE_DEBUG (INFO, "%sFail to unload plugin %s : %s",DIAMEAP_EXTENSION, item->methodname, dlerror());
			}
		}

		free(item->conffile);
		free(item->pluginfile);
		free(item);
	}
	return 0;
}
