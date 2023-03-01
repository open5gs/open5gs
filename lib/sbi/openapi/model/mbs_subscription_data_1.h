/*
 * mbs_subscription_data_1.h
 *
 * Contains the 5MBS Subscription Data.
 */

#ifndef _OpenAPI_mbs_subscription_data_1_H_
#define _OpenAPI_mbs_subscription_data_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "mbs_session_id_1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_mbs_subscription_data_1_s OpenAPI_mbs_subscription_data_1_t;
typedef struct OpenAPI_mbs_subscription_data_1_s {
    bool is_mbs_allowed;
    int mbs_allowed;
    OpenAPI_list_t *mbs_session_id_list;
} OpenAPI_mbs_subscription_data_1_t;

OpenAPI_mbs_subscription_data_1_t *OpenAPI_mbs_subscription_data_1_create(
    bool is_mbs_allowed,
    int mbs_allowed,
    OpenAPI_list_t *mbs_session_id_list
);
void OpenAPI_mbs_subscription_data_1_free(OpenAPI_mbs_subscription_data_1_t *mbs_subscription_data_1);
OpenAPI_mbs_subscription_data_1_t *OpenAPI_mbs_subscription_data_1_parseFromJSON(cJSON *mbs_subscription_data_1JSON);
cJSON *OpenAPI_mbs_subscription_data_1_convertToJSON(OpenAPI_mbs_subscription_data_1_t *mbs_subscription_data_1);
OpenAPI_mbs_subscription_data_1_t *OpenAPI_mbs_subscription_data_1_copy(OpenAPI_mbs_subscription_data_1_t *dst, OpenAPI_mbs_subscription_data_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mbs_subscription_data_1_H_ */

