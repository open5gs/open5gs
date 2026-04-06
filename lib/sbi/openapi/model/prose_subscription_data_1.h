/*
 * prose_subscription_data_1.h
 *
 * Contains the ProSe Subscription Data.
 */

#ifndef _OpenAPI_prose_subscription_data_1_H_
#define _OpenAPI_prose_subscription_data_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_prose_subscription_data_1_s OpenAPI_prose_subscription_data_1_t;
#include "pro_se_allowed_plmn_1.h"
#include "prose_service_auth_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_prose_subscription_data_1_s {
    struct OpenAPI_prose_service_auth_1_s *prose_service_auth;
    char *nr_ue_pc5_ambr;
    OpenAPI_list_t *prose_allowed_plmn;
};

OpenAPI_prose_subscription_data_1_t *OpenAPI_prose_subscription_data_1_create(
    OpenAPI_prose_service_auth_1_t *prose_service_auth,
    char *nr_ue_pc5_ambr,
    OpenAPI_list_t *prose_allowed_plmn
);
void OpenAPI_prose_subscription_data_1_free(OpenAPI_prose_subscription_data_1_t *prose_subscription_data_1);
OpenAPI_prose_subscription_data_1_t *OpenAPI_prose_subscription_data_1_parseFromJSON(cJSON *prose_subscription_data_1JSON);
cJSON *OpenAPI_prose_subscription_data_1_convertToJSON(OpenAPI_prose_subscription_data_1_t *prose_subscription_data_1);
OpenAPI_prose_subscription_data_1_t *OpenAPI_prose_subscription_data_1_copy(OpenAPI_prose_subscription_data_1_t *dst, OpenAPI_prose_subscription_data_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_prose_subscription_data_1_H_ */

