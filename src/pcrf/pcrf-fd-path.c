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

#include "pcrf-context.h"
#include "pcrf-fd-path.h"

int pcrf_fd_init(void)
{
    int rv, ret;
    struct dict_object *gx_app, *rx_app, *vnd;
    struct dict_vendor_data vnd_data;
    struct dict_application_data gx_app_data, rx_app_data;

    ret = ogs_diam_init(FD_MODE_CLIENT|FD_MODE_SERVER,
                pcrf_self()->diam_conf_path, pcrf_self()->diam_config);
    ogs_assert(ret == 0);

    vnd_data.vendor_id = 10415;
    vnd_data.vendor_name = (char *) "3GPP";

    ret = fd_dict_new(fd_g_config->cnf_dict,
            DICT_VENDOR, &vnd_data, NULL, &vnd);
    ogs_assert(ret == 0);

    gx_app_data.application_id = 16777238;
    gx_app_data.application_name = (char *) "Gx";

    rx_app_data.application_id = 16777236;
    rx_app_data.application_name = (char *) "Rx";

    ret = fd_dict_new(fd_g_config->cnf_dict, DICT_APPLICATION,
            &gx_app_data, NULL, &gx_app);
    ogs_assert(ret == 0);
    ret = fd_dict_new(fd_g_config->cnf_dict, DICT_APPLICATION,
            &rx_app_data, NULL, &rx_app);
    ogs_assert(ret == 0);

    ret = fd_disp_app_support(gx_app, vnd, 1, 0);
    ogs_assert(ret == 0);
    ret = fd_disp_app_support(rx_app, vnd, 1, 0);
    ogs_assert(ret == 0);

    rv = pcrf_gx_init();
    ogs_assert(rv == OGS_OK);
    rv = pcrf_rx_init();
    ogs_assert(rv == OGS_OK);

	return OGS_OK;
}

void pcrf_fd_final(void)
{
    pcrf_gx_final();
    pcrf_rx_final();

    ogs_diam_final();
}
