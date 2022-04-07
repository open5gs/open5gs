/*********************************************************************************************************
 * Software License Agreement (BSD License)                                                               *
 * Author: Thomas Klausner <tk@giga.or.at>                                                                *
 *                                                                                                        *
 * Copyright (c) 2013, Thomas Klausner                                                                    *
 * All rights reserved.                                                                                   *
 *                                                                                                        *
 * Written under contract by nfotex IT GmbH, http://nfotex.com/                                           *
 *                                                                                                        *
 * Redistribution and use of this software in source and binary forms, with or without modification, are  *
 * permitted provided that the following conditions are met:                                              *
 *                                                                                                        *
 * * Redistributions of source code must retain the above                                                 *
 *   copyright notice, this list of conditions and the                                                    *
 *   following disclaimer.                                                                                *
 *                                                                                                        *
 * * Redistributions in binary form must reproduce the above                                              *
 *   copyright notice, this list of conditions and the                                                    *
 *   following disclaimer in the documentation and/or other                                               *
 *   materials provided with the distribution.                                                            *
 *                                                                                                        *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED *
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A *
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR *
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT     *
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS    *
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR *
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF   *
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                             *
 *********************************************************************************************************/

/*
 * Dictionary definitions for objects specified for DCCA by 3GPP.
 *
 * This extensions contains a lot of AVPs from various 3GPP standards
 * documents, and some rules for the grouped AVPs described therein.
 *
 * This extension does not contain ALL AVPs described by 3GPP, but
 * quite a big number of them.
 *
 * When extending the AVPs, please edit dict_rx.org instead and
 * create pastable code with contrib/tools/org_to_fd.pl.
 *
 * Some points of consideration:
 * 1. This dictionary could be split up per document.
 *
 * + pro: you can only load the AVPs/Rules you're interested in ->
 * smaller memory size
 *
 * - con: the documents use AVPs from each other A LOT, so setting the
 * dependencies correctly will be annoying
 *
 * - con: you need to load all of them as extensions
 *
 * 2. This dictionary contains ONE AVP in the "3GPP2" vendor space,
 * since I found it wasteful to write a separate dictionary just for
 * one AVP. Also, it is defined in a 3GPP document.
 *
 * 3. While there are quite a number of rules here already, many more
 * are missing. I've only added rules for those grouped AVPs or
 * commands in which I was concretely interested so far; many more
 * will need to be added to make this complete.
 *
 * That being said, I hope this will be useful for you.
 *
 */


/*
 * Some comments on the 3GPP Standards documents themselves:
 *
 * 1. It would be good if 29.061 was reviewed to check for each AVP if
 * it is Mandatory or not. The data currently in the document does not
 * match what was in the previous version of the freeDiameter
 * extension (the one that existedbefore I rewrote it) or what I saw
 * so far. IIRC, even the table and the document contradict each
 * other. The AVP table is also missing an entry for
 * "External-Identifier", 28.
 *
 * 2. 29.140 has conflicting AVP names with other documents:
 *   - Sequence-Number is also in 32.329
 *   - Recipient-Address is also in 32.299
 *   - Status is also in 32.299
 *
 * 3. 29.229 has name conflict with 29.329 about User-Data (different
 * AVP code 702, instead of 606) -- the weird thing is, the latter
 * uses some AVPs from the former, but not this one.
*/
#include <freeDiameter/extension.h>
#include "ogs-diameter-gy.h"


/* The content of this file follows the same structure as dict_base_proto.c */

#define CHECK_dict_new( _type, _data, _parent, _ref )  \
  CHECK_FCT(  fd_dict_new( fd_g_config->cnf_dict, (_type), (_data), (_parent), (_ref))  );

#define CHECK_dict_search( _type, _criteria, _what, _result )  \
  CHECK_FCT(  fd_dict_search( fd_g_config->cnf_dict, (_type), (_criteria), (_what), (_result), ENOENT) );

struct local_rules_definition {
  struct dict_avp_request avp_vendor_plus_name;
  enum rule_position  position;
  int       min;
  int      max;
};

#define RULE_ORDER( _position ) ((((_position) == RULE_FIXED_HEAD) || ((_position) == RULE_FIXED_TAIL)) ? 1 : 0 )

