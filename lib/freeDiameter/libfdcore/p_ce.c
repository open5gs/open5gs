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

/* This file contains code to handle Capabilities Exchange messages (CER and CEA) and election process */

/* Save a connection as peer's principal */
static int set_peer_cnx(struct fd_peer * peer, struct cnxctx **cnx)
{
	CHECK_PARAMS( peer->p_cnxctx == NULL );
	
	/* Save the connection in peer */
	peer->p_cnxctx = *cnx;
	*cnx = NULL;
	
	/* Set the events to be sent to the PSM */
	CHECK_FCT( fd_cnx_recv_setaltfifo(peer->p_cnxctx, peer->p_events) );
	
	/* Read the credentials if possible */
	if (fd_cnx_getTLS(peer->p_cnxctx)) {
		CHECK_FCT( fd_cnx_getcred(peer->p_cnxctx, &peer->p_hdr.info.runtime.pir_cert_list, &peer->p_hdr.info.runtime.pir_cert_list_size) );
	}
	
	/* Read the endpoints, maybe used to reconnect to the peer later */
	CHECK_FCT( fd_cnx_getremoteeps(peer->p_cnxctx, &peer->p_hdr.info.pi_endpoints) );
	
	/* Read the protocol */
	peer->p_hdr.info.runtime.pir_proto = fd_cnx_getproto(peer->p_cnxctx);
	
	return 0;
}

/* Delete the peer connection, and cleanup associated information */
void fd_p_ce_clear_cnx(struct fd_peer * peer, struct cnxctx ** cnx_kept)
{
	peer->p_hdr.info.runtime.pir_cert_list = NULL;
	peer->p_hdr.info.runtime.pir_cert_list_size = 0;
	peer->p_hdr.info.runtime.pir_proto = 0;
	
	if (peer->p_cnxctx) {
		if (cnx_kept != NULL) {
			*cnx_kept = peer->p_cnxctx;
		} else {
			fd_cnx_destroy(peer->p_cnxctx);
		}
		peer->p_cnxctx = NULL;
	}
}

/* Election: compare the Diameter Ids by lexical order, return true if the election is won */
static __inline__ int election_result(struct fd_peer * peer, struct msg * cer)
{
	/* Default to winning the election.  We only hit this case if the peer's CER is malformed. */
	int ret = 1;

	/* Find the Origin-Host AVP in the CER */
	struct avp * avp = NULL;
	CHECK_FCT( fd_msg_browse( cer, MSG_BRW_FIRST_CHILD, &avp, NULL) );
	while (avp) {
		struct avp_hdr * hdr;
		CHECK_FCT(  fd_msg_avp_hdr( avp, &hdr )  );
		if ((!(hdr->avp_flags & AVP_FLAG_VENDOR)) && (hdr->avp_code == AC_ORIGIN_HOST))
		{
			/* Found the Origin-Host.  Compare it with the configured identity. */
			ret = (strncasecmp(hdr->avp_value->os.data, fd_g_config->cnf_diamid, hdr->avp_value->os.len) < 0);
			break;
		}
	}
	if (ret) {
		TRACE_DEBUG(INFO, "Election WON against peer '%s'", peer->p_hdr.info.pi_diamid);
	} else {
		TRACE_DEBUG(INFO, "Election LOST against peer '%s'", peer->p_hdr.info.pi_diamid);
	}
	return ret;
}

