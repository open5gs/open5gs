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

/* handler for DiamEAP server callback */
static struct disp_hdl * handle;

/* session handler for DiamEAP sessions state machine */
static struct session_handler * diameap_server_reg = NULL;


struct avp_max_occurences auth_avps[] =
{
{ "Service-Type", 1 },
{ "Callback-Number", 1 },
{ "Callback-Id", 1 },
{ "Idle-Timeout", 1 },
{ "Port-Limit", 1 },
{ "NAS-Filter-Rule" - 1 },
{ "Filter-Id", -1 },
{ "Configuration-Token", -1 },
{ "QoS-Filter-Rule", -1 },
{ "Framed-Protocol", 1 },
{ "Framed-Routing", 1 },
{ "Framed-MTU", 1 },
{ "Framed-Compression", -1 },
{ "Framed-IP-Address", 1 },
{ "Framed-IP-Netmask", 1 },
{ "Framed-Route", -1 },
{ "Framed-Pool", 1 },
{ "Framed-Interface-Id", 1 },
{ "Framed-IPv6-Prefix", -1 },
{ "Framed-IPv6-Pool", 1 },
{ "Framed-IPv6-Route", -1 },
{ "Framed-IPX-Network", 1 },
{ "Framed-Appletalk-Link", 1 },
{ "Framed-Appletalk-Network", -1 },
{ "Framed-Appletalk-Zone", 1 },
{ "NAS-IPv6-Address", 0 },
{ "NAS-Identifier", 0 },
{ "NAS-IP-Address", 0 },
{ "NAS-Port", 0 },
{ "NAS-Port-Id", 0 },
{ "NAS-Port-Type", 0 },
{ "Called-Station-Id", 0 },
{ "Calling-Station-Id", 0 },
{ "Connect-Info", 0 },
{ "Originating-Line-Info", 0 } };



void diameap_cli_sess_cleanup(struct sess_state * diameap_sess_data, os0_t sid, void * opaque)
{

	CHECK_PARAMS_DO( diameap_sess_data, return );

	if (diameap_sess_data != NULL)
	{
		if (diameap_sess_data->methodData != NULL)
		{
			struct plugin * cplugin;
			if (diameap_plugin_get(diameap_sess_data->currentVendor,
					diameap_sess_data->currentMethod, &cplugin))
			{
				TRACE_DEBUG(INFO,"%sUnable to access EAP Method plugin {Type=%d, Vendor=%d}.",DIAMEAP_EXTENSION,diameap_sess_data->currentMethod,diameap_sess_data->currentVendor);
			}

			if (cplugin->eap_method_free)
			{
				(*cplugin->eap_method_free)(diameap_sess_data->methodData);
				diameap_sess_data->methodData = NULL;
			}
			else
			{
				TRACE_DEBUG(FULL+1,"%s[%s plugin] datafree function not available.",DIAMEAP_EXTENSION,cplugin->methodname);
				if (diameap_sess_data->methodData != NULL)
				{
					free(diameap_sess_data->methodData);
					diameap_sess_data->methodData = NULL;
				}
			}
			if (diameap_sess_data->methodData)
			{
				TRACE_DEBUG(INFO,"%sSession state was not been freed correctly!!!",DIAMEAP_EXTENSION);
			}
		}

		if (diameap_sess_data->user.password != NULL)
		{
			free(diameap_sess_data->user.password);
			diameap_sess_data->user.password = NULL;
		}

		if (diameap_sess_data->user.userid != NULL)
		{
			free(diameap_sess_data->user.userid);
			diameap_sess_data->user.userid = NULL;
		}
		free(diameap_sess_data);
		diameap_sess_data = NULL;
	}
}

static int diameap_initialize_diameap_sm(
		struct diameap_state_machine * diameap_sm,
		struct sess_state * diameap_sess_data)
{
	TRACE_ENTRY("%p %p", diameap_sm, diameap_sess_data);

	int i;

	/* Initialize Long Term Variables */
	if (diameap_sess_data != NULL)
	{
		diameap_sm->invalid_eappackets = diameap_sess_data->invalid_eappackets;

		/* Initialize eap state machine variables */
		/*User*/
		diameap_sm->eap_sm.user.id = diameap_sess_data->user.id;

		if ((diameap_sess_data->user.userid != NULL)
				&& (diameap_sess_data->user.useridLength > 0))
		{
			diameap_sm->eap_sm.user.useridLength
					= diameap_sess_data->user.useridLength;
			CHECK_MALLOC(diameap_sm->eap_sm.user.userid= malloc(diameap_sm->eap_sm.user.useridLength+1));
			U8COPY(diameap_sm->eap_sm.user.userid,0,diameap_sm->eap_sm.user.useridLength+1,diameap_sess_data->user.userid);
			free(diameap_sess_data->user.userid);
			diameap_sess_data->user.userid = NULL;

		}
		else
		{
			TRACE_DEBUG(INFO,"%s user not identified yet.",DIAMEAP_EXTENSION);
			diameap_sm->eap_sm.user.useridLength = 0;
			diameap_sm->eap_sm.user.userid = NULL;
		}

		if ((diameap_sess_data->user.password != NULL)
				&& (diameap_sess_data->user.passwordLength > 0))
		{
			diameap_sm->eap_sm.user.passwordLength
					= diameap_sess_data->user.passwordLength;
			CHECK_MALLOC(diameap_sm->eap_sm.user.password = malloc(diameap_sm->eap_sm.user.passwordLength+1));
			U8COPY(diameap_sm->eap_sm.user.password,0,diameap_sm->eap_sm.user.passwordLength+1, diameap_sess_data->user.password);
			free(diameap_sess_data->user.password);
			diameap_sess_data->user.password = NULL;
		}
		else
		{
			diameap_sm->eap_sm.user.passwordLength = 0;
			diameap_sm->eap_sm.user.password = NULL;
		}

		diameap_sm->eap_sm.user.methodId = diameap_sess_data->user.methodId;
		for (i = 0; i < MAXMETHODS; i++)
		{
			diameap_sm->eap_sm.user.methods[i].method
					= diameap_sess_data->user.methods[i].method;
			diameap_sm->eap_sm.user.methods[i].vendor
					= diameap_sess_data->user.methods[i].vendor;
		}
		for (i = 0; i < MAXPROPOSEDMETHODS; i++)
		{
			diameap_sm->eap_sm.user.proposedmethods[i].method
					= diameap_sess_data->user.proposedmethods[i].method;
			diameap_sm->eap_sm.user.proposedmethods[i].vendor
					= diameap_sess_data->user.proposedmethods[i].vendor;
		}

		diameap_sm->eap_sm.user.pmethods = diameap_sess_data->user.pmethods;
		diameap_sm->eap_sm.user.proposed_eap_method
				= diameap_sess_data->user.proposed_eap_method;
		diameap_sm->eap_sm.user.proposed_eap_method_vendor
				= diameap_sess_data->user.proposed_eap_method_vendor;
		diameap_sm->eap_sm.user.success = diameap_sess_data->user.success;

		diameap_sm->eap_sm.currentId = diameap_sess_data->currentId;
		diameap_sm->eap_sm.currentVendor = diameap_sess_data->currentVendor;
		diameap_sm->eap_sm.lastId = diameap_sess_data->lastId;
		diameap_sm->eap_sm.methodState = diameap_sess_data->methodState;
		diameap_sm->eap_sm.currentMethod = diameap_sess_data->currentMethod;

		diameap_sm->eap_sm.methodData = diameap_sess_data->methodData;
		diameap_sess_data->methodData = NULL;

		if (diameap_sm->eap_sm.currentMethod != TYPE_NONE)
		{
			diameap_plugin_get(diameap_sm->eap_sm.currentVendor,
					diameap_sm->eap_sm.currentMethod,
					&diameap_sm->eap_sm.selectedMethod);
		}

		/* free session data*/
		free(diameap_sess_data);
		diameap_sess_data = NULL;

	}
	else
	{
		diameap_sm->invalid_eappackets = 0;

		/* Initialize eap state machine variables */
		/*User*/
		diameap_sm->eap_sm.user.id = 0;
		diameap_sm->eap_sm.user.userid = NULL;
		diameap_sm->eap_sm.user.useridLength = 0;
		diameap_sm->eap_sm.user.password = NULL;
		diameap_sm->eap_sm.user.passwordLength = 0;
		diameap_sm->eap_sm.user.methodId = -1;
		for (i = 0; i < MAXMETHODS; i++)
		{
			diameap_sm->eap_sm.user.methods[i].method = TYPE_NONE;
			diameap_sm->eap_sm.user.methods[i].vendor = VENDOR_IETF;
		}
		for (i = 0; i < MAXPROPOSEDMETHODS; i++)
		{
			diameap_sm->eap_sm.user.proposedmethods[i].method = TYPE_NONE;
			diameap_sm->eap_sm.user.proposedmethods[i].vendor = VENDOR_IETF;
		}

		diameap_sm->eap_sm.user.pmethods = -1;
		diameap_sm->eap_sm.user.proposed_eap_method = TYPE_NONE;
		diameap_sm->eap_sm.user.proposed_eap_method_vendor = VENDOR_IETF;
		diameap_sm->eap_sm.user.success = FALSE;

		diameap_sm->eap_sm.currentId = -1;
		diameap_sm->eap_sm.currentVendor = VENDOR_IETF;
		diameap_sm->eap_sm.lastId = -1;
		diameap_sm->eap_sm.methodState = EAP_M_PROPOSED;

		diameap_sm->eap_sm.currentMethod = TYPE_NONE;

		diameap_sm->eap_sm.methodData = NULL;

	}

	diameap_sm->result_code = 0;
	fd_list_init(&diameap_sm->attributes, NULL);
	fd_list_init(&diameap_sm->req_attributes, NULL);
	fd_list_init(&diameap_sm->ans_attributes, NULL);
	diameap_sm->failedavp = NULL;
	diameap_sm->auth_request_val = AUTHENTICATE_ONLY;
	diameap_sm->verify_authorization = FALSE;
	diameap_sm->authSuccess = FALSE;
	diameap_sm->authFailure = FALSE;
	diameap_sm->lastReqEAPavp = NULL;
	diameap_sm->privateUser = FALSE;
	diameap_sm->authorized = FALSE;

	diameap_sm->eap_sm.rxResp = FALSE;
	diameap_sm->eap_sm.respId = -1;
	diameap_sm->eap_sm.respMethod = TYPE_NONE;
	diameap_sm->eap_sm.respVendor = VENDOR_IETF;
	diameap_sm->eap_sm.respVendorMethod = TYPE_NONE;

	return 0;
}

static int diameap_initialize_diameap_eap_interface(
		struct diameap_eap_interface * eap_i)
{
	TRACE_ENTRY("%p", eap_i);

	//Initialize AAA-EAP Interface
	eap_i->aaaEapResp = FALSE;
	eap_i->aaaEapRespData.data = NULL;
	//Initialize EAP-AAA Interface
	eap_i->aaaEapReq = FALSE;
	eap_i->aaaEapNoReq = FALSE;
	eap_i->aaaSuccess = FALSE;
	eap_i->aaaFail = FALSE;
	eap_i->aaaEapReqData.data = NULL;
	eap_i->aaaEapMSKData = NULL;
	eap_i->aaaEapEMSKData = NULL;
	eap_i->aaaEapKeyAvailable = FALSE;
	eap_i->aaaMethodTimeout = 0;

	return 0;
}

static int diameap_failed_avp(struct diameap_state_machine * diameap_sm,
		struct avp * invalidavp)
{
	TRACE_ENTRY("%p %p",diameap_sm,invalidavp);
	if (!invalidavp)
		return EINVAL;

	if (!diameap_sm)
		return EINVAL;

	if (diameap_sm->failedavp == NULL)
	{
		CHECK_FCT( fd_msg_avp_new( dataobj_failed_avp, 0, &diameap_sm->failedavp) );

		CHECK_FCT( fd_msg_avp_add( diameap_sm->failedavp, MSG_BRW_LAST_CHILD, invalidavp ) );

	}
	else
	{
		//add multiple AVPs in Failed-AVP
	}
	return 0;
}

static int diameap_parse_eap_resp(struct eap_state_machine * eap_sm,
		struct eap_packet *eappacket)
{
	TRACE_ENTRY("%p %p",eap_sm, eappacket)

	eap_sm->rxResp = FALSE;
	eap_sm->respId = -1;
	eap_sm->respMethod = TYPE_NONE;
	eap_sm->respVendor = VENDOR_IETF;
	eap_sm->respVendorMethod = TYPE_NONE;

	if (eappacket->data == NULL)
	{
		TRACE_DEBUG(INFO,"%s Empty EAP packet",DIAMEAP_EXTENSION);
		return 0;
	}

	u16 plength;
	CHECK_FCT(diameap_eap_get_packetlength(eappacket,&plength));
	if ((int) plength < EAP_HEADER)
	{
		TRACE_DEBUG(INFO,"%s EAP packet length less than EAP header.",DIAMEAP_EXTENSION);
		return 0;
	}

	u16 length;
	CHECK_FCT(diameap_eap_get_length(eappacket,&length));
	if ((int) length < EAP_HEADER)
	{
		TRACE_DEBUG(INFO,"%sEAP packet length field less than EAP header.",DIAMEAP_EXTENSION);
		return 0;
	}

	if (plength < length)
	{
		TRACE_DEBUG(INFO,"%sLength of received EAP packet is less than the value of the length field.",DIAMEAP_EXTENSION);
		return 0;
	}

	eap_code code;
	CHECK_FCT(diameap_eap_get_code(eappacket,&code));
	if (code == EAP_REQUEST || code == EAP_SUCCESS || code == EAP_FAILURE)
	{
		TRACE_DEBUG(INFO,"%sOnly EAP Responses are accepted at EAP server side.",DIAMEAP_EXTENSION);
		return 0;
	}

	u8 id;
	CHECK_FCT(diameap_eap_get_identifier(eappacket,&id));
	eap_sm->respId = id;

	CHECK_FCT(diameap_eap_get_type(eappacket,&eap_sm->respMethod));
	if ((eap_sm->methodState != EAP_M_PROPOSED) && (eap_sm->respMethod
			== TYPE_NAK || eap_sm->respMethod == TYPE_EXPANDED_TYPES))
	{
		TRACE_DEBUG(INFO,"%sNAK or EXPANDED_NAK received after an EAP TYPE been selected",DIAMEAP_EXTENSION);
		return 0;
	}

	if ((eap_sm->respMethod == TYPE_EXPANDED_TYPES) && (length < 20))
	{
		TRACE_DEBUG(INFO,"%s Truncated EAP Packet received.",DIAMEAP_EXTENSION);
		return 0;
	}

	if ((eap_sm->respMethod == TYPE_NAK) && (eap_sm->currentMethod < 4))
	{
		TRACE_DEBUG(INFO,"%sNAK response not expected at this step (Only EAP type = 4 and above are accepted).",DIAMEAP_EXTENSION);
		return 0;
	}

	if (eap_sm->respMethod == TYPE_EXPANDED_TYPES)
	{
		u8 *data = (u8 *) eappacket->data;
		//int len = 0;
		//u32 respVendor, respVendorMethod;
		data += 5;
		eap_sm->respVendor = G24BIGE(data);
		data += 3;
		eap_sm->respVendorMethod = G32BIGE(data);
		data += 4;
		/*		while ((length - 12) > (len * 8))
		 {
		 if (((eap_type) G8(data)) != TYPE_EXPANDED_TYPES)
		 {
		 return FALSE;
		 }
		 data += 1;
		 respVendor = G24BIGE(data);
		 data += 3;
		 respVendorMethod = G32BIGE(data);
		 eap_sm->user.proposedmethods[len].method = respVendor;
		 eap_sm->user.proposedmethods[len].vendor = respVendorMethod;
		 len++;
		 data += 4;
		 }
		 eap_sm->user.methodId = 0;*/
	}

	if((eap_sm->respMethod == TYPE_IDENTITY) && (length < 6)){
		TRACE_DEBUG(INFO,"%sUser Identity missing",DIAMEAP_EXTENSION);
		return 0;
	}

