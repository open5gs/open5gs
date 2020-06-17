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
#include "null_value.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_odb_packet_services_s OpenAPI_odb_packet_services_t;
typedef struct OpenAPI_odb_packet_services_s {
} OpenAPI_odb_packet_services_t;

OpenAPI_odb_packet_services_t *OpenAPI_odb_packet_services_create(
    );
void OpenAPI_odb_packet_services_free(OpenAPI_odb_packet_services_t *odb_packet_services);
OpenAPI_odb_packet_services_t *OpenAPI_odb_packet_services_parseFromJSON(cJSON *odb_packet_servicesJSON);
cJSON *OpenAPI_odb_packet_services_convertToJSON(OpenAPI_odb_packet_services_t *odb_packet_services);
OpenAPI_odb_packet_services_t *OpenAPI_odb_packet_services_copy(OpenAPI_odb_packet_services_t *dst, OpenAPI_odb_packet_services_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_odb_packet_services_H_ */

