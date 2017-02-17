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


#include "diameap_tls.h"

//GCRY_THREAD_OPTION_PTHREAD_IMPL;

int diameap_tls_init(struct tls_config * tls_conf)
{
	int ret;
	ret = gnutls_global_init();
	if (ret < 0)
	{
		gnutls_perror(ret);
	}

	gnutls_global_set_log_function(diameap_tls_log);
	//gnutls_global_set_log_level(9);


	if(tls_conf->cafile ==NULL){
		fprintf(stderr,"[DiamEAP extension] [EAP TLS] Missing certification authority (CA) certificates. Please provide CA configuration directive.\n"); 
		return EINVAL;
	}
	if( !tls_conf->certfile || !tls_conf->keyfile){
		fprintf(stderr,"[DiamEAP extension] [EAP TLS] Missing private Key. Please provide Cred configuration directive.\n"); 
		return EINVAL;
	}

	ret = gnutls_certificate_allocate_credentials(&tls_conf->cert_cred);

	if (ret < 0)
	{
		gnutls_perror(ret);
		return ret;
	}

	ret = gnutls_certificate_set_x509_trust_file(tls_conf->cert_cred,
			tls_conf->cafile, GNUTLS_X509_FMT_PEM);
	if (ret < 0)
	{
		gnutls_perror(ret);
		return ret;
	}
	if (tls_conf->crlfile)
	{
		ret = gnutls_certificate_set_x509_crl_file(tls_conf->cert_cred,
				tls_conf->crlfile, GNUTLS_X509_FMT_PEM);
		if (ret < 0)
		{
			gnutls_perror(ret);
			return ret;
		}
	}

	ret = gnutls_certificate_set_x509_key_file(tls_conf->cert_cred,
			tls_conf->certfile, tls_conf->keyfile, GNUTLS_X509_FMT_PEM);
	if (ret < 0)
	{
		gnutls_perror(ret);
		return ret;
	}
	return 0;
}

void diameap_tls_log(int lev, const char * text)
{
	u8 * msg;
	if (text == NULL)
		return;
	msg = (u8 *) strdup(text);
	int i;
	for (i = 0; (G8(text+i) != '\n') && (G8(text+i) != '\0'); i++)
	{
	}
	P8((msg+i),'\0');
	fprintf(stderr, "[DiamEAP extension] [EAP TLS] GNUTLS log[%d] : %s\n", lev, msg);
	free(msg);
}

int diameap_tls_init_session(struct tls_config * tls_conf,
		struct tls_data * data)
{
	int ret;
	ret = gnutls_init(&data->session, GNUTLS_SERVER);
	if (ret < 0)
	{
		gnutls_perror(ret);
	}
	ret = gnutls_set_default_priority(data->session);
	if (ret < 0)
	{
		gnutls_perror(ret);
	}

	ret = gnutls_credentials_set(data->session, GNUTLS_CRD_CERTIFICATE,
			tls_conf->cert_cred);
	if (ret < 0)
	{
		gnutls_perror(ret);
	}

	/* request client certificate if any.
	 */
	gnutls_certificate_server_set_request(data->session, GNUTLS_CERT_REQUIRE); //GNUTLS_CERT_REQUEST);

	gnutls_transport_set_pull_function(data->session, diameap_tls_receive);
	gnutls_transport_set_push_function(data->session, diameap_tls_send);
	gnutls_transport_set_ptr(data->session, (gnutls_transport_ptr_t) data);

	/* starting version 2.12, this call is not needed */
	//gnutls_transport_set_lowat(data->session, 0);
	
	return ret;
}

ssize_t diameap_tls_receive(gnutls_transport_ptr_t ptr, void *buffer,
		size_t length)
{
	struct tls_data * data = (struct tls_data *) ptr;
	if (data->p_length == 0)
	{
		errno = EWOULDBLOCK;
		return -1;
	}
	if (length > data->p_length)
	{
		length = data->p_length;
	}
	memcpy(buffer, data->tlsResp.data + (data->tlsResp.datalength
			- data->p_length), length);
	data->p_length -= length;

	return length;
}

ssize_t diameap_tls_send(gnutls_transport_ptr_t ptr, const void *buffer,
		size_t length)
{
	struct tls_data * data = (struct tls_data *) ptr;
	data->tlsReq.data = realloc(data->tlsReq.data, data->tlsReq.datalength
			+ length);
	U8COPY(data->tlsReq.data,data->tlsReq.datalength,length,(u8*)buffer);
	data->tlsReq.datalength += length;

	return length;
}

int diameap_tls_new(struct tls_msg * tlsmsg)
{
	if (!tlsmsg)
	{
		return EINVAL;
	}
	tlsmsg->flags = 0x00;
	tlsmsg->length = 0;
	tlsmsg->data = NULL;
	tlsmsg->datalength = 0;
	return 0;
}