	eap_sm->rxResp = TRUE;
	return 0;
}

static int diameap_eappacket_new(struct eap_packet * eappacket,
		struct avp_hdr * avpdata)
{
	TRACE_ENTRY("%p %p",eappacket,avpdata);
	eappacket->ulength = (u16) avpdata->avp_value->os.len;
	eappacket->data = (u8 *) avpdata->avp_value->os.data;
	diameap_eap_get_packetlength(eappacket, &eappacket->length);
	return 0;
}

static int diameap_parse_avps(struct diameap_state_machine * diameap_sm,
		struct msg * req, struct diameap_eap_interface * eap_i)
{
	TRACE_ENTRY("%p %p %p",diameap_sm,req,eap_i);
	struct avp * avp, *avp2;
	struct avp_hdr * avpdata;
	int ret;
	int depth;

	/* EAP-Payload data*/
	avp = NULL;
	CHECK_FCT(fd_msg_search_avp(req, dataobj_eap_payload, &avp));
	if (avp != NULL)
	{
		CHECK_FCT(fd_msg_avp_hdr(avp, &avpdata));
		CHECK_FCT(diameap_eappacket_new(&eap_i->aaaEapRespData, avpdata));
		eap_i->aaaEapResp = TRUE;
		u16 length;
		diameap_eap_get_length(&eap_i->aaaEapRespData, &length);
		if (length >= 4)
		{
			eap_code code;
			CHECK_FCT(diameap_eap_get_code(&eap_i->aaaEapRespData,&code));

			if (code != EAP_RESPONSE)
			{
				diameap_sm->result_code = 5004; /* DIAMETER_INVALID_AVP_VALUE 5004 */
				struct avp * invalidavp;
				union avp_value val;
				CHECK_FCT( fd_msg_avp_new ( dataobj_eap_payload, 0, &invalidavp));
				val.os.data = eap_i->aaaEapRespData.data;
				val.os.len = eap_i->aaaEapRespData.length;
				CHECK_FCT( fd_msg_avp_setvalue( invalidavp, &val ))
				CHECK_FCT( diameap_failed_avp(diameap_sm, invalidavp));
				TRACE_DEBUG(INFO,"%sIncorrect EAP Packet. EAP Code != Response.",DIAMEAP_EXTENSION);
				return 0;
			}
			else
			{
				CHECK_FCT(diameap_parse_eap_resp(&diameap_sm->eap_sm, &eap_i->aaaEapRespData));
				if (diameap_sm->eap_sm.rxResp == FALSE)
				{
					diameap_sm->result_code = 1001; /*DIAMETER_MULTI_ROUND_AUTH*/
					eap_i->aaaEapNoReq = TRUE;
					eap_i->aaaEapResp = FALSE;
				}
			}
		}
		else
		{
			if (diameap_sm->eap_sm.currentMethod != TYPE_NONE)
			{
				diameap_sm->result_code = 5004; /* DIAMETER_INVALID_AVP_VALUE 5004 */
				CHECK_FCT(diameap_failed_avp(diameap_sm, avp));
				TRACE_DEBUG(INFO,"%sEAP packet length < Minimum EAP packet length.",DIAMEAP_EXTENSION);
				return 1;
			}
			//EAP start received

		}
	}

	/* User-Name AVP */
	avp = NULL;
	CHECK_FCT(fd_msg_search_avp(req, dataobj_user_name, &avp));
	if (avp != NULL)
	{
		CHECK_FCT(fd_msg_avp_hdr(avp, &avpdata));
		struct avp_attribute * attribute;
		CHECK_MALLOC(attribute = malloc(sizeof(struct avp_attribute)));
		memset(attribute, 0, sizeof(struct avp_attribute));
		fd_list_init(&attribute->chain, attribute);
		attribute->attrib = "User-Name";
		attribute->value.os.data = avpdata->avp_value->os.data;
		attribute->value.os.len = avpdata->avp_value->os.len;
		fd_list_insert_before(&diameap_sm->req_attributes, &attribute->chain);
	}

	/* EAP-Key-Name AVP */

	avp = NULL;
	CHECK_FCT(fd_msg_search_avp(req, dataobj_eap_key_name, &avp));
	if (avp != NULL)
	{
		CHECK_FCT(fd_msg_avp_hdr(avp, &avpdata));
		struct avp_attribute * attribute;
		CHECK_MALLOC(attribute = malloc(sizeof(struct avp_attribute)));
		memset(attribute, 0, sizeof(struct avp_attribute));
		fd_list_init(&attribute->chain, attribute);
		attribute->attrib = "EAP-Key-Name";
		attribute->value.os.data = avpdata->avp_value->os.data;
		attribute->value.os.len = avpdata->avp_value->os.len;
		fd_list_insert_before(&diameap_sm->req_attributes, &attribute->chain);
	}

	/* Auth-Request-Type AVP */
	avp = NULL;
	CHECK_FCT(fd_msg_search_avp(req, dataobj_auth_request_type, &avp));
	if (avp != NULL)
	{
		CHECK_FCT(fd_msg_avp_hdr(avp, &avpdata));
		diameap_sm->auth_request_val = avpdata->avp_value->i32;
	}

	/* Authorization-Lifetime AVP */
	avp = NULL;
	CHECK_FCT(fd_msg_search_avp(req, dataobj_authorization_lifetime, &avp));
	if (avp != NULL)
	{
		CHECK_FCT(fd_msg_avp_hdr(avp, &avpdata));
		struct avp_attribute * attribute;
		CHECK_MALLOC(attribute = malloc(sizeof(struct avp_attribute)));
		memset(attribute, 0, sizeof(struct avp_attribute));
		fd_list_init(&attribute->chain, attribute);
		attribute->attrib = "Authorization-Lifetime";
		attribute->value.u32 = avpdata->avp_value->u32;
		fd_list_insert_before(&diameap_sm->req_attributes, &attribute->chain);
	}

	/* Auth-Grace-Period AVP */
	avp = NULL;
	CHECK_FCT(fd_msg_search_avp(req, dataobj_auth_grace_period, &avp));
	if (avp != NULL)
	{
		CHECK_FCT(fd_msg_avp_hdr(avp, &avpdata));
		struct avp_attribute * attribute;
		CHECK_MALLOC(attribute = malloc(sizeof(struct avp_attribute)));
		memset(attribute, 0, sizeof(struct avp_attribute));
		fd_list_init(&attribute->chain, attribute);
		attribute->attrib = "Auth-Grace-Period";
		attribute->value.u32 = avpdata->avp_value->u32;
		fd_list_insert_before(&diameap_sm->req_attributes, &attribute->chain);
	}

	/* Auth-Session-State AVP */
	avp = NULL;
	CHECK_FCT(fd_msg_search_avp(req, dataobj_auth_session_state, &avp));
	if (avp != NULL)
	{
		CHECK_FCT(fd_msg_avp_hdr(avp, &avpdata));
		struct avp_attribute * attribute;
		CHECK_MALLOC(attribute = malloc(sizeof(struct avp_attribute)));
		memset(attribute, 0, sizeof(struct avp_attribute));
		fd_list_init(&attribute->chain, attribute);
		attribute->attrib = "Auth-Session-State";
		attribute->value.i32 = avpdata->avp_value->i32;
		fd_list_insert_before(&diameap_sm->req_attributes, &attribute->chain);
	}

	/* Origin-state-Id AVP */
	avp = NULL;
	CHECK_FCT(fd_msg_search_avp(req, dataobj_origin_state_id, &avp));
	if (avp != NULL)
	{
		CHECK_FCT(fd_msg_avp_hdr(avp, &avpdata));
		struct avp_attribute * attribute;
		CHECK_MALLOC(attribute = malloc(sizeof(struct avp_attribute)));
		memset(attribute, 0, sizeof(struct avp_attribute));
		fd_list_init(&attribute->chain, attribute);
		attribute->attrib = "Origin-state-Id AVP";
		attribute->value.u32 = avpdata->avp_value->u32;
		fd_list_insert_before(&diameap_sm->req_attributes, &attribute->chain);
	}

	/* NAS-Port AVP*/
	avp = NULL;
	CHECK_FCT(fd_msg_search_avp(req, dataobj_nas_port, &avp));
	if (avp != NULL)
	{
		CHECK_FCT(fd_msg_avp_hdr(avp, &avpdata));
		struct avp_attribute * attribute;
		CHECK_MALLOC(attribute = malloc(sizeof(struct avp_attribute)));
		memset(attribute, 0, sizeof(struct avp_attribute));
		fd_list_init(&attribute->chain, attribute);
		attribute->attrib = "NAS-Port";
		attribute->value.u32 = avpdata->avp_value->u32;
		fd_list_insert_before(&diameap_sm->req_attributes, &attribute->chain);
	}

	/* NAS-Port-Id AVP */
	avp = NULL;
	CHECK_FCT(fd_msg_search_avp(req, dataobj_nas_port_id, &avp));
	if (avp != NULL)
	{
		CHECK_FCT(fd_msg_avp_hdr(avp, &avpdata));
		struct avp_attribute * attribute;
		CHECK_MALLOC(attribute = malloc(sizeof(struct avp_attribute)));
		memset(attribute, 0, sizeof(struct avp_attribute));
		fd_list_init(&attribute->chain, attribute);
		attribute->attrib = "NAS-Port-Id";
		attribute->value.os.data = avpdata->avp_value->os.data;
		attribute->value.os.len = avpdata->avp_value->os.len;
		fd_list_insert_before(&diameap_sm->req_attributes, &attribute->chain);
	}

	/* NAS-Port-Type AVP */
	avp = NULL;
	CHECK_FCT(fd_msg_search_avp(req, dataobj_nas_port_type, &avp));
	if (avp != NULL)
	{
		CHECK_FCT(fd_msg_avp_hdr(avp, &avpdata));
		struct avp_attribute * attribute;
		CHECK_MALLOC(attribute = malloc(sizeof(struct avp_attribute)));
		memset(attribute, 0, sizeof(struct avp_attribute));
		fd_list_init(&attribute->chain, attribute);
		attribute->attrib = "NAS-Port-Type";
		attribute->value.u32 = avpdata->avp_value->u32;
		fd_list_insert_before(&diameap_sm->req_attributes, &attribute->chain);
	}

	/* Called-Station-Id AVP */
	avp = NULL;
	CHECK_FCT(fd_msg_search_avp(req, dataobj_called_station_id, &avp));
	if (avp != NULL)
	{
		CHECK_FCT(fd_msg_avp_hdr(avp, &avpdata));
		struct avp_attribute * attribute;
		CHECK_MALLOC(attribute = malloc(sizeof(struct avp_attribute)));
		memset(attribute, 0, sizeof(struct avp_attribute));
		fd_list_init(&attribute->chain, attribute);
		attribute->attrib = "Called-Station-Id";
		attribute->value.os.data = avpdata->avp_value->os.data;
		attribute->value.os.len = avpdata->avp_value->os.len;
		fd_list_insert_before(&diameap_sm->req_attributes, &attribute->chain);
	}

	/* Calling-Station-Id AVP */
	avp = NULL;
	CHECK_FCT(fd_msg_search_avp(req, dataobj_calling_station_id, &avp));
	if (avp != NULL)
	{
		CHECK_FCT(fd_msg_avp_hdr(avp, &avpdata));
		struct avp_attribute * attribute;
		CHECK_MALLOC(attribute = malloc(sizeof(struct avp_attribute)));
		memset(attribute, 0, sizeof(struct avp_attribute));
		fd_list_init(&attribute->chain, attribute);
		attribute->attrib = "Calling-Station-Id";
		attribute->value.os.data = avpdata->avp_value->os.data;
		attribute->value.os.len = avpdata->avp_value->os.len;
		fd_list_insert_before(&diameap_sm->req_attributes, &attribute->chain);
	}

	/* Connect-Info AVP */
	avp = NULL;
	CHECK_FCT(fd_msg_search_avp(req, dataobj_connect_info, &avp));
	if (avp != NULL)
	{
		CHECK_FCT(fd_msg_avp_hdr(avp, &avpdata));
		struct avp_attribute * attribute;
		CHECK_MALLOC(attribute = malloc(sizeof(struct avp_attribute)));
		memset(attribute, 0, sizeof(struct avp_attribute));
		fd_list_init(&attribute->chain, attribute);
		attribute->attrib = "Connect-Info";
		attribute->value.os.data = avpdata->avp_value->os.data;
		attribute->value.os.len = avpdata->avp_value->os.len;
		fd_list_insert_before(&diameap_sm->req_attributes, &attribute->chain);
	}

	/* Originating-Line-Info AVP */
	avp = NULL;
	CHECK_FCT(fd_msg_search_avp(req, dataobj_originating_line_info, &avp));
	if (avp != NULL)
	{
		CHECK_FCT(fd_msg_avp_hdr(avp, &avpdata));
		struct avp_attribute * attribute;
		CHECK_MALLOC(attribute = malloc(sizeof(struct avp_attribute)));
		memset(attribute, 0, sizeof(struct avp_attribute));
		fd_list_init(&attribute->chain, attribute);
		attribute->attrib = "Originating-Line-Info";
		attribute->value.os.data = avpdata->avp_value->os.data;
		attribute->value.os.len = avpdata->avp_value->os.len;
		fd_list_insert_before(&diameap_sm->req_attributes, &attribute->chain);
	}

	/* Service-Type AVP */
	avp = NULL;
	CHECK_FCT(fd_msg_search_avp(req, dataobj_service_type, &avp));
	if (avp != NULL)
	{
		CHECK_FCT(fd_msg_avp_hdr(avp, &avpdata));
		struct avp_attribute * attribute;
		CHECK_MALLOC(attribute = malloc(sizeof(struct avp_attribute)));
		memset(attribute, 0, sizeof(struct avp_attribute));
		fd_list_init(&attribute->chain, attribute);
		attribute->attrib = "Service-Type";
		attribute->value.u32 = avpdata->avp_value->u32;
		fd_list_insert_before(&diameap_sm->req_attributes, &attribute->chain);
	}

	/* Callback-Number AVP */
	avp = NULL;
	CHECK_FCT(fd_msg_search_avp(req, dataobj_callback_number, &avp));
	if (avp != NULL)
	{
		CHECK_FCT(fd_msg_avp_hdr(avp, &avpdata));
		struct avp_attribute * attribute;
		CHECK_MALLOC(attribute = malloc(sizeof(struct avp_attribute)));
		memset(attribute, 0, sizeof(struct avp_attribute));
		fd_list_init(&attribute->chain, attribute);
		attribute->attrib = "Callback-Number";
		attribute->value.os.data = avpdata->avp_value->os.data;
		attribute->value.os.len = avpdata->avp_value->os.len;
		fd_list_insert_before(&diameap_sm->req_attributes, &attribute->chain);
	}

	/* Port-Limit AVP */
	avp = NULL;
	CHECK_FCT(fd_msg_search_avp(req, dataobj_port_limit, &avp));
	if (avp != NULL)
	{
		CHECK_FCT(fd_msg_avp_hdr(avp, &avpdata));
		struct avp_attribute * attribute;
		CHECK_MALLOC(attribute = malloc(sizeof(struct avp_attribute)));
		memset(attribute, 0, sizeof(struct avp_attribute));
		fd_list_init(&attribute->chain, attribute);
		attribute->attrib = "Port-Limit";
		attribute->value.u32 = avpdata->avp_value->u32;
		fd_list_insert_before(&diameap_sm->req_attributes, &attribute->chain);
	}

	/* Framed-Protocol AVP */
	avp = NULL;
	CHECK_FCT(fd_msg_search_avp(req, dataobj_framed_protocol, &avp));
	if (avp != NULL)
	{
		CHECK_FCT(fd_msg_avp_hdr(avp, &avpdata));
		struct avp_attribute * attribute;
		CHECK_MALLOC(attribute = malloc(sizeof(struct avp_attribute)));
		memset(attribute, 0, sizeof(struct avp_attribute));
		fd_list_init(&attribute->chain, attribute);
		attribute->attrib = "Framed-Protocol";
		attribute->value.u32 = avpdata->avp_value->u32;
		fd_list_insert_before(&diameap_sm->req_attributes, &attribute->chain);
	}

