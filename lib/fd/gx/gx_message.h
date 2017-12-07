#ifndef __GX_MESSAGE_H__
#define __GX_MESSAGE_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "3gpp_types.h"

typedef struct _gx_cca_message_t {
#define GX_CC_REQUEST_TYPE_INITIAL_REQUEST      1
#define GX_CC_REQUEST_TYPE_UPDATE_REQUEST       2
#define GX_CC_REQUEST_TYPE_TERMINATION_REQUEST  3
#define GX_CC_REQUEST_TYPE_EVENT_REQUEST        4
    c_uint32_t  cc_request_type;

    pdn_t       pdn;
    pcc_rule_t  pcc_rule[MAX_NUM_OF_PCC_RULE];
    int         num_of_pcc_rule;
} gx_cca_message_t;

typedef struct _gx_message_t {
#define GX_CMD_CODE_CREDIT_CONTROL              272
    c_uint16_t          cmd_code;

/* Result-Code AVP */
#define GX_DIAMETER_ERROR_USER_UNKNOWN                  5030

/* Experimental-Result-Codes */
#define GX_DIAMETER_ERROR_LATE_OVERLAPPING_REQUEST      5453
#define GX_DIAMETER_ERROR_TIMED_OUT_REQUEST             5454
#define GX_DIAMETER_ERROR_INITIAL_PARAMETERS            5140
#define GX_DIAMETER_ERROR_TRIGGER_EVENT                 5141
#define GX_DIAMETER_PCC_RULE_EVENT                      5142
#define GX_DIAMETER_ERROR_BEARER_NOT_AUTHORIZED         5143
#define GX_DIAMETER_ERROR_TRAFFIC_MAPPING_INFO_REJECTED 5144
#define GX_DIAMETER_ERROR_CONFLICTING_REQUEST           5147
#define GX_DIAMETER_ADC_RULE_EVENT                      5148
#define GX_DIAMETER_ERROR_NBIFOM_NOT_AUTHORIZED         5149
    c_uint32_t          result_code;

    gx_cca_message_t    cca_message;
} gx_message_t;

CORE_DECLARE(void) gx_cca_message_free(gx_cca_message_t *cca_message);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GX_MESSAGE_H__ */
