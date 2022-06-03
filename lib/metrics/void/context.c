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

typedef struct ogs_metrics_context_s {
    int unused;
} ogs_metrics_context_t;
typedef struct ogs_metrics_spec_s {
    int unused;
} ogs_metrics_spec_t;
typedef struct ogs_metrics_inst_s {
    int unused;
} ogs_metrics_inst_t;

static ogs_metrics_context_t self;
static int context_initialized = 0;

void ogs_metrics_context_init(void)
{
    ogs_assert(context_initialized == 0);
    ogs_log_install_domain(&__ogs_metrics_domain, "metrics", ogs_core()->log.level);
    context_initialized = 1;
}

void ogs_metrics_context_final(void)
{
    ogs_assert(context_initialized == 1);
    context_initialized = 0;
}

ogs_metrics_context_t *ogs_metrics_self(void)
{
    return &self;
}

int ogs_metrics_context_parse_config(void)
{
    return OGS_OK;
}

void ogs_metrics_context_open(ogs_metrics_context_t *ctx)
{
}

void ogs_metrics_context_close(ogs_metrics_context_t *ctx)
{
}

ogs_metrics_spec_t *ogs_metrics_spec_new(
        ogs_metrics_context_t *ctx, ogs_metrics_metric_type_t type,
        const char *name, const char *description,
        int initial_val, unsigned int num_labels, const char ** labels)
{
    return (ogs_metrics_spec_t *)&self;
}

void ogs_metrics_spec_free(ogs_metrics_spec_t *spec)
{
}

ogs_metrics_inst_t *ogs_metrics_inst_new(
        ogs_metrics_spec_t *spec,
        unsigned int num_labels, const char **label_values)
{
    return (ogs_metrics_inst_t *)&self;
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
