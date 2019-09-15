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


#include "libdiameap.h"

int diameap_eap_get_code(struct eap_packet *eappacket, eap_code * eapcode)
{
	TRACE_ENTRY("%p %p",eappacket,eapcode);

	if (eappacket->data == NULL)
	{
		*eapcode = ERROR;
		TRACE_DEBUG(INFO,"%sEmpty data in EAP packet: no EAP Code to return.",DIAMEAP_EXTENSION);
		return EINVAL;
	}
	if (eappacket->length < 1)
	{
		*eapcode = ERROR;
		TRACE_DEBUG(INFO,"%sEAP packet length %d : no EAP Code to return.",DIAMEAP_EXTENSION, eappacket->length);
		return EINVAL;
	}
	*eapcode = G8(eappacket->data);
	return 0;
}

int diameap_eap_get_identifier(struct eap_packet *eappacket, u8 * identifier)
{
	TRACE_ENTRY("%p %p",eappacket,identifier);
	*identifier = 0;
	if (eappacket->data == NULL)
	{
		TRACE_DEBUG(INFO,"%sEmpty data in EAP packet: no Identifier field to return.",DIAMEAP_EXTENSION);
		return EINVAL;
	}
	if (eappacket->length < 2)
	{
		TRACE_DEBUG(INFO,"%sEAP packet length %d : no Identifier field to return.",DIAMEAP_EXTENSION, eappacket->length);
		return EINVAL;
	}
	*identifier = G8(eappacket->data + 1);
	return 0;
}

int diameap_eap_get_length(struct eap_packet *eappacket, u16 *length)
{
	TRACE_ENTRY("%p %p",eappacket,length);
	*length = 0;

	if (eappacket->data == NULL)
	{
		TRACE_DEBUG(INFO,"%sEmpty data in EAP packet: no Length field to return.",DIAMEAP_EXTENSION);
		return EINVAL;
	}
	if (eappacket->ulength < 4)
	{
		TRACE_DEBUG(INFO,"%sEAP packet length %d : no Length field to return.",DIAMEAP_EXTENSION, eappacket->length);
		return EINVAL;
	}
	*length = (u16) G16BIGE((eappacket->data + 2));
	return 0;
}

int diameap_eap_get_packetlength(struct eap_packet *eappacket, u16 *length)
{
	TRACE_ENTRY("%p %p",eappacket,length);
	if (eappacket->data == NULL)
	{
		TRACE_DEBUG(INFO,"%sEmpty data in EAP packet: no Length field to return.",DIAMEAP_EXTENSION);
		return EINVAL;
	}
	*length = eappacket->ulength;
	return 0;
}

int diameap_eap_get_type(struct eap_packet *eappacket, eap_type * eaptype)
{
	TRACE_ENTRY("%p %p",eappacket,eaptype);
	*eaptype = 0;

	if (eappacket->data == NULL)
	{
		TRACE_DEBUG(INFO,"%sEmpty data in EAP packet: no EAP Type field to return.",DIAMEAP_EXTENSION);
		return EINVAL;
	}
	if (eappacket->length < 5)
	{
		TRACE_DEBUG(INFO,"%sEAP packet length %d : no EAP Type field to return.",DIAMEAP_EXTENSION, eappacket->length);
		return EINVAL;
	}
	*eaptype = (u32) G8(eappacket->data + 4);
	return 0;
}

boolean diameap_eap_check_code(eap_code code)
{
	TRACE_ENTRY("%d",code);
	if (code != EAP_REQUEST && code != EAP_RESPONSE && code != EAP_SUCCESS
			&& code != EAP_FAILURE)
	{
		TRACE_DEBUG(FULL,"%sIncorrect EAP Code (%d).",DIAMEAP_EXTENSION,code);
		return FALSE;
	}
	return TRUE;
}

int diameap_eap_get_packetdata(struct eap_packet *eappacket, u8 ** data,
		int *len)
{
	TRACE_ENTRY("%p %p",eappacket,data);
	if (eappacket->length > 0)
	{
		*data = malloc(sizeof(u8) * eappacket->length);
		U8COPY(*data,0,eappacket->length,eappacket->data);
		*len = eappacket->length;
		return 0;
	}
	TRACE_DEBUG(INFO,"%sEAP packet length=%d: empty or wrong EAP Packet.",DIAMEAP_EXTENSION, eappacket->length);
	*data = NULL;
	*len = 0;
	return EINVAL;
}

int diameap_eap_get_data(struct eap_packet *eappacket, u8 ** data, int * len)
{
	TRACE_ENTRY("%p %p",eappacket,data);
	if (eappacket->length > 5)
	{
		CHECK_MALLOC( *data = malloc(sizeof(u8) * (eappacket->length - 5)) );
		U8COPY(*data,0,(eappacket->length-5),(eappacket->data+5));
		*len = eappacket->length - 5;
		return 0;
	}
	TRACE_DEBUG(INFO,"%sEAP packet length=%d: empty or wrong EAP Packet.",DIAMEAP_EXTENSION, eappacket->length);
	*data = NULL;
	*len = 0;

	return EINVAL;
}

