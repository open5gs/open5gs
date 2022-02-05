
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "vn_group_data.h"

OpenAPI_vn_group_data_t *OpenAPI_vn_group_data_create(
    OpenAPI_pdu_session_types_1_t *pdu_session_types,
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
    if (NULL == vn_group_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_pdu_session_types_1_free(vn_group_data->pdu_session_types);
    ogs_free(vn_group_data->dnn);
    OpenAPI_snssai_free(vn_group_data->single_nssai);
    OpenAPI_list_for_each(vn_group_data->app_descriptors, node) {
        OpenAPI_app_descriptor_free(node->data);
    }
    OpenAPI_list_free(vn_group_data->app_descriptors);
    ogs_free(vn_group_data);
}

cJSON *OpenAPI_vn_group_data_convertToJSON(OpenAPI_vn_group_data_t *vn_group_data)
{
    cJSON *item = NULL;

    if (vn_group_data == NULL) {
        ogs_error("OpenAPI_vn_group_data_convertToJSON() failed [VnGroupData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (vn_group_data->pdu_session_types) {
    cJSON *pdu_session_types_local_JSON = OpenAPI_pdu_session_types_1_convertToJSON(vn_group_data->pdu_session_types);
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

    OpenAPI_lnode_t *app_descriptors_node;
    if (vn_group_data->app_descriptors) {
        OpenAPI_list_for_each(vn_group_data->app_descriptors, app_descriptors_node) {
            cJSON *itemLocal = OpenAPI_app_descriptor_convertToJSON(app_descriptors_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_vn_group_data_convertToJSON() failed [app_descriptors]");
                goto end;
            }
            cJSON_AddItemToArray(app_descriptorsList, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_vn_group_data_t *OpenAPI_vn_group_data_parseFromJSON(cJSON *vn_group_dataJSON)
{
    OpenAPI_vn_group_data_t *vn_group_data_local_var = NULL;
    cJSON *pdu_session_types = cJSON_GetObjectItemCaseSensitive(vn_group_dataJSON, "pduSessionTypes");

    OpenAPI_pdu_session_types_1_t *pdu_session_types_local_nonprim = NULL;
    if (pdu_session_types) {
    pdu_session_types_local_nonprim = OpenAPI_pdu_session_types_1_parseFromJSON(pdu_session_types);
    }

    cJSON *dnn = cJSON_GetObjectItemCaseSensitive(vn_group_dataJSON, "dnn");

    if (dnn) {
    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_vn_group_data_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    cJSON *single_nssai = cJSON_GetObjectItemCaseSensitive(vn_group_dataJSON, "singleNssai");

    OpenAPI_snssai_t *single_nssai_local_nonprim = NULL;
    if (single_nssai) {
    single_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(single_nssai);
    }

    cJSON *app_descriptors = cJSON_GetObjectItemCaseSensitive(vn_group_dataJSON, "appDescriptors");

    OpenAPI_list_t *app_descriptorsList;
    if (app_descriptors) {
    cJSON *app_descriptors_local_nonprimitive;
    if (!cJSON_IsArray(app_descriptors)){
        ogs_error("OpenAPI_vn_group_data_parseFromJSON() failed [app_descriptors]");
        goto end;
    }

    app_descriptorsList = OpenAPI_list_create();

    cJSON_ArrayForEach(app_descriptors_local_nonprimitive, app_descriptors ) {
        if (!cJSON_IsObject(app_descriptors_local_nonprimitive)) {
            ogs_error("OpenAPI_vn_group_data_parseFromJSON() failed [app_descriptors]");
            goto end;
        }
        OpenAPI_app_descriptor_t *app_descriptorsItem = OpenAPI_app_descriptor_parseFromJSON(app_descriptors_local_nonprimitive);

        if (!app_descriptorsItem) {
            ogs_error("No app_descriptorsItem");
            OpenAPI_list_free(app_descriptorsList);
            goto end;
        }

        OpenAPI_list_add(app_descriptorsList, app_descriptorsItem);
    }
    }

    vn_group_data_local_var = OpenAPI_vn_group_data_create (
        pdu_session_types ? pdu_session_types_local_nonprim : NULL,
        dnn ? ogs_strdup(dnn->valuestring) : NULL,
        single_nssai ? single_nssai_local_nonprim : NULL,
        app_descriptors ? app_descriptorsList : NULL
    );

    return vn_group_data_local_var;
end:
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

