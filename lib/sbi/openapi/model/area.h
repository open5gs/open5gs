/*
 * area.h
 *
 *
 */

#ifndef _OpenAPI_area_H_
#define _OpenAPI_area_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_area_s OpenAPI_area_t;
typedef struct OpenAPI_area_s {
    OpenAPI_list_t *tacs;
    char *area_code;
} OpenAPI_area_t;

OpenAPI_area_t *OpenAPI_area_create(
    OpenAPI_list_t *tacs,
    char *area_code
    );
void OpenAPI_area_free(OpenAPI_area_t *area);
OpenAPI_area_t *OpenAPI_area_parseFromJSON(cJSON *areaJSON);
cJSON *OpenAPI_area_convertToJSON(OpenAPI_area_t *area);
OpenAPI_area_t *OpenAPI_area_copy(OpenAPI_area_t *dst, OpenAPI_area_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_area_H_ */

