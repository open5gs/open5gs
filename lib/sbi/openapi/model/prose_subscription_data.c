
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "prose_subscription_data.h"

OpenAPI_prose_subscription_data_t *OpenAPI_prose_subscription_data_create(
    OpenAPI_prose_service_auth_t *prose_service_auth,
    char *nr_ue_pc5_ambr,
    OpenAPI_list_t *prose_allowed_plmn
)
{
    OpenAPI_prose_subscription_data_t *prose_subscription_data_local_var = ogs_malloc(sizeof(OpenAPI_prose_subscription_data_t));
    ogs_assert(prose_subscription_data_local_var);

    prose_subscription_data_local_var->prose_service_auth = prose_service_auth;
    prose_subscription_data_local_var->nr_ue_pc5_ambr = nr_ue_pc5_ambr;
    prose_subscription_data_local_var->prose_allowed_plmn = prose_allowed_plmn;

    return prose_subscription_data_local_var;
}

void OpenAPI_prose_subscription_data_free(OpenAPI_prose_subscription_data_t *prose_subscription_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == prose_subscription_data) {
        return;
    }
    if (prose_subscription_data->prose_service_auth) {
        OpenAPI_prose_service_auth_free(prose_subscription_data->prose_service_auth);
        prose_subscription_data->prose_service_auth = NULL;
    }
    if (prose_subscription_data->nr_ue_pc5_ambr) {
        ogs_free(prose_subscription_data->nr_ue_pc5_ambr);
        prose_subscription_data->nr_ue_pc5_ambr = NULL;
    }
    if (prose_subscription_data->prose_allowed_plmn) {
        OpenAPI_list_for_each(prose_subscription_data->prose_allowed_plmn, node) {
            OpenAPI_pro_se_allowed_plmn_free(node->data);
        }
        OpenAPI_list_free(prose_subscription_data->prose_allowed_plmn);
        prose_subscription_data->prose_allowed_plmn = NULL;
    }
    ogs_free(prose_subscription_data);
}

