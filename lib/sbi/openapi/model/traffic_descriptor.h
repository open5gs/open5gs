/*
 * traffic_descriptor.h
 *
 *
 */

#ifndef _OpenAPI_traffic_descriptor_H_
#define _OpenAPI_traffic_descriptor_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ddd_traffic_descriptor.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_traffic_descriptor_s OpenAPI_traffic_descriptor_t;
typedef struct OpenAPI_traffic_descriptor_s {
    char *dnn;
    struct OpenAPI_snssai_s *s_nssai;
    OpenAPI_list_t *ddd_traffic_descriptor_list;
} OpenAPI_traffic_descriptor_t;

OpenAPI_traffic_descriptor_t *OpenAPI_traffic_descriptor_create(
    char *dnn,
    OpenAPI_snssai_t *s_nssai,
    OpenAPI_list_t *ddd_traffic_descriptor_list
    );
void OpenAPI_traffic_descriptor_free(OpenAPI_traffic_descriptor_t *traffic_descriptor);
OpenAPI_traffic_descriptor_t *OpenAPI_traffic_descriptor_parseFromJSON(cJSON *traffic_descriptorJSON);
cJSON *OpenAPI_traffic_descriptor_convertToJSON(OpenAPI_traffic_descriptor_t *traffic_descriptor);
OpenAPI_traffic_descriptor_t *OpenAPI_traffic_descriptor_copy(OpenAPI_traffic_descriptor_t *dst, OpenAPI_traffic_descriptor_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_traffic_descriptor_H_ */

