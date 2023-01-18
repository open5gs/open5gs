/*
 * Copyright (C) 2023 by Ryan Dimsey <ryan@omnitouch.com.au>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <freeDiameter/extension.h>

/* The content of this file follows the same structure as dict_base_proto.c */

#define CHECK_dict_new( _type, _data, _parent, _ref )    \
    CHECK_FCT(  fd_dict_new( fd_g_config->cnf_dict, (_type), (_data), (_parent), (_ref))  );

#define CHECK_dict_search( _type, _criteria, _what, _result )    \
    CHECK_FCT(  fd_dict_search( fd_g_config->cnf_dict, (_type), (_criteria), (_what), (_result), ENOENT) );

struct local_rules_definition {
    struct dict_avp_request avp_vendor_plus_name;
    enum rule_position    position;
    int            min;
    int            max;
};

#define RULE_ORDER( _position ) ((((_position) == RULE_FIXED_HEAD) || ((_position) == RULE_FIXED_TAIL)) ? 1 : 0 )

/* Attention! This version of the macro uses AVP_BY_NAME_AND_VENDOR, in contrast to most other copies! */
#define PARSE_loc_rules( _rulearray, _parent) {                                \
    int __ar;                                            \
    for (__ar=0; __ar < sizeof(_rulearray) / sizeof((_rulearray)[0]); __ar++) {            \
        struct dict_rule_data __data = { NULL,                             \
            (_rulearray)[__ar].position,                            \
            0,                                         \
            (_rulearray)[__ar].min,                                \
            (_rulearray)[__ar].max};                            \
        __data.rule_order = RULE_ORDER(__data.rule_position);                    \
        CHECK_FCT(  fd_dict_search(                                 \
            fd_g_config->cnf_dict,                                \
            DICT_AVP,                                     \
            AVP_BY_NAME_AND_VENDOR,                             \
            &(_rulearray)[__ar].avp_vendor_plus_name,                    \
            &__data.rule_avp, 0 ) );                            \
        if ( !__data.rule_avp ) {                                \
            TRACE_DEBUG(INFO, "AVP Not found: '%s'", (_rulearray)[__ar].avp_vendor_plus_name.avp_name);        \
            return ENOENT;                                    \
        }                                            \
        CHECK_FCT_DO( fd_dict_new( fd_g_config->cnf_dict, DICT_RULE, &__data, _parent, NULL),    \
            {                                            \
                TRACE_DEBUG(INFO, "Error on rule with AVP '%s'",                  \
                        (_rulearray)[__ar].avp_vendor_plus_name.avp_name);        \
                return EINVAL;                                      \
            } );                                              \
    }                                                      \
}

#define enumval_def_u32( _val_, _str_ ) \
        { _str_,         { .u32 = _val_ }}

#define enumval_def_os( _len_, _val_, _str_ ) \
        { _str_,         { .os = { .data = (unsigned char *)_val_, .len = _len_ }}}

