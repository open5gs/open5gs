#ifndef __3GPP_COMMON_H__
#define __3GPP_COMMON_H__

#include "core_list.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MAX_NUM_OF_PDN      4

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
#define MAX_IMEI_BCD_LEN    15
#define MAX_IMEI_LEN        BCD_TO_BUFFER_LEN(MAX_IMEI_BCD_LEN)

#define RAND_LEN            16
#define AUTN_LEN            16
#define MAX_RES_LEN         16

#define MAX_APN_LEN         100

#define NEXT_ID(__id, __max) \
    ((__id) = ((__id) == (__max) ? 1 : ((__id) + 1)))
#define COMPARE_ID(__id1, __id2, __max) \
    ((__id2) > (__id1) ? ((__id2) - (__id1) < ((__max)-1) ? -1 : 1) : \
     (__id1) > (__id2) ? ((__id1) - (__id2) < ((__max)-1) ? 1 : -1) : 0)

/**********************************
 * PLMN_ID Structure             */
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

typedef struct _tai_t {
    plmn_id_t plmn_id;
    c_uint16_t tac;
} __attribute__ ((packed)) tai_t;

typedef struct _e_cgi_t {
    plmn_id_t plmn_id;
    c_uint32_t cell_id; /* 28 bit */
} __attribute__ ((packed)) e_cgi_t;

/**********************************
 * PDN Structure                 */
typedef c_uint32_t pdn_id_t;
typedef struct _pdn_t {
    lnode_t         node; /**< A node of list_t */

    pdn_id_t        id;
    c_int8_t        apn[MAX_APN_LEN];
#define PDN_TYPE_IPV4                                   0
#define PDN_TYPE_IPV6                                   1
#define PDN_TYPE_IPV4_AND_IPV6                          2
#define PDN_TYPE_IPV4_OR_IPV6                           3
    c_int8_t        type;

    c_uint32_t      max_bandwidth_ul; /* Kbps */
    c_uint32_t      max_bandwidth_dl; /* Kbps */

#define PDN_QCI_1                                       1
#define PDN_QCI_2                                       2
#define PDN_QCI_3                                       3
#define PDN_QCI_4                                       4
#define PDN_QCI_5                                       5
#define PDN_QCI_6                                       6
#define PDN_QCI_7                                       7
#define PDN_QCI_8                                       8
#define PDN_QCI_9                                       9
#define PDN_QCI_65                                      65
#define PDN_QCI_66                                      66
#define PDN_QCI_69                                      69
#define PDN_QCI_70                                      70
    c_uint8_t       qci;

    /* Values 1 to 8 should only be assigned for services that are authorized 
     * to receive prioritized treatment within an operator domain. 
     * Values 9 to 15 may be assigned to resources that are authorized 
     * by the home network and thus applicable when a UE is roaming. */
    c_uint8_t       priority_level;

#define PDN_PRE_EMPTION_CAPABILITY_ENABLED              0
#define PDN_PRE_EMPTION_CAPABILITY_DISABLED             1
    c_uint8_t       pre_emption_capability;
#define PDN_PRE_EMPTION_VULNERABILITY_ENABLED           0
#define PDN_PRE_EMPTION_VULNERABILITY_DISABLED          1
    c_uint8_t       pre_emption_vulnerability;
} pdn_t;

/**************************************************
 * Protocol Configuration Options Structure
 * 8.13 Protocol Configuration Options (PCO) 
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
