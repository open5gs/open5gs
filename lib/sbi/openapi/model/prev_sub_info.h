/*
 * prev_sub_info.h
 *
 * Information of the previous subscription.
 */

#ifndef _OpenAPI_prev_sub_info_H_
#define _OpenAPI_prev_sub_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "nwdaf_event.h"
#include "ue_analytics_context_descriptor.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_prev_sub_info_s OpenAPI_prev_sub_info_t;
typedef struct OpenAPI_prev_sub_info_s {
    char *producer_id;
    char *producer_set_id;
    char *subscription_id;
    OpenAPI_list_t *nf_ana_events;
    OpenAPI_list_t *ue_ana_events;
} OpenAPI_prev_sub_info_t;

OpenAPI_prev_sub_info_t *OpenAPI_prev_sub_info_create(
    char *producer_id,
    char *producer_set_id,
    char *subscription_id,
    OpenAPI_list_t *nf_ana_events,
    OpenAPI_list_t *ue_ana_events
);
void OpenAPI_prev_sub_info_free(OpenAPI_prev_sub_info_t *prev_sub_info);
OpenAPI_prev_sub_info_t *OpenAPI_prev_sub_info_parseFromJSON(cJSON *prev_sub_infoJSON);
cJSON *OpenAPI_prev_sub_info_convertToJSON(OpenAPI_prev_sub_info_t *prev_sub_info);
OpenAPI_prev_sub_info_t *OpenAPI_prev_sub_info_copy(OpenAPI_prev_sub_info_t *dst, OpenAPI_prev_sub_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_prev_sub_info_H_ */

