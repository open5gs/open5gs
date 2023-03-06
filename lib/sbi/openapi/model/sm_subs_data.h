/*
 * sm_subs_data.h
 *
 * 
 */

#ifndef _OpenAPI_sm_subs_data_H_
#define _OpenAPI_sm_subs_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "extended_sm_subs_data.h"
#include "session_management_subscription_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sm_subs_data_s OpenAPI_sm_subs_data_t;
typedef struct OpenAPI_sm_subs_data_s {
    OpenAPI_list_t *shared_sm_subs_data_ids;
    OpenAPI_list_t *individual_sm_subs_data;
} OpenAPI_sm_subs_data_t;

OpenAPI_sm_subs_data_t *OpenAPI_sm_subs_data_create(
    OpenAPI_list_t *shared_sm_subs_data_ids,
    OpenAPI_list_t *individual_sm_subs_data
);
void OpenAPI_sm_subs_data_free(OpenAPI_sm_subs_data_t *sm_subs_data);
OpenAPI_sm_subs_data_t *OpenAPI_sm_subs_data_parseFromJSON(cJSON *sm_subs_dataJSON);
cJSON *OpenAPI_sm_subs_data_convertToJSON(OpenAPI_sm_subs_data_t *sm_subs_data);
OpenAPI_sm_subs_data_t *OpenAPI_sm_subs_data_copy(OpenAPI_sm_subs_data_t *dst, OpenAPI_sm_subs_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sm_subs_data_H_ */

