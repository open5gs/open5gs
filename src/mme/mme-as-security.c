/* Copyright (C) 2026 Open5GS contributors
 * SPDX-License-Identifier: AGPL-3.0-or-later */
#include "mme-as-security.h"

static void from_nas(mme_as_security_t *out, const mme_ue_t *ue)
{
    memset(out, 0, sizeof(*out));
    out->valid = true;
    memcpy(out->kasme, ue->kasme, sizeof(out->kasme));
    out->ksi = ue->nas_eps.mme.ksi;
    memcpy(out->nh, ue->nh, sizeof(out->nh));
    out->ncc = ue->nhcc;
    out->enb_ue_id = ue->enb_ue_id;
}

void mme_as_security_prepare(mme_ue_t *ue, int procedure)
{
    if (ue->as_pending.valid) return;
    from_nas(&ue->as_pending, ue);
    ue->as_pending_procedure = procedure;
}

void mme_as_security_complete(mme_ue_t *ue, ogs_pool_id_t enb_ue_id,
        int procedure, bool success)
{
    if (!ue->as_pending.valid || ue->as_pending.enb_ue_id != enb_ue_id ||
            ue->as_pending_procedure != procedure) return;
    if (success) ue->as_security = ue->as_pending;
    memset(&ue->as_pending, 0, sizeof(ue->as_pending));
    ue->as_pending_procedure = 0;
}

void mme_as_security_release(mme_ue_t *ue, ogs_pool_id_t enb_ue_id)
{
    if (ue->as_pending.valid && ue->as_pending.enb_ue_id == enb_ue_id) {
        memset(&ue->as_pending, 0, sizeof(ue->as_pending));
        ue->as_pending_procedure = 0;
    }
    if (ue->as_security.valid && ue->as_security.enb_ue_id == enb_ue_id)
        memset(&ue->as_security, 0, sizeof(ue->as_security));
}

int mme_as_security_next_hop(mme_ue_t *ue)
{
    uint8_t nh[OGS_SHA256_DIGEST_SIZE];
    if (!ue->as_security.valid) {
        if (!SECURITY_CONTEXT_IS_VALID(ue) || ue->as_pending.valid)
            return OGS_ERROR;
        from_nas(&ue->as_security, ue);
    }
    ogs_kdf_nh_enb(ue->as_security.kasme, ue->as_security.nh, nh);
    memcpy(ue->as_security.nh, nh, sizeof(nh));
    ue->as_security.ncc = (ue->as_security.ncc + 1) & 7;
    if (!memcmp(ue->as_security.kasme, ue->kasme, sizeof(ue->kasme)) &&
            ue->as_security.ksi == ue->nas_eps.mme.ksi) {
        memcpy(ue->nh, nh, sizeof(nh));
        ue->nhcc = ue->as_security.ncc;
    }
    return OGS_OK;
}

void mme_as_security_export_old(const mme_ue_t *ue,
        ogs_gtp2_eps_mm_context_t *mm)
{
    if (!ue->as_security.valid ||
            (!memcmp(ue->as_security.kasme, ue->kasme, sizeof(ue->kasme)) &&
             ue->as_security.ksi == ue->nas_eps.mme.ksi)) return;
    mm->old_context_present = true;
    mm->old_ksi = ue->as_security.ksi;
    memcpy(mm->old_kasme, ue->as_security.kasme, sizeof(mm->old_kasme));
    mm->old_nh_present = true;
    memcpy(mm->old_nh, ue->as_security.nh, sizeof(mm->old_nh));
    mm->old_ncc = ue->as_security.ncc;
}

void mme_as_security_import(mme_ue_t *ue,
        const ogs_gtp2_eps_mm_context_t *mm)
{
    mme_as_security_t *as = &ue->as_security;
    memset(as, 0, sizeof(*as));
    as->valid = true;
    if (mm->old_context_present) {
        memcpy(as->kasme, mm->old_kasme, sizeof(as->kasme));
        as->ksi = mm->old_ksi;
        memcpy(as->nh, mm->old_nh, sizeof(as->nh));
        as->ncc = mm->old_ncc;
    } else {
        memcpy(as->kasme, mm->kasme, sizeof(as->kasme));
        as->ksi = mm->ksi;
        memcpy(as->nh, mm->nh, sizeof(as->nh));
        as->ncc = mm->ncc;
    }
}
