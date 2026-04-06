/*
 * sm_subs_data_1.h
 *
 * Session Management Subscription Data
 */

#ifndef _OpenAPI_sm_subs_data_1_H_
#define _OpenAPI_sm_subs_data_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_sm_subs_data_1_s OpenAPI_sm_subs_data_1_t;
#include "extended_sm_subs_data.h"
#include "session_management_subscription_data_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_sm_subs_data_1_s {
    OpenAPI_list_t *session_management_subscription_data_list;
    struct OpenAPI_extended_sm_subs_data_s *extended_sm_subs_data;
};

OpenAPI_sm_subs_data_1_t *OpenAPI_sm_subs_data_1_create(
    OpenAPI_list_t *session_management_subscription_data_list,
    OpenAPI_extended_sm_subs_data_t *extended_sm_subs_data
);
void OpenAPI_sm_subs_data_1_free(OpenAPI_sm_subs_data_1_t *sm_subs_data_1);
OpenAPI_sm_subs_data_1_t *OpenAPI_sm_subs_data_1_parseFromJSON(cJSON *sm_subs_data_1JSON);
cJSON *OpenAPI_sm_subs_data_1_convertToJSON(OpenAPI_sm_subs_data_1_t *sm_subs_data_1);
OpenAPI_sm_subs_data_1_t *OpenAPI_sm_subs_data_1_copy(OpenAPI_sm_subs_data_1_t *dst, OpenAPI_sm_subs_data_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sm_subs_data_1_H_ */

