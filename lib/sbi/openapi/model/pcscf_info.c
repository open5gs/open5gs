
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pcscf_info.h"

OpenAPI_pcscf_info_t *OpenAPI_pcscf_info_create(
    OpenAPI_list_t *access_type,
    OpenAPI_list_t *dnn_list
    )
{
    OpenAPI_pcscf_info_t *pcscf_info_local_var = OpenAPI_malloc(sizeof(OpenAPI_pcscf_info_t));
    if (!pcscf_info_local_var) {
        return NULL;
    }
    pcscf_info_local_var->access_type = access_type;
    pcscf_info_local_var->dnn_list = dnn_list;

    return pcscf_info_local_var;
}

void OpenAPI_pcscf_info_free(OpenAPI_pcscf_info_t *pcscf_info)
{
    if (NULL == pcscf_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_free(pcscf_info->access_type);
    OpenAPI_list_for_each(pcscf_info->dnn_list, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(pcscf_info->dnn_list);
    ogs_free(pcscf_info);
}

cJSON *OpenAPI_pcscf_info_convertToJSON(OpenAPI_pcscf_info_t *pcscf_info)
{
    cJSON *item = NULL;

    if (pcscf_info == NULL) {
        ogs_error("OpenAPI_pcscf_info_convertToJSON() failed [PcscfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pcscf_info->access_type) {
        cJSON *access_type = cJSON_AddArrayToObject(item, "accessType");
        if (access_type == NULL) {
            ogs_error("OpenAPI_pcscf_info_convertToJSON() failed [access_type]");
            goto end;
        }
        OpenAPI_lnode_t *access_type_node;
        OpenAPI_list_for_each(pcscf_info->access_type, access_type_node) {
            if (cJSON_AddStringToObject(access_type, "", OpenAPI_access_type_ToString((OpenAPI_access_type_e)access_type_node->data)) == NULL) {
                ogs_error("OpenAPI_pcscf_info_convertToJSON() failed [access_type]");
                goto end;
            }
        }
    }

    if (pcscf_info->dnn_list) {
        cJSON *dnn_list = cJSON_AddArrayToObject(item, "dnnList");
        if (dnn_list == NULL) {
            ogs_error("OpenAPI_pcscf_info_convertToJSON() failed [dnn_list]");
            goto end;
        }

        OpenAPI_lnode_t *dnn_list_node;
        OpenAPI_list_for_each(pcscf_info->dnn_list, dnn_list_node)  {
            if (cJSON_AddStringToObject(dnn_list, "", (char*)dnn_list_node->data) == NULL) {
                ogs_error("OpenAPI_pcscf_info_convertToJSON() failed [dnn_list]");
                goto end;
            }
        }
    }

end:
    return item;
}

OpenAPI_pcscf_info_t *OpenAPI_pcscf_info_parseFromJSON(cJSON *pcscf_infoJSON)
{
    OpenAPI_pcscf_info_t *pcscf_info_local_var = NULL;
    cJSON *access_type = cJSON_GetObjectItemCaseSensitive(pcscf_infoJSON, "accessType");

    OpenAPI_list_t *access_typeList;
    if (access_type) {
        cJSON *access_type_local_nonprimitive;
        if (!cJSON_IsArray(access_type)) {
            ogs_error("OpenAPI_pcscf_info_parseFromJSON() failed [access_type]");
            goto end;
        }

        access_typeList = OpenAPI_list_create();

        cJSON_ArrayForEach(access_type_local_nonprimitive, access_type ) {
            if (!cJSON_IsString(access_type_local_nonprimitive)) {
                ogs_error("OpenAPI_pcscf_info_parseFromJSON() failed [access_type]");
                goto end;
            }

            OpenAPI_list_add(access_typeList, (void *)OpenAPI_access_type_FromString(access_type_local_nonprimitive->valuestring));
        }
    }

    cJSON *dnn_list = cJSON_GetObjectItemCaseSensitive(pcscf_infoJSON, "dnnList");

    OpenAPI_list_t *dnn_listList;
    if (dnn_list) {
        cJSON *dnn_list_local;
        if (!cJSON_IsArray(dnn_list)) {
            ogs_error("OpenAPI_pcscf_info_parseFromJSON() failed [dnn_list]");
            goto end;
        }
        dnn_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnn_list_local, dnn_list) {
            if (!cJSON_IsString(dnn_list_local)) {
                ogs_error("OpenAPI_pcscf_info_parseFromJSON() failed [dnn_list]");
                goto end;
            }
            OpenAPI_list_add(dnn_listList, ogs_strdup(dnn_list_local->valuestring));
        }
    }

    pcscf_info_local_var = OpenAPI_pcscf_info_create (
        access_type ? access_typeList : NULL,
        dnn_list ? dnn_listList : NULL
        );

    return pcscf_info_local_var;
end:
    return NULL;
}

OpenAPI_pcscf_info_t *OpenAPI_pcscf_info_copy(OpenAPI_pcscf_info_t *dst, OpenAPI_pcscf_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pcscf_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pcscf_info_convertToJSON() failed");
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

    OpenAPI_pcscf_info_free(dst);
    dst = OpenAPI_pcscf_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

