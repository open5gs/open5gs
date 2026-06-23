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
 *           "pdu_state": "inactive",
 *           "ue_location_timestamp": 1778223227627488
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
#include <inttypes.h>

#include "ogs-core.h"
#include "context.h"
#include "migration.h"
#include "pdu-info.h"
#include "sbi/openapi/external/cJSON.h"
#include "metrics/prometheus/json_pager.h"

#ifndef OGS_GTPV1_U_UDP_PORT
#define OGS_GTPV1_U_UDP_PORT 2152
#endif

#define SMF_TESTBED_API_VERSION "5g3e-smf-api/v1"
#define SMF_PDU_INFO_SCHEMA "5g3e.smf.pdu-info/v1"
#define SMF_UPF_INFO_SCHEMA "5g3e.smf.upf-info/v1"
#define SMF_UPF_ADMIN_SCHEMA "5g3e.smf.upf-admin/v1"

static void add_testbed_api_metadata(
        cJSON *root, const char *endpoint, const char *schema);

/*
 * UPF administrative state (research: explicit UPF member states required by
 * PROJECT_GOAL.md). A UPF marked "draining" stays PFCP-associated but is removed
 * from migration-target eligibility, so the controller can quiesce a UPF before
 * scale-in without racing its own bookkeeping. Kept as a small SMF-side table
 * keyed by the peer IP string (same representation /upf-info reports and
 * /pdu-migrate accepts) so the shared PFCP node struct is left untouched.
 */
#define SMF_UPF_ADMIN_TABLE_MAX 64
static struct {
    char addr[OGS_ADDRSTRLEN];
    bool draining;
} smf_upf_admin_table[SMF_UPF_ADMIN_TABLE_MAX];

int smf_upf_admin_is_draining(const char *addr)
{
    int i;
    if (!addr || !addr[0])
        return 0;
    for (i = 0; i < SMF_UPF_ADMIN_TABLE_MAX; i++) {
        if (smf_upf_admin_table[i].addr[0] &&
                strcmp(smf_upf_admin_table[i].addr, addr) == 0)
            return smf_upf_admin_table[i].draining ? 1 : 0;
    }
    return 0;
}

void smf_upf_admin_set(const char *addr, int draining)
{
    int i, free_slot = -1;
    if (!addr || !addr[0])
        return;
    for (i = 0; i < SMF_UPF_ADMIN_TABLE_MAX; i++) {
        if (smf_upf_admin_table[i].addr[0] == '\0') {
            if (free_slot < 0)
                free_slot = i;
            continue;
        }
        if (strcmp(smf_upf_admin_table[i].addr, addr) == 0) {
            smf_upf_admin_table[i].draining = draining ? true : false;
            return;
        }
    }
    if (!draining)
        return; /* clearing an unknown peer is a no-op */
    if (free_slot >= 0) {
        ogs_cpystrn(smf_upf_admin_table[free_slot].addr, addr, OGS_ADDRSTRLEN);
        smf_upf_admin_table[free_slot].draining = true;
    } else {
        ogs_warn("[UPF-ADMIN] admin table full; cannot mark %s draining", addr);
    }
}

