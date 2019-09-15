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
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************************************/


#ifndef _PLUGINS_H
#define _PLUGINS_H

#include "libdiameap.h"
#include "libcrypt.h"




/* Macro that define the register functions of an EAP method */
#define REGISTER_METHOD(_methodName, _configFunction, _initFunction, _initPickUpFunction, _buildReqFunction, _getTimeoutFunction, _checkFunction, _processFunction, _isDoneFunction, _getKeyFunction, _unregisterFunction, _datafreeFunction  ) \
static struct register_plugin *registerplugin = NULL;	\
static int isregistered = 0;	\
int diameap_plugin_register() {	\
		if (!isregistered){	\
			registerplugin = malloc (sizeof(struct register_plugin)); \
			if(registerplugin==NULL) \
				fprintf(stderr,"[DiamEAP extension] Error in registering plug-in\t%s\n",strerror(errno)); \
			memset(registerplugin, 0, sizeof(struct register_plugin)); \
			if(_configFunction) registerplugin->configure=_configFunction;	\
			if(_initFunction) registerplugin->init=_initFunction;	\
			if(_initPickUpFunction) registerplugin->initPickUp=_initPickUpFunction;	\
			if(_buildReqFunction) registerplugin->buildReq=_buildReqFunction;	\
			if(_getTimeoutFunction) registerplugin->getTimeout=_getTimeoutFunction;	\
			if(_checkFunction) registerplugin->check=_checkFunction;	\
			if(_processFunction) registerplugin->process=_processFunction;	\
			if(_isDoneFunction) registerplugin->isDone=_isDoneFunction;	\
			if(_getKeyFunction) registerplugin->getKey=_getKeyFunction;	\
			if(_unregisterFunction) registerplugin->unregister=_unregisterFunction;	\
			if(_datafreeFunction) registerplugin->datafree=_datafreeFunction;	\
			isregistered++; \
		}else{ \
			 fprintf(stderr, "Cannot register the " _methodName " plugin twice\n");	\
			return EINVAL; \
		} \
		return 0; \
	}	\
int diameap_plugin_objects(struct register_plugin ** rplugin){ \
*rplugin=registerplugin; \
return 0; \
} \


#endif /*_PLUGINS_H*/
