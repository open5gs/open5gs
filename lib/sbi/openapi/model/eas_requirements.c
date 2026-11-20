
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "eas_requirements.h"

OpenAPI_eas_requirements_t *OpenAPI_eas_requirements_create(
    OpenAPI_serving_location_t *required_ea_sserving_location,
    OpenAPI_software_image_info_t *software_image_info,
    OpenAPI_affinity_anti_affinity_t *affinity_anti_affinity,
    bool is_service_continuity,
    int service_continuity,
    OpenAPI_virtual_resource_t *virtual_resource
)
{
    OpenAPI_eas_requirements_t *eas_requirements_local_var = ogs_malloc(sizeof(OpenAPI_eas_requirements_t));
    ogs_assert(eas_requirements_local_var);

    eas_requirements_local_var->required_ea_sserving_location = required_ea_sserving_location;
    eas_requirements_local_var->software_image_info = software_image_info;
    eas_requirements_local_var->affinity_anti_affinity = affinity_anti_affinity;
    eas_requirements_local_var->is_service_continuity = is_service_continuity;
    eas_requirements_local_var->service_continuity = service_continuity;
    eas_requirements_local_var->virtual_resource = virtual_resource;

    return eas_requirements_local_var;
}

void OpenAPI_eas_requirements_free(OpenAPI_eas_requirements_t *eas_requirements)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == eas_requirements) {
        return;
    }
    if (eas_requirements->required_ea_sserving_location) {
        OpenAPI_serving_location_free(eas_requirements->required_ea_sserving_location);
        eas_requirements->required_ea_sserving_location = NULL;
    }
    if (eas_requirements->software_image_info) {
        OpenAPI_software_image_info_free(eas_requirements->software_image_info);
        eas_requirements->software_image_info = NULL;
    }
    if (eas_requirements->affinity_anti_affinity) {
        OpenAPI_affinity_anti_affinity_free(eas_requirements->affinity_anti_affinity);
        eas_requirements->affinity_anti_affinity = NULL;
    }
    if (eas_requirements->virtual_resource) {
        OpenAPI_virtual_resource_free(eas_requirements->virtual_resource);
        eas_requirements->virtual_resource = NULL;
    }
    ogs_free(eas_requirements);
}

