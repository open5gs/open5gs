/*
 * media_transport_proto.h
 *
 * 
 */

#ifndef _OpenAPI_media_transport_proto_H_
#define _OpenAPI_media_transport_proto_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_media_transport_proto_NULL = 0, OpenAPI_media_transport_proto_RTP, OpenAPI_media_transport_proto_SRTP, OpenAPI_media_transport_proto_MOQT } OpenAPI_media_transport_proto_e;

char* OpenAPI_media_transport_proto_ToString(OpenAPI_media_transport_proto_e media_transport_proto);

OpenAPI_media_transport_proto_e OpenAPI_media_transport_proto_FromString(char* media_transport_proto);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_media_transport_proto_H_ */

