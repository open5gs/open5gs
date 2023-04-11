
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "authentication_info_result.h"

OpenAPI_authentication_info_result_t *OpenAPI_authentication_info_result_create(
    OpenAPI_auth_type_e auth_type,
    char *supported_features,
    OpenAPI_authentication_vector_t *authentication_vector,
    char *supi,
    bool is_akma_ind,
    int akma_ind,
    bool is_auth_aaa,
    int auth_aaa,
    char *routing_id,
    OpenAPI_list_t *pvs_info
)
{
    OpenAPI_authentication_info_result_t *authentication_info_result_local_var = ogs_malloc(sizeof(OpenAPI_authentication_info_result_t));
    ogs_assert(authentication_info_result_local_var);

    authentication_info_result_local_var->auth_type = auth_type;
    authentication_info_result_local_var->supported_features = supported_features;
    authentication_info_result_local_var->authentication_vector = authentication_vector;
    authentication_info_result_local_var->supi = supi;
    authentication_info_result_local_var->is_akma_ind = is_akma_ind;
    authentication_info_result_local_var->akma_ind = akma_ind;
    authentication_info_result_local_var->is_auth_aaa = is_auth_aaa;
    authentication_info_result_local_var->auth_aaa = auth_aaa;
    authentication_info_result_local_var->routing_id = routing_id;
    authentication_info_result_local_var->pvs_info = pvs_info;

    return authentication_info_result_local_var;
}

void OpenAPI_authentication_info_result_free(OpenAPI_authentication_info_result_t *authentication_info_result)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == authentication_info_result) {
        return;
    }
    if (authentication_info_result->supported_features) {
        ogs_free(authentication_info_result->supported_features);
        authentication_info_result->supported_features = NULL;
    }
    if (authentication_info_result->authentication_vector) {
        OpenAPI_authentication_vector_free(authentication_info_result->authentication_vector);
        authentication_info_result->authentication_vector = NULL;
    }
    if (authentication_info_result->supi) {
        ogs_free(authentication_info_result->supi);
        authentication_info_result->supi = NULL;
    }
    if (authentication_info_result->routing_id) {
        ogs_free(authentication_info_result->routing_id);
        authentication_info_result->routing_id = NULL;
    }
    if (authentication_info_result->pvs_info) {
        OpenAPI_list_for_each(authentication_info_result->pvs_info, node) {
            OpenAPI_server_addressing_info_free(node->data);
        }
        OpenAPI_list_free(authentication_info_result->pvs_info);
        authentication_info_result->pvs_info = NULL;
    }
    ogs_free(authentication_info_result);
}

