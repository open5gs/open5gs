/*
 * amf_cond.h
 *
 * Subscription to a set of AMFs, based on AMF Set Id and/or AMF Region Id
 */

#ifndef _OpenAPI_amf_cond_H_
#define _OpenAPI_amf_cond_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_amf_cond_s OpenAPI_amf_cond_t;
typedef struct OpenAPI_amf_cond_s {
    char *amf_set_id;
    char *amf_region_id;
} OpenAPI_amf_cond_t;

OpenAPI_amf_cond_t *OpenAPI_amf_cond_create(
    char *amf_set_id,
    char *amf_region_id
);
void OpenAPI_amf_cond_free(OpenAPI_amf_cond_t *amf_cond);
OpenAPI_amf_cond_t *OpenAPI_amf_cond_parseFromJSON(cJSON *amf_condJSON);
cJSON *OpenAPI_amf_cond_convertToJSON(OpenAPI_amf_cond_t *amf_cond);
OpenAPI_amf_cond_t *OpenAPI_amf_cond_copy(OpenAPI_amf_cond_t *dst, OpenAPI_amf_cond_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_amf_cond_H_ */