	/* Framed-MTU AVP */
	avp = NULL;
	CHECK_FCT(fd_msg_search_avp(req, dataobj_framed_mtu, &avp));
	if (avp != NULL)
	{
		CHECK_FCT(fd_msg_avp_hdr(avp, &avpdata));
		struct avp_attribute * attribute;
		CHECK_MALLOC(attribute = malloc(sizeof(struct avp_attribute)));
		memset(attribute, 0, sizeof(struct avp_attribute));
		fd_list_init(&attribute->chain, attribute);
		attribute->attrib = "Framed-MTU";
		attribute->value.u32 = avpdata->avp_value->u32;
		fd_list_insert_before(&diameap_sm->req_attributes, &attribute->chain);
	}

	/* Framed-Compression AVP */
	avp = NULL;
	avp2 = NULL;
	CHECK_FCT(fd_msg_search_avp(req, dataobj_framed_compression, &avp));
	if (avp != NULL)
	{
		CHECK_FCT(fd_msg_avp_hdr(avp, &avpdata));
		u32 Pi_Code = avpdata->avp_code;
		int depth;
		do
		{
			struct avp_attribute * attribute;
			CHECK_MALLOC(attribute = malloc(sizeof(struct avp_attribute)));
			memset(attribute, 0, sizeof(struct avp_attribute));
			fd_list_init(&attribute->chain, attribute);
			attribute->attrib = "Framed-Compression";
			attribute->value.u32 = avpdata->avp_value->u32;
			fd_list_insert_before(&diameap_sm->req_attributes,
					&attribute->chain);
			ret = 0;
			depth = 0;
			ret = fd_msg_browse ( avp, MSG_BRW_NEXT, &avp2, &depth);
			if (avp2 != NULL)
			{
				CHECK_FCT(fd_msg_avp_hdr(avp2, &avpdata));
			}
			avp = avp2;
		} while ((avp2 != NULL) && (ret == 0) && (ret == 0)
				&& (avpdata->avp_code == Pi_Code));
	}

	/* Framed-IP-Address AVP */
	avp = NULL;
	CHECK_FCT(fd_msg_search_avp(req, dataobj_framed_ip_address, &avp));
	if (avp != NULL)
	{
		CHECK_FCT(fd_msg_avp_hdr(avp, &avpdata));
		struct avp_attribute * attribute;
		CHECK_MALLOC(attribute = malloc(sizeof(struct avp_attribute)));
		memset(attribute, 0, sizeof(struct avp_attribute));
		fd_list_init(&attribute->chain, attribute);
		attribute->attrib = "Framed-IP-Address";
		attribute->value.os.data = avpdata->avp_value->os.data;
		attribute->value.os.len = avpdata->avp_value->os.len;
		fd_list_insert_before(&diameap_sm->req_attributes, &attribute->chain);
	}

	/* Framed-IP-Netmask AVP */
	avp = NULL;
	CHECK_FCT(fd_msg_search_avp(req, dataobj_framed_ip_netmask, &avp));
	if (avp != NULL)
	{
		CHECK_FCT(fd_msg_avp_hdr(avp, &avpdata));
		struct avp_attribute * attribute;
		CHECK_MALLOC(attribute = malloc(sizeof(struct avp_attribute)));
		memset(attribute, 0, sizeof(struct avp_attribute));
		fd_list_init(&attribute->chain, attribute);
		attribute->attrib = "Framed-IP-Netmask";
		attribute->value.os.data = avpdata->avp_value->os.data;
		attribute->value.os.len = avpdata->avp_value->os.len;
		fd_list_insert_before(&diameap_sm->req_attributes, &attribute->chain);
	}

	/* Framed-Interface-Id AVP */
	avp = NULL;
	CHECK_FCT(fd_msg_search_avp(req, dataobj_framed_interface_id, &avp));
	if (avp != NULL)
	{
		CHECK_FCT(fd_msg_avp_hdr(avp, &avpdata));
		struct avp_attribute * attribute;
		CHECK_MALLOC(attribute = malloc(sizeof(struct avp_attribute)));
		memset(attribute, 0, sizeof(struct avp_attribute));
		fd_list_init(&attribute->chain, attribute);
		attribute->attrib = "Framed-Interface-Id";
		attribute->value.u64 = avpdata->avp_value->u64;
		fd_list_insert_before(&diameap_sm->req_attributes, &attribute->chain);
	}

	/* Framed-IPv6-Prefix AVP */
	avp = NULL;
	avp2 = NULL;
	CHECK_FCT(fd_msg_search_avp(req, dataobj_framed_ipv6_prefix, &avp));
	if (avp != NULL)
	{
		CHECK_FCT(fd_msg_avp_hdr(avp, &avpdata));
		u32 Pi_Code = avpdata->avp_code;
		do
		{
			struct avp_attribute * attribute;
			CHECK_MALLOC(attribute = malloc(sizeof(struct avp_attribute)));
			memset(attribute, 0, sizeof(struct avp_attribute));
			fd_list_init(&attribute->chain, attribute);
			attribute->attrib = "Framed-IPv6-Prefix";
			attribute->value.u32 = avpdata->avp_value->u32;
			fd_list_insert_before(&diameap_sm->req_attributes,
					&attribute->chain);
			ret = 0;
			depth = 0;
			ret = fd_msg_browse ( avp, MSG_BRW_NEXT, &avp2, &depth);
			if (avp2 != NULL)
			{
				CHECK_FCT(fd_msg_avp_hdr(avp2, &avpdata));
			}
			avp = avp2;
		} while ((avp2 != NULL) && (ret == 0) && (ret == 0)
				&& (avpdata->avp_code == Pi_Code));
	}

	/* Tunneling AVP */
	avp = NULL;
	avp2 = NULL;
	CHECK_FCT(fd_msg_search_avp(req, dataobj_tunneling, &avp));
	if (avp != NULL)
	{
		CHECK_FCT(fd_msg_avp_hdr(avp, &avpdata));
		u32 Pi_Code = avpdata->avp_code;
		int depth;
		do
		{
			struct avp_attribute * attribute;
			CHECK_MALLOC(attribute = malloc(sizeof(struct avp_attribute)));
			memset(attribute, 0, sizeof(struct avp_attribute));
			fd_list_init(&attribute->chain, attribute);
			attribute->attrib = "Tunneling";
			//grouped AVP
			fd_list_insert_before(&diameap_sm->req_attributes,
					&attribute->chain);
			ret = 0;
			depth = 0;
			ret = fd_msg_browse ( avp, MSG_BRW_NEXT, &avp2, &depth);
			if (avp2 != NULL)
			{
				CHECK_FCT(fd_msg_avp_hdr(avp2, &avpdata));
			}
			avp = avp2;
		} while ((avp2 != NULL) && (ret == 0) && (ret == 0)
				&& (avpdata->avp_code == Pi_Code));
	}

	/* NAS-Identifier AVP */
	avp = NULL;
	CHECK_FCT(fd_msg_search_avp(req, dataobj_nas_identifier, &avp));
	if (avp != NULL)
	{
		CHECK_FCT(fd_msg_avp_hdr(avp, &avpdata));
		struct avp_attribute * attribute;
		CHECK_MALLOC(attribute = malloc(sizeof(struct avp_attribute)));
		memset(attribute, 0, sizeof(struct avp_attribute));
		fd_list_init(&attribute->chain, attribute);
		attribute->attrib = "NAS-Identifier";
		attribute->value.os.data = avpdata->avp_value->os.data;
		attribute->value.os.len = avpdata->avp_value->os.len;
		fd_list_insert_before(&diameap_sm->req_attributes, &attribute->chain);
	}

	/* NAS-IP-Address AVP */
	avp = NULL;
	CHECK_FCT(fd_msg_search_avp(req, dataobj_nas_ip_address, &avp));
	if (avp != NULL)
	{
		CHECK_FCT(fd_msg_avp_hdr(avp, &avpdata));
		struct avp_attribute * attribute;
		CHECK_MALLOC(attribute = malloc(sizeof(struct avp_attribute)));
		memset(attribute, 0, sizeof(struct avp_attribute));
		fd_list_init(&attribute->chain, attribute);
		attribute->attrib = "NAS-IP-Address";
		attribute->value.os.data = avpdata->avp_value->os.data;
		attribute->value.os.len = avpdata->avp_value->os.len;
		fd_list_insert_before(&diameap_sm->req_attributes, &attribute->chain);
	}

	/* NAS-IPv6-Address AVP */
	avp = NULL;
	CHECK_FCT(fd_msg_search_avp(req, dataobj_nas_ipv6_address, &avp));
	if (avp != NULL)
	{
		CHECK_FCT(fd_msg_avp_hdr(avp, &avpdata));
		struct avp_attribute * attribute;
		CHECK_MALLOC(attribute = malloc(sizeof(struct avp_attribute)));
		memset(attribute, 0, sizeof(struct avp_attribute));
		fd_list_init(&attribute->chain, attribute);
		attribute->attrib = "NAS-IPv6-Address";
		attribute->value.os.data = avpdata->avp_value->os.data;
		attribute->value.os.len = avpdata->avp_value->os.len;
		fd_list_insert_before(&diameap_sm->req_attributes, &attribute->chain);
	}

	/* State AVP */
	avp = NULL;
	CHECK_FCT(fd_msg_search_avp(req, dataobj_state, &avp));
	if (avp != NULL)
	{
		CHECK_FCT(fd_msg_avp_hdr(avp, &avpdata));
		struct avp_attribute * attribute;
		CHECK_MALLOC(attribute = malloc(sizeof(struct avp_attribute)));
		memset(attribute, 0, sizeof(struct avp_attribute));
		fd_list_init(&attribute->chain, attribute);
		attribute->attrib = "State";
		attribute->value.os.data = avpdata->avp_value->os.data;
		attribute->value.os.len = avpdata->avp_value->os.len;
		fd_list_insert_before(&diameap_sm->req_attributes, &attribute->chain);
	}

	return 0;
}


static int diameap_sess_data_new(
		struct sess_state *diameap_sess_data,
		struct diameap_state_machine *diameap_sm)
{
	if (!diameap_sm)
		return EINVAL;

	int i;

	diameap_sess_data->invalid_eappackets = diameap_sm->invalid_eappackets;

	diameap_sess_data->user.id = diameap_sm->eap_sm.user.id;

	if ((diameap_sm->eap_sm.user.userid != NULL)
			&& (diameap_sm->eap_sm.user.useridLength > 0))
	{
		diameap_sess_data->user.useridLength
				= diameap_sm->eap_sm.user.useridLength;
		CHECK_MALLOC(diameap_sess_data->user.userid= malloc(diameap_sess_data->user.useridLength+1));
		U8COPY(diameap_sess_data->user.userid,0,diameap_sess_data->user.useridLength+1,diameap_sm->eap_sm.user.userid);
		free(diameap_sm->eap_sm.user.userid);
		diameap_sm->eap_sm.user.userid = NULL;

	}
	else
	{
		diameap_sess_data->user.useridLength = 0;
		diameap_sess_data->user.userid = NULL;
	}

	if ((diameap_sm->eap_sm.user.password != NULL)
			&& (diameap_sm->eap_sm.user.passwordLength > 0))
	{
		diameap_sess_data->user.passwordLength
				= diameap_sm->eap_sm.user.passwordLength;
		CHECK_MALLOC(diameap_sess_data->user.password = malloc(diameap_sess_data->user.passwordLength+1));
		U8COPY(diameap_sess_data->user.password,0,diameap_sess_data->user.passwordLength+1,diameap_sm->eap_sm.user.password);
		free(diameap_sm->eap_sm.user.password);
		diameap_sm->eap_sm.user.password = NULL;
	}
	else
	{
		diameap_sess_data->user.passwordLength = 0;
		diameap_sess_data->user.password = NULL;
	}

	diameap_sess_data->user.methodId = diameap_sm->eap_sm.user.methodId;
	for (i = 0; i < MAXMETHODS; i++)
	{
		diameap_sess_data->user.methods[i].method
				= diameap_sm->eap_sm.user.methods[i].method;
		diameap_sess_data->user.methods[i].vendor
				= diameap_sm->eap_sm.user.methods[i].vendor;
	}

	for (i = 0; i < MAXPROPOSEDMETHODS; i++)
	{
		diameap_sess_data->user.proposedmethods[i].method
				= diameap_sm->eap_sm.user.proposedmethods[i].method;
		diameap_sess_data->user.proposedmethods[i].vendor
				= diameap_sm->eap_sm.user.proposedmethods[i].vendor;
	}

	diameap_sess_data->user.pmethods = diameap_sm->eap_sm.user.pmethods;
	diameap_sess_data->user.proposed_eap_method
			= diameap_sm->eap_sm.user.proposed_eap_method;
	diameap_sess_data->user.proposed_eap_method_vendor
			= diameap_sm->eap_sm.user.proposed_eap_method_vendor;
	diameap_sess_data->user.success = diameap_sm->eap_sm.user.success;

	diameap_sess_data->currentId = diameap_sm->eap_sm.currentId;
	diameap_sess_data->currentMethod = diameap_sm->eap_sm.currentMethod;
	diameap_sess_data->currentVendor = diameap_sm->eap_sm.currentVendor;
	diameap_sess_data->lastId = diameap_sm->eap_sm.lastId;
	diameap_sess_data->methodState = diameap_sm->eap_sm.methodState;

	diameap_sess_data->methodData = diameap_sm->eap_sm.methodData;
	diameap_sm->eap_sm.methodData = NULL;

	return 0;
}

static void free_attrib(struct auth_attribute * auth_attrib)
{
	if (auth_attrib == NULL)
	{
		return;
	}
	if (auth_attrib->attrib != NULL)
	{
		free(auth_attrib->attrib);
		auth_attrib->attrib = NULL;
	}
	if (auth_attrib->op != NULL)
	{
		free(auth_attrib->op);
		auth_attrib->op = NULL;
	}
	if (auth_attrib->value != NULL)
	{
		free(auth_attrib->value);
		auth_attrib->value = NULL;
	}
	free(auth_attrib);
	auth_attrib = NULL;
}

static void free_avp_attrib(struct avp_attribute * avp_attrib)
{
	if(avp_attrib){
	free(avp_attrib);
	avp_attrib = NULL;
	}
}

static void free_ans_attrib(struct avp_attribute * ans_attrib)
{
	if(ans_attrib){
		if (ans_attrib->tofree == 1)
		{
			if(ans_attrib->value.os.data){
			free(ans_attrib->value.os.data);
			ans_attrib->value.os.data = NULL;
			}
		}
		free(ans_attrib);
		ans_attrib = NULL;
	}
}

static int diameap_unlink_attributes_lists(
		struct diameap_state_machine * diameap_sm)
{
	TRACE_ENTRY("%p ", diameap_sm);
	if (diameap_sm == NULL)
	{
		return EINVAL;
	}

	while (!FD_IS_LIST_EMPTY(&diameap_sm->attributes))
	{
		struct fd_list * item = (struct fd_list *) diameap_sm->attributes.next;
		struct auth_attribute * auth = (struct auth_attribute *) item;
		fd_list_unlink(item);
		free_attrib(auth);
	}

	while (!FD_IS_LIST_EMPTY(&diameap_sm->req_attributes))
	{
		struct fd_list * item =
				(struct fd_list *) diameap_sm->req_attributes.next;
		struct avp_attribute * avp = (struct avp_attribute *) item;
		fd_list_unlink(item);
		free_avp_attrib(avp);
	}

	while (!FD_IS_LIST_EMPTY(&diameap_sm->ans_attributes))
	{
		struct fd_list * item =
				(struct fd_list *) diameap_sm->ans_attributes.next;
		struct avp_attribute * avp_ans = (struct avp_attribute *) item;
		fd_list_unlink(item);
		free_ans_attrib(avp_ans);
	}

	return 0;
}

