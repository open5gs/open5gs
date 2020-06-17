
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "model_5_g_vn_group_data.h"

OpenAPI_model_5_g_vn_group_data_t *OpenAPI_model_5_g_vn_group_data_create(
    char *dnn,
    OpenAPI_snssai_t *s_nssai,
    OpenAPI_list_t *pdu_session_types,
    OpenAPI_list_t *app_descriptors
    )
{
    OpenAPI_model_5_g_vn_group_data_t *model_5_g_vn_group_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_model_5_g_vn_group_data_t));
    if (!model_5_g_vn_group_data_local_var) {
        return NULL;
    }
    model_5_g_vn_group_data_local_var->dnn = dnn;
    model_5_g_vn_group_data_local_var->s_nssai = s_nssai;
    model_5_g_vn_group_data_local_var->pdu_session_types = pdu_session_types;
    model_5_g_vn_group_data_local_var->app_descriptors = app_descriptors;

    return model_5_g_vn_group_data_local_var;
}

void OpenAPI_model_5_g_vn_group_data_free(OpenAPI_model_5_g_vn_group_data_t *model_5_g_vn_group_data)
{
    if (NULL == model_5_g_vn_group_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(model_5_g_vn_group_data->dnn);
    OpenAPI_snssai_free(model_5_g_vn_group_data->s_nssai);
    OpenAPI_list_free(model_5_g_vn_group_data->pdu_session_types);
    OpenAPI_list_for_each(model_5_g_vn_group_data->app_descriptors, node) {
        OpenAPI_app_descriptor_free(node->data);
    }
    OpenAPI_list_free(model_5_g_vn_group_data->app_descriptors);
    ogs_free(model_5_g_vn_group_data);
}

cJSON *OpenAPI_model_5_g_vn_group_data_convertToJSON(OpenAPI_model_5_g_vn_group_data_t *model_5_g_vn_group_data)
{
    cJSON *item = NULL;

    if (model_5_g_vn_group_data == NULL) {
        ogs_error("OpenAPI_model_5_g_vn_group_data_convertToJSON() failed [5GVnGroupData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!model_5_g_vn_group_data->dnn) {
        ogs_error("OpenAPI_model_5_g_vn_group_data_convertToJSON() failed [dnn]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "dnn", model_5_g_vn_group_data->dnn) == NULL) {
        ogs_error("OpenAPI_model_5_g_vn_group_data_convertToJSON() failed [dnn]");
        goto end;
    }

    if (!model_5_g_vn_group_data->s_nssai) {
        ogs_error("OpenAPI_model_5_g_vn_group_data_convertToJSON() failed [s_nssai]");
        goto end;
    }
    cJSON *s_nssai_local_JSON = OpenAPI_snssai_convertToJSON(model_5_g_vn_group_data->s_nssai);
    if (s_nssai_local_JSON == NULL) {
        ogs_error("OpenAPI_model_5_g_vn_group_data_convertToJSON() failed [s_nssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sNssai", s_nssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_model_5_g_vn_group_data_convertToJSON() failed [s_nssai]");
        goto end;
    }

    if (model_5_g_vn_group_data->pdu_session_types) {
        cJSON *pdu_session_types = cJSON_AddArrayToObject(item, "pduSessionTypes");
        if (pdu_session_types == NULL) {
            ogs_error("OpenAPI_model_5_g_vn_group_data_convertToJSON() failed [pdu_session_types]");
            goto end;
        }
        OpenAPI_lnode_t *pdu_session_types_node;
        OpenAPI_list_for_each(model_5_g_vn_group_data->pdu_session_types, pdu_session_types_node) {
            if (cJSON_AddStringToObject(pdu_session_types, "", OpenAPI_pdu_session_type_ToString((OpenAPI_pdu_session_type_e)pdu_session_types_node->data)) == NULL) {
                ogs_error("OpenAPI_model_5_g_vn_group_data_convertToJSON() failed [pdu_session_types]");
                goto end;
            }
        }
    }

    if (model_5_g_vn_group_data->app_descriptors) {
        cJSON *app_descriptorsList = cJSON_AddArrayToObject(item, "appDescriptors");
        if (app_descriptorsList == NULL) {
            ogs_error("OpenAPI_model_5_g_vn_group_data_convertToJSON() failed [app_descriptors]");
            goto end;
        }

        OpenAPI_lnode_t *app_descriptors_node;
        if (model_5_g_vn_group_data->app_descriptors) {
            OpenAPI_list_for_each(model_5_g_vn_group_data->app_descriptors, app_descriptors_node) {
                cJSON *itemLocal = OpenAPI_app_descriptor_convertToJSON(app_descriptors_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_model_5_g_vn_group_data_convertToJSON() failed [app_descriptors]");
                    goto end;
                }
                cJSON_AddItemToArray(app_descriptorsList, itemLocal);
            }
        }
    }

end:
    return item;
}

OpenAPI_model_5_g_vn_group_data_t *OpenAPI_model_5_g_vn_group_data_parseFromJSON(cJSON *model_5_g_vn_group_dataJSON)
{
    OpenAPI_model_5_g_vn_group_data_t *model_5_g_vn_group_data_local_var = NULL;
    cJSON *dnn = cJSON_GetObjectItemCaseSensitive(model_5_g_vn_group_dataJSON, "dnn");
    if (!dnn) {
        ogs_error("OpenAPI_model_5_g_vn_group_data_parseFromJSON() failed [dnn]");
        goto end;
    }


    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_model_5_g_vn_group_data_parseFromJSON() failed [dnn]");
        goto end;
    }

    cJSON *s_nssai = cJSON_GetObjectItemCaseSensitive(model_5_g_vn_group_dataJSON, "sNssai");
    if (!s_nssai) {
        ogs_error("OpenAPI_model_5_g_vn_group_data_parseFromJSON() failed [s_nssai]");
        goto end;
    }

    OpenAPI_snssai_t *s_nssai_local_nonprim = NULL;

    s_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(s_nssai);

    cJSON *pdu_session_types = cJSON_GetObjectItemCaseSensitive(model_5_g_vn_group_dataJSON, "pduSessionTypes");

    OpenAPI_list_t *pdu_session_typesList;
    if (pdu_session_types) {
        cJSON *pdu_session_types_local_nonprimitive;
        if (!cJSON_IsArray(pdu_session_types)) {
            ogs_error("OpenAPI_model_5_g_vn_group_data_parseFromJSON() failed [pdu_session_types]");
            goto end;
        }

        pdu_session_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(pdu_session_types_local_nonprimitive, pdu_session_types ) {
            if (!cJSON_IsString(pdu_session_types_local_nonprimitive)) {
                ogs_error("OpenAPI_model_5_g_vn_group_data_parseFromJSON() failed [pdu_session_types]");
                goto end;
            }

            OpenAPI_list_add(pdu_session_typesList, (void *)OpenAPI_pdu_session_type_FromString(pdu_session_types_local_nonprimitive->valuestring));
        }
    }

    cJSON *app_descriptors = cJSON_GetObjectItemCaseSensitive(model_5_g_vn_group_dataJSON, "appDescriptors");

    OpenAPI_list_t *app_descriptorsList;
    if (app_descriptors) {
        cJSON *app_descriptors_local_nonprimitive;
        if (!cJSON_IsArray(app_descriptors)) {
            ogs_error("OpenAPI_model_5_g_vn_group_data_parseFromJSON() failed [app_descriptors]");
            goto end;
        }

        app_descriptorsList = OpenAPI_list_create();

        cJSON_ArrayForEach(app_descriptors_local_nonprimitive, app_descriptors ) {
            if (!cJSON_IsObject(app_descriptors_local_nonprimitive)) {
                ogs_error("OpenAPI_model_5_g_vn_group_data_parseFromJSON() failed [app_descriptors]");
                goto end;
            }
            OpenAPI_app_descriptor_t *app_descriptorsItem = OpenAPI_app_descriptor_parseFromJSON(app_descriptors_local_nonprimitive);

            OpenAPI_list_add(app_descriptorsList, app_descriptorsItem);
        }
    }

    model_5_g_vn_group_data_local_var = OpenAPI_model_5_g_vn_group_data_create (
        ogs_strdup(dnn->valuestring),
        s_nssai_local_nonprim,
        pdu_session_types ? pdu_session_typesList : NULL,
        app_descriptors ? app_descriptorsList : NULL
        );

    return model_5_g_vn_group_data_local_var;
end:
    return NULL;
}

OpenAPI_model_5_g_vn_group_data_t *OpenAPI_model_5_g_vn_group_data_copy(OpenAPI_model_5_g_vn_group_data_t *dst, OpenAPI_model_5_g_vn_group_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_model_5_g_vn_group_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_model_5_g_vn_group_data_convertToJSON() failed");
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

    OpenAPI_model_5_g_vn_group_data_free(dst);
    dst = OpenAPI_model_5_g_vn_group_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

