/*
 * bw_requirement.h
 *
 * Represents bandwidth requirements.
 */

#ifndef _OpenAPI_bw_requirement_H_
#define _OpenAPI_bw_requirement_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_bw_requirement_s OpenAPI_bw_requirement_t;
typedef struct OpenAPI_bw_requirement_s {
    char *app_id;
    char *mar_bw_dl;
    char *mar_bw_ul;
    char *mir_bw_dl;
    char *mir_bw_ul;
} OpenAPI_bw_requirement_t;

OpenAPI_bw_requirement_t *OpenAPI_bw_requirement_create(
    char *app_id,
    char *mar_bw_dl,
    char *mar_bw_ul,
    char *mir_bw_dl,
    char *mir_bw_ul
);
void OpenAPI_bw_requirement_free(OpenAPI_bw_requirement_t *bw_requirement);
OpenAPI_bw_requirement_t *OpenAPI_bw_requirement_parseFromJSON(cJSON *bw_requirementJSON);
cJSON *OpenAPI_bw_requirement_convertToJSON(OpenAPI_bw_requirement_t *bw_requirement);
OpenAPI_bw_requirement_t *OpenAPI_bw_requirement_copy(OpenAPI_bw_requirement_t *dst, OpenAPI_bw_requirement_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_bw_requirement_H_ */

