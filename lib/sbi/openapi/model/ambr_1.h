/*
 * ambr_1.h
 *
 * Contains the maximum aggregated uplink and downlink bit rates.
 */

#ifndef _OpenAPI_ambr_1_H_
#define _OpenAPI_ambr_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ambr_1_s OpenAPI_ambr_1_t;
typedef struct OpenAPI_ambr_1_s {
    char *uplink;
    char *downlink;
} OpenAPI_ambr_1_t;

OpenAPI_ambr_1_t *OpenAPI_ambr_1_create(
    char *uplink,
    char *downlink
);
void OpenAPI_ambr_1_free(OpenAPI_ambr_1_t *ambr_1);
OpenAPI_ambr_1_t *OpenAPI_ambr_1_parseFromJSON(cJSON *ambr_1JSON);
cJSON *OpenAPI_ambr_1_convertToJSON(OpenAPI_ambr_1_t *ambr_1);
OpenAPI_ambr_1_t *OpenAPI_ambr_1_copy(OpenAPI_ambr_1_t *dst, OpenAPI_ambr_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ambr_1_H_ */

