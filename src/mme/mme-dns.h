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
 * Asynchronous DNS-based SGW/PGW selection (3GPP TS 29.303) for the MME.
 *
 * Active only when the `mme.dns` configuration section is present AND the
 * MME was built with c-ares. Without either, every entry point below is an
 * inert no-op and the static (configuration-file) gateway selection is
 * used, byte-for-byte identical to previous behavior.
 */

#ifndef MME_DNS_H
#define MME_DNS_H

#include "mme-context.h"
#include "mme-event.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Return values of mme_dns_prepare() */
#define MME_DNS_SEND_NOW    0   /* proceed with CSR immediately */
#define MME_DNS_DEFERRED    1   /* CSR will be sent on MME_EVENT_DNS_RESOLVED */

#ifdef MME_HAVE_CARES

int mme_dns_open(void);
void mme_dns_close(void);
bool mme_dns_enabled(void);

int mme_dns_prepare(enb_ue_t *enb_ue, mme_sess_t *sess, int create_action);

/* DNS-resolved PGW address for this session, or NULL */
ogs_sockaddr_t *mme_dns_sess_pgw_addr(mme_sess_t *sess);

/* Retry hooks: true = a next candidate was armed, caller must not fail
 * the procedure */
bool mme_dns_retry_on_csr_failure(mme_sess_t *sess);
bool mme_dns_retry_on_gtp_timeout(mme_sess_t *sess);

/* MME_EVENT_DNS_RESOLVED handler body (called from mme-sm.c) */
void mme_dns_handle_resolved(mme_event_t *e);

/* Called from mme_sess_remove() */
void mme_dns_sess_clear(mme_sess_t *sess);

#else /* !MME_HAVE_CARES */

static inline int mme_dns_open(void) { return OGS_OK; }
static inline void mme_dns_close(void) {}
static inline bool mme_dns_enabled(void) { return false; }
static inline int mme_dns_prepare(
        enb_ue_t *enb_ue, mme_sess_t *sess, int create_action)
{ return MME_DNS_SEND_NOW; }
static inline ogs_sockaddr_t *mme_dns_sess_pgw_addr(mme_sess_t *sess)
{ return NULL; }
static inline bool mme_dns_retry_on_csr_failure(mme_sess_t *sess)
{ return false; }
static inline bool mme_dns_retry_on_gtp_timeout(mme_sess_t *sess)
{ return false; }
static inline void mme_dns_handle_resolved(mme_event_t *e) {}
static inline void mme_dns_sess_clear(mme_sess_t *sess) {}

#endif /* MME_HAVE_CARES */

#ifdef __cplusplus
}
#endif

#endif /* MME_DNS_H */
