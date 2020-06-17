/*
 * sor_update_info.h
 *
 *
 */

#ifndef _OpenAPI_sor_update_info_H_
#define _OpenAPI_sor_update_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "plmn_id.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sor_update_info_s OpenAPI_sor_update_info_t;
typedef struct OpenAPI_sor_update_info_s {
    struct OpenAPI_plmn_id_s *vplmn_id;
} OpenAPI_sor_update_info_t;

OpenAPI_sor_update_info_t *OpenAPI_sor_update_info_create(
    OpenAPI_plmn_id_t *vplmn_id
    );
void OpenAPI_sor_update_info_free(OpenAPI_sor_update_info_t *sor_update_info);
OpenAPI_sor_update_info_t *OpenAPI_sor_update_info_parseFromJSON(cJSON *sor_update_infoJSON);
cJSON *OpenAPI_sor_update_info_convertToJSON(OpenAPI_sor_update_info_t *sor_update_info);
OpenAPI_sor_update_info_t *OpenAPI_sor_update_info_copy(OpenAPI_sor_update_info_t *dst, OpenAPI_sor_update_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sor_update_info_H_ */

