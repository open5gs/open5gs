/*
 * guami_list_cond.h
 *
 * Subscription to a set of AMFs, based on their GUAMIs
 */

#ifndef _OpenAPI_guami_list_cond_H_
#define _OpenAPI_guami_list_cond_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "guami.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_guami_list_cond_s OpenAPI_guami_list_cond_t;
typedef struct OpenAPI_guami_list_cond_s {
    OpenAPI_list_t *guami_list;
} OpenAPI_guami_list_cond_t;

OpenAPI_guami_list_cond_t *OpenAPI_guami_list_cond_create(
    OpenAPI_list_t *guami_list
);
void OpenAPI_guami_list_cond_free(OpenAPI_guami_list_cond_t *guami_list_cond);
OpenAPI_guami_list_cond_t *OpenAPI_guami_list_cond_parseFromJSON(cJSON *guami_list_condJSON);
cJSON *OpenAPI_guami_list_cond_convertToJSON(OpenAPI_guami_list_cond_t *guami_list_cond);
OpenAPI_guami_list_cond_t *OpenAPI_guami_list_cond_copy(OpenAPI_guami_list_cond_t *dst, OpenAPI_guami_list_cond_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_guami_list_cond_H_ */

