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
 * Connected PDUs JSON dumper for the Prometheus HTTP server (/pdu-info).
 * - 5G PDUs:  psi+dnn, snssai, qos_flows [{qfi,5qi}], n3.{gnb,upf}, handover{}, pdu_state
 * - LTE PDUs: ebi(+psi if non-zero)+apn, qos_flows [{ebi,qci}], pdu_state ("unknown" at SMF scope)
 * - UE-level: ue_activity ("active"/"unknown"/"idle")
 * - pager: /pdu-info?page=0&page_size=100 (0-based, page=SIZE_MAX -> no paging)
 *
 * path: http://SMF_IP:9090/pdu-info
 *
 * curl -s "http://127.0.0.4:9090/pdu-info?page_size=1" |jq . 
 * {
 *   "items": [
 *     {
 *       "supi": "imsi-231510000114763",
 *       "pdu": [
 *         {
 *           "psi": 1,
 *           "dnn": "internet",
 *           "ipv4": "10.45.0.11",
 *           "snssai": {
 *             "sst": 1,
 *             "sd": "ffffff"
 *           },
 *           "qos_flows": [
 *             {
 *               "qfi": 1,
 *               "5qi": 9
 *             }
 *           ],
 *           "n3": {
 *             "gnb": {
 *               "teid": 76,
 *               "addr": "[192.168.168.100]:2152"
 *             },
 *             "upf": {
 *               "teid": 11426,
 *               "addr": "[192.168.168.7]:2152",
 *               "pdr_id": 2
 *             }
 *           },
 *           "pdu_state": "inactive"
 *         }
 *       ],
 *       "ue_activity": "idle"
 *     }
 *   ],
 *   "pager": {
 *     "page": 0,
 *     "page_size": 100,
 *     "count": 1,
 *   }
 * }
 */ 

#include <string.h>
#include <stdbool.h>
#include <limits.h>

#include "ogs-core.h"
#include "context.h"
#include "pdu-info.h"
#include "sbi/openapi/external/cJSON.h"
#include "metrics/prometheus/json_pager.h"

#ifndef OGS_GTPV1_U_UDP_PORT
#define OGS_GTPV1_U_UDP_PORT 2152
#endif

/* Only used in ip_is_unspecified (currently disabled) */
/* static const uint8_t zero6[OGS_IPV6_LEN] = {0}; */

static int ip_to_text(const ogs_ip_t *ip, char *out, size_t outlen)
{
    const char *ret;

    if (!ip || !out || outlen == 0)
        return 0;

    out[0] = '\0';

    if (ip->ipv4) {
        ret = OGS_INET_NTOP(&ip->addr, out);
        if (ret)
            return 1;
    }

    if (ip->ipv6) {
        ret = OGS_INET6_NTOP(ip->addr6, out);
        if (ret)
            return 1;
    }

    return 0;
}

/* Only used in handover function (currently disabled) */
/*
static bool ip_is_unspecified(const ogs_ip_t *ip)
{
    if (!ip)
        return true;

    if (ip->ipv4 && ip->addr == 0)
        return true;

    if (ip->ipv6 && memcmp(ip->addr6, zero6, sizeof(zero6)) == 0)
        return true;

    if (!ip->ipv4 && !ip->ipv6)
        return true;

    return false;
}
*/

static cJSON *addr_string_item(const ogs_ip_t *ip, int port)
{
    if (!ip) return NULL;
    char ipbuf[OGS_ADDRSTRLEN] = "";
    if (!ip_to_text(ip, ipbuf, sizeof ipbuf)) return NULL;
    char buf[OGS_ADDRSTRLEN + 16];
    snprintf(buf, sizeof buf, "[%s]:%d", ipbuf, port);
    return cJSON_CreateString(buf);
}

/* Only used in handover function (currently disabled) */
/*
static cJSON *addr_string_from_sockaddr(ogs_sockaddr_t *sa4, ogs_sockaddr_t *sa6, int default_port)
{
    ogs_ip_t ip;
    memset(&ip, 0, sizeof(ip));
    if (OGS_OK != ogs_sockaddr_to_ip(sa4, sa6, &ip))
        return NULL;
    return addr_string_item(&ip, default_port);
}
*/

static inline uint32_t u24_to_u32(ogs_uint24_t v)
{
    uint32_t x = 0;
    memcpy(&x, &v, sizeof(v) < sizeof(x) ? sizeof(v) : sizeof(x));
    return (x & 0xFFFFFFu);
}

