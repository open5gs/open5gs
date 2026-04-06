/*
 * mrfp_info.h
 *
 * Information of a Mrfp NF Instance
 */

#ifndef _OpenAPI_mrfp_info_H_
#define _OpenAPI_mrfp_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_mrfp_info_s OpenAPI_mrfp_info_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_mrfp_info_s {
    OpenAPI_list_t *media_capability_list;
};

OpenAPI_mrfp_info_t *OpenAPI_mrfp_info_create(
    OpenAPI_list_t *media_capability_list
);
void OpenAPI_mrfp_info_free(OpenAPI_mrfp_info_t *mrfp_info);
OpenAPI_mrfp_info_t *OpenAPI_mrfp_info_parseFromJSON(cJSON *mrfp_infoJSON);
cJSON *OpenAPI_mrfp_info_convertToJSON(OpenAPI_mrfp_info_t *mrfp_info);
OpenAPI_mrfp_info_t *OpenAPI_mrfp_info_copy(OpenAPI_mrfp_info_t *dst, OpenAPI_mrfp_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mrfp_info_H_ */

