/*
 * ie_info.h
 *
 * Protection and modification policy for the IE
 */

#ifndef _OpenAPI_ie_info_H_
#define _OpenAPI_ie_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ie_location.h"
#include "ie_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ie_info_s OpenAPI_ie_info_t;
typedef struct OpenAPI_ie_info_s {
    OpenAPI_ie_location_e ie_loc;
    OpenAPI_ie_type_e ie_type;
    char *req_ie;
    char *rsp_ie;
    bool is_is_modifiable;
    int is_modifiable;
    OpenAPI_list_t* is_modifiable_by_ipx;
} OpenAPI_ie_info_t;

OpenAPI_ie_info_t *OpenAPI_ie_info_create(
    OpenAPI_ie_location_e ie_loc,
    OpenAPI_ie_type_e ie_type,
    char *req_ie,
    char *rsp_ie,
    bool is_is_modifiable,
    int is_modifiable,
    OpenAPI_list_t* is_modifiable_by_ipx
);
void OpenAPI_ie_info_free(OpenAPI_ie_info_t *ie_info);
OpenAPI_ie_info_t *OpenAPI_ie_info_parseFromJSON(cJSON *ie_infoJSON);
cJSON *OpenAPI_ie_info_convertToJSON(OpenAPI_ie_info_t *ie_info);
OpenAPI_ie_info_t *OpenAPI_ie_info_copy(OpenAPI_ie_info_t *dst, OpenAPI_ie_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ie_info_H_ */

