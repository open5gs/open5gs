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

char *ogs_serving_network_name_from_plmn_id(ogs_plmn_id_t *plmn_id)
{
    ogs_assert(plmn_id);
    return ogs_msprintf("5G:mnc%03d.mcc%03d.3gppnetwork.org",
            ogs_plmn_id_mnc(plmn_id), ogs_plmn_id_mcc(plmn_id));
}

char *ogs_plmn_id_mcc_string(ogs_plmn_id_t *plmn_id)
{
    ogs_assert(plmn_id);
    return ogs_msprintf("%03d", ogs_plmn_id_mcc(plmn_id));
}

char *ogs_plmn_id_mnc_string(ogs_plmn_id_t *plmn_id)
{
    ogs_assert(plmn_id);
    if (ogs_plmn_id_mnc_len(plmn_id) == 2)
        return ogs_msprintf("%02d", ogs_plmn_id_mnc(plmn_id));
    else
        return ogs_msprintf("%03d", ogs_plmn_id_mnc(plmn_id));
}

char *ogs_plmn_id_to_string(ogs_plmn_id_t *plmn_id, char *buf)
{
    ogs_assert(plmn_id);
    ogs_assert(buf);

    if (ogs_plmn_id_mnc_len(plmn_id) == 2)
        ogs_snprintf(buf, OGS_PLMNIDSTRLEN, "%03d%02d",
                ogs_plmn_id_mcc(plmn_id), ogs_plmn_id_mnc(plmn_id));
    else
        ogs_snprintf(buf, OGS_PLMNIDSTRLEN, "%03d%03d",
                ogs_plmn_id_mcc(plmn_id), ogs_plmn_id_mnc(plmn_id));

    return buf;
}

uint32_t ogs_amf_id_hexdump(ogs_amf_id_t *amf_id)
{
    uint32_t hex;

    ogs_assert(amf_id);

    memcpy(&hex, amf_id, sizeof(ogs_amf_id_t));
    hex = be32toh(hex) >> 8;

    return hex;
}

ogs_amf_id_t *ogs_amf_id_from_string(ogs_amf_id_t *amf_id, const char *hex)
{
    char hexbuf[sizeof(ogs_amf_id_t)];

    ogs_assert(amf_id);
    ogs_assert(hex);

    OGS_HEX(hex, strlen(hex), hexbuf);

    amf_id->region = hexbuf[0];
    amf_id->set1 = hexbuf[1];
    amf_id->set2 = (hexbuf[2] & 0xc0) >> 6;
    amf_id->pointer = hexbuf[2] & 0x3f;

    return amf_id;
}

#define OGS_AMFIDSTRLEN    (sizeof(ogs_amf_id_t)*2+1)
char *ogs_amf_id_to_string(ogs_amf_id_t *amf_id)
{
    char *str = NULL;
    ogs_assert(amf_id);

    str = ogs_calloc(1, OGS_AMFIDSTRLEN);
    ogs_assert(str);

    ogs_hex_to_ascii(amf_id, sizeof(ogs_amf_id_t), str, OGS_AMFIDSTRLEN);

    return str;
}

uint8_t ogs_amf_region_id(ogs_amf_id_t *amf_id)
{
    ogs_assert(amf_id);
    return amf_id->region;
}
uint16_t ogs_amf_set_id(ogs_amf_id_t *amf_id)
{
    ogs_assert(amf_id);
    return (amf_id->set1 << 2) + amf_id->set2;
}
uint8_t ogs_amf_pointer(ogs_amf_id_t *amf_id)
{
    ogs_assert(amf_id);
    return amf_id->pointer;
}

ogs_amf_id_t *ogs_amf_id_build(ogs_amf_id_t *amf_id,
        uint8_t region, uint16_t set, uint8_t pointer)
{
    amf_id->region = region;
    amf_id->set1 = set >> 2;
    amf_id->set2 = set & 0x3;
    amf_id->pointer = pointer;

    return amf_id;
}

