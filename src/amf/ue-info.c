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
 * Connected AMF UEs JSON dumper for the Prometheus HTTP server (/ue-info).
 * - supi, suci, pei, cm_state, guti,m_tmsi, gnb_info, location, security, ambr, slices, am_policy
 * - pager: /ue-info?page=0&page_size=100 (0-based, page=-1 without paging) Default: page=0 page_size=100=MAXSIZE
 *
 * path: http://AMF_IP:9090/ue-info
 * curl -s "http://127.0.0.5:9090/ue-info" |jq . 
 * {
 *   "items": [
 *     {
 *       "supi": "imsi-001010000056492",
 *       "suci": "suci-0-001-01-0000-1-1-85731c4241d466407311a85135e914987944f361550f13f4532e29bdc5859548a363cc1a81798bb4cc59f2b4db",
 *       "pei": "imeisv-3535938300494715",
 *       "cm_state": "connected",
 *       "guti": "00101-030080-CC00007AB",
 *       "m_tmsi": 3221227435,
 *       "gnb": {
 *         "ostream_id": 1,
 *         "amf_ue_ngap_id": 2,
 *         "ran_ue_ngap_id": 39,
 *         "gnb_id": 100,
 *         "cell_id": 1
 *       },
 *       "location": {
 *         "timestamp": 1758733375895512,
 *         "nr_tai": {
 *           "plmn": "99970",
 *           "tac_hex": "000001",
 *           "tac": 1
 *         },
 *         "nr_cgi": {
 *           "plmn": "99970",
 *           "nci": 1638401,
 *           "gnb_id": 100,
 *           "cell_id": 1
 *         },
 *         "last_visited_plmn_id": "000000"
 *       },
 *       "msisdn": [],
 *       "security": {
 *         "valid": 1,
 *         "enc": "nea2",
 *         "int": "nia2"
 *       },
 *       "ambr": {
 *         "downlink": 1000000000,
 *         "uplink": 1000000000
 *       },
 *       "pdu_sessions": [],
 *       "pdu_sessions_count": 0,
 *       "requested_slices": [
 *         {
 *           "sst": 1,
 *           "sd": "ffffff"
 *         }
 *       ],
 *       "allowed_slices": [
 *         {
 *           "sst": 1,
 *           "sd": "ffffff"
 *         }
 *       ],
 *       "requested_slices_count": 1,
 *       "allowed_slices_count": 1,
 *       "am_policy_features": 4,
 *       "am_policy_features_info": {
 *         "hex": "0x0000000000000004",
 *         "bits": [
 *           2
 *         ],
 *         "labels": [
 *           "QoS Policy Control"
 *         ]
 *       }
 *     }
 *   ],
 *   "pager": {
 *     "page": 0,
 *     "page_size": 100,
 *     "count": 2
 *   }
 * }
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>

#include "ogs-core.h"
#include "ogs-proto.h"
#include "context.h"
#include "ue-info.h"

#include "sbi/openapi/external/cJSON.h"
#include "metrics/prometheus/json_pager.h"

/* -------- pager state (wired from init) -------- */
static size_t g_ue_page = 0;
static size_t g_ue_page_size = 0;

void amf_metrics_ue_info_set_pager(size_t page, size_t page_size)
{
    g_ue_page = page;
    g_ue_page_size = page_size;
}

size_t amf_dump_ue_info(char *buf, size_t buflen)
{
    size_t page = g_ue_page;
    size_t page_size = g_ue_page_size ? g_ue_page_size : 100;
    if (page_size > 100) page_size = 100;
    return amf_dump_ue_info_paged(buf, buflen, page, page_size);
}

/* -------- small local helpers (no JSON helpers) -------- */
static inline uint32_t u24_to_u32(ogs_uint24_t v)
{
    uint32_t x = 0;
    memcpy(&x, &v, sizeof(v) < sizeof(x) ? sizeof(v) : sizeof(x));
    return (x & 0xFFFFFFu);
}

