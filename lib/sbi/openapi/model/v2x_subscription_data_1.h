/*
 * v2x_subscription_data_1.h
 *
 * 
 */

#ifndef _OpenAPI_v2x_subscription_data_1_H_
#define _OpenAPI_v2x_subscription_data_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "lte_v2x_auth_1.h"
#include "nr_v2x_auth_1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_v2x_subscription_data_1_s OpenAPI_v2x_subscription_data_1_t;
typedef struct OpenAPI_v2x_subscription_data_1_s {
    struct OpenAPI_nr_v2x_auth_1_s *nr_v2x_services_auth;
    struct OpenAPI_lte_v2x_auth_1_s *lte_v2x_services_auth;
    char *nr_ue_pc5_ambr;
    char *lte_pc5_ambr;
} OpenAPI_v2x_subscription_data_1_t;

OpenAPI_v2x_subscription_data_1_t *OpenAPI_v2x_subscription_data_1_create(
    OpenAPI_nr_v2x_auth_1_t *nr_v2x_services_auth,
    OpenAPI_lte_v2x_auth_1_t *lte_v2x_services_auth,
    char *nr_ue_pc5_ambr,
    char *lte_pc5_ambr
);
void OpenAPI_v2x_subscription_data_1_free(OpenAPI_v2x_subscription_data_1_t *v2x_subscription_data_1);
OpenAPI_v2x_subscription_data_1_t *OpenAPI_v2x_subscription_data_1_parseFromJSON(cJSON *v2x_subscription_data_1JSON);
cJSON *OpenAPI_v2x_subscription_data_1_convertToJSON(OpenAPI_v2x_subscription_data_1_t *v2x_subscription_data_1);
OpenAPI_v2x_subscription_data_1_t *OpenAPI_v2x_subscription_data_1_copy(OpenAPI_v2x_subscription_data_1_t *dst, OpenAPI_v2x_subscription_data_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_v2x_subscription_data_1_H_ */

