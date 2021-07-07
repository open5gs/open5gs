/*
 * ddn_failure_sub_info.h
 *
 * 
 */

#ifndef _OpenAPI_ddn_failure_sub_info_H_
#define _OpenAPI_ddn_failure_sub_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ddd_traffic_descriptor.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ddn_failure_sub_info_s OpenAPI_ddn_failure_sub_info_t;
typedef struct OpenAPI_ddn_failure_sub_info_s {
    char *notify_correlation_id;
    OpenAPI_list_t *ddd_traffic_descriptor_list;
} OpenAPI_ddn_failure_sub_info_t;

OpenAPI_ddn_failure_sub_info_t *OpenAPI_ddn_failure_sub_info_create(
    char *notify_correlation_id,
    OpenAPI_list_t *ddd_traffic_descriptor_list
);
void OpenAPI_ddn_failure_sub_info_free(OpenAPI_ddn_failure_sub_info_t *ddn_failure_sub_info);
OpenAPI_ddn_failure_sub_info_t *OpenAPI_ddn_failure_sub_info_parseFromJSON(cJSON *ddn_failure_sub_infoJSON);
cJSON *OpenAPI_ddn_failure_sub_info_convertToJSON(OpenAPI_ddn_failure_sub_info_t *ddn_failure_sub_info);
OpenAPI_ddn_failure_sub_info_t *OpenAPI_ddn_failure_sub_info_copy(OpenAPI_ddn_failure_sub_info_t *dst, OpenAPI_ddn_failure_sub_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ddn_failure_sub_info_H_ */

