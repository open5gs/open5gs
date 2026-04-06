/*
 * transport_mode.h
 *
 * Indicates the Transport Mode when the steering functionality is MPQUIC-UDP, MPQUIC-IE, or MPQUIC-E functionality. 
 */

#ifndef _OpenAPI_transport_mode_H_
#define _OpenAPI_transport_mode_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_transport_mode_NULL = 0, OpenAPI_transport_mode_DATAGRAM_MODE_1, OpenAPI_transport_mode_DATAGRAM_MODE_2, OpenAPI_transport_mode_STREAM_MODE } OpenAPI_transport_mode_e;

char* OpenAPI_transport_mode_ToString(OpenAPI_transport_mode_e transport_mode);

OpenAPI_transport_mode_e OpenAPI_transport_mode_FromString(char* transport_mode);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_transport_mode_H_ */

