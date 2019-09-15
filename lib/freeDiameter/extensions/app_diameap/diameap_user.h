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


#ifndef DIAMEAP_USER_H_
#define DIAMEAP_USER_H_

#define MAXMETHODS 8 	/*Maximum number of allowed methods */

#define MAXPROPOSEDMETHODS 8 	/*Maximum number of allowed proposed methods */

#define	VENDOR_IETF 0

/*EAP User structure */
struct eap_user
{
	int id;	/*user's identifier*/
	u8 	*userid;	/*user's identity*/
	u16	useridLength;	/*User's identity length*/
	u8 	*password; /*user's password*/
	u16 passwordLength;	/*Length of password*/

	int methodId;	/*Current method Id*/
	struct eap_method
	{
		u32 vendor;
		eap_type method;
	} methods[MAXMETHODS]; /*used method for authentication*/

	struct proposed_eap_method
	{
		u32 vendor;
		eap_type method;
	} proposedmethods[MAXPROPOSEDMETHODS]; /* methods proposed throw NAK response or EXPANDED NAK*/
	int pmethods; /*Number of accepted methods from peer's proposed methods*/

	eap_type proposed_eap_method; /* Proposed EAP Method*/
	u32 proposed_eap_method_vendor;
	boolean success; /* Set to TRUE if User is authenticated successfully */
};

boolean check_user_identity;

int diameap_user_get_password(struct eap_user *user, u8 * password,u16 *length);

int diameap_user_get_userid(struct eap_user *user, u8 * userid);

int diameap_user_set_password(struct eap_user * user, u8 * password, u16 Length);

int diameap_user_set_userid(struct eap_user * user, u8 * userid, u16 Length);

int diameap_user_get_methodid(struct eap_user *user,int *methodId);

int diameap_user_set_methodid(struct eap_user * user, int methodId);

boolean diameap_user_issuccess(struct eap_user *user);

int diameap_user_set_success(struct eap_user * user);

int diameap_user_get_eap_method(struct eap_user *user, int id, struct eap_method * eapmethod);

int diameap_user_set_eap_method(struct eap_user * user, int id, struct eap_method * method);

#endif /* DIAMEAP_USER_H_ */