static inline int up_state_of(const smf_sess_t *s)
{
    if (!s) return 0;
    int u = (int)s->up_cnx_state;
    if (u == 0) u = (int)s->nsmf_param.up_cnx_state;
    return u;
}

static inline bool has_n3_teid(const smf_sess_t *s)
{
    return s && (s->remote_ul_teid != 0U || s->remote_dl_teid != 0U);
}

static inline bool bearer_list_has_qfi(const smf_sess_t *s)
{
    if (!s) return false;
    smf_bearer_t *b = NULL;
    ogs_list_for_each(&((smf_sess_t *)s)->bearer_list, b) {
        if (b && b->qfi > 0) return true;
    }
    return false;
}

/* 5G heuristic: S-NSSAI present or any QFI bearer */
static inline bool looks_5g_sess(const smf_sess_t *s)
{
    if (!s) return false;
    if (s->s_nssai.sst != 0) return true;
    if (u24_to_u32(s->s_nssai.sd) != 0) return true;
    if (bearer_list_has_qfi(s)) return true;
    return false;
}

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

/* LTE/EPC state at SMF scope: unknown */
static const char *pdu_state_from_lte(const smf_sess_t *sess)
{
    (void)sess;
    return "unknown";
}

/* ---------- N3 object (gNB from sess->remote_dl_*, UPF from PDR F-TEID) ---------- */

static cJSON *build_n3_object_5g(const smf_sess_t *sess)
{
    /* Build n3 object with "gnb" and "upf" children. If nothing to emit or OOM, return NULL. */
    if (!sess) return NULL;

    cJSON *n3  = cJSON_CreateObject();
    if (!n3) return NULL;

    /* -------- gNB (DL endpoint) from sess->remote_dl_* -------- */
    if (sess->remote_dl_teid || sess->remote_dl_ip.ipv4 || sess->remote_dl_ip.ipv6) {
        cJSON *gnb = cJSON_CreateObject();
        if (!gnb) { cJSON_Delete(n3); return NULL; }
        bool has_content = false;

        if (sess->remote_dl_teid) {
            cJSON *t = cJSON_CreateNumber((double)(unsigned)sess->remote_dl_teid);
            if (!t) { cJSON_Delete(gnb); cJSON_Delete(n3); return NULL; }
            cJSON_AddItemToObjectCS(gnb, "teid", t);
            has_content = true;
        }

        cJSON *gs = addr_string_item(&sess->remote_dl_ip, OGS_GTPV1_U_UDP_PORT);
        if (gs) {
            cJSON_AddItemToObjectCS(gnb, "addr", gs);
            has_content = true;
        }

        if (has_content) {
            cJSON_AddItemToObjectCS(n3, "gnb", gnb);
        } else {
            cJSON_Delete(gnb);
        }
    }

    /* -------- UPF (UL endpoint) from PFCP PDR F-TEID -------- */
    do {
        ogs_pfcp_pdr_t *pdr = NULL, *pick = NULL;
        ogs_list_for_each(&((smf_sess_t *)sess)->pfcp.pdr_list, pdr) {
            if (pdr && pdr->f_teid_len > 0 &&
                pdr->src_if == OGS_PFCP_INTERFACE_ACCESS) { pick = pdr; break; }
        }
        if (!pick) {
            ogs_list_for_each(&((smf_sess_t *)sess)->pfcp.pdr_list, pdr) {
                if (pdr && pdr->f_teid_len > 0) { pick = pdr; break; }
            }
        }
        if (!pick) break;

        ogs_ip_t ip;
        memset(&ip, 0, sizeof(ip));

        if (OGS_OK == ogs_pfcp_f_teid_to_ip(&pick->f_teid, &ip)) {
            cJSON *upf = cJSON_CreateObject();
            if (!upf) { cJSON_Delete(n3); return NULL; }

            bool has_content = false;

            if (pick->f_teid.teid) {
                cJSON *t = cJSON_CreateNumber((double)(unsigned)pick->f_teid.teid);
                if (!t) { cJSON_Delete(upf); cJSON_Delete(n3); return NULL; }
                cJSON_AddItemToObjectCS(upf, "teid", t);
                has_content = true;
            }

            cJSON *addr = addr_string_item(&ip, OGS_GTPV1_U_UDP_PORT);
            if (addr) {
                cJSON_AddItemToObjectCS(upf, "addr", addr);
                has_content = true;
            }

            if (pick->id) {
                cJSON *pid = cJSON_CreateNumber((double)(unsigned)pick->id);
                if (!pid) { cJSON_Delete(upf); cJSON_Delete(n3); return NULL; }
                cJSON_AddItemToObjectCS(upf, "pdr_id", pid);
                has_content = true;
            }

            if (has_content) {
                cJSON_AddItemToObjectCS(n3, "upf", upf);
            } else {
                cJSON_Delete(upf);
            }
        }
    } while (0);

    if (n3->child == NULL) { cJSON_Delete(n3); return NULL; }
    return n3;
}

