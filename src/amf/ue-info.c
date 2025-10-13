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


size_t amf_dump_ue_info(char *buf, size_t buflen, size_t page, size_t page_size)
{
    page_size = page_size ? page_size : 100;
    if (page_size > 100) page_size = 100;

    return amf_dump_ue_info_paged(buf, buflen, page, page_size);
}

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

/* -------- JSON builders: 0=OK, -1=OOM. Children are attached only when complete. -------- */
static int add_snssai_sd_string(cJSON *obj, const char *key, const void *sd_ptr)
{
    char sd_hex[7] = "ffffff";
    if (sd_ptr) bytes3_to_hex_lower(sd_ptr, sd_hex);
    cJSON *s = cJSON_CreateString(sd_hex);
    if (!s) return -1;
    cJSON_AddItemToObjectCS(obj, key, s);
    return 0;
}

static int add_basic_identity(cJSON *o, const amf_ue_t *ue)
{
    if (ue->supi && ue->supi[0]) {
        cJSON *v = cJSON_CreateString(ue->supi); if (!v) return -1;
        cJSON_AddItemToObjectCS(o, "supi", v);
    }
    if (ue->suci && ue->suci[0]) {
        cJSON *v = cJSON_CreateString(ue->suci); if (!v) return -1;
        cJSON_AddItemToObjectCS(o, "suci", v);
    }
    if (ue->pei && ue->pei[0]) {
        cJSON *v = cJSON_CreateString(ue->pei); if (!v) return -1;
        cJSON_AddItemToObjectCS(o, "pei", v);
    }

    {
        const char *cm = CM_CONNECTED(ue) ? "connected" : "idle";
        cJSON *v = cJSON_CreateString(cm); if (!v) return -1;
        cJSON_AddItemToObjectCS(o, "cm_state", v);
    }

    /* If M-TMSI present, expose GUTI string and m_tmsi numeric */
    if (ue->current.m_tmsi) {
        char plmn_str[OGS_PLMNIDSTRLEN] = {0};
        ogs_plmn_id_to_string(&ue->guami->plmn_id, plmn_str);

        /* ogs_amf_id_to_string() returns heap memory — free it */
        char *amf_hex = ogs_amf_id_to_string(&ue->guami->amf_id);
        const char *amf_str = amf_hex ? amf_hex : "";

        char guti_buf[80];
        (void)snprintf(guti_buf, sizeof guti_buf, "%s-%s-C%08X",
                       plmn_str, amf_str, (unsigned)ue->current.guti.m_tmsi);

        cJSON *g = cJSON_CreateString(guti_buf);
        if (amf_hex) ogs_free(amf_hex);
        if (!g) return -1;
        cJSON_AddItemToObjectCS(o, "guti", g);

        cJSON *m = cJSON_CreateNumber((double)ue->current.guti.m_tmsi);
        if (!m) return -1;
        cJSON_AddItemToObjectCS(o, "m_tmsi", m);
    }
    return 0;
}

static int add_gnb(cJSON *parent, const amf_ue_t *ue)
{
    cJSON *gnb = cJSON_CreateObject();
    if (!gnb) return -1;

    cJSON *osid = cJSON_CreateNumber((double)ue->gnb_ostream_id);
    if (!osid) { cJSON_Delete(gnb); return -1; }
    cJSON_AddItemToObjectCS(gnb, "ostream_id", osid);

    ran_ue_t *ran = ran_ue_find_by_id(ue->ran_ue_id);
    if (ran) {
        uint64_t nci = ue->nr_cgi.cell_id & 0xFFFFFFFFFULL; /* 36-bit */
        uint32_t gnb_id  = (uint32_t)((nci >> 14) & 0x3FFFFF);
        uint32_t cell_id = (uint32_t)(nci & 0x3FFF);

        cJSON *a = cJSON_CreateNumber((double)ran->amf_ue_ngap_id);
        cJSON *r = cJSON_CreateNumber((double)ran->ran_ue_ngap_id);
        cJSON *g = cJSON_CreateNumber((double)gnb_id);
        cJSON *c = cJSON_CreateNumber((double)cell_id);
        if (!a || !r || !g || !c) {
            if (a) cJSON_Delete(a);
            if (r) cJSON_Delete(r);
            if (g) cJSON_Delete(g);
            if (c) cJSON_Delete(c);
            cJSON_Delete(gnb);
            return -1;
        }
        cJSON_AddItemToObjectCS(gnb, "amf_ue_ngap_id", a);
        cJSON_AddItemToObjectCS(gnb, "ran_ue_ngap_id", r);
        cJSON_AddItemToObjectCS(gnb, "gnb_id", g);
        cJSON_AddItemToObjectCS(gnb, "cell_id", c);
    } else {
        cJSON *st = cJSON_CreateString("not-connected");
        if (!st) { cJSON_Delete(gnb); return -1; }
        cJSON_AddItemToObjectCS(gnb, "status", st);
    }

    cJSON_AddItemToObjectCS(parent, "gnb", gnb);
    return 0;
}

