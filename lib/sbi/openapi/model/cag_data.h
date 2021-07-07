/*
 * cag_data.h
 *
 * 
 */

#ifndef _OpenAPI_cag_data_H_
#define _OpenAPI_cag_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "cag_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_cag_data_s OpenAPI_cag_data_t;
typedef struct OpenAPI_cag_data_s {
    OpenAPI_list_t* cag_infos;
    char *provisioning_time;
} OpenAPI_cag_data_t;

OpenAPI_cag_data_t *OpenAPI_cag_data_create(
    OpenAPI_list_t* cag_infos,
    char *provisioning_time
);
void OpenAPI_cag_data_free(OpenAPI_cag_data_t *cag_data);
OpenAPI_cag_data_t *OpenAPI_cag_data_parseFromJSON(cJSON *cag_dataJSON);
cJSON *OpenAPI_cag_data_convertToJSON(OpenAPI_cag_data_t *cag_data);
OpenAPI_cag_data_t *OpenAPI_cag_data_copy(OpenAPI_cag_data_t *dst, OpenAPI_cag_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_cag_data_H_ */