static inline void bytes3_to_hex_lower(const void *p, char out[7])
{
    const uint8_t *b = (const uint8_t *)p;
    static const char *hx = "0123456789abcdef";
    out[0] = hx[(b[0] >> 4) & 0xF];
    out[1] = hx[b[0] & 0xF];
    out[2] = hx[(b[1] >> 4) & 0xF];
    out[3] = hx[b[1] & 0xF];
    out[4] = hx[(b[2] >> 4) & 0xF];
    out[5] = hx[b[2] & 0xF];
    out[6] = '\0';
}

/* AM policy feature labels */
static const char *am_policy_feature_names[64] = {
    /*0*/ "AM Policy Association",
    /*1*/ "Slice-specific Policy",
    /*2*/ "QoS Policy Control",
    /*3*/ "Access & Mobility Policy",
    /*4*/ "Charging Policy",
    /*5*/ "Traffic Steering",
    /*6*/ "Roaming Policy",
    /*7*/ "Emergency Services Policy",
    /*8*/ "UE Policy",
    /*9*/ "Session Continuity Policy",
    /*10*/ "Application-based Policy",
    /*11*/ "Location-based Policy",
    /*12*/ "Time-based Policy",
    /*13*/ "Data Network Selection",
    /*14*/ "Policy Update Notification",
    /*15*/ "Policy Control Event Exposure",
};

/* -------- JSON builders: each returns 0 on success, -1 on OOM; they free their own partial trees on failure -------- */

static int add_plmn_string(cJSON *obj, const char *key, const ogs_plmn_id_t *plmn)
{
    char s[OGS_PLMNIDSTRLEN] = {0};
    if (plmn) ogs_plmn_id_to_string(plmn, s);
    if (!cJSON_AddStringToObject(obj, key, s)) return -1;
    return 0;
}

static int add_snssai_sd_string(cJSON *obj, const char *key, const void *sd_ptr)
{
    char sd_hex[7] = "ffffff";
    if (sd_ptr) bytes3_to_hex_lower(sd_ptr, sd_hex);
    if (!cJSON_AddStringToObject(obj, key, sd_hex)) return -1;
    return 0;
}

static int add_basic_identity(cJSON *o, const amf_ue_t *ue)
{
    if (ue->supi && !cJSON_AddStringToObject(o, "supi", ue->supi)) return -1;
    if (ue->suci && !cJSON_AddStringToObject(o, "suci", ue->suci)) return -1;
    if (ue->pei  && !cJSON_AddStringToObject(o, "pei",  ue->pei))  return -1;

    if (!cJSON_AddStringToObject(o, "cm_state", CM_CONNECTED(ue) ? "connected" : "idle"))
        return -1;

    if (ue->current.m_tmsi) {
        char plmn_str[OGS_PLMNIDSTRLEN] = {0};
        ogs_plmn_id_to_string(&ue->guami->plmn_id, plmn_str);

        char *amf_hex = ogs_amf_id_to_string(&ue->guami->amf_id);
        const char *amf_str = amf_hex ? amf_hex : "";

        char guti_buf[80];
        (void)snprintf(guti_buf, sizeof guti_buf, "%s-%s-C%08X",
                       plmn_str, amf_str, (unsigned)ue->current.guti.m_tmsi);

        bool ok = cJSON_AddStringToObject(o, "guti", guti_buf) != NULL
               && cJSON_AddNumberToObject(o, "m_tmsi", (double)ue->current.guti.m_tmsi) != NULL;

        if (amf_hex) ogs_free(amf_hex);
        if (!ok) return -1;
    }
    return 0;
}

