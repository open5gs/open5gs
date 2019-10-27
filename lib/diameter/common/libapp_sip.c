/*********************************************************************************************************
* Software License Agreement (BSD License)                                                               *
* Author: Alexandre Westfahl <awestfahl@freediameter.net>						 *
*													 *
* Copyright (c) 2010, Alexandre Westfahl, Teraoka Laboratory (Keio University), and the WIDE Project. 	 *		
*													 *
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
* * Neither the name of the Teraoka Laboratory nor the 							 *
*   names of its contributors may be used to endorse or 						 *
*   promote products derived from this software without 						 *
*   specific prior written permission of Teraoka Laboratory 						 *
*   													 *
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

#include "ogs-diameter-common.h"

/* Search a given AVP model in an AVP (extracted from libfreediameter/message.c ) */
int fd_avp_search_avp ( struct avp * groupedavp, struct dict_object * what, struct avp ** avp )
{
	struct avp * nextavp;
	struct avp_hdr * nextavphdr;
	struct dict_avp_data 	dictdata;
	
	
	TRACE_ENTRY("%p %p %p", groupedavp, what, avp);
	
	CHECK_FCT(  fd_dict_getval(what, &dictdata)  );
	
	// Loop only in the group AVP 
	CHECK_FCT(  fd_msg_browse(groupedavp, MSG_BRW_FIRST_CHILD, (void *)&nextavp, NULL)  );
	CHECK_FCT( fd_msg_avp_hdr( nextavp, &nextavphdr )  );
	
	while (nextavphdr) {
		
		if ( (nextavphdr->avp_code   == dictdata.avp_code) && (nextavphdr->avp_vendor == dictdata.avp_vendor) ) // always 0 if no Vendor flag
		{
			break;
		}
		
		// Otherwise move to next AVP in the grouped AVP 
		CHECK_FCT( fd_msg_browse(nextavp, MSG_BRW_NEXT, (void *)&nextavp, NULL) );
		
		if(nextavp!=NULL)
		{
			CHECK_FCT( fd_msg_avp_hdr( nextavp, &nextavphdr )  );
		}
		else
			nextavphdr=NULL;
	}
	if (avp)
		*avp = nextavp;
	
	if (avp && nextavp) {
		struct dictionary * dict;
		CHECK_FCT( fd_dict_getdict( what, &dict) );
		CHECK_FCT_DO( fd_msg_parse_dict( nextavp, dict, NULL ),  );
	}
	
	if (avp || nextavp)
		return 0;
	else
		return ENOENT;
}