size_t smf_handle_upf_admin(char *buf, size_t buflen,
        const char *method, const char *body, size_t body_len,
        int *status_code)
{
    cJSON *root = NULL, *request = NULL, *upf = NULL, *admin_state = NULL;
    const char *state_str = NULL;
    int draining = 0;
    size_t out;

    (void)body_len;

    if (!buf || buflen == 0)
        return 0;
    if (status_code)
        *status_code = 200;

    root = cJSON_CreateObject();
    if (!root) { if (buflen >= 3) { memcpy(buf, "{}", 3); return 2; } buf[0] = '\0'; return 0; }
    add_testbed_api_metadata(root, "/upf-admin", SMF_UPF_ADMIN_SCHEMA);

    if (!method || strcmp(method, "POST") != 0) {
        if (status_code) *status_code = 405;
        cJSON_AddStringToObject(root, "result", "rejected");
        cJSON_AddStringToObject(root, "reason", "post_required");
        goto finalize;
    }

    request = (body && body[0]) ? cJSON_Parse(body) : NULL;
    if (!request) {
        if (status_code) *status_code = 400;
        cJSON_AddStringToObject(root, "result", "rejected");
        cJSON_AddStringToObject(root, "reason", "invalid_json");
        goto finalize;
    }

    upf = cJSON_GetObjectItemCaseSensitive(request, "upf");
    admin_state = cJSON_GetObjectItemCaseSensitive(request, "admin_state");
    if (!cJSON_IsString(upf) || !upf->valuestring ||
            !cJSON_IsString(admin_state) || !admin_state->valuestring) {
        if (status_code) *status_code = 400;
        cJSON_AddStringToObject(root, "result", "rejected");
        cJSON_AddStringToObject(root, "reason", "upf_and_admin_state_required");
        cJSON_Delete(request);
        goto finalize;
    }

    state_str = admin_state->valuestring;
    if (strcmp(state_str, "draining") == 0) {
        draining = 1;
    } else if (strcmp(state_str, "active") == 0) {
        draining = 0;
    } else {
        if (status_code) *status_code = 400;
        cJSON_AddStringToObject(root, "result", "rejected");
        cJSON_AddStringToObject(root, "reason", "admin_state_must_be_draining_or_active");
        cJSON_Delete(request);
        goto finalize;
    }

    smf_upf_admin_set(upf->valuestring, draining);
    cJSON_AddStringToObject(root, "result", "accepted");
    cJSON_AddStringToObject(root, "upf", upf->valuestring);
    cJSON_AddStringToObject(root, "admin_state", draining ? "draining" : "active");
    cJSON_Delete(request);

finalize:
    {
        char *tmp = cJSON_PrintUnformatted(root);
        cJSON_Delete(root);
        if (!tmp) { if (buflen >= 3) { memcpy(buf, "{}", 3); return 2; } buf[0] = '\0'; return 0; }
        out = strlen(tmp);
        if (out >= buflen) out = buflen - 1;
        memcpy(buf, tmp, out);
        buf[out] = '\0';
        cJSON_free(tmp);
    }
    return out;
}

static void add_testbed_api_metadata(cJSON *root, const char *endpoint, const char *schema)
{
    if (!root)
        return;
    cJSON_AddStringToObject(root, "api_version", SMF_TESTBED_API_VERSION);
    cJSON_AddStringToObject(root, "schema", schema);
    cJSON_AddStringToObject(root, "endpoint", endpoint);
    cJSON_AddStringToObject(root, "integer_encoding", "numeric fields are kept for compatibility; *_str or *_us fields are integer-safe strings");
}

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

static cJSON *addr_string_from_sockaddr(ogs_sockaddr_t *sa4, ogs_sockaddr_t *sa6, int default_port)
{
    ogs_ip_t ip;
    if (!sa4 && !sa6)
        return NULL;

    memset(&ip, 0, sizeof(ip));
    if (OGS_OK != ogs_sockaddr_to_ip(sa4, sa6, &ip))
        return NULL;
    return addr_string_item(&ip, default_port);
}

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

