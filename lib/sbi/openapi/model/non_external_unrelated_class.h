/*
 * non_external_unrelated_class.h
 *
 *
 */

#ifndef _OpenAPI_non_external_unrelated_class_H_
#define _OpenAPI_non_external_unrelated_class_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "af_non_external.h"
#include "lcs_client_non_external.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_non_external_unrelated_class_s OpenAPI_non_external_unrelated_class_t;
typedef struct OpenAPI_non_external_unrelated_class_s {
    OpenAPI_list_t *lcs_client_non_externals;
    OpenAPI_list_t *af_non_externals;
} OpenAPI_non_external_unrelated_class_t;

OpenAPI_non_external_unrelated_class_t *OpenAPI_non_external_unrelated_class_create(
    OpenAPI_list_t *lcs_client_non_externals,
    OpenAPI_list_t *af_non_externals
    );
void OpenAPI_non_external_unrelated_class_free(OpenAPI_non_external_unrelated_class_t *non_external_unrelated_class);
OpenAPI_non_external_unrelated_class_t *OpenAPI_non_external_unrelated_class_parseFromJSON(cJSON *non_external_unrelated_classJSON);
cJSON *OpenAPI_non_external_unrelated_class_convertToJSON(OpenAPI_non_external_unrelated_class_t *non_external_unrelated_class);
OpenAPI_non_external_unrelated_class_t *OpenAPI_non_external_unrelated_class_copy(OpenAPI_non_external_unrelated_class_t *dst, OpenAPI_non_external_unrelated_class_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_non_external_unrelated_class_H_ */

