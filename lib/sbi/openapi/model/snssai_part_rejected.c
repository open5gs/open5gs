
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "snssai_part_rejected.h"

OpenAPI_snssai_part_rejected_t *OpenAPI_snssai_part_rejected_create(
    OpenAPI_snssai_t *snssai,
    OpenAPI_list_t *allowed_tai_list,
    OpenAPI_list_t *rejected_tai_list
)
{
    OpenAPI_snssai_part_rejected_t *snssai_part_rejected_local_var = ogs_malloc(sizeof(OpenAPI_snssai_part_rejected_t));
    ogs_assert(snssai_part_rejected_local_var);

    snssai_part_rejected_local_var->snssai = snssai;
    snssai_part_rejected_local_var->allowed_tai_list = allowed_tai_list;
    snssai_part_rejected_local_var->rejected_tai_list = rejected_tai_list;

    return snssai_part_rejected_local_var;
}

void OpenAPI_snssai_part_rejected_free(OpenAPI_snssai_part_rejected_t *snssai_part_rejected)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == snssai_part_rejected) {
        return;
    }
    if (snssai_part_rejected->snssai) {
        OpenAPI_snssai_free(snssai_part_rejected->snssai);
        snssai_part_rejected->snssai = NULL;
    }
    if (snssai_part_rejected->allowed_tai_list) {
        OpenAPI_list_for_each(snssai_part_rejected->allowed_tai_list, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(snssai_part_rejected->allowed_tai_list);
        snssai_part_rejected->allowed_tai_list = NULL;
    }
    if (snssai_part_rejected->rejected_tai_list) {
        OpenAPI_list_for_each(snssai_part_rejected->rejected_tai_list, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(snssai_part_rejected->rejected_tai_list);
        snssai_part_rejected->rejected_tai_list = NULL;
    }
    ogs_free(snssai_part_rejected);
}

cJSON *OpenAPI_snssai_part_rejected_convertToJSON(OpenAPI_snssai_part_rejected_t *snssai_part_rejected)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (snssai_part_rejected == NULL) {
        ogs_error("OpenAPI_snssai_part_rejected_convertToJSON() failed [SnssaiPartRejected]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!snssai_part_rejected->snssai) {
        ogs_error("OpenAPI_snssai_part_rejected_convertToJSON() failed [snssai]");
        return NULL;
    }
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(snssai_part_rejected->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_snssai_part_rejected_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_snssai_part_rejected_convertToJSON() failed [snssai]");
        goto end;
    }

    if (snssai_part_rejected->allowed_tai_list) {
    cJSON *allowed_tai_listList = cJSON_AddArrayToObject(item, "allowedTaiList");
    if (allowed_tai_listList == NULL) {
        ogs_error("OpenAPI_snssai_part_rejected_convertToJSON() failed [allowed_tai_list]");
        goto end;
    }
    OpenAPI_list_for_each(snssai_part_rejected->allowed_tai_list, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_snssai_part_rejected_convertToJSON() failed [allowed_tai_list]");
            goto end;
        }
        cJSON_AddItemToArray(allowed_tai_listList, itemLocal);
    }
    }

    if (snssai_part_rejected->rejected_tai_list) {
    cJSON *rejected_tai_listList = cJSON_AddArrayToObject(item, "rejectedTaiList");
    if (rejected_tai_listList == NULL) {
        ogs_error("OpenAPI_snssai_part_rejected_convertToJSON() failed [rejected_tai_list]");
        goto end;
    }
    OpenAPI_list_for_each(snssai_part_rejected->rejected_tai_list, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_snssai_part_rejected_convertToJSON() failed [rejected_tai_list]");
            goto end;
        }
        cJSON_AddItemToArray(rejected_tai_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_snssai_part_rejected_t *OpenAPI_snssai_part_rejected_parseFromJSON(cJSON *snssai_part_rejectedJSON)
{
    OpenAPI_snssai_part_rejected_t *snssai_part_rejected_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *allowed_tai_list = NULL;
    OpenAPI_list_t *allowed_tai_listList = NULL;
    cJSON *rejected_tai_list = NULL;
    OpenAPI_list_t *rejected_tai_listList = NULL;
    snssai = cJSON_GetObjectItemCaseSensitive(snssai_part_rejectedJSON, "snssai");
    if (!snssai) {
        ogs_error("OpenAPI_snssai_part_rejected_parseFromJSON() failed [snssai]");
        goto end;
    }
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }

    allowed_tai_list = cJSON_GetObjectItemCaseSensitive(snssai_part_rejectedJSON, "allowedTaiList");
    if (allowed_tai_list) {
        cJSON *allowed_tai_list_local = NULL;
        if (!cJSON_IsArray(allowed_tai_list)) {
            ogs_error("OpenAPI_snssai_part_rejected_parseFromJSON() failed [allowed_tai_list]");
            goto end;
        }

        allowed_tai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_tai_list_local, allowed_tai_list) {
            if (!cJSON_IsObject(allowed_tai_list_local)) {
                ogs_error("OpenAPI_snssai_part_rejected_parseFromJSON() failed [allowed_tai_list]");
                goto end;
            }
            OpenAPI_tai_t *allowed_tai_listItem = OpenAPI_tai_parseFromJSON(allowed_tai_list_local);
            if (!allowed_tai_listItem) {
                ogs_error("No allowed_tai_listItem");
                goto end;
            }
            OpenAPI_list_add(allowed_tai_listList, allowed_tai_listItem);
        }
    }

    rejected_tai_list = cJSON_GetObjectItemCaseSensitive(snssai_part_rejectedJSON, "rejectedTaiList");
    if (rejected_tai_list) {
        cJSON *rejected_tai_list_local = NULL;
        if (!cJSON_IsArray(rejected_tai_list)) {
            ogs_error("OpenAPI_snssai_part_rejected_parseFromJSON() failed [rejected_tai_list]");
            goto end;
        }

        rejected_tai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(rejected_tai_list_local, rejected_tai_list) {
            if (!cJSON_IsObject(rejected_tai_list_local)) {
                ogs_error("OpenAPI_snssai_part_rejected_parseFromJSON() failed [rejected_tai_list]");
                goto end;
            }
            OpenAPI_tai_t *rejected_tai_listItem = OpenAPI_tai_parseFromJSON(rejected_tai_list_local);
            if (!rejected_tai_listItem) {
                ogs_error("No rejected_tai_listItem");
                goto end;
            }
            OpenAPI_list_add(rejected_tai_listList, rejected_tai_listItem);
        }
    }

    snssai_part_rejected_local_var = OpenAPI_snssai_part_rejected_create (
        snssai_local_nonprim,
        allowed_tai_list ? allowed_tai_listList : NULL,
        rejected_tai_list ? rejected_tai_listList : NULL
    );

    return snssai_part_rejected_local_var;
end:
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    if (allowed_tai_listList) {
        OpenAPI_list_for_each(allowed_tai_listList, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(allowed_tai_listList);
        allowed_tai_listList = NULL;
    }
    if (rejected_tai_listList) {
        OpenAPI_list_for_each(rejected_tai_listList, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(rejected_tai_listList);
        rejected_tai_listList = NULL;
    }
    return NULL;
}

OpenAPI_snssai_part_rejected_t *OpenAPI_snssai_part_rejected_copy(OpenAPI_snssai_part_rejected_t *dst, OpenAPI_snssai_part_rejected_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_snssai_part_rejected_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_snssai_part_rejected_convertToJSON() failed");
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

    OpenAPI_snssai_part_rejected_free(dst);
    dst = OpenAPI_snssai_part_rejected_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

