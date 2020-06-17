/*
 * cm_info.h
 *
 *
 */

#ifndef _OpenAPI_cm_info_H_
#define _OpenAPI_cm_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_type.h"
#include "cm_state.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_cm_info_s OpenAPI_cm_info_t;
typedef struct OpenAPI_cm_info_s {
    struct OpenAPI_cm_state_s *cm_state;
    OpenAPI_access_type_e access_type;
} OpenAPI_cm_info_t;

OpenAPI_cm_info_t *OpenAPI_cm_info_create(
    OpenAPI_cm_state_t *cm_state,
    OpenAPI_access_type_e access_type
    );
void OpenAPI_cm_info_free(OpenAPI_cm_info_t *cm_info);
OpenAPI_cm_info_t *OpenAPI_cm_info_parseFromJSON(cJSON *cm_infoJSON);
cJSON *OpenAPI_cm_info_convertToJSON(OpenAPI_cm_info_t *cm_info);
OpenAPI_cm_info_t *OpenAPI_cm_info_copy(OpenAPI_cm_info_t *dst, OpenAPI_cm_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_cm_info_H_ */