static int add_location(cJSON *parent, const amf_ue_t *ue)
{
    cJSON *loc = cJSON_CreateObject();
    if (!loc) return -1;

    cJSON *ts = cJSON_CreateNumber((double)ue->ue_location_timestamp);
    if (!ts) { cJSON_Delete(loc); return -1; }
    cJSON_AddItemToObjectCS(loc, "timestamp", ts);

    /* nr_tai */
    {
        cJSON *tai = cJSON_CreateObject();
        if (!tai) { cJSON_Delete(loc); return -1; }

        char plmn_str[OGS_PLMNIDSTRLEN] = {0};
        ogs_plmn_id_to_string(&ue->nr_tai.plmn_id, plmn_str);
        cJSON *p = cJSON_CreateString(plmn_str);
        if (!p) { cJSON_Delete(tai); cJSON_Delete(loc); return -1; }
        cJSON_AddItemToObjectCS(tai, "plmn", p);

        char tac_hex[7];
        (void)snprintf(tac_hex, sizeof tac_hex, "%06x", (unsigned)u24_to_u32(ue->nr_tai.tac));
        cJSON *th = cJSON_CreateString(tac_hex);
        cJSON *tn = cJSON_CreateNumber((double)u24_to_u32(ue->nr_tai.tac));
        if (!th || !tn) { if (th) cJSON_Delete(th); if (tn) cJSON_Delete(tn); cJSON_Delete(tai); cJSON_Delete(loc); return -1; }
        cJSON_AddItemToObjectCS(tai, "tac_hex", th);
        cJSON_AddItemToObjectCS(tai, "tac", tn);

        cJSON_AddItemToObjectCS(loc, "nr_tai", tai);
    }

    /* nr_cgi */
    {
        cJSON *cgi = cJSON_CreateObject();
        if (!cgi) { cJSON_Delete(loc); return -1; }

        char plmn_str[OGS_PLMNIDSTRLEN] = {0};
        ogs_plmn_id_to_string(&ue->nr_cgi.plmn_id, plmn_str);
        cJSON *p = cJSON_CreateString(plmn_str);
        if (!p) { cJSON_Delete(cgi); cJSON_Delete(loc); return -1; }
        cJSON_AddItemToObjectCS(cgi, "plmn", p);

        uint64_t nci     = ue->nr_cgi.cell_id & 0xFFFFFFFFFULL; /* 36-bit */
        uint32_t gnb_id  = (uint32_t)((nci >> 14) & 0x3FFFFF);
        uint32_t cell_id = (uint32_t)(nci & 0x3FFF);

        cJSON *n = cJSON_CreateNumber((double)nci);
        cJSON *g = cJSON_CreateNumber((double)gnb_id);
        cJSON *c = cJSON_CreateNumber((double)cell_id);
        if (!n || !g || !c) { if (n) cJSON_Delete(n); if (g) cJSON_Delete(g); if (c) cJSON_Delete(c); cJSON_Delete(cgi); cJSON_Delete(loc); return -1; }
        cJSON_AddItemToObjectCS(cgi, "nci", n);
        cJSON_AddItemToObjectCS(cgi, "gnb_id", g);
        cJSON_AddItemToObjectCS(cgi, "cell_id", c);

        cJSON_AddItemToObjectCS(loc, "nr_cgi", cgi);
    }

    /* last_visited_plmn_id */
    {
        char plmn_str[OGS_PLMNIDSTRLEN] = {0};
        ogs_plmn_id_to_string(&ue->last_visited_plmn_id, plmn_str);
        cJSON *lv = cJSON_CreateString(plmn_str);
        if (!lv) { cJSON_Delete(loc); return -1; }
        cJSON_AddItemToObjectCS(loc, "last_visited_plmn_id", lv);
    }

    cJSON_AddItemToObjectCS(parent, "location", loc);
    return 0;
}

static int add_msisdn_array(cJSON *parent, const amf_ue_t *ue)
{
    cJSON *arr = cJSON_CreateArray();
    int i;

    if (!arr) return -1;

    for (i = 0; i < ue->num_of_msisdn; i++) {
        if (!ue->msisdn[i] || !ue->msisdn[i][0]) continue;
        cJSON *s = cJSON_CreateString(ue->msisdn[i]);
        if (!s) { cJSON_Delete(arr); return -1; }
        cJSON_AddItemToArray(arr, s);
    }

    cJSON_AddItemToObjectCS(parent, "msisdn", arr);
    return 0;
}

