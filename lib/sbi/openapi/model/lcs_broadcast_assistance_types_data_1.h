/*
 * lcs_broadcast_assistance_types_data_1.h
 *
 * 
 */

#ifndef _OpenAPI_lcs_broadcast_assistance_types_data_1_H_
#define _OpenAPI_lcs_broadcast_assistance_types_data_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_lcs_broadcast_assistance_types_data_1_s OpenAPI_lcs_broadcast_assistance_types_data_1_t;
typedef struct OpenAPI_lcs_broadcast_assistance_types_data_1_s {
    OpenAPI_list_t *location_assistance_type;
} OpenAPI_lcs_broadcast_assistance_types_data_1_t;

OpenAPI_lcs_broadcast_assistance_types_data_1_t *OpenAPI_lcs_broadcast_assistance_types_data_1_create(
    OpenAPI_list_t *location_assistance_type
);
void OpenAPI_lcs_broadcast_assistance_types_data_1_free(OpenAPI_lcs_broadcast_assistance_types_data_1_t *lcs_broadcast_assistance_types_data_1);
OpenAPI_lcs_broadcast_assistance_types_data_1_t *OpenAPI_lcs_broadcast_assistance_types_data_1_parseFromJSON(cJSON *lcs_broadcast_assistance_types_data_1JSON);
cJSON *OpenAPI_lcs_broadcast_assistance_types_data_1_convertToJSON(OpenAPI_lcs_broadcast_assistance_types_data_1_t *lcs_broadcast_assistance_types_data_1);
OpenAPI_lcs_broadcast_assistance_types_data_1_t *OpenAPI_lcs_broadcast_assistance_types_data_1_copy(OpenAPI_lcs_broadcast_assistance_types_data_1_t *dst, OpenAPI_lcs_broadcast_assistance_types_data_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_lcs_broadcast_assistance_types_data_1_H_ */

