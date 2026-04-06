
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dnai_eas_mapping.h"

OpenAPI_dnai_eas_mapping_t *OpenAPI_dnai_eas_mapping_create(
    OpenAPI_list_t *dnai_eas_infos
)
{
    OpenAPI_dnai_eas_mapping_t *dnai_eas_mapping_local_var = ogs_malloc(sizeof(OpenAPI_dnai_eas_mapping_t));
    ogs_assert(dnai_eas_mapping_local_var);

    dnai_eas_mapping_local_var->dnai_eas_infos = dnai_eas_infos;

    return dnai_eas_mapping_local_var;
}

void OpenAPI_dnai_eas_mapping_free(OpenAPI_dnai_eas_mapping_t *dnai_eas_mapping)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == dnai_eas_mapping) {
        return;
    }
    if (dnai_eas_mapping->dnai_eas_infos) {
        OpenAPI_list_for_each(dnai_eas_mapping->dnai_eas_infos, node) {
            OpenAPI_dnai_eas_info_free(node->data);
        }
        OpenAPI_list_free(dnai_eas_mapping->dnai_eas_infos);
        dnai_eas_mapping->dnai_eas_infos = NULL;
    }
    ogs_free(dnai_eas_mapping);
}

cJSON *OpenAPI_dnai_eas_mapping_convertToJSON(OpenAPI_dnai_eas_mapping_t *dnai_eas_mapping)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (dnai_eas_mapping == NULL) {
        ogs_error("OpenAPI_dnai_eas_mapping_convertToJSON() failed [DnaiEasMapping]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!dnai_eas_mapping->dnai_eas_infos) {
        ogs_error("OpenAPI_dnai_eas_mapping_convertToJSON() failed [dnai_eas_infos]");
        return NULL;
    }
    cJSON *dnai_eas_infosList = cJSON_AddArrayToObject(item, "dnaiEasInfos");
    if (dnai_eas_infosList == NULL) {
        ogs_error("OpenAPI_dnai_eas_mapping_convertToJSON() failed [dnai_eas_infos]");
        goto end;
    }
    OpenAPI_list_for_each(dnai_eas_mapping->dnai_eas_infos, node) {
        cJSON *itemLocal = OpenAPI_dnai_eas_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_dnai_eas_mapping_convertToJSON() failed [dnai_eas_infos]");
            goto end;
        }
        cJSON_AddItemToArray(dnai_eas_infosList, itemLocal);
    }

end:
    return item;
}

OpenAPI_dnai_eas_mapping_t *OpenAPI_dnai_eas_mapping_parseFromJSON(cJSON *dnai_eas_mappingJSON)
{
    OpenAPI_dnai_eas_mapping_t *dnai_eas_mapping_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *dnai_eas_infos = NULL;
    OpenAPI_list_t *dnai_eas_infosList = NULL;
    dnai_eas_infos = cJSON_GetObjectItemCaseSensitive(dnai_eas_mappingJSON, "dnaiEasInfos");
    if (!dnai_eas_infos) {
        ogs_error("OpenAPI_dnai_eas_mapping_parseFromJSON() failed [dnai_eas_infos]");
        goto end;
    }
        cJSON *dnai_eas_infos_local = NULL;
        if (!cJSON_IsArray(dnai_eas_infos)) {
            ogs_error("OpenAPI_dnai_eas_mapping_parseFromJSON() failed [dnai_eas_infos]");
            goto end;
        }

        dnai_eas_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnai_eas_infos_local, dnai_eas_infos) {
            if (!cJSON_IsObject(dnai_eas_infos_local)) {
                ogs_error("OpenAPI_dnai_eas_mapping_parseFromJSON() failed [dnai_eas_infos]");
                goto end;
            }
            OpenAPI_dnai_eas_info_t *dnai_eas_infosItem = OpenAPI_dnai_eas_info_parseFromJSON(dnai_eas_infos_local);
            if (!dnai_eas_infosItem) {
                ogs_error("No dnai_eas_infosItem");
                goto end;
            }
            OpenAPI_list_add(dnai_eas_infosList, dnai_eas_infosItem);
        }

    dnai_eas_mapping_local_var = OpenAPI_dnai_eas_mapping_create (
        dnai_eas_infosList
    );

    return dnai_eas_mapping_local_var;
end:
    if (dnai_eas_infosList) {
        OpenAPI_list_for_each(dnai_eas_infosList, node) {
            OpenAPI_dnai_eas_info_free(node->data);
        }
        OpenAPI_list_free(dnai_eas_infosList);
        dnai_eas_infosList = NULL;
    }
    return NULL;
}

OpenAPI_dnai_eas_mapping_t *OpenAPI_dnai_eas_mapping_copy(OpenAPI_dnai_eas_mapping_t *dst, OpenAPI_dnai_eas_mapping_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_dnai_eas_mapping_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_dnai_eas_mapping_convertToJSON() failed");
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

    OpenAPI_dnai_eas_mapping_free(dst);
    dst = OpenAPI_dnai_eas_mapping_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