/* Handover function disabled */
/*
static cJSON *build_handover_object_5g(const smf_sess_t *sess)
{
    if (!sess) return NULL;
    int any = 0;
    cJSON *ho = cJSON_CreateObject();
    if (!ho) return NULL;

    if (sess->handover.prepared) {
        cJSON *b = cJSON_CreateBool(1);
        if (!b) { cJSON_Delete(ho); return NULL; }
        cJSON_AddItemToObjectCS(ho, "prepared", b);
        any = 1;
    }
    if (sess->handover.indirect_data_forwarding) {
        cJSON *b = cJSON_CreateBool(1);
        if (!b) { cJSON_Delete(ho); return NULL; }
        cJSON_AddItemToObjectCS(ho, "indirect_data_forwarding", b);
        any = 1;
    }
    if (sess->handover.data_forwarding_not_possible) {
        cJSON *b = cJSON_CreateBool(1);
        if (!b) { cJSON_Delete(ho); return NULL; }
        cJSON_AddItemToObjectCS(ho, "data_forwarding_not_possible", b);
        any = 1;
    }

    // Target gNB (N3 for target cell)
    if (sess->handover.gnb_n3_teid || !ip_is_unspecified(&sess->handover.gnb_n3_ip)) {
        cJSON *gnb = cJSON_CreateObject();
        if (!gnb) { cJSON_Delete(ho); return NULL; }
        bool has_content = false;

        if (sess->handover.gnb_n3_teid) {
            cJSON *t = cJSON_CreateNumber((double)(unsigned)sess->handover.gnb_n3_teid);
            if (!t) { cJSON_Delete(gnb); cJSON_Delete(ho); return NULL; }
            cJSON_AddItemToObjectCS(gnb, "teid", t);
            has_content = true;
        }
        cJSON *addr = addr_string_item(&sess->handover.gnb_n3_ip, OGS_GTPV1_U_UDP_PORT);
        if (addr) {
            cJSON_AddItemToObjectCS(gnb, "addr", addr);
            has_content = true;
        }

        if (has_content) {
            cJSON_AddItemToObjectCS(ho, "target_gnb", gnb);
            any = 1;
        } else {
            cJSON_Delete(gnb);
        }
    }

    if (sess->handover.local_dl_teid || sess->handover.remote_dl_teid ||
        sess->handover.local_dl_addr || sess->handover.local_dl_addr6 ||
        !ip_is_unspecified(&sess->handover.remote_dl_ip)) {

        cJSON *fwd = cJSON_CreateObject();
        if (!fwd) { cJSON_Delete(ho); return NULL; }

        // Local endpoint (SMF side of indirect forwarding tunnel)
        if (sess->handover.local_dl_teid || sess->handover.local_dl_addr || sess->handover.local_dl_addr6) {
            cJSON *loc = cJSON_CreateObject();
            if (!loc) { cJSON_Delete(fwd); cJSON_Delete(ho); return NULL; }
            bool has_content = false;

            if (sess->handover.local_dl_teid) {
                cJSON *t = cJSON_CreateNumber((double)(unsigned)sess->handover.local_dl_teid);
                if (!t) { cJSON_Delete(loc); cJSON_Delete(fwd); cJSON_Delete(ho); return NULL; }
                cJSON_AddItemToObjectCS(loc, "teid", t);
                has_content = true;
            }
            cJSON *laddr = addr_string_from_sockaddr(sess->handover.local_dl_addr,
                                                     sess->handover.local_dl_addr6,
                                                     OGS_GTPV1_U_UDP_PORT);
            if (laddr) {
                cJSON_AddItemToObjectCS(loc, "addr", laddr);
                has_content = true;
            }

            if (has_content) {
                cJSON_AddItemToObjectCS(fwd, "local", loc);
            } else {
                cJSON_Delete(loc);
            }
        }

        // Remote endpoint (UPF side of indirect forwarding tunnel)
        if (sess->handover.remote_dl_teid || !ip_is_unspecified(&sess->handover.remote_dl_ip)) {
            cJSON *rem = cJSON_CreateObject();
            if (!rem) { cJSON_Delete(fwd); cJSON_Delete(ho); return NULL; }
            bool has_content = false;

            if (sess->handover.remote_dl_teid) {
                cJSON *t = cJSON_CreateNumber((double)(unsigned)sess->handover.remote_dl_teid);
                if (!t) { cJSON_Delete(rem); cJSON_Delete(fwd); cJSON_Delete(ho); return NULL; }
                cJSON_AddItemToObjectCS(rem, "teid", t);
                has_content = true;
            }
            cJSON *raddr = addr_string_item(&sess->handover.remote_dl_ip, OGS_GTPV1_U_UDP_PORT);
            if (raddr) {
                cJSON_AddItemToObjectCS(rem, "addr", raddr);
                has_content = true;
            }

            if (has_content) {
                cJSON_AddItemToObjectCS(fwd, "upf", rem);
            } else {
                cJSON_Delete(rem);
            }
        }

        if (fwd->child != NULL) {
            cJSON_AddItemToObjectCS(ho, "dl_forwarding", fwd);
            any = 1;
        } else {
            cJSON_Delete(fwd);
        }
    }

    if (!any) { cJSON_Delete(ho); return NULL; }
    return ho;
}
*/

