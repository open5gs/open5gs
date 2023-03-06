/*
 * extended_sm_subs_data.h
 *
 * Contains identifiers of shared Session Management Subscription Data and optionally individual Session Management Subscription Data.
 */

#ifndef _OpenAPI_extended_sm_subs_data_H_
#define _OpenAPI_extended_sm_subs_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "session_management_subscription_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_extended_sm_subs_data_s OpenAPI_extended_sm_subs_data_t;
typedef struct OpenAPI_extended_sm_subs_data_s {
    OpenAPI_list_t *shared_sm_subs_data_ids;
    OpenAPI_list_t *individual_sm_subs_data;
} OpenAPI_extended_sm_subs_data_t;

OpenAPI_extended_sm_subs_data_t *OpenAPI_extended_sm_subs_data_create(
    OpenAPI_list_t *shared_sm_subs_data_ids,
    OpenAPI_list_t *individual_sm_subs_data
);
void OpenAPI_extended_sm_subs_data_free(OpenAPI_extended_sm_subs_data_t *extended_sm_subs_data);
OpenAPI_extended_sm_subs_data_t *OpenAPI_extended_sm_subs_data_parseFromJSON(cJSON *extended_sm_subs_dataJSON);
cJSON *OpenAPI_extended_sm_subs_data_convertToJSON(OpenAPI_extended_sm_subs_data_t *extended_sm_subs_data);
OpenAPI_extended_sm_subs_data_t *OpenAPI_extended_sm_subs_data_copy(OpenAPI_extended_sm_subs_data_t *dst, OpenAPI_extended_sm_subs_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_extended_sm_subs_data_H_ */

