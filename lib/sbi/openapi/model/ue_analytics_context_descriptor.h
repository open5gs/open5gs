/*
 * ue_analytics_context_descriptor.h
 *
 * Contains information about available UE related analytics contexts.
 */

#ifndef _OpenAPI_ue_analytics_context_descriptor_H_
#define _OpenAPI_ue_analytics_context_descriptor_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "nwdaf_event.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_analytics_context_descriptor_s OpenAPI_ue_analytics_context_descriptor_t;
typedef struct OpenAPI_ue_analytics_context_descriptor_s {
    char *supi;
    OpenAPI_list_t *ana_types;
} OpenAPI_ue_analytics_context_descriptor_t;

OpenAPI_ue_analytics_context_descriptor_t *OpenAPI_ue_analytics_context_descriptor_create(
    char *supi,
    OpenAPI_list_t *ana_types
);
void OpenAPI_ue_analytics_context_descriptor_free(OpenAPI_ue_analytics_context_descriptor_t *ue_analytics_context_descriptor);
OpenAPI_ue_analytics_context_descriptor_t *OpenAPI_ue_analytics_context_descriptor_parseFromJSON(cJSON *ue_analytics_context_descriptorJSON);
cJSON *OpenAPI_ue_analytics_context_descriptor_convertToJSON(OpenAPI_ue_analytics_context_descriptor_t *ue_analytics_context_descriptor);
OpenAPI_ue_analytics_context_descriptor_t *OpenAPI_ue_analytics_context_descriptor_copy(OpenAPI_ue_analytics_context_descriptor_t *dst, OpenAPI_ue_analytics_context_descriptor_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_analytics_context_descriptor_H_ */

