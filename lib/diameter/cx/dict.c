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


int ogs_dict_cx_entry(char *conffile)
{
  /* Applications section */
  {    
    /* Create the vendors */

  {
    struct dict_object * vendor;
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_VENDOR, VENDOR_BY_NAME, "3GPP", &vendor, ENOENT));
    struct dict_application_data cx = { 16777216, "Cx" };
    struct dict_application_data swx = { 16777265, "SWx" };
    CHECK_FCT(fd_dict_new(fd_g_config->cnf_dict, DICT_APPLICATION, &cx, vendor, NULL));
    CHECK_FCT(fd_dict_new(fd_g_config->cnf_dict, DICT_APPLICATION, &swx, vendor, NULL));
  }

  }

  /* Command section */

  {
    struct dict_object *cx;
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_APPLICATION, APPLICATION_BY_NAME, "Cx", &cx, ENOENT));

    /* User-Authorization-Request (UAR) Command */
    {
      struct dict_object* cmd_uar;
      struct dict_cmd_data data = {
        300,                                                    /* Code */
        "3GPP/User-Authorization-Request",                      /* Name */
        CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, /* Fixed flags */
        CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE                   /* Fixed flag values */
      };
      struct local_rules_definition rules[] =
      {
        {  {                      .avp_name = "Session-Id" }, RULE_FIXED_HEAD, -1, 1 },
        {  {                      .avp_name = "Vendor-Specific-Application-Id" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Auth-Session-State" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Origin-Host" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Origin-Realm" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Destination-Host" }, RULE_OPTIONAL, -1, 1 },
        {  {                      .avp_name = "Destination-Realm" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "User-Name" }, RULE_REQUIRED, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "Supported-Features" }, RULE_OPTIONAL, -1, -1 },
        {  { .avp_vendor = 10415, .avp_name = "Public-Identity" }, RULE_REQUIRED, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "Visited-Network-Identifier" }, RULE_REQUIRED, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "User-Authorization-Type" }, RULE_OPTIONAL, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "UAR-Flags" }, RULE_OPTIONAL, -1, 1 },
        {  {                      .avp_name = "Proxy-Info" }, RULE_OPTIONAL, -1, -1 },
        {  {                      .avp_name = "Route-Record" }, RULE_OPTIONAL, -1, -1 }
      };

      CHECK_dict_new(DICT_COMMAND, &data, cx, &cmd_uar);
      PARSE_loc_rules(rules, cmd_uar);
    }

    /* User-Authorization-Answer (UAA) Command */
    {
      struct dict_object* cmd_uaa;
      struct dict_cmd_data data = {
        300,                                                    /* Code */
        "3GPP/User-Authorization-Answer",                       /* Name */
        CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, /* Fixed flags */
        CMD_FLAG_PROXIABLE                                      /* Fixed flag values */
      };

      struct local_rules_definition rules[] =
      {
        {  {                      .avp_name = "Session-Id" }, RULE_FIXED_HEAD, -1, 1 },
        {  {                      .avp_name = "Vendor-Specific-Application-Id" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Result-Code" }, RULE_OPTIONAL, -1, 1 },
        {  {                      .avp_name = "Experimental-Result" }, RULE_OPTIONAL, -1, 1 },
        {  {                      .avp_name = "Auth-Session-State" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Origin-Host" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Origin-Realm" }, RULE_REQUIRED, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "Supported-Features" }, RULE_OPTIONAL, -1, -1 },
        {  { .avp_vendor = 10415, .avp_name = "Server-Name" }, RULE_OPTIONAL, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "Server-Capabilities" }, RULE_OPTIONAL, -1, 1 },
        {  {                      .avp_name = "Failed-AVP" }, RULE_OPTIONAL, -1, -1 },
        {  {                      .avp_name = "Proxy-Info" }, RULE_OPTIONAL, -1, -1 },
        {  {                      .avp_name = "Route-Record" }, RULE_OPTIONAL, -1, -1 }
      };

      CHECK_dict_new(DICT_COMMAND, &data, cx, &cmd_uaa);
      PARSE_loc_rules(rules, cmd_uaa);
    }

    /* Location-Info-Request (LIR) Command */
    {
      struct dict_object* cmd_lir;
      struct dict_cmd_data data = {
        302,                                                    /* Code */
        "3GPP/Location-Info-Request",                           /* Name */
        CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, /* Fixed flags */
        CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE                   /* Fixed flag values */
      };

      struct local_rules_definition rules[] =
      {
        {  {                      .avp_name = "Session-Id" }, RULE_FIXED_HEAD, -1, 1 },
        {  {                      .avp_name = "Vendor-Specific-Application-Id" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Auth-Session-State" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Origin-Host" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Origin-Realm" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Destination-Host" }, RULE_OPTIONAL, -1, 1 },
        {  {                      .avp_name = "Destination-Realm" }, RULE_REQUIRED, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "Originating-Request" }, RULE_OPTIONAL, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "Supported-Features" }, RULE_OPTIONAL, -1, -1 },
        {  { .avp_vendor = 10415, .avp_name = "Public-Identity" }, RULE_REQUIRED, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "User-Authorization-Type" }, RULE_OPTIONAL, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "Session-Priority" }, RULE_OPTIONAL, -1, 1 },
        {  {                      .avp_name = "Proxy-Info" }, RULE_OPTIONAL, -1, -1 },
        {  {                      .avp_name = "Route-Record" }, RULE_OPTIONAL, -1, -1 }
      };

      CHECK_dict_new(DICT_COMMAND, &data, cx, &cmd_lir);
      PARSE_loc_rules(rules, cmd_lir);
    }

    /* Location-Info-Answer (LIA) Command */
    {
      struct dict_object* cmd_lia;
      struct dict_cmd_data data = {
        302,                                                    /* Code */
        "3GPP/Location-Info-Answer",                            /* Name */
        CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, /* Fixed flags */
        CMD_FLAG_PROXIABLE                                      /* Fixed flag values */
      };

      struct local_rules_definition rules[] =
      {
        {  {                      .avp_name = "Session-Id" }, RULE_FIXED_HEAD, -1, 1 },
        {  {                      .avp_name = "Vendor-Specific-Application-Id" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Result-Code" }, RULE_OPTIONAL, -1, 1 },
        {  {                      .avp_name = "Experimental-Result" }, RULE_OPTIONAL, -1, 1 },
        {  {                      .avp_name = "Auth-Session-State" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Origin-Host" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Origin-Realm" }, RULE_REQUIRED, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "Supported-Features" }, RULE_OPTIONAL, -1, -1 },
        {  { .avp_vendor = 10415, .avp_name = "Server-Name" }, RULE_OPTIONAL, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "Server-Capabilities" }, RULE_OPTIONAL, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "Wildcarded-Public-Identity" }, RULE_OPTIONAL, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "LIA-Flags" }, RULE_OPTIONAL, -1, 1 },
        {  {                      .avp_name = "Failed-AVP" }, RULE_OPTIONAL, -1, -1 },
        {  {                      .avp_name = "Proxy-Info" }, RULE_OPTIONAL, -1, -1 },
        {  {                      .avp_name = "Route-Record" }, RULE_OPTIONAL, -1, -1 }
      };

      CHECK_dict_new(DICT_COMMAND, &data, cx, &cmd_lia);
      PARSE_loc_rules(rules, cmd_lia);
    }

    /* Multimedia-Auth-Request (MAR) Command */
    {
      struct dict_object* cmd_mar;
      struct dict_cmd_data data = {
        303,                                                    /* Code */
        "3GPP/Multimedia-Auth-Request",                         /* Name */
        CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, /* Fixed flags */
        CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE                   /* Fixed flag values */
      };

      struct local_rules_definition rules[] =
      {
        {  {                      .avp_name = "Session-Id" }, RULE_FIXED_HEAD, -1, 1 },
        {  {                      .avp_name = "Vendor-Specific-Application-Id" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Auth-Session-State" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Origin-Host" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Origin-Realm" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Destination-Realm" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Destination-Host" }, RULE_OPTIONAL, -1, 1 },
        {  {                      .avp_name = "User-Name" }, RULE_REQUIRED, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "Supported-Features" }, RULE_OPTIONAL, -1, -1 },
#if 0 /* modified by acetcom */
        {  { .avp_vendor = 10415, .avp_name = "Public-Identity" }, RULE_REQUIRED, -1, 1 },
#else
        {  { .avp_vendor = 10415, .avp_name = "Public-Identity" }, RULE_OPTIONAL, -1, 1 },
#endif
        {  { .avp_vendor = 10415, .avp_name = "SIP-Auth-Data-Item" }, RULE_REQUIRED, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "SIP-Number-Auth-Items" }, RULE_REQUIRED, -1, 1 },
#if 0 /* modified by acetcom */
        {  { .avp_vendor = 10415, .avp_name = "Server-Name" }, RULE_REQUIRED, -1, 1 },
#else
        {  { .avp_vendor = 10415, .avp_name = "Server-Name" }, RULE_OPTIONAL, -1, 1 },
#endif
        {  {                      .avp_name = "Proxy-Info" }, RULE_OPTIONAL, -1, -1 },
        {  {                      .avp_name = "Route-Record" }, RULE_OPTIONAL, -1, -1 },
      };

      CHECK_dict_new(DICT_COMMAND, &data, cx, &cmd_mar);
      PARSE_loc_rules(rules, cmd_mar);
    }

    /* Multimedia-Auth-Answer (MAA) Command */
    {
      struct dict_object* cmd_maa;
      struct dict_cmd_data data = {
        303,                                                    /* Code */
        "3GPP/Multimedia-Auth-Answer",                          /* Name */
        CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, /* Fixed flags */
        CMD_FLAG_PROXIABLE                                      /* Fixed flag values */
      };

      struct local_rules_definition rules[] =
      {
        {  {                      .avp_name = "Session-Id" }, RULE_FIXED_HEAD, -1, 1 },
        {  {                      .avp_name = "Vendor-Specific-Application-Id" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Result-Code" }, RULE_OPTIONAL, -1, 1 },
        {  {                      .avp_name = "Experimental-Result" }, RULE_OPTIONAL, -1, 1 },
        {  {                      .avp_name = "Auth-Session-State" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Origin-Host" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Origin-Realm" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "User-Name" }, RULE_OPTIONAL, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "Supported-Features" }, RULE_OPTIONAL, -1, -1 },
        {  { .avp_vendor = 10415, .avp_name = "Public-Identity" }, RULE_OPTIONAL, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "SIP-Number-Auth-Items" }, RULE_OPTIONAL, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "SIP-Auth-Data-Item" }, RULE_OPTIONAL, -1, 1 },
        {  {                      .avp_name = "Failed-AVP" }, RULE_OPTIONAL, -1, -1 },
        {  {                      .avp_name = "Proxy-Info" }, RULE_OPTIONAL, -1, -1 },
        {  {                      .avp_name = "Route-Record" }, RULE_OPTIONAL, -1, -1 }
      };

      CHECK_dict_new(DICT_COMMAND, &data, cx, &cmd_maa);
      PARSE_loc_rules(rules, cmd_maa);
    }

    /* Server-Assignment-Request (SAR) Command */
    {
      struct dict_object* cmd_sar;
      struct dict_cmd_data data = {
        301,                                                    /* Code */
        "3GPP/Server-Assignment-Request",                       /* Name */
        CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, /* Fixed flags */
        CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE                   /* Fixed flag values */
      };

      struct local_rules_definition rules[] =
      {
        {  {                      .avp_name = "Session-Id" }, RULE_FIXED_HEAD, -1, 1 },
        {  {                      .avp_name = "Vendor-Specific-Application-Id" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Auth-Session-State" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Origin-Host" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Origin-Realm" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Destination-Host" }, RULE_OPTIONAL, -1, 1 },
        {  {                      .avp_name = "Destination-Realm" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "User-Name" }, RULE_OPTIONAL, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "Supported-Features" }, RULE_OPTIONAL, -1, -1 },
        {  { .avp_vendor = 10415, .avp_name = "Public-Identity" }, RULE_OPTIONAL, -1, -1 },
        {  { .avp_vendor = 10415, .avp_name = "Wildcarded-Public-Identity" }, RULE_OPTIONAL, -1, 1 },
#if 0 /* modified by acetcom */
        {  { .avp_vendor = 10415, .avp_name = "Server-Name" }, RULE_REQUIRED, -1, 1 },
#else
        {  { .avp_vendor = 10415, .avp_name = "Server-Name" }, RULE_OPTIONAL, -1, 1 },
#endif
        {  { .avp_vendor = 10415, .avp_name = "Server-Assignment-Type" }, RULE_REQUIRED, -1, 1 },
#if 0 /* modified by acetcom */
        {  { .avp_vendor = 10415, .avp_name = "User-Data-Already-Available" }, RULE_REQUIRED, -1, 1 },
#else
        {  { .avp_vendor = 10415, .avp_name = "User-Data-Already-Available" }, RULE_OPTIONAL, -1, 1 },
#endif
        {  { .avp_vendor = 10415, .avp_name = "SCSCF-Restoration-Info" }, RULE_OPTIONAL, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "Multiple-Registration-Indication" }, RULE_OPTIONAL, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "Session-Priority" }, RULE_OPTIONAL, -1, 1 },
        {  {                      .avp_name = "Proxy-Info" }, RULE_OPTIONAL, -1, -1 },
        {  {                      .avp_name = "Route-Record" }, RULE_OPTIONAL, -1, -1 }
      };

      CHECK_dict_new(DICT_COMMAND, &data, cx, &cmd_sar);
      PARSE_loc_rules(rules, cmd_sar);
    }

    /* Server-Assignment-Answer (SAA) Command */
    {
      struct dict_object* cmd_saa;
      struct dict_cmd_data data = {
        301,                                                    /* Code */
        "3GPP/Server-Assignment-Answer",                        /* Name */
        CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, /* Fixed flags */
        CMD_FLAG_PROXIABLE                                      /* Fixed flag values */
      };

      struct local_rules_definition rules[] =
      {
        {  {                      .avp_name = "Session-Id" }, RULE_FIXED_HEAD, -1, 1 },
        {  {                      .avp_name = "Vendor-Specific-Application-Id" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Result-Code" }, RULE_OPTIONAL, -1, 1 },
        {  {                      .avp_name = "Experimental-Result" }, RULE_OPTIONAL, -1, 1 },
        {  {                      .avp_name = "Auth-Session-State" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Origin-Host" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Origin-Realm" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "User-Name" }, RULE_OPTIONAL, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "Supported-Features" }, RULE_OPTIONAL, -1, -1 },
        {  { .avp_vendor = 10415, .avp_name = "User-Data" }, RULE_OPTIONAL, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "Charging-Information" }, RULE_OPTIONAL, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "Associated-Identities" }, RULE_OPTIONAL, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "Loose-Route-Indication" }, RULE_OPTIONAL, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "SCSCF-Restoration-Info" }, RULE_OPTIONAL, -1, -1 },
        {  { .avp_vendor = 10415, .avp_name = "Associated-Registered-Identities" }, RULE_OPTIONAL, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "Server-Name" }, RULE_OPTIONAL, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "Wildcarded-Public-Identity" }, RULE_OPTIONAL, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "Priviledged-Sender-Indication" }, RULE_OPTIONAL, -1, 1 },
        {  {                      .avp_name = "Failed-AVP" }, RULE_OPTIONAL, -1, -1 },
        {  {                      .avp_name = "Proxy-Info" }, RULE_OPTIONAL, -1, -1 },
        {  {                      .avp_name = "Route-Record" }, RULE_OPTIONAL, -1, -1 }
      };

      CHECK_dict_new(DICT_COMMAND, &data, cx, &cmd_saa);
      PARSE_loc_rules(rules, cmd_saa);
    }

    /* Registration-Termination-Request (RTR) Command */
    {
      struct dict_object* cmd_rtr;
      struct dict_cmd_data data = {
        304,                                                    /* Code */
        "3GPP/Registration-Termination-Request",                /* Name */
        CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, /* Fixed flags */
        CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE                   /* Fixed flag values */
      };

      struct local_rules_definition rules[] =
      {
        {  {                      .avp_name = "Session-Id" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Vendor-Specific-Application-Id" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Auth-Session-State" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Origin-Host" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Origin-Realm" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Destination-Host" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Destination-Realm" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "User-Name" }, RULE_REQUIRED, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "Associated-Identities" }, RULE_OPTIONAL, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "Supported-Features" }, RULE_OPTIONAL, -1, -1 },
        {  { .avp_vendor = 10415, .avp_name = "Public-Identity" }, RULE_OPTIONAL, -1, -1 },
        {  { .avp_vendor = 10415, .avp_name = "Deregistration-Reason" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Proxy-Info" }, RULE_OPTIONAL, -1, -1 },
        {  {                      .avp_name = "Route-Record" }, RULE_OPTIONAL, -1, -1 }
      };

      CHECK_dict_new(DICT_COMMAND, &data, cx, &cmd_rtr);
      PARSE_loc_rules(rules, cmd_rtr);
    }

    /* Registration-Termination-Answer (RTA) Command */
    {
      struct dict_object* cmd_rta;
      struct dict_cmd_data data = {
        304,                                                    /* Code */
        "3GPP/Registration-Termination-Answer",                 /* Name */
        CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, /* Fixed flags */
        CMD_FLAG_PROXIABLE                                      /* Fixed flag values */
      };

      struct local_rules_definition rules[] =
      {
        {  {                      .avp_name = "Session-Id" }, RULE_FIXED_HEAD, -1, 1 },
        {  {                      .avp_name = "Vendor-Specific-Application-Id" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Result-Code" }, RULE_OPTIONAL, -1, 1 },
        {  {                      .avp_name = "Experimental-Result" }, RULE_OPTIONAL, -1, 1 },
        {  {                      .avp_name = "Auth-Session-State" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Origin-Host" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Origin-Realm" }, RULE_REQUIRED, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "Associated-Identities" }, RULE_OPTIONAL, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "Supported-Features" }, RULE_OPTIONAL, -1, -1 },
        {  { .avp_vendor = 10415, .avp_name = "Identity-with-Emergency-Registration" }, RULE_OPTIONAL, -1, -1 },
        {  {                      .avp_name = "Failed-AVP" }, RULE_OPTIONAL, -1, -1 },
        {  {                      .avp_name = "Proxy-Info" }, RULE_OPTIONAL, -1, -1 },
        {  {                      .avp_name = "Route-Record" }, RULE_OPTIONAL, -1, -1 }
      };

      CHECK_dict_new(DICT_COMMAND, &data, cx, &cmd_rta);
      PARSE_loc_rules(rules, cmd_rta);
    }

    /* Push-Profile-Request (PPR) Command */
    {
      struct dict_object* cmd_ppr;
      struct dict_cmd_data data = {
        305,                                                    /* Code */
        "3GPP/Push-Profile-Request",                            /* Name */
        CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, /* Fixed flags */
        CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE                   /* Fixed flag values */
      };

      struct local_rules_definition rules[] =
      {
        {  {                      .avp_name = "Session-Id" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Vendor-Specific-Application-Id" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Auth-Session-State" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Origin-Host" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Origin-Realm" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Destination-Host" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Destination-Realm" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "User-Name" }, RULE_REQUIRED, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "Supported-Features" }, RULE_OPTIONAL, -1, -1 },
        {  { .avp_vendor = 10415, .avp_name = "User-Data" }, RULE_OPTIONAL, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "Charging-Information" }, RULE_OPTIONAL, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "SIP-Auth-Data-Item" }, RULE_OPTIONAL, -1, 1 },
        {  {                      .avp_name = "Proxy-Info" }, RULE_OPTIONAL, -1, -1 },
        {  {                      .avp_name = "Route-Record" }, RULE_OPTIONAL, -1, -1 }
      };

      CHECK_dict_new(DICT_COMMAND, &data, cx, &cmd_ppr);
      PARSE_loc_rules(rules, cmd_ppr);
    }

    /* Push-Profile-Answer (PPA) Command */
    {
      struct dict_object* cmd_ppa;
      struct dict_cmd_data data = {
        305,                                                    /* Code */
        "3GPP/Push-Profile-Answer",                             /* Name */
        CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, /* Fixed flags */
        CMD_FLAG_PROXIABLE                                      /* Fixed flag values */
      };

      struct local_rules_definition rules[] =
      {
        {  {                      .avp_name = "Session-Id" }, RULE_FIXED_HEAD, -1, 1 },
        {  {                      .avp_name = "Vendor-Specific-Application-Id" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Result-Code" }, RULE_OPTIONAL, -1, 1 },
        {  {                      .avp_name = "Experimental-Result" }, RULE_OPTIONAL, -1, 1 },
        {  {                      .avp_name = "Auth-Session-State" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Origin-Host" }, RULE_REQUIRED, -1, 1 },
        {  {                      .avp_name = "Origin-Realm" }, RULE_REQUIRED, -1, 1 },
        {  { .avp_vendor = 10415, .avp_name = "Supported-Features" }, RULE_OPTIONAL, -1, -1 },
        {  {                      .avp_name = "Failed-AVP" }, RULE_OPTIONAL, -1, -1 },
        {  {                      .avp_name = "Proxy-Info" }, RULE_OPTIONAL, -1, -1 },
        {  {                      .avp_name = "Route-Record" }, RULE_OPTIONAL, -1, -1 }
      };

      CHECK_dict_new(DICT_COMMAND, &data, cx, &cmd_ppa);
      PARSE_loc_rules(rules, cmd_ppa);
    }
  }

  LOG_D( "Extension 'Dictionary definitions for DCCA 3GPP' initialized");
  return 0;
}

#if 0 /* modified by acetcom */
EXTENSION_ENTRY("dict_cx", ogs_dict_cx_entry, "dict_dcca_3gpp");
#endif
