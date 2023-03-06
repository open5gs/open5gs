/*
 * prose_subscription_data.h
 *
 * Contains the ProSe Subscription Data.
 */

#ifndef _OpenAPI_prose_subscription_data_H_
#define _OpenAPI_prose_subscription_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "pro_se_allowed_plmn.h"
#include "prose_service_auth.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_prose_subscription_data_s OpenAPI_prose_subscription_data_t;
typedef struct OpenAPI_prose_subscription_data_s {
    struct OpenAPI_prose_service_auth_s *prose_service_auth;
    char *nr_ue_pc5_ambr;
    OpenAPI_list_t *prose_allowed_plmn;
} OpenAPI_prose_subscription_data_t;

OpenAPI_prose_subscription_data_t *OpenAPI_prose_subscription_data_create(
    OpenAPI_prose_service_auth_t *prose_service_auth,
    char *nr_ue_pc5_ambr,
    OpenAPI_list_t *prose_allowed_plmn
);
void OpenAPI_prose_subscription_data_free(OpenAPI_prose_subscription_data_t *prose_subscription_data);
OpenAPI_prose_subscription_data_t *OpenAPI_prose_subscription_data_parseFromJSON(cJSON *prose_subscription_dataJSON);
cJSON *OpenAPI_prose_subscription_data_convertToJSON(OpenAPI_prose_subscription_data_t *prose_subscription_data);
OpenAPI_prose_subscription_data_t *OpenAPI_prose_subscription_data_copy(OpenAPI_prose_subscription_data_t *dst, OpenAPI_prose_subscription_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_prose_subscription_data_H_ */

