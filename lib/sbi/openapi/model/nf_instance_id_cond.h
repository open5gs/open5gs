/*
 * nf_instance_id_cond.h
 *
 * Subscription to a given NF Instance Id
 */

#ifndef _OpenAPI_nf_instance_id_cond_H_
#define _OpenAPI_nf_instance_id_cond_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nf_instance_id_cond_s OpenAPI_nf_instance_id_cond_t;
typedef struct OpenAPI_nf_instance_id_cond_s {
    char *nf_instance_id;
} OpenAPI_nf_instance_id_cond_t;

OpenAPI_nf_instance_id_cond_t *OpenAPI_nf_instance_id_cond_create(
    char *nf_instance_id
    );
void OpenAPI_nf_instance_id_cond_free(OpenAPI_nf_instance_id_cond_t *nf_instance_id_cond);
OpenAPI_nf_instance_id_cond_t *OpenAPI_nf_instance_id_cond_parseFromJSON(cJSON *nf_instance_id_condJSON);
cJSON *OpenAPI_nf_instance_id_cond_convertToJSON(OpenAPI_nf_instance_id_cond_t *nf_instance_id_cond);
OpenAPI_nf_instance_id_cond_t *OpenAPI_nf_instance_id_cond_copy(OpenAPI_nf_instance_id_cond_t *dst, OpenAPI_nf_instance_id_cond_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nf_instance_id_cond_H_ */

