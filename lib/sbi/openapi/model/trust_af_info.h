/*
 * trust_af_info.h
 *
 * Information of a trusted AF Instance
 */

#ifndef _OpenAPI_trust_af_info_H_
#define _OpenAPI_trust_af_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "af_event.h"
#include "snssai_info_item.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_trust_af_info_s OpenAPI_trust_af_info_t;
typedef struct OpenAPI_trust_af_info_s {
    OpenAPI_list_t *s_nssai_info_list;
    OpenAPI_list_t *af_events;
    OpenAPI_list_t *app_ids;
    OpenAPI_list_t *internal_group_id;
    bool is_mapping_ind;
    int mapping_ind;
} OpenAPI_trust_af_info_t;

OpenAPI_trust_af_info_t *OpenAPI_trust_af_info_create(
    OpenAPI_list_t *s_nssai_info_list,
    OpenAPI_list_t *af_events,
    OpenAPI_list_t *app_ids,
    OpenAPI_list_t *internal_group_id,
    bool is_mapping_ind,
    int mapping_ind
);
void OpenAPI_trust_af_info_free(OpenAPI_trust_af_info_t *trust_af_info);
OpenAPI_trust_af_info_t *OpenAPI_trust_af_info_parseFromJSON(cJSON *trust_af_infoJSON);
cJSON *OpenAPI_trust_af_info_convertToJSON(OpenAPI_trust_af_info_t *trust_af_info);
OpenAPI_trust_af_info_t *OpenAPI_trust_af_info_copy(OpenAPI_trust_af_info_t *dst, OpenAPI_trust_af_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_trust_af_info_H_ */

