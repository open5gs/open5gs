/*
 * requested_qos.h
 *
 *
 */

#ifndef _OpenAPI_requested_qos_H_
#define _OpenAPI_requested_qos_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_requested_qos_s OpenAPI_requested_qos_t;
typedef struct OpenAPI_requested_qos_s {
    int _5qi;
    char *gbr_ul;
    char *gbr_dl;
} OpenAPI_requested_qos_t;

OpenAPI_requested_qos_t *OpenAPI_requested_qos_create(
    int _5qi,
    char *gbr_ul,
    char *gbr_dl
    );
void OpenAPI_requested_qos_free(OpenAPI_requested_qos_t *requested_qos);
OpenAPI_requested_qos_t *OpenAPI_requested_qos_parseFromJSON(cJSON *requested_qosJSON);
cJSON *OpenAPI_requested_qos_convertToJSON(OpenAPI_requested_qos_t *requested_qos);
OpenAPI_requested_qos_t *OpenAPI_requested_qos_copy(OpenAPI_requested_qos_t *dst, OpenAPI_requested_qos_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_requested_qos_H_ */