static void diameap_free(struct diameap_state_machine * diameap_sm)
{

	if (diameap_sm != NULL)
	{
		if (diameap_sm->eap_sm.user.userid != NULL)
		{
			free(diameap_sm->eap_sm.user.userid);
			diameap_sm->eap_sm.user.userid = NULL;
		}

		if (diameap_sm->eap_sm.user.password != NULL)
		{
			free(diameap_sm->eap_sm.user.password);
			diameap_sm->eap_sm.user.password = NULL;
		}

		diameap_sm->eap_sm.selectedMethod = NULL;

		if (diameap_sm->eap_sm.methodData != NULL)
		{

			struct plugin * cplugin;
			if (diameap_plugin_get(diameap_sm->eap_sm.currentVendor,
					diameap_sm->eap_sm.currentMethod, &cplugin))
			{
				TRACE_DEBUG(INFO,"%sUnable to access EAP Method plugin {Type=%d, Vendor=%d}.",DIAMEAP_EXTENSION,diameap_sm->eap_sm.currentMethod,diameap_sm->eap_sm.currentVendor);
			}

			if (cplugin->eap_method_free)
			{
				(*cplugin->eap_method_free)(diameap_sm->eap_sm.methodData);
				diameap_sm->eap_sm.methodData = NULL;
			}
			else
			{
				TRACE_DEBUG(INFO,"%s[%s plugin] datafree function not available.",DIAMEAP_EXTENSION,cplugin->methodname);
				if (diameap_sm->eap_sm.methodData != NULL)
				{
					free(diameap_sm->eap_sm.methodData);
					diameap_sm->eap_sm.methodData = NULL;
				}
			}
			if (diameap_sm->eap_sm.methodData)
			{
				TRACE_DEBUG(INFO,"%sSession state was not been freed correctly!!!",DIAMEAP_EXTENSION);
			}
		}

		if (diameap_sm->failedavp != NULL)
		{
			CHECK_FCT_DO(fd_msg_free(diameap_sm->failedavp), );
		}

		if (diameap_sm->lastReqEAPavp != NULL)
		{
			CHECK_FCT_DO(fd_msg_free(diameap_sm->lastReqEAPavp), );
		}

		CHECK_FCT_DO(diameap_unlink_attributes_lists(diameap_sm), );

		free(diameap_sm);
		diameap_sm = NULL;
	}

}

static int diameap_get_avp_attribute(struct fd_list * avp_attributes,
		char * attribute, struct avp_attribute ** avp_attrib, int unlink,
		int *ret)
{
	TRACE_ENTRY("%p %p %p %d %p", avp_attributes, attribute, avp_attrib, unlink, ret);
	if (avp_attributes == NULL)
	{
		return EINVAL;
	}
	if (attribute == NULL)
	{
		return EINVAL;
	}
	struct fd_list * attrib;
	for (attrib = avp_attributes->next; attrib != avp_attributes; attrib
			= attrib->next)
	{
		*avp_attrib = (struct avp_attribute *) attrib;
		if (strcmp((*avp_attrib)->attrib, attribute) == 0)
		{
			*ret = 0;
			if (unlink == 1)
			{
				fd_list_unlink(&(*avp_attrib)->chain);
			}
			return 0;
		}
	}
	*avp_attrib = NULL;
	*ret = 1;
	return 0;
}

static int diameap_get_auth_attribute(struct fd_list * auth_attributes,
		char * attribute, struct auth_attribute ** auth_attrib, int unlink,
		int *ret)
{

	TRACE_ENTRY("%p %p %p %d %p", auth_attributes, attribute, auth_attrib, unlink, ret);

	if (auth_attributes == NULL)
	{
		return EINVAL;
	}
	if (attribute == NULL)
	{
		return EINVAL;
	}

	struct fd_list * attrib;

	for (attrib = auth_attributes->next; attrib != auth_attributes; attrib
			= attrib->next)
	{
		*auth_attrib = (struct auth_attribute *) attrib;
		if (strcmp((*auth_attrib)->attrib, attribute) == 0)
		{
			*ret = 0;
			if (unlink == 1)
			{
				fd_list_unlink(&(*auth_attrib)->chain);
			}
			return 0;
		}
	}
	*auth_attrib = NULL;
	*ret = 1;
	return 0;
}

static int diameap_get_ans_attribute(struct fd_list * ans_attributes,
		char * attribute, struct avp_attribute ** ans_attrib, int unlink,
		int *ret)
{
	TRACE_ENTRY("%p %p %p %d %p", ans_attributes, attribute, ans_attrib, unlink, ret);
	if (ans_attributes == NULL)
	{
		return EINVAL;
	}
	if (attribute == NULL)
	{
		return EINVAL;
	}
	struct fd_list * attrib;
	for (attrib = ans_attributes->next; attrib != ans_attributes; attrib
			= attrib->next)
	{
		*ans_attrib = (struct avp_attribute *) attrib;
		if (strcmp((*ans_attrib)->attrib, attribute) == 0)
		{
			*ret = 0;
			if (unlink == 1)
			{
				fd_list_unlink(&(*ans_attrib)->chain);
			}
			return 0;
		}
	}
	*ans_attrib = NULL;
	*ret = 1;
	return 0;
}

static int diameap_answer_avp_attributes(
		struct diameap_state_machine * diameap_sm)
{
	TRACE_ENTRY("%p",diameap_sm);

	if (diameap_sm == NULL)
	{
		return EINVAL;
	}
	int ret1, ret2;
	struct avp_attribute * avp_attrib;
	struct avp_attribute * ans_attrib;
	struct auth_attribute * auth_attrib;

	/* Authorization-Lifetime */
	{
		CHECK_FCT(diameap_get_avp_attribute(&diameap_sm->req_attributes, "Authorization-Lifetime", &avp_attrib,1, &ret1));

		CHECK_FCT(diameap_get_auth_attribute(&diameap_sm->attributes, "Authorization-Lifetime", &auth_attrib,1, &ret2));

		if ((ret1 == 1) && (ret2 == 0) && (auth_attrib != NULL))
		{

			CHECK_MALLOC(ans_attrib = malloc(sizeof(struct avp_attribute)));

			memset(ans_attrib, 0, sizeof(struct avp_attribute));

			fd_list_init(&ans_attrib->chain, NULL);

			ans_attrib->attrib = "Authorization-Lifetime";

			ans_attrib->value.u32 = atoi(auth_attrib->value);

			fd_list_insert_before(&diameap_sm->ans_attributes,
					&ans_attrib->chain);

			free_attrib(auth_attrib);

		}
		if ((ret1 == 0) && (ret2 == 1) && (avp_attrib != NULL))
		{

			CHECK_MALLOC(ans_attrib = malloc(sizeof(struct avp_attribute)));

			memset(ans_attrib, 0, sizeof(struct avp_attribute));

			fd_list_init(&ans_attrib->chain, NULL);

			ans_attrib->attrib = "Authorization-Lifetime";

			ans_attrib->value.u32 = avp_attrib->value.u32;

			fd_list_insert_before(&diameap_sm->ans_attributes,
					&ans_attrib->chain);

			free_avp_attrib(avp_attrib);

		}
		if ((ret1 == 0) && (ret2 == 0) && (auth_attrib != NULL) && (avp_attrib
				!= NULL))
		{

			CHECK_MALLOC(ans_attrib = malloc(sizeof(struct avp_attribute)));

			memset(ans_attrib, 0, sizeof(struct avp_attribute));
			fd_list_init(&ans_attrib->chain, NULL);
			ans_attrib->attrib = "Authorization-Lifetime";
			if (avp_attrib->value.u32 < atoi(auth_attrib->value))
			{
				ans_attrib->value.u32 = avp_attrib->value.u32;
			}
			else
			{
				ans_attrib->value.u32 = atoi(auth_attrib->value);
			}
			fd_list_insert_before(&diameap_sm->ans_attributes,
					&ans_attrib->chain);
			free_avp_attrib(avp_attrib);
			free_attrib(auth_attrib);
		}
	}

	/* Auth-Grace-Period */
	{
		CHECK_FCT(diameap_get_avp_attribute(&diameap_sm->req_attributes, "Auth-Grace-Period", &avp_attrib,1, &ret1));

		CHECK_FCT(diameap_get_auth_attribute(&diameap_sm->attributes, "Auth-Grace-Period", &auth_attrib,1, &ret2));

		if ((ret1 == 1) && (ret2 == 0) && (auth_attrib != NULL))
		{
			CHECK_MALLOC(ans_attrib = malloc(sizeof(struct avp_attribute)));
			memset(ans_attrib, 0, sizeof(struct avp_attribute));
			fd_list_init(&ans_attrib->chain, NULL);
			ans_attrib->attrib = "Auth-Grace-Period";
			ans_attrib->value.u32 = atoi(auth_attrib->value);
			fd_list_insert_before(&diameap_sm->ans_attributes,
					&ans_attrib->chain);
			free_attrib(auth_attrib);
		}
		if ((ret1 == 0) && (ret2 == 1) && (avp_attrib != NULL))
		{
			CHECK_MALLOC(ans_attrib = malloc(sizeof(struct avp_attribute)));
			memset(ans_attrib, 0, sizeof(struct avp_attribute));
			fd_list_init(&ans_attrib->chain, NULL);
			ans_attrib->attrib = "Auth-Grace-Period";
			ans_attrib->value.u32 = avp_attrib->value.u32;
			fd_list_insert_before(&diameap_sm->ans_attributes,
					&ans_attrib->chain);
			free_avp_attrib(avp_attrib);
		}
		if ((ret1 == 0) && (ret2 == 0) && (auth_attrib != NULL) && (avp_attrib
				!= NULL))
		{
			CHECK_MALLOC(ans_attrib = malloc(sizeof(struct avp_attribute)));
			memset(ans_attrib, 0, sizeof(struct avp_attribute));
			fd_list_init(&ans_attrib->chain, NULL);
			ans_attrib->attrib = "Auth-Grace-Period";
			if (avp_attrib->value.u32 < atoi(auth_attrib->value))
			{
				ans_attrib->value.u32 = avp_attrib->value.u32;
			}
			else
			{
				ans_attrib->value.u32 = atoi(auth_attrib->value);
			}
			fd_list_insert_before(&diameap_sm->ans_attributes,
					&ans_attrib->chain);
			free_attrib(auth_attrib);
			free_avp_attrib(avp_attrib);
		}
	}

	/* Auth-Session-State */
	{
		CHECK_FCT(diameap_get_auth_attribute(&diameap_sm->attributes, "Auth-Session-State", &auth_attrib,1, &ret2));
		if ((ret2 == 0) && (auth_attrib != NULL))
		{
			CHECK_MALLOC(ans_attrib = malloc(sizeof(struct avp_attribute)));
			memset(ans_attrib, 0, sizeof(struct avp_attribute));
			fd_list_init(&ans_attrib->chain, NULL);
			ans_attrib->attrib = "Auth-Session-State";
			ans_attrib->value.i32 = atoi(auth_attrib->value);
			fd_list_insert_before(&diameap_sm->ans_attributes,
					&ans_attrib->chain);
			free_attrib(auth_attrib);
		}
	}

	/* Re-Auth-Request-Type */
	{
		CHECK_FCT(diameap_get_auth_attribute(&diameap_sm->attributes, "Re-Auth-Request-Type", &auth_attrib,1, &ret2));
		if ((ret2 == 0) && (auth_attrib != NULL))
		{
			CHECK_MALLOC(ans_attrib = malloc(sizeof(struct avp_attribute)));
			memset(ans_attrib, 0, sizeof(struct avp_attribute));
			fd_list_init(&ans_attrib->chain, NULL);
			ans_attrib->attrib = "Re-Auth-Request-Type";
			ans_attrib->value.i32 = atoi(auth_attrib->value);
			fd_list_insert_before(&diameap_sm->ans_attributes,
					&ans_attrib->chain);
			free_attrib(auth_attrib);
		}
		else
		{
			ans_attrib = NULL;
			CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes, "Authorization-Lifetime", &ans_attrib,0, &ret1));
			if ((ret1 == 0) && (ans_attrib != NULL))
			{
				CHECK_MALLOC(ans_attrib = malloc(sizeof(struct avp_attribute)));
				memset(ans_attrib, 0, sizeof(struct avp_attribute));
				fd_list_init(&ans_attrib->chain, NULL);
				ans_attrib->attrib = "Re-Auth-Request-Type";
				ans_attrib->value.i32 = 0;
				fd_list_insert_before(&diameap_sm->ans_attributes,
						&ans_attrib->chain);
			}
		}
	}

	/* Session-Timeout */
	{
		CHECK_FCT(diameap_get_auth_attribute(&diameap_sm->attributes, "Session-Timeout", &auth_attrib,1, &ret2));
		if ((ret2 == 0) && (auth_attrib != NULL))
		{
			CHECK_MALLOC(ans_attrib = malloc(sizeof(struct avp_attribute)));
			memset(ans_attrib, 0, sizeof(struct avp_attribute));
			fd_list_init(&ans_attrib->chain, NULL);
			ans_attrib->attrib = "Session-Timeout";
			ans_attrib->value.u32 = atoi(auth_attrib->value);
			fd_list_insert_before(&diameap_sm->ans_attributes,
					&ans_attrib->chain);
			free_attrib(auth_attrib);
		}
	}

	/* Multi-Round-Time-Out */
	{
		CHECK_FCT(diameap_get_auth_attribute(&diameap_sm->attributes, "Multi-Round-Time-Out", &auth_attrib,1, &ret2));
		if ((ret2 == 0) && (auth_attrib != NULL))
		{
			CHECK_MALLOC(ans_attrib = malloc(sizeof(struct avp_attribute)));
			memset(ans_attrib, 0, sizeof(struct avp_attribute));
			fd_list_init(&ans_attrib->chain, NULL);
			ans_attrib->attrib = "Multi-Round-Time-Out";
			ans_attrib->value.u32 = atoi(auth_attrib->value);
			fd_list_insert_before(&diameap_sm->ans_attributes,
					&ans_attrib->chain);
			free_attrib(auth_attrib);
		}
	}

	/* Acct-Interim-Interval */
	{
		CHECK_FCT(diameap_get_auth_attribute(&diameap_sm->attributes, "Acct-Interim-Interval", &auth_attrib,1, &ret2));
		if ((ret2 == 0) && (auth_attrib != NULL))
		{
			CHECK_MALLOC(ans_attrib = malloc(sizeof(struct avp_attribute)));
			memset(ans_attrib, 0, sizeof(struct avp_attribute));
			fd_list_init(&ans_attrib->chain, NULL);
			ans_attrib->attrib = "Acct-Interim-Interval";
			ans_attrib->value.u32 = atoi(auth_attrib->value);
			fd_list_insert_before(&diameap_sm->ans_attributes,
					&ans_attrib->chain);
			free_attrib(auth_attrib);
		}
	}

	return 0;
}

#define	DIAMEAP_STR	1
#define	DIAMEAP_NUM	2

#define DIAMEAP_OP_NO	0	//Not supported operator
#define	DIAMEAP_OP_EQ	1	//==
#define	DIAMEAP_OP_GT	2	//>
#define	DIAMEAP_OP_LT	3	//<
#define	DIAMEAP_OP_GE	4	//>=
#define	DIAMEAP_OP_LE	5	//<=
#define	DIAMEAP_OP_NE	6	//!=
#define	DIAMEAP_OP_EX	7	//~=
#define EQ(A,B) A==B ? TRUE : FALSE
#define GT(A,B) A>B ? TRUE : FALSE
#define GE(A,B) A>=B ? TRUE : FALSE
#define LT(A,B) A<B ? TRUE : FALSE
#define LE(A,B) A<=B ? TRUE : FALSE
#define NE(A,B) A!=B ? TRUE : FALSE

