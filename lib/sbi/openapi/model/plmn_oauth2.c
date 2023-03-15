
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "plmn_oauth2.h"

OpenAPI_plmn_oauth2_t *OpenAPI_plmn_oauth2_create(
    OpenAPI_list_t *oauth2_required_plmn_id_list,
    OpenAPI_list_t *oauth2_not_required_plmn_id_list
)
{
    OpenAPI_plmn_oauth2_t *plmn_oauth2_local_var = ogs_malloc(sizeof(OpenAPI_plmn_oauth2_t));
    ogs_assert(plmn_oauth2_local_var);

    plmn_oauth2_local_var->oauth2_required_plmn_id_list = oauth2_required_plmn_id_list;
    plmn_oauth2_local_var->oauth2_not_required_plmn_id_list = oauth2_not_required_plmn_id_list;

    return plmn_oauth2_local_var;
}

void OpenAPI_plmn_oauth2_free(OpenAPI_plmn_oauth2_t *plmn_oauth2)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == plmn_oauth2) {
        return;
    }
    if (plmn_oauth2->oauth2_required_plmn_id_list) {
        OpenAPI_list_for_each(plmn_oauth2->oauth2_required_plmn_id_list, node) {
            OpenAPI_plmn_id_free(node->data);
        }
        OpenAPI_list_free(plmn_oauth2->oauth2_required_plmn_id_list);
        plmn_oauth2->oauth2_required_plmn_id_list = NULL;
    }
    if (plmn_oauth2->oauth2_not_required_plmn_id_list) {
        OpenAPI_list_for_each(plmn_oauth2->oauth2_not_required_plmn_id_list, node) {
            OpenAPI_plmn_id_free(node->data);
        }
        OpenAPI_list_free(plmn_oauth2->oauth2_not_required_plmn_id_list);
        plmn_oauth2->oauth2_not_required_plmn_id_list = NULL;
    }
    ogs_free(plmn_oauth2);
}

