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

static void diameap_ba_nextid(struct eap_state_machine * sm, int * id)
{
	TRACE_ENTRY("%p %p",sm,id);

	if (sm->currentId < 0)
	{
		*id = (u8) (255 * rand() / RAND_MAX) & 0xFFU;
	}
	else
	{
		*id = (sm->currentId++) & 0xFFU;
	}
	if (*id == sm->lastId)
	{
		*id=*id+1;
	}
}

static void diameap_ba_policyupdate(struct eap_state_machine * eap_sm,
		struct eap_packet *eapPacket)
{
	TRACE_ENTRY("%p %p",eap_sm, eapPacket);
	if ((eap_sm->respMethod == TYPE_NAK))
	{
		int id;
		eap_sm->user.pmethods = 0;
		u32 vendor;
		eap_type type;
		u8 *data = (u8 *) eapPacket->data;
		data += 5;
		id = 5;
		while (id < eapPacket->length)
		{
			vendor = VENDOR_IETF;
			type = G8(data);
			if (diameap_plugin_exist(vendor, type) == TRUE)
			{
				eap_sm->user.proposedmethods[id - 5].method = type;
				eap_sm->user.proposedmethods[id - 5].vendor = vendor;
				eap_sm->user.pmethods++;
			}
			data++;
			id++;
		}
		eap_sm->user.methodId = -1;
	}
}

static int diameap_ba_policygetnextmethod(struct eap_state_machine * eap_sm,
		eap_type * eaptype, u32 * vendor)
{
	TRACE_ENTRY("%p %p %p",eap_sm,eaptype,vendor);
	*vendor = 0;
	*eaptype = TYPE_NONE;
	if (eap_sm == NULL)
	{
		return EINVAL;
	}

	eap_sm->selectedMethod = NULL;

	if (eap_sm->user.userid == NULL)
	{
		if ((eap_sm->currentMethod == TYPE_NONE))
		{
			*vendor = VENDOR_IETF;
			*eaptype = TYPE_IDENTITY;
			if (eap_sm->selectedMethod != NULL)
			{
				(*eap_sm->selectedMethod->eap_method_free)(eap_sm->methodData);
				eap_sm->methodData = NULL;
			}
			CHECK_FCT(diameap_plugin_get(VENDOR_IETF,TYPE_IDENTITY,&eap_sm->selectedMethod));
			return 0;
		}

		eap_sm->selectedMethod = NULL;
		*vendor = 0;
		*eaptype = TYPE_NONE;
		return 0;
	}

	if (eap_sm->user.methodId == -1)
	{
		if (eap_sm->user.proposed_eap_method >= TYPE_EAP_MD5)
		{
			*vendor = eap_sm->user.proposed_eap_method_vendor;
			if (*vendor == VENDOR_IETF)
			{
				*eaptype = eap_sm->user.proposed_eap_method;
			}
			else
			{
				*eaptype = TYPE_EXPANDED_TYPES;
			}
			if (eap_sm->selectedMethod != NULL)
			{
				(*eap_sm->selectedMethod->eap_method_free)(eap_sm->methodData);
				eap_sm->methodData = NULL;
			}
			CHECK_FCT_DO(diameap_plugin_get(*vendor,*eaptype,&eap_sm->selectedMethod),
					{	TRACE_DEBUG(INFO,"%s [EAP Protocol] Invalid EAP-TYPE %d (vendor %d)",DIAMEAP_EXTENSION,*eaptype,*vendor);return 1;});

		}
		eap_sm->user.proposed_eap_method = TYPE_NONE;
	}
	else
	{
		*vendor = eap_sm->user.proposedmethods[eap_sm->user.methodId].vendor;
		if (eap_sm->user.proposedmethods[eap_sm->user.methodId].vendor
				== VENDOR_IETF)
		{
			*eaptype
					= eap_sm->user.proposedmethods[eap_sm->user.methodId].method;
		}
		else
		{
			*eaptype = TYPE_EXPANDED_TYPES;
		}
		if (eap_sm->selectedMethod != NULL)
		{
			(*eap_sm->selectedMethod->eap_method_free)(eap_sm->methodData);
			eap_sm->methodData=NULL;
		}
		CHECK_FCT(diameap_plugin_get(eap_sm->user.proposedmethods[eap_sm->user.methodId].vendor,eap_sm->user.proposedmethods[eap_sm->user.methodId].method,&eap_sm->selectedMethod));

		eap_sm->user.methodId++;
	}

	return 0;
}

