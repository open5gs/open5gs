/*
 * pei_update_info.h
 *
 * 
 */

#ifndef _OpenAPI_pei_update_info_H_
#define _OpenAPI_pei_update_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pei_update_info_s OpenAPI_pei_update_info_t;
typedef struct OpenAPI_pei_update_info_s {
    char *pei;
} OpenAPI_pei_update_info_t;

OpenAPI_pei_update_info_t *OpenAPI_pei_update_info_create(
    char *pei
);
void OpenAPI_pei_update_info_free(OpenAPI_pei_update_info_t *pei_update_info);
OpenAPI_pei_update_info_t *OpenAPI_pei_update_info_parseFromJSON(cJSON *pei_update_infoJSON);
cJSON *OpenAPI_pei_update_info_convertToJSON(OpenAPI_pei_update_info_t *pei_update_info);
OpenAPI_pei_update_info_t *OpenAPI_pei_update_info_copy(OpenAPI_pei_update_info_t *dst, OpenAPI_pei_update_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pei_update_info_H_ */

