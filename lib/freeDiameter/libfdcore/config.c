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
#include <sys/stat.h>

/* Configuration management */

#ifndef GNUTLS_DEFAULT_PRIORITY
# define GNUTLS_DEFAULT_PRIORITY "NORMAL"
#endif /* GNUTLS_DEFAULT_PRIORITY */
#ifndef GNUTLS_DEFAULT_DHBITS
# define GNUTLS_DEFAULT_DHBITS 1024
#endif /* GNUTLS_DEFAULT_DHBITS */

/* Initialize the fd_g_config structure to default values -- it should already have been initialized to all-0 */
int fd_conf_init()
{
	TRACE_ENTRY();
	
	fd_g_config->cnf_eyec = EYEC_CONFIG;
	
	fd_g_config->cnf_timer_tc = 30;
	fd_g_config->cnf_timer_tw = 30;
	
	fd_g_config->cnf_port     = DIAMETER_PORT;
	fd_g_config->cnf_port_tls = DIAMETER_SECURE_PORT;
	fd_g_config->cnf_sctp_str = 30;
	fd_g_config->cnf_thr_srv  = 5;
	fd_g_config->cnf_dispthr  = 4;
	fd_list_init(&fd_g_config->cnf_endpoints, NULL);
	fd_list_init(&fd_g_config->cnf_apps, NULL);
	#ifdef DISABLE_SCTP
	fd_g_config->cnf_flags.no_sctp = 1;
	#endif /* DISABLE_SCTP */
	
	fd_g_config->cnf_orstateid = (uint32_t) time(NULL);
	
	CHECK_FCT( fd_dict_init(&fd_g_config->cnf_dict) );
	CHECK_FCT( fd_fifo_new(&fd_g_config->cnf_main_ev, 0) );
	
	/* TLS parameters */
	CHECK_GNUTLS_DO( gnutls_certificate_allocate_credentials (&fd_g_config->cnf_sec_data.credentials), return ENOMEM );
	CHECK_GNUTLS_DO( gnutls_dh_params_init (&fd_g_config->cnf_sec_data.dh_cache), return ENOMEM );
#ifdef GNUTLS_VERSION_300
	CHECK_GNUTLS_DO( gnutls_x509_trust_list_init(&fd_g_config->cnf_sec_data.trustlist, 0), return ENOMEM );
#endif /* GNUTLS_VERSION_300 */

	return 0;
}