void diameap_eap_dump(int level, struct eap_packet *eappacket)
{
	TRACE_ENTRY("%d %p",level,eappacket);
	if (!TRACE_BOOL(level))
		return;

	if (eappacket->data == NULL)
		return;
	if (eappacket->length < 5)
		return;
	if (eappacket->ulength < 5)
		return;

	fd_log_debug("------------- Dump EAP Packet-------------");
	int i;
	fd_log_debug("\t -Code       : ");
	eap_code code;
	CHECK_FCT_DO(diameap_eap_get_code(eappacket,&code),return);
	switch (code)
	{
	case 1:
		fd_log_debug("Request");
		break;
	case 2:
		fd_log_debug("Response");
		break;
	case 3:
		fd_log_debug("Success");
		break;
	case 4:
		fd_log_debug("Failure");
		break;
	default:
		fd_log_debug("Error (EAP Code value [%d] not allowed)",code);
		break;
	}
	u8 id;
	CHECK_FCT_DO(diameap_eap_get_identifier(eappacket,&id),return);
	fd_log_debug("\t -Identifier : %x ", id);
	u16 length;
	CHECK_FCT_DO(diameap_eap_get_length(eappacket,&length),return);
	fd_log_debug("\t -Length     : %d ", (unsigned int)length);
	if (eappacket->length > 4)
	{
		eap_type type;
		CHECK_FCT_DO(diameap_eap_get_type(eappacket,&type),return);
		fd_log_debug("\t -Type       : %d ", type);
	}
	if (eappacket->length > 5)
	{
		char buf[1024];
		snprintf(buf, sizeof(buf), "\t -Data       : ");
		for (i = 5; i < eappacket->length && i < 30; i++)
		{
			snprintf(buf+strlen(buf), sizeof(buf)-strlen(buf), "%02x ", G8(eappacket->data + i));
		}
		if(i+1<eappacket->length)
			snprintf(buf+strlen(buf), sizeof(buf)-strlen(buf), "[...] (len=%d)",(unsigned int) length);
		fd_log_debug("%s", buf);
	}
	fd_log_debug("-------------End Dump EAP Packet-------------");
}

int diameap_eap_new(eap_code code, u8 id, eap_type type, u8 * data,
		u16 dataLength, struct eap_packet *eappacket)
{

	TRACE_ENTRY("%d %hhu %d %p %hu %p", code, id, type, data, dataLength,eappacket);

	int length = 0;

	//Initialize EAP Packet
	eappacket->length = 0;
	eappacket->ulength = 0;
	eappacket->data = NULL;

	if (diameap_eap_check_code(code) == FALSE)
	{
		TRACE_DEBUG(
				INFO,
				"%sWrong EAP Code. Only Request, Response, Success or Failure are permitted.",DIAMEAP_EXTENSION);
		return EINVAL;
	}

	length += EAP_HEADER;
	if (code == EAP_REQUEST || code == EAP_RESPONSE)
	{ // EAP Packet code is either Request or Response
		if (type == TYPE_NONE)
		{
			TRACE_DEBUG(INFO,
					"%sUnable to create a new EAP Packet (Request or Response) without EAP Type field.",DIAMEAP_EXTENSION);
			return EINVAL;
		}
		length += EAP_TYPE; // EAP Packet code
		if (data != NULL)
		{
			length += dataLength;

			if (length > (int) MAX_EAP_PACKET)
			{

				TRACE_DEBUG(INFO,
						"%sUnable to add Data to EAP Packet. Data field length exceed allowed data size.", DIAMEAP_EXTENSION);
				return EINVAL;
			}
			else
			{

				CHECK_MALLOC(eappacket->data=malloc(sizeof(u8)*length));
				memset(eappacket->data, 0, sizeof(u8) * length);
				U8COPY(eappacket->data, 5, dataLength, data);
			}
		}
		else
		{
			CHECK_MALLOC(eappacket->data=malloc(sizeof(u8)*length));
			memset(eappacket->data, 0, sizeof(u8) * length);
		}
		P8(eappacket->data + 4, type);
	}
	else
	{ //EAP Packet code is SUCCESS or FAILURE
		CHECK_MALLOC(eappacket->data=malloc(sizeof(u8)*length));
		memset(eappacket->data, 0, sizeof(u8) * length);
		//No Data filed
		//No Type field
	}
	//Add Code and Identifier fields
	P8(eappacket->data, code);
	P8(eappacket->data + 1, id);
	//Add length field
	P16BIGE(eappacket->data + 2, length);
	eappacket->length = length;
	eappacket->ulength = length;
	return 0;
}
