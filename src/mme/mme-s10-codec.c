/* Copyright (C) 2026 Open5GS contributors
 * SPDX-License-Identifier: AGPL-3.0-or-later */
#include "mme-s10.h"

/* TS 29.274 8.51.3-5: the identifier is right-aligned in the GTP IE;
 * the S1AP BIT STRING representation is left-aligned. */
int mme_s10_encode_target(ogs_tlv_octet_t *ie,
        const S1AP_TargeteNB_ID_t *target, uint8_t buffer[10])
{
    const S1AP_Global_ENB_ID_t *global;
    uint32_t id;
    size_t n;
    if (!target || !(global = target->global_ENB_ID) ||
            !global->pLMNidentity.buf || global->pLMNidentity.size != 3 ||
            !global->eNB_ID ||
            !target->selected_TAI || target->selected_TAI->tAC.size != 2 ||
            !target->selected_TAI->tAC.buf ||
            ogs_s1ap_ENB_ID_to_uint32(global->eNB_ID, &id) != OGS_OK)
        return OGS_ERROR;
    memset(buffer, 0, 10);
    memcpy(buffer + 1, global->pLMNidentity.buf, 3);
    switch (global->eNB_ID->present) {
    case S1AP_ENB_ID_PR_macroENB_ID:
        buffer[0] = 1;
        n = 9;
        break;
    case S1AP_ENB_ID_PR_homeENB_ID:
        buffer[0] = 3;
        n = 10;
        break;
    case S1AP_ENB_ID_PR_short_macroENB_ID:
        buffer[0] = 4;
        buffer[4] = 0x80;
        n = 9;
        break;
    case S1AP_ENB_ID_PR_long_macroENB_ID:
        buffer[0] = 4;
        n = 9;
        break;
    default:
        return OGS_ERROR;
    }
    if (n == 10) buffer[4] = (id >> 24) & 15;
    buffer[n - 5] |= id >> 16;
    buffer[n - 4] = id >> 8;
    buffer[n - 3] = id;
    memcpy(buffer + n - 2, target->selected_TAI->tAC.buf, 2);
    ie->presence = 1;
    ie->data = buffer;
    ie->len = n;
    return OGS_OK;
}

int mme_s10_decode_target(mme_s10_target_t *target, const ogs_tlv_octet_t *ie)
{
    const uint8_t *p = ie->data;
    mme_s10_target_t out = {0};
    uint16_t tac;
    if (!ie->presence || !p || ie->len < 9) return OGS_ERROR;
    memcpy(&out.enb_plmn, p + 1, 3);
    out.tai.plmn_id = out.enb_plmn;
    switch (p[0]) {
    case 1:
        if (ie->len != 9) return OGS_ERROR;
        out.enb_type = S1AP_ENB_ID_PR_macroENB_ID;
        out.enb_id = ((uint32_t)(p[4] & 15) << 16) | ((uint32_t)p[5] << 8) | p[6];
        break;
    case 3:
        if (ie->len != 10) return OGS_ERROR;
        out.enb_type = S1AP_ENB_ID_PR_homeENB_ID;
        out.enb_id = ((uint32_t)(p[4] & 15) << 24) |
            ((uint32_t)p[5] << 16) | ((uint32_t)p[6] << 8) | p[7];
        break;
    case 4:
        if (ie->len != 9) return OGS_ERROR;
        out.enb_type = p[4] & 0x80 ? S1AP_ENB_ID_PR_short_macroENB_ID :
            S1AP_ENB_ID_PR_long_macroENB_ID;
        out.enb_id = ((uint32_t)(p[4] & (p[4] & 0x80 ? 3 : 31)) << 16) |
            ((uint32_t)p[5] << 8) | p[6];
        break;
    default:
        return OGS_ERROR;
    }
    memcpy(&tac, p + ie->len - 2, 2);
    out.tai.tac = be16toh(tac);
    *target = out;
    return OGS_OK;
}

int mme_s10_encode_cause(ogs_tlv_octet_t *ie,
        const S1AP_Cause_t *cause, uint8_t buffer[2])
{
    long value;
    switch (cause->present) {
    case S1AP_Cause_PR_radioNetwork: value = cause->choice.radioNetwork; break;
    case S1AP_Cause_PR_transport: value = cause->choice.transport; break;
    case S1AP_Cause_PR_nas: value = cause->choice.nas; break;
    case S1AP_Cause_PR_protocol: value = cause->choice.protocol; break;
    case S1AP_Cause_PR_misc: value = cause->choice.misc; break;
    default: return OGS_ERROR;
    }
    if (value < 0 || value > 255) return OGS_ERROR;
    buffer[0] = cause->present - 1;
    buffer[1] = value;
    ie->presence = 1;
    ie->data = buffer;
    ie->len = 2;
    return OGS_OK;
}

int mme_s10_decode_cause(S1AP_Cause_t *cause, const ogs_tlv_octet_t *ie)
{
    const uint8_t *p = ie->data;
    S1AP_Cause_t out = {0};
    if (!ie->presence || ie->len != 2 || !p) return OGS_ERROR;
    out.present = (p[0] & 15) + 1;
    switch (out.present) {
    case S1AP_Cause_PR_radioNetwork: out.choice.radioNetwork = p[1]; break;
    case S1AP_Cause_PR_transport: out.choice.transport = p[1]; break;
    case S1AP_Cause_PR_nas: out.choice.nas = p[1]; break;
    case S1AP_Cause_PR_protocol: out.choice.protocol = p[1]; break;
    case S1AP_Cause_PR_misc: out.choice.misc = p[1]; break;
    default: return OGS_ERROR;
    }
    *cause = out;
    return OGS_OK;
}

/* TS 29.274 8.50 numbers the three MNC digits differently from the
 * Serving Network and Target Identification IEs. Two-digit MNCs retain
 * the usual filler nibble. Do not copy a three-digit PLMN blindly. */
int mme_s10_encode_selected_plmn(ogs_tlv_octet_t *ie,
        const ogs_plmn_id_t *plmn, uint8_t buffer[3])
{
    const uint8_t *p = (const uint8_t *)plmn;
    if ((p[0] & 15) > 9 || (p[0] >> 4) > 9 || (p[1] & 15) > 9 ||
            ((p[1] >> 4) > 9 && (p[1] >> 4) != 15) ||
            (p[2] & 15) > 9 || (p[2] >> 4) > 9) return OGS_ERROR;
    memcpy(buffer, p, 3);
    if ((p[1] >> 4) != 15) {
        buffer[1] = (p[1] & 15) | ((p[2] & 15) << 4);
        buffer[2] = (p[2] >> 4) | (p[1] & 0xf0);
    }
    ie->presence = 1;
    ie->len = 3;
    ie->data = buffer;
    return OGS_OK;
}

int mme_s10_decode_selected_plmn(ogs_plmn_id_t *plmn,
        const ogs_tlv_octet_t *ie)
{
    const uint8_t *p = ie->data;
    uint8_t out[3];
    if (!ie->presence || ie->len != 3 || !p ||
            (p[0] & 15) > 9 || (p[0] >> 4) > 9 || (p[1] & 15) > 9 ||
            ((p[1] >> 4) > 9 && (p[1] >> 4) != 15) ||
            (p[2] & 15) > 9 || (p[2] >> 4) > 9) return OGS_ERROR;
    memcpy(out, p, 3);
    if ((p[1] >> 4) != 15) {
        out[1] = (p[1] & 15) | (p[2] & 0xf0);
        out[2] = ((p[2] & 15) << 4) | (p[1] >> 4);
    }
    memcpy(plmn, out, sizeof(out));
    return OGS_OK;
}
