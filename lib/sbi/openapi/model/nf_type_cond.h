/*
 * nf_type_cond.h
 *
 * Subscription to a set of NFs based on their NF Type
 */

#ifndef _OpenAPI_nf_type_cond_H_
#define _OpenAPI_nf_type_cond_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "nf_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nf_type_cond_s OpenAPI_nf_type_cond_t;
typedef struct OpenAPI_nf_type_cond_s {
    OpenAPI_nf_type_e nf_type;
} OpenAPI_nf_type_cond_t;

OpenAPI_nf_type_cond_t *OpenAPI_nf_type_cond_create(
    OpenAPI_nf_type_e nf_type
);
void OpenAPI_nf_type_cond_free(OpenAPI_nf_type_cond_t *nf_type_cond);
OpenAPI_nf_type_cond_t *OpenAPI_nf_type_cond_parseFromJSON(cJSON *nf_type_condJSON);
cJSON *OpenAPI_nf_type_cond_convertToJSON(OpenAPI_nf_type_cond_t *nf_type_cond);
OpenAPI_nf_type_cond_t *OpenAPI_nf_type_cond_copy(OpenAPI_nf_type_cond_t *dst, OpenAPI_nf_type_cond_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nf_type_cond_H_ */