cJSON *OpenAPI_authentication_info_result_convertToJSON(OpenAPI_authentication_info_result_t *authentication_info_result)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (authentication_info_result == NULL) {
        ogs_error("OpenAPI_authentication_info_result_convertToJSON() failed [AuthenticationInfoResult]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (authentication_info_result->auth_type == OpenAPI_auth_type_NULL) {
        ogs_error("OpenAPI_authentication_info_result_convertToJSON() failed [auth_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "authType", OpenAPI_auth_type_ToString(authentication_info_result->auth_type)) == NULL) {
        ogs_error("OpenAPI_authentication_info_result_convertToJSON() failed [auth_type]");
        goto end;
    }

    if (authentication_info_result->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", authentication_info_result->supported_features) == NULL) {
        ogs_error("OpenAPI_authentication_info_result_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (authentication_info_result->authentication_vector) {
    cJSON *authentication_vector_local_JSON = OpenAPI_authentication_vector_convertToJSON(authentication_info_result->authentication_vector);
    if (authentication_vector_local_JSON == NULL) {
        ogs_error("OpenAPI_authentication_info_result_convertToJSON() failed [authentication_vector]");
        goto end;
    }
    cJSON_AddItemToObject(item, "authenticationVector", authentication_vector_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_authentication_info_result_convertToJSON() failed [authentication_vector]");
        goto end;
    }
    }

    if (authentication_info_result->supi) {
    if (cJSON_AddStringToObject(item, "supi", authentication_info_result->supi) == NULL) {
        ogs_error("OpenAPI_authentication_info_result_convertToJSON() failed [supi]");
        goto end;
    }
    }

    if (authentication_info_result->is_akma_ind) {
    if (cJSON_AddBoolToObject(item, "akmaInd", authentication_info_result->akma_ind) == NULL) {
        ogs_error("OpenAPI_authentication_info_result_convertToJSON() failed [akma_ind]");
        goto end;
    }
    }

    if (authentication_info_result->is_auth_aaa) {
    if (cJSON_AddBoolToObject(item, "authAaa", authentication_info_result->auth_aaa) == NULL) {
        ogs_error("OpenAPI_authentication_info_result_convertToJSON() failed [auth_aaa]");
        goto end;
    }
    }

    if (authentication_info_result->routing_id) {
    if (cJSON_AddStringToObject(item, "routingId", authentication_info_result->routing_id) == NULL) {
        ogs_error("OpenAPI_authentication_info_result_convertToJSON() failed [routing_id]");
        goto end;
    }
    }

    if (authentication_info_result->pvs_info) {
    cJSON *pvs_infoList = cJSON_AddArrayToObject(item, "pvsInfo");
    if (pvs_infoList == NULL) {
        ogs_error("OpenAPI_authentication_info_result_convertToJSON() failed [pvs_info]");
        goto end;
    }
    OpenAPI_list_for_each(authentication_info_result->pvs_info, node) {
        cJSON *itemLocal = OpenAPI_server_addressing_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_authentication_info_result_convertToJSON() failed [pvs_info]");
            goto end;
        }
        cJSON_AddItemToArray(pvs_infoList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_authentication_info_result_t *OpenAPI_authentication_info_result_parseFromJSON(cJSON *authentication_info_resultJSON)
{
    OpenAPI_authentication_info_result_t *authentication_info_result_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *auth_type = NULL;
    OpenAPI_auth_type_e auth_typeVariable = 0;
    cJSON *supported_features = NULL;
    cJSON *authentication_vector = NULL;
    OpenAPI_authentication_vector_t *authentication_vector_local_nonprim = NULL;
    cJSON *supi = NULL;
    cJSON *akma_ind = NULL;
    cJSON *auth_aaa = NULL;
    cJSON *routing_id = NULL;
    cJSON *pvs_info = NULL;
    OpenAPI_list_t *pvs_infoList = NULL;
    auth_type = cJSON_GetObjectItemCaseSensitive(authentication_info_resultJSON, "authType");
    if (!auth_type) {
        ogs_error("OpenAPI_authentication_info_result_parseFromJSON() failed [auth_type]");
        goto end;
    }
    if (!cJSON_IsString(auth_type)) {
        ogs_error("OpenAPI_authentication_info_result_parseFromJSON() failed [auth_type]");
        goto end;
    }
    auth_typeVariable = OpenAPI_auth_type_FromString(auth_type->valuestring);

    supported_features = cJSON_GetObjectItemCaseSensitive(authentication_info_resultJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_authentication_info_result_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    authentication_vector = cJSON_GetObjectItemCaseSensitive(authentication_info_resultJSON, "authenticationVector");
    if (authentication_vector) {
    authentication_vector_local_nonprim = OpenAPI_authentication_vector_parseFromJSON(authentication_vector);
    if (!authentication_vector_local_nonprim) {
        ogs_error("OpenAPI_authentication_vector_parseFromJSON failed [authentication_vector]");
        goto end;
    }
    }

    supi = cJSON_GetObjectItemCaseSensitive(authentication_info_resultJSON, "supi");
    if (supi) {
    if (!cJSON_IsString(supi) && !cJSON_IsNull(supi)) {
        ogs_error("OpenAPI_authentication_info_result_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    akma_ind = cJSON_GetObjectItemCaseSensitive(authentication_info_resultJSON, "akmaInd");
    if (akma_ind) {
    if (!cJSON_IsBool(akma_ind)) {
        ogs_error("OpenAPI_authentication_info_result_parseFromJSON() failed [akma_ind]");
        goto end;
    }
    }

    auth_aaa = cJSON_GetObjectItemCaseSensitive(authentication_info_resultJSON, "authAaa");
    if (auth_aaa) {
    if (!cJSON_IsBool(auth_aaa)) {
        ogs_error("OpenAPI_authentication_info_result_parseFromJSON() failed [auth_aaa]");
        goto end;
    }
    }

    routing_id = cJSON_GetObjectItemCaseSensitive(authentication_info_resultJSON, "routingId");
    if (routing_id) {
    if (!cJSON_IsString(routing_id) && !cJSON_IsNull(routing_id)) {
        ogs_error("OpenAPI_authentication_info_result_parseFromJSON() failed [routing_id]");
        goto end;
    }
    }

    pvs_info = cJSON_GetObjectItemCaseSensitive(authentication_info_resultJSON, "pvsInfo");
    if (pvs_info) {
        cJSON *pvs_info_local = NULL;
        if (!cJSON_IsArray(pvs_info)) {
            ogs_error("OpenAPI_authentication_info_result_parseFromJSON() failed [pvs_info]");
            goto end;
        }

        pvs_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(pvs_info_local, pvs_info) {
            if (!cJSON_IsObject(pvs_info_local)) {
                ogs_error("OpenAPI_authentication_info_result_parseFromJSON() failed [pvs_info]");
                goto end;
            }
            OpenAPI_server_addressing_info_t *pvs_infoItem = OpenAPI_server_addressing_info_parseFromJSON(pvs_info_local);
            if (!pvs_infoItem) {
                ogs_error("No pvs_infoItem");
                goto end;
            }
            OpenAPI_list_add(pvs_infoList, pvs_infoItem);
        }
    }

    authentication_info_result_local_var = OpenAPI_authentication_info_result_create (
        auth_typeVariable,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        authentication_vector ? authentication_vector_local_nonprim : NULL,
        supi && !cJSON_IsNull(supi) ? ogs_strdup(supi->valuestring) : NULL,
        akma_ind ? true : false,
        akma_ind ? akma_ind->valueint : 0,
        auth_aaa ? true : false,
        auth_aaa ? auth_aaa->valueint : 0,
        routing_id && !cJSON_IsNull(routing_id) ? ogs_strdup(routing_id->valuestring) : NULL,
        pvs_info ? pvs_infoList : NULL
    );

    return authentication_info_result_local_var;
end:
    if (authentication_vector_local_nonprim) {
        OpenAPI_authentication_vector_free(authentication_vector_local_nonprim);
        authentication_vector_local_nonprim = NULL;
    }
    if (pvs_infoList) {
        OpenAPI_list_for_each(pvs_infoList, node) {
            OpenAPI_server_addressing_info_free(node->data);
        }
        OpenAPI_list_free(pvs_infoList);
        pvs_infoList = NULL;
    }
    return NULL;
}

OpenAPI_authentication_info_result_t *OpenAPI_authentication_info_result_copy(OpenAPI_authentication_info_result_t *dst, OpenAPI_authentication_info_result_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_authentication_info_result_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_authentication_info_result_convertToJSON() failed");
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

    OpenAPI_authentication_info_result_free(dst);
    dst = OpenAPI_authentication_info_result_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