static int add_security(cJSON *parent, const amf_ue_t *ue)
{
    cJSON *sec = cJSON_CreateObject();
    if (!sec) return -1;

    {
        int valid_ctx = SECURITY_CONTEXT_IS_VALID(ue);
        cJSON *v = cJSON_CreateNumber((double)valid_ctx);
        if (!v) { cJSON_Delete(sec); return -1; }
        cJSON_AddItemToObjectCS(sec, "valid", v);
    }

    const char *enc =
        ue->selected_enc_algorithm == OGS_NAS_SECURITY_ALGORITHMS_128_NEA2 ? "nea2" :
        ue->selected_enc_algorithm == OGS_NAS_SECURITY_ALGORITHMS_128_NEA1 ? "nea1" :
        ue->selected_enc_algorithm == OGS_NAS_SECURITY_ALGORITHMS_128_NEA3 ? "nea3" : "nea0";

    const char *integ =
        ue->selected_int_algorithm == OGS_NAS_SECURITY_ALGORITHMS_128_NIA2 ? "nia2" :
        ue->selected_int_algorithm == OGS_NAS_SECURITY_ALGORITHMS_128_NIA1 ? "nia1" :
        ue->selected_int_algorithm == OGS_NAS_SECURITY_ALGORITHMS_128_NIA3 ? "nia3" : "nia0";

    cJSON *e = cJSON_CreateString(enc);
    cJSON *i = cJSON_CreateString(integ);
    if (!e || !i) { if (e) cJSON_Delete(e); if (i) cJSON_Delete(i); cJSON_Delete(sec); return -1; }
    cJSON_AddItemToObjectCS(sec, "enc", e);
    cJSON_AddItemToObjectCS(sec, "int", i);

    cJSON_AddItemToObjectCS(parent, "security", sec);
    return 0;
}

static int add_ambr(cJSON *parent, const amf_ue_t *ue)
{
    cJSON *ambr = cJSON_CreateObject();
    if (!ambr) return -1;

    cJSON *dl = cJSON_CreateNumber((double)ue->ue_ambr.downlink);
    cJSON *ul = cJSON_CreateNumber((double)ue->ue_ambr.uplink);
    if (!dl || !ul) { if (dl) cJSON_Delete(dl); if (ul) cJSON_Delete(ul); cJSON_Delete(ambr); return -1; }

    cJSON_AddItemToObjectCS(ambr, "downlink", dl);
    cJSON_AddItemToObjectCS(ambr, "uplink", ul);

    cJSON_AddItemToObjectCS(parent, "ambr", ambr);
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

        /* PSI */
        {
            cJSON *n = cJSON_CreateNumber((double)sess->psi);
            if (!n) { cJSON_Delete(it); cJSON_Delete(arr); return -1; }
            cJSON_AddItemToObjectCS(it, "psi", n);
        }

        /* DNN */
        if (sess->dnn && sess->dnn[0]) {
            cJSON *d = cJSON_CreateString(sess->dnn);
            if (!d) { cJSON_Delete(it); cJSON_Delete(arr); return -1; }
            cJSON_AddItemToObjectCS(it, "dnn", d);
        }

        /* S-NSSAI */
        {
            cJSON *sn = cJSON_CreateObject();
            if (!sn) { cJSON_Delete(it); cJSON_Delete(arr); return -1; }
            cJSON *sst = cJSON_CreateNumber((double)sess->s_nssai.sst);
            if (!sst) { cJSON_Delete(sn); cJSON_Delete(it); cJSON_Delete(arr); return -1; }
            cJSON_AddItemToObjectCS(sn, "sst", sst);
            if (add_snssai_sd_string(sn, "sd", &sess->s_nssai.sd) < 0) { cJSON_Delete(sn); cJSON_Delete(it); cJSON_Delete(arr); return -1; }
            cJSON_AddItemToObjectCS(it, "snssai", sn);
        }

        /* Flags/counters mirrored as-is */
        {
            cJSON *lbo = cJSON_CreateBool(sess->lbo_roaming_allowed);
            cJSON *rs  = cJSON_CreateNumber((double)sess->resource_status);
            cJSON *n1  = cJSON_CreateBool(sess->n1_released);
            cJSON *n2  = cJSON_CreateBool(sess->n2_released);
            if (!lbo || !rs || !n1 || !n2) {
                if (lbo) cJSON_Delete(lbo);
                if (rs)  cJSON_Delete(rs);
                if (n1)  cJSON_Delete(n1);
                if (n2)  cJSON_Delete(n2);
                cJSON_Delete(it);
                cJSON_Delete(arr);
                return -1;
            }
            cJSON_AddItemToObjectCS(it, "lbo_roaming_allowed", lbo);
            cJSON_AddItemToObjectCS(it, "resource_status", rs);
            cJSON_AddItemToObjectCS(it, "n1_released", n1);
            cJSON_AddItemToObjectCS(it, "n2_released", n2);
        }

        cJSON_AddItemToArray(arr, it);
        count++;
    }

    cJSON_AddItemToObjectCS(parent, "pdu_sessions", arr);
    {
        cJSON *n = cJSON_CreateNumber((double)count);
        if (!n) return -1;
        cJSON_AddItemToObjectCS(parent, "pdu_sessions_count", n);
    }
    return 0;
}