int diameap_get_operator(char *operator)
{
	TRACE_ENTRY("%p",operator);
	if (strcmp(operator, "==") == 0)
	{
		return DIAMEAP_OP_EQ;
	}
	if (strcmp(operator, ">") == 0)
	{
		return DIAMEAP_OP_GT;
	}
	if (strcmp(operator, "<") == 0)
	{
		return DIAMEAP_OP_LT;
	}
	if (strcmp(operator, ">=") == 0)
	{
		return DIAMEAP_OP_GE;
	}
	if (strcmp(operator, "<=") == 0)
	{
		return DIAMEAP_OP_LE;
	}
	if (strcmp(operator, "!=") == 0)
	{
		return DIAMEAP_OP_NE;
	}
	if (strcmp(operator, "~=") == 0)
	{
		return DIAMEAP_OP_EX;
	}
	return DIAMEAP_OP_NO;
}

boolean is_operator(int format_type, char * operator)
{
	TRACE_ENTRY("%d %p",format_type,operator);
	if ((format_type == DIAMEAP_STR) && (strcmp(operator, "==") == 0 || strcmp(
			operator, "~=") == 0 || strcmp(operator, "!=") == 0))
	{
		return TRUE;
	}
	if ((format_type == DIAMEAP_NUM) && (strcmp(operator, "~=") != 0))
	{
		return TRUE;
	}
	return FALSE;
}

union avp_value diameap_get_num(char * num, enum dict_avp_basetype datatype)
{
	TRACE_ENTRY("%p %d",num,datatype);
	union avp_value val;
	switch (datatype)
	{
	case AVP_TYPE_INTEGER32://i32
		val.i32 = atoi(num);
		break;
	case AVP_TYPE_INTEGER64://i64
		val.i64 = atoll(num);
		break;
	case AVP_TYPE_UNSIGNED32://u32
		val.u32 = strtoul(num, NULL, 10);
		break;
	case AVP_TYPE_UNSIGNED64://u64
		val.u64 = strtoull(num, NULL, 10);
		break;
	case AVP_TYPE_FLOAT32://f32
		val.f32 = atof(num);
		break;
	case AVP_TYPE_FLOAT64://f64
		val.f64 = strtod(num, NULL);
		break;
	default:
		TRACE_DEBUG(INFO, "%sUnknown AVP Base Type.",DIAMEAP_EXTENSION)
		;
	}
	return val;
}

boolean diameap_check(union avp_value *A, char * B, char * operator,
		enum dict_avp_basetype datatype)
{
	TRACE_ENTRY("%p %p %p %d",A,B,operator,datatype);
	if (((datatype == AVP_TYPE_OCTETSTRING) && (is_operator(DIAMEAP_STR,
			operator) == TRUE)) || ((datatype != AVP_TYPE_OCTETSTRING)
			&& (datatype != AVP_TYPE_GROUPED) && (is_operator(DIAMEAP_NUM,
			operator) == TRUE)))
	{
		switch (diameap_get_operator(operator))
		{
		case DIAMEAP_OP_EQ:
			if ((datatype == AVP_TYPE_OCTETSTRING) && (is_operator(DIAMEAP_STR,
					operator) == TRUE))
			{
				if (strcmp((char *)A->os.data, B) == 0)
					return TRUE;
				else
					return FALSE;
			}
			else if ((datatype != AVP_TYPE_OCTETSTRING) && (datatype
					!= AVP_TYPE_GROUPED) && (is_operator(DIAMEAP_NUM, operator)
					== TRUE))
			{

				switch (datatype)
				{
				case AVP_TYPE_INTEGER32://i32
					return EQ(A->i32,diameap_get_num(B, datatype).i32);
					break;
				case AVP_TYPE_INTEGER64://i64
					return EQ(A->i64,diameap_get_num(B, datatype).i64);
					break;
				case AVP_TYPE_UNSIGNED32://u32
					return EQ(A->u32,diameap_get_num(B, datatype).u32);
					break;
				case AVP_TYPE_UNSIGNED64://u64
					return EQ(A->u64,diameap_get_num(B, datatype).u64);
					break;
				case AVP_TYPE_FLOAT32://f32
					return EQ(A->f32,diameap_get_num(B, datatype).f32);
					break;
				case AVP_TYPE_FLOAT64://f64
					return EQ(A->f64,diameap_get_num(B, datatype).f64);
					break;
				default:
					return FALSE;
				}
			}
			else
			{
				return FALSE;
			}
			break;
		case DIAMEAP_OP_EX:
		{
			//string only
			boolean authorized = FALSE;
			if ((datatype == AVP_TYPE_OCTETSTRING) && (is_operator(DIAMEAP_STR,
					operator) == TRUE))
			{
				regex_t rule_regexp;
				regcomp(&rule_regexp, B, REG_EXTENDED | REG_NOSUB | REG_ICASE);
				if (regexec(&rule_regexp, (char *)A->os.data, 0, NULL, 0) != 0)
				{
					authorized = FALSE;
				}
				else
				{
					authorized = TRUE;
				}
				regfree(&rule_regexp);
			}
			return authorized;
		}
		case DIAMEAP_OP_GT:

			if ((datatype != AVP_TYPE_OCTETSTRING) && (datatype
					!= AVP_TYPE_GROUPED) && (is_operator(DIAMEAP_NUM, operator)
					== TRUE))
			{
				switch (datatype)
				{
				case AVP_TYPE_INTEGER32://i32
					return GT(A->i32, diameap_get_num(B, datatype).i32);
					break;
				case AVP_TYPE_INTEGER64://i64
					return GT(A->i64, diameap_get_num(B, datatype).i64);
					break;
				case AVP_TYPE_UNSIGNED32://u32
					return GT(A->u32, diameap_get_num(B, datatype).u32);
					break;
				case AVP_TYPE_UNSIGNED64://u64
					return GT(A->u64, diameap_get_num(B, datatype).u64);
					break;
				case AVP_TYPE_FLOAT32://f32
					return GT(A->f32, diameap_get_num(B, datatype).f32);
					break;
				case AVP_TYPE_FLOAT64://f64
					return GT(A->f64, diameap_get_num(B, datatype).f64);
					break;
				default:
					return FALSE;
				}
			}
			else
			{
				return FALSE;
			}
			break;
		case DIAMEAP_OP_GE:
			if ((datatype != AVP_TYPE_OCTETSTRING) && (datatype
					!= AVP_TYPE_GROUPED) && (is_operator(DIAMEAP_NUM, operator)
					== TRUE))
			{
				switch (datatype)
				{
				case AVP_TYPE_INTEGER32://i32
					return GE(A->i32,diameap_get_num(B, datatype).i32);
					break;
				case AVP_TYPE_INTEGER64://i64
					return GE(A->i64,diameap_get_num(B, datatype).i64);
					break;
				case AVP_TYPE_UNSIGNED32://u32
					return GE(A->u32,diameap_get_num(B, datatype).u32);
					break;
				case AVP_TYPE_UNSIGNED64://u64
					return GE(A->u64,diameap_get_num(B, datatype).u64);
					break;
				case AVP_TYPE_FLOAT32://f32
					return GE(A->f32,diameap_get_num(B, datatype).f32);
					break;
				case AVP_TYPE_FLOAT64://f64
					return GE(A->f64,diameap_get_num(B, datatype).f64);
					break;
				default:
					return FALSE;
				}
			}
			else
			{
				return FALSE;
			}
			break;
		case DIAMEAP_OP_LT:
			if ((datatype != AVP_TYPE_OCTETSTRING) && (datatype
					!= AVP_TYPE_GROUPED) && (is_operator(DIAMEAP_NUM, operator)
					== TRUE))
			{
				switch (datatype)
				{
				case AVP_TYPE_INTEGER32://i32
					return LT(A->i32, diameap_get_num(B, datatype).i32);
					break;
				case AVP_TYPE_INTEGER64://i64
					return LT(A->i64, diameap_get_num(B, datatype).i64);
					break;
				case AVP_TYPE_UNSIGNED32://u32
					return LT(A->u32, diameap_get_num(B, datatype).u32);
					break;
				case AVP_TYPE_UNSIGNED64://u64
					return LT(A->u64, diameap_get_num(B, datatype).u64);
					break;
				case AVP_TYPE_FLOAT32://f32
					return LT(A->f32, diameap_get_num(B, datatype).f32);
					break;
				case AVP_TYPE_FLOAT64://f64
					return LT(A->f64, diameap_get_num(B, datatype).f64);
					break;
				default:
					return FALSE;
				}
			}
			else
			{
				return FALSE;
			}
			break;
		case DIAMEAP_OP_LE:
			if ((datatype != AVP_TYPE_OCTETSTRING) && (datatype
					!= AVP_TYPE_GROUPED) && (is_operator(DIAMEAP_NUM, operator)
					== TRUE))
			{
				switch (datatype)
				{
				case AVP_TYPE_INTEGER32://i32
					return LE(A->i32, diameap_get_num(B, datatype).i32);
					break;
				case AVP_TYPE_INTEGER64://i64
					return LE(A->i64, diameap_get_num(B, datatype).i64);
					break;
				case AVP_TYPE_UNSIGNED32://u32
					return LE(A->u32, diameap_get_num(B, datatype).u32);
					break;
				case AVP_TYPE_UNSIGNED64://u64
					return LE(A->u64, diameap_get_num(B, datatype).u64);
					break;
				case AVP_TYPE_FLOAT32://f32
					return LE(A->f32, diameap_get_num(B, datatype).f32);
					break;
				case AVP_TYPE_FLOAT64://f64
					return LE(A->f64, diameap_get_num(B, datatype).f64);
					break;
				default:
					return FALSE;
				}
			}
			else
			{
				return FALSE;
			}
			break;
		case DIAMEAP_OP_NE:
			if ((datatype == AVP_TYPE_OCTETSTRING) && (is_operator(DIAMEAP_STR,
					operator) == TRUE))
			{
				if (strcmp((char *)A->os.data, B) != 0)
					return TRUE;
				else
					return FALSE;
			}
			else if ((datatype != AVP_TYPE_OCTETSTRING) && (datatype
					!= AVP_TYPE_GROUPED) && (is_operator(DIAMEAP_NUM, operator)
					== TRUE))
			{
				switch (datatype)
				{
				case AVP_TYPE_INTEGER32://i32
					return NE(A->i32, diameap_get_num(B, datatype).i32);
					break;
				case AVP_TYPE_INTEGER64://i64
					return NE(A->i64, diameap_get_num(B, datatype).i64);
					break;
				case AVP_TYPE_UNSIGNED32://u32
					return NE(A->u32, diameap_get_num(B, datatype).u32);
					break;
				case AVP_TYPE_UNSIGNED64://u64
					return NE(A->u64, diameap_get_num(B, datatype).u64);
					break;
				case AVP_TYPE_FLOAT32://f32
					return NE(A->f32, diameap_get_num(B, datatype).f32);
					break;
				case AVP_TYPE_FLOAT64://f64
					return NE(A->f64, diameap_get_num(B, datatype).f64);
					break;
				default:
					return FALSE;
				}
			}
			else
			{
				return FALSE;
			}
			break;
		}
	}
	return FALSE;
}

char * diameap_attribute_operator(char * op, int * toadd, boolean *isrule)
{
	TRACE_ENTRY("%p %p %p",op,toadd,isrule);
	char * attribute_op;

	if (op[0] == '+')
	{
		*toadd = 1;
	}
	else if (op[strlen(op) - 1] == '+')
	{
		*toadd = 2;
	}
	else
	{
		*toadd = 0;
	}

	switch (*toadd)
	{
	case 1:
		attribute_op = malloc(strlen(op));
		memset(attribute_op, 0, strlen(op));
		strncpy(attribute_op, op + 1, strlen(op) - 1);
		attribute_op[strlen(op)] = '\0';
		break;
	case 2:
		attribute_op = malloc(strlen(op));
		memset(attribute_op, 0, strlen(op));
		strncpy(attribute_op, op, strlen(op) - 1);
		attribute_op[strlen(op)] = '\0';
		break;
	default:
		attribute_op = malloc(strlen(op) + 1);
		memset(attribute_op, 0, strlen(op) + 1);
		strcpy(attribute_op, op);
		attribute_op[strlen(op) + 1] = '\0';
	}
	if (strcmp(attribute_op, "=") == 0)
	{
		*isrule = FALSE;
		*toadd = 2;
	}
	else
	{
		*isrule = TRUE;
	}

	return attribute_op;
}

int diameap_answer_set_attribute_valueA(union avp_value *A, int *tofree,
		enum dict_avp_basetype datatype, union avp_value * rval)
{
	TRACE_ENTRY("%p %p %d %p",A,tofree,datatype,rval);
	if (datatype == AVP_TYPE_OCTETSTRING)
	{
		CHECK_MALLOC(rval->os.data=malloc(A->os.len));
		memcpy(rval->os.data,A->os.data,A->os.len);
		rval->os.len = A->os.len;
		*tofree = 1;
	}
	else
	{
		*rval = *A;
	}
	return 0;
}
int diameap_answer_set_attribute_valueB(char * B, int *tofree,
		enum dict_avp_basetype datatype, union avp_value * rval)
{
	TRACE_ENTRY("%p %p %d %p",B,tofree,datatype,rval);
	if (datatype == AVP_TYPE_OCTETSTRING)
	{
		CHECK_MALLOC(rval->os.data=malloc(strlen(B)));
		memcpy(rval->os.data,B,strlen(B));
		rval->os.len = strlen(B);

		*tofree = 1;
	}
	else
	{

		*rval = diameap_get_num(B, datatype);
	}

	return 0;
}

static int diameap_attribute_limits(char * attrib, int * max, int *ret)
{
	TRACE_ENTRY("%p %p %p",attrib,max,ret);
	if (attrib == NULL)
	{
		return EINVAL;
	}
	int i;
	for (i = 0; i < sizeof(auth_avps); i++)
	{
		if (strcmp(auth_avps[i].avp_attribute, attrib) == 0)
		{
			*max = auth_avps[i].max;
			*ret = 0;
			return 0;
		}
	}
	*max = 0;
	*ret = 1;
	return 0;
}

static int diameap_answer_authorization_attributes(
		struct diameap_state_machine * diameap_sm)
{
	TRACE_ENTRY("%p",diameap_sm);

	if (diameap_sm == NULL)
	{
		return EINVAL;
	}
	boolean checked = TRUE;

	struct fd_list * attrib;
	struct auth_attribute * auth_attrib;
	struct avp_attribute * avp_attrib;
	int ret;

	for (attrib = (&diameap_sm->attributes)->next; attrib
			!= (&diameap_sm->attributes); attrib = attrib->next)
	{
		avp_attrib = NULL;
		auth_attrib = (struct auth_attribute *) attrib;

		int toadd = 0;
		boolean isrule = FALSE;
		char * op;

		op = diameap_attribute_operator(auth_attrib->op, &toadd, &isrule);

		struct dict_object * dobj;
		struct dict_avp_data avpdata;
		fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME,
				auth_attrib->attrib, &dobj, ENOENT);
		fd_dict_getval(dobj, &avpdata);

