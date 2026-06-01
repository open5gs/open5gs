/*
 * Copyright (C) 2024 by sysmocom - s.f.m.c. GmbH <info@sysmocom.de>
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

#ifndef MME_DNS_H
#define MME_DNS_H

#include "ogs-proto.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MME_DNS_MAX_FLAGS       16
#define MME_DNS_MAX_SERVICES    256
/* A DNS name is at most 255 octets (RFC 1035); +1 for the terminator. */
#define MME_DNS_MAX_NAME        256

/* A single parsed NAPTR record (subset relevant to S-NAPTR). */
typedef struct mme_dns_naptr_s {
    uint16_t    order;
    uint16_t    preference;
    char        flags[MME_DNS_MAX_FLAGS];
    char        services[MME_DNS_MAX_SERVICES];
    char        replacement[MME_DNS_MAX_NAME];
    uint32_t    ttl;
} mme_dns_naptr_t;

/*
 * TS 29.303 DNS S-NAPTR based EPC node discovery (PGW S5/S8 selection).
 *
 * mme_dns_init()/mme_dns_final() set up the resolver state and the TTL cache.
 * They are safe to call even when discovery is disabled.
 */
void mme_dns_init(void);
void mme_dns_final(void);

/*
 * Resolve the PGW GTP-C (S5/S8) address for the given APN network identifier
 * and the subscriber's HOME PLMN, following the S-NAPTR procedure of
 * TS 29.303 (NAPTR -> optional SRV -> A/AAAA) on the APN-FQDN
 *   <apn>.apn.epc.mnc<MNC>.mcc<MCC>.3gppnetwork.org
 *
 * On success returns OGS_OK and sets *sa_list to a newly-allocated address
 * list (mixed IPv4/IPv6, best candidate first). The caller owns the list and
 * MUST release it with ogs_freeaddrinfo().
 *
 * Returns OGS_ERROR when discovery is disabled, the inputs are invalid, or no
 * usable record could be resolved. Results (including failures) are cached
 * per FQDN with the DNS TTL so repeated attaches do not block on DNS.
 */
int mme_dns_resolve_pgw(
        const char *apn, const ogs_plmn_id_t *home_plmn,
        ogs_sockaddr_t **sa_list);

/*
 * The following are pure, side-effect-free helpers exposed for unit testing
 * (see tests/unit/mme-dns-test.c). They do not depend on the resolver state
 * or the global MME context.
 */

/*
 * Return true if a NAPTR Service field selects a PGW for the given interface
 * mode (MME_PGW_DISCOVERY_IFACE_S8 / _S5 / _BOTH). Matching is
 * case-insensitive per RFC 3958.
 */
bool mme_dns_service_matches(const char *services, int iface);

/*
 * Build the TS 23.003 APN-FQDN
 *   <apn-NI>.apn.epc.mnc<MNC>.mcc<MCC>.3gppnetwork.org
 * for the subscriber's home PLMN. Returns OGS_OK on success.
 */
int mme_dns_apn_fqdn(const char *apn, const ogs_plmn_id_t *home_plmn,
        char *buf, size_t sz);

/*
 * Parse a raw DNS NAPTR response message, keeping only records that match the
 * requested interface mode, sorted by NAPTR Order then Preference. Returns the
 * number of records written to 'out' (capped at 'max').
 */
int mme_dns_parse_naptr(const unsigned char *answer, int answer_len,
        int iface, mme_dns_naptr_t *out, int max);

#ifdef __cplusplus
}
#endif

#endif /* MME_DNS_H */
