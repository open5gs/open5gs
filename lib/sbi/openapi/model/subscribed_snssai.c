
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "subscribed_snssai.h"

OpenAPI_subscribed_snssai_t *OpenAPI_subscribed_snssai_create(
    OpenAPI_snssai_t *subscribed_snssai,
    bool is_default_indication,
    int default_indication,
    OpenAPI_list_t *subscribed_ns_srg_list
)
{
    OpenAPI_subscribed_snssai_t *subscribed_snssai_local_var = ogs_malloc(sizeof(OpenAPI_subscribed_snssai_t));
    ogs_assert(subscribed_snssai_local_var);

    subscribed_snssai_local_var->subscribed_snssai = subscribed_snssai;
    subscribed_snssai_local_var->is_default_indication = is_default_indication;
    subscribed_snssai_local_var->default_indication = default_indication;
    subscribed_snssai_local_var->subscribed_ns_srg_list = subscribed_ns_srg_list;

    return subscribed_snssai_local_var;
}

void OpenAPI_subscribed_snssai_free(OpenAPI_subscribed_snssai_t *subscribed_snssai)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == subscribed_snssai) {
        return;
    }
    if (subscribed_snssai->subscribed_snssai) {
        OpenAPI_snssai_free(subscribed_snssai->subscribed_snssai);
        subscribed_snssai->subscribed_snssai = NULL;
    }
    if (subscribed_snssai->subscribed_ns_srg_list) {
        OpenAPI_list_for_each(subscribed_snssai->subscribed_ns_srg_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(subscribed_snssai->subscribed_ns_srg_list);
        subscribed_snssai->subscribed_ns_srg_list = NULL;
    }
    ogs_free(subscribed_snssai);
}

cJSON *OpenAPI_subscribed_snssai_convertToJSON(OpenAPI_subscribed_snssai_t *subscribed_snssai)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (subscribed_snssai == NULL) {
        ogs_error("OpenAPI_subscribed_snssai_convertToJSON() failed [SubscribedSnssai]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!subscribed_snssai->subscribed_snssai) {
        ogs_error("OpenAPI_subscribed_snssai_convertToJSON() failed [subscribed_snssai]");
        return NULL;
    }
    cJSON *subscribed_snssai_local_JSON = OpenAPI_snssai_convertToJSON(subscribed_snssai->subscribed_snssai);
    if (subscribed_snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_subscribed_snssai_convertToJSON() failed [subscribed_snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "subscribedSnssai", subscribed_snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_subscribed_snssai_convertToJSON() failed [subscribed_snssai]");
        goto end;
    }

    if (subscribed_snssai->is_default_indication) {
    if (cJSON_AddBoolToObject(item, "defaultIndication", subscribed_snssai->default_indication) == NULL) {
        ogs_error("OpenAPI_subscribed_snssai_convertToJSON() failed [default_indication]");
        goto end;
    }
    }

    if (subscribed_snssai->subscribed_ns_srg_list) {
    cJSON *subscribed_ns_srg_listList = cJSON_AddArrayToObject(item, "subscribedNsSrgList");
    if (subscribed_ns_srg_listList == NULL) {
        ogs_error("OpenAPI_subscribed_snssai_convertToJSON() failed [subscribed_ns_srg_list]");
        goto end;
    }
    OpenAPI_list_for_each(subscribed_snssai->subscribed_ns_srg_list, node) {
        if (cJSON_AddStringToObject(subscribed_ns_srg_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_subscribed_snssai_convertToJSON() failed [subscribed_ns_srg_list]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_subscribed_snssai_t *OpenAPI_subscribed_snssai_parseFromJSON(cJSON *subscribed_snssaiJSON)
{
    OpenAPI_subscribed_snssai_t *subscribed_snssai_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *subscribed_snssai = NULL;
    OpenAPI_snssai_t *subscribed_snssai_local_nonprim = NULL;
    cJSON *default_indication = NULL;
    cJSON *subscribed_ns_srg_list = NULL;
    OpenAPI_list_t *subscribed_ns_srg_listList = NULL;
    subscribed_snssai = cJSON_GetObjectItemCaseSensitive(subscribed_snssaiJSON, "subscribedSnssai");
    if (!subscribed_snssai) {
        ogs_error("OpenAPI_subscribed_snssai_parseFromJSON() failed [subscribed_snssai]");
        goto end;
    }
    subscribed_snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(subscribed_snssai);
    if (!subscribed_snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [subscribed_snssai]");
        goto end;
    }

    default_indication = cJSON_GetObjectItemCaseSensitive(subscribed_snssaiJSON, "defaultIndication");
    if (default_indication) {
    if (!cJSON_IsBool(default_indication)) {
        ogs_error("OpenAPI_subscribed_snssai_parseFromJSON() failed [default_indication]");
        goto end;
    }
    }

    subscribed_ns_srg_list = cJSON_GetObjectItemCaseSensitive(subscribed_snssaiJSON, "subscribedNsSrgList");
    if (subscribed_ns_srg_list) {
        cJSON *subscribed_ns_srg_list_local = NULL;
        if (!cJSON_IsArray(subscribed_ns_srg_list)) {
            ogs_error("OpenAPI_subscribed_snssai_parseFromJSON() failed [subscribed_ns_srg_list]");
            goto end;
        }

        subscribed_ns_srg_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(subscribed_ns_srg_list_local, subscribed_ns_srg_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(subscribed_ns_srg_list_local)) {
                ogs_error("OpenAPI_subscribed_snssai_parseFromJSON() failed [subscribed_ns_srg_list]");
                goto end;
            }
            OpenAPI_list_add(subscribed_ns_srg_listList, ogs_strdup(subscribed_ns_srg_list_local->valuestring));
        }
    }

    subscribed_snssai_local_var = OpenAPI_subscribed_snssai_create (
        subscribed_snssai_local_nonprim,
        default_indication ? true : false,
        default_indication ? default_indication->valueint : 0,
        subscribed_ns_srg_list ? subscribed_ns_srg_listList : NULL
    );

    return subscribed_snssai_local_var;
end:
    if (subscribed_snssai_local_nonprim) {
        OpenAPI_snssai_free(subscribed_snssai_local_nonprim);
        subscribed_snssai_local_nonprim = NULL;
    }
    if (subscribed_ns_srg_listList) {
        OpenAPI_list_for_each(subscribed_ns_srg_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(subscribed_ns_srg_listList);
        subscribed_ns_srg_listList = NULL;
    }
    return NULL;
}

OpenAPI_subscribed_snssai_t *OpenAPI_subscribed_snssai_copy(OpenAPI_subscribed_snssai_t *dst, OpenAPI_subscribed_snssai_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_subscribed_snssai_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_subscribed_snssai_convertToJSON() failed");
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

    OpenAPI_subscribed_snssai_free(dst);
    dst = OpenAPI_subscribed_snssai_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

