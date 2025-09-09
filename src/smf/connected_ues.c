/*
 * Copyright (C) 2019-2025 by Sukchan Lee <acetcom@gmail.com>
 * Copyright (C) 2025 by Juraj Elias <juraj.elias@gmail.com>
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

/*
 * Connected UEs JSON dumper for the Prometheus HTTP server (/connected-ues).
 * - 5G PDUs:  psi+dnn, snssai, qos_flows [{qfi,5qi}], pdu_state ("active"/"inactive"/"unknown")
 * - LTE PDUs: ebi(+psi if non-zero)+apn, qos_flows [{ebi,qci}], pdu_state ("unknown" at SMF scope)
 * - UE-level: ue_activity ("active" if any PDU active; "unknown" if none active but any unknown; else "idle")
 */

#include "ogs-core.h"          /* ogs_time_now, OGS_INET_NTOP, OGS_INET6_NTOP, OGS_ADDRSTRLEN, ogs_uint24_t */
#include "context.h"           /* smf_self(), smf_ue_t, smf_sess_t, smf_bearer_t, ogs_s_nssai_t */
#include "connected_ues.h"     /* size_t smf_dump_connected_ues(char *buf, size_t buflen) */

#include <stdarg.h>
#include <string.h>
#include <stdbool.h>

#define JSON_NEAR_END_GUARD 256

/* ---------- small safe helpers ---------- */

static inline size_t json_append(char *buf, size_t off, size_t buflen,
                                 const char *fmt, ...)
{
    if (!buf || buflen == 0) return 0;
    if (off >= buflen) return buflen - 1;

    size_t rem = buflen - off;
    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(buf + off, rem, fmt, ap);
    va_end(ap);

    if (n < 0) return off;
    if ((size_t)n >= rem) return buflen - 1;
    return off + (size_t)n;
}

static size_t json_append_escaped_string(char *buf, size_t off, size_t buflen,
                                         const char *key, const char *val)
{
    if (!val) val = "";
    off = json_append(buf, off, buflen, "\"%s\":\"", key);
    for (const unsigned char *p = (const unsigned char *)val; *p; ++p) {
        unsigned char c = *p;
        switch (c) {
        case '\\': off = json_append(buf, off, buflen, "\\\\"); break;
        case '\"': off = json_append(buf, off, buflen, "\\\""); break;
        case '\b': off = json_append(buf, off, buflen, "\\b");  break;
        case '\f': off = json_append(buf, off, buflen, "\\f");  break;
        case '\n': off = json_append(buf, off, buflen, "\\n");  break;
        case '\r': off = json_append(buf, off, buflen, "\\r");  break;
        case '\t': off = json_append(buf, off, buflen, "\\t");  break;
        default:
            if (c < 0x20) off = json_append(buf, off, buflen, "\\u%04x", (unsigned)c);
            else          off = json_append(buf, off, buflen, "%c", c);
        }
    }
    off = json_append(buf, off, buflen, "\"");
    return off;
}

static inline uint32_t u24_to_u32_portable(ogs_uint24_t v)
{
    uint32_t x = 0;
    memcpy(&x, &v, sizeof(v) < sizeof(x) ? sizeof(v) : sizeof(x));
    return (x & 0xFFFFFFu);
}

static inline int up_state_of(const smf_sess_t *s) {
    if (!s) return 0;
    int u = (int)s->up_cnx_state;
    if (u == 0) u = (int)s->nsmf_param.up_cnx_state;
    return u;
}

static inline bool has_n3_teid(const smf_sess_t *s) {
    return s && (s->remote_ul_teid != 0U || s->remote_dl_teid != 0U);
}

static inline bool bearer_list_has_qfi(const smf_sess_t *s) {
    if (!s) return false;
    smf_bearer_t *b = NULL;
    ogs_list_for_each(&((smf_sess_t *)s)->bearer_list, b) {
        if (b && b->qfi > 0) return true;
    }
    return false;
}

/* Looks-5G heuristic: S-NSSAI present (sst != 0 or sd != 0) or any QFI bearer */
static inline bool looks_5g_sess(const smf_sess_t *s) {
    if (!s) return false;
    if (s->s_nssai.sst != 0) return true;
    if (u24_to_u32_portable(s->s_nssai.sd) != 0) return true;
    if (bearer_list_has_qfi(s)) return true;
    return false;
}