static int add_gnb(cJSON *parent, const amf_ue_t *ue)
{
    cJSON *gnb = cJSON_CreateObject();
    if (!gnb) return -1;

    if (!cJSON_AddNumberToObject(gnb, "ostream_id", (double)ue->gnb_ostream_id)) goto fail;

    ran_ue_t *ran = ran_ue_find_by_id(ue->ran_ue_id);
    if (ran) {
        uint64_t nci = ue->nr_cgi.cell_id & 0xFFFFFFFFFULL; /* 36-bit */
        uint32_t gnb_id  = (uint32_t)((nci >> 14) & 0x3FFFFF);
        uint32_t cell_id = (uint32_t)(nci & 0x3FFF);

        if (!cJSON_AddNumberToObject(gnb, "amf_ue_ngap_id", (double)ran->amf_ue_ngap_id)) goto fail;
        if (!cJSON_AddNumberToObject(gnb, "ran_ue_ngap_id", (double)ran->ran_ue_ngap_id)) goto fail;
        if (!cJSON_AddNumberToObject(gnb, "gnb_id", (double)gnb_id)) goto fail;
        if (!cJSON_AddNumberToObject(gnb, "cell_id", (double)cell_id)) goto fail;
    } else {
        if (!cJSON_AddStringToObject(gnb, "status", "not-connected")) goto fail;
    }

    cJSON_AddItemToObject(parent, "gnb", gnb);
    return 0;

fail:
    cJSON_Delete(gnb);
    return -1;
}

static int add_location(cJSON *parent, const amf_ue_t *ue)
{
    cJSON *loc = cJSON_CreateObject();
    if (!loc) return -1;

    if (!cJSON_AddNumberToObject(loc, "timestamp", (double)ue->ue_location_timestamp)) goto fail;

    /* nr_tai */
    {
        cJSON *tai = cJSON_CreateObject();
        if (!tai) goto fail;

        if (add_plmn_string(tai, "plmn", &ue->nr_tai.plmn_id) < 0) { cJSON_Delete(tai); goto fail; }

        const char *tac_hex = ogs_uint24_to_0string(ue->nr_tai.tac);
        if (!cJSON_AddStringToObject(tai, "tac_hex", tac_hex)) { cJSON_Delete(tai); goto fail; }
        if (!cJSON_AddNumberToObject(tai, "tac", (double)u24_to_u32(ue->nr_tai.tac))) { cJSON_Delete(tai); goto fail; }

        cJSON_AddItemToObject(loc, "nr_tai", tai);
    }

    /* nr_cgi */
    {
        cJSON *cgi = cJSON_CreateObject();
        if (!cgi) goto fail;

        if (add_plmn_string(cgi, "plmn", &ue->nr_cgi.plmn_id) < 0) { cJSON_Delete(cgi); goto fail; }

        uint64_t nci = ue->nr_cgi.cell_id & 0xFFFFFFFFFULL; /* 36-bit */
        uint32_t gnb_id  = (uint32_t)((nci >> 14) & 0x3FFFFF);
        uint32_t cell_id = (uint32_t)(nci & 0x3FFF);

        if (!cJSON_AddNumberToObject(cgi, "nci", (double)nci))            { cJSON_Delete(cgi); goto fail; }
        if (!cJSON_AddNumberToObject(cgi, "gnb_id", (double)gnb_id))      { cJSON_Delete(cgi); goto fail; }
        if (!cJSON_AddNumberToObject(cgi, "cell_id", (double)cell_id))    { cJSON_Delete(cgi); goto fail; }

        cJSON_AddItemToObject(loc, "nr_cgi", cgi);
    }

    if (add_plmn_string(loc, "last_visited_plmn_id", &ue->last_visited_plmn_id) < 0) goto fail;

    cJSON_AddItemToObject(parent, "location", loc);
    return 0;

fail:
    cJSON_Delete(loc);
    return -1;
}

