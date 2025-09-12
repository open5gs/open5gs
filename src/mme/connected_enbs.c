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
 * JSON dumper for /connected-enbs (MME)
 * Output (one item per connected eNB):
 *[
 *  {
 *    "enb_id": 264040,
 *    "plmn": "99970",
 *    "network": {
 *      "mme_name": "efire-mme0"
 *    },
 *    "s1": {
 *      "setup_success": true,
 *      "sctp": {
 *        "peer": "[192.168.168.254]:36412",
 *        "max_out_streams": 10,
 *        "next_ostream_id": 1
 *      }
 *    },
 *    "supported_ta_list": [
 *      {
 *        "tac": "000001",
 *        "plmn": "99970"
 *      }
 *    ],
 *    "num_connected_ues": 1
 *  }
 *]
 */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#include "ogs-core.h"
#include "ogs-proto.h"
#include "ogs-app.h"
#include "mme-context.h"

/* Exported */
size_t mme_dump_connected_enbs(char *buf, size_t buflen);

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

static size_t append_u24_hex6_str(char *buf, size_t off, size_t buflen, uint32_t v24)
{
    return append_safe(buf, off, buflen, "\"%06X\"", (unsigned)(v24 & 0xFFFFFF));
}

static inline const char *safe_sa_str(const ogs_sockaddr_t *sa)
{
    if (!sa) return "";
    int fam = ((const struct sockaddr *)&sa->sa)->sa_family;
    if (fam != AF_INET && fam != AF_INET6) return "";
    return ogs_sockaddr_to_string_static((ogs_sockaddr_t *)sa);
}

#define APPF(...)  do { off = append_safe(buf, off, buflen, __VA_ARGS__); if (off==(size_t)-1) goto trunc; } while(0)
#define APPX(expr) do { off = (expr); if (off==(size_t)-1) goto trunc; } while(0)

/* ------------------------------- main ------------------------------- */

size_t mme_dump_connected_enbs(char *buf, size_t buflen)
{
    size_t off = 0;
    if (!buf || buflen == 0) return 0;

    mme_context_t *ctxt = mme_self();
    if (!ctxt) {
        APPF("[]");
        return off;
    }

    APPF("[");
    bool first = true;

    mme_enb_t *enb = NULL;
    ogs_list_for_each(&ctxt->enb_list, enb) {
        if (!first) APPF(",");
        first = false;

        size_t num_connected_ues = 0;
        {
            enb_ue_t *ue = NULL;
            ogs_list_for_each(&enb->enb_ue_list, ue) num_connected_ues++;
        }

        APPF("{");

        APPF("\"enb_id\":%u", (unsigned)enb->enb_id);
        APPF(",");
        APPX(append_plmn_kv(buf, off, buflen, &enb->plmn_id));

        APPF(",\"network\":{");
        APPX(append_json_kv_escaped(buf, off, buflen, "mme_name",
                                    ctxt->mme_name ? ctxt->mme_name : ""));
        APPF("}");

        APPF(",\"s1\":{");
        APPF("\"setup_success\":%s", enb->state.s1_setup_success ? "true" : "false");
        APPF(",\"sctp\":{");
        APPF("\"peer\":\"%s\"", safe_sa_str(enb->sctp.addr));
        APPF(",\"max_out_streams\":%d", enb->max_num_of_ostreams);
        APPF(",\"next_ostream_id\":%u", (unsigned)enb->ostream_id);
        APPF("}");
        APPF("}");

        APPF(",\"supported_ta_list\":[");
        for (int t = 0; t < enb->num_of_supported_ta_list; t++) {
            if (t) APPF(",");
            APPF("{");
            APPF("\"tac\":");
            APPX(append_u24_hex6_str(buf, off, buflen, enb->supported_ta_list[t].tac));
            APPF(",\"plmn\":");
            APPX(append_plmn_kv(buf, off, buflen, &enb->supported_ta_list[t].plmn_id));
            APPF("}");
        }
        APPF("]");

        APPF(",\"num_connected_ues\":%zu", num_connected_ues);

        APPF("}");
    }

    APPF("]");
    return off;

trunc:
    if (buf && buflen >= 3) { buf[0]='['; buf[1]=']'; buf[2]='\0'; return 2; }
    if (buf && buflen) buf[0]='\0';
    return 0;
}

