/*
 * Copyright (C) 2026 by Evgenii Grigorev <tothe8c@gmail.com>
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
 * 3GPP TS 29.303 S-NAPTR gateway selection: pure record-processing logic.
 *
 * This module has no dependency on any DNS library or on MME context;
 * it operates on already-parsed NAPTR/SRV record structs so that the
 * selection rules are unit-testable in isolation.
 */

#ifndef MME_DNS_SELECT_H
#define MME_DNS_SELECT_H

#include "ogs-proto.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MME_DNS_MAX_CANDIDATES      8
#define MME_DNS_MAX_FQDN_LEN        255
#define MME_DNS_MAX_SERVICE_LEN     63
#define MME_DNS_MAX_FLAGS_LEN       7

typedef enum {
    MME_DNS_SVC_SGW,
    MME_DNS_SVC_PGW,
} mme_dns_service_e;

typedef enum {
    MME_DNS_PROTO_AUTO = 0,
    MME_DNS_PROTO_S5,
    MME_DNS_PROTO_S8,
} mme_dns_proto_e;

typedef struct mme_dns_naptr_record_s {
    uint16_t order;
    uint16_t preference;
    char flags[MME_DNS_MAX_FLAGS_LEN+1];
    char service[MME_DNS_MAX_SERVICE_LEN+1];
    char replacement[MME_DNS_MAX_FQDN_LEN+1];
} mme_dns_naptr_record_t;

typedef struct mme_dns_srv_record_s {
    uint16_t priority;
    uint16_t weight;    /* ignored: candidates are ordered by priority only */
    uint16_t port;
    char target[MME_DNS_MAX_FQDN_LEN+1];
} mme_dns_srv_record_t;

typedef struct mme_dns_candidate_s {
    /* Name to resolve with an A query, verbatim from the NAPTR/SRV answer
     * (topon./topoff. prefix kept - such names are resolvable as-is) */
    char fqdn[MME_DNS_MAX_FQDN_LEN+1];
    /* Canonical node name per TS 29.303 4.3.2 (topon./topoff. and the
     * following interface label stripped); used only as the dedup key */
    char canon[MME_DNS_MAX_FQDN_LEN+1];
    uint16_t order;         /* NAPTR sort keys */
    uint16_t preference;
    uint16_t port;          /* from SRV, 0 = use the default GTP-C port */
    bool needs_srv;         /* NAPTR flag "s": SRV lookup still pending */
    bool resolved;          /* addr is valid */
    ogs_sockaddr_t addr;
} mme_dns_candidate_t;

/*
 * APN-FQDN (TS 23.003 19.4.2.2):
 *   <APN-NI>.apn.epc.mnc<MNC>.mcc<MCC>.3gppnetwork.org
 * Returns OGS_OK or OGS_ERROR (overflow/invalid input).
 */
int mme_dns_build_apn_fqdn(char *buf, size_t sz,
        const char *apn_ni, const ogs_plmn_id_t *plmn_id);

/*
 * TAI-FQDN (TS 23.003 19.4.2.3):
 *   tac-lb<TAC-low-byte>.tac-hb<TAC-high-byte>.tac.epc
 *       .mnc<MNC>.mcc<MCC>.3gppnetwork.org
 * TAC bytes are lowercase hex, zero-padded to 2 digits.
 */
int mme_dns_build_tai_fqdn(char *buf, size_t sz, const ogs_eps_tai_t *tai);

/*
 * s5 vs s8 decision: s8 if and only if the IMSI does not begin with the
 * serving PLMN digits (MCC + MNC honoring mnc_len). A non-AUTO override
 * always wins.
 */
mme_dns_proto_e mme_dns_decide_proto(const char *imsi_bcd,
        const ogs_plmn_id_t *serving_plmn, mme_dns_proto_e override);

/*
 * Case-insensitive S-NAPTR service-field match. The field format is
 *   x-3gpp-<node>:x-<proto>[:x-<proto>]...
 * The first token must equal x-3gpp-sgw / x-3gpp-pgw and the remaining
 * tokens must contain the protocol selected by 'proto'
 * (x-s5-gtp / x-s8-gtp).
 */
bool mme_dns_service_match(const char *service,
        mme_dns_service_e svc, mme_dns_proto_e proto);

/*
 * Canonical node name: strip a leading "topon." or "topoff." label
 * together with the single following interface label (TS 29.303 4.3.2).
 * Copies the input unchanged when no prefix is present.
 */
void mme_dns_canonical_name(const char *fqdn, char *canon, size_t sz);

/*
 * Filter a NAPTR answer set by flags ("a"/"s" only) and service match,
 * stable-sort by (order, preference), dedup on the canonical name and
 * append up to max_cand candidates. Returns the candidate count.
 */
int mme_dns_candidates_from_naptr(
        const mme_dns_naptr_record_t *recs, int num_recs,
        mme_dns_service_e svc, mme_dns_proto_e proto,
        mme_dns_candidate_t *cand, int max_cand);

/*
 * Resolve an "s"-flagged candidate's target name from its SRV answer:
 * pick the lowest-priority record (RFC 2782 weight is ignored) and
 * clear needs_srv. Returns false - leaving the candidate untouched,
 * needs_srv still set - when the answer contains no usable target
 * (every record has an empty target or "." = service not available,
 * RFC 2782); the caller MUST then skip this candidate instead of
 * retrying it.
 */
bool mme_dns_candidate_apply_srv(mme_dns_candidate_t *cand,
        const mme_dns_srv_record_t *recs, int num_recs);

#ifdef __cplusplus
}
#endif

#endif /* MME_DNS_SELECT_H */
