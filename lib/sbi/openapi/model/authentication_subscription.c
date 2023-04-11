
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "authentication_subscription.h"

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
    char *hss_group_id,
    OpenAPI_auth_method_e n5gc_auth_method,
    bool is_rg_authentication_ind,
    int rg_authentication_ind,
    char *supi,
    bool is_akma_allowed,
    int akma_allowed,
    char *routing_id
)
{
    OpenAPI_authentication_subscription_t *authentication_subscription_local_var = ogs_malloc(sizeof(OpenAPI_authentication_subscription_t));
    ogs_assert(authentication_subscription_local_var);

    authentication_subscription_local_var->authentication_method = authentication_method;
    authentication_subscription_local_var->enc_permanent_key = enc_permanent_key;
    authentication_subscription_local_var->protection_parameter_id = protection_parameter_id;
    authentication_subscription_local_var->sequence_number = sequence_number;
    authentication_subscription_local_var->authentication_management_field = authentication_management_field;
    authentication_subscription_local_var->algorithm_id = algorithm_id;
    authentication_subscription_local_var->enc_opc_key = enc_opc_key;
    authentication_subscription_local_var->enc_topc_key = enc_topc_key;
    authentication_subscription_local_var->is_vector_generation_in_hss = is_vector_generation_in_hss;
    authentication_subscription_local_var->vector_generation_in_hss = vector_generation_in_hss;
    authentication_subscription_local_var->hss_group_id = hss_group_id;
    authentication_subscription_local_var->n5gc_auth_method = n5gc_auth_method;
    authentication_subscription_local_var->is_rg_authentication_ind = is_rg_authentication_ind;
    authentication_subscription_local_var->rg_authentication_ind = rg_authentication_ind;
    authentication_subscription_local_var->supi = supi;
    authentication_subscription_local_var->is_akma_allowed = is_akma_allowed;
    authentication_subscription_local_var->akma_allowed = akma_allowed;
    authentication_subscription_local_var->routing_id = routing_id;

    return authentication_subscription_local_var;
}

void OpenAPI_authentication_subscription_free(OpenAPI_authentication_subscription_t *authentication_subscription)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == authentication_subscription) {
        return;
    }
    if (authentication_subscription->enc_permanent_key) {
        ogs_free(authentication_subscription->enc_permanent_key);
        authentication_subscription->enc_permanent_key = NULL;
    }
    if (authentication_subscription->protection_parameter_id) {
        ogs_free(authentication_subscription->protection_parameter_id);
        authentication_subscription->protection_parameter_id = NULL;
    }
    if (authentication_subscription->sequence_number) {
        OpenAPI_sequence_number_free(authentication_subscription->sequence_number);
        authentication_subscription->sequence_number = NULL;
    }
    if (authentication_subscription->authentication_management_field) {
        ogs_free(authentication_subscription->authentication_management_field);
        authentication_subscription->authentication_management_field = NULL;
    }
    if (authentication_subscription->algorithm_id) {
        ogs_free(authentication_subscription->algorithm_id);
        authentication_subscription->algorithm_id = NULL;
    }
    if (authentication_subscription->enc_opc_key) {
        ogs_free(authentication_subscription->enc_opc_key);
        authentication_subscription->enc_opc_key = NULL;
    }
    if (authentication_subscription->enc_topc_key) {
        ogs_free(authentication_subscription->enc_topc_key);
        authentication_subscription->enc_topc_key = NULL;
    }
    if (authentication_subscription->hss_group_id) {
        ogs_free(authentication_subscription->hss_group_id);
        authentication_subscription->hss_group_id = NULL;
    }
    if (authentication_subscription->supi) {
        ogs_free(authentication_subscription->supi);
        authentication_subscription->supi = NULL;
    }
    if (authentication_subscription->routing_id) {
        ogs_free(authentication_subscription->routing_id);
        authentication_subscription->routing_id = NULL;
    }
    ogs_free(authentication_subscription);
}

