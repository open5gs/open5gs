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

#include "ogs-app.h"
#include "microhttpd.h"
#include "prom.h"
#include "promhttp.h"

struct MHD_Daemon *mhddaemon;

static void init(void) {
    prom_collector_registry_default_init();

    promhttp_set_active_collector_registry(NULL);
}

int app_initialize(const char *const argv[])
{
    init();
    int rv;

    mhddaemon = promhttp_start_daemon(MHD_USE_SELECT_INTERNALLY, 8000, NULL, NULL);
    if (mhddaemon == NULL) {
        return 1;
    }

    rv = pcrf_initialize();
    if (rv != OGS_OK) {
        ogs_error("Failed to intialize PCRF");
        return rv;
    }
    ogs_info("PCRF initialize...done");

    return OGS_OK;
}

void app_terminate(void)
{
    prom_collector_registry_destroy(PROM_COLLECTOR_REGISTRY_DEFAULT);
    MHD_stop_daemon(mhddaemon);

    pcrf_terminate();
    ogs_info("PCRF terminate...done");
}

