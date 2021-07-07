/*
 * lcs_broadcast_assistance_types_data.h
 *
 * 
 */

#ifndef _OpenAPI_lcs_broadcast_assistance_types_data_H_
#define _OpenAPI_lcs_broadcast_assistance_types_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_lcs_broadcast_assistance_types_data_s OpenAPI_lcs_broadcast_assistance_types_data_t;
typedef struct OpenAPI_lcs_broadcast_assistance_types_data_s {
    OpenAPI_list_t *location_assistance_type;
} OpenAPI_lcs_broadcast_assistance_types_data_t;

OpenAPI_lcs_broadcast_assistance_types_data_t *OpenAPI_lcs_broadcast_assistance_types_data_create(
    OpenAPI_list_t *location_assistance_type
);
void OpenAPI_lcs_broadcast_assistance_types_data_free(OpenAPI_lcs_broadcast_assistance_types_data_t *lcs_broadcast_assistance_types_data);
OpenAPI_lcs_broadcast_assistance_types_data_t *OpenAPI_lcs_broadcast_assistance_types_data_parseFromJSON(cJSON *lcs_broadcast_assistance_types_dataJSON);
cJSON *OpenAPI_lcs_broadcast_assistance_types_data_convertToJSON(OpenAPI_lcs_broadcast_assistance_types_data_t *lcs_broadcast_assistance_types_data);
OpenAPI_lcs_broadcast_assistance_types_data_t *OpenAPI_lcs_broadcast_assistance_types_data_copy(OpenAPI_lcs_broadcast_assistance_types_data_t *dst, OpenAPI_lcs_broadcast_assistance_types_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_lcs_broadcast_assistance_types_data_H_ */

