
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "monitoring_suspension_1.h"

OpenAPI_monitoring_suspension_1_t *OpenAPI_monitoring_suspension_1_create(
    OpenAPI_list_t *suspended_inside_plmn_list,
    OpenAPI_list_t *suspended_outside_plmn_list
)
{
    OpenAPI_monitoring_suspension_1_t *monitoring_suspension_1_local_var = ogs_malloc(sizeof(OpenAPI_monitoring_suspension_1_t));
    ogs_assert(monitoring_suspension_1_local_var);

    monitoring_suspension_1_local_var->suspended_inside_plmn_list = suspended_inside_plmn_list;
    monitoring_suspension_1_local_var->suspended_outside_plmn_list = suspended_outside_plmn_list;

    return monitoring_suspension_1_local_var;
}

void OpenAPI_monitoring_suspension_1_free(OpenAPI_monitoring_suspension_1_t *monitoring_suspension_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == monitoring_suspension_1) {
        return;
    }
    if (monitoring_suspension_1->suspended_inside_plmn_list) {
        OpenAPI_list_for_each(monitoring_suspension_1->suspended_inside_plmn_list, node) {
            OpenAPI_plmn_id_nid_1_free(node->data);
        }
        OpenAPI_list_free(monitoring_suspension_1->suspended_inside_plmn_list);
        monitoring_suspension_1->suspended_inside_plmn_list = NULL;
    }
    if (monitoring_suspension_1->suspended_outside_plmn_list) {
        OpenAPI_list_for_each(monitoring_suspension_1->suspended_outside_plmn_list, node) {
            OpenAPI_plmn_id_nid_1_free(node->data);
        }
        OpenAPI_list_free(monitoring_suspension_1->suspended_outside_plmn_list);
        monitoring_suspension_1->suspended_outside_plmn_list = NULL;
    }
    ogs_free(monitoring_suspension_1);
}

