
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mm_tel_charging_information.h"

OpenAPI_mm_tel_charging_information_t *OpenAPI_mm_tel_charging_information_create(
    OpenAPI_list_t *supplementary_services
)
{
    OpenAPI_mm_tel_charging_information_t *mm_tel_charging_information_local_var = ogs_malloc(sizeof(OpenAPI_mm_tel_charging_information_t));
    ogs_assert(mm_tel_charging_information_local_var);

    mm_tel_charging_information_local_var->supplementary_services = supplementary_services;

    return mm_tel_charging_information_local_var;
}

void OpenAPI_mm_tel_charging_information_free(OpenAPI_mm_tel_charging_information_t *mm_tel_charging_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == mm_tel_charging_information) {
        return;
    }
    if (mm_tel_charging_information->supplementary_services) {
        OpenAPI_list_for_each(mm_tel_charging_information->supplementary_services, node) {
            OpenAPI_supplementary_service_free(node->data);
        }
        OpenAPI_list_free(mm_tel_charging_information->supplementary_services);
        mm_tel_charging_information->supplementary_services = NULL;
    }
    ogs_free(mm_tel_charging_information);
}

cJSON *OpenAPI_mm_tel_charging_information_convertToJSON(OpenAPI_mm_tel_charging_information_t *mm_tel_charging_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (mm_tel_charging_information == NULL) {
        ogs_error("OpenAPI_mm_tel_charging_information_convertToJSON() failed [MMTelChargingInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (mm_tel_charging_information->supplementary_services) {
    cJSON *supplementary_servicesList = cJSON_AddArrayToObject(item, "supplementaryServices");
    if (supplementary_servicesList == NULL) {
        ogs_error("OpenAPI_mm_tel_charging_information_convertToJSON() failed [supplementary_services]");
        goto end;
    }
    OpenAPI_list_for_each(mm_tel_charging_information->supplementary_services, node) {
        cJSON *itemLocal = OpenAPI_supplementary_service_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_mm_tel_charging_information_convertToJSON() failed [supplementary_services]");
            goto end;
        }
        cJSON_AddItemToArray(supplementary_servicesList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_mm_tel_charging_information_t *OpenAPI_mm_tel_charging_information_parseFromJSON(cJSON *mm_tel_charging_informationJSON)
{
    OpenAPI_mm_tel_charging_information_t *mm_tel_charging_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *supplementary_services = NULL;
    OpenAPI_list_t *supplementary_servicesList = NULL;
    supplementary_services = cJSON_GetObjectItemCaseSensitive(mm_tel_charging_informationJSON, "supplementaryServices");
    if (supplementary_services) {
        cJSON *supplementary_services_local = NULL;
        if (!cJSON_IsArray(supplementary_services)) {
            ogs_error("OpenAPI_mm_tel_charging_information_parseFromJSON() failed [supplementary_services]");
            goto end;
        }

        supplementary_servicesList = OpenAPI_list_create();

        cJSON_ArrayForEach(supplementary_services_local, supplementary_services) {
            if (!cJSON_IsObject(supplementary_services_local)) {
                ogs_error("OpenAPI_mm_tel_charging_information_parseFromJSON() failed [supplementary_services]");
                goto end;
            }
            OpenAPI_supplementary_service_t *supplementary_servicesItem = OpenAPI_supplementary_service_parseFromJSON(supplementary_services_local);
            if (!supplementary_servicesItem) {
                ogs_error("No supplementary_servicesItem");
                goto end;
            }
            OpenAPI_list_add(supplementary_servicesList, supplementary_servicesItem);
        }
    }

    mm_tel_charging_information_local_var = OpenAPI_mm_tel_charging_information_create (
        supplementary_services ? supplementary_servicesList : NULL
    );

    return mm_tel_charging_information_local_var;
end:
    if (supplementary_servicesList) {
        OpenAPI_list_for_each(supplementary_servicesList, node) {
            OpenAPI_supplementary_service_free(node->data);
        }
        OpenAPI_list_free(supplementary_servicesList);
        supplementary_servicesList = NULL;
    }
    return NULL;
}

OpenAPI_mm_tel_charging_information_t *OpenAPI_mm_tel_charging_information_copy(OpenAPI_mm_tel_charging_information_t *dst, OpenAPI_mm_tel_charging_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mm_tel_charging_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mm_tel_charging_information_convertToJSON() failed");
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

    OpenAPI_mm_tel_charging_information_free(dst);
    dst = OpenAPI_mm_tel_charging_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

