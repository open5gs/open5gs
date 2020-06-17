/*
 * ambr_rm.h
 *
 *
 */

#ifndef _OpenAPI_ambr_rm_H_
#define _OpenAPI_ambr_rm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ambr_rm_s OpenAPI_ambr_rm_t;
typedef struct OpenAPI_ambr_rm_s {
    char *uplink;
    char *downlink;
} OpenAPI_ambr_rm_t;

OpenAPI_ambr_rm_t *OpenAPI_ambr_rm_create(
    char *uplink,
    char *downlink
    );
void OpenAPI_ambr_rm_free(OpenAPI_ambr_rm_t *ambr_rm);
OpenAPI_ambr_rm_t *OpenAPI_ambr_rm_parseFromJSON(cJSON *ambr_rmJSON);
cJSON *OpenAPI_ambr_rm_convertToJSON(OpenAPI_ambr_rm_t *ambr_rm);
OpenAPI_ambr_rm_t *OpenAPI_ambr_rm_copy(OpenAPI_ambr_rm_t *dst, OpenAPI_ambr_rm_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ambr_rm_H_ */