DECLARE_FD_DUMP_PROTOTYPE(fd_conf_dump)
{
	FD_DUMP_HANDLE_OFFSET();
	
	CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "freeDiameter configuration:\n"), return NULL);	
	CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "  Default trace level .... : %+d\n", fd_g_debug_lvl), return NULL);
	CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "  Configuration file ..... : %s\n", fd_g_config->cnf_file), return NULL);
	CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "  Diameter Identity ...... : %s (l:%Zi)\n", fd_g_config->cnf_diamid, fd_g_config->cnf_diamid_len), return NULL);
	CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "  Diameter Realm ......... : %s (l:%Zi)\n", fd_g_config->cnf_diamrlm, fd_g_config->cnf_diamrlm_len), return NULL);
	CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "  Tc Timer ............... : %u\n", fd_g_config->cnf_timer_tc), return NULL);
	CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "  Tw Timer ............... : %u\n", fd_g_config->cnf_timer_tw), return NULL);
	CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "  Local port ............. : %hu\n", fd_g_config->cnf_port), return NULL);
	CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "  Local secure port ...... : %hu\n", fd_g_config->cnf_port_tls), return NULL);
	if (fd_g_config->cnf_port_3436) {
		CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "  Local SCTP TLS port .... : %hu\n", fd_g_config->cnf_port_3436), return NULL);
	}
	CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "  Number of SCTP streams . : %hu\n", fd_g_config->cnf_sctp_str), return NULL);
	CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "  Number of clients thr .. : %d\n", fd_g_config->cnf_thr_srv), return NULL);
	CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "  Number of app threads .. : %hu\n", fd_g_config->cnf_dispthr), return NULL);
	if (FD_IS_LIST_EMPTY(&fd_g_config->cnf_endpoints)) {
		CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "  Local endpoints ........ : Default (use all available)\n"), return NULL);
	} else {
		CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "  Local endpoints ........ : "), return NULL);
		CHECK_MALLOC_DO( fd_ep_dump( FD_DUMP_STD_PARAMS, 0, 0, &fd_g_config->cnf_endpoints ), return NULL);
	}
	if (FD_IS_LIST_EMPTY(&fd_g_config->cnf_apps)) {
		CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "  Local applications ..... : (none)"), return NULL);
	} else {
		struct fd_list * li = fd_g_config->cnf_apps.next;
		CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "  Local applications ..... : "), return NULL);
		while (li != &fd_g_config->cnf_apps) {
			struct fd_app * app = (struct fd_app *)li;
			CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "App: %u,%s%s,Vnd:%u\t", 
					app->appid,
					app->flags.auth ? "Au" : "--",
					app->flags.acct ? "Ac" : "--",
					app->vndid), return NULL);
			li = li->next;
		}
	}
	
	CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "\n  Flags : - IP ........... : %s\n", fd_g_config->cnf_flags.no_ip4 ? "DISABLED" : "Enabled"), return NULL);
	CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "          - IPv6 ......... : %s\n", fd_g_config->cnf_flags.no_ip6 ? "DISABLED" : "Enabled"), return NULL);
	CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "          - Relay app .... : %s\n", fd_g_config->cnf_flags.no_fwd ? "DISABLED" : "Enabled"), return NULL);
	CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "          - TCP .......... : %s\n", fd_g_config->cnf_flags.no_tcp ? "DISABLED" : "Enabled"), return NULL);
	#ifdef DISABLE_SCTP
	CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "          - SCTP ......... : DISABLED (at compilation)\n"), return NULL);
	#else /* DISABLE_SCTP */
	CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "          - SCTP ......... : %s\n", fd_g_config->cnf_flags.no_sctp ? "DISABLED" : "Enabled"), return NULL);
	#endif /* DISABLE_SCTP */
	CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "          - Pref. proto .. : %s\n", fd_g_config->cnf_flags.pr_tcp ? "TCP" : "SCTP"), return NULL);
	CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "          - TLS method ... : %s\n", fd_g_config->cnf_flags.tls_alg ? "INBAND" : "Separate port"), return NULL);
	
	CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "  TLS :   - Certificate .. : %s\n", fd_g_config->cnf_sec_data.cert_file ?: "(NONE)"), return NULL);
	CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "          - Private key .. : %s\n", fd_g_config->cnf_sec_data.key_file ?: "(NONE)"), return NULL);
	CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "          - CA (trust) ... : %s (%d certs)\n", fd_g_config->cnf_sec_data.ca_file ?: "(none)", fd_g_config->cnf_sec_data.ca_file_nr), return NULL);
	CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "          - CRL .......... : %s\n", fd_g_config->cnf_sec_data.crl_file ?: "(none)"), return NULL);
	CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "          - Priority ..... : %s\n", fd_g_config->cnf_sec_data.prio_string ?: "(default: '" GNUTLS_DEFAULT_PRIORITY "')"), return NULL);
	if (fd_g_config->cnf_sec_data.dh_file) {
		CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "          - DH file ...... : %s\n", fd_g_config->cnf_sec_data.dh_file), return NULL);
	} else {
		CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "          - DH bits ...... : %d\n", fd_g_config->cnf_sec_data.dh_bits ?: GNUTLS_DEFAULT_DHBITS), return NULL);
	}
	
	CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "  Origin-State-Id ........ : %u", fd_g_config->cnf_orstateid), return NULL);
	
	return *buf;
}

/* read contents of a file opened in "rb" mode and alloc this data into a gnutls_datum_t (must be freed afterwards) */
int fd_conf_stream_to_gnutls_datum(FILE * pemfile, gnutls_datum_t *out)
{
	size_t alloc = 0;
	
	CHECK_PARAMS( pemfile && out );
	memset(out, 0, sizeof(gnutls_datum_t));

	do {
		uint8_t * realloced = NULL;
		size_t read = 0;

		if (alloc < out->size + BUFSIZ + 1) {
			alloc += alloc / 2 + BUFSIZ + 1;
			CHECK_MALLOC_DO( realloced = realloc(out->data, alloc),
				{
					free(out->data);
					return ENOMEM;
				} )
			out->data = realloced;
		}

		read = fread( out->data + out->size, 1, alloc - out->size - 1, pemfile );
		out->size += read;

		if (ferror(pemfile)) {
			int err = errno;
			TRACE_DEBUG(INFO, "An error occurred while reading file: %s", strerror(err));
			return err; 
		}
	} while (!feof(pemfile));
	
	out->data[out->size] = '\0';
	return 0;
}