/* Attention! This version of the macro uses AVP_BY_NAME_AND_VENDOR, in contrast to most other copies! */
#define PARSE_loc_rules( _rulearray, _parent) {                \
  int __ar;                      \
  for (__ar=0; __ar < sizeof(_rulearray) / sizeof((_rulearray)[0]); __ar++) {      \
    struct dict_rule_data __data = { NULL,               \
      (_rulearray)[__ar].position,              \
      0,                     \
      (_rulearray)[__ar].min,                \
      (_rulearray)[__ar].max};              \
    __data.rule_order = RULE_ORDER(__data.rule_position);          \
    CHECK_FCT(  fd_dict_search(                 \
      fd_g_config->cnf_dict,                \
      DICT_AVP,                   \
      AVP_BY_NAME_AND_VENDOR,               \
      &(_rulearray)[__ar].avp_vendor_plus_name,          \
      &__data.rule_avp, 0 ) );              \
    if ( !__data.rule_avp ) {                \
      TRACE_DEBUG(INFO, "AVP Not found: '%s'", (_rulearray)[__ar].avp_vendor_plus_name.avp_name);    \
      return ENOENT;                  \
    }                      \
    CHECK_FCT_DO( fd_dict_new( fd_g_config->cnf_dict, DICT_RULE, &__data, _parent, NULL),  \
      {                          \
        TRACE_DEBUG(INFO, "Error on rule with AVP '%s'",            \
              (_rulearray)[__ar].avp_vendor_plus_name.avp_name);    \
        return EINVAL;                      \
      } );                          \
  }                              \
}

#define enumval_def_u32( _val_, _str_ ) \
    { _str_,     { .u32 = _val_ }}

#define enumval_def_os( _len_, _val_, _str_ ) \
    { _str_,     { .os = { .data = (unsigned char *)_val_, .len = _len_ }}}


