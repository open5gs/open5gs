/*
 * user_identifier.h
 *
 * 
 */

#ifndef _OpenAPI_user_identifier_H_
#define _OpenAPI_user_identifier_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_user_identifier_s OpenAPI_user_identifier_t;
typedef struct OpenAPI_user_identifier_s {
    char *supi;
    char *gpsi;
    char *validity_time;
} OpenAPI_user_identifier_t;

OpenAPI_user_identifier_t *OpenAPI_user_identifier_create(
    char *supi,
    char *gpsi,
    char *validity_time
);
void OpenAPI_user_identifier_free(OpenAPI_user_identifier_t *user_identifier);
OpenAPI_user_identifier_t *OpenAPI_user_identifier_parseFromJSON(cJSON *user_identifierJSON);
cJSON *OpenAPI_user_identifier_convertToJSON(OpenAPI_user_identifier_t *user_identifier);
OpenAPI_user_identifier_t *OpenAPI_user_identifier_copy(OpenAPI_user_identifier_t *dst, OpenAPI_user_identifier_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_user_identifier_H_ */

