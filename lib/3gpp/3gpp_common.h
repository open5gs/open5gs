#ifndef __3GPP_COMMON_H__
#define __3GPP_COMMON_H__

#include "core.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MAX_UE_PER_ENB      128
#define MAX_NUM_OF_ENB      128
#define MAX_NUM_OF_UE       (MAX_NUM_OF_ENB * MAX_UE_PER_ENB)

#define IPV6_LEN            16

#define MAX_SDU_LEN         2048

#define PLMN_ID_LEN         3

#define BCD_TO_BUFFER_LEN(x)    (((x)+1)/2)
#define MAX_IMSI_BCD_LEN    15
#define MAX_IMSI_LEN        BCD_TO_BUFFER_LEN(MAX_IMSI_BCD_LEN)
#define MAX_MSISDN_BCD_LEN  15
#define MAX_MSISDN_LEN      BCD_TO_BUFFER_LEN(MAX_MSISDN_BCD_LEN)
#define MAX_MEI_BCD_LEN     16
#define MAX_MEI_LEN         BCD_TO_BUFFER_LEN(MAX_MEI_BCD_LEN)

#define RAND_LEN            16
#define AUTN_LEN            16
#define MAX_RES_LEN         16

#define MAX_APN_LEN         100

#define NEXT_ID(__id, __max) \
    ((__id) = ((__id) == (__max) ? 1 : ((__id) + 1)))
#define COMPARE_ID(__id1, __id2, __max) \
    ((__id2) > (__id1) ? ((__id2) - (__id1) < ((__max)-1) ? -1 : 1) : \
     (__id1) > (__id2) ? ((__id1) - (__id2) < ((__max)-1) ? 1 : -1) : 0)

typedef struct _plmn_id_t {
ED2(c_uint8_t mcc2:4;,
    c_uint8_t mcc1:4;)
ED2(c_uint8_t mnc1:4;,
    c_uint8_t mcc3:4;)
ED2(c_uint8_t mnc3:4;,
    c_uint8_t mnc2:4;)
} __attribute__ ((packed)) plmn_id_t;

CORE_DECLARE(c_uint16_t) plmn_id_mcc(plmn_id_t *plmn_id);
CORE_DECLARE(c_uint16_t) plmn_id_mnc(plmn_id_t *plmn_id);
CORE_DECLARE(c_uint16_t) plmn_id_mnc_len(plmn_id_t *plmn_id);

CORE_DECLARE(void *) plmn_id_build(plmn_id_t *plmn_id, 
        c_uint16_t mcc, c_uint16_t mnc, c_uint16_t mnc_len);

/* 8.13 Protocol Configuration Options (PCO) 
 * 10.5.6.3 Protocol configuration options in 3GPP TS 24.008 
 * RFC 3232 [103]
 * RFC 1661 [102] */
#define PCO_PPP_FOR_USE_WITH_IP_PDP_TYPE_OR_IP_PDN_TYPE 0

#define PROTOCOL_OR_CONTAINER_ID_INTERNET_PROTOCOL_CONTROL_PROTOCOL 0x8021
#define PROTOCOL_OR_CONTAINER_ID_DNS_SERVER_IPV4_ADDRESS_REQUEST 0x000d
#define PROTOCOL_OR_CONTAINER_ID_IP_ADDRESS_ALLOCATION_VIA_NAS_SIGNALLING 0x000a
typedef struct _protocol_or_container_id_t {
    c_uint16_t id;
    c_uint8_t length;
    void *contents;
} protocol_or_container_id_t;

#define MAX_NUM_OF_PROTOCOL_OR_CONTAINER_ID    4
#define MAX_PCO_LEN 251
typedef struct _pco_t {
ED3(c_uint8_t ext:1;,
    c_uint8_t spare:4;,
    c_uint8_t configuration_protocol:3;)
    c_uint8_t num_of_id;
    protocol_or_container_id_t ids[MAX_NUM_OF_PROTOCOL_OR_CONTAINER_ID];
} pco_t;

CORE_DECLARE(c_int16_t) pco_parse(pco_t *pco, void *data, int data_len);
CORE_DECLARE(c_int16_t) pco_build(void *data, int data_len, pco_t *pco);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __3GPP_COMMON_H__ */
