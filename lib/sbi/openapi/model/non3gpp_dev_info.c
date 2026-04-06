
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "non3gpp_dev_info.h"

OpenAPI_non3gpp_dev_info_t *OpenAPI_non3gpp_dev_info_create(
    char *supi,
    OpenAPI_list_t *non3gpp_dev_infos,
    char *supp_feat
)
{
    OpenAPI_non3gpp_dev_info_t *non3gpp_dev_info_local_var = ogs_malloc(sizeof(OpenAPI_non3gpp_dev_info_t));
    ogs_assert(non3gpp_dev_info_local_var);

    non3gpp_dev_info_local_var->supi = supi;
    non3gpp_dev_info_local_var->non3gpp_dev_infos = non3gpp_dev_infos;
    non3gpp_dev_info_local_var->supp_feat = supp_feat;

    return non3gpp_dev_info_local_var;
}

void OpenAPI_non3gpp_dev_info_free(OpenAPI_non3gpp_dev_info_t *non3gpp_dev_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == non3gpp_dev_info) {
        return;
    }
    if (non3gpp_dev_info->supi) {
        ogs_free(non3gpp_dev_info->supi);
        non3gpp_dev_info->supi = NULL;
    }
    if (non3gpp_dev_info->non3gpp_dev_infos) {
        OpenAPI_list_for_each(non3gpp_dev_info->non3gpp_dev_infos, node) {
            OpenAPI_non3gpp_device_information_free(node->data);
        }
        OpenAPI_list_free(non3gpp_dev_info->non3gpp_dev_infos);
        non3gpp_dev_info->non3gpp_dev_infos = NULL;
    }
    if (non3gpp_dev_info->supp_feat) {
        ogs_free(non3gpp_dev_info->supp_feat);
        non3gpp_dev_info->supp_feat = NULL;
    }
    ogs_free(non3gpp_dev_info);
}

cJSON *OpenAPI_non3gpp_dev_info_convertToJSON(OpenAPI_non3gpp_dev_info_t *non3gpp_dev_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (non3gpp_dev_info == NULL) {
        ogs_error("OpenAPI_non3gpp_dev_info_convertToJSON() failed [Non3gppDevInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!non3gpp_dev_info->supi) {
        ogs_error("OpenAPI_non3gpp_dev_info_convertToJSON() failed [supi]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "supi", non3gpp_dev_info->supi) == NULL) {
        ogs_error("OpenAPI_non3gpp_dev_info_convertToJSON() failed [supi]");
        goto end;
    }

    if (!non3gpp_dev_info->non3gpp_dev_infos) {
        ogs_error("OpenAPI_non3gpp_dev_info_convertToJSON() failed [non3gpp_dev_infos]");
        return NULL;
    }
    cJSON *non3gpp_dev_infosList = cJSON_AddArrayToObject(item, "non3gppDevInfos");
    if (non3gpp_dev_infosList == NULL) {
        ogs_error("OpenAPI_non3gpp_dev_info_convertToJSON() failed [non3gpp_dev_infos]");
        goto end;
    }
    OpenAPI_list_for_each(non3gpp_dev_info->non3gpp_dev_infos, node) {
        cJSON *itemLocal = OpenAPI_non3gpp_device_information_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_non3gpp_dev_info_convertToJSON() failed [non3gpp_dev_infos]");
            goto end;
        }
        cJSON_AddItemToArray(non3gpp_dev_infosList, itemLocal);
    }

    if (non3gpp_dev_info->supp_feat) {
    if (cJSON_AddStringToObject(item, "suppFeat", non3gpp_dev_info->supp_feat) == NULL) {
        ogs_error("OpenAPI_non3gpp_dev_info_convertToJSON() failed [supp_feat]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_non3gpp_dev_info_t *OpenAPI_non3gpp_dev_info_parseFromJSON(cJSON *non3gpp_dev_infoJSON)
{
    OpenAPI_non3gpp_dev_info_t *non3gpp_dev_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *supi = NULL;
    cJSON *non3gpp_dev_infos = NULL;
    OpenAPI_list_t *non3gpp_dev_infosList = NULL;
    cJSON *supp_feat = NULL;
    supi = cJSON_GetObjectItemCaseSensitive(non3gpp_dev_infoJSON, "supi");
    if (!supi) {
        ogs_error("OpenAPI_non3gpp_dev_info_parseFromJSON() failed [supi]");
        goto end;
    }
    if (!cJSON_IsString(supi)) {
        ogs_error("OpenAPI_non3gpp_dev_info_parseFromJSON() failed [supi]");
        goto end;
    }

    non3gpp_dev_infos = cJSON_GetObjectItemCaseSensitive(non3gpp_dev_infoJSON, "non3gppDevInfos");
    if (!non3gpp_dev_infos) {
        ogs_error("OpenAPI_non3gpp_dev_info_parseFromJSON() failed [non3gpp_dev_infos]");
        goto end;
    }
        cJSON *non3gpp_dev_infos_local = NULL;
        if (!cJSON_IsArray(non3gpp_dev_infos)) {
            ogs_error("OpenAPI_non3gpp_dev_info_parseFromJSON() failed [non3gpp_dev_infos]");
            goto end;
        }

        non3gpp_dev_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(non3gpp_dev_infos_local, non3gpp_dev_infos) {
            if (!cJSON_IsObject(non3gpp_dev_infos_local)) {
                ogs_error("OpenAPI_non3gpp_dev_info_parseFromJSON() failed [non3gpp_dev_infos]");
                goto end;
            }
            OpenAPI_non3gpp_device_information_t *non3gpp_dev_infosItem = OpenAPI_non3gpp_device_information_parseFromJSON(non3gpp_dev_infos_local);
            if (!non3gpp_dev_infosItem) {
                ogs_error("No non3gpp_dev_infosItem");
                goto end;
            }
            OpenAPI_list_add(non3gpp_dev_infosList, non3gpp_dev_infosItem);
        }

    supp_feat = cJSON_GetObjectItemCaseSensitive(non3gpp_dev_infoJSON, "suppFeat");
    if (supp_feat) {
    if (!cJSON_IsString(supp_feat) && !cJSON_IsNull(supp_feat)) {
        ogs_error("OpenAPI_non3gpp_dev_info_parseFromJSON() failed [supp_feat]");
        goto end;
    }
    }

    non3gpp_dev_info_local_var = OpenAPI_non3gpp_dev_info_create (
        ogs_strdup(supi->valuestring),
        non3gpp_dev_infosList,
        supp_feat && !cJSON_IsNull(supp_feat) ? ogs_strdup(supp_feat->valuestring) : NULL
    );

    return non3gpp_dev_info_local_var;
end:
    if (non3gpp_dev_infosList) {
        OpenAPI_list_for_each(non3gpp_dev_infosList, node) {
            OpenAPI_non3gpp_device_information_free(node->data);
        }
        OpenAPI_list_free(non3gpp_dev_infosList);
        non3gpp_dev_infosList = NULL;
    }
    return NULL;
}

OpenAPI_non3gpp_dev_info_t *OpenAPI_non3gpp_dev_info_copy(OpenAPI_non3gpp_dev_info_t *dst, OpenAPI_non3gpp_dev_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_non3gpp_dev_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_non3gpp_dev_info_convertToJSON() failed");
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

    OpenAPI_non3gpp_dev_info_free(dst);
    dst = OpenAPI_non3gpp_dev_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

