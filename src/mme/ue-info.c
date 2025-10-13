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
 *
 * Connected MME UEs (LTE) JSON dumper for the Prometheus HTTP server (/ue-info).
 * - supi, cm_state, enb_info, location, ambr, slices, pdn_count
 * - pager: /ue-info?page=0&page_size=100 (0-based, page=-1 without paging) Default: page=0 page_size=100=MAXSIZE
 *
 * path: http://MME_IP:9090/ue-info
 * 
 * curl -s "http://127.0.0.2:9090/ue-info?" |jq . 
 * {
 *   "items": [
 *     {
 *       "supi": "999700000021632",
 *       "domain": "EPS",
 *       "rat": "E-UTRA",
 *       "cm_state": "connected",
 *       "enb": {
 *         "ostream_id": 3,
 *         "mme_ue_ngap_id": 3,
 *         "ran_ue_ngap_id": 9,
 *         "enb_id": 264040,
 *         "cell_id": 67594275
 *       },
 *       "location": {
 *         "tai": {
 *           "plmn": "99970",
 *           "tac_hex": "0001",
 *           "tac": 1
 *         }
 *       },
 *       "ambr": {
 *         "downlink": 1000000000,
 *         "uplink": 1000000000
 *       },
 *       "pdn": [
 *         {
 *           "apn": "internet",
 *           "qos_flows": [
 *             {
 *               "ebi": 5
 *             }
 *           ],
 *           "qci": 9,
 *           "ebi": 5,
 *           "bearer_count": 1,
 *           "pdu_state": "active"
 *         }
 *       ],
 *       "pdn_count": 1
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
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>

#include "ogs-core.h"
#include "ogs-proto.h"

#include "mme-context.h"
#include "ue-info.h"
#include "mme-context.h"

#include "metrics/prometheus/json_pager.h"
#include "metrics/ogs-metrics.h"
#include "sbi/openapi/external/cJSON.h"

#ifndef MME_UE_INFO_PAGE_SIZE_DEFAULT
#define MME_UE_INFO_PAGE_SIZE_DEFAULT 100U
#endif


size_t mme_dump_ue_info(char *buf, size_t buflen, size_t page, size_t page_size)
{
    if (page == SIZE_MAX) {
        page = 0;
        page_size = MME_UE_INFO_PAGE_SIZE_DEFAULT;
    } else if (page_size == 0) {
        page_size = MME_UE_INFO_PAGE_SIZE_DEFAULT;
    } else if (page_size > MME_UE_INFO_PAGE_SIZE_DEFAULT) {
        page_size = MME_UE_INFO_PAGE_SIZE_DEFAULT;
    }

    return mme_dump_ue_info_paged(buf, buflen, page, page_size);
}

static inline const char *cm_state_str(const mme_ue_t *ue)
{
    return (ue && ECM_CONNECTED(ue)) ? "connected" : "idle";
}

static cJSON *build_enb(const mme_ue_t *ue)
{
    cJSON *enb = cJSON_CreateObject();
    if (!enb) return NULL;

    /* ostream_id */
    if (!cJSON_AddNumberToObject(enb, "ostream_id", (double)ue->enb_ostream_id))
        goto end;

    /* S1AP IDs */
    enb_ue_t *ran = enb_ue_find_by_id(ue->enb_ue_id);
    if (ran) {
        if (!cJSON_AddNumberToObject(enb, "mme_ue_ngap_id", (double)ran->mme_ue_s1ap_id))
            goto end;
        if (!cJSON_AddNumberToObject(enb, "ran_ue_ngap_id", (double)ran->enb_ue_s1ap_id))
            goto end;

        mme_enb_t *enb_obj = mme_enb_find_by_id(ran->enb_id);
        if (enb_obj && enb_obj->enb_id_presence) {
            if (!cJSON_AddNumberToObject(enb, "enb_id", (double)enb_obj->enb_id))
                goto end;
        }

        /* cell_id: prefer last reported via E-UTRAN_CGI from RAN; else fallback to UE’s e_cgi */
        uint32_t cell_id = 0;
        if (ran->saved.e_cgi.cell_id) cell_id = ran->saved.e_cgi.cell_id;
        else if (ue->e_cgi.cell_id)   cell_id = ue->e_cgi.cell_id;

        if (cell_id) {
            if (!cJSON_AddNumberToObject(enb, "cell_id", (double)cell_id))
                goto end;
        }
    }

    return enb;

end:
    cJSON_Delete(enb);
    return NULL;
}

static cJSON *build_location(const mme_ue_t *ue)
{
    cJSON *loc = cJSON_CreateObject();
    if (!loc) return NULL;

    cJSON *tai = cJSON_CreateObject();
    if (!tai) goto end;

    /* TAI: PLMN + TAC (hex and numeric) */
    char plmn_str[OGS_PLMNIDSTRLEN] = {0};
    ogs_plmn_id_to_string(&ue->tai.plmn_id, plmn_str);
    if (!cJSON_AddStringToObject(tai, "plmn", plmn_str)) { cJSON_Delete(tai); goto end; }

    char tac_hex[8];
    (void)snprintf(tac_hex, sizeof tac_hex, "%04x", (unsigned)ue->tai.tac);
    if (!cJSON_AddStringToObject(tai, "tac_hex", tac_hex)) { cJSON_Delete(tai); goto end; }
    if (!cJSON_AddNumberToObject(tai, "tac", (double)ue->tai.tac)) { cJSON_Delete(tai); goto end; }

    cJSON_AddItemToObjectCS(loc, "tai", tai);
    return loc;

end:
    cJSON_Delete(loc);
    return NULL;
}

static cJSON *build_ambr(const mme_ue_t *ue)
{
    cJSON *ambr = cJSON_CreateObject();
    if (!ambr) return NULL;

    if (!cJSON_AddNumberToObject(ambr, "downlink", (double)ue->ambr.downlink)) goto end;
    if (!cJSON_AddNumberToObject(ambr, "uplink",   (double)ue->ambr.uplink))   goto end;

    return ambr;

end:
    cJSON_Delete(ambr);
    return NULL;
}

static cJSON *build_pdn_array(const mme_ue_t *ue)
{
    cJSON *arr = cJSON_CreateArray();
    if (!arr) return NULL;

    mme_sess_t *sess = NULL;

    ogs_list_for_each(&(ue->sess_list), sess) {
        cJSON *it = cJSON_CreateObject();
        if (!it) goto oom_all;

        /* APN */
        const char *apn = (sess->session && sess->session->name && sess->session->name[0])
                            ? (const char*)sess->session->name : "";
        if (apn[0]) {
            if (!cJSON_AddStringToObject(it, "apn", apn)) { cJSON_Delete(it); goto oom_all; }
        }

        /* QoS flows: list EBIs; QCI at session-level if present */
        unsigned ebi_root = 0;
        unsigned bearer_count = 0;

        cJSON *qarr = cJSON_CreateArray();
        if (!qarr) { cJSON_Delete(it); goto oom_all; }

        mme_bearer_t *b = NULL;
        ogs_list_for_each(&((mme_sess_t *)sess)->bearer_list, b) {
            if (!b || b->ebi == 0) continue;

            bearer_count++;
            if (ebi_root == 0 || b->ebi < ebi_root)
                ebi_root = (unsigned)b->ebi;

            cJSON *q = cJSON_CreateObject();
            if (!q) { cJSON_Delete(qarr); cJSON_Delete(it); goto oom_all; }
            if (!cJSON_AddNumberToObject(q, "ebi", (double)b->ebi)) { cJSON_Delete(q); cJSON_Delete(qarr); cJSON_Delete(it); goto oom_all; }

            cJSON_AddItemToArray(qarr, q);
        }

        cJSON_AddItemToObjectCS(it, "qos_flows", qarr);

        /* Session-level QCI (if known) */
        if (sess->session && sess->session->qos.index > 0) {
            if (!cJSON_AddNumberToObject(it, "qci", (double)sess->session->qos.index)) { cJSON_Delete(it); goto oom_all; }
        }

        if (ebi_root) {
            if (!cJSON_AddNumberToObject(it, "ebi", (double)ebi_root)) { cJSON_Delete(it); goto oom_all; }
        }
        if (!cJSON_AddNumberToObject(it, "bearer_count", (double)bearer_count)) { cJSON_Delete(it); goto oom_all; }

        const char *state = bearer_count ? "active" : "unknown";
        if (!cJSON_AddStringToObject(it, "pdu_state", state)) { cJSON_Delete(it); goto oom_all; }

        cJSON_AddItemToArray(arr, it);
    }

    return arr;

oom_all:
    cJSON_Delete(arr);
    return NULL;
}

static cJSON *ue_to_json(const mme_ue_t *ue)
{
    cJSON *o = cJSON_CreateObject();
    if (!o) return NULL;

    /* identity */
    if (ue->imsi_bcd[0]) {
        if (!cJSON_AddStringToObject(o, "supi", ue->imsi_bcd)) goto end;
    }
    if (!cJSON_AddStringToObject(o, "domain", "EPS")) goto end;
    if (!cJSON_AddStringToObject(o, "rat", "E-UTRA")) goto end;
    if (!cJSON_AddStringToObject(o, "cm_state", cm_state_str(ue))) goto end;

    /* enb */
    {
        cJSON *enb = build_enb(ue);
        if (!enb) goto end;
        cJSON_AddItemToObjectCS(o, "enb", enb);
    }

    /* location */
    {
        cJSON *loc = build_location(ue);
        if (!loc) goto end;
        cJSON_AddItemToObjectCS(o, "location", loc);
    }

    /* ambr */
    {
        cJSON *ambr = build_ambr(ue);
        if (!ambr) goto end;
        cJSON_AddItemToObjectCS(o, "ambr", ambr);
    }

    /* pdn + pdn_count */
    {
        cJSON *pdn = build_pdn_array(ue);
        if (!pdn) goto end;

        /* Count PDNs before attaching */
        size_t pdn_count = 0;
        {
            cJSON *it = NULL;
            cJSON_ArrayForEach(it, pdn) pdn_count++;
        }

        cJSON_AddItemToObjectCS(o, "pdn", pdn);
        if (!cJSON_AddNumberToObject(o, "pdn_count", (double)pdn_count)) goto end;
    }

    return o;

end:
    cJSON_Delete(o);
    return NULL;
}

size_t mme_dump_ue_info_paged(char *buf, size_t buflen, size_t page, size_t page_size)
{
    if (!buf || buflen == 0) return 0;

    const bool no_paging = (page == SIZE_MAX);
    if (!no_paging) {
        if (page_size == 0) page_size = MME_UE_INFO_PAGE_SIZE_DEFAULT;
        if (page_size > MME_UE_INFO_PAGE_SIZE_DEFAULT) page_size = MME_UE_INFO_PAGE_SIZE_DEFAULT;
    } else {
        page_size = SIZE_MAX;
        page = 0;
    }

    const size_t start_index = json_pager_safe_start_index(no_paging, page, page_size);

    cJSON *root  = cJSON_CreateObject();
    if (!root) {
        if (buflen >= 3) { memcpy(buf, "{}", 3); return 2; }
        if (buflen) buf[0] = '\0';
        return 0;
    }

    cJSON *items = cJSON_CreateArray();
    if (!items) {
        cJSON_Delete(root);
        if (buflen >= 3) { memcpy(buf, "{}", 3); return 2; }
        if (buflen) buf[0] = '\0';
        return 0;
    }

    size_t idx = 0, emitted = 0;
    bool has_next = false, oom = false;

    mme_context_t *ctxt = mme_self();
    if (!ctxt) {
        cJSON_Delete(items);
        cJSON_Delete(root);
        if (buflen >= 3) { memcpy(buf, "{}", 3); return 2; }
        if (buflen) buf[0] = '\0';
        return 0;
    }

    mme_ue_t *ue = NULL;
    ogs_list_for_each(&ctxt->mme_ue_list, ue) {
        int act = json_pager_advance(no_paging, idx, start_index, emitted, page_size, &has_next);
        if (act == 1) { idx++; continue; }
        if (act == 2) break;

        cJSON *one = ue_to_json(ue);
        if (!one) { oom = true; break; }

        cJSON_AddItemToArray(items, one);
        emitted++;
        idx++;
    }

    /* attach only when array is fully built */
    cJSON_AddItemToObjectCS(root, "items", items);
    json_pager_add_trailing(root, no_paging, page, page_size, emitted,
                            has_next && !oom, "/ue-info", oom);

    return json_pager_finalize(root, buf, buflen);
}

