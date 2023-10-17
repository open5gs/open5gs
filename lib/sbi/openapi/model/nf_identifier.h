/*
 * nf_identifier.h
 *
 * 
 */

#ifndef _OpenAPI_nf_identifier_H_
#define _OpenAPI_nf_identifier_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "nf_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nf_identifier_s OpenAPI_nf_identifier_t;
typedef struct OpenAPI_nf_identifier_s {
    OpenAPI_nf_type_e nf_type;
    char *nf_instance_id;
} OpenAPI_nf_identifier_t;

OpenAPI_nf_identifier_t *OpenAPI_nf_identifier_create(
    OpenAPI_nf_type_e nf_type,
    char *nf_instance_id
);
void OpenAPI_nf_identifier_free(OpenAPI_nf_identifier_t *nf_identifier);
OpenAPI_nf_identifier_t *OpenAPI_nf_identifier_parseFromJSON(cJSON *nf_identifierJSON);
cJSON *OpenAPI_nf_identifier_convertToJSON(OpenAPI_nf_identifier_t *nf_identifier);
OpenAPI_nf_identifier_t *OpenAPI_nf_identifier_copy(OpenAPI_nf_identifier_t *dst, OpenAPI_nf_identifier_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nf_identifier_H_ */

