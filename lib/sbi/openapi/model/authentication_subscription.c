
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
    int vector_generation_in_hss
    )
{
    OpenAPI_authentication_subscription_t *authentication_subscription_local_var = OpenAPI_malloc(sizeof(OpenAPI_authentication_subscription_t));
    if (!authentication_subscription_local_var) {
        return NULL;
    }
    authentication_subscription_local_var->authentication_method = authentication_method;
    authentication_subscription_local_var->enc_permanent_key = enc_permanent_key;
    authentication_subscription_local_var->protection_parameter_id = protection_parameter_id;
    authentication_subscription_local_var->sequence_number = sequence_number;
    authentication_subscription_local_var->authentication_management_field = authentication_management_field;
    authentication_subscription_local_var->algorithm_id = algorithm_id;
    authentication_subscription_local_var->enc_opc_key = enc_opc_key;
    authentication_subscription_local_var->enc_topc_key = enc_topc_key;
    authentication_subscription_local_var->vector_generation_in_hss = vector_generation_in_hss;

    return authentication_subscription_local_var;
}

void OpenAPI_authentication_subscription_free(OpenAPI_authentication_subscription_t *authentication_subscription)
{
    if (NULL == authentication_subscription) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(authentication_subscription->enc_permanent_key);
    ogs_free(authentication_subscription->protection_parameter_id);
    OpenAPI_sequence_number_free(authentication_subscription->sequence_number);
    ogs_free(authentication_subscription->authentication_management_field);
    ogs_free(authentication_subscription->algorithm_id);
    ogs_free(authentication_subscription->enc_opc_key);
    ogs_free(authentication_subscription->enc_topc_key);
    ogs_free(authentication_subscription);
}

cJSON *OpenAPI_authentication_subscription_convertToJSON(OpenAPI_authentication_subscription_t *authentication_subscription)
{
    cJSON *item = NULL;

    if (authentication_subscription == NULL) {
        ogs_error("OpenAPI_authentication_subscription_convertToJSON() failed [AuthenticationSubscription]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!authentication_subscription->authentication_method) {
        ogs_error("OpenAPI_authentication_subscription_convertToJSON() failed [authentication_method]");
        goto end;
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

    if (authentication_subscription->vector_generation_in_hss) {
        if (cJSON_AddBoolToObject(item, "vectorGenerationInHss", authentication_subscription->vector_generation_in_hss) == NULL) {
            ogs_error("OpenAPI_authentication_subscription_convertToJSON() failed [vector_generation_in_hss]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_authentication_subscription_t *OpenAPI_authentication_subscription_parseFromJSON(cJSON *authentication_subscriptionJSON)
{
    OpenAPI_authentication_subscription_t *authentication_subscription_local_var = NULL;
    cJSON *authentication_method = cJSON_GetObjectItemCaseSensitive(authentication_subscriptionJSON, "authenticationMethod");
    if (!authentication_method) {
        ogs_error("OpenAPI_authentication_subscription_parseFromJSON() failed [authentication_method]");
        goto end;
    }

    OpenAPI_auth_method_e authentication_methodVariable;

    if (!cJSON_IsString(authentication_method)) {
        ogs_error("OpenAPI_authentication_subscription_parseFromJSON() failed [authentication_method]");
        goto end;
    }
    authentication_methodVariable = OpenAPI_auth_method_FromString(authentication_method->valuestring);

    cJSON *enc_permanent_key = cJSON_GetObjectItemCaseSensitive(authentication_subscriptionJSON, "encPermanentKey");

    if (enc_permanent_key) {
        if (!cJSON_IsString(enc_permanent_key)) {
            ogs_error("OpenAPI_authentication_subscription_parseFromJSON() failed [enc_permanent_key]");
            goto end;
        }
    }

    cJSON *protection_parameter_id = cJSON_GetObjectItemCaseSensitive(authentication_subscriptionJSON, "protectionParameterId");

    if (protection_parameter_id) {
        if (!cJSON_IsString(protection_parameter_id)) {
            ogs_error("OpenAPI_authentication_subscription_parseFromJSON() failed [protection_parameter_id]");
            goto end;
        }
    }

    cJSON *sequence_number = cJSON_GetObjectItemCaseSensitive(authentication_subscriptionJSON, "sequenceNumber");

    OpenAPI_sequence_number_t *sequence_number_local_nonprim = NULL;
    if (sequence_number) {
        sequence_number_local_nonprim = OpenAPI_sequence_number_parseFromJSON(sequence_number);
    }

    cJSON *authentication_management_field = cJSON_GetObjectItemCaseSensitive(authentication_subscriptionJSON, "authenticationManagementField");

    if (authentication_management_field) {
        if (!cJSON_IsString(authentication_management_field)) {
            ogs_error("OpenAPI_authentication_subscription_parseFromJSON() failed [authentication_management_field]");
            goto end;
        }
    }

    cJSON *algorithm_id = cJSON_GetObjectItemCaseSensitive(authentication_subscriptionJSON, "algorithmId");

    if (algorithm_id) {
        if (!cJSON_IsString(algorithm_id)) {
            ogs_error("OpenAPI_authentication_subscription_parseFromJSON() failed [algorithm_id]");
            goto end;
        }
    }

    cJSON *enc_opc_key = cJSON_GetObjectItemCaseSensitive(authentication_subscriptionJSON, "encOpcKey");

    if (enc_opc_key) {
        if (!cJSON_IsString(enc_opc_key)) {
            ogs_error("OpenAPI_authentication_subscription_parseFromJSON() failed [enc_opc_key]");
            goto end;
        }
    }

    cJSON *enc_topc_key = cJSON_GetObjectItemCaseSensitive(authentication_subscriptionJSON, "encTopcKey");

    if (enc_topc_key) {
        if (!cJSON_IsString(enc_topc_key)) {
            ogs_error("OpenAPI_authentication_subscription_parseFromJSON() failed [enc_topc_key]");
            goto end;
        }
    }

    cJSON *vector_generation_in_hss = cJSON_GetObjectItemCaseSensitive(authentication_subscriptionJSON, "vectorGenerationInHss");

    if (vector_generation_in_hss) {
        if (!cJSON_IsBool(vector_generation_in_hss)) {
            ogs_error("OpenAPI_authentication_subscription_parseFromJSON() failed [vector_generation_in_hss]");
            goto end;
        }
    }

    authentication_subscription_local_var = OpenAPI_authentication_subscription_create (
        authentication_methodVariable,
        enc_permanent_key ? ogs_strdup(enc_permanent_key->valuestring) : NULL,
        protection_parameter_id ? ogs_strdup(protection_parameter_id->valuestring) : NULL,
        sequence_number ? sequence_number_local_nonprim : NULL,
        authentication_management_field ? ogs_strdup(authentication_management_field->valuestring) : NULL,
        algorithm_id ? ogs_strdup(algorithm_id->valuestring) : NULL,
        enc_opc_key ? ogs_strdup(enc_opc_key->valuestring) : NULL,
        enc_topc_key ? ogs_strdup(enc_topc_key->valuestring) : NULL,
        vector_generation_in_hss ? vector_generation_in_hss->valueint : 0
        );

    return authentication_subscription_local_var;
end:
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

