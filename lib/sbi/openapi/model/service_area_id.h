/*
 * service_area_id.h
 *
 * Contains a Service Area Identifier as defined in 3GPP TS 23.003, clause 12.5.
 */

#ifndef _OpenAPI_service_area_id_H_
#define _OpenAPI_service_area_id_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "plmn_id.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_service_area_id_s OpenAPI_service_area_id_t;
typedef struct OpenAPI_service_area_id_s {
    struct OpenAPI_plmn_id_s *plmn_id;
    char *lac;
    char *sac;
} OpenAPI_service_area_id_t;

OpenAPI_service_area_id_t *OpenAPI_service_area_id_create(
    OpenAPI_plmn_id_t *plmn_id,
    char *lac,
    char *sac
);
void OpenAPI_service_area_id_free(OpenAPI_service_area_id_t *service_area_id);
OpenAPI_service_area_id_t *OpenAPI_service_area_id_parseFromJSON(cJSON *service_area_idJSON);
cJSON *OpenAPI_service_area_id_convertToJSON(OpenAPI_service_area_id_t *service_area_id);
OpenAPI_service_area_id_t *OpenAPI_service_area_id_copy(OpenAPI_service_area_id_t *dst, OpenAPI_service_area_id_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_service_area_id_H_ */

