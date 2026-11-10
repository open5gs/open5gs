/* Copyright (C) 2026 Open5GS contributors
 * SPDX-License-Identifier: AGPL-3.0-or-later */
#ifndef MME_AS_SECURITY_H
#define MME_AS_SECURITY_H
#include "mme-context.h"

/* TS 33.401 7.2.8/7.2.10: the key used by the radio can remain based on
 * the previous KASME after NAS has activated a new security context. */
void mme_as_security_prepare(mme_ue_t *ue, int procedure);
void mme_as_security_release(mme_ue_t *ue, ogs_pool_id_t enb_ue_id);
void mme_as_security_complete(mme_ue_t *ue, ogs_pool_id_t enb_ue_id,
        int procedure, bool success);
int mme_as_security_next_hop(mme_ue_t *ue);
void mme_as_security_export_old(const mme_ue_t *ue,
        ogs_gtp2_eps_mm_context_t *mm);
void mme_as_security_import(mme_ue_t *ue,
        const ogs_gtp2_eps_mm_context_t *mm);
#endif
