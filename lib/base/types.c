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

#include "types.h"

#define PLMN_ID_DIGIT1(x) (((x) / 100) % 10)
#define PLMN_ID_DIGIT2(x) (((x) / 10) % 10)
#define PLMN_ID_DIGIT3(x) ((x) % 10)

uint32_t plmn_id_hexdump(plmn_id_t *plmn_id)
{
    uint32_t hex;
    ogs_assert(plmn_id);
    memcpy(&hex, plmn_id, sizeof *plmn_id);
    hex = ntohl(hex) >> 8;
    return hex;
}

uint16_t plmn_id_mcc(plmn_id_t *plmn_id)
{
    return plmn_id->mcc1 * 100 + plmn_id->mcc2 * 10 + plmn_id->mcc3;
}
uint16_t plmn_id_mnc(plmn_id_t *plmn_id)
{
    return plmn_id->mnc1 == 0xf ? plmn_id->mnc2 * 10 + plmn_id->mnc3 :
        plmn_id->mnc1 * 100 + plmn_id->mnc2 * 10 + plmn_id->mnc3;
}
uint16_t plmn_id_mnc_len(plmn_id_t *plmn_id)
{
    return plmn_id->mnc1 == 0xf ? 2 : 3;
}

void *plmn_id_build(plmn_id_t *plmn_id, 
        uint16_t mcc, uint16_t mnc, uint16_t mnc_len)
{
    plmn_id->mcc1 = PLMN_ID_DIGIT1(mcc);
    plmn_id->mcc2 = PLMN_ID_DIGIT2(mcc);
    plmn_id->mcc3 = PLMN_ID_DIGIT3(mcc);

    if (mnc_len == 2)
        plmn_id->mnc1 = 0xf;
    else
        plmn_id->mnc1 = PLMN_ID_DIGIT1(mnc);

    plmn_id->mnc2 = PLMN_ID_DIGIT2(mnc);
    plmn_id->mnc3 = PLMN_ID_DIGIT3(mnc);

    return plmn_id;
}

int fqdn_build(char *dst, char *src, int length)
{
    int i = 0, j = 0;

    for (i = 0, j = 0; i < length; i++, j++) {
        if (src[i] == '.') {
            dst[i-j] = j;
            j = -1;
        } else {
            dst[i+1] = src[i];
        }
    }
    dst[i-j] = j;

    return length+1;
}

int fqdn_parse(char *dst, char *src, int length)
{
    int i = 0, j = 0;
    uint8_t len = 0;

    do {
        len = src[i++];
        memcpy(&dst[j], &src[i], len);

        i += len;
        j += len;
        
        if (i < length)
            dst[j++] = '.';
        else
            dst[j] = 0;
    } while(i < length);

    return j;
}

/* 8.13 Protocol Configuration Options (PCO) 
 * 10.5.6.3 Protocol configuration options in 3GPP TS 24.008 */
int pco_parse(pco_t *pco, void *data, int data_len)
{
    pco_t *source = (pco_t *)data;
    int size = 0;
    int i = 0;

    ogs_assert(pco);
    ogs_assert(data);
    ogs_assert(data_len);

    memset(pco, 0, sizeof(pco_t));

    pco->ext = source->ext;
    pco->configuration_protocol = source->configuration_protocol;
    size++;

    while(size < data_len && i < MAX_NUM_OF_PROTOCOL_OR_CONTAINER_ID) {
        pco_id_t *id = &pco->ids[i];
        ogs_assert(size + sizeof(id->id) <= data_len);
        memcpy(&id->id, data + size, sizeof(id->id));
        id->id = ntohs(id->id);
        size += sizeof(id->id);

        ogs_assert(size + sizeof(id->len) <= data_len);
        memcpy(&id->len, data + size, sizeof(id->len));
        size += sizeof(id->len);

        id->data = data + size;
        size += id->len;

        i++;
    }
    pco->num_of_id = i;
    ogs_assert(size == data_len);
    
    return size;
}
int pco_build(void *data, int data_len, pco_t *pco)
{
    pco_t target;
    int size = 0;
    int i = 0;

    ogs_assert(pco);
    ogs_assert(data);
    ogs_assert(data_len);

    memcpy(&target, pco, sizeof(pco_t));

    ogs_assert(size + 1 <= data_len);
    memcpy(data + size, &target, 1);
    size += 1;

    ogs_assert(target.num_of_id <= MAX_NUM_OF_PROTOCOL_OR_CONTAINER_ID);
    for (i = 0; i < target.num_of_id; i++) {
        pco_id_t *id = &target.ids[i];

        ogs_assert(size + sizeof(id->id) <= data_len);
        id->id = htons(id->id);
        memcpy(data + size, &id->id, sizeof(id->id));
        size += sizeof(id->id);

        ogs_assert(size + sizeof(id->len) <= data_len);
        memcpy(data + size, &id->len, sizeof(id->len));
        size += sizeof(id->len);

        ogs_assert(size + id->len <= data_len);
        memcpy(data + size, id->data, id->len);
        size += id->len;
    }

    return size;
}
