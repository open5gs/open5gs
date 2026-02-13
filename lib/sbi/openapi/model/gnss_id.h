/*
 * gnss_id.h
 *
 * Global Navigation Satellite System (GNSS) ID.
 */

#ifndef _OpenAPI_gnss_id_H_
#define _OpenAPI_gnss_id_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "gnss_id_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_gnss_id_s OpenAPI_gnss_id_t;
typedef struct OpenAPI_gnss_id_s {
} OpenAPI_gnss_id_t;

OpenAPI_gnss_id_t *OpenAPI_gnss_id_create(void);
void OpenAPI_gnss_id_free(OpenAPI_gnss_id_t *gnss_id);
OpenAPI_gnss_id_t *OpenAPI_gnss_id_parseFromJSON(cJSON *gnss_idJSON);
cJSON *OpenAPI_gnss_id_convertToJSON(OpenAPI_gnss_id_t *gnss_id);
OpenAPI_gnss_id_t *OpenAPI_gnss_id_copy(OpenAPI_gnss_id_t *dst, OpenAPI_gnss_id_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_gnss_id_H_ */

