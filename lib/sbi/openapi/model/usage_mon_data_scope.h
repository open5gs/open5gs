/*
 * usage_mon_data_scope.h
 *
 * Contains a SNSSAI and DNN combinations to which the UsageMonData instance belongs to.
 */

#ifndef _OpenAPI_usage_mon_data_scope_H_
#define _OpenAPI_usage_mon_data_scope_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_usage_mon_data_scope_s OpenAPI_usage_mon_data_scope_t;
typedef struct OpenAPI_usage_mon_data_scope_s {
    struct OpenAPI_snssai_s *snssai;
    OpenAPI_list_t *dnn;
} OpenAPI_usage_mon_data_scope_t;

OpenAPI_usage_mon_data_scope_t *OpenAPI_usage_mon_data_scope_create(
    OpenAPI_snssai_t *snssai,
    OpenAPI_list_t *dnn
);
void OpenAPI_usage_mon_data_scope_free(OpenAPI_usage_mon_data_scope_t *usage_mon_data_scope);
OpenAPI_usage_mon_data_scope_t *OpenAPI_usage_mon_data_scope_parseFromJSON(cJSON *usage_mon_data_scopeJSON);
cJSON *OpenAPI_usage_mon_data_scope_convertToJSON(OpenAPI_usage_mon_data_scope_t *usage_mon_data_scope);
OpenAPI_usage_mon_data_scope_t *OpenAPI_usage_mon_data_scope_copy(OpenAPI_usage_mon_data_scope_t *dst, OpenAPI_usage_mon_data_scope_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_usage_mon_data_scope_H_ */