cJSON *OpenAPI_authentication_subscription_convertToJSON(OpenAPI_authentication_subscription_t *authentication_subscription)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (authentication_subscription == NULL) {
        ogs_error("OpenAPI_authentication_subscription_convertToJSON() failed [AuthenticationSubscription]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (authentication_subscription->authentication_method == OpenAPI_auth_method_NULL) {
        ogs_error("OpenAPI_authentication_subscription_convertToJSON() failed [authentication_method]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "authenticationMethod", OpenAPI_auth_method_ToString(authentication_subscription->authentication_method)) == NULL) {
        ogs_error("OpenAPI_authentication_subscription_convertToJSON() failed [authentication_method]");
        goto end;
    }

    if (authentication_subscription->enc_permanent_key) {
    if (cJSON_AddStringToObject(item, "encPermanentKey", authentication_subscription->enc_permanent_key) == NULL) {
        ogs_error("OpenAPI_authentication_subscription_convertToJSON() failed [enc_permanent_key]");
        goto end;
    }
    }

    if (authentication_subscription->protection_parameter_id) {
    if (cJSON_AddStringToObject(item, "protectionParameterId", authentication_subscription->protection_parameter_id) == NULL) {
        ogs_error("OpenAPI_authentication_subscription_convertToJSON() failed [protection_parameter_id]");
        goto end;
    }
    }

    if (authentication_subscription->sequence_number) {
    cJSON *sequence_number_local_JSON = OpenAPI_sequence_number_convertToJSON(authentication_subscription->sequence_number);
    if (sequence_number_local_JSON == NULL) {
        ogs_error("OpenAPI_authentication_subscription_convertToJSON() failed [sequence_number]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sequenceNumber", sequence_number_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_authentication_subscription_convertToJSON() failed [sequence_number]");
        goto end;
    }
    }

    if (authentication_subscription->authentication_management_field) {
    if (cJSON_AddStringToObject(item, "authenticationManagementField", authentication_subscription->authentication_management_field) == NULL) {
        ogs_error("OpenAPI_authentication_subscription_convertToJSON() failed [authentication_management_field]");
        goto end;
    }
    }

    if (authentication_subscription->algorithm_id) {
    if (cJSON_AddStringToObject(item, "algorithmId", authentication_subscription->algorithm_id) == NULL) {
        ogs_error("OpenAPI_authentication_subscription_convertToJSON() failed [algorithm_id]");
        goto end;
    }
    }

    if (authentication_subscription->enc_opc_key) {
    if (cJSON_AddStringToObject(item, "encOpcKey", authentication_subscription->enc_opc_key) == NULL) {
        ogs_error("OpenAPI_authentication_subscription_convertToJSON() failed [enc_opc_key]");
        goto end;
    }
    }

    if (authentication_subscription->enc_topc_key) {
    if (cJSON_AddStringToObject(item, "encTopcKey", authentication_subscription->enc_topc_key) == NULL) {
        ogs_error("OpenAPI_authentication_subscription_convertToJSON() failed [enc_topc_key]");
        goto end;
    }
    }

    if (authentication_subscription->is_vector_generation_in_hss) {
    if (cJSON_AddBoolToObject(item, "vectorGenerationInHss", authentication_subscription->vector_generation_in_hss) == NULL) {
        ogs_error("OpenAPI_authentication_subscription_convertToJSON() failed [vector_generation_in_hss]");
        goto end;
    }
    }

    if (authentication_subscription->hss_group_id) {
    if (cJSON_AddStringToObject(item, "hssGroupId", authentication_subscription->hss_group_id) == NULL) {
        ogs_error("OpenAPI_authentication_subscription_convertToJSON() failed [hss_group_id]");
        goto end;
    }
    }

    if (authentication_subscription->n5gc_auth_method != OpenAPI_auth_method_NULL) {
    if (cJSON_AddStringToObject(item, "n5gcAuthMethod", OpenAPI_auth_method_ToString(authentication_subscription->n5gc_auth_method)) == NULL) {
        ogs_error("OpenAPI_authentication_subscription_convertToJSON() failed [n5gc_auth_method]");
        goto end;
    }
    }

    if (authentication_subscription->is_rg_authentication_ind) {
    if (cJSON_AddBoolToObject(item, "rgAuthenticationInd", authentication_subscription->rg_authentication_ind) == NULL) {
        ogs_error("OpenAPI_authentication_subscription_convertToJSON() failed [rg_authentication_ind]");
        goto end;
    }
    }

    if (authentication_subscription->supi) {
    if (cJSON_AddStringToObject(item, "supi", authentication_subscription->supi) == NULL) {
        ogs_error("OpenAPI_authentication_subscription_convertToJSON() failed [supi]");
        goto end;
    }
    }

    if (authentication_subscription->is_akma_allowed) {
    if (cJSON_AddBoolToObject(item, "akmaAllowed", authentication_subscription->akma_allowed) == NULL) {
        ogs_error("OpenAPI_authentication_subscription_convertToJSON() failed [akma_allowed]");
        goto end;
    }
    }

    if (authentication_subscription->routing_id) {
    if (cJSON_AddStringToObject(item, "routingId", authentication_subscription->routing_id) == NULL) {
        ogs_error("OpenAPI_authentication_subscription_convertToJSON() failed [routing_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_authentication_subscription_t *OpenAPI_authentication_subscription_parseFromJSON(cJSON *authentication_subscriptionJSON)
{
    OpenAPI_authentication_subscription_t *authentication_subscription_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *authentication_method = NULL;
    OpenAPI_auth_method_e authentication_methodVariable = 0;
    cJSON *enc_permanent_key = NULL;
    cJSON *protection_parameter_id = NULL;
    cJSON *sequence_number = NULL;
    OpenAPI_sequence_number_t *sequence_number_local_nonprim = NULL;
    cJSON *authentication_management_field = NULL;
    cJSON *algorithm_id = NULL;
    cJSON *enc_opc_key = NULL;
    cJSON *enc_topc_key = NULL;
    cJSON *vector_generation_in_hss = NULL;
    cJSON *hss_group_id = NULL;
    cJSON *n5gc_auth_method = NULL;
    OpenAPI_auth_method_e n5gc_auth_methodVariable = 0;
    cJSON *rg_authentication_ind = NULL;
    cJSON *supi = NULL;
    cJSON *akma_allowed = NULL;
    cJSON *routing_id = NULL;
    authentication_method = cJSON_GetObjectItemCaseSensitive(authentication_subscriptionJSON, "authenticationMethod");
    if (!authentication_method) {
        ogs_error("OpenAPI_authentication_subscription_parseFromJSON() failed [authentication_method]");
        goto end;
    }
    if (!cJSON_IsString(authentication_method)) {
        ogs_error("OpenAPI_authentication_subscription_parseFromJSON() failed [authentication_method]");
        goto end;
    }
    authentication_methodVariable = OpenAPI_auth_method_FromString(authentication_method->valuestring);

    enc_permanent_key = cJSON_GetObjectItemCaseSensitive(authentication_subscriptionJSON, "encPermanentKey");
    if (enc_permanent_key) {
    if (!cJSON_IsString(enc_permanent_key) && !cJSON_IsNull(enc_permanent_key)) {
        ogs_error("OpenAPI_authentication_subscription_parseFromJSON() failed [enc_permanent_key]");
        goto end;
    }
    }

    protection_parameter_id = cJSON_GetObjectItemCaseSensitive(authentication_subscriptionJSON, "protectionParameterId");
    if (protection_parameter_id) {
    if (!cJSON_IsString(protection_parameter_id) && !cJSON_IsNull(protection_parameter_id)) {
        ogs_error("OpenAPI_authentication_subscription_parseFromJSON() failed [protection_parameter_id]");
        goto end;
    }
    }

    sequence_number = cJSON_GetObjectItemCaseSensitive(authentication_subscriptionJSON, "sequenceNumber");
    if (sequence_number) {
    sequence_number_local_nonprim = OpenAPI_sequence_number_parseFromJSON(sequence_number);
    if (!sequence_number_local_nonprim) {
        ogs_error("OpenAPI_sequence_number_parseFromJSON failed [sequence_number]");
        goto end;
    }
    }

    authentication_management_field = cJSON_GetObjectItemCaseSensitive(authentication_subscriptionJSON, "authenticationManagementField");
    if (authentication_management_field) {
    if (!cJSON_IsString(authentication_management_field) && !cJSON_IsNull(authentication_management_field)) {
        ogs_error("OpenAPI_authentication_subscription_parseFromJSON() failed [authentication_management_field]");
        goto end;
    }
    }

    algorithm_id = cJSON_GetObjectItemCaseSensitive(authentication_subscriptionJSON, "algorithmId");
    if (algorithm_id) {
    if (!cJSON_IsString(algorithm_id) && !cJSON_IsNull(algorithm_id)) {
        ogs_error("OpenAPI_authentication_subscription_parseFromJSON() failed [algorithm_id]");
        goto end;
    }
    }

    enc_opc_key = cJSON_GetObjectItemCaseSensitive(authentication_subscriptionJSON, "encOpcKey");
    if (enc_opc_key) {
    if (!cJSON_IsString(enc_opc_key) && !cJSON_IsNull(enc_opc_key)) {
        ogs_error("OpenAPI_authentication_subscription_parseFromJSON() failed [enc_opc_key]");
        goto end;
    }
    }

    enc_topc_key = cJSON_GetObjectItemCaseSensitive(authentication_subscriptionJSON, "encTopcKey");
    if (enc_topc_key) {
    if (!cJSON_IsString(enc_topc_key) && !cJSON_IsNull(enc_topc_key)) {
        ogs_error("OpenAPI_authentication_subscription_parseFromJSON() failed [enc_topc_key]");
        goto end;
    }
    }

    vector_generation_in_hss = cJSON_GetObjectItemCaseSensitive(authentication_subscriptionJSON, "vectorGenerationInHss");
    if (vector_generation_in_hss) {
    if (!cJSON_IsBool(vector_generation_in_hss)) {
        ogs_error("OpenAPI_authentication_subscription_parseFromJSON() failed [vector_generation_in_hss]");
        goto end;
    }
    }

    hss_group_id = cJSON_GetObjectItemCaseSensitive(authentication_subscriptionJSON, "hssGroupId");
    if (hss_group_id) {
    if (!cJSON_IsString(hss_group_id) && !cJSON_IsNull(hss_group_id)) {
        ogs_error("OpenAPI_authentication_subscription_parseFromJSON() failed [hss_group_id]");
        goto end;
    }
    }

    n5gc_auth_method = cJSON_GetObjectItemCaseSensitive(authentication_subscriptionJSON, "n5gcAuthMethod");
    if (n5gc_auth_method) {
    if (!cJSON_IsString(n5gc_auth_method)) {
        ogs_error("OpenAPI_authentication_subscription_parseFromJSON() failed [n5gc_auth_method]");
        goto end;
    }
    n5gc_auth_methodVariable = OpenAPI_auth_method_FromString(n5gc_auth_method->valuestring);
    }

    rg_authentication_ind = cJSON_GetObjectItemCaseSensitive(authentication_subscriptionJSON, "rgAuthenticationInd");
    if (rg_authentication_ind) {
    if (!cJSON_IsBool(rg_authentication_ind)) {
        ogs_error("OpenAPI_authentication_subscription_parseFromJSON() failed [rg_authentication_ind]");
        goto end;
    }
    }

    supi = cJSON_GetObjectItemCaseSensitive(authentication_subscriptionJSON, "supi");
    if (supi) {
    if (!cJSON_IsString(supi) && !cJSON_IsNull(supi)) {
        ogs_error("OpenAPI_authentication_subscription_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    akma_allowed = cJSON_GetObjectItemCaseSensitive(authentication_subscriptionJSON, "akmaAllowed");
    if (akma_allowed) {
    if (!cJSON_IsBool(akma_allowed)) {
        ogs_error("OpenAPI_authentication_subscription_parseFromJSON() failed [akma_allowed]");
        goto end;
    }
    }

    routing_id = cJSON_GetObjectItemCaseSensitive(authentication_subscriptionJSON, "routingId");
    if (routing_id) {
    if (!cJSON_IsString(routing_id) && !cJSON_IsNull(routing_id)) {
        ogs_error("OpenAPI_authentication_subscription_parseFromJSON() failed [routing_id]");
        goto end;
    }
    }

    authentication_subscription_local_var = OpenAPI_authentication_subscription_create (
        authentication_methodVariable,
        enc_permanent_key && !cJSON_IsNull(enc_permanent_key) ? ogs_strdup(enc_permanent_key->valuestring) : NULL,
        protection_parameter_id && !cJSON_IsNull(protection_parameter_id) ? ogs_strdup(protection_parameter_id->valuestring) : NULL,
        sequence_number ? sequence_number_local_nonprim : NULL,
        authentication_management_field && !cJSON_IsNull(authentication_management_field) ? ogs_strdup(authentication_management_field->valuestring) : NULL,
        algorithm_id && !cJSON_IsNull(algorithm_id) ? ogs_strdup(algorithm_id->valuestring) : NULL,
        enc_opc_key && !cJSON_IsNull(enc_opc_key) ? ogs_strdup(enc_opc_key->valuestring) : NULL,
        enc_topc_key && !cJSON_IsNull(enc_topc_key) ? ogs_strdup(enc_topc_key->valuestring) : NULL,
        vector_generation_in_hss ? true : false,
        vector_generation_in_hss ? vector_generation_in_hss->valueint : 0,
        hss_group_id && !cJSON_IsNull(hss_group_id) ? ogs_strdup(hss_group_id->valuestring) : NULL,
        n5gc_auth_method ? n5gc_auth_methodVariable : 0,
        rg_authentication_ind ? true : false,
        rg_authentication_ind ? rg_authentication_ind->valueint : 0,
        supi && !cJSON_IsNull(supi) ? ogs_strdup(supi->valuestring) : NULL,
        akma_allowed ? true : false,
        akma_allowed ? akma_allowed->valueint : 0,
        routing_id && !cJSON_IsNull(routing_id) ? ogs_strdup(routing_id->valuestring) : NULL
    );

    return authentication_subscription_local_var;
end:
    if (sequence_number_local_nonprim) {
        OpenAPI_sequence_number_free(sequence_number_local_nonprim);
        sequence_number_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_authentication_subscription_t *OpenAPI_authentication_subscription_copy(OpenAPI_authentication_subscription_t *dst, OpenAPI_authentication_subscription_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_authentication_subscription_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_authentication_subscription_convertToJSON() failed");
        return NULL;
    }

    content = cJSON_Print(item);
    cJSON_Delete(item);

    if (!content) {
        ogs_error("cJSON_Print() failed");
        return NULL;
    }

    item = cJSON_Parse(content);
    ogs_free(content);
    if (!item) {
        ogs_error("cJSON_Parse() failed");
        return NULL;
    }

    OpenAPI_authentication_subscription_free(dst);
    dst = OpenAPI_authentication_subscription_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

