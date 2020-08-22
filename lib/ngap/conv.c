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

#include "ogs-ngap.h"

void ogs_ngap_uint32_to_GNB_ID(
        uint32_t gnb_id, uint8_t bitsize, NGAP_GNB_ID_t *gNB_ID)
{
    ogs_assert(gNB_ID);

    /* gNB ID : 22bit ~ 32bit */
    ogs_asn_uint32_to_BIT_STRING(gnb_id, bitsize, &gNB_ID->choice.gNB_ID);
    gNB_ID->present = NGAP_GNB_ID_PR_gNB_ID;
}

void ogs_ngap_GNB_ID_to_uint32(NGAP_GNB_ID_t *gNB_ID, uint32_t *gnb_id)
{
    ogs_assert(gnb_id);
    ogs_assert(gNB_ID);

    /* gNB ID : 22bit ~ 32bit */
    ogs_asn_BIT_STRING_to_uint32(&gNB_ID->choice.gNB_ID, gnb_id);
}

void ogs_ngap_uint8_to_AMFRegionID(
        uint8_t region, NGAP_AMFRegionID_t *aMFRegionID)
{
    ogs_assert(aMFRegionID);

    aMFRegionID->size = 1;
    aMFRegionID->buf = CALLOC(aMFRegionID->size, sizeof(uint8_t));

    aMFRegionID->buf[0] = region;
}
void ogs_ngap_uint16_to_AMFSetID(
        uint16_t set, NGAP_AMFSetID_t *aMFSetID)
{
    ogs_assert(aMFSetID);

    aMFSetID->size = 2;
    aMFSetID->buf = CALLOC(aMFSetID->size, sizeof(uint8_t));
    aMFSetID->bits_unused = 6;

    aMFSetID->buf[0] = (set >> 2);
    aMFSetID->buf[1] = ((set & 0x03) << 6);
}
void ogs_ngap_uint8_to_AMFPointer(
        uint8_t pointer, NGAP_AMFPointer_t *aMFPointer)
{
    ogs_assert(aMFPointer);

    aMFPointer->size = 1;
    aMFPointer->buf = CALLOC(aMFPointer->size, sizeof(uint8_t));
    aMFPointer->bits_unused = 2;

    aMFPointer->buf[0] = (pointer << 2);
}

void ogs_ngap_AMFRegionID_to_uint8(
        NGAP_AMFRegionID_t *aMFRegionID, uint8_t *region)
{
    uint8_t *buf = NULL;

    ogs_assert(aMFRegionID);
    ogs_assert(region);

    buf = aMFRegionID->buf;
    ogs_assert(buf);

    *region = buf[0];
}
void ogs_ngap_AMFSetID_to_uint16(
        NGAP_AMFSetID_t *aMFSetID, uint16_t *set)
{
    uint8_t *buf = NULL;

    ogs_assert(aMFSetID);
    ogs_assert(set);

    buf = aMFSetID->buf;
    ogs_assert(buf);

    *set = (buf[0] << 2) + ((buf[1] >> 6) & 0x03);
}
void ogs_ngap_AMFPointer_to_uint8(
        NGAP_AMFPointer_t *aMFPointer, uint8_t *pointer)
{
    uint8_t *buf = NULL;

    ogs_assert(aMFPointer);
    ogs_assert(pointer);

    buf = aMFPointer->buf;
    ogs_assert(buf);

    *pointer = (buf[0] >> 2);
}

void ogs_ngap_nr_cgi_to_ASN(ogs_nr_cgi_t *nr_cgi, NGAP_NR_CGI_t *nR_CGI)
{
    char buf[5];

    ogs_assert(nr_cgi);
    ogs_assert(nR_CGI);

    ogs_asn_buffer_to_OCTET_STRING(
            &nr_cgi->plmn_id, OGS_PLMN_ID_LEN, &nR_CGI->pLMNIdentity);

    /* CellIdentity : 36bit */
    ogs_uint64_to_buffer((nr_cgi->cell_id << 4), 5, buf);
    ogs_asn_buffer_to_BIT_STRING(buf, 5, 4, &nR_CGI->nRCellIdentity);
}

void ogs_ngap_ASN_to_nr_cgi(NGAP_NR_CGI_t *nR_CGI, ogs_nr_cgi_t *nr_cgi)
{
    ogs_assert(nr_cgi);
    ogs_assert(nR_CGI);

    memcpy(&nr_cgi->plmn_id, nR_CGI->pLMNIdentity.buf, OGS_PLMN_ID_LEN);

    /* CellIdentity : 36bit */
    nr_cgi->cell_id =
        (ogs_buffer_to_uint64(nR_CGI->nRCellIdentity.buf, 5) >> 4);
}

void ogs_ngap_5gs_tai_to_ASN(ogs_5gs_tai_t *tai, NGAP_TAI_t *tAI)
{
    ogs_assert(tai);
    ogs_assert(tAI);

    ogs_asn_buffer_to_OCTET_STRING(
            &tai->plmn_id, OGS_PLMN_ID_LEN, &tAI->pLMNIdentity);
    ogs_asn_uint24_to_OCTET_STRING(tai->tac, &tAI->tAC);
}

void ogs_ngap_ASN_to_5gs_tai(NGAP_TAI_t *tAI, ogs_5gs_tai_t *tai)
{
    ogs_assert(tai);
    ogs_assert(tAI);

    memcpy(&tai->plmn_id, tAI->pLMNIdentity.buf, OGS_PLMN_ID_LEN);
    ogs_asn_OCTET_STRING_to_uint24(&tAI->tAC, &tai->tac);
}
