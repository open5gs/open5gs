#ifndef __SBC_MESSAGE_H__
#define __SBC_MESSAGE_H__

#include "base/types.h"

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
    uint16_t message_id;
    uint16_t serial_number;
    uint32_t no_of_tai;
    tai_t tai[16];              /* TODO: max 65535 */
    uint32_t repetition_period;
    uint32_t number_of_broadcast;
    uint8_t data_coding_scheme;
    uint32_t message_length;
    uint8_t message_contents[1024]; /* TODO: max 9600 */
} sbc_pws_data_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SBC_MESSAGE_H__ */
