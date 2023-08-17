#include "ogs-trace.h"


int ogs_trace_init(const char *nf_name, const char *trace_uri)
{
    (void)nf_name;
    (void)trace_uri;

    return OGS_OK;
}

void ogs_trace_final(void)
{
}

ogs_trace_span_t *ogs_trace_span_start(const char *name,
        ogs_trace_kind_e kind, const char *traceparent)
{
    (void)name;
    (void)kind;
    (void)traceparent;

    return NULL;
}

void ogs_trace_span_stop(ogs_trace_span_t *span)
{
    (void)span;
}

void ogs_trace_span_update_name(ogs_trace_span_t *span, char *new_name)
{
    (void)span;
    (void)new_name;

    return;
}

char *ogs_trace_span_get_traceparent(ogs_trace_span_t *span)
{
    (void)span;

    return NULL;
}

void ogs_trace_span_add_attr(ogs_trace_span_t *span,
        const char *key, const char *val)
{
    (void)span;
    (void)key;
    (void)val;
}

void ogs_trace_span_add_attr_int(ogs_trace_span_t *span,
        const char *key, int64_t val)
{
    (void)span;
    (void)key;
    (void)val;
}

void ogs_trace_span_add_attrs(ogs_trace_span_t *span,
        const char *key_prefix, ogs_hash_t *vals)
{
    (void)span;
    (void)key_prefix;
    (void)vals;
}
