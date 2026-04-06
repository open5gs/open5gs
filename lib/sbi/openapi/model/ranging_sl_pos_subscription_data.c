
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ranging_sl_pos_subscription_data.h"

OpenAPI_ranging_sl_pos_subscription_data_t *OpenAPI_ranging_sl_pos_subscription_data_create(
    OpenAPI_ranging_sl_pos_auth_t *ranging_sl_pos_auth,
    OpenAPI_list_t *ranging_sl_pos_plmn,
    OpenAPI_ranging_sl_pos_qos_t *ranging_sl_pos_qos
)
{
    OpenAPI_ranging_sl_pos_subscription_data_t *ranging_sl_pos_subscription_data_local_var = ogs_malloc(sizeof(OpenAPI_ranging_sl_pos_subscription_data_t));
    ogs_assert(ranging_sl_pos_subscription_data_local_var);

    ranging_sl_pos_subscription_data_local_var->ranging_sl_pos_auth = ranging_sl_pos_auth;
    ranging_sl_pos_subscription_data_local_var->ranging_sl_pos_plmn = ranging_sl_pos_plmn;
    ranging_sl_pos_subscription_data_local_var->ranging_sl_pos_qos = ranging_sl_pos_qos;

    return ranging_sl_pos_subscription_data_local_var;
}

void OpenAPI_ranging_sl_pos_subscription_data_free(OpenAPI_ranging_sl_pos_subscription_data_t *ranging_sl_pos_subscription_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ranging_sl_pos_subscription_data) {
        return;
    }
    if (ranging_sl_pos_subscription_data->ranging_sl_pos_auth) {
        OpenAPI_ranging_sl_pos_auth_free(ranging_sl_pos_subscription_data->ranging_sl_pos_auth);
        ranging_sl_pos_subscription_data->ranging_sl_pos_auth = NULL;
    }
    if (ranging_sl_pos_subscription_data->ranging_sl_pos_plmn) {
        OpenAPI_list_for_each(ranging_sl_pos_subscription_data->ranging_sl_pos_plmn, node) {
            OpenAPI_ranging_sl_pos_plmn_free(node->data);
        }
        OpenAPI_list_free(ranging_sl_pos_subscription_data->ranging_sl_pos_plmn);
        ranging_sl_pos_subscription_data->ranging_sl_pos_plmn = NULL;
    }
    if (ranging_sl_pos_subscription_data->ranging_sl_pos_qos) {
        OpenAPI_ranging_sl_pos_qos_free(ranging_sl_pos_subscription_data->ranging_sl_pos_qos);
        ranging_sl_pos_subscription_data->ranging_sl_pos_qos = NULL;
    }
    ogs_free(ranging_sl_pos_subscription_data);
}

