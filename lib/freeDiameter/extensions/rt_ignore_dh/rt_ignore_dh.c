/*********************************************************************************************************
* Software License Agreement (BSD License)                                                               *
* Author: Thomas Klausner <tk@giga.or.at>                                                                *
*                                                                                                        *
* Copyright (c) 2013, Thomas Klausner                                                                    *
* All rights reserved.                                                                                   *
*                                                                                                        *
* Written under contract by nfotex IT GmbH, http://nfotex.com/                                           *
*                                                                                                        *
* Redistribution and use of this software in source and binary forms, with or without modification, are  *
* permitted provided that the following conditions are met:                                              *
*                                                                                                        *
* * Redistributions of source code must retain the above                                                 *
*   copyright notice, this list of conditions and the                                                    *
*   following disclaimer.                                                                                *
*                                                                                                        *
* * Redistributions in binary form must reproduce the above                                              *
*   copyright notice, this list of conditions and the                                                    *
*   following disclaimer in the documentation and/or other                                               *
*   materials provided with the distribution.                                                            *
*                                                                                                        *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED *
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A *
* PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR *
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT     *
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS    *
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR *
* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF   *
* ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                             *
*********************************************************************************************************/

#include <freeDiameter/extension.h>

/* 
 * Remove Destination-Hosts, putting it into Proxy-Info, and restore it to
 * Origin-Host for answers.
 */

struct dict_object * dh_avp_do; /* cache the Destination-Host dictionary object */
struct dict_object * oh_avp_do; /* cache the Origin-Host dictionary object */
struct dict_object * ph_avp_do; /* cache the Proxy-Host dictionary object */
struct dict_object * pi_avp_do; /* cache the Proxy-Info dictionary object */
struct dict_object * ps_avp_do; /* cache the Proxy-State dictionary object */

static void *memdup(void *data, size_t len)
{
	void *mem;
	if ((mem=malloc(len)) == NULL)
		return NULL;
	memcpy(mem, data, len);
	return mem;
}

static int restore_origin_host(struct msg **msg) {
	struct avp *avp, *child;
	struct avp *oh_avp = NULL;
	struct avp *pi_avp = NULL;
	void *ps, *new_oh;
	size_t ps_len, new_oh_len = 0;
	union avp_value val;

	ps = new_oh = NULL;

	CHECK_FCT(fd_msg_browse(*msg, MSG_BRW_FIRST_CHILD, &avp, NULL));
	/* look for Origin-Host and Proxy-Info matching this host */
	while (avp && (!oh_avp || !pi_avp)) {
		struct avp_hdr * ahdr;
		int match = 0;
		
		CHECK_FCT(fd_msg_avp_hdr(avp, &ahdr));
		if (!(ahdr->avp_flags & AVP_FLAG_VENDOR)) {
			switch (ahdr->avp_code) {
			case AC_ORIGIN_HOST:
				oh_avp = avp;
				CHECK_FCT(fd_msg_parse_dict(oh_avp, fd_g_config->cnf_dict, NULL));
				break;
			case AC_PROXY_INFO:
				ps = NULL;
				ps_len = 0;
				CHECK_FCT(fd_msg_parse_dict(avp, fd_g_config->cnf_dict, NULL));
				CHECK_FCT(fd_msg_browse(avp, MSG_BRW_FIRST_CHILD, &child, NULL));
				while (child && (!match || !ps)) {
					struct avp_hdr *chdr;
					CHECK_FCT(fd_msg_avp_hdr(child, &chdr));
					if (!(chdr->avp_flags & AVP_FLAG_VENDOR)) {
						switch (chdr->avp_code) {
						case AC_PROXY_HOST:
							if (fd_os_cmp(chdr->avp_value->os.data, chdr->avp_value->os.len, 
								      fd_g_config->cnf_diamid, fd_g_config->cnf_diamid_len) == 0) {
								match = 1;
							}
							break;
						case AC_PROXY_STATE:
							ps_len = chdr->avp_value->os.len;
							ps = memdup(chdr->avp_value->os.data, ps_len);
							if (!ps) {
								TRACE_ERROR("malloc failure");
								return 0;
							}
							break;
						default:
							break;
						}
					}
					CHECK_FCT(fd_msg_browse(child, MSG_BRW_NEXT, &child, NULL));
				}
				if (match && ps) {
					new_oh = ps;
					new_oh_len = ps_len;
					pi_avp = avp;
				} else
					free(ps);
				break;
			default:
				break;
			}
		}
		CHECK_FCT(fd_msg_browse(avp, MSG_BRW_NEXT, &avp, NULL));
	}
	if (!pi_avp)
		return 0;

	memset(&val, 0, sizeof(val));
	val.os.data = new_oh;
	val.os.len = new_oh_len;
	if (!oh_avp) {
		TRACE_ERROR("Message contained no Origin-Host");
	} else {
		CHECK_FCT(fd_msg_avp_setvalue(oh_avp, &val));
	}
	fd_msg_free((msg_or_avp*)pi_avp);

	fd_log_debug("Restored Origin-Host '%.*s' from Proxy-Info", (int)new_oh_len, (char *)new_oh);
	return 0;
}

