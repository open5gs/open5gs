/*
 * resources_allocation_info.h
 *
 * describes the status of the PCC rule(s) related to certain media components.
 */

#ifndef _OpenAPI_resources_allocation_info_H_
#define _OpenAPI_resources_allocation_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "flows.h"
#include "media_component_resources_status.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_resources_allocation_info_s OpenAPI_resources_allocation_info_t;
typedef struct OpenAPI_resources_allocation_info_s {
    OpenAPI_media_component_resources_status_e mc_resourc_status;
    OpenAPI_list_t *flows;
    char *alt_ser_req;
} OpenAPI_resources_allocation_info_t;

OpenAPI_resources_allocation_info_t *OpenAPI_resources_allocation_info_create(
    OpenAPI_media_component_resources_status_e mc_resourc_status,
    OpenAPI_list_t *flows,
    char *alt_ser_req
);
void OpenAPI_resources_allocation_info_free(OpenAPI_resources_allocation_info_t *resources_allocation_info);
OpenAPI_resources_allocation_info_t *OpenAPI_resources_allocation_info_parseFromJSON(cJSON *resources_allocation_infoJSON);
cJSON *OpenAPI_resources_allocation_info_convertToJSON(OpenAPI_resources_allocation_info_t *resources_allocation_info);
OpenAPI_resources_allocation_info_t *OpenAPI_resources_allocation_info_copy(OpenAPI_resources_allocation_info_t *dst, OpenAPI_resources_allocation_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_resources_allocation_info_H_ */

