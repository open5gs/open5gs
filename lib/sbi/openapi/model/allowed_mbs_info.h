/*
 * allowed_mbs_info.h
 *
 * 
 */

#ifndef _OpenAPI_allowed_mbs_info_H_
#define _OpenAPI_allowed_mbs_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_allowed_mbs_info_s OpenAPI_allowed_mbs_info_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_allowed_mbs_info_s {
    char *af_id;
};

OpenAPI_allowed_mbs_info_t *OpenAPI_allowed_mbs_info_create(
    char *af_id
);
void OpenAPI_allowed_mbs_info_free(OpenAPI_allowed_mbs_info_t *allowed_mbs_info);
OpenAPI_allowed_mbs_info_t *OpenAPI_allowed_mbs_info_parseFromJSON(cJSON *allowed_mbs_infoJSON);
cJSON *OpenAPI_allowed_mbs_info_convertToJSON(OpenAPI_allowed_mbs_info_t *allowed_mbs_info);
OpenAPI_allowed_mbs_info_t *OpenAPI_allowed_mbs_info_copy(OpenAPI_allowed_mbs_info_t *dst, OpenAPI_allowed_mbs_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_allowed_mbs_info_H_ */

