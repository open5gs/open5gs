#ifndef __S1AP_CONV_H__
#define __S1AP_CONV_H__

#include "s1ap/s1ap_message.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(void) s1ap_uint8_to_OCTET_STRING(
        c_uint8_t uint8, OCTET_STRING_t *octet_string);
CORE_DECLARE(void) s1ap_uint16_to_OCTET_STRING(
        c_uint16_t uint16, OCTET_STRING_t *octet_string);
CORE_DECLARE(void) s1ap_uint32_to_OCTET_STRING(
        c_uint32_t uint32, OCTET_STRING_t *octet_string);

CORE_DECLARE(void) s1ap_buffer_to_OCTET_STRING(
        void *buf, int size, S1AP_TBCD_STRING_t *tbcd_string);

CORE_DECLARE(void) s1ap_uint32_to_ENB_ID(
        S1AP_ENB_ID_PR present, c_uint32_t enb_id, S1AP_ENB_ID_t *eNB_ID);
CORE_DECLARE(void) s1ap_ENB_ID_to_uint32(
        S1AP_ENB_ID_t *eNB_ID, c_uint32_t *uint32);

CORE_DECLARE(status_t) s1ap_BIT_STRING_to_ip(
        BIT_STRING_t *bit_string, ip_t *ip);
CORE_DECLARE(status_t) s1ap_ip_to_BIT_STRING(
        ip_t *ip, BIT_STRING_t *bit_string);

CORE_DECLARE(status_t) s1ap_copy_ie(
        const asn_TYPE_descriptor_t *td, void *src, void *dst);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __S1AP_CONV_H__ */

