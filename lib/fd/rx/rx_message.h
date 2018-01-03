#ifndef __RX_MESSAGE_H__
#define __RX_MESSAGE_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "3gpp_types.h"

typedef struct _rx_message_t {
#define RX_CMD_CODE_AA                                  265
    c_uint16_t          cmd_code;

/* Result-Code AVP */
#define RX_DIAMETER_ERROR_USER_UNKNOWN                  5030

/* Experimental-Result-Codes */
#define RX_DIAMETER_ERROR_LATE_OVERLAPPING_REQUEST      5453
#define RX_DIAMETER_ERROR_TIMED_OUT_REQUEST             5454
#define RX_DIAMETER_ERROR_INITIAL_PARAMETERS            5140
#define RX_DIAMETER_ERROR_TRIGGER_EVENT                 5141
#define RX_DIAMETER_PCC_RULE_EVENT                      5142
#define RX_DIAMETER_ERROR_BEARER_NOT_AUTHORIZED         5143
#define RX_DIAMETER_ERROR_TRAFFIC_MAPPING_INFO_REJECTED 5144
#define RX_DIAMETER_ERROR_CONFLICTING_REQUEST           5147
#define RX_DIAMETER_ADC_RULE_EVENT                      5148
#define RX_DIAMETER_ERROR_NBIFOM_NOT_AUTHORIZED         5149
    c_uint32_t          result_code;

} rx_message_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RX_MESSAGE_H__ */