static cJSON *build_snssai_object(const smf_sess_t *sess)
{
    cJSON *sn = cJSON_CreateObject();
    if (!sn) return NULL;

    cJSON *sst = cJSON_CreateNumber((double)sess->s_nssai.sst);
    if (!sst) { cJSON_Delete(sn); return NULL; }
    cJSON_AddItemToObjectCS(sn, "sst", sst);

    char sd[7];
    snprintf(sd, sizeof sd, "%06x", (unsigned)u24_to_u32(sess->s_nssai.sd));
    cJSON *sdj = cJSON_CreateString(sd);
    if (!sdj) { cJSON_Delete(sn); return NULL; }
    cJSON_AddItemToObjectCS(sn, "sd", sdj);

    return sn;
}

static cJSON *build_qos_flows_array_5g(const smf_sess_t *sess)
{
    cJSON *arr = cJSON_CreateArray();
    if (!arr) return NULL;

    smf_bearer_t *b = NULL;
    ogs_list_for_each(&((smf_sess_t *)sess)->bearer_list, b) {
        if (!b || b->qfi == 0) continue;

        cJSON *q = cJSON_CreateObject();
        if (!q) { cJSON_Delete(arr); return NULL; }

        cJSON *qfi = cJSON_CreateNumber((double)(unsigned)b->qfi);
        if (!qfi) { cJSON_Delete(q); cJSON_Delete(arr); return NULL; }
        cJSON_AddItemToObjectCS(q, "qfi", qfi);

        if (b->qos.index > 0) {
            cJSON *q5 = cJSON_CreateNumber((double)(unsigned)b->qos.index);
            if (!q5) { cJSON_Delete(q); cJSON_Delete(arr); return NULL; }
            cJSON_AddItemToObjectCS(q, "5qi", q5);
        }

        cJSON_AddItemToArray(arr, q);
    }

    return arr;
}

static cJSON *build_qos_flows_array_lte(const smf_sess_t *sess)
{
    cJSON *arr = cJSON_CreateArray();
    if (!arr) return NULL;

    smf_bearer_t *b = NULL;
    ogs_list_for_each(&((smf_sess_t *)sess)->bearer_list, b) {
        if (!b || b->ebi == 0) continue;

        unsigned qci_val = (unsigned)b->qos.index;
        if (qci_val == 0) qci_val = (unsigned)sess->session.qos.index;

        cJSON *q = cJSON_CreateObject();
        if (!q) { cJSON_Delete(arr); return NULL; }

        cJSON *ebi = cJSON_CreateNumber((double)(unsigned)b->ebi);
        if (!ebi) { cJSON_Delete(q); cJSON_Delete(arr); return NULL; }
        cJSON_AddItemToObjectCS(q, "ebi", ebi);

        if (qci_val > 0) {
            cJSON *qci = cJSON_CreateNumber((double)qci_val);
            if (!qci) { cJSON_Delete(q); cJSON_Delete(arr); return NULL; }
            cJSON_AddItemToObjectCS(q, "qci", qci);
        }

        cJSON_AddItemToArray(arr, q);
    }

    return arr;
}

