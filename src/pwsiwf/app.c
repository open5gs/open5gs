/*
 * Copyright (C) 2019-2025 by Sukchan Lee <acetcom@gmail.com>
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

#include "ogs-sctp.h"
#include "ogs-app.h"
#include "init.h"
#include "context.h"
#include "ogs-ngap.h"

int app_initialize(const char *const argv[])
{
    int rv;

    /* Install log domain first */
    ogs_log_install_domain(&__pwsiws_log_domain, "pwsiws", ogs_core()->log.level);

    // Register NGAP log domain
    if (!ogs_log_find_domain("ngap"))
        ogs_log_install_domain(&__ogs_ngap_domain, "ngap", ogs_core()->log.level);

    ogs_sctp_init(ogs_app()->usrsctp.udp_port);
    rv = pwsiws_initialize();
    if (rv != OGS_OK) {
        return rv;
    }

    return OGS_OK;
}

void app_terminate(void)
{
    pwsiws_terminate();
    ogs_sctp_final();
} 