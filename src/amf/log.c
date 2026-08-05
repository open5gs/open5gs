/*
 * Copyright (C) 2019-2026 by Sukchan Lee <acetcom@gmail.com>
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

#include "log.h"

static const char *amf_log_level_string(ogs_log_level_e level)
{
    switch (level) {
    case OGS_LOG_FATAL:  return "FATAL";
    case OGS_LOG_ERROR:  return "ERROR";
    case OGS_LOG_WARN:   return "WARNING";
    case OGS_LOG_INFO:   return "INFO";
    case OGS_LOG_DEBUG:  return "DEBUG";
    case OGS_LOG_TRACE:  return "TRACE";
    default:             return "NONE";
    }
}

static char *amf_log_json_id(char *buf, char *last,
        const char *field, ogs_pool_id_t id)
{
    if (id >= OGS_MIN_POOL_ID && id <= OGS_MAX_POOL_ID)
        return ogs_slprintf(buf, last, "\"%s\":%d,", field, (int)id);
    return ogs_slprintf(buf, last, "\"%s\":null,", field);
}

static char *amf_log_json_ngap_id(char *buf, char *last,
        const char *field, uint64_t id)
{
    if (id != INVALID_UE_NGAP_ID)
        return ogs_slprintf(buf, last, "\"%s\":%llu,", field,
                (unsigned long long)id);
    return ogs_slprintf(buf, last, "\"%s\":null,", field);
}

static void amf_log_ue_event_ex(ogs_log_level_e level,
    const char *event_name, const char *outcome,
    const char *event_type,
    amf_ue_t *amf_ue, ran_ue_t *ran_ue,
    const char *message, const amf_log_error_t *error,
    const char *suci_raw, const amf_log_guti_t *guti,
    const amf_log_trace_t *trace)
{
    amf_log_context_t ctx;

    if (!event_name)
        return;

    if (!outcome)
        outcome = AMF_EVENT_OUTCOME_SUCCESS;
    if (!event_type)
        event_type = AMF_EVENT_TYPE_PROCEDURE;

    amf_log_context_clear(&ctx);

    if (amf_ue)
        amf_log_context_from_ue(&ctx, amf_ue);
    if (ran_ue)
        amf_log_context_from_ran_ue(&ctx, ran_ue);

    if (ogs_log_get_format() == OGS_LOG_FORMAT_JSON) {
        char buf[OGS_HUGE_LEN];
        char *p = buf;
        char *last = buf + OGS_HUGE_LEN;
        const char *level_str = amf_log_level_string(level);

        p = ogs_slprintf(p, last, "{");
        p = ogs_log_json_timestamp(p, last);
        p = ogs_slprintf(p, last, ",\"level\":\"%s\"", level_str);
        p = ogs_slprintf(p, last, ",");
        p = ogs_log_json_string(p, last, "nf", "amf");
        p = ogs_slprintf(p, last, ",");
        p = ogs_log_json_string(p, last, "domain", "gmm");

        p = ogs_slprintf(p, last, ",\"event\":{");
        p = ogs_log_json_string(p, last, "name", event_name);
        p = ogs_slprintf(p, last, ",\"category\":\"amf\"");
        p = ogs_slprintf(p, last, ",");
        p = ogs_log_json_string(p, last, "type", event_type);
        p = ogs_slprintf(p, last, ",");
        p = ogs_log_json_string(p, last, "outcome", outcome);
        p = ogs_slprintf(p, last, "}");

        p = ogs_slprintf(p, last, ",\"ue\":{");
        p = amf_log_json_id(p, last, "amf_ue_id", ctx.amf_ue_id);
        p = amf_log_json_id(p, last, "ran_ue_id", ctx.ran_ue_id);
        p = ogs_log_json_string(p, last, "suci", ctx.suci);
        p = ogs_slprintf(p, last, ",");
        p = ogs_log_json_string(p, last, "supi", ctx.supi);
        p = ogs_slprintf(p, last, "}");

        if (suci_raw) {
            p = ogs_slprintf(p, last, ",\"suci\":{");
            p = ogs_log_json_string(p, last, "raw", suci_raw);
            p = ogs_slprintf(p, last, "}");
        }

        if (guti) {
            p = ogs_slprintf(p, last, ",\"guti\":{");
            p = ogs_log_json_string(p, last, "raw", guti->raw);
            p = ogs_slprintf(p, last, ",");
            p = ogs_log_json_string(p, last, "plmn_id_hex", guti->plmn_id_hex);
            p = ogs_slprintf(p, last, ",");
            p = ogs_log_json_string(p, last, "amf_id_hex", guti->amf_id_hex);
            p = ogs_slprintf(p, last, ",");
            p = ogs_log_json_string(p, last, "m_tmsi_hex", guti->m_tmsi_hex);
            p = ogs_slprintf(p, last, "}");
        }

        p = ogs_slprintf(p, last, ",\"ngap\":{");
        p = amf_log_json_ngap_id(p, last, "ran_ue_ngap_id", ctx.ran_ue_ngap_id);
        p = amf_log_json_ngap_id(p, last, "amf_ue_ngap_id", ctx.amf_ue_ngap_id);
        p = ogs_slprintf(p, last, "}");

        if (error) {
            p = ogs_slprintf(p, last, ",\"error\":{");
            p = ogs_log_json_string(p, last, "reason", error->reason);
            p = ogs_slprintf(p, last, ",\"code\":%d", error->code);
            p = ogs_slprintf(p, last, ",");
            p = ogs_log_json_string(p, last, "source", error->source);
            p = ogs_slprintf(p, last, "}");
        }

        if (trace) {
            p = ogs_slprintf(p, last, ",\"trace\":{");
            p = ogs_log_json_string(p, last, "id", trace->trace_id);
            p = ogs_slprintf(p, last, ",");
            p = ogs_log_json_string(p, last, "span_id", trace->span_id);
            p = ogs_slprintf(p, last, "}");

            p = ogs_slprintf(p, last, ",\"correlation\":{");
            p = ogs_log_json_string(p, last, "id", trace->correlation_id);
            p = ogs_slprintf(p, last, "}");
        }

        p = ogs_slprintf(p, last, ",");
        p = ogs_log_json_string(p, last, "message", message ? message : "");
        p = ogs_slprintf(p, last, "}\n");

        ogs_log_write_raw(level, __gmm_log_domain, buf);
    } else {
        ogs_log_printf(level, __gmm_log_domain,
                0, NULL, 0, NULL, 1, "[%s] %s",
                event_name, message ? message : event_name);
    }
}

void amf_log_ue_event(ogs_log_level_e level,
    const char *event_name, const char *outcome,
    const char *event_type,
    amf_ue_t *amf_ue, ran_ue_t *ran_ue,
    const char *message)
{
    amf_log_ue_event_ex(level, event_name, outcome, event_type,
            amf_ue, ran_ue, message, NULL, NULL, NULL, NULL);
}

void amf_log_ue_error_event(ogs_log_level_e level,
    const char *event_name, const char *event_type,
    amf_ue_t *amf_ue, ran_ue_t *ran_ue,
    const char *message, const amf_log_error_t *error)
{
    amf_log_ue_event_ex(level, event_name, AMF_EVENT_OUTCOME_FAILURE,
            event_type, amf_ue, ran_ue, message, error, NULL, NULL, NULL);
}

void amf_log_ue_suci_event(ogs_log_level_e level,
    const char *event_name, const char *outcome,
    const char *event_type,
    amf_ue_t *amf_ue, ran_ue_t *ran_ue,
    const char *message, const char *suci_raw)
{
    amf_log_ue_event_ex(level, event_name, outcome, event_type,
            amf_ue, ran_ue, message, NULL, suci_raw, NULL, NULL);
}

void amf_log_ue_guti_event(ogs_log_level_e level,
    const char *event_name, const char *outcome,
    const char *event_type,
    amf_ue_t *amf_ue, ran_ue_t *ran_ue,
    const char *message, const amf_log_guti_t *guti)
{
    amf_log_ue_event_ex(level, event_name, outcome, event_type,
            amf_ue, ran_ue, message, NULL, NULL, guti, NULL);
}

void amf_log_ue_sbi_event(ogs_log_level_e level,
    const char *event_name, const char *outcome,
    amf_ue_t *amf_ue, ran_ue_t *ran_ue,
    const char *message, const amf_log_trace_t *trace)
{
    amf_log_ue_event_ex(level, event_name, outcome,
            AMF_EVENT_TYPE_DEPENDENCY,
            amf_ue, ran_ue, message, NULL, NULL, NULL, trace);
}

void amf_log_ue_sbi_error_event(ogs_log_level_e level,
    const char *event_name,
    amf_ue_t *amf_ue, ran_ue_t *ran_ue,
    const char *message, const amf_log_error_t *error,
    const amf_log_trace_t *trace)
{
    amf_log_ue_event_ex(level, event_name, AMF_EVENT_OUTCOME_FAILURE,
            AMF_EVENT_TYPE_DEPENDENCY,
            amf_ue, ran_ue, message, error, NULL, NULL, trace);
}

void amf_log_context_clear(amf_log_context_t *ctx)
{
    if (!ctx)
        return;

    ctx->gnb_id = OGS_INVALID_POOL_ID;
    ctx->ran_ue_id = OGS_INVALID_POOL_ID;
    ctx->amf_ue_id = OGS_INVALID_POOL_ID;
    ctx->sess_id = OGS_INVALID_POOL_ID;

    ctx->ran_ue_ngap_id = INVALID_UE_NGAP_ID;
    ctx->amf_ue_ngap_id = 0;

    ctx->suci = NULL;
    ctx->supi = NULL;
}

void amf_log_context_from_event(amf_log_context_t *ctx, amf_event_t *e)
{
    amf_ue_t *amf_ue = NULL;
    ran_ue_t *ran_ue = NULL;
    amf_sess_t *sess = NULL;

    if (!ctx)
        return;
    if (!e)
        return;

    ctx->gnb_id = e->gnb_id;
    ctx->ran_ue_id = e->ran_ue_id;
    ctx->amf_ue_id = e->amf_ue_id;
    ctx->sess_id = e->sess_id;

    if (e->ran_ue_id >= OGS_MIN_POOL_ID && e->ran_ue_id <= OGS_MAX_POOL_ID)
        ran_ue = ran_ue_find_by_id(e->ran_ue_id);
    if (e->amf_ue_id >= OGS_MIN_POOL_ID && e->amf_ue_id <= OGS_MAX_POOL_ID)
        amf_ue = amf_ue_find_by_id(e->amf_ue_id);
    if (e->sess_id >= OGS_MIN_POOL_ID && e->sess_id <= OGS_MAX_POOL_ID)
        sess = amf_sess_find_by_id(e->sess_id);

    if (ran_ue) {
        ctx->ran_ue_ngap_id = ran_ue->ran_ue_ngap_id;
        ctx->amf_ue_ngap_id = ran_ue->amf_ue_ngap_id;
        ctx->gnb_id = ran_ue->gnb_id;
    }
    if (amf_ue) {
        ctx->amf_ue_id = amf_ue->id;
        ctx->suci = amf_ue->suci;
        ctx->supi = amf_ue->supi;
        if (!ran_ue && amf_ue->ran_ue_id >= OGS_MIN_POOL_ID &&
                amf_ue->ran_ue_id <= OGS_MAX_POOL_ID) {
            ran_ue_t *linked_ran = ran_ue_find_by_id(amf_ue->ran_ue_id);
            if (linked_ran) {
                ctx->ran_ue_id = amf_ue->ran_ue_id;
                ctx->ran_ue_ngap_id = linked_ran->ran_ue_ngap_id;
                ctx->amf_ue_ngap_id = linked_ran->amf_ue_ngap_id;
                ctx->gnb_id = linked_ran->gnb_id;
            }
        }
    }
    if (sess) {
        ctx->sess_id = sess->id;
    }
}

void amf_log_context_from_ue(amf_log_context_t *ctx, amf_ue_t *amf_ue)
{
    ran_ue_t *ran_ue = NULL;

    if (!ctx)
        return;
    if (!amf_ue)
        return;

    ctx->amf_ue_id = amf_ue->id;
    ctx->suci = amf_ue->suci;
    ctx->supi = amf_ue->supi;

    if (amf_ue->ran_ue_id >= OGS_MIN_POOL_ID &&
            amf_ue->ran_ue_id <= OGS_MAX_POOL_ID) {
        ran_ue = ran_ue_find_by_id(amf_ue->ran_ue_id);
    }
    if (ran_ue) {
        ctx->ran_ue_id = amf_ue->ran_ue_id;
        ctx->ran_ue_ngap_id = ran_ue->ran_ue_ngap_id;
        ctx->amf_ue_ngap_id = ran_ue->amf_ue_ngap_id;
        ctx->gnb_id = ran_ue->gnb_id;
    }
}

void amf_log_context_from_ran_ue(amf_log_context_t *ctx, ran_ue_t *ran_ue)
{
    if (!ctx)
        return;
    if (!ran_ue)
        return;

    ctx->ran_ue_id = ran_ue->id;
    ctx->ran_ue_ngap_id = ran_ue->ran_ue_ngap_id;
    ctx->amf_ue_ngap_id = ran_ue->amf_ue_ngap_id;
    ctx->gnb_id = ran_ue->gnb_id;
    ctx->amf_ue_id = ran_ue->amf_ue_id;

    if (ran_ue->amf_ue_id >= OGS_MIN_POOL_ID &&
            ran_ue->amf_ue_id <= OGS_MAX_POOL_ID) {
        amf_ue_t *amf_ue = amf_ue_find_by_id(ran_ue->amf_ue_id);
        if (amf_ue) {
            ctx->suci = amf_ue->suci;
            ctx->supi = amf_ue->supi;
        }
    }
}

void amf_log_context_from_sess(amf_log_context_t *ctx, amf_sess_t *sess)
{
    if (!ctx)
        return;
    if (!sess)
        return;

    ctx->sess_id = sess->id;

    if (sess->amf_ue_id >= OGS_MIN_POOL_ID &&
            sess->amf_ue_id <= OGS_MAX_POOL_ID) {
        amf_ue_t *amf_ue = amf_ue_find_by_id(sess->amf_ue_id);
        if (amf_ue)
            amf_log_context_from_ue(ctx, amf_ue);
    }
}