static int diameap_ba_policygetdecision(struct eap_state_machine * eap_sm,
		struct diameap_eap_interface * eap_i, decision * gdecision)
{
	TRACE_ENTRY("%p %p %p",eap_sm,eap_i,gdecision);

	if (eap_sm->user.userid != NULL)
	{

		if (eap_sm->methodState == EAP_M_END)
		{

			if (eap_sm->respMethod == TYPE_IDENTITY)
			{

				*gdecision = DECISION_CONTINUE;
				return 0;
			}

			if ((eap_sm->respMethod == TYPE_NAK) || ((eap_sm->respMethod
					== TYPE_EXPANDED_TYPES) && (eap_sm->respVendor
					== VENDOR_IETF) && (eap_sm->respVendorMethod == TYPE_NAK)))
			{
				goto SelectNextMethod;
			}

			if (eap_sm->user.success == TRUE)
			{

				*gdecision = DECISION_SUCCESS;
			}
			else
			{

				*gdecision = DECISION_FAILURE;
			}

		}
		else
		{
			goto SelectNextMethod;
		}
		return 0;

		SelectNextMethod: if ((eap_sm->user.methodId
				== (MAXPROPOSEDMETHODS - 1))
				|| ((eap_sm->user.proposedmethods[eap_sm->user.methodId + 1].method
						== TYPE_NONE)
						&& (eap_sm->user.proposedmethods[eap_sm->user.methodId
								+ 1].vendor == VENDOR_IETF)))
		{
			TRACE_DEBUG(FULL+1,
					"%s [EAP protocol] None of proposed EAP Methods authenticated the user.(FAILURE)",DIAMEAP_EXTENSION);
			*gdecision = DECISION_FAILURE;
			return 0;
		}

		eap_sm->user.methodId = 0;
		*gdecision = DECISION_CONTINUE;
		return 0;
	}

	if (eap_sm->currentMethod == TYPE_IDENTITY)
	{
		*gdecision = DECISION_FAILURE;
		return 0;
	}

	*gdecision = DECISION_CONTINUE;
	return 0;
}

static boolean diameap_ba_policydopickup(eap_type type)
{
	TRACE_ENTRY("%d",type);
	if (type == TYPE_IDENTITY)
	{
		return TRUE;
	}
	return FALSE;
}

int diameap_eap_statemachine(struct eap_state_machine * eap_sm,
		struct diameap_eap_interface * eap_i, boolean * non_fatal_error)
{
	TRACE_ENTRY("%p %p %p", eap_sm, eap_i, non_fatal_error);
	int ret;

