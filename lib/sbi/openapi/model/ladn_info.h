/*
 * ladn_info.h
 *
 * 
 */

#ifndef _OpenAPI_ladn_info_H_
#define _OpenAPI_ladn_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "presence_state.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ladn_info_s OpenAPI_ladn_info_t;
typedef struct OpenAPI_ladn_info_s {
    char *ladn;
    OpenAPI_presence_state_e presence;
} OpenAPI_ladn_info_t;

OpenAPI_ladn_info_t *OpenAPI_ladn_info_create(
    char *ladn,
    OpenAPI_presence_state_e presence
);
void OpenAPI_ladn_info_free(OpenAPI_ladn_info_t *ladn_info);
OpenAPI_ladn_info_t *OpenAPI_ladn_info_parseFromJSON(cJSON *ladn_infoJSON);
cJSON *OpenAPI_ladn_info_convertToJSON(OpenAPI_ladn_info_t *ladn_info);
OpenAPI_ladn_info_t *OpenAPI_ladn_info_copy(OpenAPI_ladn_info_t *dst, OpenAPI_ladn_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ladn_info_H_ */

