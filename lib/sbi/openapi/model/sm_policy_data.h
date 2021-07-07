/*
 * sm_policy_data.h
 *
 * Contains the SM policy data for a given subscriber.
 */

#ifndef _OpenAPI_sm_policy_data_H_
#define _OpenAPI_sm_policy_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "sm_policy_snssai_data.h"
#include "usage_mon_data.h"
#include "usage_mon_data_limit.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sm_policy_data_s OpenAPI_sm_policy_data_t;
typedef struct OpenAPI_sm_policy_data_s {
    OpenAPI_list_t* sm_policy_snssai_data;
    OpenAPI_list_t* um_data_limits;
    OpenAPI_list_t* um_data;
    char *supp_feat;
} OpenAPI_sm_policy_data_t;

OpenAPI_sm_policy_data_t *OpenAPI_sm_policy_data_create(
    OpenAPI_list_t* sm_policy_snssai_data,
    OpenAPI_list_t* um_data_limits,
    OpenAPI_list_t* um_data,
    char *supp_feat
);
void OpenAPI_sm_policy_data_free(OpenAPI_sm_policy_data_t *sm_policy_data);
OpenAPI_sm_policy_data_t *OpenAPI_sm_policy_data_parseFromJSON(cJSON *sm_policy_dataJSON);
cJSON *OpenAPI_sm_policy_data_convertToJSON(OpenAPI_sm_policy_data_t *sm_policy_data);
OpenAPI_sm_policy_data_t *OpenAPI_sm_policy_data_copy(OpenAPI_sm_policy_data_t *dst, OpenAPI_sm_policy_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sm_policy_data_H_ */

