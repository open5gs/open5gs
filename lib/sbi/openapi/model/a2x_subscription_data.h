/*
 * a2x_subscription_data.h
 *
 * Contains A2X Subscription Data
 */

#ifndef _OpenAPI_a2x_subscription_data_H_
#define _OpenAPI_a2x_subscription_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_a2x_subscription_data_s OpenAPI_a2x_subscription_data_t;
#include "lte_a2x_auth.h"
#include "nr_a2x_auth.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_a2x_subscription_data_s {
    struct OpenAPI_nr_a2x_auth_s *nr_a2x_services_auth;
    struct OpenAPI_lte_a2x_auth_s *lte_a2x_services_auth;
    char *nr_ue_pc5_ambr;
    char *lte_pc5_ambr;
};

OpenAPI_a2x_subscription_data_t *OpenAPI_a2x_subscription_data_create(
    OpenAPI_nr_a2x_auth_t *nr_a2x_services_auth,
    OpenAPI_lte_a2x_auth_t *lte_a2x_services_auth,
    char *nr_ue_pc5_ambr,
    char *lte_pc5_ambr
);
void OpenAPI_a2x_subscription_data_free(OpenAPI_a2x_subscription_data_t *a2x_subscription_data);
OpenAPI_a2x_subscription_data_t *OpenAPI_a2x_subscription_data_parseFromJSON(cJSON *a2x_subscription_dataJSON);
cJSON *OpenAPI_a2x_subscription_data_convertToJSON(OpenAPI_a2x_subscription_data_t *a2x_subscription_data);
OpenAPI_a2x_subscription_data_t *OpenAPI_a2x_subscription_data_copy(OpenAPI_a2x_subscription_data_t *dst, OpenAPI_a2x_subscription_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_a2x_subscription_data_H_ */

