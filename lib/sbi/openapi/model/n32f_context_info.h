/*
 * n32f_context_info.h
 *
 * N32-f context information
 */

#ifndef _OpenAPI_n32f_context_info_H_
#define _OpenAPI_n32f_context_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_n32f_context_info_s OpenAPI_n32f_context_info_t;
typedef struct OpenAPI_n32f_context_info_s {
    char *n32f_context_id;
} OpenAPI_n32f_context_info_t;

OpenAPI_n32f_context_info_t *OpenAPI_n32f_context_info_create(
    char *n32f_context_id
);
void OpenAPI_n32f_context_info_free(OpenAPI_n32f_context_info_t *n32f_context_info);
OpenAPI_n32f_context_info_t *OpenAPI_n32f_context_info_parseFromJSON(cJSON *n32f_context_infoJSON);
cJSON *OpenAPI_n32f_context_info_convertToJSON(OpenAPI_n32f_context_info_t *n32f_context_info);
OpenAPI_n32f_context_info_t *OpenAPI_n32f_context_info_copy(OpenAPI_n32f_context_info_t *dst, OpenAPI_n32f_context_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n32f_context_info_H_ */

