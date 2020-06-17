/*
 * upf_cond.h
 *
 *
 */

#ifndef _OpenAPI_upf_cond_H_
#define _OpenAPI_upf_cond_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "tai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_upf_cond_s OpenAPI_upf_cond_t;
typedef struct OpenAPI_upf_cond_s {
    OpenAPI_list_t *smf_serving_area;
    OpenAPI_list_t *tai_list;
} OpenAPI_upf_cond_t;

OpenAPI_upf_cond_t *OpenAPI_upf_cond_create(
    OpenAPI_list_t *smf_serving_area,
    OpenAPI_list_t *tai_list
    );
void OpenAPI_upf_cond_free(OpenAPI_upf_cond_t *upf_cond);
OpenAPI_upf_cond_t *OpenAPI_upf_cond_parseFromJSON(cJSON *upf_condJSON);
cJSON *OpenAPI_upf_cond_convertToJSON(OpenAPI_upf_cond_t *upf_cond);
OpenAPI_upf_cond_t *OpenAPI_upf_cond_copy(OpenAPI_upf_cond_t *dst, OpenAPI_upf_cond_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_upf_cond_H_ */

