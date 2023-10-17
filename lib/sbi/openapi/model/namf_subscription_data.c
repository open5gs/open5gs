
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "namf_subscription_data.h"

OpenAPI_namf_subscription_data_t *OpenAPI_namf_subscription_data_create(
    char *amf_status_uri,
    OpenAPI_list_t *guami_list
)
{
    OpenAPI_namf_subscription_data_t *namf_subscription_data_local_var = ogs_malloc(sizeof(OpenAPI_namf_subscription_data_t));
    ogs_assert(namf_subscription_data_local_var);

    namf_subscription_data_local_var->amf_status_uri = amf_status_uri;
    namf_subscription_data_local_var->guami_list = guami_list;

    return namf_subscription_data_local_var;
}

void OpenAPI_namf_subscription_data_free(OpenAPI_namf_subscription_data_t *namf_subscription_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == namf_subscription_data) {
        return;
    }
    if (namf_subscription_data->amf_status_uri) {
        ogs_free(namf_subscription_data->amf_status_uri);
        namf_subscription_data->amf_status_uri = NULL;
    }
    if (namf_subscription_data->guami_list) {
        OpenAPI_list_for_each(namf_subscription_data->guami_list, node) {
            OpenAPI_guami_free(node->data);
        }
        OpenAPI_list_free(namf_subscription_data->guami_list);
        namf_subscription_data->guami_list = NULL;
    }
    ogs_free(namf_subscription_data);
}

cJSON *OpenAPI_namf_subscription_data_convertToJSON(OpenAPI_namf_subscription_data_t *namf_subscription_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (namf_subscription_data == NULL) {
        ogs_error("OpenAPI_namf_subscription_data_convertToJSON() failed [NamfSubscriptionData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!namf_subscription_data->amf_status_uri) {
        ogs_error("OpenAPI_namf_subscription_data_convertToJSON() failed [amf_status_uri]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "amfStatusUri", namf_subscription_data->amf_status_uri) == NULL) {
        ogs_error("OpenAPI_namf_subscription_data_convertToJSON() failed [amf_status_uri]");
        goto end;
    }

    if (namf_subscription_data->guami_list) {
    cJSON *guami_listList = cJSON_AddArrayToObject(item, "guamiList");
    if (guami_listList == NULL) {
        ogs_error("OpenAPI_namf_subscription_data_convertToJSON() failed [guami_list]");
        goto end;
    }
    OpenAPI_list_for_each(namf_subscription_data->guami_list, node) {
        cJSON *itemLocal = OpenAPI_guami_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_namf_subscription_data_convertToJSON() failed [guami_list]");
            goto end;
        }
        cJSON_AddItemToArray(guami_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_namf_subscription_data_t *OpenAPI_namf_subscription_data_parseFromJSON(cJSON *namf_subscription_dataJSON)
{
    OpenAPI_namf_subscription_data_t *namf_subscription_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *amf_status_uri = NULL;
    cJSON *guami_list = NULL;
    OpenAPI_list_t *guami_listList = NULL;
    amf_status_uri = cJSON_GetObjectItemCaseSensitive(namf_subscription_dataJSON, "amfStatusUri");
    if (!amf_status_uri) {
        ogs_error("OpenAPI_namf_subscription_data_parseFromJSON() failed [amf_status_uri]");
        goto end;
    }
    if (!cJSON_IsString(amf_status_uri)) {
        ogs_error("OpenAPI_namf_subscription_data_parseFromJSON() failed [amf_status_uri]");
        goto end;
    }

    guami_list = cJSON_GetObjectItemCaseSensitive(namf_subscription_dataJSON, "guamiList");
    if (guami_list) {
        cJSON *guami_list_local = NULL;
        if (!cJSON_IsArray(guami_list)) {
            ogs_error("OpenAPI_namf_subscription_data_parseFromJSON() failed [guami_list]");
            goto end;
        }

        guami_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(guami_list_local, guami_list) {
            if (!cJSON_IsObject(guami_list_local)) {
                ogs_error("OpenAPI_namf_subscription_data_parseFromJSON() failed [guami_list]");
                goto end;
            }
            OpenAPI_guami_t *guami_listItem = OpenAPI_guami_parseFromJSON(guami_list_local);
            if (!guami_listItem) {
                ogs_error("No guami_listItem");
                goto end;
            }
            OpenAPI_list_add(guami_listList, guami_listItem);
        }
    }

    namf_subscription_data_local_var = OpenAPI_namf_subscription_data_create (
        ogs_strdup(amf_status_uri->valuestring),
        guami_list ? guami_listList : NULL
    );

    return namf_subscription_data_local_var;
end:
    if (guami_listList) {
        OpenAPI_list_for_each(guami_listList, node) {
            OpenAPI_guami_free(node->data);
        }
        OpenAPI_list_free(guami_listList);
        guami_listList = NULL;
    }
    return NULL;
}

OpenAPI_namf_subscription_data_t *OpenAPI_namf_subscription_data_copy(OpenAPI_namf_subscription_data_t *dst, OpenAPI_namf_subscription_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_namf_subscription_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_namf_subscription_data_convertToJSON() failed");
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

    OpenAPI_namf_subscription_data_free(dst);
    dst = OpenAPI_namf_subscription_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

