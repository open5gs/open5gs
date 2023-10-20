
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
    OpenAPI_non_ue_n2_info_subscription_create_data_t *non_ue_n2_info_subscription_create_data_local_var = ogs_malloc(sizeof(OpenAPI_non_ue_n2_info_subscription_create_data_t));
    ogs_assert(non_ue_n2_info_subscription_create_data_local_var);

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
    OpenAPI_lnode_t *node = NULL;

    if (NULL == non_ue_n2_info_subscription_create_data) {
        return;
    }
    if (non_ue_n2_info_subscription_create_data->global_ran_node_list) {
        OpenAPI_list_for_each(non_ue_n2_info_subscription_create_data->global_ran_node_list, node) {
            OpenAPI_global_ran_node_id_free(node->data);
        }
        OpenAPI_list_free(non_ue_n2_info_subscription_create_data->global_ran_node_list);
        non_ue_n2_info_subscription_create_data->global_ran_node_list = NULL;
    }
    if (non_ue_n2_info_subscription_create_data->an_type_list) {
        OpenAPI_list_free(non_ue_n2_info_subscription_create_data->an_type_list);
        non_ue_n2_info_subscription_create_data->an_type_list = NULL;
    }
    if (non_ue_n2_info_subscription_create_data->n2_notify_callback_uri) {
        ogs_free(non_ue_n2_info_subscription_create_data->n2_notify_callback_uri);
        non_ue_n2_info_subscription_create_data->n2_notify_callback_uri = NULL;
    }
    if (non_ue_n2_info_subscription_create_data->nf_id) {
        ogs_free(non_ue_n2_info_subscription_create_data->nf_id);
        non_ue_n2_info_subscription_create_data->nf_id = NULL;
    }
    if (non_ue_n2_info_subscription_create_data->supported_features) {
        ogs_free(non_ue_n2_info_subscription_create_data->supported_features);
        non_ue_n2_info_subscription_create_data->supported_features = NULL;
    }
    ogs_free(non_ue_n2_info_subscription_create_data);
}

