/*
 * Copyright (C) 2022 by sysmocom - s.f.m.c. GmbH <info@sysmocom.de>
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

#include "ogs-metrics.h"

typedef struct ogs_metrics_server_s {
    int unused;
} ogs_metrics_server_t;
typedef struct ogs_metrics_spec_s {
    int unused;
} ogs_metrics_spec_t;
typedef struct ogs_metrics_inst_s {
    int unused;
} ogs_metrics_inst_t;

void ogs_metrics_server_init(ogs_metrics_context_t *ctx)
{
}

void ogs_metrics_server_open(ogs_metrics_context_t *ctx)
{
}

void ogs_metrics_server_close(ogs_metrics_context_t *ctx)
{
}

void ogs_metrics_server_final(ogs_metrics_context_t *ctx)
{
}

ogs_metrics_server_t *ogs_metrics_server_add(
        ogs_sockaddr_t *addr, ogs_sockopt_t *option)
{
    return (ogs_metrics_server_t *)1;
}

void ogs_metrics_server_remove(ogs_metrics_server_t *server)
{
}

void ogs_metrics_server_remove_all(void)
{
}

void ogs_metrics_spec_init(ogs_metrics_context_t *ctx)
{
}

void ogs_metrics_spec_final(ogs_metrics_context_t *ctx)
{
}

ogs_metrics_spec_t *ogs_metrics_spec_new(
        ogs_metrics_context_t *ctx, ogs_metrics_metric_type_t type,
        const char *name, const char *description,
        int initial_val, unsigned int num_labels, const char ** labels,
        ogs_metrics_histogram_params_t *histogram_params)
{
    return (ogs_metrics_spec_t *)1;
}

void ogs_metrics_spec_free(ogs_metrics_spec_t *spec)
{
}

ogs_metrics_inst_t *ogs_metrics_inst_new(
        ogs_metrics_spec_t *spec,
        unsigned int num_labels, const char **label_values)
{
    return (ogs_metrics_inst_t *)1;
}

void ogs_metrics_inst_free(ogs_metrics_inst_t *inst)
{
}

void ogs_metrics_inst_set(ogs_metrics_inst_t *inst, int val)
{
}

void ogs_metrics_inst_reset(ogs_metrics_inst_t *inst)
{
}

void ogs_metrics_inst_add(ogs_metrics_inst_t *inst, int val)
{
}
