/*
 * patch_result.h
 *
 * 
 */

#ifndef _OpenAPI_patch_result_H_
#define _OpenAPI_patch_result_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "report_item.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_patch_result_s OpenAPI_patch_result_t;
typedef struct OpenAPI_patch_result_s {
    OpenAPI_list_t *report;
} OpenAPI_patch_result_t;

OpenAPI_patch_result_t *OpenAPI_patch_result_create(
    OpenAPI_list_t *report
);
void OpenAPI_patch_result_free(OpenAPI_patch_result_t *patch_result);
OpenAPI_patch_result_t *OpenAPI_patch_result_parseFromJSON(cJSON *patch_resultJSON);
cJSON *OpenAPI_patch_result_convertToJSON(OpenAPI_patch_result_t *patch_result);
OpenAPI_patch_result_t *OpenAPI_patch_result_copy(OpenAPI_patch_result_t *dst, OpenAPI_patch_result_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_patch_result_H_ */

