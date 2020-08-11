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
#include "prom.h"
#include "pcrf-metrics.h"

static int initialized = 0;
void pcrf_metrics_initialize(void);

void pcrf_metrics_initialize(void) {
    pcrf_up_gauge = prom_collector_registry_must_register_metric(
            prom_gauge_new(
                    "open5gs_pcrf_up",
                    "Open5gs Pcrf is up",
                    0,
                    NULL)
                    );
    pcrf_sessions_counter = prom_collector_registry_must_register_metric(
            prom_counter_new(
                    "open5gs_pcrf_sessions",
                    "Open5gs Pcrf active sessions",
                    0,
                    NULL)
    );
}


int pcrf_initialize(void)
{
    int rv;

    pcrf_metrics_initialize();
    pcrf_context_init();

    rv = pcrf_context_parse_config();
    if (rv != OGS_OK) return rv;

    rv = ogs_log_config_domain(
            ogs_config()->logger.domain, ogs_config()->logger.level);
    if (rv != OGS_OK) return rv;

    rv = pcrf_db_init();
    if (rv != OGS_OK) return rv;

    rv = pcrf_fd_init();
    if (rv != OGS_OK) return OGS_ERROR;

    initialized = 1;
    prom_gauge_set(pcrf_up_gauge,1,NULL);

	return OGS_OK;
}

void pcrf_terminate(void)
{
    if (!initialized) return;

    pcrf_fd_final();

    pcrf_db_final();
    pcrf_context_final();
	
	return;
}
