/*
 * aiotf_info.h
 *
 * Information of an AIOTF NF Instance
 */

#ifndef _OpenAPI_aiotf_info_H_
#define _OpenAPI_aiotf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_aiotf_info_s OpenAPI_aiotf_info_t;
#include "aiot_area_id.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_aiotf_info_s {
    OpenAPI_list_t *aiot_area_id_list;
};

OpenAPI_aiotf_info_t *OpenAPI_aiotf_info_create(
    OpenAPI_list_t *aiot_area_id_list
);
void OpenAPI_aiotf_info_free(OpenAPI_aiotf_info_t *aiotf_info);
OpenAPI_aiotf_info_t *OpenAPI_aiotf_info_parseFromJSON(cJSON *aiotf_infoJSON);
cJSON *OpenAPI_aiotf_info_convertToJSON(OpenAPI_aiotf_info_t *aiotf_info);
OpenAPI_aiotf_info_t *OpenAPI_aiotf_info_copy(OpenAPI_aiotf_info_t *dst, OpenAPI_aiotf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_aiotf_info_H_ */

