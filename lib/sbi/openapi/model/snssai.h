/*
 * snssai.h
 *
 * 
 */

#ifndef _OpenAPI_snssai_H_
#define _OpenAPI_snssai_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_snssai_s OpenAPI_snssai_t;
typedef struct OpenAPI_snssai_s {
    int sst;
    char *sd;
} OpenAPI_snssai_t;

OpenAPI_snssai_t *OpenAPI_snssai_create(
    int sst,
    char *sd
);
void OpenAPI_snssai_free(OpenAPI_snssai_t *snssai);
OpenAPI_snssai_t *OpenAPI_snssai_parseFromJSON(cJSON *snssaiJSON);
cJSON *OpenAPI_snssai_convertToJSON(OpenAPI_snssai_t *snssai);
OpenAPI_snssai_t *OpenAPI_snssai_copy(OpenAPI_snssai_t *dst, OpenAPI_snssai_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_snssai_H_ */

