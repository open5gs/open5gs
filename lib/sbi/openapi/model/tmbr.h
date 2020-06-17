/*
 * tmbr.h
 *
 *
 */

#ifndef _OpenAPI_tmbr_H_
#define _OpenAPI_tmbr_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_tmbr_s OpenAPI_tmbr_t;
typedef struct OpenAPI_tmbr_s {
    char *uplink;
    char *downlink;
} OpenAPI_tmbr_t;

OpenAPI_tmbr_t *OpenAPI_tmbr_create(
    char *uplink,
    char *downlink
    );
void OpenAPI_tmbr_free(OpenAPI_tmbr_t *tmbr);
OpenAPI_tmbr_t *OpenAPI_tmbr_parseFromJSON(cJSON *tmbrJSON);
cJSON *OpenAPI_tmbr_convertToJSON(OpenAPI_tmbr_t *tmbr);
OpenAPI_tmbr_t *OpenAPI_tmbr_copy(OpenAPI_tmbr_t *dst, OpenAPI_tmbr_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_tmbr_H_ */

