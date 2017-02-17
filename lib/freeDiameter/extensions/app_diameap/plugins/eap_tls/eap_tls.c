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


#include "eap_tls.h"

int eap_tls_configure(char * configfile);
int eap_tls_init(struct eap_state_machine *smd);
int eap_tls_initPickUp(struct eap_state_machine *smd);
int eap_tls_buildReq(struct eap_state_machine *smd, u8 eap_md5,
		struct eap_packet * eapPacket);
int eap_tls_getTimeout(struct eap_state_machine *smd, int * timeout);
boolean eap_tls_check(struct eap_state_machine *smd,
		struct eap_packet *eapRespData);
int eap_tls_process(struct eap_state_machine *smd,
		struct eap_packet *eapRespData);
boolean eap_tls_isDone(struct eap_state_machine *smd);
int eap_tls_getKey(struct eap_state_machine *smd, u8** msk, int * msklen, u8** emsk, int * emsklen);
void eap_tls_unregister(void);
void eap_tls_free(void * data);

REGISTER_METHOD("eap_tls", "eap_tls_configure", "eap_tls_init", "eap_tls_initPickUp", "eap_tls_buildReq", "eap_tls_getTimeout", "eap_tls_check", "eap_tls_process", "eap_tls_isDone", "eap_tls_getKey", "eap_tls_unregister", "eap_tls_free")
;

int eap_tls_configure(char * configfile)
{
	int ret;
	extern FILE * eaptlsin;

	if (configfile)
	{
		tls_global_conf.conffile = configfile;
	}
	tls_global_conf.certfile = NULL;
	tls_global_conf.keyfile = NULL;
	tls_global_conf.cafile = NULL;
	tls_global_conf.crlfile = NULL;
	tls_global_conf.check_cert_cn_username = FALSE;

	/*Parse EAP TLS configuration file */
	eaptlsin = fopen(tls_global_conf.conffile, "r");
	if (!eaptlsin)
	{
		TRACE_DEBUG(INFO,"%s[EAP TLS plugin] Unable to open configuration file %s for reading: %s",DIAMEAP_EXTENSION, tls_global_conf.conffile, strerror(errno));
		return errno;
	}

	/* call yacc parser */
	CHECK_FCT(eaptlsparse(&tls_global_conf));


	tls_global_conf.max_size = 64*1024 /* As per RFC 5216 recommendation */;

	/* Initializing GnuTLS library */
	ret = diameap_tls_init(&tls_global_conf);

	return ret;
}

int eap_tls_init(struct eap_state_machine *smd)
{
	int ret;
	struct tls_data *data = NULL;
	CHECK_MALLOC(data = malloc(sizeof(struct tls_data)));
	memset(data, 0, sizeof(struct tls_data));
	CHECK_FCT(diameap_tls_initialize(data));
	ret = diameap_tls_init_session(&tls_global_conf, data);

	smd->methodData = (struct tls_data*) data;
	if (ret < 0)
	{
		return ret;
	}
	return 0;
}

int eap_tls_initPickUp(struct eap_state_machine *smd)
{
	return 0;
}

int eap_tls_buildReq(struct eap_state_machine *smd, u8 id,
		struct eap_packet * eapPacket)
{
	struct tls_data * data;
	data = (struct tls_data *) smd->methodData;

	if (data->more_toreceive == TRUE)
	{
		CHECK_FCT(diameap_eap_tls_buildReq_ack(id,eapPacket));
		return 0;
	}

	if (data->state == START)
	{
		CHECK_FCT(diameap_eap_tls_buildReq_start(id,eapPacket));
		return 0;
	}

	if (data->state == CONTINUE)
	{
		diameap_eap_tls_buildReq_data(data, id, eapPacket);

		smd->methodData = (struct tls_data*) data;
		return 0;
	}

	return 0;
}

int eap_tls_getTimeout(struct eap_state_machine *smd, int * timeout)
{
	return 0;
}

boolean eap_tls_check(struct eap_state_machine *smd,
		struct eap_packet *eapRespData)
{
	eap_type type;
	if(diameap_eap_get_type(eapRespData,&type)!=0){
		goto cf;
	}
	if (type == TYPE_EAP_TLS)
	{
		return TRUE;
	}
cf:
	TRACE_DEBUG(INFO,"%s[EAP TLS plugin] EAP-TLS check failed: Received EAP packet with different EAP-Type (Type = %d)",DIAMEAP_EXTENSION, type);
	return FALSE;
}

