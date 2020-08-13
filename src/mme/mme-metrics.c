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

#include "mme-metrics.h"
#include "prom.h"

void mme_metrics_initialize(void) {
    mme_up_gauge = prom_collector_registry_must_register_metric(
            prom_gauge_new(
                    "open5gs_mme_up",
                    "Open5gs MME is up",
                    0,
                    NULL)
                    );
    mme_sessions_counter = prom_collector_registry_must_register_metric(
            prom_counter_new(
                    "open5gs_mme_sessions",
                    "Open5gs MME sessions added",
                    0,
                    NULL)
    );

    mme_ue_gauge = prom_collector_registry_must_register_metric(
            prom_gauge_new(
                    "open5gs_mme_gauge",
                    "Open5gs MME number of UE",
                    0,
                    NULL)
                    );

    mme_enb_gauge = prom_collector_registry_must_register_metric(
            prom_gauge_new(
                    "open5gs_enb_gauge",
                    "Open5gs MME number of eNB",
                    0,
                    NULL)
                    );

    mme_sessions_gauge = prom_collector_registry_must_register_metric(
            prom_gauge_new(
                    "open5gs_mme_session_gauge",
                    "Open5gs MME number of sessions",
                    0,
                    NULL)
                    );

}