static int add_security(cJSON *parent, const amf_ue_t *ue)
{
    cJSON *sec = cJSON_CreateObject();
    if (!sec) return -1;

    if (!cJSON_AddNumberToObject(sec, "valid", (double)SECURITY_CONTEXT_IS_VALID(ue))) goto fail;

    const char *enc =
        ue->selected_enc_algorithm == OGS_NAS_SECURITY_ALGORITHMS_128_NEA2 ? "nea2" :
        ue->selected_enc_algorithm == OGS_NAS_SECURITY_ALGORITHMS_128_NEA1 ? "nea1" :
        ue->selected_enc_algorithm == OGS_NAS_SECURITY_ALGORITHMS_128_NEA3 ? "nea3" : "nea0";

    const char *integ =
        ue->selected_int_algorithm == OGS_NAS_SECURITY_ALGORITHMS_128_NIA2 ? "nia2" :
        ue->selected_int_algorithm == OGS_NAS_SECURITY_ALGORITHMS_128_NIA1 ? "nia1" :
        ue->selected_int_algorithm == OGS_NAS_SECURITY_ALGORITHMS_128_NIA3 ? "nia3" : "nia0";

    if (!cJSON_AddStringToObject(sec, "enc", enc)) goto fail;
    if (!cJSON_AddStringToObject(sec, "int", integ)) goto fail;

    cJSON_AddItemToObject(parent, "security", sec);
    return 0;

fail:
    cJSON_Delete(sec);
    return -1;
}

static int add_ambr(cJSON *parent, const amf_ue_t *ue)
{
    cJSON *ambr = cJSON_CreateObject();
    if (!ambr) return -1;

    if (!cJSON_AddNumberToObject(ambr, "downlink", (double)ue->ue_ambr.downlink)) goto fail;
    if (!cJSON_AddNumberToObject(ambr, "uplink",   (double)ue->ue_ambr.uplink))   goto fail;

    cJSON_AddItemToObject(parent, "ambr", ambr);
    return 0;

fail:
    cJSON_Delete(ambr);
    return -1;
}

static int add_requested_allowed_slices(cJSON *parent, const amf_ue_t *ue)
{
    cJSON *req = cJSON_CreateArray();
    cJSON *allow = cJSON_CreateArray();
    if (!req || !allow) { if (req) cJSON_Delete(req); if (allow) cJSON_Delete(allow); return -1; }

    /* requested */
    for (int i = 0; i < ue->requested_nssai.num_of_s_nssai; i++) {
        const ogs_nas_s_nssai_ie_t *ie = &ue->requested_nssai.s_nssai[i];
        cJSON *sn = cJSON_CreateObject(); if (!sn) goto fail;
        if (!cJSON_AddNumberToObject(sn, "sst", (double)ie->sst))       { cJSON_Delete(sn); goto fail; }
        if (add_snssai_sd_string(sn, "sd", &ie->sd) < 0)                { cJSON_Delete(sn); goto fail; }
        cJSON_AddItemToArray(req, sn);
    }

    /* allowed */
    for (int i = 0; i < ue->allowed_nssai.num_of_s_nssai; i++) {
        const ogs_nas_s_nssai_ie_t *ie = &ue->allowed_nssai.s_nssai[i];
        cJSON *sn = cJSON_CreateObject(); if (!sn) goto fail;
        if (!cJSON_AddNumberToObject(sn, "sst", (double)ie->sst))       { cJSON_Delete(sn); goto fail; }
        if (add_snssai_sd_string(sn, "sd", &ie->sd) < 0)                { cJSON_Delete(sn); goto fail; }
        cJSON_AddItemToArray(allow, sn);
    }

    if (!cJSON_AddItemToObject(parent, "requested_slices", req)) { /* void in cJSON, keep for symmetry */ }
    if (!cJSON_AddItemToObject(parent, "allowed_slices", allow)) { /* void */ }

    if (!cJSON_AddNumberToObject(parent, "requested_slices_count",
                                 (double)ue->requested_nssai.num_of_s_nssai)) return -1;
    if (!cJSON_AddNumberToObject(parent, "allowed_slices_count",
                                 (double)ue->allowed_nssai.num_of_s_nssai))   return -1;

    return 0;

fail:
    cJSON_Delete(req);
    cJSON_Delete(allow);
    return -1;
}

