
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "non_ue_n2_info_subscription_create_data.h"

OpenAPI_non_ue_n2_info_subscription_create_data_t *OpenAPI_non_ue_n2_info_subscription_create_data_create(
    OpenAPI_list_t *global_ran_node_list,
    OpenAPI_list_t *an_type_list,
    OpenAPI_n2_information_class_e n2_information_class,
    char *n2_notify_callback_uri,
    char *nf_id,
    char *supported_features
    )
{
    OpenAPI_non_ue_n2_info_subscription_create_data_t *non_ue_n2_info_subscription_create_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_non_ue_n2_info_subscription_create_data_t));
    if (!non_ue_n2_info_subscription_create_data_local_var) {
        return NULL;
    }
    non_ue_n2_info_subscription_create_data_local_var->global_ran_node_list = global_ran_node_list;
    non_ue_n2_info_subscription_create_data_local_var->an_type_list = an_type_list;
    non_ue_n2_info_subscription_create_data_local_var->n2_information_class = n2_information_class;
    non_ue_n2_info_subscription_create_data_local_var->n2_notify_callback_uri = n2_notify_callback_uri;
    non_ue_n2_info_subscription_create_data_local_var->nf_id = nf_id;
    non_ue_n2_info_subscription_create_data_local_var->supported_features = supported_features;

    return non_ue_n2_info_subscription_create_data_local_var;
}

