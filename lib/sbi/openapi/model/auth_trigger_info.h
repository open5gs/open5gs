/*
 * auth_trigger_info.h
 *
 * This data type contains the SUPI for authorization information. 
 */

#ifndef _OpenAPI_auth_trigger_info_H_
#define _OpenAPI_auth_trigger_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_auth_trigger_info_s OpenAPI_auth_trigger_info_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_auth_trigger_info_s {
    char *supi;
};

OpenAPI_auth_trigger_info_t *OpenAPI_auth_trigger_info_create(
    char *supi
);
void OpenAPI_auth_trigger_info_free(OpenAPI_auth_trigger_info_t *auth_trigger_info);
OpenAPI_auth_trigger_info_t *OpenAPI_auth_trigger_info_parseFromJSON(cJSON *auth_trigger_infoJSON);
cJSON *OpenAPI_auth_trigger_info_convertToJSON(OpenAPI_auth_trigger_info_t *auth_trigger_info);
OpenAPI_auth_trigger_info_t *OpenAPI_auth_trigger_info_copy(OpenAPI_auth_trigger_info_t *dst, OpenAPI_auth_trigger_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_auth_trigger_info_H_ */

