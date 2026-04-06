/*
 * roaming_info.h
 *
 * Information related to roaming analytics.
 */

#ifndef _OpenAPI_roaming_info_H_
#define _OpenAPI_roaming_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_roaming_info_s OpenAPI_roaming_info_t;
#include "geographical_area.h"
#include "plmn_id_nid.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_roaming_info_s {
    struct OpenAPI_plmn_id_nid_s *plmn_id;
    OpenAPI_list_t *aois;
    OpenAPI_list_t *serving_nf_ids;
    OpenAPI_list_t *serving_nf_set_ids;
};

OpenAPI_roaming_info_t *OpenAPI_roaming_info_create(
    OpenAPI_plmn_id_nid_t *plmn_id,
    OpenAPI_list_t *aois,
    OpenAPI_list_t *serving_nf_ids,
    OpenAPI_list_t *serving_nf_set_ids
);
void OpenAPI_roaming_info_free(OpenAPI_roaming_info_t *roaming_info);
OpenAPI_roaming_info_t *OpenAPI_roaming_info_parseFromJSON(cJSON *roaming_infoJSON);
cJSON *OpenAPI_roaming_info_convertToJSON(OpenAPI_roaming_info_t *roaming_info);
OpenAPI_roaming_info_t *OpenAPI_roaming_info_copy(OpenAPI_roaming_info_t *dst, OpenAPI_roaming_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_roaming_info_H_ */

