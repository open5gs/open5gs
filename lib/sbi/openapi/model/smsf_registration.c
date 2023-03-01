
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "smsf_registration.h"

OpenAPI_smsf_registration_t *OpenAPI_smsf_registration_create(
    char *smsf_instance_id,
    char *smsf_set_id,
    char *supported_features,
    OpenAPI_plmn_id_t *plmn_id,
    char *smsf_map_address,
    OpenAPI_network_node_diameter_address_t *smsf_diameter_address,
    char *registration_time,
    OpenAPI_context_info_t *context_info
)
{
    OpenAPI_smsf_registration_t *smsf_registration_local_var = ogs_malloc(sizeof(OpenAPI_smsf_registration_t));
    ogs_assert(smsf_registration_local_var);

    smsf_registration_local_var->smsf_instance_id = smsf_instance_id;
    smsf_registration_local_var->smsf_set_id = smsf_set_id;
    smsf_registration_local_var->supported_features = supported_features;
    smsf_registration_local_var->plmn_id = plmn_id;
    smsf_registration_local_var->smsf_map_address = smsf_map_address;
    smsf_registration_local_var->smsf_diameter_address = smsf_diameter_address;
    smsf_registration_local_var->registration_time = registration_time;
    smsf_registration_local_var->context_info = context_info;

    return smsf_registration_local_var;
}

void OpenAPI_smsf_registration_free(OpenAPI_smsf_registration_t *smsf_registration)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == smsf_registration) {
        return;
    }
    if (smsf_registration->smsf_instance_id) {
        ogs_free(smsf_registration->smsf_instance_id);
        smsf_registration->smsf_instance_id = NULL;
    }
    if (smsf_registration->smsf_set_id) {
        ogs_free(smsf_registration->smsf_set_id);
        smsf_registration->smsf_set_id = NULL;
    }
    if (smsf_registration->supported_features) {
        ogs_free(smsf_registration->supported_features);
        smsf_registration->supported_features = NULL;
    }
    if (smsf_registration->plmn_id) {
        OpenAPI_plmn_id_free(smsf_registration->plmn_id);
        smsf_registration->plmn_id = NULL;
    }
    if (smsf_registration->smsf_map_address) {
        ogs_free(smsf_registration->smsf_map_address);
        smsf_registration->smsf_map_address = NULL;
    }
    if (smsf_registration->smsf_diameter_address) {
        OpenAPI_network_node_diameter_address_free(smsf_registration->smsf_diameter_address);
        smsf_registration->smsf_diameter_address = NULL;
    }
    if (smsf_registration->registration_time) {
        ogs_free(smsf_registration->registration_time);
        smsf_registration->registration_time = NULL;
    }
    if (smsf_registration->context_info) {
        OpenAPI_context_info_free(smsf_registration->context_info);
        smsf_registration->context_info = NULL;
    }
    ogs_free(smsf_registration);
}