int eap_tls_process(struct eap_state_machine *smd,
		struct eap_packet *eapRespData)
{
	struct tls_data * data;
	data = (struct tls_data *) smd->methodData;
	struct tls_msg tlsmsg;
	CHECK_FCT(diameap_eap_tls_parse(&tlsmsg,eapRespData));

	if ((tlsmsg.datalength == 0))
	{
		if (data->more_tosend_length > 0)
		{
			//ACK and more to send
			return 0;
		}
		else
		{
			//Success
			if (data->handshake == TRUE)
			{
				data->state = SUCCESS;
				smd->user.success = TRUE;

				if(tls_global_conf.check_cert_cn_username == TRUE){
					unsigned int list_size;
					const gnutls_datum_t * list = gnutls_certificate_get_peers (data->session, &list_size);
					if(list_size<1){
						goto failure;
					}

					gnutls_x509_crt_t cert;
		
					CHECK_GNUTLS_DO(gnutls_x509_crt_init(&cert),{
						TRACE_DEBUG(NONE,"%s[EAP TLS plugin] [GnuTLS] error in initialization crt init",DIAMEAP_EXTENSION);
						goto failure;});
					
					CHECK_GNUTLS_DO(gnutls_x509_crt_import(cert, &list[0], GNUTLS_X509_FMT_DER), {
						TRACE_DEBUG(NONE,"%s[EAP TLS plugin] [GnuTLS] error parsing certificate",DIAMEAP_EXTENSION);
						goto failure;});

					void * buff;
					size_t size_buffer;
					int ret;
					ret = gnutls_x509_crt_get_dn_by_oid(cert,GNUTLS_OID_X520_COMMON_NAME,0,0,NULL,&size_buffer);
					if( ret != GNUTLS_E_SHORT_MEMORY_BUFFER){
						CHECK_GNUTLS_DO(ret,{
							TRACE_DEBUG(NONE,"%s[EAP TLS plugin] [GnuTLS] error get dn by oid",DIAMEAP_EXTENSION);
							goto failure;});
					}

					CHECK_MALLOC_DO(buff=malloc(size_buffer), goto failure);

					CHECK_GNUTLS_DO(gnutls_x509_crt_get_dn_by_oid(cert,GNUTLS_OID_X520_COMMON_NAME,0,0,buff,&size_buffer),{
						TRACE_DEBUG(NONE,"%s[EAP TLS plugin] [GnuTLS] error get dn by oid",DIAMEAP_EXTENSION);
						goto failure;});

					if(strncmp((char *)smd->user.userid,buff,smd->user.useridLength)!=0){
						goto failure;
					}

					gnutls_x509_crt_deinit(cert);				
					goto next;

					failure:
					TRACE_DEBUG(NONE,"%s[EAP TLS plugin] Checking failed. certificate's CN does not match User_Name AVP value.",DIAMEAP_EXTENSION);
					data->state = FAILURE;
					smd->user.success = FALSE;
					gnutls_x509_crt_deinit(cert);
				}

				next:
				smd->methodData = (struct tls_data*) data;
				return 0;

			}

			return 0;
		}

	}

	if (data->more_toreceive == TRUE)
	{
		//reassemble received fragment to TLS Response
		CHECK_FCT(diameap_tls_reassemble(&data->tlsResp,tlsmsg));
	}
	else
	{
		//receive the first fragment or a complete TLS message
		CHECK_FCT(diameap_tls_copy(&data->tlsResp,tlsmsg));
	}

	if (tlsmsg.flags & TLS_FLAG_MORE)
	{
		data->more_toreceive = TRUE;
		smd->methodData = (struct tls_data*) data;
		return 0;
	}
	else
	{
		//last fragment received
		data->more_toreceive = FALSE;
	}
	data->state = CONTINUE;
	diameap_tls_process_receive(data);

	if (data->state == SUCCESS)
	{
		smd->user.success = TRUE;
	}
	smd->methodData = (struct tls_data*) data;
	return 0;
}

boolean eap_tls_isDone(struct eap_state_machine *smd)
{
	struct tls_data * data;
	data = (struct tls_data *) smd->methodData;
	if (data->state == CONTINUE || data->state == START)
	{
		return FALSE;
	}
	return TRUE;
}

