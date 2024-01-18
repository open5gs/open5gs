
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lcs_mo_data.h"

OpenAPI_lcs_mo_data_t *OpenAPI_lcs_mo_data_create(
    OpenAPI_list_t *allowed_service_classes,
    OpenAPI_lcs_broadcast_assistance_types_data_t *mo_assistance_data_types
)
{
    OpenAPI_lcs_mo_data_t *lcs_mo_data_local_var = ogs_malloc(sizeof(OpenAPI_lcs_mo_data_t));
    ogs_assert(lcs_mo_data_local_var);

    lcs_mo_data_local_var->allowed_service_classes = allowed_service_classes;
    lcs_mo_data_local_var->mo_assistance_data_types = mo_assistance_data_types;

    return lcs_mo_data_local_var;
}

void OpenAPI_lcs_mo_data_free(OpenAPI_lcs_mo_data_t *lcs_mo_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == lcs_mo_data) {
        return;
    }
    if (lcs_mo_data->allowed_service_classes) {
        OpenAPI_list_free(lcs_mo_data->allowed_service_classes);
        lcs_mo_data->allowed_service_classes = NULL;
    }
    if (lcs_mo_data->mo_assistance_data_types) {
        OpenAPI_lcs_broadcast_assistance_types_data_free(lcs_mo_data->mo_assistance_data_types);
        lcs_mo_data->mo_assistance_data_types = NULL;
    }
    ogs_free(lcs_mo_data);
}

cJSON *OpenAPI_lcs_mo_data_convertToJSON(OpenAPI_lcs_mo_data_t *lcs_mo_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (lcs_mo_data == NULL) {
        ogs_error("OpenAPI_lcs_mo_data_convertToJSON() failed [LcsMoData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (lcs_mo_data->allowed_service_classes == OpenAPI_lcs_mo_service_class_NULL) {
        ogs_error("OpenAPI_lcs_mo_data_convertToJSON() failed [allowed_service_classes]");
        return NULL;
    }
    cJSON *allowed_service_classesList = cJSON_AddArrayToObject(item, "allowedServiceClasses");
    if (allowed_service_classesList == NULL) {
        ogs_error("OpenAPI_lcs_mo_data_convertToJSON() failed [allowed_service_classes]");
        goto end;
    }
    OpenAPI_list_for_each(lcs_mo_data->allowed_service_classes, node) {
        if (cJSON_AddStringToObject(allowed_service_classesList, "", OpenAPI_lcs_mo_service_class_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_lcs_mo_data_convertToJSON() failed [allowed_service_classes]");
            goto end;
        }
    }

    if (lcs_mo_data->mo_assistance_data_types) {
    cJSON *mo_assistance_data_types_local_JSON = OpenAPI_lcs_broadcast_assistance_types_data_convertToJSON(lcs_mo_data->mo_assistance_data_types);
    if (mo_assistance_data_types_local_JSON == NULL) {
        ogs_error("OpenAPI_lcs_mo_data_convertToJSON() failed [mo_assistance_data_types]");
        goto end;
    }
    cJSON_AddItemToObject(item, "moAssistanceDataTypes", mo_assistance_data_types_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_lcs_mo_data_convertToJSON() failed [mo_assistance_data_types]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_lcs_mo_data_t *OpenAPI_lcs_mo_data_parseFromJSON(cJSON *lcs_mo_dataJSON)
{
    OpenAPI_lcs_mo_data_t *lcs_mo_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *allowed_service_classes = NULL;
    OpenAPI_list_t *allowed_service_classesList = NULL;
    cJSON *mo_assistance_data_types = NULL;
    OpenAPI_lcs_broadcast_assistance_types_data_t *mo_assistance_data_types_local_nonprim = NULL;
    allowed_service_classes = cJSON_GetObjectItemCaseSensitive(lcs_mo_dataJSON, "allowedServiceClasses");
    if (!allowed_service_classes) {
        ogs_error("OpenAPI_lcs_mo_data_parseFromJSON() failed [allowed_service_classes]");
        goto end;
    }
        cJSON *allowed_service_classes_local = NULL;
        if (!cJSON_IsArray(allowed_service_classes)) {
            ogs_error("OpenAPI_lcs_mo_data_parseFromJSON() failed [allowed_service_classes]");
            goto end;
        }

        allowed_service_classesList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_service_classes_local, allowed_service_classes) {
            OpenAPI_lcs_mo_service_class_e localEnum = OpenAPI_lcs_mo_service_class_NULL;
            if (!cJSON_IsString(allowed_service_classes_local)) {
                ogs_error("OpenAPI_lcs_mo_data_parseFromJSON() failed [allowed_service_classes]");
                goto end;
            }
            localEnum = OpenAPI_lcs_mo_service_class_FromString(allowed_service_classes_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"allowed_service_classes\" is not supported. Ignoring it ...",
                         allowed_service_classes_local->valuestring);
            } else {
                OpenAPI_list_add(allowed_service_classesList, (void *)localEnum);
            }
        }
        if (allowed_service_classesList->count == 0) {
            ogs_error("OpenAPI_lcs_mo_data_parseFromJSON() failed: Expected allowed_service_classesList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }

    mo_assistance_data_types = cJSON_GetObjectItemCaseSensitive(lcs_mo_dataJSON, "moAssistanceDataTypes");
    if (mo_assistance_data_types) {
    mo_assistance_data_types_local_nonprim = OpenAPI_lcs_broadcast_assistance_types_data_parseFromJSON(mo_assistance_data_types);
    if (!mo_assistance_data_types_local_nonprim) {
        ogs_error("OpenAPI_lcs_broadcast_assistance_types_data_parseFromJSON failed [mo_assistance_data_types]");
        goto end;
    }
    }

    lcs_mo_data_local_var = OpenAPI_lcs_mo_data_create (
        allowed_service_classesList,
        mo_assistance_data_types ? mo_assistance_data_types_local_nonprim : NULL
    );

    return lcs_mo_data_local_var;
end:
    if (allowed_service_classesList) {
        OpenAPI_list_free(allowed_service_classesList);
        allowed_service_classesList = NULL;
    }
    if (mo_assistance_data_types_local_nonprim) {
        OpenAPI_lcs_broadcast_assistance_types_data_free(mo_assistance_data_types_local_nonprim);
        mo_assistance_data_types_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_lcs_mo_data_t *OpenAPI_lcs_mo_data_copy(OpenAPI_lcs_mo_data_t *dst, OpenAPI_lcs_mo_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_lcs_mo_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_lcs_mo_data_convertToJSON() failed");
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

    OpenAPI_lcs_mo_data_free(dst);
    dst = OpenAPI_lcs_mo_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