		checked = TRUE;
		if (isrule == TRUE)
		{
			CHECK_FCT(diameap_get_avp_attribute(&diameap_sm->req_attributes,auth_attrib->attrib,&avp_attrib,0,&ret));
			if (ret == 0)
			{
				checked = diameap_check(&avp_attrib->value, auth_attrib->value,
						op, avpdata.avp_basetype);
			}
		}
		if (checked == TRUE && toadd != 0)
		{

			struct avp_attribute * ans_attrib;
			int max = 0;
			diameap_attribute_limits(auth_attrib->attrib, &max, &ret);
			if ((ret == 0) && (max != 0))
			{
				if (max == 1)//only one
				{
					int ret = 0;
					diameap_get_ans_attribute(&diameap_sm->ans_attributes,
							auth_attrib->attrib, &ans_attrib, 0, &ret);
					if (ret == 1)
					{
						ans_attrib = NULL;
						CHECK_MALLOC(ans_attrib = malloc(sizeof(struct avp_attribute)));
						memset(ans_attrib, 0, sizeof(struct avp_attribute));
						fd_list_init(&ans_attrib->chain, NULL);
						ans_attrib->attrib = strdup(auth_attrib->attrib);
						if (toadd == 1)
						{
							diameap_answer_set_attribute_valueA(
									&avp_attrib->value, &ans_attrib->tofree,
									avpdata.avp_basetype, &ans_attrib->value);
						}
						else
						{
							diameap_answer_set_attribute_valueB(
									auth_attrib->value, &ans_attrib->tofree,
									avpdata.avp_basetype, &ans_attrib->value);
						}
						fd_list_insert_before(&diameap_sm->ans_attributes,
								&ans_attrib->chain);

					}
					else
					{
						//an answer avp is already added
					}
				}
				else
				{
					ans_attrib = NULL;
					CHECK_MALLOC(ans_attrib = malloc(sizeof(struct avp_attribute)));
					memset(ans_attrib, 0, sizeof(struct avp_attribute));
					fd_list_init(&ans_attrib->chain, NULL);
					ans_attrib->attrib = auth_attrib->attrib;
					if (toadd == 1)
					{
						diameap_answer_set_attribute_valueA(&avp_attrib->value,
								&ans_attrib->tofree, avpdata.avp_basetype,
								&ans_attrib->value);
					}
					else
					{
						diameap_answer_set_attribute_valueB(auth_attrib->value,
								&ans_attrib->tofree, avpdata.avp_basetype,
								&ans_attrib->value);
					}
					fd_list_insert_before(&diameap_sm->ans_attributes,
							&ans_attrib->chain);
				}
			}
		}
		if (checked == FALSE)
		{

			diameap_sm->authorized = FALSE;
			return 0;
		}
	}
	diameap_sm->authorized = checked;
	return 0;

	return 0;
}



static int diameap_policy_decision(struct diameap_state_machine * diameap_sm,
		struct diameap_eap_interface *eap_i)
{
	TRACE_ENTRY("%p %p",diameap_sm,eap_i);

	if ((eap_i->aaaFail == TRUE) && (eap_i->aaaSuccess == TRUE))
	{
		TRACE_DEBUG(INFO,"%s Incorrect EAP decision. EAP process should not return both success and failure for the same session.(please report this problem.)",DIAMEAP_EXTENSION);
		return -1;
	}

	if (eap_i->aaaFail == TRUE)
	{
		diameap_sm->result_code = 4001; /* DIAMETER_AUTHENTICATION_REJECTED 4001 */
		diameap_sm->authFailure = TRUE;
		TRACE_DEBUG(FULL+1,"%s Auth failure: Authentication Rejected ",DIAMEAP_EXTENSION);
		return 0;
	}

	if (eap_i->aaaSuccess == FALSE)
	{
		diameap_sm->result_code = 1001; /* DIAMETER_MULTI_ROUND_AUTH 1001 */
		return 0;
	}

	if (eap_i->aaaSuccess == TRUE)
	{
		if (diameap_sm->auth_request_val == AUTHORIZE_AUTHENTICATE)
		{
			if ((diameap_sm->verify_authorization == TRUE)
					&& (diameap_sm->result_code == 0))
			{
				diameap_sm->result_code = 2001; /* DIAMETER_SUCCESS 2001 */
				diameap_sm->authSuccess = TRUE;
				TRACE_DEBUG(FULL+1,"%s Auth success: Authorization and Authentication ",DIAMEAP_EXTENSION);
				return 0;
			}
			else
			{
				//
			}
		}
		if (diameap_sm->auth_request_val == AUTHENTICATE_ONLY)
		{
			diameap_sm->result_code = 2001; /* DIAMETER_SUCCESS 2001 */
			diameap_sm->authSuccess = TRUE;
			TRACE_DEBUG(FULL+1,"%s Auth success: Authentication Only ",DIAMEAP_EXTENSION);
			return 0;
		}
	}

	return 0;
}

static int diameap_add_avps(struct diameap_state_machine * diameap_sm,
		struct msg * ans, struct msg * req)
{
	TRACE_ENTRY("%p %p %p",diameap_sm,ans,req);

	struct avp * avp, *avp2;
	struct avp_hdr * avpdata;
	union avp_value avp_val;
	int ret = 0;

	/* Origin-Host AVP and Origin-Realm AVP */
	{
		CHECK_FCT( fd_msg_add_origin ( ans, 0 ) );
	}

	/* Auth-Application-Id AVP */
	{
		CHECK_FCT(fd_msg_avp_new(dataobj_auth_application_id, 0, &avp));
		avp_val.u32 = diameap_config->application_id;
		CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
		CHECK_FCT(fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp));
	}

	/* Auth-Request-Type AVP
	 * Enumerated values:
	 * 	AUTHENTICATE_ONLY 1
	 * 	AUTHORIZE_ONLY 2
	 * 	AUTHORIZE_AUTHENTICATE 3 */
	{
		CHECK_FCT(fd_msg_avp_new(dataobj_auth_request_type, 0, &avp));
		if (!diameap_config->authorize)
		{
			//AUTHENTICATE ONLY
			avp_val.i32 = AUTHENTICATE_ONLY;
		}
		else
		{
			if (diameap_sm->auth_request_val == 3)
			{
				//AUTHORIZE_AUTHENTICATE
				avp_val.i32 = AUTHORIZE_AUTHENTICATE;
			}
			else
			{
				//AUTHENTICATE_ONLY
				avp_val.i32 = AUTHENTICATE_ONLY;
			}
		}
		CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
		CHECK_FCT(fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp));
	}

	/* Proxy-Info AVP */
	CHECK_FCT(fd_msg_search_avp(req, dataobj_proxy_info, &avp));
	if (avp != NULL)
	{
		CHECK_FCT(fd_msg_avp_hdr ( avp, &avpdata ) );
		u32 proxy_info_code = avpdata->avp_code;
		int depth;
		do
		{
			ret = 0;
			depth = 0;
			CHECK_FCT(fd_msg_avp_hdr ( avp, &avpdata ) );
			if (avpdata->avp_code == proxy_info_code)
			{
				CHECK_FCT(fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp));
			}
			ret = fd_msg_browse ( avp, MSG_BRW_NEXT, &avp2, &depth);
			avp = avp2;
		} while ((avp != NULL) && (ret == 0) && (ret == 0));
	}

	if (diameap_sm->eap_sm.user.userid)
	{
		/* User-Name AVP */
		{
			CHECK_FCT(fd_msg_avp_new(dataobj_user_name, 0, &avp));

			if (diameap_sm->privateUser == FALSE)
			{

				avp_val.os.data = diameap_sm->eap_sm.user.userid;
				avp_val.os.len = diameap_sm->eap_sm.user.useridLength;
				CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
				CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
			}
			else
			{

			}

		}
	}
	return 0;
}

static int diameap_add_user_sessions_avps(
		struct diameap_state_machine * diameap_sm, struct msg * ans)
{
	TRACE_ENTRY("%p %p",diameap_sm,ans);

	struct avp * avp;
	union avp_value avp_val;
	int ret;

	/* Authorization-Lifetime AVP */
	{
		struct avp_attribute * ans_attrib;
		CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Authorization-Lifetime",&ans_attrib,1,&ret));
		if ((ret == 0) && (ans_attrib != NULL))
		{
			CHECK_FCT(fd_msg_avp_new(dataobj_authorization_lifetime, 0, &avp));
			avp_val.u32 = ans_attrib->value.u32;
			CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
			CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
			free_ans_attrib(ans_attrib);
		}
	}

	/* Auth-Grace-Period AVP */
	{
		struct avp_attribute * ans_attrib;
		CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Auth-Grace-Period",&ans_attrib,1,&ret));
		if ((ret == 0) && (ans_attrib != NULL))
		{
			CHECK_FCT(fd_msg_avp_new(dataobj_auth_grace_period, 0, &avp));
			avp_val.u32 = ans_attrib->value.u32;
			CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
			CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
			free_ans_attrib(ans_attrib);
		}
	}

	/* Auth-Session-State AVP */
	{
		struct avp_attribute * ans_attrib;
		CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Auth-Session-State",&ans_attrib,1,&ret));
		if ((ret == 0) && (ans_attrib != NULL))
		{
			CHECK_FCT(fd_msg_avp_new(dataobj_auth_session_state, 0, &avp));
			avp_val.i32 = ans_attrib->value.i32;
			CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
			CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
			free_ans_attrib(ans_attrib);
		}
	}

	/* Re-Auth-Request-Type AVP */
	{
		struct avp_attribute * ans_attrib;
		CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Re-Auth-Request-Type",&ans_attrib,1,&ret));
		if ((ret == 0) && (ans_attrib != NULL))
		{
			CHECK_FCT(fd_msg_avp_new(dataobj_re_auth_request_type, 0, &avp));
			avp_val.i32 = ans_attrib->value.i32;
			CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
			CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
			free_ans_attrib(ans_attrib);
		}
	}

	/* Session-Timeout AVP */
	{
		struct avp_attribute * ans_attrib;
		CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Session-Timeout",&ans_attrib,1,&ret));
		if ((ret == 0) && (ans_attrib != NULL))
		{
			CHECK_FCT(fd_msg_avp_new(dataobj_session_timeout, 0, &avp));
			avp_val.u32 = ans_attrib->value.u32;
			CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
			CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
			free_ans_attrib(ans_attrib);
		}
	}

	/* Acct-Interim-Interval AVP */
	{
		struct avp_attribute * ans_attrib;
		CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Acct-Interim-Interval",&ans_attrib,1,&ret));
		if ((ret == 0) && (ans_attrib != NULL))
		{
			CHECK_FCT(fd_msg_avp_new(dataobj_acct_interim_interval, 0, &avp));
			avp_val.u32 = ans_attrib->value.u32;
			CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
			CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
			free_ans_attrib(ans_attrib);
		}
	}
	return 0;
}

static int diameap_add_authorization_avps(
		struct diameap_state_machine * diameap_sm, struct msg * ans)
{

	TRACE_ENTRY("%p %p",diameap_sm, ans);

	struct avp * avp;
	union avp_value avp_val;
	int ret;

	if (diameap_sm == NULL)
	{
		return EINVAL;
	}
	if (ans == NULL)
	{
		return EINVAL;
	}

	/* Reply-Message */
	{
		struct avp_attribute * ans_attrib;
		CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Reply-Message",&ans_attrib,1,&ret));
		while ((ret == 0) && (ans_attrib != NULL))
		{
			CHECK_FCT(fd_msg_avp_new(dataobj_reply_message, 0, &avp));
			avp_val.os.data = ans_attrib->value.os.data;
			avp_val.os.len = ans_attrib->value.os.len;
			CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
			CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
			free_ans_attrib(ans_attrib);
			CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Reply-Message",&ans_attrib,1,&ret));
		}

	}

	/* Service-Type */
	{
		struct avp_attribute * ans_attrib;
		CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Service-Type",&ans_attrib,1,&ret));
		if ((ret == 0) && (ans_attrib != NULL))
		{
			CHECK_FCT(fd_msg_avp_new(dataobj_service_type, 0, &avp));
			avp_val.i32 = ans_attrib->value.i32;
			CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
			CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
			free_ans_attrib(ans_attrib);
		}
	}

	/* Callback-Number */
	{
		struct avp_attribute * ans_attrib;
		CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Callback-Number",&ans_attrib,1,&ret));
		if ((ret == 0) && (ans_attrib != NULL))
		{
			CHECK_FCT(fd_msg_avp_new(dataobj_callback_number, 0, &avp));
			avp_val.os.data = ans_attrib->value.os.data;
			avp_val.os.len = ans_attrib->value.os.len;
			CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
			CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
			free_ans_attrib(ans_attrib);
		}
	}

	/* Callback-Id */
	{
		struct avp_attribute * ans_attrib;
		CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Callback-Id",&ans_attrib,1,&ret));
		if ((ret == 0) && (ans_attrib != NULL))
		{
			CHECK_FCT(fd_msg_avp_new(dataobj_callback_id, 0, &avp));
			avp_val.os.data = ans_attrib->value.os.data;
			avp_val.os.len = ans_attrib->value.os.len;
			CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
			CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
			free_ans_attrib(ans_attrib);
		}
	}

	/* Idle-Timeout */
	{
		struct avp_attribute * ans_attrib;
		CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Idle-Timeout",&ans_attrib,1,&ret));
		if ((ret == 0) && (ans_attrib != NULL))
		{
			CHECK_FCT(fd_msg_avp_new(dataobj_idle_timeout, 0, &avp));
			avp_val.u32 = ans_attrib->value.u32;
			CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
			CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
			free_ans_attrib(ans_attrib);
		}
	}

