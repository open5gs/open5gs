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

#include "ogs-core.h"

#define PLMN_ID_DIGIT1(x) (((x) / 100) % 10)
#define PLMN_ID_DIGIT2(x) (((x) / 10) % 10)
#define PLMN_ID_DIGIT3(x) ((x) % 10)

uint32_t ogs_plmn_id_hexdump(void *plmn_id)
{
    uint32_t hex;
    ogs_assert(plmn_id);
    memcpy(&hex, plmn_id, sizeof(ogs_plmn_id_t));
    hex = be32toh(hex) >> 8;
    return hex;
}

uint16_t ogs_plmn_id_mcc(ogs_plmn_id_t *plmn_id)
{
    return plmn_id->mcc1 * 100 + plmn_id->mcc2 * 10 + plmn_id->mcc3;
}
uint16_t ogs_plmn_id_mnc(ogs_plmn_id_t *plmn_id)
{
    return plmn_id->mnc1 == 0xf ? plmn_id->mnc2 * 10 + plmn_id->mnc3 :
        plmn_id->mnc1 * 100 + plmn_id->mnc2 * 10 + plmn_id->mnc3;
}
uint16_t ogs_plmn_id_mnc_len(ogs_plmn_id_t *plmn_id)
{
    return plmn_id->mnc1 == 0xf ? 2 : 3;
}

void *ogs_plmn_id_build(ogs_plmn_id_t *plmn_id, 
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

int ogs_fqdn_build(char *dst, char *src, int length)
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

int ogs_fqdn_parse(char *dst, char *src, int length)
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
    } while (i < length);

    return j;
}

/* 8.13 Protocol Configuration Options (PCO) 
 * 10.5.6.3 Protocol configuration options in 3GPP TS 24.008 */
int ogs_pco_parse(ogs_pco_t *pco, unsigned char *data, int data_len)
{
    ogs_pco_t *source = (ogs_pco_t *)data;
    int size = 0;
    int i = 0;

    ogs_assert(pco);
    ogs_assert(data);
    ogs_assert(data_len);

    memset(pco, 0, sizeof(ogs_pco_t));

    pco->ext = source->ext;
    pco->configuration_protocol = source->configuration_protocol;
    size++;

    while(size < data_len && i < OGS_MAX_NUM_OF_PROTOCOL_OR_CONTAINER_ID) {
        ogs_pco_id_t *id = &pco->ids[i];
        ogs_assert(size + sizeof(id->id) <= data_len);
        memcpy(&id->id, data + size, sizeof(id->id));
        id->id = be16toh(id->id);
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
int ogs_pco_build(unsigned char *data, int data_len, ogs_pco_t *pco)
{
    ogs_pco_t target;
    int size = 0;
    int i = 0;

    ogs_assert(pco);
    ogs_assert(data);
    ogs_assert(data_len);

    memcpy(&target, pco, sizeof(ogs_pco_t));

    ogs_assert(size + 1 <= data_len);
    memcpy(data + size, &target, 1);
    size += 1;

    ogs_assert(target.num_of_id <= OGS_MAX_NUM_OF_PROTOCOL_OR_CONTAINER_ID);
    for (i = 0; i < target.num_of_id; i++) {
        ogs_pco_id_t *id = &target.ids[i];

        ogs_assert(size + sizeof(id->id) <= data_len);
        id->id = htobe16(id->id);
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

int ogs_ip_to_sockaddr(ogs_ip_t *ip, uint16_t port, ogs_sockaddr_t **list)
{
    ogs_sockaddr_t *addr = NULL, *addr6 = NULL;

    ogs_assert(ip);
    ogs_assert(list);

    addr = ogs_calloc(1, sizeof(ogs_sockaddr_t));
    ogs_assert(addr);
    addr->ogs_sa_family = AF_INET;
    addr->ogs_sin_port = htobe16(port);

    addr6 = ogs_calloc(1, sizeof(ogs_sockaddr_t));
    ogs_assert(addr6);
    addr6->ogs_sa_family = AF_INET6;
    addr6->ogs_sin_port = htobe16(port);

    if (ip->ipv4 && ip->ipv6) {
        addr->next = addr6;

        addr->sin.sin_addr.s_addr = ip->both.addr;
        memcpy(addr6->sin6.sin6_addr.s6_addr, ip->both.addr6, OGS_IPV6_LEN);

        *list = addr;
    } else if (ip->ipv4) {
        addr->sin.sin_addr.s_addr = ip->addr;
        ogs_free(addr6);

        *list = addr;
    } else if (ip->ipv6) {
        memcpy(addr6->sin6.sin6_addr.s6_addr, ip->addr6, OGS_IPV6_LEN);
        ogs_free(addr);

        *list = addr6;
    } else {
        ogs_free(addr);
        ogs_free(addr6);
        return OGS_ERROR;
    }

    return OGS_OK;
}