/* ---------- PDU state calculators ---------- */

/* 5G PDU state: conservative to avoid false 'active' during idle */
static const char *pdu_state_from_5g(const smf_sess_t *sess)
{
    if (!sess) return "unknown";

    if ((int)sess->resource_status == (int)OpenAPI_resource_status_RELEASED)
        return "inactive";

    /* Explicit 'deactivated' => inactive */
    if (up_state_of(sess) == (int)OpenAPI_up_cnx_state_DEACTIVATED)
        return "inactive";

    /* control plane down => inactive */
    if (sess->n1_released || sess->n2_released)
        return "inactive";

    /* no N3 user-plane binding => inactive */
    if (!has_n3_teid(sess))
        return "inactive";

    return "active";
}

/*
 * LTE/EPC PDU state:
 * SMF does not own/track S1-U (eNB<->SGW-U) TEIDs
 * Therefore, at SMF scope the LTE PDU state is not reliably derivable.
 */
static const char *pdu_state_from_4g(const smf_sess_t *sess)
{
    (void)sess;
    return "unknown";
}

/* ---------- JSON field emitters ---------- */

static size_t append_snssai(char *buf, size_t off, size_t buflen, const ogs_s_nssai_t *snssai)
{
    if (!snssai)
        return json_append(buf, off, buflen, "\"snssai\":{}");

    unsigned sst = (unsigned)snssai->sst;
    uint32_t sd_u32 = u24_to_u32_portable(snssai->sd);

    off = json_append(buf, off, buflen, "\"snssai\":{");
    off = json_append(buf, off, buflen, "\"sst\":%u,", sst);
    off = json_append(buf, off, buflen, "\"sd\":\"%06x\"}", (unsigned)(sd_u32 & 0xFFFFFFu));
    return off;
}

/* QoS (5G -> {qfi,5qi}) */
static size_t append_qos_info_5g(char *buf, size_t off, size_t buflen, const smf_sess_t *sess)
{
    smf_bearer_t *b = NULL;
    int first = 1;

    off = json_append(buf, off, buflen, ",\"qos_flows\":[");
    ogs_list_for_each(&((smf_sess_t *)sess)->bearer_list, b) {
        if (!b || b->qfi == 0) continue;

        if (!first) off = json_append(buf, off, buflen, ",");
        first = 0;

        off = json_append(buf, off, buflen, "{");
        off = json_append(buf, off, buflen, "\"qfi\":%u", (unsigned)b->qfi);
        if (b->qos.index > 0)
            off = json_append(buf, off, buflen, ",\"5qi\":%u", (unsigned)b->qos.index);
        off = json_append(buf, off, buflen, "}");

        if (off >= buflen - JSON_NEAR_END_GUARD) break;
    }
    off = json_append(buf, off, buflen, "]");
    return off;
}

/* QoS (LTE -> {ebi,qci}); includes fallback to session-level QCI if bearer-level is 0 */
static size_t append_qos_info_4g(char *buf, size_t off, size_t buflen, const smf_sess_t *sess)
{
    smf_bearer_t *b = NULL;
    int first = 1;

    off = json_append(buf, off, buflen, ",\"qos_flows\":[");
    ogs_list_for_each(&((smf_sess_t *)sess)->bearer_list, b) {
        if (!b || b->ebi == 0) continue;

        if (!first) off = json_append(buf, off, buflen, ",");
        first = 0;

        unsigned qci_val = (unsigned)b->qos.index;
        if (qci_val == 0 && sess)
            qci_val = (unsigned)sess->session.qos.index; 

        off = json_append(buf, off, buflen, "{");
        off = json_append(buf, off, buflen, "\"ebi\":%u", (unsigned)b->ebi);
        if (qci_val > 0)
            off = json_append(buf, off, buflen, ",\"qci\":%u", qci_val);
        off = json_append(buf, off, buflen, "}");

        if (off >= buflen - JSON_NEAR_END_GUARD) break;
    }
    off = json_append(buf, off, buflen, "]");
    return off;
}

/* ---------- main dump ---------- */

