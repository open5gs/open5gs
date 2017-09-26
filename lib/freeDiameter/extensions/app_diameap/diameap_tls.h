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


#ifndef DIAMEAP_TLS_H_
#define DIAMEAP_TLS_H_

#if defined(__GNUC__)
# define GCC_DIAG_DO_PRAGMA(x) _Pragma (#x)
# define GCC_DIAG_PRAGMA(x) GCC_DIAG_DO_PRAGMA(GCC diagnostic x)
# if ((__GNUC__ * 100) + __GNUC_MINOR__) >= 406		/* 4.6.x */
#  define GCC_DIAG_OFF(x) GCC_DIAG_PRAGMA(push) \
     GCC_DIAG_PRAGMA(ignored x)
#  define GCC_DIAG_ON(x) GCC_DIAG_PRAGMA(pop)
# else							/* older */
#  define GCC_DIAG_OFF(x) GCC_DIAG_PRAGMA(ignored x)
#  define GCC_DIAG_ON(x)  GCC_DIAG_PRAGMA(warning x)
# endif
#else
# define GCC_DIAG_OFF(x)
# define GCC_DIAG_ON(x)
#endif


#include "diameap_defs.h"
#include <gnutls/gnutls.h>
GCC_DIAG_OFF("-Wdeprecated-declarations")
#include <gcrypt.h>
GCC_DIAG_ON("-Wdeprecated-declarations")
#include <errno.h>
#include <pthread.h>



#define TLS_FLAG_LENGTH	0x80
#define TLS_FLAG_MORE	0x40
#define TLS_FLAG_START	0x20

struct tls_config{
	char * conffile;
	//certificates
	char * keyfile;
	char * certfile;
	char * cafile;
	char * crlfile;

	//configuration parameters
	boolean check_cert_cn_username;

	int max_size;

	gnutls_certificate_credentials_t cert_cred;
};

struct tls_msg{
	u8	flags;
	u32 length;
	u8 *data;
	u32 datalength;
};

struct tls_data
{
	gnutls_session_t session;
	enum { START, CONTINUE, SUCCESS, FAILURE } state;
	struct tls_msg tlsReq;
	int p_length; 			/* length of buffer still not returned to handshake */
	struct tls_msg tlsResp;
	int more_tosend_length;	/* 0 if no more fragment to send. Otherwise the length of the buff not yet sent */
	boolean more_toreceive; /* TRUE if wait for more fragments. */
	boolean handshake; 		/* TRUE if handshake terminated successful. */
};

int diameap_tls_init(struct tls_config * tls_conf );
void diameap_tls_log(int lev, const char * text);
int diameap_tls_init_session(struct tls_config * tls_conf, struct tls_data * data);
int diameap_tls_new(struct tls_msg * tlsmsg);

int diameap_tls_get_message_length(struct tls_msg tlsmsg, u32 * length);
int diameap_tls_set_message_length(struct tls_msg * tlsmsg, u32 length);

int diameap_tls_get_flags(struct tls_msg tlsmsg, u8 * flags);
int diameap_tls_set_flags(struct tls_msg * tlsmsg, u8 flags);

int diameap_tls_get_data(struct tls_msg tlsmsg, u8** tls_data, u32 * data_length);
int diameap_tls_set_data(struct tls_msg * tlsmsg, u8* tls_data, int data_length);

int diameap_tls_parse(u8* data, int length, struct tls_msg * tlsmsg);

int diameap_tls_new_tls_packet(u8** data, int * len, struct tls_msg tlsmsg);

int diameap_set_tls(struct tls_msg * tlsmsg, u8 flags, u32 length,u8 *tls_data, int data_length);

void diameap_tls_dump(struct tls_msg tlsmsg);

int diameap_tls_initialize(struct tls_data * data);

int diameap_tls_reassemble(struct tls_msg * to,struct tls_msg from);

int diameap_tls_copy(struct tls_msg * to,struct tls_msg from);

int diameap_tls_process_receive(struct tls_data * data);

ssize_t diameap_tls_receive(gnutls_transport_ptr_t data , void *buffer, size_t length);
ssize_t diameap_tls_send(gnutls_transport_ptr_t data , const void *buffer, size_t length);

#endif /* DIAMEAP_TLS_H_ */