static int add_requested_allowed_slices(cJSON *parent, const amf_ue_t *ue)
{
    cJSON *req = cJSON_CreateArray();
    cJSON *allow = cJSON_CreateArray();
    int i;

    if (!req || !allow) { if (req) cJSON_Delete(req); if (allow) cJSON_Delete(allow); return -1; }

    /* requested */
    for (i = 0; i < ue->requested_nssai.num_of_s_nssai; i++) {
        const ogs_nas_s_nssai_ie_t *ie = &ue->requested_nssai.s_nssai[i];
        cJSON *sn = cJSON_CreateObject();
        if (!sn) { cJSON_Delete(req); cJSON_Delete(allow); return -1; }
        cJSON *sst = cJSON_CreateNumber((double)ie->sst);
        if (!sst) { cJSON_Delete(sn); cJSON_Delete(req); cJSON_Delete(allow); return -1; }
        cJSON_AddItemToObjectCS(sn, "sst", sst);
        if (add_snssai_sd_string(sn, "sd", &ie->sd) < 0) { cJSON_Delete(sn); cJSON_Delete(req); cJSON_Delete(allow); return -1; }
        cJSON_AddItemToArray(req, sn);
    }

    /* allowed */
    for (i = 0; i < ue->allowed_nssai.num_of_s_nssai; i++) {
        const ogs_nas_s_nssai_ie_t *ie = &ue->allowed_nssai.s_nssai[i];
        cJSON *sn = cJSON_CreateObject();
        if (!sn) { cJSON_Delete(req); cJSON_Delete(allow); return -1; }
        cJSON *sst = cJSON_CreateNumber((double)ie->sst);
        if (!sst) { cJSON_Delete(sn); cJSON_Delete(req); cJSON_Delete(allow); return -1; }
        cJSON_AddItemToObjectCS(sn, "sst", sst);
        if (add_snssai_sd_string(sn, "sd", &ie->sd) < 0) { cJSON_Delete(sn); cJSON_Delete(req); cJSON_Delete(allow); return -1; }
        cJSON_AddItemToArray(allow, sn);
    }

    cJSON_AddItemToObjectCS(parent, "requested_slices", req);
    cJSON_AddItemToObjectCS(parent, "allowed_slices", allow);

    cJSON *rc = cJSON_CreateNumber((double)ue->requested_nssai.num_of_s_nssai);
    cJSON *ac = cJSON_CreateNumber((double)ue->allowed_nssai.num_of_s_nssai);
    if (!rc || !ac) { if (rc) cJSON_Delete(rc); if (ac) cJSON_Delete(ac); return -1; }
    cJSON_AddItemToObjectCS(parent, "requested_slices_count", rc);
    cJSON_AddItemToObjectCS(parent, "allowed_slices_count", ac);

    return 0;
}

static int add_am_policy_features(cJSON *parent, const amf_ue_t *ue)
{
    uint64_t f = ue->am_policy_control_features;

    cJSON *fv = cJSON_CreateNumber((double)f);
    if (!fv) return -1;
    cJSON_AddItemToObjectCS(parent, "am_policy_features", fv);

    cJSON *feat = cJSON_CreateObject();
    if (!feat) return -1;

    char hex[2 + 16 + 1];
    (void)snprintf(hex, sizeof hex, "0x%016" PRIx64, f);
    cJSON *hx = cJSON_CreateString(hex);
    if (!hx) { cJSON_Delete(feat); return -1; }
    cJSON_AddItemToObjectCS(feat, "hex", hx);

    cJSON *bits = cJSON_CreateArray();
    cJSON *labels = cJSON_CreateArray();
    if (!bits || !labels) { if (bits) cJSON_Delete(bits); if (labels) cJSON_Delete(labels); cJSON_Delete(feat); return -1; }

    int i;
    for (i = 0; i < 64; i++) {
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

    cJSON_AddItemToObjectCS(feat, "bits", bits);
    cJSON_AddItemToObjectCS(feat, "labels", labels);
    cJSON_AddItemToObjectCS(parent, "am_policy_features_info", feat);
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
    if (!root) { if (buflen) buf[0] = '\0'; return 0; }

    cJSON *items = cJSON_CreateArray();
    if (!items) { cJSON_Delete(root); if (buflen) buf[0] = '\0'; return 0; }
    cJSON_AddItemToObjectCS(root, "items", items);

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