cJSON *OpenAPI_non_ue_n2_info_subscription_create_data_convertToJSON(OpenAPI_non_ue_n2_info_subscription_create_data_t *non_ue_n2_info_subscription_create_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

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
    OpenAPI_list_for_each(non_ue_n2_info_subscription_create_data->global_ran_node_list, node) {
        cJSON *itemLocal = OpenAPI_global_ran_node_id_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_convertToJSON() failed [global_ran_node_list]");
            goto end;
        }
        cJSON_AddItemToArray(global_ran_node_listList, itemLocal);
    }
    }

    if (non_ue_n2_info_subscription_create_data->an_type_list != OpenAPI_access_type_NULL) {
    cJSON *an_type_listList = cJSON_AddArrayToObject(item, "anTypeList");
    if (an_type_listList == NULL) {
        ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_convertToJSON() failed [an_type_list]");
        goto end;
    }
    OpenAPI_list_for_each(non_ue_n2_info_subscription_create_data->an_type_list, node) {
        if (cJSON_AddStringToObject(an_type_listList, "", OpenAPI_access_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_convertToJSON() failed [an_type_list]");
            goto end;
        }
    }
    }

    if (non_ue_n2_info_subscription_create_data->n2_information_class == OpenAPI_n2_information_class_NULL) {
        ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_convertToJSON() failed [n2_information_class]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "n2InformationClass", OpenAPI_n2_information_class_ToString(non_ue_n2_info_subscription_create_data->n2_information_class)) == NULL) {
        ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_convertToJSON() failed [n2_information_class]");
        goto end;
    }

    if (!non_ue_n2_info_subscription_create_data->n2_notify_callback_uri) {
        ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_convertToJSON() failed [n2_notify_callback_uri]");
        return NULL;
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
    OpenAPI_lnode_t *node = NULL;
    cJSON *global_ran_node_list = NULL;
    OpenAPI_list_t *global_ran_node_listList = NULL;
    cJSON *an_type_list = NULL;
    OpenAPI_list_t *an_type_listList = NULL;
    cJSON *n2_information_class = NULL;
    OpenAPI_n2_information_class_e n2_information_classVariable = 0;
    cJSON *n2_notify_callback_uri = NULL;
    cJSON *nf_id = NULL;
    cJSON *supported_features = NULL;
    global_ran_node_list = cJSON_GetObjectItemCaseSensitive(non_ue_n2_info_subscription_create_dataJSON, "globalRanNodeList");
    if (global_ran_node_list) {
        cJSON *global_ran_node_list_local = NULL;
        if (!cJSON_IsArray(global_ran_node_list)) {
            ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_parseFromJSON() failed [global_ran_node_list]");
            goto end;
        }

        global_ran_node_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(global_ran_node_list_local, global_ran_node_list) {
            if (!cJSON_IsObject(global_ran_node_list_local)) {
                ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_parseFromJSON() failed [global_ran_node_list]");
                goto end;
            }
            OpenAPI_global_ran_node_id_t *global_ran_node_listItem = OpenAPI_global_ran_node_id_parseFromJSON(global_ran_node_list_local);
            if (!global_ran_node_listItem) {
                ogs_error("No global_ran_node_listItem");
                goto end;
            }
            OpenAPI_list_add(global_ran_node_listList, global_ran_node_listItem);
        }
    }

    an_type_list = cJSON_GetObjectItemCaseSensitive(non_ue_n2_info_subscription_create_dataJSON, "anTypeList");
    if (an_type_list) {
        cJSON *an_type_list_local = NULL;
        if (!cJSON_IsArray(an_type_list)) {
            ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_parseFromJSON() failed [an_type_list]");
            goto end;
        }

        an_type_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(an_type_list_local, an_type_list) {
            OpenAPI_access_type_e localEnum = OpenAPI_access_type_NULL;
            if (!cJSON_IsString(an_type_list_local)) {
                ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_parseFromJSON() failed [an_type_list]");
                goto end;
            }
            localEnum = OpenAPI_access_type_FromString(an_type_list_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"an_type_list\" is not supported. Ignoring it ...",
                         an_type_list_local->valuestring);
            } else {
                OpenAPI_list_add(an_type_listList, (void *)localEnum);
            }
        }
        if (an_type_listList->count == 0) {
            ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_parseFromJSON() failed: Expected an_type_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    n2_information_class = cJSON_GetObjectItemCaseSensitive(non_ue_n2_info_subscription_create_dataJSON, "n2InformationClass");
    if (!n2_information_class) {
        ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_parseFromJSON() failed [n2_information_class]");
        goto end;
    }
    if (!cJSON_IsString(n2_information_class)) {
        ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_parseFromJSON() failed [n2_information_class]");
        goto end;
    }
    n2_information_classVariable = OpenAPI_n2_information_class_FromString(n2_information_class->valuestring);

    n2_notify_callback_uri = cJSON_GetObjectItemCaseSensitive(non_ue_n2_info_subscription_create_dataJSON, "n2NotifyCallbackUri");
    if (!n2_notify_callback_uri) {
        ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_parseFromJSON() failed [n2_notify_callback_uri]");
        goto end;
    }
    if (!cJSON_IsString(n2_notify_callback_uri)) {
        ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_parseFromJSON() failed [n2_notify_callback_uri]");
        goto end;
    }

    nf_id = cJSON_GetObjectItemCaseSensitive(non_ue_n2_info_subscription_create_dataJSON, "nfId");
    if (nf_id) {
    if (!cJSON_IsString(nf_id) && !cJSON_IsNull(nf_id)) {
        ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_parseFromJSON() failed [nf_id]");
        goto end;
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(non_ue_n2_info_subscription_create_dataJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_non_ue_n2_info_subscription_create_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    non_ue_n2_info_subscription_create_data_local_var = OpenAPI_non_ue_n2_info_subscription_create_data_create (
        global_ran_node_list ? global_ran_node_listList : NULL,
        an_type_list ? an_type_listList : NULL,
        n2_information_classVariable,
        ogs_strdup(n2_notify_callback_uri->valuestring),
        nf_id && !cJSON_IsNull(nf_id) ? ogs_strdup(nf_id->valuestring) : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return non_ue_n2_info_subscription_create_data_local_var;
end:
    if (global_ran_node_listList) {
        OpenAPI_list_for_each(global_ran_node_listList, node) {
            OpenAPI_global_ran_node_id_free(node->data);
        }
        OpenAPI_list_free(global_ran_node_listList);
        global_ran_node_listList = NULL;
    }
    if (an_type_listList) {
        OpenAPI_list_free(an_type_listList);
        an_type_listList = NULL;
    }
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

