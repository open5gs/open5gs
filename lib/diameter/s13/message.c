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

#include "ogs-diameter-s13.h"

#define CHECK_dict_search( _type, _criteria, _what, _result )    \
    CHECK_FCT(  fd_dict_search( fd_g_config->cnf_dict, (_type), (_criteria), (_what), (_result), ENOENT) );

struct dict_object *ogs_diam_s13_application = NULL;

struct dict_object *ogs_diam_s13_cmd_ecr = NULL;
struct dict_object *ogs_diam_s13_cmd_eca = NULL;

struct dict_object *ogs_diam_s13_terminal_information = NULL;
struct dict_object *ogs_diam_s13_imei = NULL;
struct dict_object *ogs_diam_s13_software_version = NULL;
struct dict_object *ogs_diam_s13_equipment_status = NULL;

extern int ogs_dict_s13_entry(char *conffile);

int ogs_diam_s13_init(void)
{
    application_id_t id = OGS_DIAM_S13_APPLICATION_ID;

    ogs_assert(ogs_dict_s13_entry(NULL) == 0);

    CHECK_dict_search(DICT_APPLICATION, APPLICATION_BY_ID, (void *)&id, &ogs_diam_s13_application);
    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "ME-Identity-Check-Request", &ogs_diam_s13_cmd_ecr);
    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "ME-Identity-Check-Answer", &ogs_diam_s13_cmd_eca);

    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Terminal-Information", &ogs_diam_s13_terminal_information);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "IMEI", &ogs_diam_s13_imei);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Software-Version", &ogs_diam_s13_software_version);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Equipment-Status", &ogs_diam_s13_equipment_status);

    return 0;
}