	/* NAS-Filter-Rule */
	{
		struct avp_attribute * ans_attrib;
		CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"NAS-Filter-Rule",&ans_attrib,1,&ret));
		if ((ret == 0) && (ans_attrib != NULL))
		{
			CHECK_FCT(fd_msg_avp_new(dataobj_nas_filter_rule, 0, &avp));
			avp_val.u32 = ans_attrib->value.u32;
			CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
			CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
			free_ans_attrib(ans_attrib);
		}
	}

	/* Filter-Id */
	{
		struct avp_attribute * ans_attrib;
		CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Filter-Id",&ans_attrib,1,&ret));
		while ((ret == 0) && (ans_attrib != NULL))
		{
			CHECK_FCT(fd_msg_avp_new(dataobj_filter_id, 0, &avp));
			avp_val.os.data = ans_attrib->value.os.data;
			avp_val.os.len = ans_attrib->value.os.len;
			CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
			CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
			free_ans_attrib(ans_attrib);
			CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Filter-Id",&ans_attrib,1,&ret));
		}

	}

	/* Configuration-Token */
	{
		struct avp_attribute * ans_attrib;
		CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Configuration-Token",&ans_attrib,1,&ret));
		while ((ret == 0) && (ans_attrib != NULL))
		{
			CHECK_FCT(fd_msg_avp_new(dataobj_configuration_token, 0, &avp));
			avp_val.os.data = ans_attrib->value.os.data;
			avp_val.os.len = ans_attrib->value.os.len;
			CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
			CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
			free_ans_attrib(ans_attrib);
			CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Configuration-Token",&ans_attrib,1,&ret));
		}
	}

	/* QoS-Filter-Rule */
	{
		struct avp_attribute * ans_attrib;
		CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"QoS-Filter-Rule",&ans_attrib,1,&ret));
		while ((ret == 0) && (ans_attrib != NULL))
		{
			CHECK_FCT(fd_msg_avp_new(dataobj_qos_filter_rule, 0, &avp));
			avp_val.os.data = ans_attrib->value.os.data;
			avp_val.os.len = ans_attrib->value.os.len;
			CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
			CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
			free_ans_attrib(ans_attrib);
			CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"QoS-Filter-Rule",&ans_attrib,1,&ret));
		}

	}

	/* Framed-Protocol */
	{
		struct avp_attribute * ans_attrib;
		CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Framed-Protocol",&ans_attrib,1,&ret));
		if ((ret == 0) && (ans_attrib != NULL))
		{
			CHECK_FCT(fd_msg_avp_new(dataobj_framed_protocol, 0, &avp));
			avp_val.i32 = ans_attrib->value.i32;
			CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
			CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
			free_ans_attrib(ans_attrib);
		}
	}

	/* Framed-Routing */
	{
		struct avp_attribute * ans_attrib;
		CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Framed-Routing",&ans_attrib,1,&ret));
		if ((ret == 0) && (ans_attrib != NULL))
		{
			CHECK_FCT(fd_msg_avp_new(dataobj_framed_routing, 0, &avp));
			avp_val.i32 = ans_attrib->value.i32;
			CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
			CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
			free_ans_attrib(ans_attrib);
		}

	}

	/* Framed-MTU */
	{
		struct avp_attribute * ans_attrib;
		CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Framed-MTU",&ans_attrib,1,&ret));
		if ((ret == 0) && (ans_attrib != NULL))
		{
			CHECK_FCT(fd_msg_avp_new(dataobj_framed_mtu, 0, &avp));
			avp_val.u32 = ans_attrib->value.u32;
			CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
			CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
			free_ans_attrib(ans_attrib);
		}
	}

	/* Framed-Compression */
	{
		struct avp_attribute * ans_attrib;
		CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Framed-Compression",&ans_attrib,1,&ret));
		while ((ret == 0) && (ans_attrib != NULL))
		{
			CHECK_FCT(fd_msg_avp_new(dataobj_framed_compression, 0, &avp));
			avp_val.i32 = ans_attrib->value.i32;
			CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
			CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
			free_ans_attrib(ans_attrib);
			CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Framed-Compression",&ans_attrib,1,&ret));
		}

	}
	/* Framed-IP-Address */
	{
		struct avp_attribute * ans_attrib;
		CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Framed-IP-Address",&ans_attrib,1,&ret));
		if ((ret == 0) && (ans_attrib != NULL))
		{
			CHECK_FCT(fd_msg_avp_new(dataobj_framed_ip_address, 0, &avp));
			avp_val.os.data = ans_attrib->value.os.data;
			avp_val.os.len = ans_attrib->value.os.len;
			CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
			CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
			free_ans_attrib(ans_attrib);
		}
	}

	/* Framed-IP-Netmask */
	{
		struct avp_attribute * ans_attrib;
		CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Framed-IP-Netmask",&ans_attrib,1,&ret));
		if ((ret == 0) && (ans_attrib != NULL))
		{
			CHECK_FCT(fd_msg_avp_new(dataobj_framed_ip_netmask, 0, &avp));
			avp_val.os.data = ans_attrib->value.os.data;
			avp_val.os.len = ans_attrib->value.os.len;
			CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
			CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
			free_ans_attrib(ans_attrib);
		}
	}

	/* Framed-Route */
	{
		struct avp_attribute * ans_attrib;
		CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Framed-Route",&ans_attrib,1,&ret));
		while ((ret == 0) && (ans_attrib != NULL))
		{
			CHECK_FCT(fd_msg_avp_new(dataobj_framed_route, 0, &avp));
			avp_val.os.data = ans_attrib->value.os.data;
			avp_val.os.len = ans_attrib->value.os.len;
			CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
			CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
			free_ans_attrib(ans_attrib);
			CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Framed-Route",&ans_attrib,1,&ret));
		}
	}

	/* Framed-Pool */
	{
		struct avp_attribute * ans_attrib;
		CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Framed-Pool",&ans_attrib,1,&ret));
		if ((ret == 0) && (ans_attrib != NULL))
		{
			CHECK_FCT(fd_msg_avp_new(dataobj_framed_pool, 0, &avp));
			avp_val.os.data = ans_attrib->value.os.data;
			avp_val.os.len = ans_attrib->value.os.len;
			CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
			CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
			free_ans_attrib(ans_attrib);
		}
	}

	/* Framed-Interface-Id */
	{
		struct avp_attribute * ans_attrib;
		CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Framed-Interface-Id",&ans_attrib,1,&ret));
		if ((ret == 0) && (ans_attrib != NULL))
		{
			CHECK_FCT(fd_msg_avp_new(dataobj_framed_interface_id, 0, &avp));
			avp_val.u64 = ans_attrib->value.u64;
			CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
			CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
			free_ans_attrib(ans_attrib);
		}
	}

	/* Framed-IPv6-Prefix */
	{
		struct avp_attribute * ans_attrib;
		CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Framed-IPv6-Prefix",&ans_attrib,1,&ret));
		while ((ret == 0) && (ans_attrib != NULL))
		{
			CHECK_FCT(fd_msg_avp_new(dataobj_framed_ipv6_prefix, 0, &avp));
			avp_val.os.data = ans_attrib->value.os.data;
			avp_val.os.len = ans_attrib->value.os.len;
			CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
			CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
			free_ans_attrib(ans_attrib);
			CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Framed-IPv6-Prefix",&ans_attrib,1,&ret));
		}
	}

	/* Framed-IPv6-Route */
	{
		struct avp_attribute * ans_attrib;
		CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Framed-IPv6-Route",&ans_attrib,1,&ret));
		while ((ret == 0) && (ans_attrib != NULL))
		{
			CHECK_FCT(fd_msg_avp_new(dataobj_framed_ipv6_route, 0, &avp));
			avp_val.os.data = ans_attrib->value.os.data;
			avp_val.os.len = ans_attrib->value.os.len;
			CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
			CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
			free_ans_attrib(ans_attrib);
			CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Framed-IPv6-Route",&ans_attrib,1,&ret));
		}
	}

	/* Framed-IPv6-Pool */
	{
		struct avp_attribute * ans_attrib;
		CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Framed-IPv6-Pool",&ans_attrib,1,&ret));
		if ((ret == 0) && (ans_attrib != NULL))
		{
			CHECK_FCT(fd_msg_avp_new(dataobj_framed_ipv6_pool, 0, &avp));
			avp_val.os.data = ans_attrib->value.os.data;
			avp_val.os.len = ans_attrib->value.os.len;
			CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
			CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
			free_ans_attrib(ans_attrib);
		}
	}

	/* Framed-IPX-Network */
	{
		struct avp_attribute * ans_attrib;
		CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Framed-IPX-Network",&ans_attrib,1,&ret));
		if ((ret == 0) && (ans_attrib != NULL))
		{
			CHECK_FCT(fd_msg_avp_new(dataobj_framed_ipx_network, 0, &avp));
			avp_val.u32 = ans_attrib->value.u32;
			CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
			CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
			free_ans_attrib(ans_attrib);
		}

	}
	/* Framed-AppleTalk-Link */
	{
		struct avp_attribute * ans_attrib;
		CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Framed-AppleTalk-Link",&ans_attrib,1,&ret));
		if ((ret == 0) && (ans_attrib != NULL))
		{
			CHECK_FCT(fd_msg_avp_new(dataobj_framed_appletalk_link, 0, &avp));
			avp_val.u32 = ans_attrib->value.u32;
			CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
			CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
			free_ans_attrib(ans_attrib);
		}

	}

	/* Framed-AppleTalk-Network */
	{
		struct avp_attribute * ans_attrib;
		CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Framed-AppleTalk-Network",&ans_attrib,1,&ret));
		while ((ret == 0) && (ans_attrib != NULL))
		{
			CHECK_FCT(fd_msg_avp_new(dataobj_framed_appletalk_network, 0, &avp));
			avp_val.u32 = ans_attrib->value.u32;
			CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
			CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
			free_ans_attrib(ans_attrib);
			CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Framed-AppleTalk-Network",&ans_attrib,1,&ret));
		}

	}

	/* Framed-AppleTalk-Zone */
	{
		struct avp_attribute * ans_attrib;
		CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Framed-AppleTalk-Zone",&ans_attrib,1,&ret));
		if ((ret == 0) && (ans_attrib != NULL))
		{
			CHECK_FCT(fd_msg_avp_new(dataobj_framed_appletalk_zone, 0, &avp));
			avp_val.os.data = ans_attrib->value.os.data;
			avp_val.os.len = ans_attrib->value.os.len;
			CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
			CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
			free_ans_attrib(ans_attrib);
		}
	}

	/* Tunneling */
	//

	/* State */
	//
	return 0;
}

static int diameap_add_result_code(struct diameap_state_machine * diameap_sm,
		struct msg * ans, struct session * sess)
{
	TRACE_ENTRY("%p %p",diameap_sm,ans);
	struct avp * avp;
	union avp_value avp_val;
	int ret;

	/* Result-Code AVP */
	CHECK_FCT(fd_msg_avp_new(dataobj_result_code, 0, &avp));
	avp_val.u32 = diameap_sm->result_code;
	CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
	/* Add Result-Code AVP to the message */
	CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );

	/* Multi_Round_Time_Out AVP */
	if (diameap_sm->result_code == 1001)
	{
		struct timespec sess_timeout;
		CHECK_FCT(fd_msg_avp_new(dataobj_multi_round_time_out, 0, &avp));
		struct avp_attribute * ans_attrib;
		CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Multi-Round-Time-Out",&ans_attrib,1,&ret));
		if ((ret == 0) && (ans_attrib != NULL))
		{
			avp_val.u32 = ans_attrib->value.u32;
			/* Update the session timeout with multi-round-time-out value */
			CHECK_SYS(clock_gettime(CLOCK_REALTIME,&sess_timeout));
			sess_timeout.tv_sec += diameap_config->multi_round_time_out;
			CHECK_FCT(fd_sess_settimeout(sess, &sess_timeout));
			free_ans_attrib(ans_attrib);
		}
		else
		{
			avp_val.u32 = diameap_config->multi_round_time_out;
			/* Update the session timeout with multi-round-time-out value */
			CHECK_SYS(clock_gettime(CLOCK_REALTIME,&sess_timeout));
			sess_timeout.tv_sec += diameap_config->multi_round_time_out;
			CHECK_FCT(fd_sess_settimeout(sess, &sess_timeout));
		}
		CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
		CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
	}
	return 0;
}

static int diameap_add_eap_payload(struct diameap_state_machine * diameap_sm,
		struct msg * ans, struct diameap_eap_interface *eap_i)
{
	TRACE_ENTRY("%p %p",diameap_sm,ans);
	struct avp * avp;
	union avp_value avp_val;
	int ret;
	u32 Framed_MTU = 1500; //1500 default value
	u32 NAS_Port_Type_HeaderLength = 4;
	int EAP_Max_Length = 0;

	/* get Framed-MTU AVP value */
	{
		struct avp_attribute * ans_attrib;
		CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"Framed-MTU",&ans_attrib,0,&ret));
		if ((ret == 0) && (ans_attrib != NULL))
		{
			Framed_MTU = ans_attrib->value.u32;
		}
	}

	/* get NAS-Port-Type AVP value */
	{
		struct avp_attribute * ans_attrib;
		CHECK_FCT(diameap_get_ans_attribute(&diameap_sm->ans_attributes,"NAS-Port-Type",&ans_attrib,0,&ret));
		if ((ret == 0) && (ans_attrib != NULL))
		{
			// = ans_attrib->value.i32;
		}
	}

	//TD take the link type into consideration when calculating EAP_MAX_Length
	EAP_Max_Length = Framed_MTU - NAS_Port_Type_HeaderLength;

	if (eap_i->aaaEapReqData.length <= EAP_Max_Length)
	{

		/* EAP-Payload AVP */
		{
			CHECK_FCT(fd_msg_avp_new(dataobj_eap_payload, 0, &avp));
			avp_val.os.data = eap_i->aaaEapReqData.data;
			avp_val.os.len = eap_i->aaaEapReqData.length;
			CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
			CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );

			if (diameap_sm->lastReqEAPavp != NULL)
			{
				fd_msg_free(diameap_sm->lastReqEAPavp);
				diameap_sm->lastReqEAPavp = NULL;
			}

			CHECK_FCT(fd_msg_avp_new(dataobj_eap_payload, 0, &diameap_sm->lastReqEAPavp));
			avp_val.os.data = eap_i->aaaEapReqData.data;
			avp_val.os.len = eap_i->aaaEapReqData.length;
			CHECK_FCT(fd_msg_avp_setvalue(diameap_sm->lastReqEAPavp, &avp_val));

		}
	}
	else
	{
		//if EAP Packet length > EAP_Max_Length
	}
	return 0;
}

static int diameap_send(struct msg ** rmsg)
{
	TRACE_ENTRY("%p",rmsg);
	CHECK_FCT( fd_msg_send( rmsg, NULL, NULL));
	return 0;
}

static int diameap_add_eap_success_avps(
		struct diameap_state_machine * diameap_sm, struct msg * ans,
		struct diameap_eap_interface *eap_i)
{
	TRACE_ENTRY("%p %p %p",diameap_sm,ans,eap_i);
	struct avp * avp;
	union avp_value avp_val;
	int ret;

	/* EAP-Master-Session-Key AVP */
	if (eap_i->aaaEapKeyAvailable == TRUE)
	{
		CHECK_FCT(fd_msg_avp_new(dataobj_eap_master_session_key, 0, &avp));
		avp_val.os.data = eap_i->aaaEapMSKData;
		avp_val.os.len = eap_i->aaaEapMSKLength;
		CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
		CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );

	}

	/* EAP-Key-Name AVP */
	struct avp_attribute * avp_attrib = NULL;

	CHECK_FCT(diameap_get_avp_attribute(&diameap_sm->req_attributes,"EAP-Key-Name",&avp_attrib,1,&ret))
	if ((avp_attrib != NULL) && (ret != 1))
	{
		if (avp_attrib->value.os.len == 0)
		{
			CHECK_FCT(fd_msg_avp_new(dataobj_eap_key_name, 0, &avp));
			avp_val.os.data = NULL;//
			avp_val.os.len = 0;//
			CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
			CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
			free_avp_attrib(avp_attrib);
		}
	}

	return 0;
}

int diameap_authorize(struct diameap_state_machine * diameap_sm)
{
	TRACE_ENTRY("%p",diameap_sm);

	CHECK_FCT(diameap_authorization_get_attribs(&diameap_sm->eap_sm.user, &diameap_sm->attributes));

	diameap_sm->authorized = TRUE;

	CHECK_FCT_DO(diameap_answer_authorization_attributes(diameap_sm),
	);
	if (diameap_sm->authorized == FALSE)
	{
		diameap_sm->result_code = 4001; /* DIAMETER_AUTHENTICATION_REJECTED 4001 */
	}
	return 0;
}

static int diameap_add_accounting_eap_auth_method(
		struct diameap_state_machine * diameap_sm, struct msg * ans)
{
	TRACE_ENTRY("%p %p",diameap_sm,ans);
	int i = 0;
	struct avp * avp;
	union avp_value avp_val;
	/* Accounting-EAP-Auth-Method AVP */
	while (i < diameap_sm->eap_sm.user.methodId)
	{
		CHECK_FCT(fd_msg_avp_new(dataobj_accounting_eap_auth_method, 0, &avp));

		avp_val.u64 = (u64) (((diameap_sm->eap_sm.user.methods[i].vendor)
				* pow((double) 2, (double) 32))
				+ diameap_sm->eap_sm.user.methods[i].method);
		CHECK_FCT(fd_msg_avp_setvalue(avp, &avp_val));
		CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
		i++;
	}
	return 0;
}

static int diameap_add_eap_reissued_payload(struct msg * ans, struct msg * req)
{
	TRACE_ENTRY("%p %p", ans, req);
	struct avp * avp, *re_avp;
	union avp_value avp_val;
	struct avp_hdr * avphdr;

	if ((ans == NULL) || (req == NULL))
	{
		return EINVAL;
	}
	avp = NULL;
	CHECK_FCT(fd_msg_search_avp(req, dataobj_eap_payload, &avp));
	if (avp != NULL)
	{
		CHECK_FCT( fd_msg_avp_hdr(avp, &avphdr));
		CHECK_FCT( fd_msg_avp_new(dataobj_eap_reissued_payload, 0, &re_avp));
		CHECK_MALLOC(avp_val.os.data=malloc(avphdr->avp_value->os.len));
		memcpy(avp_val.os.data,avphdr->avp_value->os.data,avphdr->avp_value->os.len);
		avp_val.os.len = avphdr->avp_value->os.len;
		CHECK_FCT(fd_msg_avp_setvalue(re_avp, &avp_val));
		CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, re_avp ) );
	}
	else
	{
		TRACE_DEBUG(INFO,"%sUnable to find EAP-Payload AVP in received Diameter-EAP-Request.",DIAMEAP_EXTENSION);
		return 1;
	}

	return 0;
}




static int diameap_server_callback(struct msg ** rmsg, struct avp * ravp,
		struct session * sess, void * opaque, enum disp_action * action)
{
	TRACE_ENTRY("%p %p %p %p", rmsg, ravp, sess, action);

	struct sess_state * diameap_sess_data = NULL;
	struct diameap_state_machine * diameap_sm = NULL;
	struct diameap_eap_interface eap_i;
	struct msg *req, *ans;
	boolean non_fatal_error = FALSE;

	if (rmsg == NULL)
		return EINVAL;

	req = *rmsg;

	CHECK_FCT_DO(fd_sess_state_retrieve(diameap_server_reg, sess, &diameap_sess_data),
			{	TRACE_DEBUG(INFO,"%s retrieving session state failed.",DIAMEAP_EXTENSION); goto s_end;});

