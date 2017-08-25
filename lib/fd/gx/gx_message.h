#ifndef __GX_MESSAGE_H__
#define __GX_MESSAGE_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "types.h"

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

    c_uint32_t          result_code;

    gx_cca_message_t    cca_message;
} gx_message_t;

CORE_DECLARE(void) gx_cca_message_free(gx_cca_message_t *cca_message);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GX_MESSAGE_H__ */