static int add_msisdn_array(cJSON *parent, const amf_ue_t *ue)
{
    cJSON *arr = cJSON_CreateArray();
    if (!arr) return -1;

    for (int i = 0; i < ue->num_of_msisdn; i++) {
        if (!ue->msisdn[i]) continue;
        cJSON *s = cJSON_CreateString(ue->msisdn[i]);
        if (!s) { cJSON_Delete(arr); return -1; }
        cJSON_AddItemToArray(arr, s);
    }

    cJSON_AddItemToObject(parent, "msisdn", arr);
    return 0;
}

static int add_pdu_sessions(cJSON *parent, const amf_ue_t *ue)
{
    cJSON *arr = cJSON_CreateArray();
    if (!arr) return -1;

    int count = 0;
    amf_sess_t *sess = NULL;
    ogs_list_for_each(&(ue->sess_list), sess) {
        cJSON *it = cJSON_CreateObject();
        if (!it) { cJSON_Delete(arr); return -1; }

        if (!cJSON_AddNumberToObject(it, "psi", (double)sess->psi)) { cJSON_Delete(it); cJSON_Delete(arr); return -1; }
        if (sess->dnn && !cJSON_AddStringToObject(it, "dnn", sess->dnn)) { cJSON_Delete(it); cJSON_Delete(arr); return -1; }

        cJSON *sn = cJSON_CreateObject();
        if (!sn) { cJSON_Delete(it); cJSON_Delete(arr); return -1; }
        if (!cJSON_AddNumberToObject(sn, "sst", (double)sess->s_nssai.sst)) { cJSON_Delete(sn); cJSON_Delete(it); cJSON_Delete(arr); return -1; }
        if (add_snssai_sd_string(sn, "sd", &sess->s_nssai.sd) < 0) { cJSON_Delete(sn); cJSON_Delete(it); cJSON_Delete(arr); return -1; }
        cJSON_AddItemToObject(it, "snssai", sn);

        if (!cJSON_AddBoolToObject(it, "lbo_roaming_allowed", sess->lbo_roaming_allowed)) { cJSON_Delete(it); cJSON_Delete(arr); return -1; }
        if (!cJSON_AddNumberToObject(it, "resource_status", (double)sess->resource_status)) { cJSON_Delete(it); cJSON_Delete(arr); return -1; }
        if (!cJSON_AddBoolToObject(it, "n1_released", sess->n1_released)) { cJSON_Delete(it); cJSON_Delete(arr); return -1; }
        if (!cJSON_AddBoolToObject(it, "n2_released", sess->n2_released)) { cJSON_Delete(it); cJSON_Delete(arr); return -1; }

        cJSON_AddItemToArray(arr, it);
        count++;
    }

    cJSON_AddItemToObject(parent, "pdu_sessions", arr);
    if (!cJSON_AddNumberToObject(parent, "pdu_sessions_count", (double)count)) return -1;
    return 0;
}