/* Add AVPs about local information in a CER or CEA */
static int add_CE_info(struct msg *msg, struct cnxctx * cnx, int isi_tls, int isi_none)
{
	struct dict_object * dictobj = NULL;
	struct avp * avp = NULL;
	union avp_value val;
	struct fd_list *li;
	
	/* Add the Origin-* AVPs */
	CHECK_FCT( fd_msg_add_origin ( msg, 1 ) );
	
	/* Find the model for Host-IP-Address AVP */
	CHECK_FCT(  fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Host-IP-Address", &dictobj, ENOENT )  );
		
	/* Add the AVP(s) -- not sure what is the purpose... We could probably only add the primary one ? */
	for (li = fd_g_config->cnf_endpoints.next; li != &fd_g_config->cnf_endpoints; li = li->next) {
		struct fd_endpoint * ep = (struct fd_endpoint *)li;
		CHECK_FCT( fd_msg_avp_new ( dictobj, 0, &avp ) );
		CHECK_FCT( fd_msg_avp_value_encode ( &ep->ss, avp ) );
		CHECK_FCT( fd_msg_avp_add( msg, MSG_BRW_LAST_CHILD, avp ) );
	}
	
	/* Vendor-Id, Product-Name, and Firmware-Revision AVPs */
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Vendor-Id", &dictobj, ENOENT )  );
	CHECK_FCT( fd_msg_avp_new ( dictobj, 0, &avp ) );
	val.u32 = MY_VENDOR_ID;
	CHECK_FCT( fd_msg_avp_setvalue( avp, &val ) );
	CHECK_FCT( fd_msg_avp_add( msg, MSG_BRW_LAST_CHILD, avp ) );
	
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Product-Name", &dictobj, ENOENT )  );
	CHECK_FCT( fd_msg_avp_new ( dictobj, 0, &avp ) );
	val.os.data = (unsigned char *)FD_PROJECT_NAME;
	val.os.len = strlen(FD_PROJECT_NAME);
	CHECK_FCT( fd_msg_avp_setvalue( avp, &val ) );
	CHECK_FCT( fd_msg_avp_add( msg, MSG_BRW_LAST_CHILD, avp ) );
	
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Firmware-Revision", &dictobj, ENOENT )  );
	CHECK_FCT( fd_msg_avp_new ( dictobj, 0, &avp ) );
	val.u32 = (uint32_t)(FD_PROJECT_VERSION_MAJOR * 10000 + FD_PROJECT_VERSION_MINOR * 100 + FD_PROJECT_VERSION_REV);
	CHECK_FCT( fd_msg_avp_setvalue( avp, &val ) );
	CHECK_FCT( fd_msg_avp_add( msg, MSG_BRW_LAST_CHILD, avp ) );
	
	
	/* Add the Inband-Security-Id AVP if needed */
	if (isi_tls || isi_none) {
		CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Inband-Security-Id", &dictobj, ENOENT )  );
		
		if (isi_none) {
			CHECK_FCT( fd_msg_avp_new ( dictobj, 0, &avp ) );
			val.u32 = ACV_ISI_NO_INBAND_SECURITY;
			CHECK_FCT( fd_msg_avp_setvalue( avp, &val ) );
			CHECK_FCT( fd_msg_avp_add( msg, MSG_BRW_LAST_CHILD, avp ) );
		}
		
		if (isi_tls) {
			CHECK_FCT( fd_msg_avp_new ( dictobj, 0, &avp ) );
			val.u32 = ACV_ISI_TLS;
			CHECK_FCT( fd_msg_avp_setvalue( avp, &val ) );
			CHECK_FCT( fd_msg_avp_add( msg, MSG_BRW_LAST_CHILD, avp ) );
		}
	}
	
	/* List of local applications */
	{
		struct dict_object * dictobj_auth = NULL;
		struct dict_object * dictobj_acct = NULL;
		struct dict_object * dictobj_vid = NULL;
		
		CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Vendor-Specific-Application-Id", &dictobj, ENOENT )  );
		CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Vendor-Id", &dictobj_vid, ENOENT )  );
		CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Auth-Application-Id", &dictobj_auth, ENOENT )  );
		CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Acct-Application-Id", &dictobj_acct, ENOENT )  );
		
		for (li = fd_g_config->cnf_apps.next; li != &fd_g_config->cnf_apps; li = li->next) {
			struct fd_app * a = (struct fd_app *)(li);

			if (a->flags.auth) {
				CHECK_FCT( fd_msg_avp_new ( dictobj_auth, 0, &avp ) );
				val.u32 = a->appid;
				CHECK_FCT( fd_msg_avp_setvalue( avp, &val ) );
				if (a->vndid != 0) {
					struct avp * avp2 = NULL;
					CHECK_FCT( fd_msg_avp_new ( dictobj, 0, &avp2 ) );
					CHECK_FCT( fd_msg_avp_add( avp2, MSG_BRW_LAST_CHILD, avp ) );
					avp = avp2;
					CHECK_FCT( fd_msg_avp_new ( dictobj_vid, 0, &avp2 ) );
					val.u32 = a->vndid;
					CHECK_FCT( fd_msg_avp_setvalue( avp2, &val ) );
					CHECK_FCT( fd_msg_avp_add( avp, MSG_BRW_LAST_CHILD, avp2 ) );
				}
				CHECK_FCT( fd_msg_avp_add( msg, MSG_BRW_LAST_CHILD, avp ) );
			}
			if (a->flags.acct) {
				CHECK_FCT( fd_msg_avp_new ( dictobj_acct, 0, &avp ) );
				val.u32 = a->appid;
				CHECK_FCT( fd_msg_avp_setvalue( avp, &val ) );
				if (a->vndid != 0) {
					struct avp * avp2 = NULL;
					CHECK_FCT( fd_msg_avp_new ( dictobj, 0, &avp2 ) );
					CHECK_FCT( fd_msg_avp_add( avp2, MSG_BRW_LAST_CHILD, avp ) );
					avp = avp2;
					CHECK_FCT( fd_msg_avp_new ( dictobj_vid, 0, &avp2 ) );
					val.u32 = a->vndid;
					CHECK_FCT( fd_msg_avp_setvalue( avp2, &val ) );
					CHECK_FCT( fd_msg_avp_add( avp, MSG_BRW_LAST_CHILD, avp2 ) );
				}
				CHECK_FCT( fd_msg_avp_add( msg, MSG_BRW_LAST_CHILD, avp ) );
			}
		}
		
		/* do not forget the relay application */
		if (! fd_g_config->cnf_flags.no_fwd) {
			CHECK_FCT( fd_msg_avp_new ( dictobj_auth, 0, &avp ) );
			val.u32 = AI_RELAY;
			CHECK_FCT( fd_msg_avp_setvalue( avp, &val ) );
			CHECK_FCT( fd_msg_avp_add( msg, MSG_BRW_LAST_CHILD, avp ) );
		}
	}
	
	/* Add the list of supported vendors */
	{
		uint32_t * array = fd_dict_get_vendorid_list(fd_g_config->cnf_dict);
		if (array) {
			int i = 0;
			CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Supported-Vendor-Id", &dictobj, ENOENT )  );
			
			while (array[i] != 0) {
				CHECK_FCT( fd_msg_avp_new ( dictobj, 0, &avp ) );
				val.u32 = array[i];
				CHECK_FCT( fd_msg_avp_setvalue( avp, &val ) );
				CHECK_FCT( fd_msg_avp_add( msg, MSG_BRW_LAST_CHILD, avp ) );
				i++;
			}
			
			free(array);
		}
	}
	
	return 0;
}

/* Remove any information saved from a previous CER/CEA exchange */
static void cleanup_remote_CE_info(struct fd_peer * peer)
{
	/* free linked information */
	free(peer->p_hdr.info.runtime.pir_host);
	free(peer->p_hdr.info.runtime.pir_realm);
	free(peer->p_hdr.info.runtime.pir_prodname);
	while (!FD_IS_LIST_EMPTY(&peer->p_hdr.info.runtime.pir_apps)) {
		struct fd_list * li = peer->p_hdr.info.runtime.pir_apps.next;
		fd_list_unlink(li);
		free(li);
	}
	/* note: pir_cert_list needs not be freed (belongs to gnutls) */
	
	/* cleanup the area */
	memset(&peer->p_hdr.info.runtime, 0, sizeof(peer->p_hdr.info.runtime));
	
	/* reinit the list */
	fd_list_init(&peer->p_hdr.info.runtime.pir_apps, peer);

	/* Remove previously advertised endpoints */
	fd_ep_clearflags( &peer->p_hdr.info.pi_endpoints, EP_FL_ADV );
}

/* Extract information sent by the remote peer and save it in our peer structure */
static int save_remote_CE_info(struct msg * msg, struct fd_peer * peer, struct fd_pei * error, uint32_t *rc)
{
	struct avp * avp = NULL;
	
	cleanup_remote_CE_info(peer);
	
	CHECK_FCT( fd_msg_browse( msg, MSG_BRW_FIRST_CHILD, &avp, NULL) );
	
	/* Loop on all AVPs and save what we are interrested into */
	while (avp) {
		struct avp_hdr * hdr;

		CHECK_FCT(  fd_msg_avp_hdr( avp, &hdr )  );

		if (hdr->avp_flags & AVP_FLAG_VENDOR) {
			/* Ignore all vendor-specific AVPs in CER/CEA because we don't support any currently */
			LOG_A("Ignored a vendor-specific AVP in CER / CEA");
			goto next;
		}

		switch (hdr->avp_code) {
			case AC_RESULT_CODE: /* Result-Code */
				if (hdr->avp_value == NULL) {
					/* This is a sanity check */
					LOG_F("Ignored an AVP (code %x) with unset value in CER/CEA", hdr->avp_code);
					ASSERT(0); /* To check if this really happens, and understand why... */
					goto next;
				}
				
				if (rc)
					*rc = hdr->avp_value->u32;
				break;
		
			case AC_ORIGIN_HOST: /* Origin-Host */
				if (hdr->avp_value == NULL) {
					/* This is a sanity check */
					LOG_F("Ignored an AVP (code %x) with unset value in CER/CEA", hdr->avp_code);
					ASSERT(0); /* To check if this really happens, and understand why... */
					goto next;
				}
				
				/* In case of multiple AVPs */
				if (peer->p_hdr.info.runtime.pir_host) {
					TRACE_DEBUG(INFO, "Multiple instances of the Origin-Host AVP");
					error->pei_errcode = "DIAMETER_AVP_OCCURS_TOO_MANY_TIMES";
					error->pei_message = "I found several Origin-Host AVPs";
					error->pei_avp = avp;
					return EINVAL;
				}
				
				/* Either learn the Diameter Identity, or check it against what we know */
				if (peer->p_hdr.info.config.pic_flags.diamid == PI_DIAMID_DYN) {
					/* If the octet string contains a \0 */
					if (!fd_os_is_valid_DiameterIdentity(hdr->avp_value->os.data, hdr->avp_value->os.len)) {
						error->pei_errcode = "DIAMETER_INVALID_AVP_VALUE";
						error->pei_message = "Your Origin-Host contains invalid characters.";
						error->pei_avp = avp;
						return EINVAL;
					}
					
				} else {
					if (fd_os_almostcasesrch(hdr->avp_value->os.data, hdr->avp_value->os.len, 
								peer->p_hdr.info.pi_diamid, peer->p_hdr.info.pi_diamidlen, NULL)) {
						TRACE_DEBUG(INFO, "Received a message with Origin-Host set to '%.*s' while expecting '%s'", 
								(int)hdr->avp_value->os.len, hdr->avp_value->os.data, peer->p_hdr.info.pi_diamid);
						error->pei_errcode = "DIAMETER_AVP_NOT_ALLOWED";
						error->pei_message = "Your Origin-Host value does not match my configuration.";
						error->pei_avp = avp;
						return EINVAL;
					}
				}

				/* Save the value */
				CHECK_MALLOC(  peer->p_hdr.info.runtime.pir_host = os0dup( hdr->avp_value->os.data, hdr->avp_value->os.len )  );
				peer->p_hdr.info.runtime.pir_hostlen = hdr->avp_value->os.len;
				break;
		
			case AC_ORIGIN_REALM: /* Origin-Realm */
				if (hdr->avp_value == NULL) {
					/* This is a sanity check */
					LOG_F("Ignored an AVP (code %x) with unset value in CER/CEA", hdr->avp_code);
					ASSERT(0); /* To check if this really happens, and understand why... */
					goto next;
				}
				
				/* In case of multiple AVPs */
				if (peer->p_hdr.info.runtime.pir_realm) {
					TRACE_DEBUG(INFO, "Multiple instances of the Origin-Realm AVP");
					error->pei_errcode = "DIAMETER_AVP_OCCURS_TOO_MANY_TIMES";
					error->pei_message = "I found several Origin-Realm AVPs";
					error->pei_avp = avp;
					return EINVAL;
				}
				
				/* If the octet string contains a \0 */
				if (!fd_os_is_valid_DiameterIdentity(hdr->avp_value->os.data, hdr->avp_value->os.len)) {
					error->pei_errcode = "DIAMETER_INVALID_AVP_VALUE";
					error->pei_message = "Your Origin-Realm contains invalid characters.";
					error->pei_avp = avp;
					return EINVAL;
				}
				
				/* Save the value */
				CHECK_MALLOC(  peer->p_hdr.info.runtime.pir_realm = os0dup( hdr->avp_value->os.data, hdr->avp_value->os.len )  );
				peer->p_hdr.info.runtime.pir_realmlen = hdr->avp_value->os.len;
				break;

			case AC_HOST_IP_ADDRESS: /* Host-IP-Address */
				if (hdr->avp_value == NULL) {
					/* This is a sanity check */
					LOG_F("Ignored an AVP (code %x) with unset value in CER/CEA", hdr->avp_code);
					ASSERT(0); /* To check if this really happens, and understand why... */
					goto next;
				}
				{
					sSS	ss;

					/* Get the sockaddr value */
					memset(&ss, 0, sizeof(ss));
					CHECK_FCT_DO( fd_msg_avp_value_interpret( avp, &ss),
						{
							/* in case of error, assume the AVP value was wrong */
							error->pei_errcode = "DIAMETER_INVALID_AVP_VALUE";
							error->pei_avp = avp;
							return EINVAL;
						} );

					/* Save this endpoint in the list as advertized */
					CHECK_FCT( fd_ep_add_merge( &peer->p_hdr.info.pi_endpoints, (sSA *)&ss, sizeof(sSS), EP_FL_ADV ) );
				}
				break;

			case AC_VENDOR_ID: /* Vendor-Id */
				if (hdr->avp_value == NULL) {
					/* This is a sanity check */
					LOG_F("Ignored an AVP (code %x) with unset value in CER/CEA", hdr->avp_code);
					ASSERT(0); /* To check if this really happens, and understand why... */
					goto next;
				}
				
				/* In case of multiple AVPs */
				if (peer->p_hdr.info.runtime.pir_vendorid) {
					TRACE_DEBUG(INFO, "Multiple instances of the Vendor-Id AVP");
					error->pei_errcode = "DIAMETER_AVP_OCCURS_TOO_MANY_TIMES";
					error->pei_message = "I found several Vendor-Id AVPs";
					error->pei_avp = avp;
					return EINVAL;
				}
				
				peer->p_hdr.info.runtime.pir_vendorid = hdr->avp_value->u32;
				break;

			case AC_PRODUCT_NAME: /* Product-Name */
				if (hdr->avp_value == NULL) {
					/* This is a sanity check */
					LOG_F("Ignored an AVP (code %x) with unset value in CER/CEA", hdr->avp_code);
					ASSERT(0); /* To check if this really happens, and understand why... */
					goto next;
				}
				
				/* In case of multiple AVPs */
				if (peer->p_hdr.info.runtime.pir_prodname) {
					TRACE_DEBUG(INFO, "Multiple instances of the Product-Name AVP");
					error->pei_errcode = "DIAMETER_AVP_OCCURS_TOO_MANY_TIMES";
					error->pei_message = "I found several Product-Name AVPs";
					error->pei_avp = avp;
					return EINVAL;
				}

				CHECK_MALLOC( peer->p_hdr.info.runtime.pir_prodname = calloc( hdr->avp_value->os.len + 1, 1 )  );
				memcpy(peer->p_hdr.info.runtime.pir_prodname, hdr->avp_value->os.data, hdr->avp_value->os.len);
				break;

			case AC_ORIGIN_STATE_ID: /* Origin-State-Id */
				if (hdr->avp_value == NULL) {
					/* This is a sanity check */
					LOG_F("Ignored an AVP (code %x) with unset value in CER/CEA", hdr->avp_code);
					ASSERT(0); /* To check if this really happens, and understand why... */
					goto next;
				}
				
				/* In case of multiple AVPs */
				if (peer->p_hdr.info.runtime.pir_orstate) {
					TRACE_DEBUG(INFO, "Multiple instances of the Origin-State-Id AVP");
					error->pei_errcode = "DIAMETER_AVP_OCCURS_TOO_MANY_TIMES";
					error->pei_message = "I found several Origin-State-Id AVPs";
					error->pei_avp = avp;
					return EINVAL;
				}
				
				peer->p_hdr.info.runtime.pir_orstate = hdr->avp_value->u32;
				break;

			case AC_SUPPORTED_VENDOR_ID: /* Supported-Vendor-Id */
				if (hdr->avp_value == NULL) {
					/* This is a sanity check */
					LOG_F("Ignored an AVP (code %x) with unset value in CER/CEA", hdr->avp_code);
					ASSERT(0); /* To check if this really happens, and understand why... */
					goto next;
				}
				
				TRACE_DEBUG(FULL, "'%s' claims support for a subset of vendor %d features.", peer->p_hdr.info.pi_diamid, hdr->avp_value->u32);
				/* not that it makes a difference for us... 
				 -- if an application actually needs this info, we could save it somewhere.
				*/
				break;

			case AC_VENDOR_SPECIFIC_APPLICATION_ID: /* Vendor-Specific-Application-Id (grouped)*/
				{
					struct avp * inavp = NULL;
					vendor_id_t vid = 0;
					application_id_t auth_aid = 0;
					application_id_t acct_aid = 0;
					int invalid=0;

					/* get the first child AVP */
					CHECK_FCT(  fd_msg_browse(avp, MSG_BRW_FIRST_CHILD, &inavp, NULL)  );

					while (inavp) {
						struct avp_hdr * inhdr;
						CHECK_FCT(  fd_msg_avp_hdr( inavp, &inhdr )  );

						if (inhdr->avp_flags & AVP_FLAG_VENDOR) {
							LOG_A("Ignored a vendor AVP inside Vendor-Specific-Application-Id AVP");
							goto innext;
						}

						if (inhdr->avp_value == NULL) {
							/* This is a sanity check */
							LOG_F("Ignored an AVP (code %x) with unset value in CER/CEA", hdr->avp_code);
							ASSERT(0); /* To check if this really happens, and understand why... */
							goto innext;
						}
						switch (inhdr->avp_code) {
							case AC_VENDOR_ID: /* Vendor-Id */
#ifndef WORKAROUND_ACCEPT_INVALID_VSAI
								if (vid != 0)
									invalid++; /* We already had one such AVP. This is invalid according to RFC6733 but not RFC3588 (but there is an erratum) */
#endif /* WORKAROUND_ACCEPT_INVALID_VSAI */
								vid = inhdr->avp_value->u32;
								break;
							case AC_AUTH_APPLICATION_ID: /* Auth-Application-Id */
								if (auth_aid != 0)
									invalid++; /* We already had one such AVP */
#ifndef WORKAROUND_ACCEPT_INVALID_VSAI
								if (acct_aid != 0)
									invalid++; /* Only 1 *-Application-Id AVP is allowed */
#endif /* WORKAROUND_ACCEPT_INVALID_VSAI */
								auth_aid = inhdr->avp_value->u32;
								break;
							case AC_ACCT_APPLICATION_ID: /* Acct-Application-Id */
								if (acct_aid != 0)
									invalid++; /* We already had one such AVP */
#ifndef WORKAROUND_ACCEPT_INVALID_VSAI
								if (auth_aid != 0)
									invalid++; /* Only 1 *-Application-Id AVP is allowed */
#endif /* WORKAROUND_ACCEPT_INVALID_VSAI */
								acct_aid = inhdr->avp_value->u32;
								break;
							/* ignore other AVPs */
						}
						
						if (invalid) {
							TRACE_DEBUG(FULL, "Invalid Vendor-Specific-Application-Id AVP received");
							error->pei_errcode = "DIAMETER_INVALID_AVP_VALUE";
							error->pei_avp = avp;
							return EINVAL;
						}

					innext:			
						/* Go to next in AVP */
						CHECK_FCT( fd_msg_browse(inavp, MSG_BRW_NEXT, &inavp, NULL) );
					}
					
					/* Add entry in the list */
					if (auth_aid) {
						CHECK_FCT( fd_app_merge(&peer->p_hdr.info.runtime.pir_apps, auth_aid, vid, 1, 0) );
					}
					if (acct_aid) {
						CHECK_FCT( fd_app_merge(&peer->p_hdr.info.runtime.pir_apps, acct_aid, vid, 0, 1) );
					}
				}
				break;

			case AC_AUTH_APPLICATION_ID: /* Auth-Application-Id */
				if (hdr->avp_value == NULL) {
					/* This is a sanity check */
					LOG_F("Ignored an AVP (code %x) with unset value in CER/CEA", hdr->avp_code);
					ASSERT(0); /* To check if this really happens, and understand why... */
					goto next;
				}
				
				if (hdr->avp_value->u32 == AI_RELAY) {
					peer->p_hdr.info.runtime.pir_relay = 1;
				} else {
					CHECK_FCT( fd_app_merge(&peer->p_hdr.info.runtime.pir_apps, hdr->avp_value->u32, 0, 1, 0) );
				}
				break;

			case AC_ACCT_APPLICATION_ID: /* Acct-Application-Id */
				if (hdr->avp_value == NULL) {
					/* This is a sanity check */
					LOG_F("Ignored an AVP (code %x) with unset value in CER/CEA", hdr->avp_code);
					ASSERT(0); /* To check if this really happens, and understand why... */
					goto next;
				}
				
				if (hdr->avp_value->u32 == AI_RELAY) {
					/* Not clear if the relay application can be inside this AVP... */
					peer->p_hdr.info.runtime.pir_relay = 1;
				} else {
					CHECK_FCT( fd_app_merge(&peer->p_hdr.info.runtime.pir_apps, hdr->avp_value->u32, 0, 0, 1) );
				}
				break;

			case AC_FIRMWARE_REVISION: /* Firmware-Revision */
				if (hdr->avp_value == NULL) {
					/* This is a sanity check */
					LOG_F("Ignored an AVP (code %x) with unset value in CER/CEA", hdr->avp_code);
					ASSERT(0); /* To check if this really happens, and understand why... */
					goto next;
				}
				
				peer->p_hdr.info.runtime.pir_firmrev = hdr->avp_value->u32;
				break;

			case AC_INBAND_SECURITY_ID: /* Inband-Security-Id */
				if (hdr->avp_value == NULL) {
					/* This is a sanity check */
					LOG_F("Ignored an AVP (code %x) with unset value in CER/CEA", hdr->avp_code);
					ASSERT(0); /* To check if this really happens, and understand why... */
					goto next;
				}
				if (hdr->avp_value->u32 >= 32 ) {
					error->pei_errcode = "DIAMETER_INVALID_AVP_VALUE";
					error->pei_message = "I don't support this Inband-Security-Id value (yet).";
					error->pei_avp = avp;
					return EINVAL;
				}
				peer->p_hdr.info.runtime.pir_isi |= (1 << hdr->avp_value->u32);
				break;
		}

next:			
		/* Go to next AVP */
		CHECK_FCT( fd_msg_browse(avp, MSG_BRW_NEXT, &avp, NULL) );
	}
	
	return 0;
}

/* Create a CER message for sending */ 
static int create_CER(struct fd_peer * peer, struct cnxctx * cnx, struct msg ** cer)
{
	int isi_tls = 0;
	int isi_none = 0;
	
	/* Find CER dictionary object and create an instance */
	CHECK_FCT( fd_msg_new ( fd_dict_cmd_CER, MSGFL_ALLOC_ETEID, cer ) );
	
	/* Do we need Inband-Security-Id AVPs ? If we're already using TLS, we don't... */
	if (!fd_cnx_getTLS(cnx)) {
		isi_none = peer->p_hdr.info.config.pic_flags.sec & PI_SEC_NONE; /* we add it even if the peer does not use the old mechanism, it is impossible to distinguish */

		if (peer->p_hdr.info.config.pic_flags.sec & PI_SEC_TLS_OLD) {
			if (fd_g_config->cnf_sec_data.tls_disabled) {
				LOG_N("TLS disabled locally, so Inband-Security-Id (TLS) not included for peer %s", peer->p_hdr.info.pi_diamid);
			} else {
				isi_tls  = 1;
			}
		}
	}
	
	/* Add the information about the local peer */
	CHECK_FCT( add_CE_info(*cer, cnx, isi_tls, isi_none) );
	
	/* Done! */
	return 0;
}


/* Continue with the initiator side */
static int to_waitcea(struct fd_peer * peer, struct cnxctx * cnx)
{
	/* We sent a CER on the connection, set the event queue so that we receive the CEA */
	CHECK_FCT( set_peer_cnx(peer, &cnx) );
	
	/* Change state and reset the timer */
	CHECK_FCT( fd_psm_change_state(peer, STATE_WAITCEA) );
	fd_psm_next_timeout(peer, 0, CEA_TIMEOUT);
	
	return 0;
}

/* Reject an incoming connection attempt */
static void receiver_reject(struct cnxctx ** recv_cnx, struct msg ** cer, struct fd_pei * error)
{
	/* Create and send the CEA with appropriate error code */
	CHECK_FCT_DO( fd_msg_new_answer_from_req ( fd_g_config->cnf_dict, cer, MSGFL_ANSW_ERROR ), goto destroy );
	CHECK_FCT_DO( fd_msg_rescode_set(*cer, error->pei_errcode, error->pei_message, error->pei_avp, 1 ), goto destroy );
	CHECK_FCT_DO( fd_out_send(cer, *recv_cnx, NULL, 0), goto destroy );
	
	if (error->pei_avp_free) {
		fd_msg_free(error->pei_avp);
	}
	
	/* And now destroy this connection */
destroy:
	fd_cnx_destroy(*recv_cnx);
	*recv_cnx = NULL;
	if (*cer) {
		fd_hook_call(HOOK_MESSAGE_DROPPED, *cer, NULL, "An error occurred while rejecting this CER.", fd_msg_pmdl_get(*cer));
		fd_msg_free(*cer);
		*cer = NULL;
	}
}

/* We have established a new connection to the remote peer, send CER and eventually process the election */
int fd_p_ce_handle_newcnx(struct fd_peer * peer, struct cnxctx * initiator)
{
	struct msg * cer = NULL;
	
	/* Send CER on the new connection */
	CHECK_FCT( create_CER(peer, initiator, &cer) );
	CHECK_FCT( fd_out_send(&cer, initiator, peer, 0) );
	
	/* Are we doing an election ? */
	if (fd_peer_getstate(peer) == STATE_WAITCNXACK_ELEC) {
		if (election_result(peer, peer->p_cer)) {
			/* Close initiator connection */
			fd_cnx_destroy(initiator);

			LOG_D("%s: Election lost on outgoing connection, closing and answering CEA on incoming connection.", peer->p_hdr.info.pi_diamid);
			
			/* Process with the receiver side */
			CHECK_FCT( fd_p_ce_process_receiver(peer) );

		} else {
			struct fd_pei pei;
			memset(&pei, 0, sizeof(pei));
			pei.pei_errcode = "ELECTION_LOST";
			pei.pei_message = "Please answer my CER instead, you won the election.";
			LOG_D("%s: Election lost on incoming connection, closing and waiting for CEA on outgoing connection.", peer->p_hdr.info.pi_diamid);

			/* Answer an ELECTION LOST to the receiver side */
			receiver_reject(&peer->p_receiver, &peer->p_cer, &pei);
			CHECK_FCT( to_waitcea(peer, initiator) );
		}
	} else {
		/* No election (yet) */
		CHECK_FCT( to_waitcea(peer, initiator) );
	}
	
	return 0;
}

/* We have received a Capabilities Exchange message on the peer connection */
int fd_p_ce_msgrcv(struct msg ** msg, int req, struct fd_peer * peer)
{
	uint32_t rc = 0;
	int st;
	struct fd_pei pei;
	
	TRACE_ENTRY("%p %p", msg, peer);
	CHECK_PARAMS( msg && *msg && CHECK_PEER(peer) );
	
	/* The only valid situation where we are called is in WAITCEA and we receive a CEA (we may have won an election) */
	
	/* Note : to implement Capabilities Update, we would need to change here */
	
	/* If it is a CER, just reply an error */
	if (req) {
		/* Create the error message */
		CHECK_FCT( fd_msg_new_answer_from_req ( fd_g_config->cnf_dict, msg, MSGFL_ANSW_ERROR ) );
		
		/* Set the error code */
		CHECK_FCT( fd_msg_rescode_set(*msg, "DIAMETER_UNABLE_TO_COMPLY", "No CER allowed in current state", NULL, 1 ) );

		/* msg now contains an answer message to send back */
		CHECK_FCT_DO( fd_out_send(msg, NULL, peer, 0), /* In case of error the message has already been dumped */ );
	}
	
	/* If the state is not WAITCEA, just discard the message */
	if (req || ((st = fd_peer_getstate(peer)) != STATE_WAITCEA)) {
		if (*msg) {
			/* In such case, just discard the message */
			char buf[128];
			snprintf(buf, sizeof(buf), "Received while peer state machine was in state %s.", STATE_STR(st));
			fd_hook_call(HOOK_MESSAGE_DROPPED, *msg, peer, buf, fd_msg_pmdl_get(*msg));

			CHECK_FCT_DO( fd_msg_free(*msg), /* continue */);
			*msg = NULL;
		}
		
		return 0;
	}
	
	memset(&pei, 0, sizeof(pei));
	
	/* Save info from the CEA into the peer */
	CHECK_FCT_DO( save_remote_CE_info(*msg, peer, &pei, &rc), 
		{
			fd_hook_call(HOOK_PEER_CONNECT_FAILED, *msg, peer, "An error occurred while processing incoming CEA.", NULL);
			goto cleanup;
		} );
	
	/* Check the Result-Code */
	switch (rc) {
		case ER_DIAMETER_SUCCESS:
			/* Log success */
			fd_hook_call(HOOK_PEER_CONNECT_SUCCESS, *msg, peer, NULL, NULL);
			
			/* Dispose of the message, we don't need it anymore */
			CHECK_FCT_DO( fd_msg_free(*msg), /* continue */ );
			*msg = NULL;
			
			/* No problem, we can continue */
			break;
			
		case ER_DIAMETER_TOO_BUSY:
			/* Retry later */
			fd_hook_call(HOOK_PEER_CONNECT_FAILED, *msg, peer, "Remote peer is too busy", NULL);
			fd_psm_cleanup(peer, 0);
			fd_psm_next_timeout(peer, 0, 300);
			return 0;
		
		case ER_ELECTION_LOST:
			/* Ok, just wait for a little while for the CER to be processed on the other connection. */
			TRACE_DEBUG(FULL, "Peer %s replied a CEA with Result-Code AVP ELECTION_LOST, waiting for events.", peer->p_hdr.info.pi_diamid);
			return 0;
		
		default:
			/* In any other case, we abort all attempts to connect to this peer */
			fd_hook_call(HOOK_PEER_CONNECT_FAILED, *msg, peer, "CEA with unexpected error code", NULL);
			return EINVAL;
	}
	
	
	/* Handshake if needed, start clear otherwise */
	if ( ! fd_cnx_getTLS(peer->p_cnxctx) ) {
		int todo = peer->p_hdr.info.config.pic_flags.sec & peer->p_hdr.info.runtime.pir_isi ;
		/* Special case: if the peer did not send a ISI AVP */
		if (peer->p_hdr.info.runtime.pir_isi == 0)
			todo = peer->p_hdr.info.config.pic_flags.sec;
		
		if (todo == PI_SEC_NONE) {
			/* Ok for clear connection */
			TRACE_DEBUG(INFO, "No TLS protection negotiated with peer '%s'.", peer->p_hdr.info.pi_diamid);
			CHECK_FCT( fd_cnx_start_clear(peer->p_cnxctx, 1) );
			
		} else if (fd_g_config->cnf_sec_data.tls_disabled) {
			LOG_E("Clear connection with remote peer '%s' is not (explicitly) allowed, and TLS is disabled. Giving up...", peer->p_hdr.info.pi_diamid);
			fd_hook_call(HOOK_PEER_CONNECT_FAILED, NULL, peer, "TLS is disabled and peer is not configured for IPsec", NULL);
			goto cleanup;
			
		} else {
			fd_psm_change_state(peer, STATE_OPEN_HANDSHAKE);
			CHECK_FCT_DO( fd_cnx_handshake(peer->p_cnxctx, GNUTLS_CLIENT, ALGO_HANDSHAKE_3436, peer->p_hdr.info.config.pic_priority, NULL),
				{
					/* Handshake failed ...  */
					fd_hook_call(HOOK_PEER_CONNECT_FAILED, NULL, peer, "TLS handshake failed after CER/CEA exchange", NULL);
					goto cleanup;
				} );

			/* Retrieve the credentials */
			CHECK_FCT( fd_cnx_getcred(peer->p_cnxctx, &peer->p_hdr.info.runtime.pir_cert_list, &peer->p_hdr.info.runtime.pir_cert_list_size) );
		}
	}
	
	/* Move to next state */
	if (peer->p_flags.pf_cnx_pb) {
		fd_psm_change_state(peer, STATE_REOPEN );
		CHECK_FCT( fd_p_dw_reopen(peer) );
	} else {
		fd_psm_change_state(peer, STATE_OPEN );
		fd_psm_next_timeout(peer, 1, peer->p_hdr.info.config.pic_twtimer ?: fd_g_config->cnf_timer_tw);
	}
	
	return 0;
	
cleanup:
	fd_p_ce_clear_cnx(peer, NULL);

	/* Send the error to the peer */
	CHECK_FCT( fd_event_send(peer->p_events, FDEVP_CNX_ERROR, 0, NULL) );

	return 0;
}

/* Handle the receiver side to go to OPEN or OPEN_NEW state (any election is resolved) */
int fd_p_ce_process_receiver(struct fd_peer * peer)
{
	struct fd_pei pei;
	struct msg * msg = NULL;
	int isi = 0;
	int fatal = 0;
	int tls_sync=0;
	
	TRACE_ENTRY("%p", peer);
	
	CHECK_FCT_DO( set_peer_cnx(peer, &peer->p_receiver),
		{
			fd_hook_call(HOOK_PEER_CONNECT_FAILED, NULL, peer, "Error saving the incoming connection in the peer structure", NULL);
			return __ret__;
		} );
	msg = peer->p_cer;
	peer->p_cer = NULL;
	
	memset(&pei, 0, sizeof(pei));
	
	/* Parse the content of the received CER */
	CHECK_FCT_DO( save_remote_CE_info(msg, peer, &pei, NULL), goto error_abort );
	
	/* Validate the realm if needed */
	if (peer->p_hdr.info.config.pic_realm) {
		size_t len = strlen(peer->p_hdr.info.config.pic_realm);
		if (fd_os_almostcasesrch(peer->p_hdr.info.config.pic_realm, len, peer->p_hdr.info.runtime.pir_realm, peer->p_hdr.info.runtime.pir_realmlen, NULL)) {
			TRACE_DEBUG(INFO, "Rejected CER from peer '%s', realm mismatch with configured value (returning DIAMETER_UNKNOWN_PEER).", peer->p_hdr.info.pi_diamid);
			pei.pei_errcode = "DIAMETER_UNKNOWN_PEER"; /* maybe AVP_NOT_ALLOWED would be better fit? */
			goto error_abort;
		}
	}
	
	/* Save the credentials if handshake already occurred */
	if ( fd_cnx_getTLS(peer->p_cnxctx) ) {
		CHECK_FCT( fd_cnx_getcred(peer->p_cnxctx, &peer->p_hdr.info.runtime.pir_cert_list, &peer->p_hdr.info.runtime.pir_cert_list_size) );
	}
	
	/* Validate the peer if needed */
	if (peer->p_flags.pf_responder) {
		int res = fd_peer_validate( peer );
		if (res < 0) {
			TRACE_DEBUG(INFO, "Rejected CER from peer '%s', validation failed (returning DIAMETER_UNKNOWN_PEER).", peer->p_hdr.info.pi_diamid);
			pei.pei_errcode = "DIAMETER_UNKNOWN_PEER";
			goto error_abort;
		}
		CHECK_FCT( res );
	}
	
	/* Check if we have common applications */
	if ( fd_g_config->cnf_flags.no_fwd && (! peer->p_hdr.info.runtime.pir_relay) ) {
		int got_common;
		CHECK_FCT( fd_app_check_common( &fd_g_config->cnf_apps, &peer->p_hdr.info.runtime.pir_apps, &got_common) );
		if (!got_common) {
			TRACE_DEBUG(INFO, "No common application with peer '%s', sending DIAMETER_NO_COMMON_APPLICATION", peer->p_hdr.info.pi_diamid);
			pei.pei_errcode = "DIAMETER_NO_COMMON_APPLICATION";
			fatal = 1;
			goto error_abort;
		}
	}
	
	/* Do we agree on ISI ? */
	if ( ! fd_cnx_getTLS(peer->p_cnxctx) ) {
		
		/* In case of responder, the validate callback must have set the config.pic_flags.sec value already */
	
		/* First case: we are not using old mechanism: ISI are deprecated, we ignore it. */
		if ( ! (peer->p_hdr.info.config.pic_flags.sec & PI_SEC_TLS_OLD)) {
			/* Just check then that the peer configuration allows for IPsec protection */
			if (peer->p_hdr.info.config.pic_flags.sec & PI_SEC_NONE) {
				isi = PI_SEC_NONE;
			} else {
				/* otherwise, we should have already been protected. Reject */
				TRACE_DEBUG(INFO, "Non TLS-protected CER/CEA exchanges are not allowed with this peer, rejecting.");
			}
		} else {
			/* The old mechanism is allowed with this peer. Now, look into the ISI AVP values */
			
			/* In case no ISI was present anyway: */
			if (!peer->p_hdr.info.runtime.pir_isi) {
				TRACE_DEBUG(INFO, "Inband-Security-Id AVP is missing in received CER.");
				if (peer->p_hdr.info.config.pic_flags.sec & PI_SEC_NONE) {
					isi = PI_SEC_NONE;
					TRACE_DEBUG(INFO, "IPsec protection allowed by configuration, allowing this mechanism to be used.");
				} else {
					/* otherwise, we should have already been protected. Reject */
					TRACE_DEBUG(INFO, "Rejecting the peer connection (please allow IPsec here or configure TLS in the remote peer).");
				}
			} else {
				/* OK, the remote peer did send the ISI AVP. */
				if ((peer->p_hdr.info.config.pic_flags.sec & PI_SEC_NONE) && (peer->p_hdr.info.runtime.pir_isi & PI_SEC_NONE)) {
					/* We have allowed IPsec */
					isi = PI_SEC_NONE;
				} else if (fd_g_config->cnf_sec_data.tls_disabled) {
					/* We can agree on TLS */
					TRACE_DEBUG(INFO, "Remote peer is not allowed for IPsec and TLS is disabled.");;
				} else if (peer->p_hdr.info.runtime.pir_isi & PI_SEC_TLS_OLD) {
					/* We can agree on TLS */
					isi = PI_SEC_TLS_OLD;
				} else {
					TRACE_DEBUG(INFO, "Remote peer requested IPsec protection, but local configuration forbids it.");
				}
			}
		}
	
		/* If we did not find an agreement */
		if (!isi) {
			TRACE_DEBUG(INFO, "No common security mechanism with '%s', sending DIAMETER_NO_COMMON_SECURITY", peer->p_hdr.info.pi_diamid);
			pei.pei_errcode = "DIAMETER_NO_COMMON_SECURITY";
			fatal = 1;
			goto error_abort;
		}
		
		/* Do not send the ISI IPsec if we are using the new mechanism */
		if ((isi == PI_SEC_NONE) && (! (peer->p_hdr.info.config.pic_flags.sec & PI_SEC_TLS_OLD)))
			isi = 0;
	}
	
	/* Reply a CEA */
	CHECK_FCT( fd_msg_new_answer_from_req ( fd_g_config->cnf_dict, &msg, 0 ) );
	CHECK_FCT( fd_msg_rescode_set(msg, "DIAMETER_SUCCESS", NULL, NULL, 0 ) );
	CHECK_FCT( add_CE_info(msg, peer->p_cnxctx, isi & PI_SEC_TLS_OLD, isi & PI_SEC_NONE) );
	
	/* The connection is complete, but we may still need TLS handshake */
	fd_hook_call(HOOK_PEER_CONNECT_SUCCESS, msg, peer, NULL, NULL);
	
	CHECK_FCT( fd_out_send(&msg, peer->p_cnxctx, peer, 0 ) );
	
	/* Handshake if needed */
	if (isi & PI_SEC_TLS_OLD) {
		fd_psm_change_state(peer, STATE_OPEN_HANDSHAKE);
		CHECK_FCT_DO( fd_cnx_handshake(peer->p_cnxctx, GNUTLS_SERVER, ALGO_HANDSHAKE_3436, peer->p_hdr.info.config.pic_priority, NULL),
			{
				/* Handshake failed ...  */
				fd_hook_call(HOOK_PEER_CONNECT_FAILED, NULL, peer, "TLS handshake failed after CER/CEA exchange", NULL);
				goto cleanup;
			} );
		
		/* Retrieve the credentials */
		CHECK_FCT_DO( fd_cnx_getcred(peer->p_cnxctx, &peer->p_hdr.info.runtime.pir_cert_list, &peer->p_hdr.info.runtime.pir_cert_list_size),
			{
				/* Error ...  */
				fd_hook_call(HOOK_PEER_CONNECT_FAILED, NULL, peer, "Unable to retrieve remote credentials after TLS handshake", NULL);
				goto cleanup;
			} );
		
		
		/* Call second validation callback if needed */
		if (peer->p_cb2) {
			TRACE_DEBUG(FULL, "Calling second validation callback for %s", peer->p_hdr.info.pi_diamid);
			CHECK_FCT_DO( (*peer->p_cb2)( &peer->p_hdr.info ),
				{
					fd_hook_call(HOOK_PEER_CONNECT_FAILED, NULL, peer, "Validation callback rejected the peer after handshake", NULL);
					CHECK_FCT( fd_psm_terminate( peer, "DO_NOT_WANT_TO_TALK_TO_YOU" ) );
					return 0;
				}  );
		}
		tls_sync = 1;
	} else {
		if ( ! fd_cnx_getTLS(peer->p_cnxctx) ) {
			TRACE_DEBUG(INFO, "No TLS protection negotiated with peer '%s'.", peer->p_hdr.info.pi_diamid);
			CHECK_FCT( fd_cnx_start_clear(peer->p_cnxctx, 1) );
		}
	}
		
	/* Move to OPEN or REOPEN state */
	if (peer->p_flags.pf_cnx_pb) {
		fd_psm_change_state(peer, STATE_REOPEN );
		CHECK_FCT( fd_p_dw_reopen(peer) );
	} else {
		if ((!tls_sync) && (fd_cnx_is_unordered_delivery_supported(peer->p_cnxctx))) {
			fd_psm_change_state(peer, STATE_OPEN_NEW );
			/* send DWR */
			CHECK_FCT( fd_p_dw_timeout(peer) );
		} else {

			fd_psm_change_state(peer, STATE_OPEN );
			fd_psm_next_timeout(peer, 1, peer->p_hdr.info.config.pic_twtimer ?: fd_g_config->cnf_timer_tw);
		}
	}
	
	return 0;

error_abort:
	if (pei.pei_errcode) {
		/* Send the error */
		fd_hook_call(HOOK_PEER_CONNECT_FAILED, msg, peer, pei.pei_message ?: pei.pei_errcode, NULL);
		receiver_reject(&peer->p_cnxctx, &msg, &pei);
	} else {
		char buf[1024];
		snprintf(buf, sizeof(buf), "Unexpected error occurred while processing incoming connection from '%s'.", peer->p_hdr.info.pi_diamid);
		fd_hook_call(HOOK_PEER_CONNECT_FAILED, msg, peer, buf, NULL);
	}
	
cleanup:
	if (msg) {
		fd_msg_free(msg);
	}
	fd_p_ce_clear_cnx(peer, NULL);

	/* Send the error to the peer */
	CHECK_FCT( fd_event_send(peer->p_events, fatal ? FDEVP_TERMINATE : FDEVP_CNX_ERROR, 0, NULL) );

	return 0;
}

/* We have received a CER on a new connection for this peer */
int fd_p_ce_handle_newCER(struct msg ** msg, struct fd_peer * peer, struct cnxctx ** cnx, int valid)
{
	struct fd_pei pei;
	int cur_state = fd_peer_getstate(peer);
	memset(&pei, 0, sizeof(pei));
	
	switch (cur_state) {
		case STATE_CLOSED:
			peer->p_receiver = *cnx;
			*cnx = NULL;
			peer->p_cer = *msg;
			*msg = NULL;
			CHECK_FCT( fd_p_ce_process_receiver(peer) );
			break;

		case STATE_WAITCNXACK:
			/* Save the parameters in the peer, move to STATE_WAITCNXACK_ELEC */
			peer->p_receiver = *cnx;
			*cnx = NULL;
			peer->p_cer = *msg;
			*msg = NULL;
			CHECK_FCT( fd_psm_change_state(peer, STATE_WAITCNXACK_ELEC) );
			break;
			
		case STATE_WAITCEA:
			if (election_result(peer, *msg)) {
				
				/* Close initiator connection (was already set as principal) */
				LOG_D("%s: Election lost on outgoing connection, closing and answering CEA on incoming connection.", peer->p_hdr.info.pi_diamid);
				fd_p_ce_clear_cnx(peer, NULL);
				
				/* and go on with the receiver side */
				peer->p_receiver = *cnx;
				*cnx = NULL;
				peer->p_cer = *msg;
				*msg = NULL;
				CHECK_FCT( fd_p_ce_process_receiver(peer) );

			} else {

				/* Answer an ELECTION LOST to the receiver side and continue */
				pei.pei_errcode = "ELECTION_LOST";
				pei.pei_message = "Please answer my CER instead, you won the election.";
				LOG_D("%s: Election lost on incoming connection, closing and waiting for CEA on outgoing connection.", peer->p_hdr.info.pi_diamid);
				receiver_reject(cnx, msg, &pei);
			}
			break;

		default:
			pei.pei_errcode = "DIAMETER_UNABLE_TO_COMPLY"; /* INVALID COMMAND? in case of Capabilities-Updates? */
			pei.pei_message = "Invalid state to receive a new connection attempt.";
			LOG_E("%s: Rejecting new connection attempt while our state machine is in state '%s'", peer->p_hdr.info.pi_diamid, STATE_STR(cur_state));
			receiver_reject(cnx, msg, &pei);
	}
				
	return 0;
}
