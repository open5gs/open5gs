
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nrf_info_served_gmlc_info_value.h"

OpenAPI_nrf_info_served_gmlc_info_value_t *OpenAPI_nrf_info_served_gmlc_info_value_create(
    OpenAPI_list_t *serving_client_types,
    OpenAPI_list_t *gmlc_numbers
)
{
    OpenAPI_nrf_info_served_gmlc_info_value_t *nrf_info_served_gmlc_info_value_local_var = ogs_malloc(sizeof(OpenAPI_nrf_info_served_gmlc_info_value_t));
    ogs_assert(nrf_info_served_gmlc_info_value_local_var);

    nrf_info_served_gmlc_info_value_local_var->serving_client_types = serving_client_types;
    nrf_info_served_gmlc_info_value_local_var->gmlc_numbers = gmlc_numbers;

    return nrf_info_served_gmlc_info_value_local_var;
}

void OpenAPI_nrf_info_served_gmlc_info_value_free(OpenAPI_nrf_info_served_gmlc_info_value_t *nrf_info_served_gmlc_info_value)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nrf_info_served_gmlc_info_value) {
        return;
    }
    if (nrf_info_served_gmlc_info_value->serving_client_types) {
        OpenAPI_list_free(nrf_info_served_gmlc_info_value->serving_client_types);
        nrf_info_served_gmlc_info_value->serving_client_types = NULL;
    }
    if (nrf_info_served_gmlc_info_value->gmlc_numbers) {
        OpenAPI_list_for_each(nrf_info_served_gmlc_info_value->gmlc_numbers, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nrf_info_served_gmlc_info_value->gmlc_numbers);
        nrf_info_served_gmlc_info_value->gmlc_numbers = NULL;
    }
    ogs_free(nrf_info_served_gmlc_info_value);
}

cJSON *OpenAPI_nrf_info_served_gmlc_info_value_convertToJSON(OpenAPI_nrf_info_served_gmlc_info_value_t *nrf_info_served_gmlc_info_value)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nrf_info_served_gmlc_info_value == NULL) {
        ogs_error("OpenAPI_nrf_info_served_gmlc_info_value_convertToJSON() failed [NrfInfo_servedGmlcInfo_value]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (nrf_info_served_gmlc_info_value->serving_client_types != OpenAPI_external_client_type_NULL) {
    cJSON *serving_client_typesList = cJSON_AddArrayToObject(item, "servingClientTypes");
    if (serving_client_typesList == NULL) {
        ogs_error("OpenAPI_nrf_info_served_gmlc_info_value_convertToJSON() failed [serving_client_types]");
        goto end;
    }
    OpenAPI_list_for_each(nrf_info_served_gmlc_info_value->serving_client_types, node) {
        if (cJSON_AddStringToObject(serving_client_typesList, "", OpenAPI_external_client_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_nrf_info_served_gmlc_info_value_convertToJSON() failed [serving_client_types]");
            goto end;
        }
    }
    }

    if (nrf_info_served_gmlc_info_value->gmlc_numbers) {
    cJSON *gmlc_numbersList = cJSON_AddArrayToObject(item, "gmlcNumbers");
    if (gmlc_numbersList == NULL) {
        ogs_error("OpenAPI_nrf_info_served_gmlc_info_value_convertToJSON() failed [gmlc_numbers]");
        goto end;
    }
    OpenAPI_list_for_each(nrf_info_served_gmlc_info_value->gmlc_numbers, node) {
        if (cJSON_AddStringToObject(gmlc_numbersList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_nrf_info_served_gmlc_info_value_convertToJSON() failed [gmlc_numbers]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_nrf_info_served_gmlc_info_value_t *OpenAPI_nrf_info_served_gmlc_info_value_parseFromJSON(cJSON *nrf_info_served_gmlc_info_valueJSON)
{
    OpenAPI_nrf_info_served_gmlc_info_value_t *nrf_info_served_gmlc_info_value_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *serving_client_types = NULL;
    OpenAPI_list_t *serving_client_typesList = NULL;
    cJSON *gmlc_numbers = NULL;
    OpenAPI_list_t *gmlc_numbersList = NULL;
    serving_client_types = cJSON_GetObjectItemCaseSensitive(nrf_info_served_gmlc_info_valueJSON, "servingClientTypes");
    if (serving_client_types) {
        cJSON *serving_client_types_local = NULL;
        if (!cJSON_IsArray(serving_client_types)) {
            ogs_error("OpenAPI_nrf_info_served_gmlc_info_value_parseFromJSON() failed [serving_client_types]");
            goto end;
        }

        serving_client_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(serving_client_types_local, serving_client_types) {
            OpenAPI_external_client_type_e localEnum = OpenAPI_external_client_type_NULL;
            if (!cJSON_IsString(serving_client_types_local)) {
                ogs_error("OpenAPI_nrf_info_served_gmlc_info_value_parseFromJSON() failed [serving_client_types]");
                goto end;
            }
            localEnum = OpenAPI_external_client_type_FromString(serving_client_types_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"serving_client_types\" is not supported. Ignoring it ...",
                         serving_client_types_local->valuestring);
            } else {
                OpenAPI_list_add(serving_client_typesList, (void *)localEnum);
            }
        }
        if (serving_client_typesList->count == 0) {
            ogs_error("OpenAPI_nrf_info_served_gmlc_info_value_parseFromJSON() failed: Expected serving_client_typesList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    gmlc_numbers = cJSON_GetObjectItemCaseSensitive(nrf_info_served_gmlc_info_valueJSON, "gmlcNumbers");
    if (gmlc_numbers) {
        cJSON *gmlc_numbers_local = NULL;
        if (!cJSON_IsArray(gmlc_numbers)) {
            ogs_error("OpenAPI_nrf_info_served_gmlc_info_value_parseFromJSON() failed [gmlc_numbers]");
            goto end;
        }

        gmlc_numbersList = OpenAPI_list_create();

        cJSON_ArrayForEach(gmlc_numbers_local, gmlc_numbers) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(gmlc_numbers_local)) {
                ogs_error("OpenAPI_nrf_info_served_gmlc_info_value_parseFromJSON() failed [gmlc_numbers]");
                goto end;
            }
            OpenAPI_list_add(gmlc_numbersList, ogs_strdup(gmlc_numbers_local->valuestring));
        }
    }

    nrf_info_served_gmlc_info_value_local_var = OpenAPI_nrf_info_served_gmlc_info_value_create (
        serving_client_types ? serving_client_typesList : NULL,
        gmlc_numbers ? gmlc_numbersList : NULL
    );

    return nrf_info_served_gmlc_info_value_local_var;
end:
    if (serving_client_typesList) {
        OpenAPI_list_free(serving_client_typesList);
        serving_client_typesList = NULL;
    }
    if (gmlc_numbersList) {
        OpenAPI_list_for_each(gmlc_numbersList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(gmlc_numbersList);
        gmlc_numbersList = NULL;
    }
    return NULL;
}

OpenAPI_nrf_info_served_gmlc_info_value_t *OpenAPI_nrf_info_served_gmlc_info_value_copy(OpenAPI_nrf_info_served_gmlc_info_value_t *dst, OpenAPI_nrf_info_served_gmlc_info_value_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nrf_info_served_gmlc_info_value_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nrf_info_served_gmlc_info_value_convertToJSON() failed");
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

    OpenAPI_nrf_info_served_gmlc_info_value_free(dst);
    dst = OpenAPI_nrf_info_served_gmlc_info_value_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

