/*
 * additional_access_info.h
 *
 *
 */

#ifndef _OpenAPI_additional_access_info_H_
#define _OpenAPI_additional_access_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_type.h"
#include "rat_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_additional_access_info_s OpenAPI_additional_access_info_t;
typedef struct OpenAPI_additional_access_info_s {
    OpenAPI_access_type_e access_type;
    OpenAPI_rat_type_e rat_type;
} OpenAPI_additional_access_info_t;

OpenAPI_additional_access_info_t *OpenAPI_additional_access_info_create(
    OpenAPI_access_type_e access_type,
    OpenAPI_rat_type_e rat_type
    );
void OpenAPI_additional_access_info_free(OpenAPI_additional_access_info_t *additional_access_info);
OpenAPI_additional_access_info_t *OpenAPI_additional_access_info_parseFromJSON(cJSON *additional_access_infoJSON);
cJSON *OpenAPI_additional_access_info_convertToJSON(OpenAPI_additional_access_info_t *additional_access_info);
OpenAPI_additional_access_info_t *OpenAPI_additional_access_info_copy(OpenAPI_additional_access_info_t *dst, OpenAPI_additional_access_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_additional_access_info_H_ */

