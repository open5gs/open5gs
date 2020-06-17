
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "amf_status_change_subscription_data.h"

OpenAPI_amf_status_change_subscription_data_t *OpenAPI_amf_status_change_subscription_data_create(
    char *amf_status_uri,
    OpenAPI_list_t *guami_list
    )
{
    OpenAPI_amf_status_change_subscription_data_t *amf_status_change_subscription_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_amf_status_change_subscription_data_t));
    if (!amf_status_change_subscription_data_local_var) {
        return NULL;
    }
    amf_status_change_subscription_data_local_var->amf_status_uri = amf_status_uri;
    amf_status_change_subscription_data_local_var->guami_list = guami_list;

    return amf_status_change_subscription_data_local_var;
}

void OpenAPI_amf_status_change_subscription_data_free(OpenAPI_amf_status_change_subscription_data_t *amf_status_change_subscription_data)
{
    if (NULL == amf_status_change_subscription_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(amf_status_change_subscription_data->amf_status_uri);
    OpenAPI_list_for_each(amf_status_change_subscription_data->guami_list, node) {
        OpenAPI_guami_free(node->data);
    }
    OpenAPI_list_free(amf_status_change_subscription_data->guami_list);
    ogs_free(amf_status_change_subscription_data);
}

cJSON *OpenAPI_amf_status_change_subscription_data_convertToJSON(OpenAPI_amf_status_change_subscription_data_t *amf_status_change_subscription_data)
{
    cJSON *item = NULL;

    if (amf_status_change_subscription_data == NULL) {
        ogs_error("OpenAPI_amf_status_change_subscription_data_convertToJSON() failed [AMFStatusChangeSubscriptionData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!amf_status_change_subscription_data->amf_status_uri) {
        ogs_error("OpenAPI_amf_status_change_subscription_data_convertToJSON() failed [amf_status_uri]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "amfStatusUri", amf_status_change_subscription_data->amf_status_uri) == NULL) {
        ogs_error("OpenAPI_amf_status_change_subscription_data_convertToJSON() failed [amf_status_uri]");
        goto end;
    }

    if (amf_status_change_subscription_data->guami_list) {
        cJSON *guami_listList = cJSON_AddArrayToObject(item, "guamiList");
        if (guami_listList == NULL) {
            ogs_error("OpenAPI_amf_status_change_subscription_data_convertToJSON() failed [guami_list]");
            goto end;
        }

        OpenAPI_lnode_t *guami_list_node;
        if (amf_status_change_subscription_data->guami_list) {
            OpenAPI_list_for_each(amf_status_change_subscription_data->guami_list, guami_list_node) {
                cJSON *itemLocal = OpenAPI_guami_convertToJSON(guami_list_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_amf_status_change_subscription_data_convertToJSON() failed [guami_list]");
                    goto end;
                }
                cJSON_AddItemToArray(guami_listList, itemLocal);
            }
        }
    }

end:
    return item;
}

OpenAPI_amf_status_change_subscription_data_t *OpenAPI_amf_status_change_subscription_data_parseFromJSON(cJSON *amf_status_change_subscription_dataJSON)
{
    OpenAPI_amf_status_change_subscription_data_t *amf_status_change_subscription_data_local_var = NULL;
    cJSON *amf_status_uri = cJSON_GetObjectItemCaseSensitive(amf_status_change_subscription_dataJSON, "amfStatusUri");
    if (!amf_status_uri) {
        ogs_error("OpenAPI_amf_status_change_subscription_data_parseFromJSON() failed [amf_status_uri]");
        goto end;
    }


    if (!cJSON_IsString(amf_status_uri)) {
        ogs_error("OpenAPI_amf_status_change_subscription_data_parseFromJSON() failed [amf_status_uri]");
        goto end;
    }

    cJSON *guami_list = cJSON_GetObjectItemCaseSensitive(amf_status_change_subscription_dataJSON, "guamiList");

    OpenAPI_list_t *guami_listList;
    if (guami_list) {
        cJSON *guami_list_local_nonprimitive;
        if (!cJSON_IsArray(guami_list)) {
            ogs_error("OpenAPI_amf_status_change_subscription_data_parseFromJSON() failed [guami_list]");
            goto end;
        }

        guami_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(guami_list_local_nonprimitive, guami_list ) {
            if (!cJSON_IsObject(guami_list_local_nonprimitive)) {
                ogs_error("OpenAPI_amf_status_change_subscription_data_parseFromJSON() failed [guami_list]");
                goto end;
            }
            OpenAPI_guami_t *guami_listItem = OpenAPI_guami_parseFromJSON(guami_list_local_nonprimitive);

            OpenAPI_list_add(guami_listList, guami_listItem);
        }
    }

    amf_status_change_subscription_data_local_var = OpenAPI_amf_status_change_subscription_data_create (
        ogs_strdup(amf_status_uri->valuestring),
        guami_list ? guami_listList : NULL
        );

    return amf_status_change_subscription_data_local_var;
end:
    return NULL;
}

OpenAPI_amf_status_change_subscription_data_t *OpenAPI_amf_status_change_subscription_data_copy(OpenAPI_amf_status_change_subscription_data_t *dst, OpenAPI_amf_status_change_subscription_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_amf_status_change_subscription_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_amf_status_change_subscription_data_convertToJSON() failed");
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

    OpenAPI_amf_status_change_subscription_data_free(dst);
    dst = OpenAPI_amf_status_change_subscription_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

