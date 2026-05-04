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

#include "ogs-app.h"

#include "metrics.h"

typedef struct nrf_metrics_spec_def_s {
    unsigned int type;
    const char *name;
    const char *description;
    int initial_val;
    unsigned int num_labels;
    const char **labels;
} nrf_metrics_spec_def_t;

/* Helper generic functions */
static int nrf_metrics_init_inst(ogs_metrics_inst_t **inst,
        ogs_metrics_spec_t **specs, unsigned int len,
        unsigned int num_labels, const char **labels)
{
    unsigned int i;
    for (i = 0; i < len; i++)
        inst[i] = ogs_metrics_inst_new(specs[i], num_labels, labels);
    return OGS_OK;
}

static int nrf_metrics_init_spec(ogs_metrics_context_t *ctx,
        ogs_metrics_spec_t **dst, nrf_metrics_spec_def_t *src,
        unsigned int len)
{
    unsigned int i;
    for (i = 0; i < len; i++) {
        dst[i] = ogs_metrics_spec_new(ctx, src[i].type,
                src[i].name, src[i].description,
                src[i].initial_val, src[i].num_labels, src[i].labels,
                NULL);
    }
    return OGS_OK;
}

/* GLOBAL */
static ogs_metrics_spec_t *nrf_metrics_spec_global[_NRF_METR_GLOB_MAX];
ogs_metrics_inst_t *nrf_metrics_inst_global[_NRF_METR_GLOB_MAX];
static nrf_metrics_spec_def_t nrf_metrics_spec_def_global[_NRF_METR_GLOB_MAX] = {
/* Global Gauges: */
[NRF_METR_GLOB_GAUGE_NF_INSTANCES] = {
    .type = OGS_METRICS_METRIC_TYPE_GAUGE,
    .name = "fivegs_nrffunction_nf_nfinstances",
    .description = "Current number of registered NF instances at the NRF",
},
/* Global Counters: */
[NRF_METR_GLOB_CTR_NF_REGISTER_SUCC] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "fivegs_nrffunction_nf_nfregistrations",
    .description = "Number of successful NF registrations at the NRF",
},
[NRF_METR_GLOB_CTR_NF_REGISTER_FAIL] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "fivegs_nrffunction_nf_nfregfailed",
    .description = "Number of failed NF registrations at the NRF",
},
[NRF_METR_GLOB_CTR_NF_HEARTBEAT] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "fivegs_nrffunction_nf_nfheartbeats",
    .description = "Number of NF heartbeat (NF update) requests at the NRF",
},
[NRF_METR_GLOB_CTR_NF_DEREGISTER] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "fivegs_nrffunction_nf_nfderegistrations",
    .description = "Number of NF deregistrations at the NRF",
},
[NRF_METR_GLOB_CTR_NF_DISCOVER_REQ] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "fivegs_nrffunction_nf_nfdiscoveryreq",
    .description = "Number of NF discovery requests received by the NRF",
},
[NRF_METR_GLOB_CTR_NF_DISCOVER_SUCC] = {
    .type = OGS_METRICS_METRIC_TYPE_COUNTER,
    .name = "fivegs_nrffunction_nf_nfdiscoverysucc",
    .description = "Number of successful NF discoveries at the NRF",
},
};

static int nrf_metrics_init_inst_global(void)
{
    return nrf_metrics_init_inst(nrf_metrics_inst_global,
            nrf_metrics_spec_global, _NRF_METR_GLOB_MAX, 0, NULL);
}

void nrf_metrics_init(void)
{
    ogs_metrics_context_t *ctx = ogs_metrics_self();
    ogs_metrics_context_init();

    nrf_metrics_init_spec(ctx, nrf_metrics_spec_global,
            nrf_metrics_spec_def_global, _NRF_METR_GLOB_MAX);

    nrf_metrics_init_inst_global();
}

void nrf_metrics_final(void)
{
    ogs_metrics_context_final();
}
