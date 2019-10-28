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


#ifndef DIAMEAP_EAPPACKET_H_
#define DIAMEAP_EAPPACKET_H_

#include "diameap_defs.h"

/*EAP Packet definition : the Extensible Authentication Protocol (RFC3748) */

/* Define Length/Size */
#define EAP_HEADER 4 	/* EAP Packet header size : 4 Bytes */
#define EAP_TYPE 1 		/* EAP Type field size : 1 Byte */
#define MAX_EAP_PACKET (u16)0xffff /* Maximum value allowed for an EAP Packet */

/*EAP Packet Codes Type*/
typedef enum
{
	ERROR = 0, 			/* EAP Packet Code should not be set to ERROR. Indicates a wrong Code value received. */
	EAP_REQUEST = 1, 	/* EAP Request (value=1). */
	EAP_RESPONSE = 2, 	/* EAP Response (value=2). */
	EAP_SUCCESS = 3, 	/* EAP Success (value=3). */
	EAP_FAILURE = 4 	/* EAP Failure (value=4). */
} eap_code;

/* EAP Packet */
struct eap_packet
{
	u16 length; 	/* Length of data (equivalent to the length field). */
	u16 ulength; 	/* Length of received or inserted bytes in the data pointer. */
	u8 *data; 		/* Data buffer (including Code, Identifier, Length,  (also Type and Data fields if exists)). */
};

/* EAP Methods Types */
typedef enum
{
	TYPE_NONE = 0, 			/* Indicate no method specified */
	TYPE_IDENTITY = 1, 		/* EAP Identity (RFC3748) */
	TYPE_NOTIFICATION = 2, 	/* EAP Notification (RFC3748) */
	TYPE_NAK = 3, 			/* EAP NAK (RFC3748) */
	TYPE_EAP_MD5 = 4,		/* EAP MD5 (RFC3748) */
	TYPE_OTP = 5, 			/* EAP OTP (RFC3748) */
	TYPE_GTC = 6, 			/* EAP GTC (RFC3748) */
	TYPE_EAP_TLS = 13,		/* EAP TLS (RFC5216) */
	TYPE_EXPANDED_TYPES = 254, 	/* EAP Expanded NAK (RFC3748) */
	TYPE_EXPERIMENTAL_USE = 255	/* for Experimental use (RFC3748) */
} eap_type;


/* Function		:	diameap_eap_get_code
 *
 * Parameters:
 * 	eapPacket 	: 	an EAP Packet
 *	eapcode		:	On successful return, the EAP Code is stored here.
 *
 * Description:
 * 	Get the EAP Code of an EAP Packet. If NULL EAP Packet or empty packet ERROR is stored in eapcode.
 *
 * Return value:
 * 	0 			: 	if successful
* 	EINVAL		:	if EAP Packet is NULL or empty
 */

int diameap_eap_get_code(struct eap_packet *eapPacket, eap_code * eapcode);


/* Function		:	diameap_eap_get_identifier
 *
 * Parameters:
 * 	eapPacket 	: 	an EAP Packet
 * 	identifier	:	on successful return, the identifier value is stored here.
 *
 * Description:
 * 	Get the Identifier field of an EAP Packet.
 *
 * Return value:
 * 	0 			: 	if successful
 * 	EINVAL		: 	if the EAP Packet is NULL or the EAP identifier value is not valid or data length < 2
 *
 */

int diameap_eap_get_identifier(struct eap_packet *eapPacket, u8 * identifier);


/* Function		:	diameap_eap_get_length
 *
 * Parameters:
 *	eapPacket	: 	an EAP Packet
 *	length		:	on successful return, the EAP Packet length field is stored here.
 *
 * Description:
 *	Get the Length field of an EAP Packet
 *
 * Return value:
 *	0			:	if successful
 *	EINVAL		: 	if the data length < 4, or the EAP Packet is NULL.
 */

int diameap_eap_get_length(struct eap_packet *eapPacket, u16 *length);


/* Function		:	diameap_eap_get_packetlength
 *
 * Parameters:
 *	eapPacket	: 	an EAP Packet
 *	length		:	on successful return, the length of EAP Packet is stored here.
 *
 * Description:
 *	Get the received or inserted length of Bytes in an EAP Packet.
 *
 * Return value:
 *	0			: 	if successful
 *	EINVAL		: 	if the EAP Packet is NULL
 */

int diameap_eap_get_packetlength(struct eap_packet *eapPacket, u16 *length);


/* Function		:	diameap_eap_get_type
 *
 * Parameters:
 *	eapPacket	: 	an EAP Packet
 *	eaptype		:	on successful return, the EAP Type field is stored here.
 *
 * Description:
 *	Get the Type field of an EAP Packet.
 *
 * Return value:
 *	0			:	if successful
 *	EINVAL		:	if the EAP Packet is NULL or data length < 5
 */

int diameap_eap_get_type(struct eap_packet *eapPacket,eap_type * eaptype);


/* Function		:	diameap_eap_get_packetdata
 *
 * Parameters:
 *	eapPacket	: 	an EAP Packet
 *	eapdata		:	on successful return, the content of EAP Packet is stored here.
 *	eapdatalen	:	on successful return, the length of EAP Packet is stored here.
 *
 * Description:
 *	Get the data content of an EAP Packet.
 *
 * Return value:
 *	0			: 	if successful
 *	EINVAL		: 	if there is no data in the current EAP Packet (ulength<1)
 */

int diameap_eap_get_packetdata(struct eap_packet *eapPacket, u8 ** data, int *len);


/* Function		:	diameap_eap_get_data
 *
 * Parameters:
 *	eapPacket	: 	an EAP Packet
 *	data		:	on successful return, the content of data field is stored here.
 *	datalen		:	on successful return, the length of data field is stored here.
 *
 * Description:
 *	Return the Data field of an EAP Packet
 *
 * Return value:
 *	0			: 	if successful
 *	EINVAL		: 	if there is no data in the current EAP Packet (ulength<6)
 */

int diameap_eap_get_data(struct eap_packet *eapPacket, u8 ** data, int * len);


/* Function		:	diameap_eap_chech_code
 *
 * Parameters:
 *	code		: 	an EAP Code value
 *
 * Description:
 *	Check if the code value is valid or not
 *
 * Return value:
 * 	TRUE		: 	if valid value
 * 	FALSE		: 	if not valid value
 *
 */

boolean diameap_eap_check_code(eap_code code);


/* Function		:	diameap_eap_dump
 *
 * Parameters:
 *	eapPacket	:	an EAP Packet
 *	level		:	Debug level
 *
 * Description:
 *	Dump the structure and the content of all the fields in this EAP Packet
 *
 */

void diameap_eap_dump(int level, struct eap_packet *eapPacket);


/* Function		:	diameap_eap_new
 *
 * Parameters:
 *	code		:	EAP CODE field value
 *	id			:	EAP Identifier field value
 *	type		:	(optional) EAP type field value if exist, otherwise NULL
 *	data		:	(optional) EAP data field value if exist, otherwise NULL
 *	dataLength	:	(optional) Length of the data to be inserted in the Packet, otherwise 0
 *	eappacket	:	on successful return, pointer to eap packet is stored here.
 * Description:
 *	Create a new EAP Packet with the specified parameters and data
 *
 * Return value:
 *	0			: 	if successful
 *	EINVAL		:	Invalid parameter
 */

int diameap_eap_new(eap_code code, u8 id, eap_type type,
		u8 * data, u16 dataLength, struct eap_packet *eappacket);

#endif /* DIAMEAP_EAPPACKET_H_ */
