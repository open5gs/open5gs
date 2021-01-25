/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
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

#if !defined(OGS_S1AP_INSIDE) && !defined(OGS_S1AP_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_S1AP_BUILD_H
#define OGS_S1AP_BUILD_H

#ifdef __cplusplus
extern "C" {
#endif

ogs_pkbuf_t *ogs_s1ap_build_error_indication(
    S1AP_MME_UE_S1AP_ID_t *mme_ue_s1ap_id,
    S1AP_ENB_UE_S1AP_ID_t *enb_ue_s1ap_id,
    S1AP_Cause_PR group, long cause);

ogs_pkbuf_t *ogs_s1ap_build_s1_reset(
    S1AP_Cause_PR group, long cause,
    S1AP_UE_associatedLogicalS1_ConnectionListRes_t *partOfS1_Interface);

void ogs_s1ap_build_part_of_s1_interface(
    S1AP_UE_associatedLogicalS1_ConnectionListRes_t **list,
    uint32_t *mme_ue_s1ap_id,
    uint32_t *enb_ue_s1ap_id);

ogs_pkbuf_t *ogs_s1ap_build_s1_reset_ack(
    S1AP_UE_associatedLogicalS1_ConnectionListRes_t *partOfS1_Interface);

#ifdef __cplusplus
}
#endif

#endif /* OGS_S1AP_BUILD_H */