int diameap_tls_get_flags(struct tls_msg tlsmsg, u8 * flags)
{
	*flags = tlsmsg.flags;
	return 0;
}

int diameap_tls_set_flags(struct tls_msg * tlsmsg, u8 flags)
{
	if (!tlsmsg)
	{
		return EINVAL;
	}
	if ((flags & TLS_FLAG_LENGTH) == TLS_FLAG_LENGTH)
	{
		flags = flags ^ TLS_FLAG_LENGTH;
	}
	tlsmsg->flags = tlsmsg->flags | flags;
	return 0;
}

int diameap_tls_get_message_length(struct tls_msg tlsmsg, u32 * length)
{
	if ((tlsmsg.flags & TLS_FLAG_LENGTH) == TLS_FLAG_LENGTH)
	{
		*length = tlsmsg.length;
	}
	else
	{
		*length = 0;
	}
	return 0;
}

int diameap_tls_set_message_length(struct tls_msg * tlsmsg, u32 length)
{
	if (!tlsmsg)
	{
		return EINVAL;
	}
	if (length > 0)
	{
		tlsmsg->length = length;
		tlsmsg->flags = tlsmsg->flags | TLS_FLAG_LENGTH;
	}
	else
	{
		tlsmsg->length = 0;
		if ((tlsmsg->flags & TLS_FLAG_LENGTH) == TLS_FLAG_LENGTH)
		{
			tlsmsg->flags = tlsmsg->flags ^ TLS_FLAG_LENGTH;
		}
	}
	return 0;
}

int diameap_tls_get_data(struct tls_msg tlsmsg, u8** tls_data,
		u32 * data_length)
{
	if (tlsmsg.datalength > 0)
	{
		*tls_data = malloc(sizeof(u8) * tlsmsg.datalength);
		U8COPY(*tls_data,0,tlsmsg.datalength,tlsmsg.data);
		*data_length = tlsmsg.datalength;
	}
	else
	{
		*tls_data = NULL;
		*data_length = 0;
	}
	return 0;
}

int diameap_tls_set_data(struct tls_msg * tlsmsg, u8* tls_data, int data_length)
{
	if (!tlsmsg)
	{
		return EINVAL;
	}
	tlsmsg->data = malloc(sizeof(u8) * data_length);
	U8COPY(tlsmsg->data,0,data_length,tls_data);
	tlsmsg->datalength = data_length;
	return 0;
}

/*
 * data : returned data
 * eaptls_data : the TLS_Data field
 * length : the length of eaptls_data
 * flags : combination of flags et set
 */
int diameap_tls_new_tls_packet(u8** data, int * len, struct tls_msg tlsmsg)
{
	int buflen, pos = 0;

	buflen = 1;

	if ((tlsmsg.flags & TLS_FLAG_LENGTH) == TLS_FLAG_LENGTH)
	{
		buflen += 4;
	}

	if (tlsmsg.datalength > 0)
	{
		buflen += tlsmsg.datalength;
	}
	*data = malloc(sizeof(u8) * buflen);
	memset(*data, 0, sizeof(u8) * buflen);
	P8(*data,tlsmsg.flags);
	pos++;

	if ((tlsmsg.flags & TLS_FLAG_LENGTH) == TLS_FLAG_LENGTH)
	{
		P32BIGE(*data+pos,tlsmsg.length);
		pos += 4;
	}
	if (tlsmsg.data)
	{
		U8COPY(*data,pos,tlsmsg.datalength,tlsmsg.data);
	}
	*len = buflen;
	return 0;
}

int diameap_set_tls(struct tls_msg * tlsmsg, u8 flags, u32 length,
		u8 *tls_data, int data_length)
{
	diameap_tls_new(tlsmsg);
	diameap_tls_set_flags(tlsmsg, flags);
	diameap_tls_set_message_length(tlsmsg, length);
	diameap_tls_set_data(tlsmsg, tls_data, data_length);
	return 0;
}

int diameap_tls_parse(u8* data, int len, struct tls_msg * tlsmsg)
{

	if (data == NULL)
		return EINVAL;
	int pos = 0;
	diameap_tls_new(tlsmsg);
	tlsmsg->flags = G8(data);
	pos++;
	if ((tlsmsg->flags & TLS_FLAG_LENGTH) == TLS_FLAG_LENGTH)
	{
		tlsmsg->length = G32BIGE(data+1);
		pos = pos + 4;
	}

	if (len > pos)
	{
		tlsmsg->data = malloc(sizeof(u8) * (len - pos));
		U8COPY(tlsmsg->data,0,(len-pos),data+pos);
		tlsmsg->datalength = len - pos;
	}
	else
	{
		tlsmsg->data = NULL;
		tlsmsg->datalength = 0;
	}
	return 0;
}

