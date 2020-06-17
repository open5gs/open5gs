/*
 * key_amf.h
 *
 *
 */

#ifndef _OpenAPI_key_amf_H_
#define _OpenAPI_key_amf_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "key_amf_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_key_amf_s OpenAPI_key_amf_t;
typedef struct OpenAPI_key_amf_s {
    OpenAPI_key_amf_type_e key_type;
    char *key_val;
} OpenAPI_key_amf_t;

OpenAPI_key_amf_t *OpenAPI_key_amf_create(
    OpenAPI_key_amf_type_e key_type,
    char *key_val
    );
void OpenAPI_key_amf_free(OpenAPI_key_amf_t *key_amf);
OpenAPI_key_amf_t *OpenAPI_key_amf_parseFromJSON(cJSON *key_amfJSON);
cJSON *OpenAPI_key_amf_convertToJSON(OpenAPI_key_amf_t *key_amf);
OpenAPI_key_amf_t *OpenAPI_key_amf_copy(OpenAPI_key_amf_t *dst, OpenAPI_key_amf_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_key_amf_H_ */

