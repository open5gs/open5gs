/*
 * v2x_subscription_data.h
 *
 * 
 */

#ifndef _OpenAPI_v2x_subscription_data_H_
#define _OpenAPI_v2x_subscription_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "lte_v2x_auth.h"
#include "nr_v2x_auth.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_v2x_subscription_data_s OpenAPI_v2x_subscription_data_t;
typedef struct OpenAPI_v2x_subscription_data_s {
    struct OpenAPI_nr_v2x_auth_s *nr_v2x_services_auth;
    struct OpenAPI_lte_v2x_auth_s *lte_v2x_services_auth;
    char *nr_ue_pc5_ambr;
    char *lte_pc5_ambr;
} OpenAPI_v2x_subscription_data_t;

OpenAPI_v2x_subscription_data_t *OpenAPI_v2x_subscription_data_create(
    OpenAPI_nr_v2x_auth_t *nr_v2x_services_auth,
    OpenAPI_lte_v2x_auth_t *lte_v2x_services_auth,
    char *nr_ue_pc5_ambr,
    char *lte_pc5_ambr
);
void OpenAPI_v2x_subscription_data_free(OpenAPI_v2x_subscription_data_t *v2x_subscription_data);
OpenAPI_v2x_subscription_data_t *OpenAPI_v2x_subscription_data_parseFromJSON(cJSON *v2x_subscription_dataJSON);
cJSON *OpenAPI_v2x_subscription_data_convertToJSON(OpenAPI_v2x_subscription_data_t *v2x_subscription_data);
OpenAPI_v2x_subscription_data_t *OpenAPI_v2x_subscription_data_copy(OpenAPI_v2x_subscription_data_t *dst, OpenAPI_v2x_subscription_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_v2x_subscription_data_H_ */