	CHECK_MALLOC_DO(diameap_sm = malloc(sizeof(struct diameap_state_machine)),
			goto s_end);
	memset(diameap_sm, 0, sizeof(struct diameap_state_machine));

	if (diameap_sess_data)
	{
		diameap_sm->state = DIAMEAP_RECEIVED;
		diameap_sm->eap_sm.eap_state = EAP_IDLE;
	}
	else
	{
		diameap_sm->state = DIAMEAP_DISABLED;
		diameap_sm->eap_sm.eap_state = EAP_INITIALIZE;
	}

	while (diameap_sm->state != DIAMEAP_IDLE && diameap_sm->state
			!= DIAMEAP_END)
	{
		switch (diameap_sm->state)
		{
		case DIAMEAP_DISABLED:
			if (rmsg)
			{
				diameap_sm->state = DIAMEAP_INITIALIZE;
			}
			else
			{
				TRACE_DEBUG(INFO,"%sReceived empty Diameter EAP Request message.",DIAMEAP_EXTENSION);
				goto s_end;
			}
			break;

		case DIAMEAP_INITIALIZE:

			CHECK_FCT_DO(diameap_initialize_diameap_sm(diameap_sm,diameap_sess_data),
					{	TRACE_DEBUG(INFO,"%s Initializing DiamEAP state machine failed.",DIAMEAP_EXTENSION); goto s_end;})
			;
			CHECK_FCT_DO(diameap_initialize_diameap_eap_interface(&eap_i),
					{	TRACE_DEBUG(INFO,"%s Initializing DiamEAP-EAP Interface failed.",DIAMEAP_EXTENSION); goto s_end;})
			;
			TRACE_DEBUG(FULL+1,"%sParsing AVPs",DIAMEAP_EXTENSION)
			;
			CHECK_FCT_DO(diameap_parse_avps(diameap_sm, req, &eap_i), TRACE_DEBUG(INFO,"%s Unable to parse Diameter-EAP-Request AVPs.",DIAMEAP_EXTENSION))
			;

			if ((diameap_sm->result_code != 0))
			{
				diameap_sm->state = DIAMEAP_SEND_ERROR_MSG;
			}
			else
			{
				diameap_sm->state = DIAMEAP_AUTHENTICATION_VERIFY;
			}
			break;

		case DIAMEAP_RECEIVED:

			CHECK_FCT_DO(diameap_initialize_diameap_sm(diameap_sm,diameap_sess_data),
					{	TRACE_DEBUG(INFO,"%s Initializing DiamEAP state machine failed.",DIAMEAP_EXTENSION); goto s_end;})
			;
			CHECK_FCT_DO(diameap_initialize_diameap_eap_interface(&eap_i),
					{	TRACE_DEBUG(INFO,"%s Initializing DiamEAP-EAP Interface failed.",DIAMEAP_EXTENSION); goto s_end;})
			;
			TRACE_DEBUG(FULL+1,"%sParsing AVPs",DIAMEAP_EXTENSION)
			;
			CHECK_FCT_DO(diameap_parse_avps(diameap_sm, req, &eap_i), TRACE_DEBUG(INFO,"%s Unable to parse Diameter-EAP-Request AVPs.",DIAMEAP_EXTENSION))
			;

			if (diameap_sm->result_code != 0)
			{
				diameap_sm->state = DIAMEAP_SEND_ERROR_MSG;
			}
			else
			{
				diameap_sm->state = DIAMEAP_AUTHENTICATION_VERIFY;
			}
			break;

		case DIAMEAP_AUTHENTICATION_VERIFY:
		{

			TRACE_DEBUG(FULL+1,"%sVerify authentication",DIAMEAP_EXTENSION);
			CHECK_FCT_DO(diameap_eap_statemachine(&diameap_sm->eap_sm, &eap_i,&non_fatal_error),
					{	TRACE_DEBUG(INFO,"%s EAP process failed.",DIAMEAP_EXTENSION); goto s_end;});

			if (non_fatal_error == TRUE)
			{
				TRACE_DEBUG(FULL+1,"%sAuthentication verify finished with a non-fatal-error.",DIAMEAP_EXTENSION);
				diameap_sm->state = DIAMEAP_SEND_ERROR_MSG;
			}
			else
			{
				diameap_sm->state = DIAMEAP_SELECT_DECISION;

			}
		}
			break;

		case DIAMEAP_SELECT_DECISION:

			CHECK_FCT_DO( diameap_policy_decision(diameap_sm,&eap_i),
					goto s_end)
			;

			if ((eap_i.aaaSuccess == TRUE) && (diameap_sm->auth_request_val
					== AUTHORIZE_AUTHENTICATE)
					&& (diameap_sm->verify_authorization == FALSE))
			{
				diameap_sm->state = DIAMEAP_AUTHORIZATION_VERIFY;
			}
			else
			{
				diameap_sm->state = DIAMEAP_DIAMETER_EAP_ANSWER;
			}
			break;

		case DIAMEAP_AUTHORIZATION_VERIFY:
			diameap_sm->verify_authorization = TRUE;
			TRACE_DEBUG(FULL+1,"%sVerify authorization",DIAMEAP_EXTENSION)
			;
			CHECK_FCT_DO(diameap_authorize(diameap_sm),
					{	TRACE_DEBUG(INFO,"%s Authorization check process failed.",DIAMEAP_EXTENSION); goto s_end;})
			;
			diameap_sm->state = DIAMEAP_SELECT_DECISION;

			break;

		case DIAMEAP_DIAMETER_EAP_ANSWER:
			TRACE_DEBUG(FULL+1,"%screate Diameter EAP Answer",DIAMEAP_EXTENSION)
			;
			CHECK_FCT_DO(fd_msg_new_answer_from_req(fd_g_config->cnf_dict, rmsg, 0),
					goto s_end)
			;
			ans = *rmsg;
			TRACE_DEBUG(FULL+1,"%sAdding AVPs to Diameter EAP Answer.",DIAMEAP_EXTENSION)
			;
			CHECK_FCT_DO( diameap_add_avps(diameap_sm, ans,req),
					{	TRACE_DEBUG(INFO,"%s Unable to add AVPs to Diameter-EAP-Answer message.",DIAMEAP_EXTENSION);goto s_end;})
			;
			if (diameap_sm->authFailure == FALSE)
			{
				if (diameap_sm->eap_sm.user.id != 0)
				{
					TRACE_DEBUG(FULL+1,"%sSelect authentication attributes.",DIAMEAP_EXTENSION);
					CHECK_FCT_DO(diameap_authentication_get_attribs(&diameap_sm->eap_sm.user, &diameap_sm->attributes),
							{	TRACE_DEBUG(INFO,"%s Unable to get user's session attributes.",DIAMEAP_EXTENSION); goto s_end;});
					TRACE_DEBUG(FULL+1,"%sCreate answer authentication attributes.",DIAMEAP_EXTENSION);
					CHECK_FCT_DO(diameap_answer_avp_attributes(diameap_sm),
							{	TRACE_DEBUG(INFO,"%s Unable to generate answer attributes.",DIAMEAP_EXTENSION); goto s_end;});
				}

				if (diameap_sm->authSuccess == FALSE)
				{
					diameap_sm->state = DIAMEAP_SEND_REQUEST;
				}
				else
				{

					diameap_sm->state = DIAMEAP_SEND_SUCCESS;
				}
			}
			else
			{
				diameap_sm->state = DIAMEAP_SEND_FAILURE;
			}
			break;

		case DIAMEAP_SEND_REQUEST:
			TRACE_DEBUG(FULL+1,"%sAdding Result Code AVP to Diameter-EAP-Answer.",DIAMEAP_EXTENSION)
			;
			CHECK_FCT_DO( diameap_add_result_code(diameap_sm, ans, sess),
					{	TRACE_DEBUG(INFO,"%s Adding Result-Code AVP failed.",DIAMEAP_EXTENSION); goto s_end;})
			;
			TRACE_DEBUG(FULL+1,"%sAdding EAP-Payload to Diameter-EAP-Answer.",DIAMEAP_EXTENSION)
			;
			CHECK_FCT_DO( diameap_add_eap_payload(diameap_sm, ans,&eap_i),
					{	TRACE_DEBUG(INFO,"%s Adding EAP-Payload AVP failed.",DIAMEAP_EXTENSION); goto s_end;})
			;
			TRACE_DEBUG(FULL+1,"%sStoring DiamEAP session data.",DIAMEAP_EXTENSION)
			;
			CHECK_MALLOC(diameap_sess_data = malloc(sizeof(struct sess_state)))
			;
			memset(diameap_sess_data, 0, sizeof(struct sess_state));
			diameap_sess_data_new(diameap_sess_data, diameap_sm);

			CHECK_FCT_DO(fd_sess_state_store(diameap_server_reg, sess, &diameap_sess_data),
					{	TRACE_DEBUG(INFO,"%s Storing session state failed.",DIAMEAP_EXTENSION); goto s_end;})
			;

			CHECK_FCT_DO( diameap_send(rmsg),
					goto s_end)
			;

			diameap_sm->state = DIAMEAP_IDLE;
			break;

		case DIAMEAP_SEND_FAILURE:
			TRACE_DEBUG(FULL+1,"%sAdding Result Code AVP to Diameter-EAP-Answer.",DIAMEAP_EXTENSION)
			;
			CHECK_FCT_DO( diameap_add_result_code(diameap_sm, ans, sess),
					{	TRACE_DEBUG(INFO,"%s Adding Result-Code AVP failed.",DIAMEAP_EXTENSION); goto s_end;})
			;
			TRACE_DEBUG(FULL+1,"%sAdding EAP-Payload to Diameter-EAP-Answer.",DIAMEAP_EXTENSION)
			;
			CHECK_FCT_DO( diameap_add_eap_payload(diameap_sm, ans,&eap_i),
					{	TRACE_DEBUG(INFO,"%s Adding EAP-Payload AVP failed.",DIAMEAP_EXTENSION); goto s_end;})
			;

			LOG_N("%s Auth FAIL: %.*s",DIAMEAP_EXTENSION, diameap_sm->eap_sm.user.useridLength, diameap_sm->eap_sm.user.userid);
			
			CHECK_FCT_DO( diameap_send(rmsg),
					goto s_end)
			;
			diameap_sm->state = DIAMEAP_END;
			break;

		case DIAMEAP_SEND_SUCCESS:
			TRACE_DEBUG(FULL+1,"%sAdding User session AVPs to Diameter-EAP-Answer.",DIAMEAP_EXTENSION)
			;
			CHECK_FCT_DO(diameap_add_user_sessions_avps(diameap_sm,ans),
					{	TRACE_DEBUG(INFO,"%s Adding user's session AVPs failed.",DIAMEAP_EXTENSION); goto s_end;})
			;

			if (diameap_sm->auth_request_val == AUTHORIZE_AUTHENTICATE)
			{
				TRACE_DEBUG(FULL+1,"%sAdding Authorization AVPs to Diameter-EAP-Answer.",DIAMEAP_EXTENSION);
				CHECK_FCT_DO(diameap_add_authorization_avps(diameap_sm,ans),
						{	TRACE_DEBUG(INFO,"%s Adding Authorization AVPs failed.",DIAMEAP_EXTENSION); goto s_end;});
			}
			TRACE_DEBUG(FULL+1,"%sAdding Result Code AVP to Diameter-EAP-Answer.",DIAMEAP_EXTENSION)
			;
			CHECK_FCT_DO( diameap_add_result_code(diameap_sm, ans, sess),
					{	TRACE_DEBUG(INFO,"%s Adding Result-Code AVP failed.",DIAMEAP_EXTENSION); goto s_end;})
			;
			TRACE_DEBUG(FULL+1,"%sAdding EAP-Payload to Diameter-EAP-Answer.",DIAMEAP_EXTENSION)
			;
			CHECK_FCT_DO( diameap_add_eap_payload(diameap_sm, ans,&eap_i),
					{	TRACE_DEBUG(INFO,"%s Adding EAP-Payload AVP failed.",DIAMEAP_EXTENSION); goto s_end;})
			;
			TRACE_DEBUG(FULL+1,"%sAdding EAP success AVPs AVPs to Diameter-EAP-Answer.",DIAMEAP_EXTENSION)
			;
			CHECK_FCT_DO( diameap_add_eap_success_avps(diameap_sm, ans, &eap_i),
					goto s_end)
			;
			TRACE_DEBUG(FULL+1,"%sAdding Accounting-EAP-Auth-Method AVPs to Diameter-EAP-Answer.",DIAMEAP_EXTENSION)
			;
			CHECK_FCT_DO(diameap_add_accounting_eap_auth_method(diameap_sm, ans),
					{	TRACE_DEBUG(INFO,"%s Adding accounting AVP failed",DIAMEAP_EXTENSION); goto s_end;})
			;
			
			LOG_N("%s Auth Success: %.*s",DIAMEAP_EXTENSION, diameap_sm->eap_sm.user.useridLength, diameap_sm->eap_sm.user.userid);
			
			CHECK_FCT_DO( diameap_send(rmsg),
					goto s_end)
			;
			diameap_sm->state = DIAMEAP_END;
			break;

		case DIAMEAP_SEND_ERROR_MSG:
			diameap_sm->invalid_eappackets++;
			if (diameap_sm->invalid_eappackets
					== diameap_config->max_invalid_eap_packet)
			{
				diameap_sm->result_code = 4001;//DIAMETER_AUTHENTICATION_REJECTED
				TRACE_DEBUG(FULL,"%s Maximum permitted invalid EAP Packet reached. Diameter Authentication Rejected.",DIAMEAP_EXTENSION);
			}

			CHECK_FCT_DO(fd_msg_new_answer_from_req(fd_g_config->cnf_dict, rmsg, 0),
					goto s_end)
			;

			ans = *rmsg;
			CHECK_FCT_DO( diameap_add_avps(diameap_sm, ans,req),
					{	TRACE_DEBUG(INFO,"%s Adding AVPs to Diameter-EAP-Answer message failed.",DIAMEAP_EXTENSION);goto s_end;})
			;
			if ((non_fatal_error == TRUE) && (diameap_sm->result_code == 0))
			{
				diameap_sm->result_code = 1001;
			}

			if (diameap_sm->result_code == 1001)
			{
				CHECK_FCT_DO( diameap_add_eap_reissued_payload(ans,req), goto s_end);
			}

			if (diameap_sm->result_code == 5004)
			{
				CHECK_FCT_DO( fd_msg_avp_add( ans , MSG_BRW_LAST_CHILD, diameap_sm->failedavp ),goto s_end );
			}

			CHECK_FCT_DO( diameap_add_result_code(diameap_sm, ans, sess), goto s_end)
			;

			CHECK_FCT_DO( diameap_send(rmsg), goto s_end)
			;
			diameap_sm->state = DIAMEAP_IDLE;
			break;

		case DIAMEAP_END:
			break;

		case DIAMEAP_IDLE:
			break;
		}
	}

	diameap_free(diameap_sm);

	s_end: return 0;
}

int diameap_start_server(void)
{
	struct disp_when when;

	/*create handler for sessions */
	CHECK_FCT(fd_sess_handler_create(&diameap_server_reg, diameap_cli_sess_cleanup, NULL, NULL));

	/* Register the callback */
	memset(&when, 0, sizeof(when));
	when.command = dataobj_diameap_cmd;
	when.app = dataobj_diameap_app;

	/* Register the callback for EAP Application */
	CHECK_FCT(fd_disp_register(diameap_server_callback, DISP_HOW_CC, &when, NULL,
					&handle));

	if (handle == NULL)
	{
		TRACE_DEBUG(INFO, "%sCannot register the callback !!!",DIAMEAP_EXTENSION);
		return 1;
	}
	return 0;
}

int diameap_stop_server(void)
{
	CHECK_FCT(fd_sess_handler_destroy(&diameap_server_reg, NULL));
	CHECK_FCT(fd_disp_unregister(&handle, NULL));

	return 0;
}