cJSON *OpenAPI_eas_requirements_convertToJSON(OpenAPI_eas_requirements_t *eas_requirements)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (eas_requirements == NULL) {
        ogs_error("OpenAPI_eas_requirements_convertToJSON() failed [EASRequirements]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (eas_requirements->required_ea_sserving_location) {
    cJSON *required_ea_sserving_location_local_JSON = OpenAPI_serving_location_convertToJSON(eas_requirements->required_ea_sserving_location);
    if (required_ea_sserving_location_local_JSON == NULL) {
        ogs_error("OpenAPI_eas_requirements_convertToJSON() failed [required_ea_sserving_location]");
        goto end;
    }
    cJSON_AddItemToObject(item, "requiredEASservingLocation", required_ea_sserving_location_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_eas_requirements_convertToJSON() failed [required_ea_sserving_location]");
        goto end;
    }
    }

    if (eas_requirements->software_image_info) {
    cJSON *software_image_info_local_JSON = OpenAPI_software_image_info_convertToJSON(eas_requirements->software_image_info);
    if (software_image_info_local_JSON == NULL) {
        ogs_error("OpenAPI_eas_requirements_convertToJSON() failed [software_image_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "softwareImageInfo", software_image_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_eas_requirements_convertToJSON() failed [software_image_info]");
        goto end;
    }
    }

    if (eas_requirements->affinity_anti_affinity) {
    cJSON *affinity_anti_affinity_local_JSON = OpenAPI_affinity_anti_affinity_convertToJSON(eas_requirements->affinity_anti_affinity);
    if (affinity_anti_affinity_local_JSON == NULL) {
        ogs_error("OpenAPI_eas_requirements_convertToJSON() failed [affinity_anti_affinity]");
        goto end;
    }
    cJSON_AddItemToObject(item, "affinityAntiAffinity", affinity_anti_affinity_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_eas_requirements_convertToJSON() failed [affinity_anti_affinity]");
        goto end;
    }
    }

    if (eas_requirements->is_service_continuity) {
    if (cJSON_AddBoolToObject(item, "serviceContinuity", eas_requirements->service_continuity) == NULL) {
        ogs_error("OpenAPI_eas_requirements_convertToJSON() failed [service_continuity]");
        goto end;
    }
    }

    if (eas_requirements->virtual_resource) {
    cJSON *virtual_resource_local_JSON = OpenAPI_virtual_resource_convertToJSON(eas_requirements->virtual_resource);
    if (virtual_resource_local_JSON == NULL) {
        ogs_error("OpenAPI_eas_requirements_convertToJSON() failed [virtual_resource]");
        goto end;
    }
    cJSON_AddItemToObject(item, "virtualResource", virtual_resource_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_eas_requirements_convertToJSON() failed [virtual_resource]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_eas_requirements_t *OpenAPI_eas_requirements_parseFromJSON(cJSON *eas_requirementsJSON)
{
    OpenAPI_eas_requirements_t *eas_requirements_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *required_ea_sserving_location = NULL;
    OpenAPI_serving_location_t *required_ea_sserving_location_local_nonprim = NULL;
    cJSON *software_image_info = NULL;
    OpenAPI_software_image_info_t *software_image_info_local_nonprim = NULL;
    cJSON *affinity_anti_affinity = NULL;
    OpenAPI_affinity_anti_affinity_t *affinity_anti_affinity_local_nonprim = NULL;
    cJSON *service_continuity = NULL;
    cJSON *virtual_resource = NULL;
    OpenAPI_virtual_resource_t *virtual_resource_local_nonprim = NULL;
    required_ea_sserving_location = cJSON_GetObjectItemCaseSensitive(eas_requirementsJSON, "requiredEASservingLocation");
    if (required_ea_sserving_location) {
    required_ea_sserving_location_local_nonprim = OpenAPI_serving_location_parseFromJSON(required_ea_sserving_location);
    if (!required_ea_sserving_location_local_nonprim) {
        ogs_error("OpenAPI_serving_location_parseFromJSON failed [required_ea_sserving_location]");
        goto end;
    }
    }

    software_image_info = cJSON_GetObjectItemCaseSensitive(eas_requirementsJSON, "softwareImageInfo");
    if (software_image_info) {
    software_image_info_local_nonprim = OpenAPI_software_image_info_parseFromJSON(software_image_info);
    if (!software_image_info_local_nonprim) {
        ogs_error("OpenAPI_software_image_info_parseFromJSON failed [software_image_info]");
        goto end;
    }
    }

    affinity_anti_affinity = cJSON_GetObjectItemCaseSensitive(eas_requirementsJSON, "affinityAntiAffinity");
    if (affinity_anti_affinity) {
    affinity_anti_affinity_local_nonprim = OpenAPI_affinity_anti_affinity_parseFromJSON(affinity_anti_affinity);
    if (!affinity_anti_affinity_local_nonprim) {
        ogs_error("OpenAPI_affinity_anti_affinity_parseFromJSON failed [affinity_anti_affinity]");
        goto end;
    }
    }

    service_continuity = cJSON_GetObjectItemCaseSensitive(eas_requirementsJSON, "serviceContinuity");
    if (service_continuity) {
    if (!cJSON_IsBool(service_continuity)) {
        ogs_error("OpenAPI_eas_requirements_parseFromJSON() failed [service_continuity]");
        goto end;
    }
    }

    virtual_resource = cJSON_GetObjectItemCaseSensitive(eas_requirementsJSON, "virtualResource");
    if (virtual_resource) {
    virtual_resource_local_nonprim = OpenAPI_virtual_resource_parseFromJSON(virtual_resource);
    if (!virtual_resource_local_nonprim) {
        ogs_error("OpenAPI_virtual_resource_parseFromJSON failed [virtual_resource]");
        goto end;
    }
    }

    eas_requirements_local_var = OpenAPI_eas_requirements_create (
        required_ea_sserving_location ? required_ea_sserving_location_local_nonprim : NULL,
        software_image_info ? software_image_info_local_nonprim : NULL,
        affinity_anti_affinity ? affinity_anti_affinity_local_nonprim : NULL,
        service_continuity ? true : false,
        service_continuity ? service_continuity->valueint : 0,
        virtual_resource ? virtual_resource_local_nonprim : NULL
    );

    return eas_requirements_local_var;
end:
    if (required_ea_sserving_location_local_nonprim) {
        OpenAPI_serving_location_free(required_ea_sserving_location_local_nonprim);
        required_ea_sserving_location_local_nonprim = NULL;
    }
    if (software_image_info_local_nonprim) {
        OpenAPI_software_image_info_free(software_image_info_local_nonprim);
        software_image_info_local_nonprim = NULL;
    }
    if (affinity_anti_affinity_local_nonprim) {
        OpenAPI_affinity_anti_affinity_free(affinity_anti_affinity_local_nonprim);
        affinity_anti_affinity_local_nonprim = NULL;
    }
    if (virtual_resource_local_nonprim) {
        OpenAPI_virtual_resource_free(virtual_resource_local_nonprim);
        virtual_resource_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_eas_requirements_t *OpenAPI_eas_requirements_copy(OpenAPI_eas_requirements_t *dst, OpenAPI_eas_requirements_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_eas_requirements_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_eas_requirements_convertToJSON() failed");
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

    OpenAPI_eas_requirements_free(dst);
    dst = OpenAPI_eas_requirements_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

