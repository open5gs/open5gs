/*
 * plmn_operator_class_1.h
 *
 * 
 */

#ifndef _OpenAPI_plmn_operator_class_1_H_
#define _OpenAPI_plmn_operator_class_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "lcs_client_class.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_plmn_operator_class_1_s OpenAPI_plmn_operator_class_1_t;
typedef struct OpenAPI_plmn_operator_class_1_s {
    OpenAPI_lcs_client_class_e lcs_client_class;
    OpenAPI_list_t *lcs_client_ids;
} OpenAPI_plmn_operator_class_1_t;

OpenAPI_plmn_operator_class_1_t *OpenAPI_plmn_operator_class_1_create(
    OpenAPI_lcs_client_class_e lcs_client_class,
    OpenAPI_list_t *lcs_client_ids
);
void OpenAPI_plmn_operator_class_1_free(OpenAPI_plmn_operator_class_1_t *plmn_operator_class_1);
OpenAPI_plmn_operator_class_1_t *OpenAPI_plmn_operator_class_1_parseFromJSON(cJSON *plmn_operator_class_1JSON);
cJSON *OpenAPI_plmn_operator_class_1_convertToJSON(OpenAPI_plmn_operator_class_1_t *plmn_operator_class_1);
OpenAPI_plmn_operator_class_1_t *OpenAPI_plmn_operator_class_1_copy(OpenAPI_plmn_operator_class_1_t *dst, OpenAPI_plmn_operator_class_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_plmn_operator_class_1_H_ */

