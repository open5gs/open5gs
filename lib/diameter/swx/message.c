/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
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

#include "ogs-diameter-swx.h"

#define CHECK_dict_search( _type, _criteria, _what, _result )    \
    CHECK_FCT(  fd_dict_search( fd_g_config->cnf_dict, (_type), (_criteria), (_what), (_result), ENOENT) );

struct dict_object *ogs_diam_swx_application = NULL;

struct dict_object *ogs_diam_swx_non_3gpp_user_data = NULL;
struct dict_object *ogs_diam_swx_non_3gpp_ip_access = NULL;
struct dict_object *ogs_diam_swx_non_3gpp_ip_access_apn = NULL;

int ogs_diam_swx_init(void)
{
    application_id_t id = OGS_DIAM_SWX_APPLICATION_ID;

    CHECK_dict_search(DICT_APPLICATION, APPLICATION_BY_ID,
            (void *)&id, &ogs_diam_swx_application);

    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS,
            "Non-3GPP-User-Data", &ogs_diam_swx_non_3gpp_user_data);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS,
            "Non-3GPP-IP-Access", &ogs_diam_swx_non_3gpp_ip_access);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS,
            "Non-3GPP-IP-Access-APN", &ogs_diam_swx_non_3gpp_ip_access_apn);

    return 0;
}
