
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "adm_info.h"

OpenAPI_adm_info_t *OpenAPI_adm_info_create(
    OpenAPI_list_t *device_id_list,
    char *dev_id_reg_ex,
    OpenAPI_list_t *af_id_list
)
{
    OpenAPI_adm_info_t *adm_info_local_var = ogs_malloc(sizeof(OpenAPI_adm_info_t));
    ogs_assert(adm_info_local_var);

    adm_info_local_var->device_id_list = device_id_list;
    adm_info_local_var->dev_id_reg_ex = dev_id_reg_ex;
    adm_info_local_var->af_id_list = af_id_list;

    return adm_info_local_var;
}

void OpenAPI_adm_info_free(OpenAPI_adm_info_t *adm_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == adm_info) {
        return;
    }
    if (adm_info->device_id_list) {
        OpenAPI_list_for_each(adm_info->device_id_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(adm_info->device_id_list);
        adm_info->device_id_list = NULL;
    }
    if (adm_info->dev_id_reg_ex) {
        ogs_free(adm_info->dev_id_reg_ex);
        adm_info->dev_id_reg_ex = NULL;
    }
    if (adm_info->af_id_list) {
        OpenAPI_list_for_each(adm_info->af_id_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(adm_info->af_id_list);
        adm_info->af_id_list = NULL;
    }
    ogs_free(adm_info);
}

cJSON *OpenAPI_adm_info_convertToJSON(OpenAPI_adm_info_t *adm_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (adm_info == NULL) {
        ogs_error("OpenAPI_adm_info_convertToJSON() failed [AdmInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (adm_info->device_id_list) {
    cJSON *device_id_listList = cJSON_AddArrayToObject(item, "deviceIdList");
    if (device_id_listList == NULL) {
        ogs_error("OpenAPI_adm_info_convertToJSON() failed [device_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(adm_info->device_id_list, node) {
        if (cJSON_AddStringToObject(device_id_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_adm_info_convertToJSON() failed [device_id_list]");
            goto end;
        }
    }
    }

    if (adm_info->dev_id_reg_ex) {
    if (cJSON_AddStringToObject(item, "devIdRegEx", adm_info->dev_id_reg_ex) == NULL) {
        ogs_error("OpenAPI_adm_info_convertToJSON() failed [dev_id_reg_ex]");
        goto end;
    }
    }

    if (adm_info->af_id_list) {
    cJSON *af_id_listList = cJSON_AddArrayToObject(item, "afIdList");
    if (af_id_listList == NULL) {
        ogs_error("OpenAPI_adm_info_convertToJSON() failed [af_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(adm_info->af_id_list, node) {
        if (cJSON_AddStringToObject(af_id_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_adm_info_convertToJSON() failed [af_id_list]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_adm_info_t *OpenAPI_adm_info_parseFromJSON(cJSON *adm_infoJSON)
{
    OpenAPI_adm_info_t *adm_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *device_id_list = NULL;
    OpenAPI_list_t *device_id_listList = NULL;
    cJSON *dev_id_reg_ex = NULL;
    cJSON *af_id_list = NULL;
    OpenAPI_list_t *af_id_listList = NULL;
    device_id_list = cJSON_GetObjectItemCaseSensitive(adm_infoJSON, "deviceIdList");
    if (device_id_list) {
        cJSON *device_id_list_local = NULL;
        if (!cJSON_IsArray(device_id_list)) {
            ogs_error("OpenAPI_adm_info_parseFromJSON() failed [device_id_list]");
            goto end;
        }

        device_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(device_id_list_local, device_id_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(device_id_list_local)) {
                ogs_error("OpenAPI_adm_info_parseFromJSON() failed [device_id_list]");
                goto end;
            }
            OpenAPI_list_add(device_id_listList, ogs_strdup(device_id_list_local->valuestring));
        }
    }

    dev_id_reg_ex = cJSON_GetObjectItemCaseSensitive(adm_infoJSON, "devIdRegEx");
    if (dev_id_reg_ex) {
    if (!cJSON_IsString(dev_id_reg_ex) && !cJSON_IsNull(dev_id_reg_ex)) {
        ogs_error("OpenAPI_adm_info_parseFromJSON() failed [dev_id_reg_ex]");
        goto end;
    }
    }

    af_id_list = cJSON_GetObjectItemCaseSensitive(adm_infoJSON, "afIdList");
    if (af_id_list) {
        cJSON *af_id_list_local = NULL;
        if (!cJSON_IsArray(af_id_list)) {
            ogs_error("OpenAPI_adm_info_parseFromJSON() failed [af_id_list]");
            goto end;
        }

        af_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(af_id_list_local, af_id_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(af_id_list_local)) {
                ogs_error("OpenAPI_adm_info_parseFromJSON() failed [af_id_list]");
                goto end;
            }
            OpenAPI_list_add(af_id_listList, ogs_strdup(af_id_list_local->valuestring));
        }
    }

    adm_info_local_var = OpenAPI_adm_info_create (
        device_id_list ? device_id_listList : NULL,
        dev_id_reg_ex && !cJSON_IsNull(dev_id_reg_ex) ? ogs_strdup(dev_id_reg_ex->valuestring) : NULL,
        af_id_list ? af_id_listList : NULL
    );

    return adm_info_local_var;
end:
    if (device_id_listList) {
        OpenAPI_list_for_each(device_id_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(device_id_listList);
        device_id_listList = NULL;
    }
    if (af_id_listList) {
        OpenAPI_list_for_each(af_id_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(af_id_listList);
        af_id_listList = NULL;
    }
    return NULL;
}

OpenAPI_adm_info_t *OpenAPI_adm_info_copy(OpenAPI_adm_info_t *dst, OpenAPI_adm_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_adm_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_adm_info_convertToJSON() failed");
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

    OpenAPI_adm_info_free(dst);
    dst = OpenAPI_adm_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

