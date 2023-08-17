/*
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

#ifndef OGS_TRACE_H
#define OGS_TRACE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "app/ogs-app.h"

#define OGS_TRACE_INSIDE
#undef OGS_TRACE_INSIDE

typedef struct ogs_trace_tracer_s ogs_trace_tracer_t;
typedef struct ogs_trace_span_s ogs_trace_span_t;

typedef enum {
    OGS_TRACE_KIND_SERVER,
    OGS_TRACE_KIND_CLIENT,
} ogs_trace_kind_e;

int ogs_trace_init(const char *nf_name, const char *trace_uri);
void ogs_trace_final(void);

ogs_trace_span_t *ogs_trace_span_start(const char *name,
        ogs_trace_kind_e kind, const char *traceparent);
void ogs_trace_span_stop(ogs_trace_span_t *span);

void ogs_trace_span_update_name(ogs_trace_span_t *span, char *new_name);
char *ogs_trace_span_get_traceparent(ogs_trace_span_t *span);
void ogs_trace_span_add_attr(ogs_trace_span_t *span,
        const char *key, const char *val);
void ogs_trace_span_add_attr_int(ogs_trace_span_t *span,
        const char *key, int64_t val);
void ogs_trace_span_add_attrs(ogs_trace_span_t *span,
        const char *key_prefix, ogs_hash_t *vals);


#ifdef __cplusplus
}
#endif

#endif /* OGS_TRACE_H */