cJSON *OpenAPI_smsf_registration_convertToJSON(OpenAPI_smsf_registration_t *smsf_registration)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (smsf_registration == NULL) {
        ogs_error("OpenAPI_smsf_registration_convertToJSON() failed [SmsfRegistration]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!smsf_registration->smsf_instance_id) {
        ogs_error("OpenAPI_smsf_registration_convertToJSON() failed [smsf_instance_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "smsfInstanceId", smsf_registration->smsf_instance_id) == NULL) {
        ogs_error("OpenAPI_smsf_registration_convertToJSON() failed [smsf_instance_id]");
        goto end;
    }

    if (smsf_registration->smsf_set_id) {
    if (cJSON_AddStringToObject(item, "smsfSetId", smsf_registration->smsf_set_id) == NULL) {
        ogs_error("OpenAPI_smsf_registration_convertToJSON() failed [smsf_set_id]");
        goto end;
    }
    }

    if (smsf_registration->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", smsf_registration->supported_features) == NULL) {
        ogs_error("OpenAPI_smsf_registration_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (!smsf_registration->plmn_id) {
        ogs_error("OpenAPI_smsf_registration_convertToJSON() failed [plmn_id]");
        return NULL;
    }
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(smsf_registration->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_smsf_registration_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_smsf_registration_convertToJSON() failed [plmn_id]");
        goto end;
    }

    if (smsf_registration->smsf_map_address) {
    if (cJSON_AddStringToObject(item, "smsfMAPAddress", smsf_registration->smsf_map_address) == NULL) {
        ogs_error("OpenAPI_smsf_registration_convertToJSON() failed [smsf_map_address]");
        goto end;
    }
    }

    if (smsf_registration->smsf_diameter_address) {
    cJSON *smsf_diameter_address_local_JSON = OpenAPI_network_node_diameter_address_convertToJSON(smsf_registration->smsf_diameter_address);
    if (smsf_diameter_address_local_JSON == NULL) {
        ogs_error("OpenAPI_smsf_registration_convertToJSON() failed [smsf_diameter_address]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smsfDiameterAddress", smsf_diameter_address_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_smsf_registration_convertToJSON() failed [smsf_diameter_address]");
        goto end;
    }
    }

    if (smsf_registration->registration_time) {
    if (cJSON_AddStringToObject(item, "registrationTime", smsf_registration->registration_time) == NULL) {
        ogs_error("OpenAPI_smsf_registration_convertToJSON() failed [registration_time]");
        goto end;
    }
    }

    if (smsf_registration->context_info) {
    cJSON *context_info_local_JSON = OpenAPI_context_info_convertToJSON(smsf_registration->context_info);
    if (context_info_local_JSON == NULL) {
        ogs_error("OpenAPI_smsf_registration_convertToJSON() failed [context_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "contextInfo", context_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_smsf_registration_convertToJSON() failed [context_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_smsf_registration_t *OpenAPI_smsf_registration_parseFromJSON(cJSON *smsf_registrationJSON)
{
    OpenAPI_smsf_registration_t *smsf_registration_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *smsf_instance_id = NULL;
    cJSON *smsf_set_id = NULL;
    cJSON *supported_features = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;
    cJSON *smsf_map_address = NULL;
    cJSON *smsf_diameter_address = NULL;
    OpenAPI_network_node_diameter_address_t *smsf_diameter_address_local_nonprim = NULL;
    cJSON *registration_time = NULL;
    cJSON *context_info = NULL;
    OpenAPI_context_info_t *context_info_local_nonprim = NULL;
    smsf_instance_id = cJSON_GetObjectItemCaseSensitive(smsf_registrationJSON, "smsfInstanceId");
    if (!smsf_instance_id) {
        ogs_error("OpenAPI_smsf_registration_parseFromJSON() failed [smsf_instance_id]");
        goto end;
    }
    if (!cJSON_IsString(smsf_instance_id)) {
        ogs_error("OpenAPI_smsf_registration_parseFromJSON() failed [smsf_instance_id]");
        goto end;
    }

    smsf_set_id = cJSON_GetObjectItemCaseSensitive(smsf_registrationJSON, "smsfSetId");
    if (smsf_set_id) {
    if (!cJSON_IsString(smsf_set_id) && !cJSON_IsNull(smsf_set_id)) {
        ogs_error("OpenAPI_smsf_registration_parseFromJSON() failed [smsf_set_id]");
        goto end;
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(smsf_registrationJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_smsf_registration_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    plmn_id = cJSON_GetObjectItemCaseSensitive(smsf_registrationJSON, "plmnId");
    if (!plmn_id) {
        ogs_error("OpenAPI_smsf_registration_parseFromJSON() failed [plmn_id]");
        goto end;
    }
    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);

    smsf_map_address = cJSON_GetObjectItemCaseSensitive(smsf_registrationJSON, "smsfMAPAddress");
    if (smsf_map_address) {
    if (!cJSON_IsString(smsf_map_address) && !cJSON_IsNull(smsf_map_address)) {
        ogs_error("OpenAPI_smsf_registration_parseFromJSON() failed [smsf_map_address]");
        goto end;
    }
    }

    smsf_diameter_address = cJSON_GetObjectItemCaseSensitive(smsf_registrationJSON, "smsfDiameterAddress");
    if (smsf_diameter_address) {
    smsf_diameter_address_local_nonprim = OpenAPI_network_node_diameter_address_parseFromJSON(smsf_diameter_address);
    }

    registration_time = cJSON_GetObjectItemCaseSensitive(smsf_registrationJSON, "registrationTime");
    if (registration_time) {
    if (!cJSON_IsString(registration_time) && !cJSON_IsNull(registration_time)) {
        ogs_error("OpenAPI_smsf_registration_parseFromJSON() failed [registration_time]");
        goto end;
    }
    }

    context_info = cJSON_GetObjectItemCaseSensitive(smsf_registrationJSON, "contextInfo");
    if (context_info) {
    context_info_local_nonprim = OpenAPI_context_info_parseFromJSON(context_info);
    }

    smsf_registration_local_var = OpenAPI_smsf_registration_create (
        ogs_strdup(smsf_instance_id->valuestring),
        smsf_set_id && !cJSON_IsNull(smsf_set_id) ? ogs_strdup(smsf_set_id->valuestring) : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        plmn_id_local_nonprim,
        smsf_map_address && !cJSON_IsNull(smsf_map_address) ? ogs_strdup(smsf_map_address->valuestring) : NULL,
        smsf_diameter_address ? smsf_diameter_address_local_nonprim : NULL,
        registration_time && !cJSON_IsNull(registration_time) ? ogs_strdup(registration_time->valuestring) : NULL,
        context_info ? context_info_local_nonprim : NULL
    );

    return smsf_registration_local_var;
end:
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
    if (smsf_diameter_address_local_nonprim) {
        OpenAPI_network_node_diameter_address_free(smsf_diameter_address_local_nonprim);
        smsf_diameter_address_local_nonprim = NULL;
    }
    if (context_info_local_nonprim) {
        OpenAPI_context_info_free(context_info_local_nonprim);
        context_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_smsf_registration_t *OpenAPI_smsf_registration_copy(OpenAPI_smsf_registration_t *dst, OpenAPI_smsf_registration_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_smsf_registration_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_smsf_registration_convertToJSON() failed");
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

    OpenAPI_smsf_registration_free(dst);
    dst = OpenAPI_smsf_registration_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

