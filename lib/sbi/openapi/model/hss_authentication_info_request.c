
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hss_authentication_info_request.h"

OpenAPI_hss_authentication_info_request_t *OpenAPI_hss_authentication_info_request_create(
    char *supported_features,
    OpenAPI_hss_auth_type_e hss_auth_type,
    int num_of_requested_vectors,
    OpenAPI_node_type_e requesting_node_type,
    OpenAPI_plmn_id_t *serving_network_id,
    OpenAPI_resynchronization_info_t *resynchronization_info,
    OpenAPI_access_network_id_e an_id
)
{
    OpenAPI_hss_authentication_info_request_t *hss_authentication_info_request_local_var = ogs_malloc(sizeof(OpenAPI_hss_authentication_info_request_t));
    ogs_assert(hss_authentication_info_request_local_var);

    hss_authentication_info_request_local_var->supported_features = supported_features;
    hss_authentication_info_request_local_var->hss_auth_type = hss_auth_type;
    hss_authentication_info_request_local_var->num_of_requested_vectors = num_of_requested_vectors;
    hss_authentication_info_request_local_var->requesting_node_type = requesting_node_type;
    hss_authentication_info_request_local_var->serving_network_id = serving_network_id;
    hss_authentication_info_request_local_var->resynchronization_info = resynchronization_info;
    hss_authentication_info_request_local_var->an_id = an_id;

    return hss_authentication_info_request_local_var;
}

void OpenAPI_hss_authentication_info_request_free(OpenAPI_hss_authentication_info_request_t *hss_authentication_info_request)
{
    if (NULL == hss_authentication_info_request) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(hss_authentication_info_request->supported_features);
    OpenAPI_plmn_id_free(hss_authentication_info_request->serving_network_id);
    OpenAPI_resynchronization_info_free(hss_authentication_info_request->resynchronization_info);
    ogs_free(hss_authentication_info_request);
}

