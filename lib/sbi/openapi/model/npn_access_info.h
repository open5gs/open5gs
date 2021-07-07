/*
 * npn_access_info.h
 *
 * NPN Access Information.
 */

#ifndef _OpenAPI_npn_access_info_H_
#define _OpenAPI_npn_access_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_npn_access_info_s OpenAPI_npn_access_info_t;
typedef struct OpenAPI_npn_access_info_s {
    OpenAPI_list_t *cell_cag_info;
} OpenAPI_npn_access_info_t;

OpenAPI_npn_access_info_t *OpenAPI_npn_access_info_create(
    OpenAPI_list_t *cell_cag_info
);
void OpenAPI_npn_access_info_free(OpenAPI_npn_access_info_t *npn_access_info);
OpenAPI_npn_access_info_t *OpenAPI_npn_access_info_parseFromJSON(cJSON *npn_access_infoJSON);
cJSON *OpenAPI_npn_access_info_convertToJSON(OpenAPI_npn_access_info_t *npn_access_info);
OpenAPI_npn_access_info_t *OpenAPI_npn_access_info_copy(OpenAPI_npn_access_info_t *dst, OpenAPI_npn_access_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_npn_access_info_H_ */

