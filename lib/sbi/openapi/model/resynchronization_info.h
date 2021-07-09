/*
 * resynchronization_info.h
 *
 * 
 */

#ifndef _OpenAPI_resynchronization_info_H_
#define _OpenAPI_resynchronization_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_resynchronization_info_s OpenAPI_resynchronization_info_t;
typedef struct OpenAPI_resynchronization_info_s {
    char *rand;
    char *auts;
} OpenAPI_resynchronization_info_t;

OpenAPI_resynchronization_info_t *OpenAPI_resynchronization_info_create(
    char *rand,
    char *auts
);
void OpenAPI_resynchronization_info_free(OpenAPI_resynchronization_info_t *resynchronization_info);
OpenAPI_resynchronization_info_t *OpenAPI_resynchronization_info_parseFromJSON(cJSON *resynchronization_infoJSON);
cJSON *OpenAPI_resynchronization_info_convertToJSON(OpenAPI_resynchronization_info_t *resynchronization_info);
OpenAPI_resynchronization_info_t *OpenAPI_resynchronization_info_copy(OpenAPI_resynchronization_info_t *dst, OpenAPI_resynchronization_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_resynchronization_info_H_ */

