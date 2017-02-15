#ifndef _S1AP_CONV_H__
#define _S1AP_CONV_H__

#include "s1ap_message.h"
#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(void) s1ap_conv_uint8_to_octet_string(
        c_uint8_t uint8, OCTET_STRING_t *octet_string);
CORE_DECLARE(void) s1ap_conv_uint16_to_octet_string(
        c_uint16_t uint16, OCTET_STRING_t *octet_string);
CORE_DECLARE(void) s1ap_conv_uint32_to_octet_string(
        c_uint32_t uint32, OCTET_STRING_t *octet_string);

CORE_DECLARE(void) s1ap_conv_plmn_id_to_tbcd_string(
        plmn_id_t *plmn_id, S1ap_TBCD_STRING_t *tbcd_string);

CORE_DECLARE(void) s1ap_conv_macro_enb_id_to_bit_string(
    c_uint32_t enb_id, BIT_STRING_t *bit_string);
CORE_DECLARE(void) s1ap_conv_home_enb_id_to_bit_string(
    c_uint32_t enb_id, BIT_STRING_t *bit_string);
CORE_DECLARE(status_t) s1ap_conv_uint32_from_enb_id(
        c_uint32_t *uint32, S1ap_ENB_ID_t *eNB_ID);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