cJSON *OpenAPI_prose_subscription_data_convertToJSON(OpenAPI_prose_subscription_data_t *prose_subscription_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (prose_subscription_data == NULL) {
        ogs_error("OpenAPI_prose_subscription_data_convertToJSON() failed [ProseSubscriptionData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (prose_subscription_data->prose_service_auth) {
    cJSON *prose_service_auth_local_JSON = OpenAPI_prose_service_auth_convertToJSON(prose_subscription_data->prose_service_auth);
    if (prose_service_auth_local_JSON == NULL) {
        ogs_error("OpenAPI_prose_subscription_data_convertToJSON() failed [prose_service_auth]");
        goto end;
    }
    cJSON_AddItemToObject(item, "proseServiceAuth", prose_service_auth_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_prose_subscription_data_convertToJSON() failed [prose_service_auth]");
        goto end;
    }
    }

    if (prose_subscription_data->nr_ue_pc5_ambr) {
    if (cJSON_AddStringToObject(item, "nrUePc5Ambr", prose_subscription_data->nr_ue_pc5_ambr) == NULL) {
        ogs_error("OpenAPI_prose_subscription_data_convertToJSON() failed [nr_ue_pc5_ambr]");
        goto end;
    }
    }

    if (prose_subscription_data->prose_allowed_plmn) {
    cJSON *prose_allowed_plmnList = cJSON_AddArrayToObject(item, "proseAllowedPlmn");
    if (prose_allowed_plmnList == NULL) {
        ogs_error("OpenAPI_prose_subscription_data_convertToJSON() failed [prose_allowed_plmn]");
        goto end;
    }
    OpenAPI_list_for_each(prose_subscription_data->prose_allowed_plmn, node) {
        cJSON *itemLocal = OpenAPI_pro_se_allowed_plmn_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_prose_subscription_data_convertToJSON() failed [prose_allowed_plmn]");
            goto end;
        }
        cJSON_AddItemToArray(prose_allowed_plmnList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_prose_subscription_data_t *OpenAPI_prose_subscription_data_parseFromJSON(cJSON *prose_subscription_dataJSON)
{
    OpenAPI_prose_subscription_data_t *prose_subscription_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *prose_service_auth = NULL;
    OpenAPI_prose_service_auth_t *prose_service_auth_local_nonprim = NULL;
    cJSON *nr_ue_pc5_ambr = NULL;
    cJSON *prose_allowed_plmn = NULL;
    OpenAPI_list_t *prose_allowed_plmnList = NULL;
    prose_service_auth = cJSON_GetObjectItemCaseSensitive(prose_subscription_dataJSON, "proseServiceAuth");
    if (prose_service_auth) {
    prose_service_auth_local_nonprim = OpenAPI_prose_service_auth_parseFromJSON(prose_service_auth);
    if (!prose_service_auth_local_nonprim) {
        ogs_error("OpenAPI_prose_service_auth_parseFromJSON failed [prose_service_auth]");
        goto end;
    }
    }

    nr_ue_pc5_ambr = cJSON_GetObjectItemCaseSensitive(prose_subscription_dataJSON, "nrUePc5Ambr");
    if (nr_ue_pc5_ambr) {
    if (!cJSON_IsString(nr_ue_pc5_ambr) && !cJSON_IsNull(nr_ue_pc5_ambr)) {
        ogs_error("OpenAPI_prose_subscription_data_parseFromJSON() failed [nr_ue_pc5_ambr]");
        goto end;
    }
    }

    prose_allowed_plmn = cJSON_GetObjectItemCaseSensitive(prose_subscription_dataJSON, "proseAllowedPlmn");
    if (prose_allowed_plmn) {
        cJSON *prose_allowed_plmn_local = NULL;
        if (!cJSON_IsArray(prose_allowed_plmn)) {
            ogs_error("OpenAPI_prose_subscription_data_parseFromJSON() failed [prose_allowed_plmn]");
            goto end;
        }

        prose_allowed_plmnList = OpenAPI_list_create();

        cJSON_ArrayForEach(prose_allowed_plmn_local, prose_allowed_plmn) {
            if (!cJSON_IsObject(prose_allowed_plmn_local)) {
                ogs_error("OpenAPI_prose_subscription_data_parseFromJSON() failed [prose_allowed_plmn]");
                goto end;
            }
            OpenAPI_pro_se_allowed_plmn_t *prose_allowed_plmnItem = OpenAPI_pro_se_allowed_plmn_parseFromJSON(prose_allowed_plmn_local);
            if (!prose_allowed_plmnItem) {
                ogs_error("No prose_allowed_plmnItem");
                goto end;
            }
            OpenAPI_list_add(prose_allowed_plmnList, prose_allowed_plmnItem);
        }
    }

    prose_subscription_data_local_var = OpenAPI_prose_subscription_data_create (
        prose_service_auth ? prose_service_auth_local_nonprim : NULL,
        nr_ue_pc5_ambr && !cJSON_IsNull(nr_ue_pc5_ambr) ? ogs_strdup(nr_ue_pc5_ambr->valuestring) : NULL,
        prose_allowed_plmn ? prose_allowed_plmnList : NULL
    );

    return prose_subscription_data_local_var;
end:
    if (prose_service_auth_local_nonprim) {
        OpenAPI_prose_service_auth_free(prose_service_auth_local_nonprim);
        prose_service_auth_local_nonprim = NULL;
    }
    if (prose_allowed_plmnList) {
        OpenAPI_list_for_each(prose_allowed_plmnList, node) {
            OpenAPI_pro_se_allowed_plmn_free(node->data);
        }
        OpenAPI_list_free(prose_allowed_plmnList);
        prose_allowed_plmnList = NULL;
    }
    return NULL;
}

OpenAPI_prose_subscription_data_t *OpenAPI_prose_subscription_data_copy(OpenAPI_prose_subscription_data_t *dst, OpenAPI_prose_subscription_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_prose_subscription_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_prose_subscription_data_convertToJSON() failed");
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

    OpenAPI_prose_subscription_data_free(dst);
    dst = OpenAPI_prose_subscription_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