int ogs_dict_gy_entry(char *conffile)
{
  /* Applications section */
  {
  {
    //struct dict_object * vendor;
    //CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_VENDOR, VENDOR_BY_NAME, "3GPP", &vendor, ENOENT));
    struct dict_application_data app_data = { OGS_DIAM_GY_APPLICATION_ID, "Gy" };
    CHECK_FCT(fd_dict_new(fd_g_config->cnf_dict, DICT_APPLICATION, &app_data, NULL, NULL));
  }

  }

  /* Credit-Control-Request (CCR) Command - Extension for Gy (3GPP TS 32.299 sec 6.4.2) */
  {
    struct dict_object * cmd;
    struct local_rules_definition rules[] =
    {
      {  {                      .avp_name = "Session-Id" }, RULE_FIXED_HEAD, -1, 1 },
      {  {                      .avp_name = "Origin-Host" }, RULE_REQUIRED, -1, 1 },
      {  {                      .avp_name = "Origin-Realm" }, RULE_REQUIRED, -1, 1 },
      {  {                      .avp_name = "Destination-Realm" }, RULE_REQUIRED, -1, 1 },
      {  {                      .avp_name = "Auth-Application-Id" }, RULE_REQUIRED, -1, 1 },
      //{  {                      .avp_name = "Service-Context-Id" }, RULE_REQUIRED, -1, 1 }, /* freeDiameter: Conflicting rule position */
      {  {                      .avp_name = "CC-Request-Type" }, RULE_REQUIRED, -1, 1 },
      {  {                      .avp_name = "CC-Request-Number" }, RULE_REQUIRED, -1, 1 },
      {  {                      .avp_name = "Destination-Host" }, RULE_OPTIONAL, -1, 1 },
      {  {                      .avp_name = "User-Name" }, RULE_OPTIONAL, -1, 1 },
      {  {                      .avp_name = "Origin-State-Id" }, RULE_OPTIONAL, -1, 1 },
      {  {                      .avp_name = "Event-Timestamp" }, RULE_OPTIONAL, -1, 1 },
      {  {                      .avp_name = "Subscription-Id" }, RULE_OPTIONAL, -1, -1 },
      {  {                      .avp_name = "Termination-Cause" }, RULE_OPTIONAL, -1, 1 },
      {  {                      .avp_name = "Requested-Action" }, RULE_OPTIONAL, -1, 1 },
      {  { .avp_vendor = 10415, .avp_name = "AoC-Request-Type" }, RULE_OPTIONAL, -1, 1 },
      {  {                      .avp_name = "Multiple-Services-Indicator" }, RULE_OPTIONAL, -1, 1 },
      {  {                      .avp_name = "Multiple-Services-Credit-Control" }, RULE_OPTIONAL, -1, -1 },
      {  { .avp_vendor = 10415, .avp_name = "3GPP-RAT-Type" }, RULE_OPTIONAL, -1, 1 },
      {  {                      .avp_name = "CC-Correlation-Id" }, RULE_OPTIONAL, -1, 1 },
      {  {                      .avp_name = "User-Equipment-Info" }, RULE_OPTIONAL, -1, 1 },
      //{  {                      .avp_name = "OC-Supported-Features" }, RULE_OPTIONAL, -1, 1 }, /*freeDiameter: AVP Not found: 'OC-Supported-Features' */
      {  {                      .avp_name = "Proxy-Info" }, RULE_OPTIONAL, -1, -1 },
      {  {                      .avp_name = "Route-Record" }, RULE_OPTIONAL, -1, -1 },
      {  { .avp_vendor = 10415, .avp_name = "Service-Information" }, RULE_OPTIONAL, -1, 1 },
       /* plus any additional AVPs { "AVP", RULE_OPTIONAL, -1, -1 } */
    };

    CHECK_dict_search( DICT_COMMAND, CMD_BY_NAME, "Credit-Control-Request", &cmd);
    PARSE_loc_rules( rules, cmd );
  }

  /* Credit-Control-Answer (CCA) Command - Extension for Gy (3GPP TS 32.299 sec 6.4.3) */
  {
    struct dict_object * cmd;
    struct local_rules_definition rules[] =
    {
      {  {                      .avp_name = "Session-Id" }, RULE_FIXED_HEAD, -1, 1 },
      {  {                      .avp_name = "Result-Code" }, RULE_REQUIRED, -1, 1 },
     // {  {                      .avp_name = "Experimental-Result" }, RULE_OPTIONAL, -1, 1 },
      {  {                      .avp_name = "Origin-Host" }, RULE_REQUIRED, -1, 1 },
      {  {                      .avp_name = "Origin-Realm" }, RULE_REQUIRED, -1, 1 },
      {  {                      .avp_name = "Auth-Application-Id" }, RULE_REQUIRED, -1, 1 },
      {  {                      .avp_name = "CC-Request-Type" }, RULE_REQUIRED, -1, 1 },
      {  {                      .avp_name = "CC-Request-Number" }, RULE_REQUIRED, -1, 1 },
      {  {                      .avp_name = "CC-Session-Failover" }, RULE_OPTIONAL, -1, 1 },
      {  {                      .avp_name = "Multiple-Services-Credit-Control" }, RULE_OPTIONAL, -1, -1 },
      {  {                      .avp_name = "Cost-Information" }, RULE_OPTIONAL, -1, 1 },
      {  { .avp_vendor = 10415, .avp_name = "Low-Balance-Indication" }, RULE_OPTIONAL, -1, 1 },
      {  { .avp_vendor = 10415, .avp_name = "Remaining-Balance" }, RULE_OPTIONAL, -1, 1 },
      {  {                      .avp_name = "Credit-Control-Failure-Handling" }, RULE_OPTIONAL, -1, 1 },
      {  {                      .avp_name = "Direct-Debiting-Failure-Handling" }, RULE_OPTIONAL, -1, 1 },
      //{  {                      .avp_name = "OC-Supported-Features" }, RULE_OPTIONAL, -1, 1 },  /*freeDiameter: AVP Not found: 'OC-Supported-Features' */
      //{  {                      .avp_name = "OC-OLR" }, RULE_OPTIONAL, -1, 1 }, /*freeDiameter: AVP Not found: 'OC-OLR' */
      {  {                      .avp_name = "Redirect-Host" }, RULE_OPTIONAL, -1, -1 },
      {  {                      .avp_name = "Redirect-Host-Usage" }, RULE_OPTIONAL, -1, 1 },
      {  {                      .avp_name = "Redirect-Max-Cache-Time" }, RULE_OPTIONAL, -1, 1 },
      {  {                      .avp_name = "Proxy-Info" }, RULE_OPTIONAL, -1, -1 },
      {  {                      .avp_name = "Route-Record" }, RULE_OPTIONAL, -1, -1 },
      {  {                      .avp_name = "Failed-AVP" }, RULE_OPTIONAL, -1, -1 }, /* 3GPP TS 32.299 6.4.3 says max=1 but RFC4006 & freeDiameter says max=-1 */
      {  { .avp_vendor = 10415, .avp_name = "Service-Information" }, RULE_OPTIONAL, -1, 1 },
       /* plus any additional AVPs { "AVP", RULE_OPTIONAL, -1, -1 } */
    };

    CHECK_dict_search( DICT_COMMAND, CMD_BY_NAME, "Credit-Control-Answer", &cmd);
    PARSE_loc_rules( rules, cmd );
  }

  LOG_D( "Extension 'Dictionary definitions for 3GPP Gy' initialized");
  return 0;
}

#if 0 /* modified by acetcom */
EXTENSION_ENTRY("dict_gy", ogs_dict_gy_entry, "dict_dcca_3gpp");
#endif
