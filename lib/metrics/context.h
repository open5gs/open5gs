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

#if !defined(OGS_METRICS_INSIDE) && !defined(OGS_METRICS_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_METRICS_CONTEXT_H
#define OGS_METRICS_CONTEXT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ogs_metrics_server_s ogs_metrics_server_t;

typedef enum ogs_metrics_metric_type_s  {
    OGS_METRICS_METRIC_TYPE_COUNTER,
    OGS_METRICS_METRIC_TYPE_GAUGE,
    OGS_METRICS_METRIC_TYPE_HISTOGRAM,
} ogs_metrics_metric_type_t;

typedef struct ogs_metrics_context_s {
    ogs_list_t  server_list;
    ogs_list_t  spec_list;

    uint16_t    metrics_port;
} ogs_metrics_context_t;

typedef enum ogs_metrics_histogram_bucket_type_s  {
    OGS_METRICS_HISTOGRAM_BUCKET_TYPE_VARIABLE,
    OGS_METRICS_HISTOGRAM_BUCKET_TYPE_LINEAR,
    OGS_METRICS_HISTOGRAM_BUCKET_TYPE_EXPONENTIAL,
} ogs_metrics_histogram_bucket_type_t;

typedef struct ogs_metrics_histogram_bucket_params_s {
    ogs_metrics_histogram_bucket_type_t type;
    unsigned int count;
#define OGS_METRICS_HIST_VAR_BUCKETS_MAX 10
    union {
        struct {
            float start;
            float width;
        } lin;
        struct {
            float start;
            float factor;
        } exp;
        struct {
            float buckets[OGS_METRICS_HIST_VAR_BUCKETS_MAX];
        } var;
    };
} ogs_metrics_histogram_params_t;

typedef struct ogs_metrics_context_s ogs_metrics_context_t;
void ogs_metrics_context_init(void);
void ogs_metrics_context_open(ogs_metrics_context_t *ctx);
void ogs_metrics_context_close(ogs_metrics_context_t *ctx);
void ogs_metrics_context_final(void);
ogs_metrics_context_t *ogs_metrics_self(void);
int ogs_metrics_context_parse_config(const char *local);

void ogs_metrics_server_init(ogs_metrics_context_t *ctx);
void ogs_metrics_server_open(ogs_metrics_context_t *ctx);
void ogs_metrics_server_close(ogs_metrics_context_t *ctx);
void ogs_metrics_server_final(ogs_metrics_context_t *ctx);
ogs_metrics_server_t *ogs_metrics_server_add(
        ogs_sockaddr_t *addr, ogs_sockopt_t *option);
void ogs_metrics_server_remove(ogs_metrics_server_t *server);
void ogs_metrics_server_remove_all(void);

typedef struct ogs_metrics_spec_s ogs_metrics_spec_t;
void ogs_metrics_spec_init(ogs_metrics_context_t *ctx); 
void ogs_metrics_spec_final(ogs_metrics_context_t *ctx);
ogs_metrics_spec_t *ogs_metrics_spec_new(
        ogs_metrics_context_t *ctx, ogs_metrics_metric_type_t type,
        const char *name, const char *description,
        int initial_val, unsigned int num_labels, const char ** labels,
        ogs_metrics_histogram_params_t *histogram_params);
void ogs_metrics_spec_free(ogs_metrics_spec_t *spec);

typedef struct ogs_metrics_inst_s ogs_metrics_inst_t;
ogs_metrics_inst_t *ogs_metrics_inst_new(
        ogs_metrics_spec_t *spec,
        unsigned int num_labels, const char **label_values);
void ogs_metrics_inst_free(ogs_metrics_inst_t *inst);
void ogs_metrics_inst_set(ogs_metrics_inst_t *inst, int val);
void ogs_metrics_inst_reset(ogs_metrics_inst_t *inst);
void ogs_metrics_inst_add(ogs_metrics_inst_t *inst, int val);
static inline void ogs_metrics_inst_inc(ogs_metrics_inst_t *inst)
{
    ogs_metrics_inst_add(inst, 1);
}
static inline void ogs_metrics_inst_dec(ogs_metrics_inst_t *inst)
{
    ogs_metrics_inst_add(inst, -1);
}

#ifdef __cplusplus
}
#endif

#endif /* OGS_METRICS_CONTEXT_H */
