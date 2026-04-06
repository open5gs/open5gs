
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "gmlc_info.h"

OpenAPI_gmlc_info_t *OpenAPI_gmlc_info_create(
    OpenAPI_list_t *serving_client_types,
    OpenAPI_list_t *gmlc_numbers
)
{
    OpenAPI_gmlc_info_t *gmlc_info_local_var = ogs_malloc(sizeof(OpenAPI_gmlc_info_t));
    ogs_assert(gmlc_info_local_var);

    gmlc_info_local_var->serving_client_types = serving_client_types;
    gmlc_info_local_var->gmlc_numbers = gmlc_numbers;

    return gmlc_info_local_var;
}

void OpenAPI_gmlc_info_free(OpenAPI_gmlc_info_t *gmlc_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == gmlc_info) {
        return;
    }
    if (gmlc_info->serving_client_types) {
        OpenAPI_list_free(gmlc_info->serving_client_types);
        gmlc_info->serving_client_types = NULL;
    }
    if (gmlc_info->gmlc_numbers) {
        OpenAPI_list_for_each(gmlc_info->gmlc_numbers, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(gmlc_info->gmlc_numbers);
        gmlc_info->gmlc_numbers = NULL;
    }
    ogs_free(gmlc_info);
}

cJSON *OpenAPI_gmlc_info_convertToJSON(OpenAPI_gmlc_info_t *gmlc_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (gmlc_info == NULL) {
        ogs_error("OpenAPI_gmlc_info_convertToJSON() failed [GmlcInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (gmlc_info->serving_client_types != OpenAPI_external_client_type_NULL) {
    cJSON *serving_client_typesList = cJSON_AddArrayToObject(item, "servingClientTypes");
    if (serving_client_typesList == NULL) {
        ogs_error("OpenAPI_gmlc_info_convertToJSON() failed [serving_client_types]");
        goto end;
    }
    OpenAPI_list_for_each(gmlc_info->serving_client_types, node) {
        if (cJSON_AddStringToObject(serving_client_typesList, "", OpenAPI_external_client_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_gmlc_info_convertToJSON() failed [serving_client_types]");
            goto end;
        }
    }
    }

    if (gmlc_info->gmlc_numbers) {
    cJSON *gmlc_numbersList = cJSON_AddArrayToObject(item, "gmlcNumbers");
    if (gmlc_numbersList == NULL) {
        ogs_error("OpenAPI_gmlc_info_convertToJSON() failed [gmlc_numbers]");
        goto end;
    }
    OpenAPI_list_for_each(gmlc_info->gmlc_numbers, node) {
        if (cJSON_AddStringToObject(gmlc_numbersList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_gmlc_info_convertToJSON() failed [gmlc_numbers]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_gmlc_info_t *OpenAPI_gmlc_info_parseFromJSON(cJSON *gmlc_infoJSON)
{
    OpenAPI_gmlc_info_t *gmlc_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *serving_client_types = NULL;
    OpenAPI_list_t *serving_client_typesList = NULL;
    cJSON *gmlc_numbers = NULL;
    OpenAPI_list_t *gmlc_numbersList = NULL;
    serving_client_types = cJSON_GetObjectItemCaseSensitive(gmlc_infoJSON, "servingClientTypes");
    if (serving_client_types) {
        cJSON *serving_client_types_local = NULL;
        if (!cJSON_IsArray(serving_client_types)) {
            ogs_error("OpenAPI_gmlc_info_parseFromJSON() failed [serving_client_types]");
            goto end;
        }

        serving_client_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(serving_client_types_local, serving_client_types) {
            OpenAPI_external_client_type_e localEnum = OpenAPI_external_client_type_NULL;
            if (!cJSON_IsString(serving_client_types_local)) {
                ogs_error("OpenAPI_gmlc_info_parseFromJSON() failed [serving_client_types]");
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
            ogs_error("OpenAPI_gmlc_info_parseFromJSON() failed: Expected serving_client_typesList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    gmlc_numbers = cJSON_GetObjectItemCaseSensitive(gmlc_infoJSON, "gmlcNumbers");
    if (gmlc_numbers) {
        cJSON *gmlc_numbers_local = NULL;
        if (!cJSON_IsArray(gmlc_numbers)) {
            ogs_error("OpenAPI_gmlc_info_parseFromJSON() failed [gmlc_numbers]");
            goto end;
        }

        gmlc_numbersList = OpenAPI_list_create();

        cJSON_ArrayForEach(gmlc_numbers_local, gmlc_numbers) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(gmlc_numbers_local)) {
                ogs_error("OpenAPI_gmlc_info_parseFromJSON() failed [gmlc_numbers]");
                goto end;
            }
            OpenAPI_list_add(gmlc_numbersList, ogs_strdup(gmlc_numbers_local->valuestring));
        }
    }

    gmlc_info_local_var = OpenAPI_gmlc_info_create (
        serving_client_types ? serving_client_typesList : NULL,
        gmlc_numbers ? gmlc_numbersList : NULL
    );

    return gmlc_info_local_var;
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

OpenAPI_gmlc_info_t *OpenAPI_gmlc_info_copy(OpenAPI_gmlc_info_t *dst, OpenAPI_gmlc_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_gmlc_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_gmlc_info_convertToJSON() failed");
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

    OpenAPI_gmlc_info_free(dst);
    dst = OpenAPI_gmlc_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