cJSON *OpenAPI_monitoring_suspension_1_convertToJSON(OpenAPI_monitoring_suspension_1_t *monitoring_suspension_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (monitoring_suspension_1 == NULL) {
        ogs_error("OpenAPI_monitoring_suspension_1_convertToJSON() failed [MonitoringSuspension_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (monitoring_suspension_1->suspended_inside_plmn_list) {
    cJSON *suspended_inside_plmn_listList = cJSON_AddArrayToObject(item, "suspendedInsidePlmnList");
    if (suspended_inside_plmn_listList == NULL) {
        ogs_error("OpenAPI_monitoring_suspension_1_convertToJSON() failed [suspended_inside_plmn_list]");
        goto end;
    }
    OpenAPI_list_for_each(monitoring_suspension_1->suspended_inside_plmn_list, node) {
        cJSON *itemLocal = OpenAPI_plmn_id_nid_1_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_monitoring_suspension_1_convertToJSON() failed [suspended_inside_plmn_list]");
            goto end;
        }
        cJSON_AddItemToArray(suspended_inside_plmn_listList, itemLocal);
    }
    }

    if (monitoring_suspension_1->suspended_outside_plmn_list) {
    cJSON *suspended_outside_plmn_listList = cJSON_AddArrayToObject(item, "suspendedOutsidePlmnList");
    if (suspended_outside_plmn_listList == NULL) {
        ogs_error("OpenAPI_monitoring_suspension_1_convertToJSON() failed [suspended_outside_plmn_list]");
        goto end;
    }
    OpenAPI_list_for_each(monitoring_suspension_1->suspended_outside_plmn_list, node) {
        cJSON *itemLocal = OpenAPI_plmn_id_nid_1_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_monitoring_suspension_1_convertToJSON() failed [suspended_outside_plmn_list]");
            goto end;
        }
        cJSON_AddItemToArray(suspended_outside_plmn_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_monitoring_suspension_1_t *OpenAPI_monitoring_suspension_1_parseFromJSON(cJSON *monitoring_suspension_1JSON)
{
    OpenAPI_monitoring_suspension_1_t *monitoring_suspension_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *suspended_inside_plmn_list = NULL;
    OpenAPI_list_t *suspended_inside_plmn_listList = NULL;
    cJSON *suspended_outside_plmn_list = NULL;
    OpenAPI_list_t *suspended_outside_plmn_listList = NULL;
    suspended_inside_plmn_list = cJSON_GetObjectItemCaseSensitive(monitoring_suspension_1JSON, "suspendedInsidePlmnList");
    if (suspended_inside_plmn_list) {
        cJSON *suspended_inside_plmn_list_local = NULL;
        if (!cJSON_IsArray(suspended_inside_plmn_list)) {
            ogs_error("OpenAPI_monitoring_suspension_1_parseFromJSON() failed [suspended_inside_plmn_list]");
            goto end;
        }

        suspended_inside_plmn_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(suspended_inside_plmn_list_local, suspended_inside_plmn_list) {
            if (!cJSON_IsObject(suspended_inside_plmn_list_local)) {
                ogs_error("OpenAPI_monitoring_suspension_1_parseFromJSON() failed [suspended_inside_plmn_list]");
                goto end;
            }
            OpenAPI_plmn_id_nid_1_t *suspended_inside_plmn_listItem = OpenAPI_plmn_id_nid_1_parseFromJSON(suspended_inside_plmn_list_local);
            if (!suspended_inside_plmn_listItem) {
                ogs_error("No suspended_inside_plmn_listItem");
                goto end;
            }
            OpenAPI_list_add(suspended_inside_plmn_listList, suspended_inside_plmn_listItem);
        }
    }

    suspended_outside_plmn_list = cJSON_GetObjectItemCaseSensitive(monitoring_suspension_1JSON, "suspendedOutsidePlmnList");
    if (suspended_outside_plmn_list) {
        cJSON *suspended_outside_plmn_list_local = NULL;
        if (!cJSON_IsArray(suspended_outside_plmn_list)) {
            ogs_error("OpenAPI_monitoring_suspension_1_parseFromJSON() failed [suspended_outside_plmn_list]");
            goto end;
        }

        suspended_outside_plmn_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(suspended_outside_plmn_list_local, suspended_outside_plmn_list) {
            if (!cJSON_IsObject(suspended_outside_plmn_list_local)) {
                ogs_error("OpenAPI_monitoring_suspension_1_parseFromJSON() failed [suspended_outside_plmn_list]");
                goto end;
            }
            OpenAPI_plmn_id_nid_1_t *suspended_outside_plmn_listItem = OpenAPI_plmn_id_nid_1_parseFromJSON(suspended_outside_plmn_list_local);
            if (!suspended_outside_plmn_listItem) {
                ogs_error("No suspended_outside_plmn_listItem");
                goto end;
            }
            OpenAPI_list_add(suspended_outside_plmn_listList, suspended_outside_plmn_listItem);
        }
    }

    monitoring_suspension_1_local_var = OpenAPI_monitoring_suspension_1_create (
        suspended_inside_plmn_list ? suspended_inside_plmn_listList : NULL,
        suspended_outside_plmn_list ? suspended_outside_plmn_listList : NULL
    );

    return monitoring_suspension_1_local_var;
end:
    if (suspended_inside_plmn_listList) {
        OpenAPI_list_for_each(suspended_inside_plmn_listList, node) {
            OpenAPI_plmn_id_nid_1_free(node->data);
        }
        OpenAPI_list_free(suspended_inside_plmn_listList);
        suspended_inside_plmn_listList = NULL;
    }
    if (suspended_outside_plmn_listList) {
        OpenAPI_list_for_each(suspended_outside_plmn_listList, node) {
            OpenAPI_plmn_id_nid_1_free(node->data);
        }
        OpenAPI_list_free(suspended_outside_plmn_listList);
        suspended_outside_plmn_listList = NULL;
    }
    return NULL;
}

OpenAPI_monitoring_suspension_1_t *OpenAPI_monitoring_suspension_1_copy(OpenAPI_monitoring_suspension_1_t *dst, OpenAPI_monitoring_suspension_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_monitoring_suspension_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_monitoring_suspension_1_convertToJSON() failed");
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

    OpenAPI_monitoring_suspension_1_free(dst);
    dst = OpenAPI_monitoring_suspension_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