#ifdef GNUTLS_VERSION_300
/* inspired from GnuTLS manual */
static int fd_conf_print_details_func (gnutls_x509_crt_t cert,
                    gnutls_x509_crt_t issuer, gnutls_x509_crl_t crl,
                    unsigned int verification_output)
{
  char name[512];
  char issuer_name[512];
  size_t name_size;
  size_t issuer_name_size;
  
  if (!TRACE_BOOL(GNUTLS_DBG_LEVEL))
	  return 0;

  issuer_name_size = sizeof (issuer_name);
  gnutls_x509_crt_get_issuer_dn (cert, issuer_name, &issuer_name_size);

  name_size = sizeof (name);
  gnutls_x509_crt_get_dn (cert, name, &name_size);

  fd_log_debug("\tSubject: %s", name);
  fd_log_debug("\tIssuer: %s", issuer_name);

  if (issuer != NULL)
    {
      issuer_name_size = sizeof (issuer_name);
      gnutls_x509_crt_get_dn (issuer, issuer_name, &issuer_name_size);

      fd_log_debug("\tVerified against: %s", issuer_name);
    }

  if (crl != NULL)
    {
      issuer_name_size = sizeof (issuer_name);
      gnutls_x509_crl_get_issuer_dn (crl, issuer_name, &issuer_name_size);

      fd_log_debug("\tVerified against CRL of: %s", issuer_name);
    }

  fd_log_debug("\tVerification output: %x", verification_output);

  return 0;
}
#endif /* GNUTLS_VERSION_300 */

