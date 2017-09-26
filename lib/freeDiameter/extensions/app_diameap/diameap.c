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

#include "diameap_common.h"



/* DiamEAP Configuration */
static struct diameap_conf conf;
struct diameap_conf * diameap_config = &conf;

/* The entry point */
static int diameap_main(char * conffile)
{
	TRACE_ENTRY("%p", conffile);

	memset(diameap_config, 0, sizeof(struct diameap_conf));

	/* Initialize configuration */
	CHECK_FCT(diameap_init(conffile));

	/* Start Diameter EAP Application (Back-end Authenticator ) */
	CHECK_FCT(diameap_start_server());

	/* Announce the support of Diameter EAP Application to other peers */
	CHECK_FCT(fd_disp_app_support(dataobj_diameap_app, dataobj_diameap_ven, 1, 0));

	LOG_D("%sDiameter EAP Application Extension started successfully.",DIAMEAP_EXTENSION);

	return 0;
}

void fd_ext_fini(void)
{
	TRACE_DEBUG(FULL,"%sStopping the server",DIAMEAP_EXTENSION);
	CHECK_FCT_DO(diameap_stop_server(),
			{	TRACE_DEBUG(INFO,"%sStopping the server: Error occurred.",DIAMEAP_EXTENSION);});

	if (db_conn)
	{
		TRACE_DEBUG(FULL,"%sDisconnecting from MySQL Server",DIAMEAP_EXTENSION);
		diameap_mysql_disconnect();
	}

	TRACE_DEBUG(FULL,"%sUnloading EAP Methods plug-ins: Error occurred.",DIAMEAP_EXTENSION);
	CHECK_FCT_DO(diameap_plugin_unload(),
			{	TRACE_DEBUG(INFO,"%sUnloading EAP Methods plug-ins: Error occurred.",DIAMEAP_EXTENSION);});
	return;
}

/* Define the entry point */
EXTENSION_ENTRY("DiamEAP", diameap_main, "dict_eap")
;
