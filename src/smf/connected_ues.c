/*
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

/*
 * JSON dumper for /connected-gnbs (AMF)
 * Output (one item per connected gNB):
 * [
 *   {
 *     "supi": "imsi-999700000083810",
 *     "pdu": [
 *       {
 *         "psi": 1,
 *         "dnn": "internet",
 *         "ipv4": "10.45.0.2",
 *         "snssai": {
 *           "sst": 1,
 *           "sd": "ffffff"
 *         },
 *         "qos_flows": [
 *           {
 *             "qfi": 1,
 *             "5qi": 9
 *           }
 *         ],
 *         "pdu_state": "inactive"
 *       }
 *     ],
 *     "ue_activity": "idle"
 *   },
 * ]
 */
/*
 * Copyright (C) 2025 by Juraj Elias <juraj.elias@gmail.com>
 * This file is part of Open5GS (AGPLv3+)
 *
 * JSON dumper for /connected-ues (SMF)
 * - 5G PDUs:  psi+dnn, snssai, qos_flows [{qfi,5qi}], pdu_state
 * - LTE PDUs: ebi(+psi if non-zero)+apn, qos_flows [{ebi,qci}], pdu_state="unknown"
 * - UE-level: ue_activity derived from PDU states
 */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "ogs-core.h"      /* OGS_INET_NTOP, OGS_INET6_NTOP, OGS_ADDRSTRLEN, ogs_uint24_t */
#include "context.h"       /* smf_self(), smf_ue_t, smf_sess_t, smf_bearer_t, ogs_s_nssai_t */
#include "connected_ues.h" /* size_t smf_dump_connected_ues(char *buf, size_t buflen) */

/* ------------------------- small helpers ------------------------- */

static inline size_t append_safe(char *buf, size_t off, size_t buflen, const char *fmt, ...)
{
    if (!buf || off == (size_t)-1 || off >= buflen) return (size_t)-1;
    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(buf + off, buflen - off, fmt, ap);
    va_end(ap);
    if (n < 0 || (size_t)n >= buflen - off) return (size_t)-1;
    return off + (size_t)n;
}

/* Escapes \" \\ and control chars, emits: "key":"escaped" */
static size_t append_json_kv_escaped(char *buf, size_t off, size_t buflen,
                                     const char *key, const char *val)
{
    if (!val) val = "";
    off = append_safe(buf, off, buflen, "\"%s\":\"", key);
    if (off == (size_t)-1) return off;

    for (const unsigned char *p = (const unsigned char *)val; *p; ++p) {
        unsigned char c = *p;
        if (c == '\\' || c == '\"') {
            off = append_safe(buf, off, buflen, "\\%c", c);
        } else if (c < 0x20) {
            off = append_safe(buf, off, buflen, "\\u%04x", (unsigned)c);
        } else {
            off = append_safe(buf, off, buflen, "%c", c);
        }
        if (off == (size_t)-1) return off;
    }
    return append_safe(buf, off, buflen, "\"");
}

/* 24-bit helpers */
static inline uint32_t u24_to_u32_portable(ogs_uint24_t v)
{
    uint32_t x = 0;
    memcpy(&x, &v, sizeof(v) < sizeof(x) ? sizeof(v) : sizeof(x));
    return (x & 0xFFFFFFu);
}

/* Emit a S-NSSAI object */
static size_t append_snssai_obj(char *buf, size_t off, size_t buflen, const ogs_s_nssai_t *sn)
{
    unsigned sst = (unsigned)sn->sst;
    uint32_t sd_u32 = u24_to_u32_portable(sn->sd);
    off = append_safe(buf, off, buflen, "{");
    off = append_safe(buf, off, buflen, "\"sst\":%u", sst);
    off = append_safe(buf, off, buflen, ",\"sd\":\"%06x\"}", (unsigned)(sd_u32 & 0xFFFFFFu));
    return off;
}

/* ------------------------- state helpers ------------------------- */

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

/* Looks-5G heuristic: S-NSSAI present or any QFI bearer */
static inline bool looks_5g_sess(const smf_sess_t *s) {
    if (!s) return false;
    if (s->s_nssai.sst != 0) return true;
    if (u24_to_u32_portable(s->s_nssai.sd) != 0) return true;
    if (bearer_list_has_qfi(s)) return true;
    return false;
}

/* 5G PDU state */
static const char *pdu_state_from_5g(const smf_sess_t *sess)
{
    if (!sess) return "unknown";
    if ((int)sess->resource_status == (int)OpenAPI_resource_status_RELEASED)
        return "inactive";
    if (up_state_of(sess) == (int)OpenAPI_up_cnx_state_DEACTIVATED)
        return "inactive";
    if (sess->n1_released || sess->n2_released)
        return "inactive";
    if (!has_n3_teid(sess))
        return "inactive";
    return "active";
}

/* LTE/EPC PDU state at SMF scope: unknown */
static const char *pdu_state_from_4g(const smf_sess_t *sess)
{
    (void)sess;
    return "unknown";
}

/* QoS emitters */
static size_t append_qos_info_5g(char *buf, size_t off, size_t buflen, const smf_sess_t *sess)
{
    smf_bearer_t *b = NULL;
    bool first = true;
    off = append_safe(buf, off, buflen, ",\"qos_flows\":[");
    ogs_list_for_each(&((smf_sess_t *)sess)->bearer_list, b) {
        if (!b || b->qfi == 0) continue;
        if (!first) off = append_safe(buf, off, buflen, ",");
        first = false;
        off = append_safe(buf, off, buflen, "{");
        off = append_safe(buf, off, buflen, "\"qfi\":%u", (unsigned)b->qfi);
        if (b->qos.index > 0)
            off = append_safe(buf, off, buflen, ",\"5qi\":%u", (unsigned)b->qos.index);
        off = append_safe(buf, off, buflen, "}");
        if (off == (size_t)-1) break;
    }
    off = append_safe(buf, off, buflen, "]");
    return off;
}