static int stow_destination_host(struct msg **msg) {
	struct avp * avp = NULL;
	struct avp * ph_avp = NULL;
	struct avp * pi_avp = NULL;
	struct avp * ps_avp = NULL;

	/* Look for the Destination-Host AVP in the message */
	CHECK_FCT(fd_msg_search_avp(*msg, dh_avp_do, &avp));
	if (avp != NULL) {
		struct avp_hdr * ahdr = NULL;
		union avp_value val;

		CHECK_FCT(fd_msg_avp_hdr(avp, &ahdr));
		if (ahdr->avp_value != NULL) {
			/* add Proxy-Info->{Proxy-Host, Proxy-State} using Destination-Host information */
			CHECK_FCT(fd_msg_avp_new(ph_avp_do, 0, &ph_avp));
			memset(&val, 0, sizeof(val));
                        val.os.data = memdup(fd_g_config->cnf_diamid, fd_g_config->cnf_diamid_len);
			val.os.len = fd_g_config->cnf_diamid_len;
			CHECK_FCT(fd_msg_avp_setvalue(ph_avp, &val));

			CHECK_FCT(fd_msg_avp_new(ps_avp_do, 0, &ps_avp));
			memset(&val, 0, sizeof(val));
			val.os.data = memdup(ahdr->avp_value->os.data, ahdr->avp_value->os.len);
			val.os.len = ahdr->avp_value->os.len;
			CHECK_FCT(fd_msg_avp_setvalue(ps_avp, &val));

			CHECK_FCT(fd_msg_avp_new(pi_avp_do, 0, &pi_avp));
			CHECK_FCT(fd_msg_avp_add(pi_avp, MSG_BRW_LAST_CHILD, ph_avp));
			CHECK_FCT(fd_msg_avp_add(pi_avp, MSG_BRW_LAST_CHILD, ps_avp));

			/* remove Destination-Host from message */
			fd_msg_free((msg_or_avp*)avp);
			/* add Proxy-Info */
			CHECK_FCT(fd_msg_avp_add(*msg, MSG_BRW_LAST_CHILD, pi_avp));
			fd_log_debug("Stowed Destination-Host '%.*s' into Proxy-Info", (int)val.os.len, (const char *)val.os.data);
		}
	}

	return 0;
}

/* The callback for putting Destination-Host into Proxy-Info and restoring it on the way back */
static int rt_ignore_destination_host(void * cbdata, struct msg **msg) {
	struct msg_hdr * hdr;
	int ret;

	TRACE_ENTRY("%p %p", cbdata, msg);

	CHECK_PARAMS(msg && *msg);

	/* Read the message header */
	CHECK_FCT(fd_msg_hdr(*msg, &hdr));
	if (hdr->msg_flags & CMD_FLAG_REQUEST) {
		ret = stow_destination_host(msg);
	} else {
		ret = restore_origin_host(msg);
	}

	return ret;
}

/* handler */
static struct fd_rt_fwd_hdl * rt_ignore_destination_host_hdl = NULL;

/* entry point */
static int rt_ignore_destination_host_entry(char * conffile)
{
	CHECK_FCT_DO(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Destination-Host", &dh_avp_do, ENOENT),
		     { TRACE_ERROR("Unable to find 'Destination-Host' AVP in the loaded dictionaries."); });
	CHECK_FCT_DO(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Origin-Host", &oh_avp_do, ENOENT),
		     { TRACE_ERROR("Unable to find 'Origin-Host' AVP in the loaded dictionaries."); });
	CHECK_FCT_DO(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Proxy-Host", &ph_avp_do, ENOENT),
		     { TRACE_ERROR("Unable to find 'Proxy-Host' AVP in the loaded dictionaries."); });
	CHECK_FCT_DO(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Proxy-Info", &pi_avp_do, ENOENT),
		     { TRACE_ERROR("Unable to find 'Proxy-Info' AVP in the loaded dictionaries."); });
	CHECK_FCT_DO(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Proxy-State", &ps_avp_do, ENOENT),
		     { TRACE_ERROR("Unable to find 'Proxy-State' AVP in the loaded dictionaries."); });

	/* Register the callback */
	CHECK_FCT(fd_rt_fwd_register(rt_ignore_destination_host, NULL, RT_FWD_ALL, &rt_ignore_destination_host_hdl));

	TRACE_DEBUG(INFO, "Extension 'Ignore Destination Host' initialized");
	return 0;
}

/* Unload */
void fd_ext_fini(void)
{
	/* Unregister the callbacks */
	CHECK_FCT_DO(fd_rt_fwd_unregister(rt_ignore_destination_host_hdl, NULL), /* continue */);
	return ;
}

EXTENSION_ENTRY("rt_ignore_destination_host", rt_ignore_destination_host_entry);
