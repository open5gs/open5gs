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
    int rv;

    rv = ogs_diam_init(FD_MODE_CLIENT|FD_MODE_SERVER,
                pcrf_self()->diam_conf_path, pcrf_self()->diam_config);
    ogs_assert(rv == 0);

    rv = pcrf_gx_init();
    ogs_assert(rv == OGS_OK);
    rv = pcrf_rx_init();
    ogs_assert(rv == OGS_OK);

    rv = ogs_diam_start();
    ogs_assert(rv == 0);

	return OGS_OK;
}

void pcrf_fd_final(void)
{
    pcrf_gx_final();
    pcrf_rx_final();

    ogs_diam_final();
}
