
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ecs_addr_config_info.h"

OpenAPI_ecs_addr_config_info_t *OpenAPI_ecs_addr_config_info_create(
    OpenAPI_ecs_server_addr_t *ecs_server_addr,
    OpenAPI_spatial_validity_cond_t *spatial_validity_cond,
    char *dnn,
    OpenAPI_snssai_t *snssai,
    OpenAPI_list_t *ecs_auth_methods,
    OpenAPI_list_t *supported_plmns
)
{
    OpenAPI_ecs_addr_config_info_t *ecs_addr_config_info_local_var = ogs_malloc(sizeof(OpenAPI_ecs_addr_config_info_t));
    ogs_assert(ecs_addr_config_info_local_var);

    ecs_addr_config_info_local_var->ecs_server_addr = ecs_server_addr;
    ecs_addr_config_info_local_var->spatial_validity_cond = spatial_validity_cond;
    ecs_addr_config_info_local_var->dnn = dnn;
    ecs_addr_config_info_local_var->snssai = snssai;
    ecs_addr_config_info_local_var->ecs_auth_methods = ecs_auth_methods;
    ecs_addr_config_info_local_var->supported_plmns = supported_plmns;

    return ecs_addr_config_info_local_var;
}

void OpenAPI_ecs_addr_config_info_free(OpenAPI_ecs_addr_config_info_t *ecs_addr_config_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ecs_addr_config_info) {
        return;
    }
    if (ecs_addr_config_info->ecs_server_addr) {
        OpenAPI_ecs_server_addr_free(ecs_addr_config_info->ecs_server_addr);
        ecs_addr_config_info->ecs_server_addr = NULL;
    }
    if (ecs_addr_config_info->spatial_validity_cond) {
        OpenAPI_spatial_validity_cond_free(ecs_addr_config_info->spatial_validity_cond);
        ecs_addr_config_info->spatial_validity_cond = NULL;
    }
    if (ecs_addr_config_info->dnn) {
        ogs_free(ecs_addr_config_info->dnn);
        ecs_addr_config_info->dnn = NULL;
    }
    if (ecs_addr_config_info->snssai) {
        OpenAPI_snssai_free(ecs_addr_config_info->snssai);
        ecs_addr_config_info->snssai = NULL;
    }
    if (ecs_addr_config_info->ecs_auth_methods) {
        OpenAPI_list_free(ecs_addr_config_info->ecs_auth_methods);
        ecs_addr_config_info->ecs_auth_methods = NULL;
    }
    if (ecs_addr_config_info->supported_plmns) {
        OpenAPI_list_for_each(ecs_addr_config_info->supported_plmns, node) {
            OpenAPI_supported_plmn_free(node->data);
        }
        OpenAPI_list_free(ecs_addr_config_info->supported_plmns);
        ecs_addr_config_info->supported_plmns = NULL;
    }
    ogs_free(ecs_addr_config_info);
}

