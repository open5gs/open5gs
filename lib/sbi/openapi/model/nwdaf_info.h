/*
 * nwdaf_info.h
 *
 * Information of a NWDAF NF Instance
 */

#ifndef _OpenAPI_nwdaf_info_H_
#define _OpenAPI_nwdaf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_nwdaf_info_s OpenAPI_nwdaf_info_t;
#include "event_id.h"
#include "ml_analytics_info.h"
#include "nf_type.h"
#include "nwdaf_capability.h"
#include "nwdaf_event.h"
#include "positioning_case.h"
#include "tai.h"
#include "tai_range.h"
#include "tai_weight_info.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_nwdaf_info_s {
    OpenAPI_list_t *event_ids;
    OpenAPI_list_t *nwdaf_events;
    OpenAPI_list_t *tai_list;
    OpenAPI_list_t *tai_range_list;
    OpenAPI_list_t *tai_weight_list;
    struct OpenAPI_nwdaf_capability_s *nwdaf_capability;
    bool is_analytics_delay;
    int analytics_delay;
    OpenAPI_list_t *serving_nf_set_id_list;
    OpenAPI_list_t *serving_nf_type_list;
    OpenAPI_list_t *ml_analytics_list;
    OpenAPI_list_t *pos_cases;
};

OpenAPI_nwdaf_info_t *OpenAPI_nwdaf_info_create(
    OpenAPI_list_t *event_ids,
    OpenAPI_list_t *nwdaf_events,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list,
    OpenAPI_list_t *tai_weight_list,
    OpenAPI_nwdaf_capability_t *nwdaf_capability,
    bool is_analytics_delay,
    int analytics_delay,
    OpenAPI_list_t *serving_nf_set_id_list,
    OpenAPI_list_t *serving_nf_type_list,
    OpenAPI_list_t *ml_analytics_list,
    OpenAPI_list_t *pos_cases
);
void OpenAPI_nwdaf_info_free(OpenAPI_nwdaf_info_t *nwdaf_info);
OpenAPI_nwdaf_info_t *OpenAPI_nwdaf_info_parseFromJSON(cJSON *nwdaf_infoJSON);
cJSON *OpenAPI_nwdaf_info_convertToJSON(OpenAPI_nwdaf_info_t *nwdaf_info);
OpenAPI_nwdaf_info_t *OpenAPI_nwdaf_info_copy(OpenAPI_nwdaf_info_t *dst, OpenAPI_nwdaf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nwdaf_info_H_ */