cJSON *OpenAPI_plmn_oauth2_convertToJSON(OpenAPI_plmn_oauth2_t *plmn_oauth2)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (plmn_oauth2 == NULL) {
        ogs_error("OpenAPI_plmn_oauth2_convertToJSON() failed [PlmnOauth2]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (plmn_oauth2->oauth2_required_plmn_id_list) {
    cJSON *oauth2_required_plmn_id_listList = cJSON_AddArrayToObject(item, "oauth2RequiredPlmnIdList");
    if (oauth2_required_plmn_id_listList == NULL) {
        ogs_error("OpenAPI_plmn_oauth2_convertToJSON() failed [oauth2_required_plmn_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(plmn_oauth2->oauth2_required_plmn_id_list, node) {
        cJSON *itemLocal = OpenAPI_plmn_id_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_plmn_oauth2_convertToJSON() failed [oauth2_required_plmn_id_list]");
            goto end;
        }
        cJSON_AddItemToArray(oauth2_required_plmn_id_listList, itemLocal);
    }
    }

    if (plmn_oauth2->oauth2_not_required_plmn_id_list) {
    cJSON *oauth2_not_required_plmn_id_listList = cJSON_AddArrayToObject(item, "oauth2NotRequiredPlmnIdList");
    if (oauth2_not_required_plmn_id_listList == NULL) {
        ogs_error("OpenAPI_plmn_oauth2_convertToJSON() failed [oauth2_not_required_plmn_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(plmn_oauth2->oauth2_not_required_plmn_id_list, node) {
        cJSON *itemLocal = OpenAPI_plmn_id_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_plmn_oauth2_convertToJSON() failed [oauth2_not_required_plmn_id_list]");
            goto end;
        }
        cJSON_AddItemToArray(oauth2_not_required_plmn_id_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_plmn_oauth2_t *OpenAPI_plmn_oauth2_parseFromJSON(cJSON *plmn_oauth2JSON)
{
    OpenAPI_plmn_oauth2_t *plmn_oauth2_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *oauth2_required_plmn_id_list = NULL;
    OpenAPI_list_t *oauth2_required_plmn_id_listList = NULL;
    cJSON *oauth2_not_required_plmn_id_list = NULL;
    OpenAPI_list_t *oauth2_not_required_plmn_id_listList = NULL;
    oauth2_required_plmn_id_list = cJSON_GetObjectItemCaseSensitive(plmn_oauth2JSON, "oauth2RequiredPlmnIdList");
    if (oauth2_required_plmn_id_list) {
        cJSON *oauth2_required_plmn_id_list_local = NULL;
        if (!cJSON_IsArray(oauth2_required_plmn_id_list)) {
            ogs_error("OpenAPI_plmn_oauth2_parseFromJSON() failed [oauth2_required_plmn_id_list]");
            goto end;
        }

        oauth2_required_plmn_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(oauth2_required_plmn_id_list_local, oauth2_required_plmn_id_list) {
            if (!cJSON_IsObject(oauth2_required_plmn_id_list_local)) {
                ogs_error("OpenAPI_plmn_oauth2_parseFromJSON() failed [oauth2_required_plmn_id_list]");
                goto end;
            }
            OpenAPI_plmn_id_t *oauth2_required_plmn_id_listItem = OpenAPI_plmn_id_parseFromJSON(oauth2_required_plmn_id_list_local);
            if (!oauth2_required_plmn_id_listItem) {
                ogs_error("No oauth2_required_plmn_id_listItem");
                goto end;
            }
            OpenAPI_list_add(oauth2_required_plmn_id_listList, oauth2_required_plmn_id_listItem);
        }
    }

    oauth2_not_required_plmn_id_list = cJSON_GetObjectItemCaseSensitive(plmn_oauth2JSON, "oauth2NotRequiredPlmnIdList");
    if (oauth2_not_required_plmn_id_list) {
        cJSON *oauth2_not_required_plmn_id_list_local = NULL;
        if (!cJSON_IsArray(oauth2_not_required_plmn_id_list)) {
            ogs_error("OpenAPI_plmn_oauth2_parseFromJSON() failed [oauth2_not_required_plmn_id_list]");
            goto end;
        }

        oauth2_not_required_plmn_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(oauth2_not_required_plmn_id_list_local, oauth2_not_required_plmn_id_list) {
            if (!cJSON_IsObject(oauth2_not_required_plmn_id_list_local)) {
                ogs_error("OpenAPI_plmn_oauth2_parseFromJSON() failed [oauth2_not_required_plmn_id_list]");
                goto end;
            }
            OpenAPI_plmn_id_t *oauth2_not_required_plmn_id_listItem = OpenAPI_plmn_id_parseFromJSON(oauth2_not_required_plmn_id_list_local);
            if (!oauth2_not_required_plmn_id_listItem) {
                ogs_error("No oauth2_not_required_plmn_id_listItem");
                goto end;
            }
            OpenAPI_list_add(oauth2_not_required_plmn_id_listList, oauth2_not_required_plmn_id_listItem);
        }
    }

    plmn_oauth2_local_var = OpenAPI_plmn_oauth2_create (
        oauth2_required_plmn_id_list ? oauth2_required_plmn_id_listList : NULL,
        oauth2_not_required_plmn_id_list ? oauth2_not_required_plmn_id_listList : NULL
    );

    return plmn_oauth2_local_var;
end:
    if (oauth2_required_plmn_id_listList) {
        OpenAPI_list_for_each(oauth2_required_plmn_id_listList, node) {
            OpenAPI_plmn_id_free(node->data);
        }
        OpenAPI_list_free(oauth2_required_plmn_id_listList);
        oauth2_required_plmn_id_listList = NULL;
    }
    if (oauth2_not_required_plmn_id_listList) {
        OpenAPI_list_for_each(oauth2_not_required_plmn_id_listList, node) {
            OpenAPI_plmn_id_free(node->data);
        }
        OpenAPI_list_free(oauth2_not_required_plmn_id_listList);
        oauth2_not_required_plmn_id_listList = NULL;
    }
    return NULL;
}

OpenAPI_plmn_oauth2_t *OpenAPI_plmn_oauth2_copy(OpenAPI_plmn_oauth2_t *dst, OpenAPI_plmn_oauth2_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_plmn_oauth2_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_plmn_oauth2_convertToJSON() failed");
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

    OpenAPI_plmn_oauth2_free(dst);
    dst = OpenAPI_plmn_oauth2_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

