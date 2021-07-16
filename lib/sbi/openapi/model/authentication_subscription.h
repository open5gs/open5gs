/*
 * authentication_subscription.h
 *
 * 
 */

#ifndef _OpenAPI_authentication_subscription_H_
#define _OpenAPI_authentication_subscription_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "auth_method.h"
#include "sequence_number.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_authentication_subscription_s OpenAPI_authentication_subscription_t;
typedef struct OpenAPI_authentication_subscription_s {
    OpenAPI_auth_method_e authentication_method;
    char *enc_permanent_key;
    char *protection_parameter_id;
    struct OpenAPI_sequence_number_s *sequence_number;
    char *authentication_management_field;
    char *algorithm_id;
    char *enc_opc_key;
    char *enc_topc_key;
    bool is_vector_generation_in_hss;
    int vector_generation_in_hss;
    OpenAPI_auth_method_e n5gc_auth_method;
    bool is_rg_authentication_ind;
    int rg_authentication_ind;
    char *supi;
} OpenAPI_authentication_subscription_t;

OpenAPI_authentication_subscription_t *OpenAPI_authentication_subscription_create(
    OpenAPI_auth_method_e authentication_method,
    char *enc_permanent_key,
    char *protection_parameter_id,
    OpenAPI_sequence_number_t *sequence_number,
    char *authentication_management_field,
    char *algorithm_id,
    char *enc_opc_key,
    char *enc_topc_key,
    bool is_vector_generation_in_hss,
    int vector_generation_in_hss,
    OpenAPI_auth_method_e n5gc_auth_method,
    bool is_rg_authentication_ind,
    int rg_authentication_ind,
    char *supi
);
void OpenAPI_authentication_subscription_free(OpenAPI_authentication_subscription_t *authentication_subscription);
OpenAPI_authentication_subscription_t *OpenAPI_authentication_subscription_parseFromJSON(cJSON *authentication_subscriptionJSON);
cJSON *OpenAPI_authentication_subscription_convertToJSON(OpenAPI_authentication_subscription_t *authentication_subscription);
OpenAPI_authentication_subscription_t *OpenAPI_authentication_subscription_copy(OpenAPI_authentication_subscription_t *dst, OpenAPI_authentication_subscription_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_authentication_subscription_H_ */