#ifndef GNUTLS_VERSION_300
GCC_DIAG_OFF("-Wdeprecated-declarations")
#endif /* !GNUTLS_VERSION_300 */
/* Parse the configuration file (using the yacc parser) */
int fd_conf_parse()
{
	extern FILE * fddin;
	const char * orig = NULL;
	
	/* Attempt to find the configuration file */
	if (!fd_g_config->cnf_file)
		fd_g_config->cnf_file = FD_DEFAULT_CONF_FILENAME;
	
	fddin = fopen(fd_g_config->cnf_file, "r");
	if ((fddin == NULL) && (*fd_g_config->cnf_file != '/')) {
		char * new_cnf = NULL;
		/* We got a relative path, attempt to add the default directory prefix */
		orig = fd_g_config->cnf_file;
		CHECK_MALLOC( new_cnf = malloc(strlen(orig) + strlen(DEFAULT_CONF_PATH) + 2) ); /* we will not free it, but not important */
		sprintf( new_cnf, DEFAULT_CONF_PATH "/%s", orig );
		fd_g_config->cnf_file = new_cnf;
		fddin = fopen(fd_g_config->cnf_file, "r");
	}
	if (fddin == NULL) {
		int ret = errno;
		LOG_F("Unable to open configuration file for reading; tried the following locations: %s%s%s; Error: %s",
				  orig ?: "", orig? " and " : "", fd_g_config->cnf_file, strerror(ret));
		return ret;
	}
	
	/* call yacc parser */
	TRACE_DEBUG (FULL, "Parsing configuration file: %s", fd_g_config->cnf_file);
	CHECK_FCT(  fddparse(fd_g_config)  );
	
	/* close the file */
	fclose(fddin);
	
	/* Check that TLS private key was given */
	if (! fd_g_config->cnf_sec_data.key_file) {
		/* If TLS is not enabled, we allow empty TLS configuration */
		if ((fd_g_config->cnf_port_tls == 0) && (fd_g_config->cnf_flags.tls_alg == 0)) {
			LOG_N("TLS is disabled, this is *NOT* a recommended practice! Diameter protocol conveys highly sensitive information on your users.");
			fd_g_config->cnf_sec_data.tls_disabled = 1;
		} else {
			LOG_F( "Missing private key configuration for TLS. Please provide the TLS_cred configuration directive.");
			return EINVAL;
		}
	}
	
	/* Resolve hostname if not provided */
	if (fd_g_config->cnf_diamid == NULL) {
		char buf[HOST_NAME_MAX + 1];
		struct addrinfo hints, *info;
		int ret;
		
		/* local host name */
		CHECK_SYS(gethostname(buf, sizeof(buf)));
		
		/* get FQDN */
		memset(&hints, 0, sizeof hints);
		hints.ai_flags = AI_CANONNAME;

		ret = getaddrinfo(buf, NULL, &hints, &info);
		if (ret != 0) {
			TRACE_ERROR( "Error resolving local FQDN : '%s' : %s"
					". Please provide Identity in configuration file.",
					buf, gai_strerror(ret));
			return EINVAL;
		}
		fd_g_config->cnf_diamid = info->ai_canonname;
		CHECK_FCT( fd_os_validate_DiameterIdentity(&fd_g_config->cnf_diamid, &fd_g_config->cnf_diamid_len, 1) );
		freeaddrinfo(info);
	} else {
		CHECK_FCT( fd_os_validate_DiameterIdentity(&fd_g_config->cnf_diamid, &fd_g_config->cnf_diamid_len, 0) );
	}
	
	/* Handle the realm part */
	if (fd_g_config->cnf_diamrlm == NULL) {
		char * start = NULL;
		
		/* Check the diameter identity is a fqdn */
		start = strchr(fd_g_config->cnf_diamid, '.');
		if ((start == NULL) || (start[1] == '\0')) {
			TRACE_ERROR( "Unable to extract realm from the Identity '%s'."
					" Please fix your Identity setting or provide Realm.",
					fd_g_config->cnf_diamid);
			return EINVAL;
		}
		
		fd_g_config->cnf_diamrlm = start + 1;
		CHECK_FCT( fd_os_validate_DiameterIdentity(&fd_g_config->cnf_diamrlm, &fd_g_config->cnf_diamrlm_len, 1) );
	} else {
		CHECK_FCT( fd_os_validate_DiameterIdentity(&fd_g_config->cnf_diamrlm, &fd_g_config->cnf_diamrlm_len, 0) );
	}
	
	/* Validate some flags */
	if (fd_g_config->cnf_flags.no_ip4 && fd_g_config->cnf_flags.no_ip6) {
		TRACE_ERROR( "IP and IPv6 cannot be disabled at the same time.");
		return EINVAL;
	}
	if (fd_g_config->cnf_flags.no_tcp && fd_g_config->cnf_flags.no_sctp) {
		TRACE_ERROR( "TCP and SCTP cannot be disabled at the same time.");
		return EINVAL;
	}
	
	/* Validate local endpoints */
	if ((!FD_IS_LIST_EMPTY(&fd_g_config->cnf_endpoints)) && (fd_g_config->cnf_flags.no_ip4 || fd_g_config->cnf_flags.no_ip6)) {
		struct fd_list * li;
		for ( li = fd_g_config->cnf_endpoints.next; li != &fd_g_config->cnf_endpoints; li = li->next) {
			struct fd_endpoint * ep = (struct fd_endpoint *)li;
			if ( (fd_g_config->cnf_flags.no_ip4 && (ep->sa.sa_family == AF_INET))
			   ||(fd_g_config->cnf_flags.no_ip6 && (ep->sa.sa_family == AF_INET6)) ) {
				char sa_buf[sSA_DUMP_STRLEN];;
				li = li->prev;
				fd_list_unlink(&ep->chain);
				fd_sa_sdump_numeric(sa_buf, &ep->sa);
				LOG_N("Info: Removing local address conflicting with the flags no_IP / no_IP6 : %s", sa_buf);
				free(ep);
			}
		}
	}
	
	/* Configure TLS default parameters */
	if ((!fd_g_config->cnf_sec_data.tls_disabled) && (!fd_g_config->cnf_sec_data.prio_string)) {
		const char * err_pos = NULL;
		CHECK_GNUTLS_DO( gnutls_priority_init( 
					&fd_g_config->cnf_sec_data.prio_cache,
					GNUTLS_DEFAULT_PRIORITY,
					&err_pos),
				 { TRACE_ERROR("Error in priority string at position : %s", err_pos); return EINVAL; } );
	}
	
	/* Verify that our certificate is valid -- otherwise remote peers will reject it */
	if (!fd_g_config->cnf_sec_data.tls_disabled) {
		int ret = 0, i;
		
		gnutls_datum_t certfile;
		
		gnutls_x509_crt_t * certs = NULL;
		unsigned int cert_max = 0;
		
		
		/* Read the certificate file */
		FILE *stream = fopen (fd_g_config->cnf_sec_data.cert_file, "rb");
		if (!stream) {
			int err = errno;
			TRACE_DEBUG(INFO, "An error occurred while opening '%s': %s", fd_g_config->cnf_sec_data.cert_file, strerror(err));
			return err; 
		}
		CHECK_FCT( fd_conf_stream_to_gnutls_datum(stream, &certfile) );
		fclose(stream);
		
		/* Import the certificate(s) */
		GNUTLS_TRACE( ret = gnutls_x509_crt_list_import(NULL, &cert_max, &certfile, GNUTLS_X509_FMT_PEM, GNUTLS_X509_CRT_LIST_IMPORT_FAIL_IF_EXCEED) );
		if (ret != GNUTLS_E_SHORT_MEMORY_BUFFER) {
			CHECK_GNUTLS_DO(ret, return EINVAL);
		}
		
		CHECK_MALLOC( certs = calloc(cert_max, sizeof(gnutls_x509_crt_t)) );
		CHECK_GNUTLS_DO( gnutls_x509_crt_list_import(certs, &cert_max, &certfile, GNUTLS_X509_FMT_PEM, 
				#ifdef GNUTLS_VERSION_300
				GNUTLS_X509_CRT_LIST_FAIL_IF_UNSORTED
				#else /* GNUTLS_VERSION_300 */
				0
				#endif /* GNUTLS_VERSION_300 */
				),
			{
				TRACE_ERROR("Failed to import the data from file '%s'", fd_g_config->cnf_sec_data.cert_file);
				free(certfile.data);
				return EINVAL;
			} );
		free(certfile.data);
		
		ASSERT(cert_max >= 1);
		
		/* Now, verify the list against the local CA and CRL */
		
		#ifdef GNUTLS_VERSION_300
		
			/* We use the trust list for this purpose */
		{
			unsigned int output;
			
			gnutls_x509_trust_list_verify_named_crt (
						fd_g_config->cnf_sec_data.trustlist, 
						certs[0], 
						fd_g_config->cnf_diamid, 
						fd_g_config->cnf_diamid_len,
                                		0,
                                		&output, 
						fd_conf_print_details_func);

			/* if this certificate is not explicitly trusted verify against CAs 
			*/
			if (output != 0)
			{
				gnutls_x509_trust_list_verify_crt (
							fd_g_config->cnf_sec_data.trustlist, 
							certs, 
							cert_max,
                                			0,
                                			&output, 
							fd_conf_print_details_func);
			}
			
			if (output & GNUTLS_CERT_INVALID)
			{
				fd_log_debug("TLS: Local certificate chain '%s' is invalid :", fd_g_config->cnf_sec_data.cert_file);
				if (output & GNUTLS_CERT_SIGNER_NOT_FOUND)
					TRACE_ERROR(" - The certificate hasn't got a known issuer. Did you forget to specify TLS_CA ?");
				if (output & GNUTLS_CERT_SIGNER_NOT_CA)
					TRACE_ERROR(" - The certificate signer is not a CA, or uses version 1, or 3 without basic constraints.");
				if (output & GNUTLS_CERT_NOT_ACTIVATED)
					TRACE_ERROR(" - The certificate is not yet activated.");
				if (output & GNUTLS_CERT_EXPIRED)
					TRACE_ERROR(" - The certificate is expired.");
				return EINVAL;
			}
			
			/* Now check the subject matches our hostname */
			if (!gnutls_x509_crt_check_hostname (certs[0], fd_g_config->cnf_diamid))
			{
				TRACE_ERROR("TLS: The certificate owner does not match the hostname '%s'", fd_g_config->cnf_diamid);
				return EINVAL;
			}
			
		}
		

		#else /* GNUTLS_VERSION_300 */ 
		
			/* GnuTLS 2.x way of checking certificates */
		{
			gnutls_x509_crt_t * CA_list;
			int CA_list_length;

			gnutls_x509_crl_t * CRL_list;
			int CRL_list_length;
			
			unsigned int verify;
			time_t now;
			GNUTLS_TRACE( gnutls_certificate_get_x509_cas (fd_g_config->cnf_sec_data.credentials, &CA_list, (unsigned int *) &CA_list_length) );
			GNUTLS_TRACE( gnutls_certificate_get_x509_crls (fd_g_config->cnf_sec_data.credentials, &CRL_list, (unsigned int *) &CRL_list_length) );
			CHECK_GNUTLS_DO( gnutls_x509_crt_list_verify(certs, cert_max, CA_list, CA_list_length, CRL_list, CRL_list_length, 0, &verify),
				{
					TRACE_ERROR("Failed to verify the local certificate '%s' against local credentials. Please check your certificate is valid.", fd_g_config->cnf_sec_data.cert_file);
					return EINVAL;
				} );
				
			if (verify) {
				fd_log_debug("TLS: Local certificate chain '%s' is invalid :", fd_g_config->cnf_sec_data.cert_file);
				if (verify & GNUTLS_CERT_INVALID)
					TRACE_ERROR(" - The certificate is not trusted (unknown CA? expired?)");
				if (verify & GNUTLS_CERT_REVOKED)
					TRACE_ERROR(" - The certificate has been revoked.");
				if (verify & GNUTLS_CERT_SIGNER_NOT_FOUND)
					TRACE_ERROR(" - The certificate hasn't got a known issuer.");
				if (verify & GNUTLS_CERT_SIGNER_NOT_CA)
					TRACE_ERROR(" - The certificate signer is not a CA, or uses version 1, or 3 without basic constraints.");
				if (verify & GNUTLS_CERT_INSECURE_ALGORITHM)
					TRACE_ERROR(" - The certificate signature uses a weak algorithm.");
				return EINVAL;
			}

			/* Check the local Identity is valid with the certificate */
			if (!gnutls_x509_crt_check_hostname (certs[0], fd_g_config->cnf_diamid)) {
				TRACE_ERROR("TLS: Local certificate '%s' is invalid :", fd_g_config->cnf_sec_data.cert_file);
				TRACE_ERROR(" - The certificate hostname does not match '%s'", fd_g_config->cnf_diamid);
				return EINVAL;
			}

			/* Check validity of all the certificates in the chain */
			now = time(NULL);
			for (i = 0; i < cert_max; i++)
			{
				time_t deadline;

				GNUTLS_TRACE( deadline = gnutls_x509_crt_get_expiration_time(certs[i]) );
				if ((deadline != (time_t)-1) && (deadline < now)) {
					TRACE_ERROR("TLS: Local certificate chain '%s' is invalid :", fd_g_config->cnf_sec_data.cert_file);
					TRACE_ERROR(" - The certificate %d in the chain is expired", i);
					return EINVAL;
				}

				GNUTLS_TRACE( deadline = gnutls_x509_crt_get_activation_time(certs[i]) );
				if ((deadline != (time_t)-1) && (deadline > now)) {
					TRACE_ERROR("TLS: Local certificate chain '%s' is invalid :", fd_g_config->cnf_sec_data.cert_file);
					TRACE_ERROR(" - The certificate %d in the chain is not yet activated", i);
					return EINVAL;
				}
			}
		}
		#endif /* GNUTLS_VERSION_300 */ 
		
		/* Everything checked OK, free the certificate list */
		for (i = 0; i < cert_max; i++)
		{
			GNUTLS_TRACE( gnutls_x509_crt_deinit (certs[i]) );
		}
		free(certs);
	
		#ifdef GNUTLS_VERSION_300
		/* Use certificate verification during the handshake */
		gnutls_certificate_set_verify_function (fd_g_config->cnf_sec_data.credentials, fd_tls_verify_credentials_2);
		#endif /* GNUTLS_VERSION_300 */

	}
	
	/* gnutls_certificate_set_verify_limits -- so far the default values are fine... */
	
	/* DH */
	if (!fd_g_config->cnf_sec_data.tls_disabled) {
		if (fd_g_config->cnf_sec_data.dh_file) {
			gnutls_datum_t dhparams = { NULL, 0 };
			size_t alloc = 0;
			FILE *stream = fopen (fd_g_config->cnf_sec_data.dh_file, "rb");
			if (!stream) {
				int err = errno;
				TRACE_DEBUG(INFO, "An error occurred while opening '%s': %s", fd_g_config->cnf_sec_data.dh_file, strerror(err));
				return err; 
			}
			do {
				uint8_t * realloced = NULL;
				size_t read = 0;

				if (alloc < dhparams.size + BUFSIZ + 1) {
					alloc += alloc / 2 + BUFSIZ + 1;
					CHECK_MALLOC_DO( realloced = realloc(dhparams.data, alloc),
						{
							free(dhparams.data);
							return ENOMEM;
						} )
					dhparams.data = realloced;
				}

				read = fread( dhparams.data + dhparams.size, 1, alloc - dhparams.size - 1, stream );
				dhparams.size += read;

				if (ferror(stream)) {
					int err = errno;
					TRACE_DEBUG(INFO, "An error occurred while reading '%s': %s", fd_g_config->cnf_sec_data.dh_file, strerror(err));
					return err; 
				}
			} while (!feof(stream));
			dhparams.data[dhparams.size] = '\0';
			fclose(stream);
			CHECK_GNUTLS_DO( gnutls_dh_params_import_pkcs3( 
						fd_g_config->cnf_sec_data.dh_cache,
						&dhparams,
						GNUTLS_X509_FMT_PEM),
						 { TRACE_ERROR("Error in DH bits value : %d", fd_g_config->cnf_sec_data.dh_bits ?: GNUTLS_DEFAULT_DHBITS); return EINVAL; } );
			free(dhparams.data);

		} else {
			LOG_D( "Generating fresh Diffie-Hellman parameters of size %d (this takes some time)... ", fd_g_config->cnf_sec_data.dh_bits ?: GNUTLS_DEFAULT_DHBITS);
			CHECK_GNUTLS_DO( gnutls_dh_params_generate2( 
						fd_g_config->cnf_sec_data.dh_cache,
						fd_g_config->cnf_sec_data.dh_bits ?: GNUTLS_DEFAULT_DHBITS),
						 { TRACE_ERROR("Error in DH bits value : %d", fd_g_config->cnf_sec_data.dh_bits ?: GNUTLS_DEFAULT_DHBITS); return EINVAL; } );
		}			
	
	}
	
	return 0;
}
#ifndef GNUTLS_VERSION_300
GCC_DIAG_ON("-Wdeprecated-declarations")
#endif /* !GNUTLS_VERSION_300 */


