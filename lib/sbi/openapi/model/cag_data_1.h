/*
 * cag_data_1.h
 *
 * 
 */

#ifndef _OpenAPI_cag_data_1_H_
#define _OpenAPI_cag_data_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "cag_info_1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_cag_data_1_s OpenAPI_cag_data_1_t;
typedef struct OpenAPI_cag_data_1_s {
    OpenAPI_list_t* cag_infos;
    char *provisioning_time;
} OpenAPI_cag_data_1_t;

OpenAPI_cag_data_1_t *OpenAPI_cag_data_1_create(
    OpenAPI_list_t* cag_infos,
    char *provisioning_time
);
void OpenAPI_cag_data_1_free(OpenAPI_cag_data_1_t *cag_data_1);
OpenAPI_cag_data_1_t *OpenAPI_cag_data_1_parseFromJSON(cJSON *cag_data_1JSON);
cJSON *OpenAPI_cag_data_1_convertToJSON(OpenAPI_cag_data_1_t *cag_data_1);
OpenAPI_cag_data_1_t *OpenAPI_cag_data_1_copy(OpenAPI_cag_data_1_t *dst, OpenAPI_cag_data_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_cag_data_1_H_ */

