/*
 * eps_bearer_info.h
 *
 * 
 */

#ifndef _OpenAPI_eps_bearer_info_H_
#define _OpenAPI_eps_bearer_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_eps_bearer_info_s OpenAPI_eps_bearer_info_t;
typedef struct OpenAPI_eps_bearer_info_s {
    int ebi;
    char pgw_s8u_fteid;
    char bearer_level_qo_s;
} OpenAPI_eps_bearer_info_t;

OpenAPI_eps_bearer_info_t *OpenAPI_eps_bearer_info_create(
    int ebi,
    char pgw_s8u_fteid,
    char bearer_level_qo_s
);
void OpenAPI_eps_bearer_info_free(OpenAPI_eps_bearer_info_t *eps_bearer_info);
OpenAPI_eps_bearer_info_t *OpenAPI_eps_bearer_info_parseFromJSON(cJSON *eps_bearer_infoJSON);
cJSON *OpenAPI_eps_bearer_info_convertToJSON(OpenAPI_eps_bearer_info_t *eps_bearer_info);
OpenAPI_eps_bearer_info_t *OpenAPI_eps_bearer_info_copy(OpenAPI_eps_bearer_info_t *dst, OpenAPI_eps_bearer_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_eps_bearer_info_H_ */

