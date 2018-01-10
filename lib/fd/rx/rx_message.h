#ifndef __RX_MESSAGE_H__
#define __RX_MESSAGE_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "3gpp_types.h"

typedef struct _rx_message_t {
#define RX_CMD_CODE_AA                                  265
    c_uint16_t          cmd_code;

/* Experimental-Result-Codes */
#define RX_DIAMETER_INVALID_SERVICE_INFORMATION                     5061
#define RX_DIAMETER_FILTER_RESTRICTIONS                             5062
#define RX_DIAMETER_REQUESTED_SERVICE_NOT_AUTHORIZED                5063
#define RX_DIAMETER_DUPLICATED_AF_SESSION                           5064
#define RX_DIAMETER_IP_CAN_SESSION_NOT_AVAILABLE                    5065
#define RX_DIAMETER_UNAUTHORIZED_NON_EMERGENCY_SESSION              5066
#define RX_DIAMETER_UNAUTHORIZED_SPONSORED_DATA_CONNECTIVITY        5067
#define RX_DIAMETER_TEMPORARY_NETWORK_FAILURE                       5068
    c_uint32_t          result_code;

} rx_message_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RX_MESSAGE_H__ */
