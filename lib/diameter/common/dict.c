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


int ogs_dict_common_entry(char *conffile)
{
  /* Applications section */
  {    
  {
    struct dict_vendor_data vendor_data = { 13019, "ETSI" };
    CHECK_FCT(fd_dict_new(fd_g_config->cnf_dict, DICT_VENDOR, &vendor_data, NULL, NULL));
  }
  }

  struct dict_object * UTF8String_type;

  CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "UTF8String", &UTF8String_type);
  
  /* Logical-Access-ID */
  {
    struct dict_avp_data data = {
      302,  /* Code */
      13019,  /* Vendor */
      "Logical-Access-ID",  /* Name */
      AVP_FLAG_VENDOR,  /* Fixed flags */
      AVP_FLAG_VENDOR,  /* Fixed flag values */
      AVP_TYPE_OCTETSTRING  /* base type of data */
    };
    CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
  };

  /* Physical-Access-ID */
  {
    struct dict_avp_data data = {
      313,  /* Code */
      13019,  /* Vendor */
      "Physical-Access-ID",  /* Name */
      AVP_FLAG_VENDOR,  /* Fixed flags */
      AVP_FLAG_VENDOR,  /* Fixed flag values */
      AVP_TYPE_OCTETSTRING  /* base type of data */
    };
    CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
  };

  /* Reservation-Priority */
  {
    struct dict_avp_data data = {
      458,  /* Code */
      13019,  /* Vendor */
      "Reservation-Priority",  /* Name */
      AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,  /* Fixed flags */
      AVP_FLAG_VENDOR,  /* Fixed flag values */
      AVP_TYPE_UNSIGNED32  /* base type of data */
    };
    CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
  };

  LOG_D( "Extension 'Dictionary definitions for DCCA 3GPP' initialized");
  return 0;
}

#if 0 /* modified by acetcom */
EXTENSION_ENTRY("dict_ogs_common", ogs_dict_common_entry, "dict_dcca_3gpp");
#endif
