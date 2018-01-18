#ifndef __RX_MESSAGE_H__
#define __RX_MESSAGE_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "3gpp_types.h"

typedef struct _rx_media_sub_component_t {
    c_uint32_t          flow_number;
#define RX_FLOW_USAGE_NO_INFORMATION        0
#define RX_FLOW_USAGE_RTCP                  1
#define RX_FLOW_USAGE_AF_SIGNALLING         2
    c_uint32_t          flow_usage;
    flow_t              flow[MAX_NUM_OF_FLOW];
    int                 num_of_flow;
} rx_media_sub_component_t;

typedef struct _rx_media_component_t {
    c_uint32_t          media_component_number;
#define RX_MEDIA_TYPE_AUDIO             0
#define RX_MEDIA_TYPE_VIDEO             1
#define RX_MEDIA_TYPE_DATA              2
#define RX_MEDIA_TYPE_APPLICATION       3
#define RX_MEDIA_TYPE_CONTROL           4
#define RX_MEDIA_TYPE_TEXT              5
#define RX_MEDIA_TYPE_MESSAGE           6
#define RX_MEDIA_TYPE_OTHER             0xFFFFFFFF
    c_uint32_t          media_type;

    c_uint64_t          max_requested_bandwidth_dl; 
    c_uint64_t          max_requested_bandwidth_ul;
    c_uint64_t          min_requested_bandwidth_dl; 
    c_uint64_t          min_requested_bandwidth_ul;
    c_uint64_t          rr_bandwidth;
    c_uint64_t          rs_bandwidth;

#define MAX_NUM_OF_MEDIA_SUB_COMPONENT            8
    rx_media_sub_component_t sub[MAX_NUM_OF_MEDIA_SUB_COMPONENT];
    int                 num_of_sub;
} rx_media_component_t;

typedef struct _rx_message_t {
#define RX_CMD_CODE_AA                                              265
#define RX_CMD_CODE_SESSION_TERMINATION                             275
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

#define MAX_NUM_OF_MEDIA_COMPONENT 16
    rx_media_component_t media_component[MAX_NUM_OF_MEDIA_COMPONENT];
    int num_of_media_component;
} rx_message_t;

CORE_DECLARE(void) rx_message_free(rx_message_t *rx_message);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RX_MESSAGE_H__ */
