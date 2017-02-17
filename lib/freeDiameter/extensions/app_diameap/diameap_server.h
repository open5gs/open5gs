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


#ifndef DIAMEAP_SERVER_H_
#define DIAMEAP_SERVER_H_

 
/* session data structure to store */
struct sess_state
{
	int invalid_eappackets; /* Number of invalid EAP Packet received*/

	eap_type currentMethod;
	u32 currentVendor;
	int currentId;
	int lastId;
	void * methodData;

	u8 NAKproposedMethods[251];

	eap_method_state methodState;

	struct eap_user user;
};

typedef enum
{
	AUTHENTICATE_ONLY = 1, AUTHORIZE_ONLY = 2, AUTHORIZE_AUTHENTICATE = 3
} auth_request;

struct diameap_state_machine
{
	int invalid_eappackets; /* Number of invalid EAP Packet received*/
	struct avp * lastReqEAPavp; //last EAP-Payload AVP

	int result_code; /*Error number for Result_code*/
	struct fd_list attributes; //database attributes
	struct fd_list req_attributes; //attributes from DER
	struct fd_list ans_attributes; //attributes to be set for DEA
	struct avp * failedavp; /* The Failed-AVP AVP. should be update whenever a Failed AVP is encountered during authentication. */
	struct eap_state_machine eap_sm; /* EAP State Machine */
	auth_request auth_request_val; /*the Request Type of Auth-Request-Type AVP*/
	boolean verify_authorization; /* Set to TRUE at the authorization state. Parameter used to indicate that authorization is performed.*/
	boolean authSuccess; // Set to TRUE if client authenticated and authorized
	boolean authFailure; //set to TRUE if client is not authenticated
	boolean authorized; //set to TRUE if client is authorized
	enum
	{
		DIAMEAP_DISABLED,
		DIAMEAP_INITIALIZE,
		DIAMEAP_RECEIVED,
		DIAMEAP_IDLE,
		DIAMEAP_AUTHENTICATION_VERIFY,
		DIAMEAP_SEND_ERROR_MSG,
		DIAMEAP_SELECT_DECISION,
		DIAMEAP_DIAMETER_EAP_ANSWER,
		DIAMEAP_END,
		DIAMEAP_AUTHORIZATION_VERIFY,
		DIAMEAP_SEND_REQUEST,
		DIAMEAP_SEND_SUCCESS,
		DIAMEAP_SEND_FAILURE

	} state; // state of DiamEAP

	boolean privateUser;//TD
};

struct avp_max_occurences
{
	char * avp_attribute;
	int max; //-1 means no limits
};



/* start server */
int diameap_start_server(void);

/* stop server*/
int diameap_stop_server(void);

#endif /* DIAMEAP_SERVER_H_ */
