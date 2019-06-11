#ifndef __S1AP_CONV_H__
#define __S1AP_CONV_H__

#include "asn1c/s1ap-message.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void s1ap_uint8_to_OCTET_STRING(
        uint8_t uint8, OCTET_STRING_t *octet_string);
void s1ap_uint16_to_OCTET_STRING(
        uint16_t uint16, OCTET_STRING_t *octet_string);
void s1ap_uint32_to_OCTET_STRING(
        uint32_t uint32, OCTET_STRING_t *octet_string);

void s1ap_buffer_to_OCTET_STRING(
        void *buf, int size, S1AP_TBCD_STRING_t *tbcd_string);

void s1ap_uint32_to_ENB_ID(
        S1AP_ENB_ID_PR present, uint32_t enb_id, S1AP_ENB_ID_t *eNB_ID);
void s1ap_ENB_ID_to_uint32(
        S1AP_ENB_ID_t *eNB_ID, uint32_t *uint32);

int s1ap_BIT_STRING_to_ip(
        BIT_STRING_t *bit_string, ip_t *ip);
int s1ap_ip_to_BIT_STRING(
        ip_t *ip, BIT_STRING_t *bit_string);

int s1ap_copy_ie(
        const asn_TYPE_descriptor_t *td, void *src, void *dst);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __S1AP_CONV_H__ */

