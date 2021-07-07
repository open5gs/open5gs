/*
 * plmn_operator_class.h
 *
 * 
 */

#ifndef _OpenAPI_plmn_operator_class_H_
#define _OpenAPI_plmn_operator_class_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "lcs_client_class.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_plmn_operator_class_s OpenAPI_plmn_operator_class_t;
typedef struct OpenAPI_plmn_operator_class_s {
    OpenAPI_lcs_client_class_e lcs_client_class;
    OpenAPI_list_t *lcs_client_ids;
} OpenAPI_plmn_operator_class_t;

OpenAPI_plmn_operator_class_t *OpenAPI_plmn_operator_class_create(
    OpenAPI_lcs_client_class_e lcs_client_class,
    OpenAPI_list_t *lcs_client_ids
);
void OpenAPI_plmn_operator_class_free(OpenAPI_plmn_operator_class_t *plmn_operator_class);
OpenAPI_plmn_operator_class_t *OpenAPI_plmn_operator_class_parseFromJSON(cJSON *plmn_operator_classJSON);
cJSON *OpenAPI_plmn_operator_class_convertToJSON(OpenAPI_plmn_operator_class_t *plmn_operator_class);
OpenAPI_plmn_operator_class_t *OpenAPI_plmn_operator_class_copy(OpenAPI_plmn_operator_class_t *dst, OpenAPI_plmn_operator_class_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_plmn_operator_class_H_ */

