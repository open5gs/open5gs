/*
 * mrf_info.h
 *
 * Information of a Mrf NF Instance
 */

#ifndef _OpenAPI_mrf_info_H_
#define _OpenAPI_mrf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_mrf_info_s OpenAPI_mrf_info_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_mrf_info_s {
    OpenAPI_list_t *media_capability_list;
};

OpenAPI_mrf_info_t *OpenAPI_mrf_info_create(
    OpenAPI_list_t *media_capability_list
);
void OpenAPI_mrf_info_free(OpenAPI_mrf_info_t *mrf_info);
OpenAPI_mrf_info_t *OpenAPI_mrf_info_parseFromJSON(cJSON *mrf_infoJSON);
cJSON *OpenAPI_mrf_info_convertToJSON(OpenAPI_mrf_info_t *mrf_info);
OpenAPI_mrf_info_t *OpenAPI_mrf_info_copy(OpenAPI_mrf_info_t *dst, OpenAPI_mrf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mrf_info_H_ */

