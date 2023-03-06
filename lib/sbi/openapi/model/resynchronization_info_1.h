/*
 * resynchronization_info_1.h
 *
 * Contains RAND and AUTS
 */

#ifndef _OpenAPI_resynchronization_info_1_H_
#define _OpenAPI_resynchronization_info_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_resynchronization_info_1_s OpenAPI_resynchronization_info_1_t;
typedef struct OpenAPI_resynchronization_info_1_s {
    char *rand;
    char *auts;
} OpenAPI_resynchronization_info_1_t;

OpenAPI_resynchronization_info_1_t *OpenAPI_resynchronization_info_1_create(
    char *rand,
    char *auts
);
void OpenAPI_resynchronization_info_1_free(OpenAPI_resynchronization_info_1_t *resynchronization_info_1);
OpenAPI_resynchronization_info_1_t *OpenAPI_resynchronization_info_1_parseFromJSON(cJSON *resynchronization_info_1JSON);
cJSON *OpenAPI_resynchronization_info_1_convertToJSON(OpenAPI_resynchronization_info_1_t *resynchronization_info_1);
OpenAPI_resynchronization_info_1_t *OpenAPI_resynchronization_info_1_copy(OpenAPI_resynchronization_info_1_t *dst, OpenAPI_resynchronization_info_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_resynchronization_info_1_H_ */

