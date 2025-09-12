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
 * JSON dumper for /connected-gnbs (AMF)
 * Output (one item per connected gNB):
 * [
 *   {
 *     "gnb_id": 100,
 *     "plmn": "99970",
 *     "network": {
 *       "amf_name": "efire-amf0",
 *       "ngap_port": 38412
 *     },
 *     "ng": {
 *       "setup_success": true,
 *       "sctp": {
 *         "peer": "[192.168.168.100]:60110",
 *         "max_out_streams": 2,
 *         "next_ostream_id": 1
 *       }
 *     },
 *     "supported_ta_list": [
 *       {
 *         "tac": "000001",
 *         "bplmns": [
 *           {
 *             "plmn": "99970",
 *             "snssai": [
 *               {
 *                 "sst": 1,
 *                 "sd": "ffffff"
 *               }
 *             ]
 *           }
 *         ]
 *       }
 *     ],
 *     "num_connected_ues": 1
 *   }
 * ]
 */

/*
 * Copyright (C) 2025 by Juraj Elias
 * JSON dumper for /connected-gnbs (AMF)
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/socket.h>

#include "context.h"
#include "ogs-proto.h"
#include "ogs-core.h"

/* Exported */
size_t amf_dump_connected_gnbs(char *buf, size_t buflen);

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

static size_t append_json_kv_escaped(char *buf, size_t off, size_t buflen,
                                     const char *key, const char *val)
{
    if (!val) val = "";
    off = append_safe(buf, off, buflen, "\"%s\":\"", key);
    if (off == (size_t)-1) return off;
    for (const unsigned char *p = (const unsigned char *)val; *p; ++p) {
        unsigned char c = *p;
        if (c == '\\' || c == '\"') off = append_safe(buf, off, buflen, "\\%c", c);
        else if (c < 0x20)          off = append_safe(buf, off, buflen, "\\u%04x", (unsigned)c);
        else                        off = append_safe(buf, off, buflen, "%c", c);
        if (off == (size_t)-1) return off;
    }
    return append_safe(buf, off, buflen, "\"");
}

/* "plmn":"XXXXX" */
static size_t append_plmn_kv(char *buf, size_t off, size_t buflen, const ogs_plmn_id_t *plmn)
{
    char s[OGS_PLMNIDSTRLEN] = {0};
    ogs_plmn_id_to_string(plmn, s);
    return append_safe(buf, off, buflen, "\"plmn\":\"%s\"", s);
}

/* 24-bit helpers */
static inline uint32_t u24_to_u32_portable(ogs_uint24_t v)
{
    uint32_t x = 0; memcpy(&x, &v, sizeof(v) < sizeof(x) ? sizeof(v) : sizeof(x));
    return (x & 0xFFFFFFu);
}

static size_t append_u24_hex6(char *buf, size_t off, size_t buflen, const ogs_uint24_t v)
{
    uint32_t u = u24_to_u32_portable(v);
    return append_safe(buf, off, buflen, "\"%06x\"", (unsigned)(u & 0xFFFFFFu));
}

/* S-NSSAI */
static size_t append_snssai_obj(char *buf, size_t off, size_t buflen, const ogs_s_nssai_t *sn)
{
    unsigned sst = (unsigned)sn->sst;
    uint32_t sd_u32 = u24_to_u32_portable(sn->sd);
    off = append_safe(buf, off, buflen, "{");
    off = append_safe(buf, off, buflen, "\"sst\":%u", sst);
    off = append_safe(buf, off, buflen, ",\"sd\":\"%06x\"}", (unsigned)(sd_u32 & 0xFFFFFFu));
    return off;
}

static size_t append_snssai_arr(char *buf, size_t off, size_t buflen,
                                const ogs_s_nssai_t *arr, int n)
{
    off = append_safe(buf, off, buflen, "[");
    for (int i = 0; i < n; i++) {
        if (i) off = append_safe(buf, off, buflen, ",");
        off = append_snssai_obj(buf, off, buflen, &arr[i]);
    }
    off = append_safe(buf, off, buflen, "]");
    return off;
}