static cJSON *build_migration_object_5g(const smf_sess_t *sess)
{
    cJSON *migration = NULL;
    cJSON *target = NULL;
    cJSON *target_n3 = NULL;
    cJSON *upf = NULL;

    if (!sess || sess->migration.state == SMF_MIGRATION_STATE_IDLE)
        return NULL;

    migration = cJSON_CreateObject();
    if (!migration) return NULL;

    if (sess->migration.id) {
        char idbuf[32];
        cJSON *id = cJSON_CreateNumber((double)sess->migration.id);
        cJSON *id_str = NULL;
        if (!id) { cJSON_Delete(migration); return NULL; }
        cJSON_AddItemToObjectCS(migration, "migration_id", id);

        snprintf(idbuf, sizeof idbuf, "%" PRIu64,
                (uint64_t)sess->migration.id);
        id_str = cJSON_CreateString(idbuf);
        if (!id_str) { cJSON_Delete(migration); return NULL; }
        cJSON_AddItemToObjectCS(migration, "migration_id_str", id_str);
    }

    if (sess->migration.source_node && sess->migration.source_node->addr_list) {
        cJSON *source = cJSON_CreateString(ogs_sockaddr_to_string_static(
                    sess->migration.source_node->addr_list));
        if (!source) { cJSON_Delete(migration); return NULL; }
        cJSON_AddItemToObjectCS(migration, "source_upf", source);
    }

    if (sess->migration.target_node && sess->migration.target_node->addr_list) {
        target = cJSON_CreateString(ogs_sockaddr_to_string_static(
                    sess->migration.target_node->addr_list));
        if (!target) { cJSON_Delete(migration); return NULL; }
        cJSON_AddItemToObjectCS(migration, "target_upf", target);
    }

    if (sess->migration.target_upf_n4_seid) {
        cJSON *seid = cJSON_CreateNumber(
                (double)sess->migration.target_upf_n4_seid);
        if (!seid) { cJSON_Delete(migration); return NULL; }
        cJSON_AddItemToObjectCS(migration, "target_upf_n4_seid", seid);

        char seidbuf[32];
        snprintf(seidbuf, sizeof seidbuf, "%" PRIu64,
                (uint64_t)sess->migration.target_upf_n4_seid);
        cJSON *seid_str = cJSON_CreateString(seidbuf);
        if (!seid_str) { cJSON_Delete(migration); return NULL; }
        cJSON_AddItemToObjectCS(migration, "target_upf_n4_seid_str", seid_str);
    }

    if (sess->migration.target_local_ul_teid ||
        sess->migration.target_local_ul_addr ||
        sess->migration.target_local_ul_addr6) {
        target_n3 = cJSON_CreateObject();
        if (!target_n3) { cJSON_Delete(migration); return NULL; }

        upf = cJSON_CreateObject();
        if (!upf) {
            cJSON_Delete(target_n3);
            cJSON_Delete(migration);
            return NULL;
        }

        if (sess->migration.target_local_ul_teid) {
            cJSON *teid = cJSON_CreateNumber(
                    (double)sess->migration.target_local_ul_teid);
            if (!teid) {
                cJSON_Delete(upf);
                cJSON_Delete(target_n3);
                cJSON_Delete(migration);
                return NULL;
            }
            cJSON_AddItemToObjectCS(upf, "teid", teid);
        }

        {
            cJSON *addr = addr_string_from_sockaddr(
                    sess->migration.target_local_ul_addr,
                    sess->migration.target_local_ul_addr6,
                    OGS_GTPV1_U_UDP_PORT);
            if (addr)
                cJSON_AddItemToObjectCS(upf, "addr", addr);
        }

        cJSON_AddItemToObjectCS(target_n3, "upf", upf);
        cJSON_AddItemToObjectCS(migration, "target_n3", target_n3);
    }

    {
        /* Live per-phase timings for research instrumentation. */
        cJSON *timings = smf_migration_timings_to_json(sess);
        if (timings)
            cJSON_AddItemToObjectCS(migration, "timings_us", timings);
    }

    if (migration->child == NULL) {
        cJSON_Delete(migration);
        return NULL;
    }

    return migration;
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

        cJSON *migration = build_migration_object_5g(sess);
        if (migration) cJSON_AddItemToObjectCS(pdu, "migration", migration);

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

    {
        cJSON *st = cJSON_CreateString(
                smf_migration_state_name(sess->migration.state));
        if (!st) { cJSON_Delete(pdu); return NULL; }
        cJSON_AddItemToObjectCS(pdu, "migration_state", st);
    }

    /* Last location update timestamp (epoch microseconds, ogs_time_t).
     * A value of 0 means the location has not yet been updated by the
     * peer NF for this session.
     * Inherited from the UE context at each N1/N2 location-bearing event
     * (Initial Registration, TAU/Registration Update, Handover, Service
     * Request response). Exposed for external reconciliation tools that
     * need to age PDU sessions — e.g. distinguish a truly stale session
     * from one that is legitimately idle-parked for later Resume. */
    {
        cJSON *ts = cJSON_CreateNumber((double)sess->ue_location_timestamp);
        if (!ts) { cJSON_Delete(pdu); return NULL; }
        cJSON_AddItemToObjectCS(pdu, "ue_location_timestamp", ts);

        char tsbuf[32];
        snprintf(tsbuf, sizeof tsbuf, "%" PRId64,
                (int64_t)sess->ue_location_timestamp);
        cJSON *ts_str = cJSON_CreateString(tsbuf);
        if (!ts_str) { cJSON_Delete(pdu); return NULL; }
        cJSON_AddItemToObjectCS(pdu, "ue_location_timestamp_us", ts_str);
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

/*
 * Concurrency invariant (load-bearing): the testbed dumpers below read live
 * smf_sess_t / migration / PFCP-peer state without locks. This is safe ONLY
 * because the Prometheus metrics server is NOT started with an MHD internal
 * thread. Its listen fd is registered on ogs_app()->pollset and MHD_run() is
 * driven from mhd_server_run() on the SMF main event loop (see
 * lib/metrics/prometheus/context.c). These handlers therefore run cooperatively
 * on the same single thread as the GSM/PFCP/NGAP state machines that mutate the
 * session, so no torn reads of SEID/TEID/node pointers can occur.
 *
 * If anyone ever adds MHD_USE_INTERNAL_POLLING_THREAD / MHD_USE_SELECT_INTERNALLY
 * to the metrics daemon, this invariant breaks and these reads must be moved
 * behind a snapshot taken on the main loop. Keep that in mind before changing
 * the metrics server threading model.
 */
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
    add_testbed_api_metadata(root, "/pdu-info", SMF_PDU_INFO_SCHEMA);

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

    /* Attach the recent-migration ring on the first page only; the exporter
     * reads it once and dedupes on seq. Avoids repeating it across pages. */
    if (start_index == 0) {
        cJSON *recent = smf_migration_recent_to_json();
        if (recent)
            cJSON_AddItemToObjectCS(root, "recent_migrations", recent);
    }

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

size_t smf_dump_upf_info(char *buf, size_t buflen, size_t page, size_t page_size)
{
    cJSON *root = NULL;
    cJSON *items = NULL;
    ogs_pfcp_node_t *node = NULL;
    bool oom = false;
    size_t emitted = 0;

    (void)page;
    (void)page_size;

    if (!buf || buflen == 0)
        return 0;

    root = cJSON_CreateObject();
    items = cJSON_CreateArray();
    if (!root || !items) {
        if (root) cJSON_Delete(root);
        if (items) cJSON_Delete(items);
        if (buflen >= 3) {
            memcpy(buf, "{}", 3);
            return 2;
        }
        buf[0] = '\0';
        return 0;
    }
    add_testbed_api_metadata(root, "/upf-info", SMF_UPF_INFO_SCHEMA);

    ogs_list_for_each(&ogs_pfcp_self()->pfcp_peer_list, node) {
        cJSON *item = NULL;
        cJSON *address = NULL;
        cJSON *pfcp = NULL;
        cJSON *associated = NULL;
        cJSON *ftup = NULL;
        cJSON *configured = NULL;
        cJSON *dynamic = NULL;
        char ipbuf[OGS_ADDRSTRLEN] = "";
        bool is_associated = false;

        if (!node || !node->addr_list)
            continue;

        is_associated = OGS_FSM_CHECK(&node->sm, smf_pfcp_state_associated);
        if (node->config_addr == NULL && !is_associated)
            continue;

        item = cJSON_CreateObject();
        address = cJSON_CreateString(OGS_ADDR(node->addr_list, ipbuf));
        pfcp = cJSON_CreateString(ogs_sockaddr_to_string_static(node->addr_list));
        associated = cJSON_CreateBool(is_associated);
        ftup = cJSON_CreateBool(node->up_function_features.ftup);
        configured = cJSON_CreateBool(node->config_addr != NULL);
        dynamic = cJSON_CreateBool(node->config_addr == NULL);
        if (!item || !address || !pfcp || !associated || !ftup ||
            !configured || !dynamic) {
            if (item) cJSON_Delete(item);
            if (address) cJSON_Delete(address);
            if (pfcp) cJSON_Delete(pfcp);
            if (associated) cJSON_Delete(associated);
            if (ftup) cJSON_Delete(ftup);
            if (configured) cJSON_Delete(configured);
            if (dynamic) cJSON_Delete(dynamic);
            oom = true;
            break;
        }

        cJSON_AddItemToObjectCS(item, "address", address);
        cJSON_AddItemToObjectCS(item, "pfcp", pfcp);
        cJSON_AddItemToObjectCS(item, "associated", associated);
        cJSON_AddItemToObjectCS(item, "ftup", ftup);
        cJSON_AddItemToObjectCS(item, "configured", configured);
        cJSON_AddItemToObjectCS(item, "dynamic", dynamic);
        cJSON_AddStringToObject(item, "admin_state",
                smf_upf_admin_is_draining(ipbuf) ? "draining" : "active");

        if (node->num_of_dnn > 0) {
            int i;
            cJSON *dnns = cJSON_CreateArray();
            if (!dnns) {
                cJSON_Delete(item);
                oom = true;
                break;
            }
            for (i = 0; i < node->num_of_dnn; i++) {
                cJSON *dnn = cJSON_CreateString(node->dnn[i] ? node->dnn[i] : "");
                if (!dnn) {
                    cJSON_Delete(dnns);
                    cJSON_Delete(item);
                    oom = true;
                    break;
                }
                cJSON_AddItemToArray(dnns, dnn);
            }
            if (oom)
                break;
            cJSON_AddItemToObjectCS(item, "dnn", dnns);
        }

        cJSON_AddItemToArray(items, item);
        emitted++;
    }

    cJSON_AddItemToObjectCS(root, "items", items);
    if (!cJSON_AddNumberToObject(root, "count", (double)emitted))
        oom = true;
    if (oom)
        cJSON_AddBoolToObject(root, "truncated", true);

    return json_pager_finalize(root, buf, buflen);
}
