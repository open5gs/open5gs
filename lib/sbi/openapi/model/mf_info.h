/*
 * mf_info.h
 *
 * Information of a MF NF Instance
 */

#ifndef _OpenAPI_mf_info_H_
#define _OpenAPI_mf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_mf_info_s OpenAPI_mf_info_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_mf_info_s {
    OpenAPI_list_t *media_capability_list;
};

OpenAPI_mf_info_t *OpenAPI_mf_info_create(
    OpenAPI_list_t *media_capability_list
);
void OpenAPI_mf_info_free(OpenAPI_mf_info_t *mf_info);
OpenAPI_mf_info_t *OpenAPI_mf_info_parseFromJSON(cJSON *mf_infoJSON);
cJSON *OpenAPI_mf_info_convertToJSON(OpenAPI_mf_info_t *mf_info);
OpenAPI_mf_info_t *OpenAPI_mf_info_copy(OpenAPI_mf_info_t *dst, OpenAPI_mf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mf_info_H_ */

