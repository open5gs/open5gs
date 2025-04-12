//
// Created by f2shafie on 20/11/24.
//

#ifndef OPEN5GS_UPF_PREPARE_DATA_H
#define OPEN5GS_UPF_PREPARE_DATA_H
#include <netinet/ip.h>
#include <arpa/inet.h>
#include "btree.h"
#include "ogs-gtp.h"
#include "types.h"
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>

void make_str_key(flow_key * key, char * str_key);
void convert_ipv4_uint32_to_string(uint32_t src_addr, char *key_src);
void convert_ipv6_to_string(uint8_t src_addr[16], char *key_src);

void prepare_data_ee(bool Uplink,uint64_t seid,ogs_pkbuf_t *pkbuf);
//void prepare_data_ee_opt(bool Uplink, uint64_t seid, ogs_pkbuf_t *pkbuf);
#endif //OPEN5GS_UPF_PREPARE_DATA_H
