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
 * GNB info - AMF gNBs JSON dumper for the Prometheus HTTP server (/gnb-info).
 * - gnb_id, plmn, amf, ng, gnb_IP, supported ta_list, slices, num_connceted_ues
 * - pager: /gnb-info?page=0&page_size=100 (0-based, page=-1 without paging) Default: page=0 page_size=100=MAXSIZE
 *
 * path: http://AMF_IP:9090/gnb-info
 * curl -s "http://127.0.0.5:9090/gnb-info?" |jq . 
 * {
 *   "items": [
 *     {
 *       "gnb_id": 100,
 *       "plmn": "99970",
 *       "network": {
 *         "amf_name": "efire-amf0",
 *         "ngap_port": 38412
 *       },
 *       "ng": {
 *         "sctp": {
 *           "peer": "[192.168.168.100]:60110",
 *           "max_out_streams": 2,
 *           "next_ostream_id": 1
 *         },
 *         "setup_success": true
 *       },
 *       "supported_ta_list": [
 *         {
 *           "tac": "000001",
 *           "bplmns": [
 *             {
 *               "plmn": "99970",
 *               "snssai": [
 *                 {
 *                   "sst": 1,
 *                   "sd": "ffffff"
 *                 }
 *               ]
 *             },
 *             {
 *               "plmn": "99971",
 *               "snssai": [
 *                 {
 *                   "sst": 2,
 *                   "sd": "000000"
 *                 }
 *               ]
 *             }
 *           ]
 *         }
 *       ],
 *       "num_connected_ues": 2
 *     }
 *   ],
 *   "pager": {
 *     "page": 0,
 *     "page_size": 100,
 *     "count": 1
 *   }
 * }
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>

#include "ogs-core.h"
#include "ogs-proto.h"
#include "context.h"
#include "gnb-info.h"

#include "sbi/openapi/external/cJSON.h"
#include "metrics/prometheus/pager.h"
#include "metrics/prometheus/json_pager.h"

#ifndef GNB_INFO_PAGE_SIZE_DEFAULT
#define GNB_INFO_PAGE_SIZE_DEFAULT 100U
#endif

/* Pager knobs coming from metrics glue (same approach as your current code) */
static size_t g_gnb_page, g_gnb_page_size;

void amf_metrics_gnb_info_set_pager(size_t page, size_t page_size)
{
    g_gnb_page = page;
    g_gnb_page_size = page_size;
}

size_t amf_dump_gnb_info(char *buf, size_t buflen)
{
    size_t page      = g_gnb_page;
    size_t page_size = g_gnb_page_size ? g_gnb_page_size : GNB_INFO_PAGE_SIZE_DEFAULT;
    if (page_size > GNB_INFO_PAGE_SIZE_DEFAULT) page_size = GNB_INFO_PAGE_SIZE_DEFAULT;
    return amf_dump_gnb_info_paged(buf, buflen, page, page_size);
}

/* --- tiny local helpers (no extra headers) -------------------------------- */

static inline uint32_t u24_to_u32(ogs_uint24_t v)
{
    uint32_t x = 0;
    memcpy(&x, &v, sizeof(v) < sizeof(x) ? sizeof(v) : sizeof(x));
    return (x & 0xFFFFFFu);
}

static inline const char *safe_sa_str(ogs_sockaddr_t *sa)
{
    if (!sa) return "";
    int fam = sa->sa.sa_family;
    if (fam != AF_INET && fam != AF_INET6) return "";
    return ogs_sockaddr_to_string_static(sa);
}

/* --- main dumper (paged) -------------------------------------------------- */

