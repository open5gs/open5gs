#ifndef __SBC_MESSAGE_H__
#define __SBC_MESSAGE_H__

#include "3gpp_types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* SBc-AP messages:
 * After the CBC integration, the encoding/decoding of 
 * SBc-AP messages will be fully supported.
 */ 

/* SBc-AP interface data definitions */

/* SBc-AP WriteReplaceWarning data */
typedef struct _sbc_pws_data_t {
    c_uint16_t message_id;
    c_uint16_t serial_number;
    c_uint32_t no_of_tai;
    tai_t tai[16];              /* TODO: max 65535 */
    c_uint32_t repetition_period;
    c_uint32_t number_of_broadcast;
    c_uint8_t data_coding_scheme;
    c_uint32_t message_length;
    c_uint8_t message_contents[1024]; /* TODO: max 9600 */
} sbc_pws_data_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SBC_MESSAGE_H__ */
