/*
 * lcs_mo_data.h
 *
 * 
 */

#ifndef _OpenAPI_lcs_mo_data_H_
#define _OpenAPI_lcs_mo_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "lcs_broadcast_assistance_types_data.h"
#include "lcs_mo_service_class.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_lcs_mo_data_s OpenAPI_lcs_mo_data_t;
typedef struct OpenAPI_lcs_mo_data_s {
    OpenAPI_list_t *allowed_service_classes;
    struct OpenAPI_lcs_broadcast_assistance_types_data_s *mo_assistance_data_types;
} OpenAPI_lcs_mo_data_t;

OpenAPI_lcs_mo_data_t *OpenAPI_lcs_mo_data_create(
    OpenAPI_list_t *allowed_service_classes,
    OpenAPI_lcs_broadcast_assistance_types_data_t *mo_assistance_data_types
);
void OpenAPI_lcs_mo_data_free(OpenAPI_lcs_mo_data_t *lcs_mo_data);
OpenAPI_lcs_mo_data_t *OpenAPI_lcs_mo_data_parseFromJSON(cJSON *lcs_mo_dataJSON);
cJSON *OpenAPI_lcs_mo_data_convertToJSON(OpenAPI_lcs_mo_data_t *lcs_mo_data);
OpenAPI_lcs_mo_data_t *OpenAPI_lcs_mo_data_copy(OpenAPI_lcs_mo_data_t *dst, OpenAPI_lcs_mo_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_lcs_mo_data_H_ */

