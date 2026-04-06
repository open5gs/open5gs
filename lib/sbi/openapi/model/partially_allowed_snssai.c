
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "partially_allowed_snssai.h"

OpenAPI_partially_allowed_snssai_t *OpenAPI_partially_allowed_snssai_create(
    OpenAPI_snssai_t *snssai,
    OpenAPI_list_t *allowed_tai_list
)
{
    OpenAPI_partially_allowed_snssai_t *partially_allowed_snssai_local_var = ogs_malloc(sizeof(OpenAPI_partially_allowed_snssai_t));
    ogs_assert(partially_allowed_snssai_local_var);

    partially_allowed_snssai_local_var->snssai = snssai;
    partially_allowed_snssai_local_var->allowed_tai_list = allowed_tai_list;

    return partially_allowed_snssai_local_var;
}

void OpenAPI_partially_allowed_snssai_free(OpenAPI_partially_allowed_snssai_t *partially_allowed_snssai)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == partially_allowed_snssai) {
        return;
    }
    if (partially_allowed_snssai->snssai) {
        OpenAPI_snssai_free(partially_allowed_snssai->snssai);
        partially_allowed_snssai->snssai = NULL;
    }
    if (partially_allowed_snssai->allowed_tai_list) {
        OpenAPI_list_for_each(partially_allowed_snssai->allowed_tai_list, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(partially_allowed_snssai->allowed_tai_list);
        partially_allowed_snssai->allowed_tai_list = NULL;
    }
    ogs_free(partially_allowed_snssai);
}

cJSON *OpenAPI_partially_allowed_snssai_convertToJSON(OpenAPI_partially_allowed_snssai_t *partially_allowed_snssai)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (partially_allowed_snssai == NULL) {
        ogs_error("OpenAPI_partially_allowed_snssai_convertToJSON() failed [PartiallyAllowedSnssai]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!partially_allowed_snssai->snssai) {
        ogs_error("OpenAPI_partially_allowed_snssai_convertToJSON() failed [snssai]");
        return NULL;
    }
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(partially_allowed_snssai->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_partially_allowed_snssai_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_partially_allowed_snssai_convertToJSON() failed [snssai]");
        goto end;
    }

    if (!partially_allowed_snssai->allowed_tai_list) {
        ogs_error("OpenAPI_partially_allowed_snssai_convertToJSON() failed [allowed_tai_list]");
        return NULL;
    }
    cJSON *allowed_tai_listList = cJSON_AddArrayToObject(item, "allowedTaiList");
    if (allowed_tai_listList == NULL) {
        ogs_error("OpenAPI_partially_allowed_snssai_convertToJSON() failed [allowed_tai_list]");
        goto end;
    }
    OpenAPI_list_for_each(partially_allowed_snssai->allowed_tai_list, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_partially_allowed_snssai_convertToJSON() failed [allowed_tai_list]");
            goto end;
        }
        cJSON_AddItemToArray(allowed_tai_listList, itemLocal);
    }

end:
    return item;
}

OpenAPI_partially_allowed_snssai_t *OpenAPI_partially_allowed_snssai_parseFromJSON(cJSON *partially_allowed_snssaiJSON)
{
    OpenAPI_partially_allowed_snssai_t *partially_allowed_snssai_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *allowed_tai_list = NULL;
    OpenAPI_list_t *allowed_tai_listList = NULL;
    snssai = cJSON_GetObjectItemCaseSensitive(partially_allowed_snssaiJSON, "snssai");
    if (!snssai) {
        ogs_error("OpenAPI_partially_allowed_snssai_parseFromJSON() failed [snssai]");
        goto end;
    }
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }

    allowed_tai_list = cJSON_GetObjectItemCaseSensitive(partially_allowed_snssaiJSON, "allowedTaiList");
    if (!allowed_tai_list) {
        ogs_error("OpenAPI_partially_allowed_snssai_parseFromJSON() failed [allowed_tai_list]");
        goto end;
    }
        cJSON *allowed_tai_list_local = NULL;
        if (!cJSON_IsArray(allowed_tai_list)) {
            ogs_error("OpenAPI_partially_allowed_snssai_parseFromJSON() failed [allowed_tai_list]");
            goto end;
        }

        allowed_tai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_tai_list_local, allowed_tai_list) {
            if (!cJSON_IsObject(allowed_tai_list_local)) {
                ogs_error("OpenAPI_partially_allowed_snssai_parseFromJSON() failed [allowed_tai_list]");
                goto end;
            }
            OpenAPI_tai_t *allowed_tai_listItem = OpenAPI_tai_parseFromJSON(allowed_tai_list_local);
            if (!allowed_tai_listItem) {
                ogs_error("No allowed_tai_listItem");
                goto end;
            }
            OpenAPI_list_add(allowed_tai_listList, allowed_tai_listItem);
        }

    partially_allowed_snssai_local_var = OpenAPI_partially_allowed_snssai_create (
        snssai_local_nonprim,
        allowed_tai_listList
    );

    return partially_allowed_snssai_local_var;
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
    return NULL;
}

OpenAPI_partially_allowed_snssai_t *OpenAPI_partially_allowed_snssai_copy(OpenAPI_partially_allowed_snssai_t *dst, OpenAPI_partially_allowed_snssai_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_partially_allowed_snssai_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_partially_allowed_snssai_convertToJSON() failed");
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

    OpenAPI_partially_allowed_snssai_free(dst);
    dst = OpenAPI_partially_allowed_snssai_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

