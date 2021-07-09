/*
 * odb_packet_services.h
 *
 * 
 */

#ifndef _OpenAPI_odb_packet_services_H_
#define _OpenAPI_odb_packet_services_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_odb_packet_services_NULL = 0, OpenAPI_odb_packet_services_ALL_PACKET_SERVICES, OpenAPI_odb_packet_services_ROAMER_ACCESS_HPLMN_AP, OpenAPI_odb_packet_services_ROAMER_ACCESS_VPLMN_AP } OpenAPI_odb_packet_services_e;

char* OpenAPI_odb_packet_services_ToString(OpenAPI_odb_packet_services_e odb_packet_services);

OpenAPI_odb_packet_services_e OpenAPI_odb_packet_services_FromString(char* odb_packet_services);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_odb_packet_services_H_ */

