/*
 * area_1.h
 *
 * 
 */

#ifndef _OpenAPI_area_1_H_
#define _OpenAPI_area_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_area_1_s OpenAPI_area_1_t;
typedef struct OpenAPI_area_1_s {
    OpenAPI_list_t *tacs;
    char *area_code;
} OpenAPI_area_1_t;

OpenAPI_area_1_t *OpenAPI_area_1_create(
    OpenAPI_list_t *tacs,
    char *area_code
);
void OpenAPI_area_1_free(OpenAPI_area_1_t *area_1);
OpenAPI_area_1_t *OpenAPI_area_1_parseFromJSON(cJSON *area_1JSON);
cJSON *OpenAPI_area_1_convertToJSON(OpenAPI_area_1_t *area_1);
OpenAPI_area_1_t *OpenAPI_area_1_copy(OpenAPI_area_1_t *dst, OpenAPI_area_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_area_1_H_ */