int eap_tls_getKey(struct eap_state_machine *smd, u8 ** msk, int *msklen, u8 ** emsk, int *emsklen)
{
	struct tls_data * data;
	int len = emsk ? 128 : 64;
	data = (struct tls_data *) smd->methodData;
	*msk = malloc(len);
	if (gnutls_prf(data->session, strlen("client EAP encryption"),
			"client EAP encryption", 0, 0, NULL, len, (char *) *msk)
			!= GNUTLS_E_SUCCESS)
	{
		free(*msk);
		*msk = NULL;
		*msklen = 0;
		return 1;
	}
	else
	{
		*msklen = 64;
	}
	if (emsk) {
		*emsk = malloc(64);
		memcpy(*emsk, (*msk)+64, 64);
		memset((*msk)+64, 0, 64);
		*emsklen = 64;
	}

	return 0;
}

void eap_tls_unregister(void)
{
	//
}

void eap_tls_free(void * mdata)
{
	struct tls_data *data;
	data = (struct tls_data*) mdata;
	gnutls_deinit(data->session);
	if(data->tlsReq.data){
		free(data->tlsReq.data);
		data->tlsReq.data=NULL;
	}
	if(data->tlsResp.data){
		free(data->tlsResp.data);
		data->tlsResp.data=NULL;
	}
	free(data);
	data=NULL;
}

//send TLS ACK Request (empty TLS msg)
int diameap_eap_tls_buildReq_ack(u8 id, struct eap_packet * eapPacket)
{
	u8* payload;
	struct tls_msg tlsmsg;
	int len;
	CHECK_FCT(diameap_tls_new(&tlsmsg));
	CHECK_FCT(diameap_tls_new_tls_packet(&payload,&len,tlsmsg));
	CHECK_FCT(diameap_eap_new(EAP_REQUEST,id,TYPE_EAP_TLS,payload,len,eapPacket));
	return 0;
}

// parse EAP TLS msg
int diameap_eap_tls_parse(struct tls_msg * tlsmsg, struct eap_packet *eapPacket)
{
	u8 *datatls;
	int len;

	//initialize a new empty EAP TLS msg
	diameap_tls_new(tlsmsg);
	//retrieve the data field from EAP Packet
	diameap_eap_get_data(eapPacket, &datatls, &len);
	//parse EAP TLS msg
	diameap_tls_parse(datatls, len, tlsmsg);
	return 0;
}

int diameap_eap_tls_buildReq_start(u8 id, struct eap_packet * eapPacket)
{
	u8* payload;
	struct tls_msg tlsmsg;
	int len;
	CHECK_FCT(diameap_tls_new(&tlsmsg));
	CHECK_FCT(diameap_tls_set_flags(&tlsmsg,TLS_FLAG_START));
	CHECK_FCT(diameap_tls_new_tls_packet(&payload,&len,tlsmsg));
	CHECK_FCT(diameap_eap_new(EAP_REQUEST,id,TYPE_EAP_TLS,payload,len,eapPacket));
	return 0;
}

int diameap_eap_tls_buildReq_data(struct tls_data * data, int id,
		struct eap_packet * eapPacket)
{
	struct tls_msg tlsmsg;
	u8* datatosend;
	u8 * eaptls_data;
	int length = 0;

	diameap_tls_new(&tlsmsg);

	if (data->more_tosend_length == 0)
	{
		//First fragment of message or the only fragment of message
		data->more_tosend_length = data->tlsReq.datalength;
	}
	if (data->more_tosend_length > tls_global_conf.max_size)
	{
		//New fragment of message. Is not the last fragment.
		length = tls_global_conf.max_size;
		CHECK_FCT(diameap_tls_set_flags(&tlsmsg,TLS_FLAG_MORE));
		if (data->more_tosend_length == data->tlsReq.datalength)
		{
			//The first fragment of message
			CHECK_FCT(diameap_tls_set_message_length(&tlsmsg,data->tlsReq.datalength));//set L flag and length value
		}
	}
	else
	{
		//The last fragment or the only fragment.
		length = data->more_tosend_length;
	}

	datatosend = malloc(sizeof(u8) * length);
	U8COPY(datatosend,0,length,data->tlsReq.data+(data->tlsReq.datalength-data->more_tosend_length));
	data->more_tosend_length -= length;
	CHECK_FCT(diameap_tls_set_data(&tlsmsg,datatosend,length));

	CHECK_FCT(diameap_tls_new_tls_packet(&eaptls_data,&length,tlsmsg));
	CHECK_FCT(diameap_eap_new(EAP_REQUEST,id,TYPE_EAP_TLS,eaptls_data,length,eapPacket));

	if (data->more_tosend_length == 0)
	{
		diameap_tls_new(&data->tlsReq);
	}
	return 0;
}