static cJSON *build_single_pdu_object(const smf_sess_t *sess, int *any_active, int *any_unknown)
{
    cJSON *pdu = cJSON_CreateObject();
    if (!pdu) return NULL;

    /* 5G vs LTE fields */
    const bool is5g = looks_5g_sess(sess);
    if (is5g) {
        cJSON *psi = cJSON_CreateNumber((double)(unsigned)sess->psi);
        if (!psi) { cJSON_Delete(pdu); return NULL; }
        cJSON_AddItemToObjectCS(pdu, "psi", psi);

        const char *dnn_c = (sess->session.name ? sess->session.name : "");
        cJSON *dnn = cJSON_CreateString(dnn_c);
        if (!dnn) { cJSON_Delete(pdu); return NULL; }
        cJSON_AddItemToObjectCS(pdu, "dnn", dnn);
    } else {
        if (sess->psi > 0) {
            cJSON *psi = cJSON_CreateNumber((double)(unsigned)sess->psi);
            if (!psi) { cJSON_Delete(pdu); return NULL; }
            cJSON_AddItemToObjectCS(pdu, "psi", psi);
        }

        /* EBI root if present */
        unsigned ebi_root = 0;
        smf_bearer_t *b0 = NULL;
        ogs_list_for_each(&((smf_sess_t *)sess)->bearer_list, b0) {
            if (b0 && b0->ebi > 0) { ebi_root = (unsigned)b0->ebi; break; }
        }
        cJSON *ebi = cJSON_CreateNumber((double)ebi_root);
        if (!ebi) { cJSON_Delete(pdu); return NULL; }
        cJSON_AddItemToObjectCS(pdu, "ebi", ebi);

        const char *apn_c = (sess->session.name ? sess->session.name : "");
        cJSON *apn = cJSON_CreateString(apn_c);
        if (!apn) { cJSON_Delete(pdu); return NULL; }
        cJSON_AddItemToObjectCS(pdu, "apn", apn);
    }

    /* IPs */
    {
        char ip4[OGS_ADDRSTRLEN] = "";
        char ip6[OGS_ADDRSTRLEN] = "";
        if (sess->ipv4) OGS_INET_NTOP(&sess->ipv4->addr, ip4);
        if (sess->ipv6) OGS_INET6_NTOP(&sess->ipv6->addr, ip6);

        if (ip4[0]) {
            cJSON *s = cJSON_CreateString(ip4);
            if (!s) { cJSON_Delete(pdu); return NULL; }
            cJSON_AddItemToObjectCS(pdu, "ipv4", s);
        }
        if (ip6[0]) {
            cJSON *s = cJSON_CreateString(ip6);
            if (!s) { cJSON_Delete(pdu); return NULL; }
            cJSON_AddItemToObjectCS(pdu, "ipv6", s);
        }
    }

    /* S-NSSAI */
    {
        cJSON *sn = build_snssai_object(sess);
        if (!sn) { cJSON_Delete(pdu); return NULL; }
        cJSON_AddItemToObjectCS(pdu, "snssai", sn);
    }

    /* QoS flows */
    {
        cJSON *qarr = is5g ? build_qos_flows_array_5g(sess)
                           : build_qos_flows_array_lte(sess);
        if (!qarr) { cJSON_Delete(pdu); return NULL; }
        cJSON_AddItemToObjectCS(pdu, "qos_flows", qarr);
    }

    /* N3 + Handover (5GS only) */
    if (is5g) {
        cJSON *n3 = build_n3_object_5g(sess);
        if (n3) cJSON_AddItemToObjectCS(pdu, "n3", n3);

        /* Handover disabled */
        /*
        cJSON *ho = build_handover_object_5g(sess);
        if (ho) cJSON_AddItemToObjectCS(pdu, "handover", ho);
        */
    }

    /* PDU state + UE activity aggregation */
    {
        const char *state = is5g ? pdu_state_from_5g(sess) : pdu_state_from_lte(sess);
        cJSON *st = cJSON_CreateString(state);
        if (!st) { cJSON_Delete(pdu); return NULL; }
        cJSON_AddItemToObjectCS(pdu, "pdu_state", st);

        if (any_active && !strcmp(state, "active")) *any_active = 1;
        else if (any_unknown && !strcmp(state, "unknown")) *any_unknown = 1;
    }

    return pdu;
}

