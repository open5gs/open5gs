/*
 * application_volume.h
 *
 * Application data volume per Application Id.
 */

#ifndef _OpenAPI_application_volume_H_
#define _OpenAPI_application_volume_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_application_volume_s OpenAPI_application_volume_t;
typedef struct OpenAPI_application_volume_s {
    char *app_id;
    long app_volume;
} OpenAPI_application_volume_t;

OpenAPI_application_volume_t *OpenAPI_application_volume_create(
    char *app_id,
    long app_volume
);
void OpenAPI_application_volume_free(OpenAPI_application_volume_t *application_volume);
OpenAPI_application_volume_t *OpenAPI_application_volume_parseFromJSON(cJSON *application_volumeJSON);
cJSON *OpenAPI_application_volume_convertToJSON(OpenAPI_application_volume_t *application_volume);
OpenAPI_application_volume_t *OpenAPI_application_volume_copy(OpenAPI_application_volume_t *dst, OpenAPI_application_volume_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_application_volume_H_ */

