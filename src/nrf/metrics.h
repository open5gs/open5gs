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

#ifndef NRF_METRICS_H
#define NRF_METRICS_H

#include "ogs-metrics.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * TS 28.552 §5.19.1 — NRF performance measurements.
 *
 * Gauge:
 *   fivegs_nrffunction_nf_nfinstances  — current NF instance count
 *
 * Counters:
 *   fivegs_nrffunction_nf_nfregistrations      — successful NF registrations
 *   fivegs_nrffunction_nf_nfregfailed          — failed NF registrations
 *   fivegs_nrffunction_nf_nfheartbeats         — NF heartbeat (NF update) reqs
 *   fivegs_nrffunction_nf_nfderegistrations    — NF deregistrations
 *   fivegs_nrffunction_nf_nfdiscoveryreq       — NF discovery requests
 *   fivegs_nrffunction_nf_nfdiscoverysucc      — successful NF discoveries
 */
typedef enum nrf_metric_type_global_s {
    NRF_METR_GLOB_GAUGE_NF_INSTANCES = 0,
    NRF_METR_GLOB_CTR_NF_REGISTER_SUCC,
    NRF_METR_GLOB_CTR_NF_REGISTER_FAIL,
    NRF_METR_GLOB_CTR_NF_HEARTBEAT,
    NRF_METR_GLOB_CTR_NF_DEREGISTER,
    NRF_METR_GLOB_CTR_NF_DISCOVER_REQ,
    NRF_METR_GLOB_CTR_NF_DISCOVER_SUCC,
    _NRF_METR_GLOB_MAX,
} nrf_metric_type_global_t;

extern ogs_metrics_inst_t *nrf_metrics_inst_global[_NRF_METR_GLOB_MAX];

static inline void nrf_metrics_inst_global_set(
        nrf_metric_type_global_t t, int val)
{ ogs_metrics_inst_set(nrf_metrics_inst_global[t], val); }
static inline void nrf_metrics_inst_global_add(
        nrf_metric_type_global_t t, int val)
{ ogs_metrics_inst_add(nrf_metrics_inst_global[t], val); }
static inline void nrf_metrics_inst_global_inc(nrf_metric_type_global_t t)
{ ogs_metrics_inst_inc(nrf_metrics_inst_global[t]); }
static inline void nrf_metrics_inst_global_dec(nrf_metric_type_global_t t)
{ ogs_metrics_inst_dec(nrf_metrics_inst_global[t]); }

void nrf_metrics_init(void);
void nrf_metrics_final(void);

#ifdef __cplusplus
}
#endif

#endif /* NRF_METRICS_H */