cJSON *OpenAPI_ecs_addr_config_info_convertToJSON(OpenAPI_ecs_addr_config_info_t *ecs_addr_config_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ecs_addr_config_info == NULL) {
        ogs_error("OpenAPI_ecs_addr_config_info_convertToJSON() failed [EcsAddrConfigInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ecs_addr_config_info->ecs_server_addr) {
    cJSON *ecs_server_addr_local_JSON = OpenAPI_ecs_server_addr_convertToJSON(ecs_addr_config_info->ecs_server_addr);
    if (ecs_server_addr_local_JSON == NULL) {
        ogs_error("OpenAPI_ecs_addr_config_info_convertToJSON() failed [ecs_server_addr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ecsServerAddr", ecs_server_addr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ecs_addr_config_info_convertToJSON() failed [ecs_server_addr]");
        goto end;
    }
    }

    if (ecs_addr_config_info->spatial_validity_cond) {
    cJSON *spatial_validity_cond_local_JSON = OpenAPI_spatial_validity_cond_convertToJSON(ecs_addr_config_info->spatial_validity_cond);
    if (spatial_validity_cond_local_JSON == NULL) {
        ogs_error("OpenAPI_ecs_addr_config_info_convertToJSON() failed [spatial_validity_cond]");
        goto end;
    }
    cJSON_AddItemToObject(item, "spatialValidityCond", spatial_validity_cond_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ecs_addr_config_info_convertToJSON() failed [spatial_validity_cond]");
        goto end;
    }
    }

    if (ecs_addr_config_info->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", ecs_addr_config_info->dnn) == NULL) {
        ogs_error("OpenAPI_ecs_addr_config_info_convertToJSON() failed [dnn]");
        goto end;
    }
    }

    if (ecs_addr_config_info->snssai) {
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(ecs_addr_config_info->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_ecs_addr_config_info_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ecs_addr_config_info_convertToJSON() failed [snssai]");
        goto end;
    }
    }

    if (ecs_addr_config_info->ecs_auth_methods != OpenAPI_ecs_auth_method_NULL) {
    cJSON *ecs_auth_methodsList = cJSON_AddArrayToObject(item, "ecsAuthMethods");
    if (ecs_auth_methodsList == NULL) {
        ogs_error("OpenAPI_ecs_addr_config_info_convertToJSON() failed [ecs_auth_methods]");
        goto end;
    }
    OpenAPI_list_for_each(ecs_addr_config_info->ecs_auth_methods, node) {
        if (cJSON_AddStringToObject(ecs_auth_methodsList, "", OpenAPI_ecs_auth_method_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_ecs_addr_config_info_convertToJSON() failed [ecs_auth_methods]");
            goto end;
        }
    }
    }

    if (ecs_addr_config_info->supported_plmns) {
    cJSON *supported_plmnsList = cJSON_AddArrayToObject(item, "supportedPlmns");
    if (supported_plmnsList == NULL) {
        ogs_error("OpenAPI_ecs_addr_config_info_convertToJSON() failed [supported_plmns]");
        goto end;
    }
    OpenAPI_list_for_each(ecs_addr_config_info->supported_plmns, node) {
        cJSON *itemLocal = OpenAPI_supported_plmn_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ecs_addr_config_info_convertToJSON() failed [supported_plmns]");
            goto end;
        }
        cJSON_AddItemToArray(supported_plmnsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_ecs_addr_config_info_t *OpenAPI_ecs_addr_config_info_parseFromJSON(cJSON *ecs_addr_config_infoJSON)
{
    OpenAPI_ecs_addr_config_info_t *ecs_addr_config_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ecs_server_addr = NULL;
    OpenAPI_ecs_server_addr_t *ecs_server_addr_local_nonprim = NULL;
    cJSON *spatial_validity_cond = NULL;
    OpenAPI_spatial_validity_cond_t *spatial_validity_cond_local_nonprim = NULL;
    cJSON *dnn = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *ecs_auth_methods = NULL;
    OpenAPI_list_t *ecs_auth_methodsList = NULL;
    cJSON *supported_plmns = NULL;
    OpenAPI_list_t *supported_plmnsList = NULL;
    ecs_server_addr = cJSON_GetObjectItemCaseSensitive(ecs_addr_config_infoJSON, "ecsServerAddr");
    if (ecs_server_addr) {
    ecs_server_addr_local_nonprim = OpenAPI_ecs_server_addr_parseFromJSON(ecs_server_addr);
    if (!ecs_server_addr_local_nonprim) {
        ogs_error("OpenAPI_ecs_server_addr_parseFromJSON failed [ecs_server_addr]");
        goto end;
    }
    }

    spatial_validity_cond = cJSON_GetObjectItemCaseSensitive(ecs_addr_config_infoJSON, "spatialValidityCond");
    if (spatial_validity_cond) {
    spatial_validity_cond_local_nonprim = OpenAPI_spatial_validity_cond_parseFromJSON(spatial_validity_cond);
    if (!spatial_validity_cond_local_nonprim) {
        ogs_error("OpenAPI_spatial_validity_cond_parseFromJSON failed [spatial_validity_cond]");
        goto end;
    }
    }

    dnn = cJSON_GetObjectItemCaseSensitive(ecs_addr_config_infoJSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_ecs_addr_config_info_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    snssai = cJSON_GetObjectItemCaseSensitive(ecs_addr_config_infoJSON, "snssai");
    if (snssai) {
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }
    }

    ecs_auth_methods = cJSON_GetObjectItemCaseSensitive(ecs_addr_config_infoJSON, "ecsAuthMethods");
    if (ecs_auth_methods) {
        cJSON *ecs_auth_methods_local = NULL;
        if (!cJSON_IsArray(ecs_auth_methods)) {
            ogs_error("OpenAPI_ecs_addr_config_info_parseFromJSON() failed [ecs_auth_methods]");
            goto end;
        }

        ecs_auth_methodsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ecs_auth_methods_local, ecs_auth_methods) {
            OpenAPI_ecs_auth_method_e localEnum = OpenAPI_ecs_auth_method_NULL;
            if (!cJSON_IsString(ecs_auth_methods_local)) {
                ogs_error("OpenAPI_ecs_addr_config_info_parseFromJSON() failed [ecs_auth_methods]");
                goto end;
            }
            localEnum = OpenAPI_ecs_auth_method_FromString(ecs_auth_methods_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"ecs_auth_methods\" is not supported. Ignoring it ...",
                         ecs_auth_methods_local->valuestring);
            } else {
                OpenAPI_list_add(ecs_auth_methodsList, (void *)localEnum);
            }
        }
        if (ecs_auth_methodsList->count == 0) {
            ogs_error("OpenAPI_ecs_addr_config_info_parseFromJSON() failed: Expected ecs_auth_methodsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    supported_plmns = cJSON_GetObjectItemCaseSensitive(ecs_addr_config_infoJSON, "supportedPlmns");
    if (supported_plmns) {
        cJSON *supported_plmns_local = NULL;
        if (!cJSON_IsArray(supported_plmns)) {
            ogs_error("OpenAPI_ecs_addr_config_info_parseFromJSON() failed [supported_plmns]");
            goto end;
        }

        supported_plmnsList = OpenAPI_list_create();

        cJSON_ArrayForEach(supported_plmns_local, supported_plmns) {
            if (!cJSON_IsObject(supported_plmns_local)) {
                ogs_error("OpenAPI_ecs_addr_config_info_parseFromJSON() failed [supported_plmns]");
                goto end;
            }
            OpenAPI_supported_plmn_t *supported_plmnsItem = OpenAPI_supported_plmn_parseFromJSON(supported_plmns_local);
            if (!supported_plmnsItem) {
                ogs_error("No supported_plmnsItem");
                goto end;
            }
            OpenAPI_list_add(supported_plmnsList, supported_plmnsItem);
        }
    }

    ecs_addr_config_info_local_var = OpenAPI_ecs_addr_config_info_create (
        ecs_server_addr ? ecs_server_addr_local_nonprim : NULL,
        spatial_validity_cond ? spatial_validity_cond_local_nonprim : NULL,
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL,
        snssai ? snssai_local_nonprim : NULL,
        ecs_auth_methods ? ecs_auth_methodsList : NULL,
        supported_plmns ? supported_plmnsList : NULL
    );

    return ecs_addr_config_info_local_var;
end:
    if (ecs_server_addr_local_nonprim) {
        OpenAPI_ecs_server_addr_free(ecs_server_addr_local_nonprim);
        ecs_server_addr_local_nonprim = NULL;
    }
    if (spatial_validity_cond_local_nonprim) {
        OpenAPI_spatial_validity_cond_free(spatial_validity_cond_local_nonprim);
        spatial_validity_cond_local_nonprim = NULL;
    }
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    if (ecs_auth_methodsList) {
        OpenAPI_list_free(ecs_auth_methodsList);
        ecs_auth_methodsList = NULL;
    }
    if (supported_plmnsList) {
        OpenAPI_list_for_each(supported_plmnsList, node) {
            OpenAPI_supported_plmn_free(node->data);
        }
        OpenAPI_list_free(supported_plmnsList);
        supported_plmnsList = NULL;
    }
    return NULL;
}

OpenAPI_ecs_addr_config_info_t *OpenAPI_ecs_addr_config_info_copy(OpenAPI_ecs_addr_config_info_t *dst, OpenAPI_ecs_addr_config_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ecs_addr_config_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ecs_addr_config_info_convertToJSON() failed");
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

    OpenAPI_ecs_addr_config_info_free(dst);
    dst = OpenAPI_ecs_addr_config_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