void diameap_tls_dump(struct tls_msg tlsmsg)
{
	u8 * data = NULL;
	u32 len;
	diameap_tls_get_data(tlsmsg, &data, &len);

	fprintf(stderr, "-------------Dump EAP-TLS msg-------------\n");
	u8 flags;
	diameap_tls_get_flags(tlsmsg, &flags);
	fprintf(stderr, "\t -flags       		: %02x ", flags);
	if (flags & TLS_FLAG_LENGTH)
		fprintf(stderr, " TLS_FLAG_LENGTH ");
	if (flags & TLS_FLAG_MORE)
		fprintf(stderr, " TLS_FLAG_MORE ");
	if (flags & TLS_FLAG_START)
		fprintf(stderr, " TLS_FLAG_START ");
	fprintf(stderr, "\n");
	if ((tlsmsg.flags & TLS_FLAG_LENGTH) == TLS_FLAG_LENGTH)
	{
		u32 length;
		diameap_tls_get_message_length(tlsmsg, &length);
		fprintf(stderr, "\t -TLS msg length	: %u (0x%02x%02x%02x%02x)\n",
				length, (length >> 24) & 0xffU, (length >> 16) & 0xffU, (length
						>> 8) & 0xffU, length & 0xffU);
	}
	fprintf(stderr, "\t -data length		: %d \n", len);
	/*
	if (len > 0)
	{
		int i;
		fprintf(stderr, "\t -Data			: ");
		for (i = 0; i < len; i++)
		{
			fprintf(stderr, "%02x ", G8(data + i));
		}
		fprintf(stderr, "\n");
	}
	*/
	fprintf(stderr, "-------------End Dump EAP-TLS msg-------------\n");
	
	free(data);
}

int diameap_tls_initialize(struct tls_data * data)
{
	if (!data)
	{
		return EINVAL;
	}
	data->state = START;
	data->more_tosend_length = 0;
	data->more_toreceive = FALSE;
	data->handshake = FALSE;
	return 0;
}

int diameap_tls_reassemble(struct tls_msg * to, struct tls_msg from)
{
	u8 from_flag;
	diameap_tls_get_flags(from, &from_flag);

	if (from_flag & TLS_FLAG_LENGTH)
	{
		diameap_tls_new(to);
		u32 length;
		diameap_tls_get_message_length(from, &length);
		diameap_tls_set_message_length(to, length);
	}
	diameap_tls_set_flags(to, from_flag);

	u8 * tlsRespData;
	u32 tlsRespDataLength;
	diameap_tls_get_data(from, &tlsRespData, &tlsRespDataLength);
	to->data = realloc(to->data, to->datalength + tlsRespDataLength);
	U8COPY(to->data,to->datalength,tlsRespDataLength,tlsRespData);
	to->datalength += tlsRespDataLength;
	free(tlsRespData);
	return 0;
}

int diameap_tls_copy(struct tls_msg * to, struct tls_msg from)
{
	u8 flag;
	u32 length;
	u8 * data;
	diameap_tls_new(to);
	diameap_tls_get_flags(from, &flag);
	diameap_tls_set_flags(to, flag);
	diameap_tls_get_message_length(from, &length);
	diameap_tls_set_message_length(to, length);
	length = 0;
	diameap_tls_get_data(from, &data, &length);
	diameap_tls_set_data(to, data, length);
	return 0;
}

int diameap_tls_process_receive(struct tls_data * data)
{
	int ret;

	data->p_length = data->tlsResp.datalength;

	ret = gnutls_handshake(data->session);

	if (ret < 0)
	{
		switch (ret)
		{
		case GNUTLS_E_AGAIN:
			break;
		case GNUTLS_E_INTERRUPTED:
			fprintf(stderr, "[DiamEAP extension] [EAP TLS] gnutls handshake : GNUTLS_E_INTERRUPTED");
			break;
		case GNUTLS_E_GOT_APPLICATION_DATA:
			fprintf(stderr,
					"[DiamEAP extension] [EAP TLS] gnutls handshake : GNUTLS_E_GOT_APPLICATION_DATA");
			break;
		case GNUTLS_E_WARNING_ALERT_RECEIVED:
			fprintf(stderr,
					"[DiamEAP extension] [EAP TLS] gnutls handshake : GNUTLS_E_WARNING_ALERT_RECEIVED");
			break;
		}
		if (ret != GNUTLS_E_AGAIN)
		{
			gnutls_perror(ret);
		}
	}
	if (ret == GNUTLS_E_SUCCESS)
	{
		data->handshake = TRUE;
	}
	return 0;
}

