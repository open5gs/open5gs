/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

#include "ogs-nas-5gs.h"

void ogs_nas_5gs_imsi_to_bcd(
    ogs_nas_5gs_mobile_identity_t *mobile_identity, char *imsi_bcd)
{
    ogs_nas_5gs_mobile_identity_suci_t *mobile_identity_suci = NULL;
    ogs_plmn_id_t plmn_id;
    char tmp[OGS_MAX_IMSI_BCD_LEN+1];
    char *p, *last;

    ogs_assert(mobile_identity);
    ogs_assert(imsi_bcd);

    p = imsi_bcd;
    last = imsi_bcd + OGS_MAX_IMSI_BCD_LEN + 1;

    mobile_identity_suci =
        (ogs_nas_5gs_mobile_identity_suci_t *)mobile_identity->buffer;
    ogs_assert(mobile_identity_suci);

    ogs_nas_to_plmn_id(&plmn_id, &mobile_identity_suci->nas_plmn_id);
    if (ogs_plmn_id_mnc_len(&plmn_id) == 2)
        p = ogs_slprintf(p, last, "%03d%02d",
                ogs_plmn_id_mcc(&plmn_id), ogs_plmn_id_mnc(&plmn_id));
    else 
        p = ogs_slprintf(p, last, "%03d%03d",
                ogs_plmn_id_mcc(&plmn_id), ogs_plmn_id_mnc(&plmn_id));

    ogs_assert(mobile_identity->length > 8);
    ogs_buffer_to_bcd(mobile_identity_suci->scheme_output,
            mobile_identity->length - 8, tmp);
    p = ogs_slprintf(p, last, "%s", tmp);
}

char *ogs_nas_5gs_suci_from_mobile_identity(
        ogs_nas_5gs_mobile_identity_t *mobile_identity)
{
    ogs_nas_5gs_mobile_identity_suci_t *mobile_identity_suci = NULL;
    ogs_plmn_id_t plmn_id;
    char tmp[OGS_MAX_IMSI_BCD_LEN+1];
    char routing_indicator[5];
    char *suci = NULL;

    ogs_assert(mobile_identity);

    mobile_identity_suci =
        (ogs_nas_5gs_mobile_identity_suci_t *)mobile_identity->buffer;
    ogs_assert(mobile_identity_suci);

    ogs_assert(mobile_identity_suci->h.supi_format ==
            OGS_NAS_5GS_SUPI_FORMAT_IMSI);
    ogs_assert(mobile_identity_suci->protection_scheme_id ==
            OGS_NAS_5GS_NULL_SCHEME);

    suci = ogs_msprintf("suci-%d-", mobile_identity_suci->h.supi_format);
    ogs_assert(suci);

    ogs_nas_to_plmn_id(&plmn_id, &mobile_identity_suci->nas_plmn_id);
    if (ogs_plmn_id_mnc_len(&plmn_id) == 2) {
        suci = ogs_mstrcatf(suci, "%03d-%02d-",
                ogs_plmn_id_mcc(&plmn_id), ogs_plmn_id_mnc(&plmn_id));
        ogs_assert(suci);
    } else {
        suci = ogs_mstrcatf(suci, "%03d-%03d-",
                ogs_plmn_id_mcc(&plmn_id), ogs_plmn_id_mnc(&plmn_id));
        ogs_assert(suci);
    }

    memset(routing_indicator, 0, sizeof(routing_indicator));
    if (mobile_identity_suci->routing_indicator1 != 0xf) {
        routing_indicator[0] =
            mobile_identity_suci->routing_indicator1 + '0';
        if (mobile_identity_suci->routing_indicator2 != 0xf) {
            routing_indicator[1] =
                mobile_identity_suci->routing_indicator2 + '0';
            if (mobile_identity_suci->routing_indicator3 != 0xf) {
                routing_indicator[2] =
                    mobile_identity_suci->routing_indicator3 + '0';
                if (mobile_identity_suci->routing_indicator4 != 0xf)
                    routing_indicator[3] =
                        mobile_identity_suci->routing_indicator4 + '0';
            }
        }
    }

    ogs_assert(mobile_identity->length > 8);
    ogs_buffer_to_bcd(mobile_identity_suci->scheme_output,
            mobile_identity->length - 8, tmp);

    suci = ogs_mstrcatf(suci, "%s-%d-%d-%s",
            routing_indicator,
            mobile_identity_suci->protection_scheme_id,
            mobile_identity_suci->home_network_pki_value,
            tmp);

    return suci;
}

void ogs_nas_5gs_mobile_identity_guti_to_nas_guti(
        ogs_nas_5gs_mobile_identity_guti_t *mobile_identity_guti,
        ogs_nas_5gs_guti_t *nas_guti)
{
    ogs_assert(mobile_identity_guti);
    ogs_assert(nas_guti);

    memset(nas_guti, 0, sizeof(*nas_guti));

    memcpy(&nas_guti->nas_plmn_id,
            &mobile_identity_guti->nas_plmn_id, OGS_PLMN_ID_LEN);
    memcpy(&nas_guti->amf_id,
            &mobile_identity_guti->amf_id, sizeof(ogs_amf_id_t));
    nas_guti->m_tmsi = be32toh(mobile_identity_guti->m_tmsi);
}

void ogs_nas_5gs_nas_guti_to_mobilty_identity_guti(
        ogs_nas_5gs_guti_t *nas_guti,
        ogs_nas_5gs_mobile_identity_guti_t *mobile_identity_guti)
{
    ogs_assert(nas_guti);
    ogs_assert(mobile_identity_guti);

    memset(mobile_identity_guti, 0, sizeof(*mobile_identity_guti));

    /*
     * TS24.501
     * 9.11.3.4 5GS mobile identity
     * Figure 9.11.3.4.1 5GS mobile identity IE for type of identity "5G-GUTI"
     *
     * Octet 1 : 5GS mobile identity IEI
     * Octet 2-3 : Length of 5GS mobile identity contents
     * Octet 4 : 1 1 1 1 0 0 1 0
     *
     * <Octet 4>
     *   h.supi_format = 0xf (1 1 1 1)
     *   h.odd_even = 0 (Spare 0)
     *   h.type = x x x (Type of identity : 5G-GUTI)
     */
    mobile_identity_guti->h.supi_format = 0xf;
    mobile_identity_guti->h.type = OGS_NAS_5GS_MOBILE_IDENTITY_GUTI;

    memcpy(&mobile_identity_guti->nas_plmn_id,
            &nas_guti->nas_plmn_id, OGS_PLMN_ID_LEN);
    memcpy(&mobile_identity_guti->amf_id,
            &nas_guti->amf_id, sizeof(ogs_amf_id_t));
    mobile_identity_guti->m_tmsi = htobe32(nas_guti->m_tmsi);
}
