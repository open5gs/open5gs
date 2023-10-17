/*
 * routing_area_id.h
 *
 * Contains a Routing Area Identification as defined in 3GPP TS 23.003, clause 4.2.
 */

#ifndef _OpenAPI_routing_area_id_H_
#define _OpenAPI_routing_area_id_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "plmn_id.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_routing_area_id_s OpenAPI_routing_area_id_t;
typedef struct OpenAPI_routing_area_id_s {
    struct OpenAPI_plmn_id_s *plmn_id;
    char *lac;
    char *rac;
} OpenAPI_routing_area_id_t;

OpenAPI_routing_area_id_t *OpenAPI_routing_area_id_create(
    OpenAPI_plmn_id_t *plmn_id,
    char *lac,
    char *rac
);
void OpenAPI_routing_area_id_free(OpenAPI_routing_area_id_t *routing_area_id);
OpenAPI_routing_area_id_t *OpenAPI_routing_area_id_parseFromJSON(cJSON *routing_area_idJSON);
cJSON *OpenAPI_routing_area_id_convertToJSON(OpenAPI_routing_area_id_t *routing_area_id);
OpenAPI_routing_area_id_t *OpenAPI_routing_area_id_copy(OpenAPI_routing_area_id_t *dst, OpenAPI_routing_area_id_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_routing_area_id_H_ */