	if ((eap_sm->eap_state == EAP_IDLE) && (eap_i->aaaEapResp == TRUE))
	{
		eap_sm->eap_state = EAP_RECEIVED;
	}
	while (!((eap_sm->eap_state == EAP_IDLE) || (eap_sm->eap_state == EAP_END)))
	{
		switch (eap_sm->eap_state)
		{
		case EAP_INITIALIZE:
			if (eap_sm->rxResp == TRUE)
			{
				eap_sm->lastId = eap_sm->currentId;
				eap_sm->currentId = eap_sm->respId;
			}
			else
			{
				eap_sm->lastId = -1;
				eap_sm->currentId = -1;
			}
			if (eap_sm->rxResp == FALSE)
			{
				eap_sm->eap_state = EAP_SELECT_ACTION;
			}
			else if ((eap_sm->respMethod == TYPE_NAK) || (eap_sm->respMethod
					== TYPE_EXPANDED_TYPES && eap_sm->respVendor == VENDOR_IETF
					&& eap_sm->respVendorMethod == TYPE_NAK))
			{
				eap_sm->eap_state = EAP_NAK;
			}
			else
			{
				eap_sm->eap_state = EAP_PICK_UP_METHOD;
			}
			break;
		case EAP_PICK_UP_METHOD:
			if (diameap_ba_policydopickup(eap_sm->respMethod) == TRUE)
			{
				eap_sm->currentMethod = eap_sm->respMethod;

				if (diameap_plugin_get(eap_sm->currentVendor,
						eap_sm->currentMethod, &eap_sm->selectedMethod))
				{
					TRACE_DEBUG(INFO,"%sNo EAP Method plugin available for EAP Method {Type=%d, Vendor=%d}.",DIAMEAP_EXTENSION,eap_sm->currentMethod, eap_sm->currentVendor);
				}
				else
				{
					TRACE_DEBUG(FULL,"%sCurrent EAP Method {Type=%d, Vendor=%d} (EAP Method plugin selected).",DIAMEAP_EXTENSION,eap_sm->currentMethod, eap_sm->currentVendor);
				}
				eap_sm->currentVendor = VENDOR_IETF;
				if (eap_sm->selectedMethod != NULL)
				{
					ret = (*eap_sm->selectedMethod->eap_method_initPickUp)(
							eap_sm);
					if (ret)
					{
						TRACE_DEBUG(INFO, "%sEAP Method InitPickUp returned error.",DIAMEAP_EXTENSION);
						eap_sm->selectedMethod = NULL;
						eap_sm->currentMethod = TYPE_NONE;
					}
				}
			}
			if (eap_sm->currentMethod == TYPE_NONE)
			{
				eap_sm->eap_state = EAP_SELECT_ACTION;
			}
			else
			{
				eap_sm->eap_state = EAP_METHOD_RESPONSE;
			}
			break;

		case EAP_RECEIVED:
			TRACE_DEBUG(FULL+1,"%s[EAP Protocol] New EAP Response received",DIAMEAP_EXTENSION)
			;
			diameap_eap_dump(FULL + 1, &eap_i->aaaEapRespData);
			if ((eap_sm->rxResp == TRUE) && (eap_sm->respId
					== eap_sm->currentId) && ((eap_sm->respMethod
					== eap_sm->currentMethod) || ((eap_sm->respMethod
					== TYPE_EXPANDED_TYPES) && (eap_sm->respVendor
					== VENDOR_IETF) && (eap_sm->respVendorMethod
					== eap_sm->currentMethod))))
			{
				eap_sm->eap_state = EAP_INTEGRITY_CHECK;
			}
			else if ((eap_sm->rxResp == TRUE) && (eap_sm->respId
					== eap_sm->currentId) && ((eap_sm->respMethod == TYPE_NAK)
					|| (eap_sm->respMethod == TYPE_EXPANDED_TYPES
							&& eap_sm->respVendor == VENDOR_IETF
							&& eap_sm->respVendorMethod == TYPE_NAK))
					&& (eap_sm->methodState == EAP_M_PROPOSED))
			{
				eap_sm->eap_state = EAP_NAK;
			}
			else
			{
				eap_sm->eap_state = EAP_DISCARD;
			}
			break;
		case EAP_DISCARD:
			TRACE_DEBUG(INFO,"%s[EAP Protocol] Invalid EAP Packet received (Non fatal error).",DIAMEAP_EXTENSION)
			;
			*non_fatal_error = TRUE;
			eap_sm->eap_state = EAP_IDLE;
			break;

		case EAP_SEND_REQUEST:
			TRACE_DEBUG(FULL+1,"%s[EAP Protocol] New EAP packet request created.",DIAMEAP_EXTENSION)
			;
			diameap_eap_dump(FULL, &eap_i->aaaEapReqData);
			eap_i->aaaEapResp = FALSE;
			eap_i->aaaEapReq = TRUE;
			eap_sm->eap_state = EAP_IDLE;
			break;

		case EAP_INTEGRITY_CHECK:
			if ((*eap_sm->selectedMethod->eap_method_check)(eap_sm,
					&eap_i->aaaEapRespData) == FALSE)
			{
				TRACE_DEBUG(INFO,"%s[EAP Protocol] Invalid EAP packet received {Type=%d, Vendor=%d}. Integrity check failed (non fatal error).",DIAMEAP_EXTENSION,eap_sm->currentMethod,eap_sm->currentVendor);
				//non_fata_error
				*non_fatal_error = TRUE;
				eap_sm->eap_state = EAP_IDLE;
			}
			else
			{
				eap_sm->eap_state = EAP_METHOD_RESPONSE;
			}

			break;
		case EAP_METHOD_REQUEST:
			eap_sm->lastId = eap_sm->currentId;
			diameap_ba_nextid(eap_sm, &eap_sm->currentId);
			CHECK_FCT((*eap_sm->selectedMethod->eap_method_buildReq)(
							eap_sm, eap_sm->currentId,&eap_i->aaaEapReqData))
			;
			if (eap_sm->selectedMethod->eap_method_getTimeout)
			{
				if ((*eap_sm->selectedMethod->eap_method_getTimeout)(eap_sm,
						&eap_i->aaaMethodTimeout))
				{
					TRACE_DEBUG(INFO,"%s[EAP Protocol] [%s plugin] getTimeout failed.",DIAMEAP_EXTENSION,eap_sm->selectedMethod->methodname);
					eap_i->aaaMethodTimeout = 0;
				}
			}
			else
			{
				eap_i->aaaMethodTimeout = 0;
			}
			eap_sm->eap_state = EAP_SEND_REQUEST;
			break;

		case EAP_METHOD_RESPONSE:
			if (eap_sm->respMethod >= TYPE_EAP_MD5)
			{
				if (eap_sm->user.methodId < 0)
				{
					eap_sm->user.methodId = 0;
					eap_sm->user.methods[eap_sm->user.methodId].vendor
							= eap_sm->respVendor;
					eap_sm->user.methods[eap_sm->user.methodId].method
							= eap_sm->respMethod;
					eap_sm->user.methodId++;

				}
				else if (!((eap_sm->user.methods[eap_sm->user.methodId - 1].vendor
						== eap_sm->respVendor)
						&& (eap_sm->user.methods[eap_sm->user.methodId - 1].method
								== eap_sm->respMethod)))
				{
					eap_sm->user.methods[eap_sm->user.methodId].vendor
							= eap_sm->respVendor;
					eap_sm->user.methods[eap_sm->user.methodId].method
							= eap_sm->respMethod;
					eap_sm->user.methodId++;
				}
			}
			if ((*eap_sm->selectedMethod->eap_method_process)(eap_sm,
					&eap_i->aaaEapRespData))
			{
				TRACE_DEBUG(INFO,"%s[EAP Protocol] [%s plugin] Authentication process failed.",DIAMEAP_EXTENSION,eap_sm->selectedMethod->methodname);
				*non_fatal_error = TRUE;
				eap_sm->eap_state = EAP_IDLE;

			}else{
			if ((*eap_sm->selectedMethod->eap_method_isDone)(eap_sm) == TRUE)
			{
				/*diameap_ba_PolicyUpdate();*/
				eap_i->aaaEapMSKLength = 0;
				eap_i->aaaEapEMSKLength = 0;
				if (eap_sm->selectedMethod->eap_method_getKey)
				{
					if ((*eap_sm->selectedMethod->eap_method_getKey)(eap_sm,
							&eap_i->aaaEapMSKData, &eap_i->aaaEapMSKLength,
							&eap_i->aaaEapEMSKData, &eap_i->aaaEapEMSKLength))
					{
						TRACE_DEBUG(INFO,"%s[EAP Protocol] [%s plugin] Generating EAP Master Key failed.",DIAMEAP_EXTENSION,eap_sm->selectedMethod->methodname)
						eap_i->aaaEapMSKLength = 0;
						eap_i->aaaEapEMSKLength = 0;
						eap_i->aaaEapKeyAvailable = FALSE;
					}
					else
					{
						eap_i->aaaEapKeyAvailable = TRUE;
					}
				}
				eap_sm->methodState = EAP_M_END;
				eap_sm->eap_state = EAP_SELECT_ACTION;
			}
			else
			{
				eap_sm->methodState = EAP_M_CONTINUE;
				eap_sm->eap_state = EAP_METHOD_REQUEST;
			}
			}
			break;
		case EAP_PROPOSE_METHOD:
			if (diameap_ba_policygetnextmethod(eap_sm, &eap_sm->currentMethod,
					&eap_sm->currentVendor))
			{
				TRACE_DEBUG(INFO,"%s[EAP Protocol] [%s plugin] Selecting EAP Method plugin failed.",DIAMEAP_EXTENSION,eap_sm->selectedMethod->methodname);

				*non_fatal_error = TRUE;
				eap_sm->eap_state = EAP_END;

			}
			TRACE_DEBUG(FULL,"%s[EAP Protocol] Selecting EAP Method plugin: %s(%d). [user: %s]",DIAMEAP_EXTENSION,eap_sm->selectedMethod->methodname,eap_sm->selectedMethod->methodtype,eap_sm->user.userid)
			;
			if ((*eap_sm->selectedMethod->eap_method_init)(eap_sm))
			{
				TRACE_DEBUG(INFO,"%s[EAP Protocol] [%s plugin] Initializing EAP plugin failed.",DIAMEAP_EXTENSION,eap_sm->selectedMethod->methodname);
			}

			if ((eap_sm->currentMethod == TYPE_IDENTITY)
					|| (eap_sm->currentMethod == TYPE_NOTIFICATION))
			{
				eap_sm->methodState = EAP_M_CONTINUE;
			}
			else
			{
				eap_sm->methodState = EAP_M_PROPOSED;
			}
			eap_sm->eap_state = EAP_METHOD_REQUEST;
			break;
		case EAP_NAK:
			TRACE_DEBUG(FULL+1,"%s[EAP Protocol] EAP NAK Packet received.",DIAMEAP_EXTENSION)
			;
			if (eap_sm->selectedMethod->eap_method_free)
			{
				(*eap_sm->selectedMethod->eap_method_free)(eap_sm->methodData);
				eap_sm->methodData = NULL;
			}
			else
			{
				if (eap_sm->methodData)
				{
					free(eap_sm->methodData);
					eap_sm->methodData = NULL;
				}
			}
			diameap_ba_policyupdate(eap_sm, &eap_i->aaaEapRespData);
			eap_sm->eap_state = EAP_SELECT_ACTION;
			break;
		case EAP_SELECT_ACTION:
			CHECK_FCT(diameap_ba_policygetdecision(eap_sm, eap_i,&eap_sm->sm_decision))
			;
			switch (eap_sm->sm_decision)
			{
			case DECISION_CONTINUE:
				eap_sm->eap_state = EAP_PROPOSE_METHOD;
				break;
			case DECISION_FAILURE:
				TRACE_DEBUG(FULL,"%s[EAP Protocol] Authentication Failure [User: %s].",DIAMEAP_EXTENSION,eap_sm->user.userid)
				;
				eap_sm->lastId = eap_sm->currentId;
				diameap_ba_nextid(eap_sm, &eap_sm->currentId);
				CHECK_FCT(diameap_eap_new(EAP_FAILURE, (u8) eap_sm->currentId, TYPE_NONE, NULL, 0,&eap_i->aaaEapReqData))
				;
				eap_i->aaaFail = TRUE;
				*non_fatal_error = FALSE;
				eap_sm->eap_state = EAP_END;
				if (eap_sm->methodData)
				{
					if (eap_sm->selectedMethod->eap_method_free)
					{
						(*eap_sm->selectedMethod->eap_method_free)(
								eap_sm->methodData);
						eap_sm->methodData = NULL;
					}
					else
					{
						free(eap_sm->methodData);
						eap_sm->methodData = NULL;
					}
				}
				break;
			case DECISION_SUCCESS:
				TRACE_DEBUG(FULL,"%s[EAP Protocol] Authentication Success [User: %s].",DIAMEAP_EXTENSION,eap_sm->user.userid)
				;
				eap_sm->lastId = eap_sm->currentId;
				diameap_ba_nextid(eap_sm, &eap_sm->currentId);
				CHECK_FCT(diameap_eap_new(EAP_SUCCESS, (u8) eap_sm->currentId, TYPE_NONE, NULL, 0,&eap_i->aaaEapReqData))
				;
				if (eap_i->aaaEapMSKData != NULL)
				{
					TRACE_DEBUG(FULL+1,"%s[EAP Protocol] EAP Key available [User: %s].",DIAMEAP_EXTENSION,eap_sm->user.userid);
					eap_i->aaaEapKeyAvailable = TRUE;
				}
				else
				{
					TRACE_DEBUG(FULL+1,"%s[EAP Protocol] No EAP Key available [User: %s].",DIAMEAP_EXTENSION,eap_sm->user.userid);
				}
				eap_i->aaaSuccess = TRUE;
				*non_fatal_error = FALSE;
				eap_sm->eap_state = EAP_END;
				if (eap_sm->selectedMethod->eap_method_free)
				{
					(*eap_sm->selectedMethod->eap_method_free)(
							eap_sm->methodData);
					eap_sm->methodData = NULL;
				}
				else
				{
					if (eap_sm->methodData)
					{
						free(eap_sm->methodData);
						eap_sm->methodData = NULL;
					}
				}
				break;
			default:
				TRACE_DEBUG(INFO,"%sIncorrect EAP Decision.(Please report this problem.)",DIAMEAP_EXTENSION)
				;
			}
			break;

		case EAP_END:
			break;

		case EAP_IDLE:
			break;
		}
	}

	return 0;
}

