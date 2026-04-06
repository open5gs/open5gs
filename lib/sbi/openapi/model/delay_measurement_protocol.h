/*
 * delay_measurement_protocol.h
 *
 * Delay measurement protocol
 */

#ifndef _OpenAPI_delay_measurement_protocol_H_
#define _OpenAPI_delay_measurement_protocol_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_delay_measurement_protocol_NULL = 0, OpenAPI_delay_measurement_protocol_TWAMP, OpenAPI_delay_measurement_protocol_OWAMP, OpenAPI_delay_measurement_protocol_STAMP, OpenAPI_delay_measurement_protocol_OTHER } OpenAPI_delay_measurement_protocol_e;

char* OpenAPI_delay_measurement_protocol_ToString(OpenAPI_delay_measurement_protocol_e delay_measurement_protocol);

OpenAPI_delay_measurement_protocol_e OpenAPI_delay_measurement_protocol_FromString(char* delay_measurement_protocol);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_delay_measurement_protocol_H_ */