static int add_am_policy_features(cJSON *parent, const amf_ue_t *ue)
{
    uint64_t f = ue->am_policy_control_features;
    if (!cJSON_AddNumberToObject(parent, "am_policy_features", (double)f)) return -1;

    cJSON *feat = cJSON_CreateObject();
    if (!feat) return -1;

    char hex[2 + 16 + 1];
    (void)snprintf(hex, sizeof hex, "0x%016" PRIx64, f);
    if (!cJSON_AddStringToObject(feat, "hex", hex)) { cJSON_Delete(feat); return -1; }

    cJSON *bits = cJSON_CreateArray();
    cJSON *labels = cJSON_CreateArray();
    if (!bits || !labels) { if (bits) cJSON_Delete(bits); if (labels) cJSON_Delete(labels); cJSON_Delete(feat); return -1; }

    for (int i = 0; i < 64; i++) {
        if ((f >> i) & 1ULL) {
            cJSON *bi = cJSON_CreateNumber((double)i);
            if (!bi) { cJSON_Delete(bits); cJSON_Delete(labels); cJSON_Delete(feat); return -1; }
            cJSON_AddItemToArray(bits, bi);

            const char *label = (i < 64 && am_policy_feature_names[i] && am_policy_feature_names[i][0])
                                  ? am_policy_feature_names[i] : NULL;
            char fb[16];
            if (!label) { (void)snprintf(fb, sizeof fb, "bit%d", i); label = fb; }

            cJSON *ls = cJSON_CreateString(label);
            if (!ls) { cJSON_Delete(bits); cJSON_Delete(labels); cJSON_Delete(feat); return -1; }
            cJSON_AddItemToArray(labels, ls);
        }
    }

    cJSON_AddItemToObject(feat, "bits", bits);
    cJSON_AddItemToObject(feat, "labels", labels);
    cJSON_AddItemToObject(parent, "am_policy_features_info", feat);
    return 0;
}

static cJSON *amf_ue_to_json(const amf_ue_t *ue)
{
    cJSON *o = cJSON_CreateObject();
    if (!o) return NULL;

    if (add_basic_identity(o, ue)            < 0) goto fail;
    if (add_gnb(o, ue)                       < 0) goto fail;
    if (add_location(o, ue)                  < 0) goto fail;
    if (add_msisdn_array(o, ue)              < 0) goto fail;
    if (add_security(o, ue)                  < 0) goto fail;
    if (add_ambr(o, ue)                      < 0) goto fail;
    if (add_pdu_sessions(o, ue)              < 0) goto fail;
    if (add_requested_allowed_slices(o, ue)  < 0) goto fail;
    if (add_am_policy_features(o, ue)        < 0) goto fail;

    return o;

fail:
    cJSON_Delete(o);
    return NULL;
}

/* -------- top-level (paged) -------- */

size_t amf_dump_ue_info_paged(char *buf, size_t buflen, size_t page, size_t page_size)
{
    if (!buf || buflen == 0) return 0;

    const bool no_paging = (page == SIZE_MAX);
    if (!no_paging) {
        if (page_size == 0) page_size = 100;
        if (page_size > 100) page_size = 100;
    } else {
        page_size = SIZE_MAX;
        page = 0;
    }

    const size_t start_index = json_pager_safe_start_index(no_paging, page, page_size);

    amf_context_t *ctxt = amf_self();

    cJSON *root = cJSON_CreateObject();
    if (!root) { if (buflen >= 3) { memcpy(buf, "{}", 3); return 2; } if (buflen) buf[0] = '\0'; return 0; }

    cJSON *items = cJSON_CreateArray();
    if (!items) { cJSON_Delete(root); if (buflen >= 3) { memcpy(buf, "{}", 3); return 2; } if (buflen) buf[0] = '\0'; return 0; }
    cJSON_AddItemToObject(root, "items", items);

    size_t idx = 0, emitted = 0;
    bool has_next = false;
    bool oom = false;

    amf_ue_t *ue = NULL;
    ogs_list_for_each(&ctxt->amf_ue_list, ue) {
        int act = json_pager_advance(no_paging, idx, start_index, emitted, page_size, &has_next);
        if (act == 1) { idx++; continue; }
        if (act == 2) break;

        cJSON *one = amf_ue_to_json(ue);
        if (!one) { oom = true; break; }

        cJSON_AddItemToArray(items, one);
        emitted++;
        idx++;
    }

    /* add trailing pager info (json_pager_finalize will free 'root') */
    json_pager_add_trailing(root, no_paging, page, page_size, emitted,
                            has_next && !oom, "/ue-info", oom);

    return json_pager_finalize(root, buf, buflen);
}