size_t smf_dump_connected_ues(char *buf, size_t buflen)
{
    if (!buf || buflen < 3) return 0; /* need at least "[]" */
    buf[0] = '\0';

    size_t off = 0;
    off = json_append(buf, off, buflen, "[");
    int first_ue = 1;

    smf_ue_t *ue = NULL;
    ogs_list_for_each(&smf_self()->smf_ue_list, ue) {
        if (!ue) continue;

        if (!first_ue) off = json_append(buf, off, buflen, ",");
        first_ue = 0;

        off = json_append(buf, off, buflen, "{");

        /* UE identity (escaped) */
        const char *id = (ue->supi && ue->supi[0]) ? ue->supi :
                         (ue->imsi_bcd[0] ? ue->imsi_bcd : "");
        off = json_append_escaped_string(buf, off, buflen, "supi", id);
        off = json_append(buf, off, buflen, ",");

        /* PDU sessions array */
        off = json_append(buf, off, buflen, "\"pdu\":[");
        int first_pdu = 1;

        /* UE-level activity aggregation from PDU state */
        bool any_active  = false;
        bool any_unknown = false;

        smf_sess_t *sess = NULL;
        ogs_list_for_each(&ue->sess_list, sess) {
            if (!sess) continue;

            const bool is_5g = looks_5g_sess(sess);
            const char *pstate = is_5g ? pdu_state_from_5g(sess)
                                       : pdu_state_from_4g(sess);

            if (!first_pdu) off = json_append(buf, off, buflen, ",");
            first_pdu = 0;

            off = json_append(buf, off, buflen, "{");

            if (is_5g) {
                /* 5G: PSI + DNN */
                off = json_append(buf, off, buflen, "\"psi\":%u,", (unsigned)sess->psi);
                off = json_append_escaped_string(buf, off, buflen, "dnn",
                            (sess->session.name ? sess->session.name : ""));
            } else {
                /* LTE: include PSI if non-zero, EBI + APN */
                unsigned ebi_root = 0;
                smf_bearer_t *b0 = NULL;
                ogs_list_for_each(&((smf_sess_t *)sess)->bearer_list, b0) {
                    if (b0 && b0->ebi > 0) { ebi_root = (unsigned)b0->ebi; break; }
                }
                if (sess->psi > 0)
                    off = json_append(buf, off, buflen, "\"psi\":%u,", (unsigned)sess->psi);
                off = json_append(buf, off, buflen, "\"ebi\":%u,", ebi_root);
                off = json_append_escaped_string(buf, off, buflen, "apn",
                            (sess->session.name ? sess->session.name : ""));
            }

            /* IPs if present */
            char ip4[OGS_ADDRSTRLEN] = "";
            char ip6[OGS_ADDRSTRLEN] = "";
            if (sess->ipv4) OGS_INET_NTOP(&sess->ipv4->addr, ip4);
            if (sess->ipv6) OGS_INET6_NTOP(&sess->ipv6->addr, ip6);
            if (ip4[0]) { off = json_append(buf, off, buflen, ","); off = json_append_escaped_string(buf, off, buflen, "ipv4", ip4); }
            if (ip6[0]) { off = json_append(buf, off, buflen, ","); off = json_append_escaped_string(buf, off, buflen, "ipv6", ip6); }

            if (is_5g) {
                /* S-NSSAI (5G only) */
                off = json_append(buf, off, buflen, ",");
                off = append_snssai(buf, off, buflen, &sess->s_nssai);

                /* QoS flows (5G) */
                off = append_qos_info_5g(buf, off, buflen, sess);
            } else {
                /* QoS flows (LTE, with QCI restored + fallback) */
                off = append_qos_info_4g(buf, off, buflen, sess);
            }

            /* Single string pdu_state */
            off = json_append(buf, off, buflen, ",\"pdu_state\":\"%s\"", pstate);

            /* end PDU object */
            off = json_append(buf, off, buflen, "}");

            if (strcmp(pstate, "active") == 0)
                any_active = true;
            else if (strcmp(pstate, "unknown") == 0)
                any_unknown = true;

            if (off >= buflen - JSON_NEAR_END_GUARD) break;
        }
        off = json_append(buf, off, buflen, "]");

        /* UE activity from PDUs:
           - "active" if any active
           - else "unknown" if none active but some unknown
           - else "idle" */
        const char *ue_act = any_active ? "active" : (any_unknown ? "unknown" : "idle");
        off = json_append(buf, off, buflen, ",\"ue_activity\":\"%s\"", ue_act);

        off = json_append(buf, off, buflen, "}");

        if (off >= buflen - JSON_NEAR_END_GUARD) break;
    }

    off = json_append(buf, off, buflen, "]");
    return off;
}

