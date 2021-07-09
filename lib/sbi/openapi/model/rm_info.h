/*
 * rm_info.h
 *
 * 
 */

#ifndef _OpenAPI_rm_info_H_
#define _OpenAPI_rm_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_type.h"
#include "rm_state.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_rm_info_s OpenAPI_rm_info_t;
typedef struct OpenAPI_rm_info_s {
    struct OpenAPI_rm_state_s *rm_state;
    OpenAPI_access_type_e access_type;
} OpenAPI_rm_info_t;

OpenAPI_rm_info_t *OpenAPI_rm_info_create(
    OpenAPI_rm_state_t *rm_state,
    OpenAPI_access_type_e access_type
);
void OpenAPI_rm_info_free(OpenAPI_rm_info_t *rm_info);
OpenAPI_rm_info_t *OpenAPI_rm_info_parseFromJSON(cJSON *rm_infoJSON);
cJSON *OpenAPI_rm_info_convertToJSON(OpenAPI_rm_info_t *rm_info);
OpenAPI_rm_info_t *OpenAPI_rm_info_copy(OpenAPI_rm_info_t *dst, OpenAPI_rm_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_rm_info_H_ */

