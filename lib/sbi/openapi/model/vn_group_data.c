
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "vn_group_data.h"

OpenAPI_vn_group_data_t *OpenAPI_vn_group_data_create(
    OpenAPI_pdu_session_types_t *pdu_session_types,
    char *dnn,
    OpenAPI_snssai_t *single_nssai,
    OpenAPI_list_t *app_descriptors
)
{
    OpenAPI_vn_group_data_t *vn_group_data_local_var = ogs_malloc(sizeof(OpenAPI_vn_group_data_t));
    ogs_assert(vn_group_data_local_var);

    vn_group_data_local_var->pdu_session_types = pdu_session_types;
    vn_group_data_local_var->dnn = dnn;
    vn_group_data_local_var->single_nssai = single_nssai;
    vn_group_data_local_var->app_descriptors = app_descriptors;

    return vn_group_data_local_var;
}

void OpenAPI_vn_group_data_free(OpenAPI_vn_group_data_t *vn_group_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == vn_group_data) {
        return;
    }
    if (vn_group_data->pdu_session_types) {
        OpenAPI_pdu_session_types_free(vn_group_data->pdu_session_types);
        vn_group_data->pdu_session_types = NULL;
    }
    if (vn_group_data->dnn) {
        ogs_free(vn_group_data->dnn);
        vn_group_data->dnn = NULL;
    }
    if (vn_group_data->single_nssai) {
        OpenAPI_snssai_free(vn_group_data->single_nssai);
        vn_group_data->single_nssai = NULL;
    }
    if (vn_group_data->app_descriptors) {
        OpenAPI_list_for_each(vn_group_data->app_descriptors, node) {
            OpenAPI_app_descriptor_free(node->data);
        }
        OpenAPI_list_free(vn_group_data->app_descriptors);
        vn_group_data->app_descriptors = NULL;
    }
    ogs_free(vn_group_data);
}

cJSON *OpenAPI_vn_group_data_convertToJSON(OpenAPI_vn_group_data_t *vn_group_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (vn_group_data == NULL) {
        ogs_error("OpenAPI_vn_group_data_convertToJSON() failed [VnGroupData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (vn_group_data->pdu_session_types) {
    cJSON *pdu_session_types_local_JSON = OpenAPI_pdu_session_types_convertToJSON(vn_group_data->pdu_session_types);
    if (pdu_session_types_local_JSON == NULL) {
        ogs_error("OpenAPI_vn_group_data_convertToJSON() failed [pdu_session_types]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pduSessionTypes", pdu_session_types_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_vn_group_data_convertToJSON() failed [pdu_session_types]");
        goto end;
    }
    }

    if (vn_group_data->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", vn_group_data->dnn) == NULL) {
        ogs_error("OpenAPI_vn_group_data_convertToJSON() failed [dnn]");
        goto end;
    }
    }

    if (vn_group_data->single_nssai) {
    cJSON *single_nssai_local_JSON = OpenAPI_snssai_convertToJSON(vn_group_data->single_nssai);
    if (single_nssai_local_JSON == NULL) {
        ogs_error("OpenAPI_vn_group_data_convertToJSON() failed [single_nssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "singleNssai", single_nssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_vn_group_data_convertToJSON() failed [single_nssai]");
        goto end;
    }
    }

    if (vn_group_data->app_descriptors) {
    cJSON *app_descriptorsList = cJSON_AddArrayToObject(item, "appDescriptors");
    if (app_descriptorsList == NULL) {
        ogs_error("OpenAPI_vn_group_data_convertToJSON() failed [app_descriptors]");
        goto end;
    }
    OpenAPI_list_for_each(vn_group_data->app_descriptors, node) {
        cJSON *itemLocal = OpenAPI_app_descriptor_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_vn_group_data_convertToJSON() failed [app_descriptors]");
            goto end;
        }
        cJSON_AddItemToArray(app_descriptorsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_vn_group_data_t *OpenAPI_vn_group_data_parseFromJSON(cJSON *vn_group_dataJSON)
{
    OpenAPI_vn_group_data_t *vn_group_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *pdu_session_types = NULL;
    OpenAPI_pdu_session_types_t *pdu_session_types_local_nonprim = NULL;
    cJSON *dnn = NULL;
    cJSON *single_nssai = NULL;
    OpenAPI_snssai_t *single_nssai_local_nonprim = NULL;
    cJSON *app_descriptors = NULL;
    OpenAPI_list_t *app_descriptorsList = NULL;
    pdu_session_types = cJSON_GetObjectItemCaseSensitive(vn_group_dataJSON, "pduSessionTypes");
    if (pdu_session_types) {
    pdu_session_types_local_nonprim = OpenAPI_pdu_session_types_parseFromJSON(pdu_session_types);
    if (!pdu_session_types_local_nonprim) {
        ogs_error("OpenAPI_pdu_session_types_parseFromJSON failed [pdu_session_types]");
        goto end;
    }
    }

    dnn = cJSON_GetObjectItemCaseSensitive(vn_group_dataJSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_vn_group_data_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    single_nssai = cJSON_GetObjectItemCaseSensitive(vn_group_dataJSON, "singleNssai");
    if (single_nssai) {
    single_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(single_nssai);
    if (!single_nssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [single_nssai]");
        goto end;
    }
    }

    app_descriptors = cJSON_GetObjectItemCaseSensitive(vn_group_dataJSON, "appDescriptors");
    if (app_descriptors) {
        cJSON *app_descriptors_local = NULL;
        if (!cJSON_IsArray(app_descriptors)) {
            ogs_error("OpenAPI_vn_group_data_parseFromJSON() failed [app_descriptors]");
            goto end;
        }

        app_descriptorsList = OpenAPI_list_create();

        cJSON_ArrayForEach(app_descriptors_local, app_descriptors) {
            if (!cJSON_IsObject(app_descriptors_local)) {
                ogs_error("OpenAPI_vn_group_data_parseFromJSON() failed [app_descriptors]");
                goto end;
            }
            OpenAPI_app_descriptor_t *app_descriptorsItem = OpenAPI_app_descriptor_parseFromJSON(app_descriptors_local);
            if (!app_descriptorsItem) {
                ogs_error("No app_descriptorsItem");
                goto end;
            }
            OpenAPI_list_add(app_descriptorsList, app_descriptorsItem);
        }
    }

    vn_group_data_local_var = OpenAPI_vn_group_data_create (
        pdu_session_types ? pdu_session_types_local_nonprim : NULL,
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL,
        single_nssai ? single_nssai_local_nonprim : NULL,
        app_descriptors ? app_descriptorsList : NULL
    );

    return vn_group_data_local_var;
end:
    if (pdu_session_types_local_nonprim) {
        OpenAPI_pdu_session_types_free(pdu_session_types_local_nonprim);
        pdu_session_types_local_nonprim = NULL;
    }
    if (single_nssai_local_nonprim) {
        OpenAPI_snssai_free(single_nssai_local_nonprim);
        single_nssai_local_nonprim = NULL;
    }
    if (app_descriptorsList) {
        OpenAPI_list_for_each(app_descriptorsList, node) {
            OpenAPI_app_descriptor_free(node->data);
        }
        OpenAPI_list_free(app_descriptorsList);
        app_descriptorsList = NULL;
    }
    return NULL;
}

OpenAPI_vn_group_data_t *OpenAPI_vn_group_data_copy(OpenAPI_vn_group_data_t *dst, OpenAPI_vn_group_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_vn_group_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_vn_group_data_convertToJSON() failed");
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

    OpenAPI_vn_group_data_free(dst);
    dst = OpenAPI_vn_group_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

