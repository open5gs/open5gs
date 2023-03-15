
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_context_in_amf_data.h"

OpenAPI_ue_context_in_amf_data_t *OpenAPI_ue_context_in_amf_data_create(
    OpenAPI_eps_interworking_info_t *eps_interworking_info,
    OpenAPI_list_t *amf_info
)
{
    OpenAPI_ue_context_in_amf_data_t *ue_context_in_amf_data_local_var = ogs_malloc(sizeof(OpenAPI_ue_context_in_amf_data_t));
    ogs_assert(ue_context_in_amf_data_local_var);

    ue_context_in_amf_data_local_var->eps_interworking_info = eps_interworking_info;
    ue_context_in_amf_data_local_var->amf_info = amf_info;

    return ue_context_in_amf_data_local_var;
}

void OpenAPI_ue_context_in_amf_data_free(OpenAPI_ue_context_in_amf_data_t *ue_context_in_amf_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_context_in_amf_data) {
        return;
    }
    if (ue_context_in_amf_data->eps_interworking_info) {
        OpenAPI_eps_interworking_info_free(ue_context_in_amf_data->eps_interworking_info);
        ue_context_in_amf_data->eps_interworking_info = NULL;
    }
    if (ue_context_in_amf_data->amf_info) {
        OpenAPI_list_for_each(ue_context_in_amf_data->amf_info, node) {
            OpenAPI_nudm_amf_info_free(node->data);
        }
        OpenAPI_list_free(ue_context_in_amf_data->amf_info);
        ue_context_in_amf_data->amf_info = NULL;
    }
    ogs_free(ue_context_in_amf_data);
}

cJSON *OpenAPI_ue_context_in_amf_data_convertToJSON(OpenAPI_ue_context_in_amf_data_t *ue_context_in_amf_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ue_context_in_amf_data == NULL) {
        ogs_error("OpenAPI_ue_context_in_amf_data_convertToJSON() failed [UeContextInAmfData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ue_context_in_amf_data->eps_interworking_info) {
    cJSON *eps_interworking_info_local_JSON = OpenAPI_eps_interworking_info_convertToJSON(ue_context_in_amf_data->eps_interworking_info);
    if (eps_interworking_info_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_in_amf_data_convertToJSON() failed [eps_interworking_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "epsInterworkingInfo", eps_interworking_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_in_amf_data_convertToJSON() failed [eps_interworking_info]");
        goto end;
    }
    }

    if (ue_context_in_amf_data->amf_info) {
    cJSON *amf_infoList = cJSON_AddArrayToObject(item, "amfInfo");
    if (amf_infoList == NULL) {
        ogs_error("OpenAPI_ue_context_in_amf_data_convertToJSON() failed [amf_info]");
        goto end;
    }
    OpenAPI_list_for_each(ue_context_in_amf_data->amf_info, node) {
        cJSON *itemLocal = OpenAPI_nudm_amf_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ue_context_in_amf_data_convertToJSON() failed [amf_info]");
            goto end;
        }
        cJSON_AddItemToArray(amf_infoList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_ue_context_in_amf_data_t *OpenAPI_ue_context_in_amf_data_parseFromJSON(cJSON *ue_context_in_amf_dataJSON)
{
    OpenAPI_ue_context_in_amf_data_t *ue_context_in_amf_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *eps_interworking_info = NULL;
    OpenAPI_eps_interworking_info_t *eps_interworking_info_local_nonprim = NULL;
    cJSON *amf_info = NULL;
    OpenAPI_list_t *amf_infoList = NULL;
    eps_interworking_info = cJSON_GetObjectItemCaseSensitive(ue_context_in_amf_dataJSON, "epsInterworkingInfo");
    if (eps_interworking_info) {
    eps_interworking_info_local_nonprim = OpenAPI_eps_interworking_info_parseFromJSON(eps_interworking_info);
    if (!eps_interworking_info_local_nonprim) {
        ogs_error("OpenAPI_eps_interworking_info_parseFromJSON failed [eps_interworking_info]");
        goto end;
    }
    }

    amf_info = cJSON_GetObjectItemCaseSensitive(ue_context_in_amf_dataJSON, "amfInfo");
    if (amf_info) {
        cJSON *amf_info_local = NULL;
        if (!cJSON_IsArray(amf_info)) {
            ogs_error("OpenAPI_ue_context_in_amf_data_parseFromJSON() failed [amf_info]");
            goto end;
        }

        amf_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(amf_info_local, amf_info) {
            if (!cJSON_IsObject(amf_info_local)) {
                ogs_error("OpenAPI_ue_context_in_amf_data_parseFromJSON() failed [amf_info]");
                goto end;
            }
            OpenAPI_nudm_amf_info_t *amf_infoItem = OpenAPI_nudm_amf_info_parseFromJSON(amf_info_local);
            if (!amf_infoItem) {
                ogs_error("No amf_infoItem");
                goto end;
            }
            OpenAPI_list_add(amf_infoList, amf_infoItem);
        }
    }

    ue_context_in_amf_data_local_var = OpenAPI_ue_context_in_amf_data_create (
        eps_interworking_info ? eps_interworking_info_local_nonprim : NULL,
        amf_info ? amf_infoList : NULL
    );

    return ue_context_in_amf_data_local_var;
end:
    if (eps_interworking_info_local_nonprim) {
        OpenAPI_eps_interworking_info_free(eps_interworking_info_local_nonprim);
        eps_interworking_info_local_nonprim = NULL;
    }
    if (amf_infoList) {
        OpenAPI_list_for_each(amf_infoList, node) {
            OpenAPI_nudm_amf_info_free(node->data);
        }
        OpenAPI_list_free(amf_infoList);
        amf_infoList = NULL;
    }
    return NULL;
}

OpenAPI_ue_context_in_amf_data_t *OpenAPI_ue_context_in_amf_data_copy(OpenAPI_ue_context_in_amf_data_t *dst, OpenAPI_ue_context_in_amf_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_context_in_amf_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_context_in_amf_data_convertToJSON() failed");
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

    OpenAPI_ue_context_in_amf_data_free(dst);
    dst = OpenAPI_ue_context_in_amf_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