/* sockaddr -> string */
static inline const char *safe_sa_str(const ogs_sockaddr_t *sa)
{
    if (!sa) return "";
    int fam = ((const struct sockaddr *)&sa->sa)->sa_family;
    if (fam != AF_INET && fam != AF_INET6) return "";
    return ogs_sockaddr_to_string_static((ogs_sockaddr_t *)sa);
}

/* UE counter on this gNB */
static size_t count_connected_ues_for_gnb(const amf_gnb_t *gnb)
{
    size_t total = 0; ran_ue_t *r = NULL;
    ogs_list_for_each(&((amf_gnb_t*)gnb)->ran_ue_list, r) total++;
    return total;
}

#define APPF(...)  do { off = append_safe(buf, off, buflen, __VA_ARGS__); if (off==(size_t)-1) goto trunc; } while(0)
#define APPX(expr) do { off = (expr); if (off==(size_t)-1) goto trunc; } while(0)

/* --------------------------- main --------------------------- */

size_t amf_dump_connected_gnbs(char *buf, size_t buflen)
{
    size_t off = 0;
    amf_context_t *ctxt = amf_self();
    ogs_assert(ctxt);
    amf_gnb_t *gnb = NULL;

    APPF("[");
    bool first_gnb = true;

    ogs_list_for_each(&ctxt->gnb_list, gnb) {
        if (!first_gnb) APPF(",");
        first_gnb = false;

        size_t num_total = count_connected_ues_for_gnb(gnb);

        APPF("{");

        APPF("\"gnb_id\":%u", (unsigned)gnb->gnb_id);
        APPF(",");
        APPX(append_plmn_kv(buf, off, buflen, &gnb->plmn_id));

        APPF(",\"network\":{");
        APPX(append_json_kv_escaped(buf, off, buflen, "amf_name", ctxt->amf_name ? ctxt->amf_name : ""));
        APPF(",\"ngap_port\":%u", (unsigned)ctxt->ngap_port);
        APPF("}");

        APPF(",\"ng\":{");
        APPF("\"setup_success\":%s", gnb->state.ng_setup_success ? "true" : "false");
        APPF(",\"sctp\":{");
        APPF("\"peer\":\"%s\"", safe_sa_str(gnb->sctp.addr));
        APPF(",\"max_out_streams\":%d", gnb->max_num_of_ostreams);
        APPF(",\"next_ostream_id\":%u", (unsigned)gnb->ostream_id);
        APPF("}");
        APPF("}");

        APPF(",\"supported_ta_list\":[");
        for (int t = 0; t < gnb->num_of_supported_ta_list; t++) {
            if (t) APPF(",");
            APPF("{");
            APPF("\"tac\":");
            APPX(append_u24_hex6(buf, off, buflen, gnb->supported_ta_list[t].tac));

            APPF(",\"bplmns\":[");
            for (int p = 0; p < gnb->supported_ta_list[t].num_of_bplmn_list; p++) {
                if (p) APPF(",");
                const ogs_plmn_id_t *bp_plmn = &gnb->supported_ta_list[t].bplmn_list[p].plmn_id;
                const int ns = gnb->supported_ta_list[t].bplmn_list[p].num_of_s_nssai;
                const ogs_s_nssai_t *sn = gnb->supported_ta_list[t].bplmn_list[p].s_nssai;

                APPF("{");
                APPX(append_plmn_kv(buf, off, buflen, bp_plmn));
                APPF(",\"snssai\":");
                APPX(append_snssai_arr(buf, off, buflen, sn, ns));
                APPF("}");
            }
            APPF("]");
            APPF("}");
        }
        APPF("]");

        APPF(",\"num_connected_ues\":%zu", num_total);

        APPF("}");
    }

    APPF("]");
    return off;

trunc:
    if (buf && buflen >= 3) { buf[0]='['; buf[1]=']'; buf[2]='\0'; return 2; }
    if (buf && buflen) buf[0]='\0';
    return 0;
}