static size_t append_qos_info_4g(char *buf, size_t off, size_t buflen, const smf_sess_t *sess)
{
    smf_bearer_t *b = NULL;
    bool first = true;
    off = append_safe(buf, off, buflen, ",\"qos_flows\":[");
    ogs_list_for_each(&((smf_sess_t *)sess)->bearer_list, b) {
        if (!b || b->ebi == 0) continue;
        if (!first) off = append_safe(buf, off, buflen, ",");
        first = false;

        unsigned qci_val = (unsigned)b->qos.index;
        if (qci_val == 0 && sess) qci_val = (unsigned)sess->session.qos.index;

        off = append_safe(buf, off, buflen, "{");
        off = append_safe(buf, off, buflen, "\"ebi\":%u", (unsigned)b->ebi);
        if (qci_val > 0)
            off = append_safe(buf, off, buflen, ",\"qci\":%u", qci_val);
        off = append_safe(buf, off, buflen, "}");
        if (off == (size_t)-1) break;
    }
    off = append_safe(buf, off, buflen, "]");
    return off;
}

/* Macros for safe appends */
#define APPF(...)  do { off = append_safe(buf, off, buflen, __VA_ARGS__); if (off==(size_t)-1) goto trunc; } while(0)
#define APPX(expr) do { off = (expr); if (off==(size_t)-1) goto trunc; } while(0)

/* ------------------------------- main ------------------------------- */

size_t smf_dump_connected_ues(char *buf, size_t buflen)
{
    size_t off = 0;
    if (!buf || buflen == 0) return 0;

    APPF("[");
    bool first_ue = true;

    smf_ue_t *ue = NULL;
    ogs_list_for_each(&smf_self()->smf_ue_list, ue) {
        if (!ue) continue;

        if (!first_ue) APPF(",");
        first_ue = false;

        bool any_active = false, any_unknown = false;

        APPF("{");

        /* UE identity */
        const char *id = (ue->supi && ue->supi[0]) ? ue->supi :
                         (ue->imsi_bcd[0] ? ue->imsi_bcd : "");
        APPX(append_json_kv_escaped(buf, off, buflen, "supi", id));

        /* PDU array */
        APPF(",\"pdu\":[");
        bool first_pdu = true;

        smf_sess_t *sess = NULL;
        ogs_list_for_each(&ue->sess_list, sess) {
            if (!sess) continue;
            const bool is_5g = looks_5g_sess(sess);
            const char *pstate = is_5g ? pdu_state_from_5g(sess)
                                       : pdu_state_from_4g(sess);

            if (!first_pdu) APPF(",");
            first_pdu = false;

            APPF("{");

            if (is_5g) {
                /* 5G: PSI + DNN */
                APPF("\"psi\":%u,", (unsigned)sess->psi);
                APPX(append_json_kv_escaped(buf, off, buflen, "dnn",
                    (sess->session.name ? sess->session.name : "")));
            } else {
                /* LTE: PSI if non-zero, EBI root + APN */
                unsigned ebi_root = 0;
                smf_bearer_t *b0 = NULL;
                ogs_list_for_each(&((smf_sess_t *)sess)->bearer_list, b0) {
                    if (b0 && b0->ebi > 0) { ebi_root = (unsigned)b0->ebi; break; }
                }
                if (sess->psi > 0) APPF("\"psi\":%u,", (unsigned)sess->psi);
                APPF("\"ebi\":%u,", ebi_root);
                APPX(append_json_kv_escaped(buf, off, buflen, "apn",
                    (sess->session.name ? sess->session.name : "")));
            }

            /* IPs if present */
            char ip4[OGS_ADDRSTRLEN] = "";
            char ip6[OGS_ADDRSTRLEN] = "";
            if (sess->ipv4) OGS_INET_NTOP(&sess->ipv4->addr, ip4);
            if (sess->ipv6) OGS_INET6_NTOP(&sess->ipv6->addr, ip6);
            if (ip4[0]) { APPF(","); APPX(append_json_kv_escaped(buf, off, buflen, "ipv4", ip4)); }
            if (ip6[0]) { APPF(","); APPX(append_json_kv_escaped(buf, off, buflen, "ipv6", ip6)); }

            if (is_5g) {
                /* S-NSSAI */
                APPF(",\"snssai\":");
                APPX(append_snssai_obj(buf, off, buflen, &sess->s_nssai));
                /* QoS flows */
                APPX(append_qos_info_5g(buf, off, buflen, sess));
            } else {
                /* LTE QoS */
                APPX(append_qos_info_4g(buf, off, buflen, sess));
            }

            APPF(",\"pdu_state\":\"%s\"", pstate);
            APPF("}");

            if (strcmp(pstate, "active") == 0) any_active = true;
            else if (strcmp(pstate, "unknown") == 0) any_unknown = true;
        }
        APPF("]");

        const char *ue_act = any_active ? "active" : (any_unknown ? "unknown" : "idle");
        APPF(",\"ue_activity\":\"%s\"", ue_act);

        APPF("}");
    }

    APPF("]");
    return off;

trunc:
    /* Minimal valid JSON on overflow */
    if (buf && buflen >= 3) { buf[0]='['; buf[1]=']'; buf[2]='\0'; return 2; }
    if (buf && buflen) buf[0]='\0';
    return 0;
}