void OpenAPI_non_ue_n2_info_subscription_create_data_free(OpenAPI_non_ue_n2_info_subscription_create_data_t *non_ue_n2_info_subscription_create_data)
{
    if (NULL == non_ue_n2_info_subscription_create_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(non_ue_n2_info_subscription_create_data->global_ran_node_list, node) {
        OpenAPI_global_ran_node_id_free(node->data);
    }
    OpenAPI_list_free(non_ue_n2_info_subscription_create_data->global_ran_node_list);
    OpenAPI_list_free(non_ue_n2_info_subscription_create_data->an_type_list);
    ogs_free(non_ue_n2_info_subscription_create_data->n2_notify_callback_uri);
    ogs_free(non_ue_n2_info_subscription_create_data->nf_id);
    ogs_free(non_ue_n2_info_subscription_create_data->supported_features);
    ogs_free(non_ue_n2_info_subscription_create_data);
}

cJSON *OpenAPI_non_ue_n2_info_subscription_create_data_convertToJSON(OpenAPI_non_ue_n2_info_subscription_create_data_t *non_ue_n2_info_subscription_create_data)
{
    cJSON *item = NULL;

    if (non_ue_n2_info_subscription_create_data == NULL) {
        ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_convertToJSON() failed [NonUeN2InfoSubscriptionCreateData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (non_ue_n2_info_subscription_create_data->global_ran_node_list) {
        cJSON *global_ran_node_listList = cJSON_AddArrayToObject(item, "globalRanNodeList");
        if (global_ran_node_listList == NULL) {
            ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_convertToJSON() failed [global_ran_node_list]");
            goto end;
        }

        OpenAPI_lnode_t *global_ran_node_list_node;
        if (non_ue_n2_info_subscription_create_data->global_ran_node_list) {
            OpenAPI_list_for_each(non_ue_n2_info_subscription_create_data->global_ran_node_list, global_ran_node_list_node) {
                cJSON *itemLocal = OpenAPI_global_ran_node_id_convertToJSON(global_ran_node_list_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_convertToJSON() failed [global_ran_node_list]");
                    goto end;
                }
                cJSON_AddItemToArray(global_ran_node_listList, itemLocal);
            }
        }
    }

    if (non_ue_n2_info_subscription_create_data->an_type_list) {
        cJSON *an_type_list = cJSON_AddArrayToObject(item, "anTypeList");
        if (an_type_list == NULL) {
            ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_convertToJSON() failed [an_type_list]");
            goto end;
        }
        OpenAPI_lnode_t *an_type_list_node;
        OpenAPI_list_for_each(non_ue_n2_info_subscription_create_data->an_type_list, an_type_list_node) {
            if (cJSON_AddStringToObject(an_type_list, "", OpenAPI_access_type_ToString((OpenAPI_access_type_e)an_type_list_node->data)) == NULL) {
                ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_convertToJSON() failed [an_type_list]");
                goto end;
            }
        }
    }

    if (!non_ue_n2_info_subscription_create_data->n2_information_class) {
        ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_convertToJSON() failed [n2_information_class]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "n2InformationClass", OpenAPI_n2_information_class_ToString(non_ue_n2_info_subscription_create_data->n2_information_class)) == NULL) {
        ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_convertToJSON() failed [n2_information_class]");
        goto end;
    }

    if (!non_ue_n2_info_subscription_create_data->n2_notify_callback_uri) {
        ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_convertToJSON() failed [n2_notify_callback_uri]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "n2NotifyCallbackUri", non_ue_n2_info_subscription_create_data->n2_notify_callback_uri) == NULL) {
        ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_convertToJSON() failed [n2_notify_callback_uri]");
        goto end;
    }

    if (non_ue_n2_info_subscription_create_data->nf_id) {
        if (cJSON_AddStringToObject(item, "nfId", non_ue_n2_info_subscription_create_data->nf_id) == NULL) {
            ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_convertToJSON() failed [nf_id]");
            goto end;
        }
    }

    if (non_ue_n2_info_subscription_create_data->supported_features) {
        if (cJSON_AddStringToObject(item, "supportedFeatures", non_ue_n2_info_subscription_create_data->supported_features) == NULL) {
            ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_convertToJSON() failed [supported_features]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_non_ue_n2_info_subscription_create_data_t *OpenAPI_non_ue_n2_info_subscription_create_data_parseFromJSON(cJSON *non_ue_n2_info_subscription_create_dataJSON)
{
    OpenAPI_non_ue_n2_info_subscription_create_data_t *non_ue_n2_info_subscription_create_data_local_var = NULL;
    cJSON *global_ran_node_list = cJSON_GetObjectItemCaseSensitive(non_ue_n2_info_subscription_create_dataJSON, "globalRanNodeList");

    OpenAPI_list_t *global_ran_node_listList;
    if (global_ran_node_list) {
        cJSON *global_ran_node_list_local_nonprimitive;
        if (!cJSON_IsArray(global_ran_node_list)) {
            ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_parseFromJSON() failed [global_ran_node_list]");
            goto end;
        }

        global_ran_node_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(global_ran_node_list_local_nonprimitive, global_ran_node_list ) {
            if (!cJSON_IsObject(global_ran_node_list_local_nonprimitive)) {
                ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_parseFromJSON() failed [global_ran_node_list]");
                goto end;
            }
            OpenAPI_global_ran_node_id_t *global_ran_node_listItem = OpenAPI_global_ran_node_id_parseFromJSON(global_ran_node_list_local_nonprimitive);

            OpenAPI_list_add(global_ran_node_listList, global_ran_node_listItem);
        }
    }

    cJSON *an_type_list = cJSON_GetObjectItemCaseSensitive(non_ue_n2_info_subscription_create_dataJSON, "anTypeList");

    OpenAPI_list_t *an_type_listList;
    if (an_type_list) {
        cJSON *an_type_list_local_nonprimitive;
        if (!cJSON_IsArray(an_type_list)) {
            ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_parseFromJSON() failed [an_type_list]");
            goto end;
        }

        an_type_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(an_type_list_local_nonprimitive, an_type_list ) {
            if (!cJSON_IsString(an_type_list_local_nonprimitive)) {
                ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_parseFromJSON() failed [an_type_list]");
                goto end;
            }

            OpenAPI_list_add(an_type_listList, (void *)OpenAPI_access_type_FromString(an_type_list_local_nonprimitive->valuestring));
        }
    }

    cJSON *n2_information_class = cJSON_GetObjectItemCaseSensitive(non_ue_n2_info_subscription_create_dataJSON, "n2InformationClass");
    if (!n2_information_class) {
        ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_parseFromJSON() failed [n2_information_class]");
        goto end;
    }

    OpenAPI_n2_information_class_e n2_information_classVariable;

    if (!cJSON_IsString(n2_information_class)) {
        ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_parseFromJSON() failed [n2_information_class]");
        goto end;
    }
    n2_information_classVariable = OpenAPI_n2_information_class_FromString(n2_information_class->valuestring);

    cJSON *n2_notify_callback_uri = cJSON_GetObjectItemCaseSensitive(non_ue_n2_info_subscription_create_dataJSON, "n2NotifyCallbackUri");
    if (!n2_notify_callback_uri) {
        ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_parseFromJSON() failed [n2_notify_callback_uri]");
        goto end;
    }


    if (!cJSON_IsString(n2_notify_callback_uri)) {
        ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_parseFromJSON() failed [n2_notify_callback_uri]");
        goto end;
    }

    cJSON *nf_id = cJSON_GetObjectItemCaseSensitive(non_ue_n2_info_subscription_create_dataJSON, "nfId");

    if (nf_id) {
        if (!cJSON_IsString(nf_id)) {
            ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_parseFromJSON() failed [nf_id]");
            goto end;
        }
    }

    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(non_ue_n2_info_subscription_create_dataJSON, "supportedFeatures");

    if (supported_features) {
        if (!cJSON_IsString(supported_features)) {
            ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_parseFromJSON() failed [supported_features]");
            goto end;
        }
    }

    non_ue_n2_info_subscription_create_data_local_var = OpenAPI_non_ue_n2_info_subscription_create_data_create (
        global_ran_node_list ? global_ran_node_listList : NULL,
        an_type_list ? an_type_listList : NULL,
        n2_information_classVariable,
        ogs_strdup(n2_notify_callback_uri->valuestring),
        nf_id ? ogs_strdup(nf_id->valuestring) : NULL,
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL
        );

    return non_ue_n2_info_subscription_create_data_local_var;
end:
    return NULL;
}

OpenAPI_non_ue_n2_info_subscription_create_data_t *OpenAPI_non_ue_n2_info_subscription_create_data_copy(OpenAPI_non_ue_n2_info_subscription_create_data_t *dst, OpenAPI_non_ue_n2_info_subscription_create_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_non_ue_n2_info_subscription_create_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_convertToJSON() failed");
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

    OpenAPI_non_ue_n2_info_subscription_create_data_free(dst);
    dst = OpenAPI_non_ue_n2_info_subscription_create_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

