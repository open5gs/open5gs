/*
 * requested_usage_data.h
 *
 * Contains usage data requested by the PCF requesting usage reports for the corresponding usage monitoring data instances.
 */

#ifndef _OpenAPI_requested_usage_data_H_
#define _OpenAPI_requested_usage_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_requested_usage_data_s OpenAPI_requested_usage_data_t;
typedef struct OpenAPI_requested_usage_data_s {
    OpenAPI_list_t *ref_um_ids;
    bool is_all_um_ids;
    int all_um_ids;
} OpenAPI_requested_usage_data_t;

OpenAPI_requested_usage_data_t *OpenAPI_requested_usage_data_create(
    OpenAPI_list_t *ref_um_ids,
    bool is_all_um_ids,
    int all_um_ids
);
void OpenAPI_requested_usage_data_free(OpenAPI_requested_usage_data_t *requested_usage_data);
OpenAPI_requested_usage_data_t *OpenAPI_requested_usage_data_parseFromJSON(cJSON *requested_usage_dataJSON);
cJSON *OpenAPI_requested_usage_data_convertToJSON(OpenAPI_requested_usage_data_t *requested_usage_data);
OpenAPI_requested_usage_data_t *OpenAPI_requested_usage_data_copy(OpenAPI_requested_usage_data_t *dst, OpenAPI_requested_usage_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_requested_usage_data_H_ */