/* Destroy contents of fd_g_config structure */
int fd_conf_deinit()
{
	TRACE_ENTRY();
	
	if (!fd_g_config)
		return 0;
	
	/* Free the TLS parameters */
#ifdef GNUTLS_VERSION_300
	gnutls_x509_trust_list_deinit(fd_g_config->cnf_sec_data.trustlist, 1);
#endif /* GNUTLS_VERSION_300 */
	gnutls_priority_deinit(fd_g_config->cnf_sec_data.prio_cache);
	gnutls_dh_params_deinit(fd_g_config->cnf_sec_data.dh_cache);
	gnutls_certificate_free_credentials(fd_g_config->cnf_sec_data.credentials);
	
	free(fd_g_config->cnf_sec_data.cert_file); fd_g_config->cnf_sec_data.cert_file = NULL;
	free(fd_g_config->cnf_sec_data.key_file); fd_g_config->cnf_sec_data.key_file = NULL;
	free(fd_g_config->cnf_sec_data.ca_file); fd_g_config->cnf_sec_data.ca_file = NULL;
	free(fd_g_config->cnf_sec_data.crl_file); fd_g_config->cnf_sec_data.crl_file = NULL;
	free(fd_g_config->cnf_sec_data.prio_string); fd_g_config->cnf_sec_data.prio_string = NULL;
	free(fd_g_config->cnf_sec_data.dh_file); fd_g_config->cnf_sec_data.dh_file = NULL;
	
	/* Destroy dictionary */
	CHECK_FCT_DO( fd_dict_fini(&fd_g_config->cnf_dict), );
	
	/* Destroy the main event queue */
	CHECK_FCT_DO( fd_fifo_del(&fd_g_config->cnf_main_ev), );
	
	/* Destroy the local endpoints and applications */
	CHECK_FCT_DO(fd_ep_filter(&fd_g_config->cnf_endpoints, 0 ), );
	CHECK_FCT_DO(fd_app_empty(&fd_g_config->cnf_apps ), );
	
	/* Destroy the local identity */	
	free(fd_g_config->cnf_diamid); fd_g_config->cnf_diamid = NULL;
	free(fd_g_config->cnf_diamrlm); fd_g_config->cnf_diamrlm = NULL;
	
	/* Destroy the untrusted AVP vendors */
	free(fd_g_config->cnf_untrusted_avp_vendors); fd_g_config->cnf_untrusted_avp_vendors = NULL;
	return 0;
}


