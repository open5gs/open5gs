/*
 * upf_packet_inspection_functionality.h
 *
 * Indicates a packet inspection functionality of the UPF.
 */

#ifndef _OpenAPI_upf_packet_inspection_functionality_H_
#define _OpenAPI_upf_packet_inspection_functionality_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_upf_packet_inspection_functionality_NULL = 0, OpenAPI_upf_packet_inspection_functionality_DEEP_PACKET_INSPECTION, OpenAPI_upf_packet_inspection_functionality_MAC_FILTER_BASED_PACKET_DETECTION, OpenAPI_upf_packet_inspection_functionality_IP_FILTER_BASED_PACKET_DETECTION } OpenAPI_upf_packet_inspection_functionality_e;

char* OpenAPI_upf_packet_inspection_functionality_ToString(OpenAPI_upf_packet_inspection_functionality_e upf_packet_inspection_functionality);

OpenAPI_upf_packet_inspection_functionality_e OpenAPI_upf_packet_inspection_functionality_FromString(char* upf_packet_inspection_functionality);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_upf_packet_inspection_functionality_H_ */

