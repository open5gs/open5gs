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

#if !defined(OGS_NGAP_INSIDE) && !defined(OGS_NGAP_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_NGAP_BUILD_H
#define OGS_NGAP_BUILD_H

#ifdef __cplusplus
extern "C" {
#endif

ogs_pkbuf_t *ogs_ngap_build_error_indication(
    uint32_t *ran_ue_ngap_id,
    uint64_t *amf_ue_ngap_id,
    NGAP_Cause_PR group, long cause);

ogs_pkbuf_t *ogs_ngap_build_ng_reset(
    NGAP_Cause_PR group, long cause,
    NGAP_UE_associatedLogicalNG_connectionList_t *partOfNG_Interface);

void ogs_ngap_build_part_of_ng_interface(
    NGAP_UE_associatedLogicalNG_connectionList_t *partOfNG_Interface,
    uint32_t *ran_ue_ngap_id, uint64_t *amf_ue_ngap_id);

ogs_pkbuf_t *ogs_ngap_build_ng_reset_ack(
    NGAP_UE_associatedLogicalNG_connectionList_t *partOfNG_Interface);

#ifdef __cplusplus
}
#endif

#endif /* OGS_NGAP_BUILD_H */
