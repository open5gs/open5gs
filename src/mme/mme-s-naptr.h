/*
 * Copyright (C) 2025 by Rami Mohamed <ramrode@gmail.com>
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
 * PGW/SMF discovery via S-NAPTR DNS procedure with in-process caching.
 *
 * Standards:
 *   3GPP TS 29.303  - Domain Name System Procedures (Stage 3)
 *   3GPP TS 23.003  - Numbering, addressing and identification
 *   3GPP TS 23.401  §5.3.2.1 - PGW selection and retry
 *   RFC 3958        - Domain-Based Application Service Location Using SRV RRs
 *                     and the Dynamic Delegation Discovery Service (DDDS)
 *
 * Resolution order per Create Session Request:
 *   1. Build APN-FQDN per TS 23.003 §19.4.2
 *   2. Check in-process DNS cache (keyed on APN-FQDN)
 *      → HIT:  return rotated copy of cached candidates (no DNS I/O)
 *      → MISS: proceed with steps 3-6
 *   3. DNS NAPTR query → find record with service "x-3gpp-pgw:x-gtp-v2"
 *   4. DNS SRV  query → resolve target hostname + port (weighted random)
 *   5. DNS A    query → resolve ALL IPv4 addresses; read TTL for cache expiry
 *   6. Store result in cache; return candidate array for this session
 *
 * The candidate array is stored in sess->pgw_candidates for per-session retry
 * per TS 23.401 §5.3.2.1.
 */

#ifndef MME_S_NAPTR_H
#define MME_S_NAPTR_H

#include "ogs-gtp.h"
#include "mme-context.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * mme_s_naptr_resolve_candidates() - resolve ALL PGW IPv4 candidates via
 * S-NAPTR for a given APN and home PLMN ID, with in-process DNS caching.
 *
 * Uses the DNS servers configured under mme.s5s8.dns in mme.yaml.
 *
 * On the first call for a given APN+PLMN the function runs the full DNS
 * NAPTR→SRV→A chain and caches the result keyed by APN-FQDN.  Subsequent
 * calls within the DNS TTL window return a freshly rotated copy of the
 * cached list, avoiding repeated DNS round-trips for the same APN.
 *
 * Returns a heap-allocated array of ogs_sockaddr_t (caller must ogs_free()
 * the array — typically in mme_sess_remove()).  Sets *count to the number
 * of entries.  Index 0 is the randomly-selected preferred candidate per
 * TS 23.401 §5.3.2.1; subsequent entries are ordered retry candidates.
 *
 * Returns NULL and sets *count = 0 on any failure.
 * All returned addresses have port set to OGS_GTPV2_C_UDP_PORT (2123).
 */
ogs_sockaddr_t *mme_s_naptr_resolve_candidates(
        const char *apn, const ogs_plmn_id_t *plmn_id, int *count);

/*
 * mme_s_naptr_cache_flush() - release all in-process DNS cache entries.
 *
 * Must be called from mme_context_final() on shutdown to free the cache and
 * prevent memory-leak reports from analysis tools such as valgrind.
 */
void mme_s_naptr_cache_flush(void);

#ifdef __cplusplus
}
#endif

#endif /* MME_S_NAPTR_H */
