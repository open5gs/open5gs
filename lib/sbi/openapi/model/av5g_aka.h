/*
 * av5g_aka.h
 *
 * 
 */

#ifndef _OpenAPI_av5g_aka_H_
#define _OpenAPI_av5g_aka_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_av5g_aka_s OpenAPI_av5g_aka_t;
typedef struct OpenAPI_av5g_aka_s {
    char *rand;
    char *hxres_star;
    char *autn;
} OpenAPI_av5g_aka_t;

OpenAPI_av5g_aka_t *OpenAPI_av5g_aka_create(
    char *rand,
    char *hxres_star,
    char *autn
);
void OpenAPI_av5g_aka_free(OpenAPI_av5g_aka_t *av5g_aka);
OpenAPI_av5g_aka_t *OpenAPI_av5g_aka_parseFromJSON(cJSON *av5g_akaJSON);
cJSON *OpenAPI_av5g_aka_convertToJSON(OpenAPI_av5g_aka_t *av5g_aka);
OpenAPI_av5g_aka_t *OpenAPI_av5g_aka_copy(OpenAPI_av5g_aka_t *dst, OpenAPI_av5g_aka_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_av5g_aka_H_ */