cJSON *OpenAPI_ranging_sl_pos_subscription_data_convertToJSON(OpenAPI_ranging_sl_pos_subscription_data_t *ranging_sl_pos_subscription_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ranging_sl_pos_subscription_data == NULL) {
        ogs_error("OpenAPI_ranging_sl_pos_subscription_data_convertToJSON() failed [RangingSlPosSubscriptionData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ranging_sl_pos_subscription_data->ranging_sl_pos_auth) {
    cJSON *ranging_sl_pos_auth_local_JSON = OpenAPI_ranging_sl_pos_auth_convertToJSON(ranging_sl_pos_subscription_data->ranging_sl_pos_auth);
    if (ranging_sl_pos_auth_local_JSON == NULL) {
        ogs_error("OpenAPI_ranging_sl_pos_subscription_data_convertToJSON() failed [ranging_sl_pos_auth]");
        goto end;
    }
    cJSON_AddItemToObject(item, "rangingSlPosAuth", ranging_sl_pos_auth_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ranging_sl_pos_subscription_data_convertToJSON() failed [ranging_sl_pos_auth]");
        goto end;
    }
    }

    if (ranging_sl_pos_subscription_data->ranging_sl_pos_plmn) {
    cJSON *ranging_sl_pos_plmnList = cJSON_AddArrayToObject(item, "rangingSlPosPlmn");
    if (ranging_sl_pos_plmnList == NULL) {
        ogs_error("OpenAPI_ranging_sl_pos_subscription_data_convertToJSON() failed [ranging_sl_pos_plmn]");
        goto end;
    }
    OpenAPI_list_for_each(ranging_sl_pos_subscription_data->ranging_sl_pos_plmn, node) {
        cJSON *itemLocal = OpenAPI_ranging_sl_pos_plmn_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ranging_sl_pos_subscription_data_convertToJSON() failed [ranging_sl_pos_plmn]");
            goto end;
        }
        cJSON_AddItemToArray(ranging_sl_pos_plmnList, itemLocal);
    }
    }

    if (ranging_sl_pos_subscription_data->ranging_sl_pos_qos) {
    cJSON *ranging_sl_pos_qos_local_JSON = OpenAPI_ranging_sl_pos_qos_convertToJSON(ranging_sl_pos_subscription_data->ranging_sl_pos_qos);
    if (ranging_sl_pos_qos_local_JSON == NULL) {
        ogs_error("OpenAPI_ranging_sl_pos_subscription_data_convertToJSON() failed [ranging_sl_pos_qos]");
        goto end;
    }
    cJSON_AddItemToObject(item, "rangingSlPosQos", ranging_sl_pos_qos_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ranging_sl_pos_subscription_data_convertToJSON() failed [ranging_sl_pos_qos]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ranging_sl_pos_subscription_data_t *OpenAPI_ranging_sl_pos_subscription_data_parseFromJSON(cJSON *ranging_sl_pos_subscription_dataJSON)
{
    OpenAPI_ranging_sl_pos_subscription_data_t *ranging_sl_pos_subscription_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ranging_sl_pos_auth = NULL;
    OpenAPI_ranging_sl_pos_auth_t *ranging_sl_pos_auth_local_nonprim = NULL;
    cJSON *ranging_sl_pos_plmn = NULL;
    OpenAPI_list_t *ranging_sl_pos_plmnList = NULL;
    cJSON *ranging_sl_pos_qos = NULL;
    OpenAPI_ranging_sl_pos_qos_t *ranging_sl_pos_qos_local_nonprim = NULL;
    ranging_sl_pos_auth = cJSON_GetObjectItemCaseSensitive(ranging_sl_pos_subscription_dataJSON, "rangingSlPosAuth");
    if (ranging_sl_pos_auth) {
    ranging_sl_pos_auth_local_nonprim = OpenAPI_ranging_sl_pos_auth_parseFromJSON(ranging_sl_pos_auth);
    if (!ranging_sl_pos_auth_local_nonprim) {
        ogs_error("OpenAPI_ranging_sl_pos_auth_parseFromJSON failed [ranging_sl_pos_auth]");
        goto end;
    }
    }

    ranging_sl_pos_plmn = cJSON_GetObjectItemCaseSensitive(ranging_sl_pos_subscription_dataJSON, "rangingSlPosPlmn");
    if (ranging_sl_pos_plmn) {
        cJSON *ranging_sl_pos_plmn_local = NULL;
        if (!cJSON_IsArray(ranging_sl_pos_plmn)) {
            ogs_error("OpenAPI_ranging_sl_pos_subscription_data_parseFromJSON() failed [ranging_sl_pos_plmn]");
            goto end;
        }

        ranging_sl_pos_plmnList = OpenAPI_list_create();

        cJSON_ArrayForEach(ranging_sl_pos_plmn_local, ranging_sl_pos_plmn) {
            if (!cJSON_IsObject(ranging_sl_pos_plmn_local)) {
                ogs_error("OpenAPI_ranging_sl_pos_subscription_data_parseFromJSON() failed [ranging_sl_pos_plmn]");
                goto end;
            }
            OpenAPI_ranging_sl_pos_plmn_t *ranging_sl_pos_plmnItem = OpenAPI_ranging_sl_pos_plmn_parseFromJSON(ranging_sl_pos_plmn_local);
            if (!ranging_sl_pos_plmnItem) {
                ogs_error("No ranging_sl_pos_plmnItem");
                goto end;
            }
            OpenAPI_list_add(ranging_sl_pos_plmnList, ranging_sl_pos_plmnItem);
        }
    }

    ranging_sl_pos_qos = cJSON_GetObjectItemCaseSensitive(ranging_sl_pos_subscription_dataJSON, "rangingSlPosQos");
    if (ranging_sl_pos_qos) {
    ranging_sl_pos_qos_local_nonprim = OpenAPI_ranging_sl_pos_qos_parseFromJSON(ranging_sl_pos_qos);
    if (!ranging_sl_pos_qos_local_nonprim) {
        ogs_error("OpenAPI_ranging_sl_pos_qos_parseFromJSON failed [ranging_sl_pos_qos]");
        goto end;
    }
    }

    ranging_sl_pos_subscription_data_local_var = OpenAPI_ranging_sl_pos_subscription_data_create (
        ranging_sl_pos_auth ? ranging_sl_pos_auth_local_nonprim : NULL,
        ranging_sl_pos_plmn ? ranging_sl_pos_plmnList : NULL,
        ranging_sl_pos_qos ? ranging_sl_pos_qos_local_nonprim : NULL
    );

    return ranging_sl_pos_subscription_data_local_var;
end:
    if (ranging_sl_pos_auth_local_nonprim) {
        OpenAPI_ranging_sl_pos_auth_free(ranging_sl_pos_auth_local_nonprim);
        ranging_sl_pos_auth_local_nonprim = NULL;
    }
    if (ranging_sl_pos_plmnList) {
        OpenAPI_list_for_each(ranging_sl_pos_plmnList, node) {
            OpenAPI_ranging_sl_pos_plmn_free(node->data);
        }
        OpenAPI_list_free(ranging_sl_pos_plmnList);
        ranging_sl_pos_plmnList = NULL;
    }
    if (ranging_sl_pos_qos_local_nonprim) {
        OpenAPI_ranging_sl_pos_qos_free(ranging_sl_pos_qos_local_nonprim);
        ranging_sl_pos_qos_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_ranging_sl_pos_subscription_data_t *OpenAPI_ranging_sl_pos_subscription_data_copy(OpenAPI_ranging_sl_pos_subscription_data_t *dst, OpenAPI_ranging_sl_pos_subscription_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ranging_sl_pos_subscription_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ranging_sl_pos_subscription_data_convertToJSON() failed");
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

    OpenAPI_ranging_sl_pos_subscription_data_free(dst);
    dst = OpenAPI_ranging_sl_pos_subscription_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