int ogs_dict_s13_entry(char *conffile)
{
    struct dict_object *s13;
    TRACE_ENTRY("%p", conffile);

    /* Applications section */
    {
        struct dict_object * vendor;
        CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_VENDOR, VENDOR_BY_NAME, "3GPP", &vendor, ENOENT));
        struct dict_application_data app_data = { 16777252, "S13" };
        CHECK_FCT(fd_dict_new(fd_g_config->cnf_dict, DICT_APPLICATION, &app_data, vendor, &s13));
    }

    /* AVP section */
    {
        /* Terminal Information AVP - 3GPP TS 29.272 #7.3.3 */
        {
            struct dict_object * avp;
            struct local_rules_definition rules[] =
            {
                { { .avp_vendor = 10415, .avp_name = "IMEI" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "3GPP2-MEID" }, RULE_OPTIONAL, -1, 1 },
                { { .avp_vendor = 10415, .avp_name = "Software-Version" }, RULE_OPTIONAL, -1, 1 },
            };

            CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Terminal-Information", &avp);
            PARSE_loc_rules(rules, avp);
        }
    };

    /* Command section */
    {
        /* S13-ME-Identity-Check-Request - 3GPP TS 29.272 #7.2.19 */
        {
            struct dict_object * cmd;
            struct dict_cmd_data data = {
                324,                                                    /* Code */
                "ME-Identity-Check-Request",                            /* Name */
                CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, /* Fixed flags */
                CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE                   /* Fixed flag values */
            };
            struct local_rules_definition rules[] =
            {
                {  {                      .avp_name = "Session-Id" }, RULE_FIXED_HEAD, -1, 1 },
                {  {                      .avp_name = "Vendor-Specific-Application-Id" }, RULE_OPTIONAL, -1, 1 },
                {  {                      .avp_name = "Auth-Session-State" }, RULE_REQUIRED, -1, 1 },
                {  {                      .avp_name = "Origin-Host" }, RULE_REQUIRED, -1, 1 },
                {  {                      .avp_name = "Origin-Realm" }, RULE_REQUIRED, -1, 1 },
                {  {                      .avp_name = "Destination-Host" }, RULE_OPTIONAL, -1, 1 },
                {  {                      .avp_name = "Destination-Realm" }, RULE_REQUIRED, -1, 1 },
                {  { .avp_vendor = 10415, .avp_name = "Terminal-Information" }, RULE_REQUIRED, -1, 1 },
                {  {                      .avp_name = "User-Name" }, RULE_REQUIRED, -1, 1 },
                {  {                      .avp_name = "Proxy-Info" }, RULE_OPTIONAL, -1, -1 },
                {  {                      .avp_name = "Route-Record" }, RULE_OPTIONAL, -1, -1 },

            };

            CHECK_dict_new(DICT_COMMAND, &data, s13, &cmd);
            PARSE_loc_rules(rules, cmd);
        }

        /* S13-ME-Identity-Check-Answer - 3GPP TS 29.272 #7.2.20 */
        {
            struct dict_object * cmd;
            struct dict_cmd_data data = {
                324,                                                    /* Code */
                "ME-Identity-Check-Answer",                             /* Name */
                CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR, /* Fixed flags */
                CMD_FLAG_PROXIABLE                                      /* Fixed flag values */
            };
            struct local_rules_definition rules[] =
            {
                {  {                      .avp_name = "Session-Id" }, RULE_FIXED_HEAD, -1, 1 },
                {  {                      .avp_name = "Vendor-Specific-Application-Id" }, RULE_OPTIONAL, -1, 1 },
                {  {                      .avp_name = "Result-Code" }, RULE_OPTIONAL, -1, 1 },
                {  {                      .avp_name = "Experimental-Result" }, RULE_OPTIONAL, -1, 1 },
                {  {                      .avp_name = "Auth-Session-State" }, RULE_REQUIRED, -1, 1 },
                {  {                      .avp_name = "Origin-Host" }, RULE_REQUIRED, -1, 1 },
                {  {                      .avp_name = "Origin-Realm" }, RULE_REQUIRED, -1, 1 },
                {  { .avp_vendor = 10415, .avp_name = "Equipment-Status" }, RULE_OPTIONAL, -1, 1 },
                {  {                      .avp_name = "Failed-AVP" }, RULE_OPTIONAL, -1, 1 },
                {  {                      .avp_name = "Proxy-Info" }, RULE_OPTIONAL, -1, -1 },
                {  {                      .avp_name = "Route-Record" }, RULE_OPTIONAL, -1, -1 },
            };

            CHECK_dict_new(DICT_COMMAND, &data, s13, &cmd);
            PARSE_loc_rules(rules, cmd);
        }
    }

    LOG_D( "Extension 'Dictionary definitions for DCCA 3GPP S13' initialized");
    return 0;
}

#if 0
EXTENSION_ENTRY("dict_s13", ogs_dict_s13_entry, "dict_dcca_3gpp");
#endif
