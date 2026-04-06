/*
 * lcs_mo_data_1.h
 *
 * LCS Mobile Originated Subscription Data
 */

#ifndef _OpenAPI_lcs_mo_data_1_H_
#define _OpenAPI_lcs_mo_data_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_lcs_mo_data_1_s OpenAPI_lcs_mo_data_1_t;
#include "lcs_broadcast_assistance_types_data_1.h"
#include "lcs_mo_service_class.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_lcs_mo_data_1_s {
    OpenAPI_list_t *allowed_service_classes;
    struct OpenAPI_lcs_broadcast_assistance_types_data_1_s *mo_assistance_data_types;
};

OpenAPI_lcs_mo_data_1_t *OpenAPI_lcs_mo_data_1_create(
    OpenAPI_list_t *allowed_service_classes,
    OpenAPI_lcs_broadcast_assistance_types_data_1_t *mo_assistance_data_types
);
void OpenAPI_lcs_mo_data_1_free(OpenAPI_lcs_mo_data_1_t *lcs_mo_data_1);
OpenAPI_lcs_mo_data_1_t *OpenAPI_lcs_mo_data_1_parseFromJSON(cJSON *lcs_mo_data_1JSON);
cJSON *OpenAPI_lcs_mo_data_1_convertToJSON(OpenAPI_lcs_mo_data_1_t *lcs_mo_data_1);
OpenAPI_lcs_mo_data_1_t *OpenAPI_lcs_mo_data_1_copy(OpenAPI_lcs_mo_data_1_t *dst, OpenAPI_lcs_mo_data_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_lcs_mo_data_1_H_ */

