
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tai_range.h"

OpenAPI_tai_range_t *OpenAPI_tai_range_create(
    OpenAPI_plmn_id_t *plmn_id,
    OpenAPI_list_t *tac_range_list,
    char *nid
)
{
    OpenAPI_tai_range_t *tai_range_local_var = ogs_malloc(sizeof(OpenAPI_tai_range_t));
    ogs_assert(tai_range_local_var);

    tai_range_local_var->plmn_id = plmn_id;
    tai_range_local_var->tac_range_list = tac_range_list;
    tai_range_local_var->nid = nid;

    return tai_range_local_var;
}

void OpenAPI_tai_range_free(OpenAPI_tai_range_t *tai_range)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == tai_range) {
        return;
    }
    if (tai_range->plmn_id) {
        OpenAPI_plmn_id_free(tai_range->plmn_id);
        tai_range->plmn_id = NULL;
    }
    if (tai_range->tac_range_list) {
        OpenAPI_list_for_each(tai_range->tac_range_list, node) {
            OpenAPI_tac_range_free(node->data);
        }
        OpenAPI_list_free(tai_range->tac_range_list);
        tai_range->tac_range_list = NULL;
    }
    if (tai_range->nid) {
        ogs_free(tai_range->nid);
        tai_range->nid = NULL;
    }
    ogs_free(tai_range);
}

cJSON *OpenAPI_tai_range_convertToJSON(OpenAPI_tai_range_t *tai_range)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (tai_range == NULL) {
        ogs_error("OpenAPI_tai_range_convertToJSON() failed [TaiRange]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!tai_range->plmn_id) {
        ogs_error("OpenAPI_tai_range_convertToJSON() failed [plmn_id]");
        return NULL;
    }
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(tai_range->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_tai_range_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_tai_range_convertToJSON() failed [plmn_id]");
        goto end;
    }

    if (!tai_range->tac_range_list) {
        ogs_error("OpenAPI_tai_range_convertToJSON() failed [tac_range_list]");
        return NULL;
    }
    cJSON *tac_range_listList = cJSON_AddArrayToObject(item, "tacRangeList");
    if (tac_range_listList == NULL) {
        ogs_error("OpenAPI_tai_range_convertToJSON() failed [tac_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(tai_range->tac_range_list, node) {
        cJSON *itemLocal = OpenAPI_tac_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_tai_range_convertToJSON() failed [tac_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(tac_range_listList, itemLocal);
    }

    if (tai_range->nid) {
    if (cJSON_AddStringToObject(item, "nid", tai_range->nid) == NULL) {
        ogs_error("OpenAPI_tai_range_convertToJSON() failed [nid]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_tai_range_t *OpenAPI_tai_range_parseFromJSON(cJSON *tai_rangeJSON)
{
    OpenAPI_tai_range_t *tai_range_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;
    cJSON *tac_range_list = NULL;
    OpenAPI_list_t *tac_range_listList = NULL;
    cJSON *nid = NULL;
    plmn_id = cJSON_GetObjectItemCaseSensitive(tai_rangeJSON, "plmnId");
    if (!plmn_id) {
        ogs_error("OpenAPI_tai_range_parseFromJSON() failed [plmn_id]");
        goto end;
    }
    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [plmn_id]");
        goto end;
    }

    tac_range_list = cJSON_GetObjectItemCaseSensitive(tai_rangeJSON, "tacRangeList");
    if (!tac_range_list) {
        ogs_error("OpenAPI_tai_range_parseFromJSON() failed [tac_range_list]");
        goto end;
    }
        cJSON *tac_range_list_local = NULL;
        if (!cJSON_IsArray(tac_range_list)) {
            ogs_error("OpenAPI_tai_range_parseFromJSON() failed [tac_range_list]");
            goto end;
        }

        tac_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tac_range_list_local, tac_range_list) {
            if (!cJSON_IsObject(tac_range_list_local)) {
                ogs_error("OpenAPI_tai_range_parseFromJSON() failed [tac_range_list]");
                goto end;
            }
            OpenAPI_tac_range_t *tac_range_listItem = OpenAPI_tac_range_parseFromJSON(tac_range_list_local);
            if (!tac_range_listItem) {
                ogs_error("No tac_range_listItem");
                goto end;
            }
            OpenAPI_list_add(tac_range_listList, tac_range_listItem);
        }

    nid = cJSON_GetObjectItemCaseSensitive(tai_rangeJSON, "nid");
    if (nid) {
    if (!cJSON_IsString(nid) && !cJSON_IsNull(nid)) {
        ogs_error("OpenAPI_tai_range_parseFromJSON() failed [nid]");
        goto end;
    }
    }

    tai_range_local_var = OpenAPI_tai_range_create (
        plmn_id_local_nonprim,
        tac_range_listList,
        nid && !cJSON_IsNull(nid) ? ogs_strdup(nid->valuestring) : NULL
    );

    return tai_range_local_var;
end:
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
    if (tac_range_listList) {
        OpenAPI_list_for_each(tac_range_listList, node) {
            OpenAPI_tac_range_free(node->data);
        }
        OpenAPI_list_free(tac_range_listList);
        tac_range_listList = NULL;
    }
    return NULL;
}

OpenAPI_tai_range_t *OpenAPI_tai_range_copy(OpenAPI_tai_range_t *dst, OpenAPI_tai_range_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_tai_range_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_tai_range_convertToJSON() failed");
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

    OpenAPI_tai_range_free(dst);
    dst = OpenAPI_tai_range_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

