/*
 * aiot_area_id.h
 *
 * AIoT Area ID.
 */

#ifndef _OpenAPI_aiot_area_id_H_
#define _OpenAPI_aiot_area_id_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_aiot_area_id_s OpenAPI_aiot_area_id_t;
#include "plmn_id.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_aiot_area_id_s {
    struct OpenAPI_plmn_id_s *plmn_id;
    char *nid;
    char *aiot_area_code;
};

OpenAPI_aiot_area_id_t *OpenAPI_aiot_area_id_create(
    OpenAPI_plmn_id_t *plmn_id,
    char *nid,
    char *aiot_area_code
);
void OpenAPI_aiot_area_id_free(OpenAPI_aiot_area_id_t *aiot_area_id);
OpenAPI_aiot_area_id_t *OpenAPI_aiot_area_id_parseFromJSON(cJSON *aiot_area_idJSON);
cJSON *OpenAPI_aiot_area_id_convertToJSON(OpenAPI_aiot_area_id_t *aiot_area_id);
OpenAPI_aiot_area_id_t *OpenAPI_aiot_area_id_copy(OpenAPI_aiot_area_id_t *dst, OpenAPI_aiot_area_id_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_aiot_area_id_H_ */

