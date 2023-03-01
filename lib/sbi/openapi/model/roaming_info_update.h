/*
 * roaming_info_update.h
 *
 * Contains the Roaming Information Update
 */

#ifndef _OpenAPI_roaming_info_update_H_
#define _OpenAPI_roaming_info_update_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "plmn_id.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_roaming_info_update_s OpenAPI_roaming_info_update_t;
typedef struct OpenAPI_roaming_info_update_s {
    bool is_roaming;
    int roaming;
    struct OpenAPI_plmn_id_s *serving_plmn;
} OpenAPI_roaming_info_update_t;

OpenAPI_roaming_info_update_t *OpenAPI_roaming_info_update_create(
    bool is_roaming,
    int roaming,
    OpenAPI_plmn_id_t *serving_plmn
);
void OpenAPI_roaming_info_update_free(OpenAPI_roaming_info_update_t *roaming_info_update);
OpenAPI_roaming_info_update_t *OpenAPI_roaming_info_update_parseFromJSON(cJSON *roaming_info_updateJSON);
cJSON *OpenAPI_roaming_info_update_convertToJSON(OpenAPI_roaming_info_update_t *roaming_info_update);
OpenAPI_roaming_info_update_t *OpenAPI_roaming_info_update_copy(OpenAPI_roaming_info_update_t *dst, OpenAPI_roaming_info_update_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_roaming_info_update_H_ */

