/*
 * mbs_subscription_data.h
 *
 * Contains the 5MBS Subscription Data.
 */

#ifndef _OpenAPI_mbs_subscription_data_H_
#define _OpenAPI_mbs_subscription_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "mbs_session_id.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_mbs_subscription_data_s OpenAPI_mbs_subscription_data_t;
typedef struct OpenAPI_mbs_subscription_data_s {
    bool is_mbs_allowed;
    int mbs_allowed;
    OpenAPI_list_t *mbs_session_id_list;
} OpenAPI_mbs_subscription_data_t;

OpenAPI_mbs_subscription_data_t *OpenAPI_mbs_subscription_data_create(
    bool is_mbs_allowed,
    int mbs_allowed,
    OpenAPI_list_t *mbs_session_id_list
);
void OpenAPI_mbs_subscription_data_free(OpenAPI_mbs_subscription_data_t *mbs_subscription_data);
OpenAPI_mbs_subscription_data_t *OpenAPI_mbs_subscription_data_parseFromJSON(cJSON *mbs_subscription_dataJSON);
cJSON *OpenAPI_mbs_subscription_data_convertToJSON(OpenAPI_mbs_subscription_data_t *mbs_subscription_data);
OpenAPI_mbs_subscription_data_t *OpenAPI_mbs_subscription_data_copy(OpenAPI_mbs_subscription_data_t *dst, OpenAPI_mbs_subscription_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mbs_subscription_data_H_ */

