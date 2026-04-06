
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ntn_tai_info.h"

OpenAPI_ntn_tai_info_t *OpenAPI_ntn_tai_info_create(
    OpenAPI_plmn_id_nid_t *plmn_id,
    OpenAPI_list_t *tac_list,
    char *derived_tac
)
{
    OpenAPI_ntn_tai_info_t *ntn_tai_info_local_var = ogs_malloc(sizeof(OpenAPI_ntn_tai_info_t));
    ogs_assert(ntn_tai_info_local_var);

    ntn_tai_info_local_var->plmn_id = plmn_id;
    ntn_tai_info_local_var->tac_list = tac_list;
    ntn_tai_info_local_var->derived_tac = derived_tac;

    return ntn_tai_info_local_var;
}

void OpenAPI_ntn_tai_info_free(OpenAPI_ntn_tai_info_t *ntn_tai_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ntn_tai_info) {
        return;
    }
    if (ntn_tai_info->plmn_id) {
        OpenAPI_plmn_id_nid_free(ntn_tai_info->plmn_id);
        ntn_tai_info->plmn_id = NULL;
    }
    if (ntn_tai_info->tac_list) {
        OpenAPI_list_for_each(ntn_tai_info->tac_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ntn_tai_info->tac_list);
        ntn_tai_info->tac_list = NULL;
    }
    if (ntn_tai_info->derived_tac) {
        ogs_free(ntn_tai_info->derived_tac);
        ntn_tai_info->derived_tac = NULL;
    }
    ogs_free(ntn_tai_info);
}

cJSON *OpenAPI_ntn_tai_info_convertToJSON(OpenAPI_ntn_tai_info_t *ntn_tai_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ntn_tai_info == NULL) {
        ogs_error("OpenAPI_ntn_tai_info_convertToJSON() failed [NtnTaiInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!ntn_tai_info->plmn_id) {
        ogs_error("OpenAPI_ntn_tai_info_convertToJSON() failed [plmn_id]");
        return NULL;
    }
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_nid_convertToJSON(ntn_tai_info->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_ntn_tai_info_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ntn_tai_info_convertToJSON() failed [plmn_id]");
        goto end;
    }

    if (!ntn_tai_info->tac_list) {
        ogs_error("OpenAPI_ntn_tai_info_convertToJSON() failed [tac_list]");
        return NULL;
    }
    cJSON *tac_listList = cJSON_AddArrayToObject(item, "tacList");
    if (tac_listList == NULL) {
        ogs_error("OpenAPI_ntn_tai_info_convertToJSON() failed [tac_list]");
        goto end;
    }
    OpenAPI_list_for_each(ntn_tai_info->tac_list, node) {
        if (cJSON_AddStringToObject(tac_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ntn_tai_info_convertToJSON() failed [tac_list]");
            goto end;
        }
    }

    if (ntn_tai_info->derived_tac) {
    if (cJSON_AddStringToObject(item, "derivedTac", ntn_tai_info->derived_tac) == NULL) {
        ogs_error("OpenAPI_ntn_tai_info_convertToJSON() failed [derived_tac]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ntn_tai_info_t *OpenAPI_ntn_tai_info_parseFromJSON(cJSON *ntn_tai_infoJSON)
{
    OpenAPI_ntn_tai_info_t *ntn_tai_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_nid_t *plmn_id_local_nonprim = NULL;
    cJSON *tac_list = NULL;
    OpenAPI_list_t *tac_listList = NULL;
    cJSON *derived_tac = NULL;
    plmn_id = cJSON_GetObjectItemCaseSensitive(ntn_tai_infoJSON, "plmnId");
    if (!plmn_id) {
        ogs_error("OpenAPI_ntn_tai_info_parseFromJSON() failed [plmn_id]");
        goto end;
    }
    plmn_id_local_nonprim = OpenAPI_plmn_id_nid_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_nid_parseFromJSON failed [plmn_id]");
        goto end;
    }

    tac_list = cJSON_GetObjectItemCaseSensitive(ntn_tai_infoJSON, "tacList");
    if (!tac_list) {
        ogs_error("OpenAPI_ntn_tai_info_parseFromJSON() failed [tac_list]");
        goto end;
    }
        cJSON *tac_list_local = NULL;
        if (!cJSON_IsArray(tac_list)) {
            ogs_error("OpenAPI_ntn_tai_info_parseFromJSON() failed [tac_list]");
            goto end;
        }

        tac_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tac_list_local, tac_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(tac_list_local)) {
                ogs_error("OpenAPI_ntn_tai_info_parseFromJSON() failed [tac_list]");
                goto end;
            }
            OpenAPI_list_add(tac_listList, ogs_strdup(tac_list_local->valuestring));
        }

    derived_tac = cJSON_GetObjectItemCaseSensitive(ntn_tai_infoJSON, "derivedTac");
    if (derived_tac) {
    if (!cJSON_IsString(derived_tac) && !cJSON_IsNull(derived_tac)) {
        ogs_error("OpenAPI_ntn_tai_info_parseFromJSON() failed [derived_tac]");
        goto end;
    }
    }

    ntn_tai_info_local_var = OpenAPI_ntn_tai_info_create (
        plmn_id_local_nonprim,
        tac_listList,
        derived_tac && !cJSON_IsNull(derived_tac) ? ogs_strdup(derived_tac->valuestring) : NULL
    );

    return ntn_tai_info_local_var;
end:
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_nid_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
    if (tac_listList) {
        OpenAPI_list_for_each(tac_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(tac_listList);
        tac_listList = NULL;
    }
    return NULL;
}

OpenAPI_ntn_tai_info_t *OpenAPI_ntn_tai_info_copy(OpenAPI_ntn_tai_info_t *dst, OpenAPI_ntn_tai_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ntn_tai_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ntn_tai_info_convertToJSON() failed");
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

    OpenAPI_ntn_tai_info_free(dst);
    dst = OpenAPI_ntn_tai_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

