
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
    char *registration_time
    )
{
    OpenAPI_smsf_registration_t *smsf_registration_local_var = OpenAPI_malloc(sizeof(OpenAPI_smsf_registration_t));
    if (!smsf_registration_local_var) {
        return NULL;
    }
    smsf_registration_local_var->smsf_instance_id = smsf_instance_id;
    smsf_registration_local_var->smsf_set_id = smsf_set_id;
    smsf_registration_local_var->supported_features = supported_features;
    smsf_registration_local_var->plmn_id = plmn_id;
    smsf_registration_local_var->smsf_map_address = smsf_map_address;
    smsf_registration_local_var->smsf_diameter_address = smsf_diameter_address;
    smsf_registration_local_var->registration_time = registration_time;

    return smsf_registration_local_var;
}

void OpenAPI_smsf_registration_free(OpenAPI_smsf_registration_t *smsf_registration)
{
    if (NULL == smsf_registration) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(smsf_registration->smsf_instance_id);
    ogs_free(smsf_registration->smsf_set_id);
    ogs_free(smsf_registration->supported_features);
    OpenAPI_plmn_id_free(smsf_registration->plmn_id);
    ogs_free(smsf_registration->smsf_map_address);
    OpenAPI_network_node_diameter_address_free(smsf_registration->smsf_diameter_address);
    ogs_free(smsf_registration->registration_time);
    ogs_free(smsf_registration);
}

cJSON *OpenAPI_smsf_registration_convertToJSON(OpenAPI_smsf_registration_t *smsf_registration)
{
    cJSON *item = NULL;

    if (smsf_registration == NULL) {
        ogs_error("OpenAPI_smsf_registration_convertToJSON() failed [SmsfRegistration]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!smsf_registration->smsf_instance_id) {
        ogs_error("OpenAPI_smsf_registration_convertToJSON() failed [smsf_instance_id]");
        goto end;
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
        goto end;
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

end:
    return item;
}

OpenAPI_smsf_registration_t *OpenAPI_smsf_registration_parseFromJSON(cJSON *smsf_registrationJSON)
{
    OpenAPI_smsf_registration_t *smsf_registration_local_var = NULL;
    cJSON *smsf_instance_id = cJSON_GetObjectItemCaseSensitive(smsf_registrationJSON, "smsfInstanceId");
    if (!smsf_instance_id) {
        ogs_error("OpenAPI_smsf_registration_parseFromJSON() failed [smsf_instance_id]");
        goto end;
    }


    if (!cJSON_IsString(smsf_instance_id)) {
        ogs_error("OpenAPI_smsf_registration_parseFromJSON() failed [smsf_instance_id]");
        goto end;
    }

    cJSON *smsf_set_id = cJSON_GetObjectItemCaseSensitive(smsf_registrationJSON, "smsfSetId");

    if (smsf_set_id) {
        if (!cJSON_IsString(smsf_set_id)) {
            ogs_error("OpenAPI_smsf_registration_parseFromJSON() failed [smsf_set_id]");
            goto end;
        }
    }

    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(smsf_registrationJSON, "supportedFeatures");

    if (supported_features) {
        if (!cJSON_IsString(supported_features)) {
            ogs_error("OpenAPI_smsf_registration_parseFromJSON() failed [supported_features]");
            goto end;
        }
    }

    cJSON *plmn_id = cJSON_GetObjectItemCaseSensitive(smsf_registrationJSON, "plmnId");
    if (!plmn_id) {
        ogs_error("OpenAPI_smsf_registration_parseFromJSON() failed [plmn_id]");
        goto end;
    }

    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;

    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);

    cJSON *smsf_map_address = cJSON_GetObjectItemCaseSensitive(smsf_registrationJSON, "smsfMAPAddress");

    if (smsf_map_address) {
        if (!cJSON_IsString(smsf_map_address)) {
            ogs_error("OpenAPI_smsf_registration_parseFromJSON() failed [smsf_map_address]");
            goto end;
        }
    }

    cJSON *smsf_diameter_address = cJSON_GetObjectItemCaseSensitive(smsf_registrationJSON, "smsfDiameterAddress");

    OpenAPI_network_node_diameter_address_t *smsf_diameter_address_local_nonprim = NULL;
    if (smsf_diameter_address) {
        smsf_diameter_address_local_nonprim = OpenAPI_network_node_diameter_address_parseFromJSON(smsf_diameter_address);
    }

    cJSON *registration_time = cJSON_GetObjectItemCaseSensitive(smsf_registrationJSON, "registrationTime");

    if (registration_time) {
        if (!cJSON_IsString(registration_time)) {
            ogs_error("OpenAPI_smsf_registration_parseFromJSON() failed [registration_time]");
            goto end;
        }
    }

    smsf_registration_local_var = OpenAPI_smsf_registration_create (
        ogs_strdup(smsf_instance_id->valuestring),
        smsf_set_id ? ogs_strdup(smsf_set_id->valuestring) : NULL,
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL,
        plmn_id_local_nonprim,
        smsf_map_address ? ogs_strdup(smsf_map_address->valuestring) : NULL,
        smsf_diameter_address ? smsf_diameter_address_local_nonprim : NULL,
        registration_time ? ogs_strdup(registration_time->valuestring) : NULL
        );

    return smsf_registration_local_var;
end:
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