cJSON *OpenAPI_hss_authentication_info_request_convertToJSON(OpenAPI_hss_authentication_info_request_t *hss_authentication_info_request)
{
    cJSON *item = NULL;

    if (hss_authentication_info_request == NULL) {
        ogs_error("OpenAPI_hss_authentication_info_request_convertToJSON() failed [HssAuthenticationInfoRequest]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (hss_authentication_info_request->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", hss_authentication_info_request->supported_features) == NULL) {
        ogs_error("OpenAPI_hss_authentication_info_request_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (cJSON_AddStringToObject(item, "hssAuthType", OpenAPI_hss_auth_type_ToString(hss_authentication_info_request->hss_auth_type)) == NULL) {
        ogs_error("OpenAPI_hss_authentication_info_request_convertToJSON() failed [hss_auth_type]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "numOfRequestedVectors", hss_authentication_info_request->num_of_requested_vectors) == NULL) {
        ogs_error("OpenAPI_hss_authentication_info_request_convertToJSON() failed [num_of_requested_vectors]");
        goto end;
    }

    if (hss_authentication_info_request->requesting_node_type) {
    if (cJSON_AddStringToObject(item, "requestingNodeType", OpenAPI_node_type_ToString(hss_authentication_info_request->requesting_node_type)) == NULL) {
        ogs_error("OpenAPI_hss_authentication_info_request_convertToJSON() failed [requesting_node_type]");
        goto end;
    }
    }

    if (hss_authentication_info_request->serving_network_id) {
    cJSON *serving_network_id_local_JSON = OpenAPI_plmn_id_convertToJSON(hss_authentication_info_request->serving_network_id);
    if (serving_network_id_local_JSON == NULL) {
        ogs_error("OpenAPI_hss_authentication_info_request_convertToJSON() failed [serving_network_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "servingNetworkId", serving_network_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hss_authentication_info_request_convertToJSON() failed [serving_network_id]");
        goto end;
    }
    }

    if (hss_authentication_info_request->resynchronization_info) {
    cJSON *resynchronization_info_local_JSON = OpenAPI_resynchronization_info_convertToJSON(hss_authentication_info_request->resynchronization_info);
    if (resynchronization_info_local_JSON == NULL) {
        ogs_error("OpenAPI_hss_authentication_info_request_convertToJSON() failed [resynchronization_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "resynchronizationInfo", resynchronization_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hss_authentication_info_request_convertToJSON() failed [resynchronization_info]");
        goto end;
    }
    }

    if (hss_authentication_info_request->an_id) {
    if (cJSON_AddStringToObject(item, "anId", OpenAPI_access_network_id_ToString(hss_authentication_info_request->an_id)) == NULL) {
        ogs_error("OpenAPI_hss_authentication_info_request_convertToJSON() failed [an_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_hss_authentication_info_request_t *OpenAPI_hss_authentication_info_request_parseFromJSON(cJSON *hss_authentication_info_requestJSON)
{
    OpenAPI_hss_authentication_info_request_t *hss_authentication_info_request_local_var = NULL;
    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(hss_authentication_info_requestJSON, "supportedFeatures");

    if (supported_features) {
    if (!cJSON_IsString(supported_features)) {
        ogs_error("OpenAPI_hss_authentication_info_request_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    cJSON *hss_auth_type = cJSON_GetObjectItemCaseSensitive(hss_authentication_info_requestJSON, "hssAuthType");
    if (!hss_auth_type) {
        ogs_error("OpenAPI_hss_authentication_info_request_parseFromJSON() failed [hss_auth_type]");
        goto end;
    }

    OpenAPI_hss_auth_type_e hss_auth_typeVariable;
    if (!cJSON_IsString(hss_auth_type)) {
        ogs_error("OpenAPI_hss_authentication_info_request_parseFromJSON() failed [hss_auth_type]");
        goto end;
    }
    hss_auth_typeVariable = OpenAPI_hss_auth_type_FromString(hss_auth_type->valuestring);

    cJSON *num_of_requested_vectors = cJSON_GetObjectItemCaseSensitive(hss_authentication_info_requestJSON, "numOfRequestedVectors");
    if (!num_of_requested_vectors) {
        ogs_error("OpenAPI_hss_authentication_info_request_parseFromJSON() failed [num_of_requested_vectors]");
        goto end;
    }

    if (!cJSON_IsNumber(num_of_requested_vectors)) {
        ogs_error("OpenAPI_hss_authentication_info_request_parseFromJSON() failed [num_of_requested_vectors]");
        goto end;
    }

    cJSON *requesting_node_type = cJSON_GetObjectItemCaseSensitive(hss_authentication_info_requestJSON, "requestingNodeType");

    OpenAPI_node_type_e requesting_node_typeVariable;
    if (requesting_node_type) {
    if (!cJSON_IsString(requesting_node_type)) {
        ogs_error("OpenAPI_hss_authentication_info_request_parseFromJSON() failed [requesting_node_type]");
        goto end;
    }
    requesting_node_typeVariable = OpenAPI_node_type_FromString(requesting_node_type->valuestring);
    }

    cJSON *serving_network_id = cJSON_GetObjectItemCaseSensitive(hss_authentication_info_requestJSON, "servingNetworkId");

    OpenAPI_plmn_id_t *serving_network_id_local_nonprim = NULL;
    if (serving_network_id) {
    serving_network_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(serving_network_id);
    }

    cJSON *resynchronization_info = cJSON_GetObjectItemCaseSensitive(hss_authentication_info_requestJSON, "resynchronizationInfo");

    OpenAPI_resynchronization_info_t *resynchronization_info_local_nonprim = NULL;
    if (resynchronization_info) {
    resynchronization_info_local_nonprim = OpenAPI_resynchronization_info_parseFromJSON(resynchronization_info);
    }

    cJSON *an_id = cJSON_GetObjectItemCaseSensitive(hss_authentication_info_requestJSON, "anId");

    OpenAPI_access_network_id_e an_idVariable;
    if (an_id) {
    if (!cJSON_IsString(an_id)) {
        ogs_error("OpenAPI_hss_authentication_info_request_parseFromJSON() failed [an_id]");
        goto end;
    }
    an_idVariable = OpenAPI_access_network_id_FromString(an_id->valuestring);
    }

    hss_authentication_info_request_local_var = OpenAPI_hss_authentication_info_request_create (
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL,
        hss_auth_typeVariable,
        
        num_of_requested_vectors->valuedouble,
        requesting_node_type ? requesting_node_typeVariable : 0,
        serving_network_id ? serving_network_id_local_nonprim : NULL,
        resynchronization_info ? resynchronization_info_local_nonprim : NULL,
        an_id ? an_idVariable : 0
    );

    return hss_authentication_info_request_local_var;
end:
    return NULL;
}

OpenAPI_hss_authentication_info_request_t *OpenAPI_hss_authentication_info_request_copy(OpenAPI_hss_authentication_info_request_t *dst, OpenAPI_hss_authentication_info_request_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_hss_authentication_info_request_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_hss_authentication_info_request_convertToJSON() failed");
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

    OpenAPI_hss_authentication_info_request_free(dst);
    dst = OpenAPI_hss_authentication_info_request_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