size_t amf_dump_gnb_info_paged(char *buf, size_t buflen, size_t page, size_t page_size)
{
    if (!buf || buflen == 0) return 0;

    const bool no_paging = (page == SIZE_MAX);
    if (!no_paging) {
        if (page_size == 0) page_size = GNB_INFO_PAGE_SIZE_DEFAULT;
        if (page_size > GNB_INFO_PAGE_SIZE_DEFAULT) page_size = GNB_INFO_PAGE_SIZE_DEFAULT;
    } else {
        page_size = SIZE_MAX;
        page = 0;
    }

    const size_t start_index = json_pager_safe_start_index(no_paging, page, page_size);

    amf_context_t *ctxt = amf_self();

    cJSON *root = cJSON_CreateObject();
    if (!root) {
        if (buflen >= 3) { memcpy(buf, "{}", 3); return 2; }
        if (buflen) buf[0] = '\0';
        return 0;
    }

    /* items[] is created AND attached here; null => OOM safe exit */
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    if (!items) {
        cJSON_Delete(root);
        if (buflen >= 3) { memcpy(buf, "{}", 3); return 2; }
        if (buflen) buf[0] = '\0';
        return 0;
    }

    size_t idx = 0, emitted = 0;
    bool has_next = false;
    bool oom = false;

    amf_gnb_t *gnb = NULL;
    ogs_list_for_each(&ctxt->gnb_list, gnb) {
        int adv = json_pager_advance(no_paging, idx, start_index, emitted, page_size, &has_next);
        if (adv == 1) { idx++; continue; }
        if (adv == 2) break;

        cJSON *g = cJSON_CreateObject();
        if (!g) { oom = true; break; }

        /* Attach first to avoid leaks if we bail out later. */
        cJSON_AddItemToArray(items, g);

        /* --- basics --- */
        if (!cJSON_AddNumberToObject(g, "gnb_id", (double)(unsigned)gnb->gnb_id)) { oom = true; break; }

        char plmn_str[OGS_PLMNIDSTRLEN] = {0};
        ogs_plmn_id_to_string(&gnb->plmn_id, plmn_str);
        if (!cJSON_AddStringToObject(g, "plmn", plmn_str)) { oom = true; break; }

        /* --- network --- */
        cJSON *network = cJSON_CreateObject();
        if (!network) { oom = true; break; }
        cJSON_AddItemToObject(g, "network", network);

        if (!cJSON_AddStringToObject(network, "amf_name", ctxt->amf_name ? ctxt->amf_name : "")) { oom = true; break; }
        if (!cJSON_AddNumberToObject(network, "ngap_port", (double)(unsigned)ctxt->ngap_port)) { oom = true; break; }

        /* --- ng + sctp --- */
        cJSON *ng = cJSON_CreateObject();
        cJSON *sctp = cJSON_CreateObject();
        if (!ng || !sctp) {
            if (ng) cJSON_Delete(ng);
            if (sctp) cJSON_Delete(sctp);
            oom = true; break;
        }
        cJSON_AddItemToObject(ng, "sctp", sctp);
        cJSON_AddItemToObject(g, "ng", ng);

        if (!cJSON_AddBoolToObject(ng, "setup_success", gnb->state.ng_setup_success ? 1 : 0)) { oom = true; break; }
        if (!cJSON_AddStringToObject(sctp, "peer", safe_sa_str(gnb->sctp.addr))) { oom = true; break; }
        if (!cJSON_AddNumberToObject(sctp, "max_out_streams", (double)gnb->max_num_of_ostreams)) { oom = true; break; }
        if (!cJSON_AddNumberToObject(sctp, "next_ostream_id", (double)(unsigned)gnb->ostream_id)) { oom = true; break; }

        /* --- supported_ta_list --- */
        cJSON *tas = cJSON_AddArrayToObject(g, "supported_ta_list");
        if (!tas) { oom = true; break; }

        for (int t = 0; t < gnb->num_of_supported_ta_list; t++) {
            const ogs_uint24_t tac = gnb->supported_ta_list[t].tac;

            cJSON *ta = cJSON_CreateObject();
            if (!ta) { oom = true; break; }
            cJSON_AddItemToArray(tas, ta); /* attach early */

            char tac_hex[7];
            snprintf(tac_hex, sizeof tac_hex, "%06x", (unsigned)u24_to_u32(tac));
            if (!cJSON_AddStringToObject(ta, "tac", tac_hex)) { oom = true; break; }

            cJSON *bplmns = cJSON_AddArrayToObject(ta, "bplmns");
            if (!bplmns) { oom = true; break; }

            const int nb = gnb->supported_ta_list[t].num_of_bplmn_list;
            for (int p = 0; p < nb; p++) {
                const ogs_plmn_id_t *bp_plmn = &gnb->supported_ta_list[t].bplmn_list[p].plmn_id;
                const int ns = gnb->supported_ta_list[t].bplmn_list[p].num_of_s_nssai;
                const ogs_s_nssai_t *sn = gnb->supported_ta_list[t].bplmn_list[p].s_nssai;

                cJSON *bp = cJSON_CreateObject();
                if (!bp) { oom = true; break; }
                cJSON_AddItemToArray(bplmns, bp); /* attach early */

                char bp_plmn_str[OGS_PLMNIDSTRLEN] = {0};
                ogs_plmn_id_to_string(bp_plmn, bp_plmn_str);
                if (!cJSON_AddStringToObject(bp, "plmn", bp_plmn_str)) { oom = true; break; }

                cJSON *sns = cJSON_AddArrayToObject(bp, "snssai");
                if (!sns) { oom = true; break; }

                for (int i = 0; i < ns; i++) {
                    cJSON *o = cJSON_CreateObject();
                    if (!o) { oom = true; break; }
                    cJSON_AddItemToArray(sns, o); /* attach early */

                    if (!cJSON_AddNumberToObject(o, "sst", (double)(unsigned)sn[i].sst)) { oom = true; break; }
                    char sd[7]; snprintf(sd, sizeof sd, "%06x", (unsigned)u24_to_u32(sn[i].sd));
                    if (!cJSON_AddStringToObject(o, "sd", sd)) { oom = true; break; }
                }
                if (oom) break;
            }
            if (oom) break;
        }
        if (oom) break;

        /* --- UE counter --- */
        /* count_connected_ues_for_gnb walks list; no allocations here */
        size_t num = 0; ran_ue_t *r = NULL;
        ogs_list_for_each(&((amf_gnb_t*)gnb)->ran_ue_list, r) num++;
        if (!cJSON_AddNumberToObject(g, "num_connected_ues", (double)num)) { oom = true; break; }

        emitted++;
        idx++;
    }

    /* pager + finalize (pager can include OOM hint) */
    json_pager_add_trailing(root, no_paging, page, page_size, emitted, has_next && !oom,
                            "/gnb-info", oom);

    return json_pager_finalize(root, buf, buflen);
}