static cJSON *build_ue_object(const smf_ue_t *ue)
{
    cJSON *ueo = cJSON_CreateObject();
    if (!ueo) return NULL;

    /* UE identity */
    const char *id = (ue->supi && ue->supi[0]) ? ue->supi :
                     (ue->imsi_bcd[0] ? ue->imsi_bcd : "");
    cJSON *idj = cJSON_CreateString(id);
    if (!idj) { cJSON_Delete(ueo); return NULL; }
    cJSON_AddItemToObjectCS(ueo, "supi", idj);

    /* PDUs */
    cJSON *pdus = cJSON_CreateArray();
    if (!pdus) { cJSON_Delete(ueo); return NULL; }

    int any_active = 0, any_unknown = 0;

    smf_sess_t *sess = NULL;
    ogs_list_for_each(&ue->sess_list, sess) {
        cJSON *pdu = build_single_pdu_object(sess, &any_active, &any_unknown);
        if (!pdu) { cJSON_Delete(pdus); cJSON_Delete(ueo); return NULL; }
        cJSON_AddItemToArray(pdus, pdu);
    }
    cJSON_AddItemToObjectCS(ueo, "pdu", pdus);

    /* UE activity */
    {
        const char *ue_act = any_active ? "active" : (any_unknown ? "unknown" : "idle");
        cJSON *ua = cJSON_CreateString(ue_act);
        if (!ua) { cJSON_Delete(ueo); return NULL; }
        cJSON_AddItemToObjectCS(ueo, "ue_activity", ua);
    }

    return ueo;
}

size_t smf_dump_pdu_info_paged(char *buf, size_t buflen, size_t page, size_t page_size)
{
    if (!buf || buflen == 0) return 0;

    const bool no_paging = (page == SIZE_MAX);
    if (!no_paging) {
        if (page_size == 0) page_size = PDU_INFO_PAGE_SIZE_DEFAULT;
        if (page_size > PDU_INFO_PAGE_SIZE_DEFAULT) page_size = PDU_INFO_PAGE_SIZE_DEFAULT;
    } else {
        page_size = SIZE_MAX;
        page = 0;
    }

    const size_t start_index = json_pager_safe_start_index(no_paging, page, page_size);

    cJSON *root = cJSON_CreateObject();
    if (!root) { if (buflen >= 3) { memcpy(buf, "{}", 3); return 2; } if (buflen) buf[0] = '\0'; return 0; }

    cJSON *items = cJSON_CreateArray();
    if (!items) { cJSON_Delete(root); if (buflen >= 3) { memcpy(buf, "{}", 3); return 2; } if (buflen) buf[0] = '\0'; return 0; }

    size_t idx = 0, emitted = 0;
    bool has_next = false, oom = false;

    smf_context_t *smf = smf_self();
    smf_ue_t *ue = NULL;

    ogs_list_for_each(&smf->smf_ue_list, ue) {
        int act = json_pager_advance(no_paging, idx, start_index, emitted, page_size, &has_next);
        if (act == 1) { idx++; continue; }
        if (act == 2) break;

        cJSON *ueo = build_ue_object(ue);
        if (!ueo) { oom = true; break; }

        cJSON_AddItemToArray(items, ueo);
        emitted++;
        idx++;
    }

    cJSON_AddItemToObjectCS(root, "items", items);
    json_pager_add_trailing(root, no_paging, page, page_size, emitted, has_next && !oom, "/pdu-info", oom);

    return json_pager_finalize(root, buf, buflen);
}

size_t smf_dump_pdu_info(char *buf, size_t buflen, size_t page, size_t page_size)
{
    if (page == SIZE_MAX) {
        page = 0;
        page_size = PDU_INFO_PAGE_SIZE_DEFAULT;
    } else if (page_size == 0) {
        page_size = PDU_INFO_PAGE_SIZE_DEFAULT;
    }

    return smf_dump_pdu_info_paged(buf, buflen, page, page_size);
}

