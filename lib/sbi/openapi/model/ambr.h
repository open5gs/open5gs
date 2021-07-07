/*
 * ambr.h
 *
 * 
 */

#ifndef _OpenAPI_ambr_H_
#define _OpenAPI_ambr_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ambr_s OpenAPI_ambr_t;
typedef struct OpenAPI_ambr_s {
    char *uplink;
    char *downlink;
} OpenAPI_ambr_t;

OpenAPI_ambr_t *OpenAPI_ambr_create(
    char *uplink,
    char *downlink
);
void OpenAPI_ambr_free(OpenAPI_ambr_t *ambr);
OpenAPI_ambr_t *OpenAPI_ambr_parseFromJSON(cJSON *ambrJSON);
cJSON *OpenAPI_ambr_convertToJSON(OpenAPI_ambr_t *ambr);
OpenAPI_ambr_t *OpenAPI_ambr_copy(OpenAPI_ambr_t *dst, OpenAPI_ambr_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ambr_H_ */