char *ogs_supi_from_suci(char *suci)
{
#define MAX_SUCI_TOKEN 16
    char *array[MAX_SUCI_TOKEN];
    char *saveptr = NULL;
    char *p, *tmp;
    int i;
    char *supi = NULL;

    ogs_assert(suci);
    tmp = ogs_strdup(suci);

    p = strtok_r(tmp, "-", &saveptr);

    memset(array, 0, sizeof(array));
    for (i = 0; i < MAX_SUCI_TOKEN && p; i++) {
        array[i] = p;
        p = strtok_r(NULL, "-", &saveptr);
    }

    SWITCH(array[0])
    CASE("suci")
        SWITCH(array[1])
        CASE("0")   /* SUPI format : IMSI */
            if (array[2] && array[3] && array[7])
                supi = ogs_msprintf("imsi-%s%s%s",
                        array[2], array[3], array[7]);

            break;
        DEFAULT
            ogs_error("Not implemented [%s]", array[1]);
            break;
        END
        break;
    DEFAULT
        ogs_error("Not implemented [%s]", array[0]);
        break;
    END

    ogs_free(tmp);
    return supi;
}

char *ogs_id_get_type(char *str)
{
    char *saveptr = NULL;
    char *p, *tmp;
    char *type = NULL;

    ogs_assert(str);
    tmp = ogs_strdup(str);

    p = strtok_r(tmp, "-", &saveptr);
    ogs_assert(p);
    type = ogs_strdup(p);

    ogs_free(tmp);
    return type;
}

char *ogs_id_get_value(char *str)
{
    char *saveptr = NULL;
    char *p, *tmp;
    char *ueid = NULL;

    ogs_assert(str);
    tmp = ogs_strdup(str);

    p = strtok_r(tmp, "-", &saveptr);
    ogs_assert(p);
    p = strtok_r(NULL, "-", &saveptr);
    ogs_assert(p);
    ueid = ogs_strdup(p);

    ogs_free(tmp);
    return ueid;
}

char *ogs_s_nssai_sd_to_string(ogs_uint24_t sd)
{
    if (sd.v == OGS_S_NSSAI_NO_SD_VALUE)
        return NULL;

    return ogs_uint24_to_string(sd);
}

ogs_uint24_t ogs_s_nssai_sd_from_string(const char *hex)
{
    ogs_uint24_t sd;

    sd.v = OGS_S_NSSAI_NO_SD_VALUE;
    if (hex == NULL)
        return sd;

    return ogs_uint24_from_string((char *)hex);
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

        addr->sin.sin_addr.s_addr = ip->addr;
        memcpy(addr6->sin6.sin6_addr.s6_addr, ip->addr6, OGS_IPV6_LEN);

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

void ogs_sockaddr_to_ip(
        ogs_sockaddr_t *addr, ogs_sockaddr_t *addr6, ogs_ip_t *ip)
{
    ogs_assert(ip);
    ogs_assert(addr || addr6);

    memset(ip, 0, sizeof(ogs_ip_t));

    if (addr && addr6) {
        ip->ipv4 = 1;
        ip->ipv6 = 1;
        ip->len = OGS_IPV4V6_LEN;
        ip->addr = addr->sin.sin_addr.s_addr;
        memcpy(ip->addr6, addr6->sin6.sin6_addr.s6_addr, OGS_IPV6_LEN);
    } else if (addr) {
        ip->ipv4 = 1;
        ip->len = OGS_IPV4_LEN;
        ip->addr = addr->sin.sin_addr.s_addr;
    } else if (addr6) {
        ip->ipv6 = 1;
        ip->len = OGS_IPV6_LEN;
        memcpy(ip->addr6, addr6->sin6.sin6_addr.s6_addr, OGS_IPV6_LEN);
    } else
        ogs_assert_if_reached();
}

char *ogs_ipv4_to_string(uint32_t addr)
{
    char *buf = NULL;

    buf = ogs_calloc(1, OGS_ADDRSTRLEN);
    ogs_assert(buf);

    return (char*)OGS_INET_NTOP(&addr, buf);
}

char *ogs_ipv6_to_string(uint8_t *addr6)
{
    char *buf = NULL;
    ogs_assert(addr6);

    buf = ogs_calloc(1, OGS_ADDRSTRLEN);
    ogs_assert(buf);

    return (char *)OGS_INET6_NTOP(addr6, buf);
}
