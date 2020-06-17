/*
 * external_unrelated_class.h
 *
 *
 */

#ifndef _OpenAPI_external_unrelated_class_H_
#define _OpenAPI_external_unrelated_class_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "af_external.h"
#include "lcs_client_external.h"
#include "lcs_client_group_external.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_external_unrelated_class_s OpenAPI_external_unrelated_class_t;
typedef struct OpenAPI_external_unrelated_class_s {
    OpenAPI_list_t *lcs_client_externals;
    OpenAPI_list_t *af_externals;
    OpenAPI_list_t *lcs_client_group_externals;
} OpenAPI_external_unrelated_class_t;

OpenAPI_external_unrelated_class_t *OpenAPI_external_unrelated_class_create(
    OpenAPI_list_t *lcs_client_externals,
    OpenAPI_list_t *af_externals,
    OpenAPI_list_t *lcs_client_group_externals
    );
void OpenAPI_external_unrelated_class_free(OpenAPI_external_unrelated_class_t *external_unrelated_class);
OpenAPI_external_unrelated_class_t *OpenAPI_external_unrelated_class_parseFromJSON(cJSON *external_unrelated_classJSON);
cJSON *OpenAPI_external_unrelated_class_convertToJSON(OpenAPI_external_unrelated_class_t *external_unrelated_class);
OpenAPI_external_unrelated_class_t *OpenAPI_external_unrelated_class_copy(OpenAPI_external_unrelated_class_t *dst, OpenAPI_external_unrelated_class_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_external_unrelated_class_H_ */

