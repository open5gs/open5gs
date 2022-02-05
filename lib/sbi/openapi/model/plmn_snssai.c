
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "plmn_snssai.h"

OpenAPI_plmn_snssai_t *OpenAPI_plmn_snssai_create(
    OpenAPI_plmn_id_t *plmn_id,
    OpenAPI_list_t *s_nssai_list,
    char *nid
)
{
    OpenAPI_plmn_snssai_t *plmn_snssai_local_var = ogs_malloc(sizeof(OpenAPI_plmn_snssai_t));
    ogs_assert(plmn_snssai_local_var);

    plmn_snssai_local_var->plmn_id = plmn_id;
    plmn_snssai_local_var->s_nssai_list = s_nssai_list;
    plmn_snssai_local_var->nid = nid;

    return plmn_snssai_local_var;
}

void OpenAPI_plmn_snssai_free(OpenAPI_plmn_snssai_t *plmn_snssai)
{
    if (NULL == plmn_snssai) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_plmn_id_free(plmn_snssai->plmn_id);
    OpenAPI_list_for_each(plmn_snssai->s_nssai_list, node) {
        OpenAPI_ext_snssai_free(node->data);
    }
    OpenAPI_list_free(plmn_snssai->s_nssai_list);
    ogs_free(plmn_snssai->nid);
    ogs_free(plmn_snssai);
}

cJSON *OpenAPI_plmn_snssai_convertToJSON(OpenAPI_plmn_snssai_t *plmn_snssai)
{
    cJSON *item = NULL;

    if (plmn_snssai == NULL) {
        ogs_error("OpenAPI_plmn_snssai_convertToJSON() failed [PlmnSnssai]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(plmn_snssai->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_plmn_snssai_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_plmn_snssai_convertToJSON() failed [plmn_id]");
        goto end;
    }

    cJSON *s_nssai_listList = cJSON_AddArrayToObject(item, "sNssaiList");
    if (s_nssai_listList == NULL) {
        ogs_error("OpenAPI_plmn_snssai_convertToJSON() failed [s_nssai_list]");
        goto end;
    }

    OpenAPI_lnode_t *s_nssai_list_node;
    if (plmn_snssai->s_nssai_list) {
        OpenAPI_list_for_each(plmn_snssai->s_nssai_list, s_nssai_list_node) {
            cJSON *itemLocal = OpenAPI_ext_snssai_convertToJSON(s_nssai_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_plmn_snssai_convertToJSON() failed [s_nssai_list]");
                goto end;
            }
            cJSON_AddItemToArray(s_nssai_listList, itemLocal);
        }
    }

    if (plmn_snssai->nid) {
    if (cJSON_AddStringToObject(item, "nid", plmn_snssai->nid) == NULL) {
        ogs_error("OpenAPI_plmn_snssai_convertToJSON() failed [nid]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_plmn_snssai_t *OpenAPI_plmn_snssai_parseFromJSON(cJSON *plmn_snssaiJSON)
{
    OpenAPI_plmn_snssai_t *plmn_snssai_local_var = NULL;
    cJSON *plmn_id = cJSON_GetObjectItemCaseSensitive(plmn_snssaiJSON, "plmnId");
    if (!plmn_id) {
        ogs_error("OpenAPI_plmn_snssai_parseFromJSON() failed [plmn_id]");
        goto end;
    }

    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;
    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);

    cJSON *s_nssai_list = cJSON_GetObjectItemCaseSensitive(plmn_snssaiJSON, "sNssaiList");
    if (!s_nssai_list) {
        ogs_error("OpenAPI_plmn_snssai_parseFromJSON() failed [s_nssai_list]");
        goto end;
    }

    OpenAPI_list_t *s_nssai_listList;
    cJSON *s_nssai_list_local_nonprimitive;
    if (!cJSON_IsArray(s_nssai_list)){
        ogs_error("OpenAPI_plmn_snssai_parseFromJSON() failed [s_nssai_list]");
        goto end;
    }

    s_nssai_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(s_nssai_list_local_nonprimitive, s_nssai_list ) {
        if (!cJSON_IsObject(s_nssai_list_local_nonprimitive)) {
            ogs_error("OpenAPI_plmn_snssai_parseFromJSON() failed [s_nssai_list]");
            goto end;
        }
        OpenAPI_ext_snssai_t *s_nssai_listItem = OpenAPI_ext_snssai_parseFromJSON(s_nssai_list_local_nonprimitive);

        if (!s_nssai_listItem) {
            ogs_error("No s_nssai_listItem");
            OpenAPI_list_free(s_nssai_listList);
            goto end;
        }

        OpenAPI_list_add(s_nssai_listList, s_nssai_listItem);
    }

    cJSON *nid = cJSON_GetObjectItemCaseSensitive(plmn_snssaiJSON, "nid");

    if (nid) {
    if (!cJSON_IsString(nid)) {
        ogs_error("OpenAPI_plmn_snssai_parseFromJSON() failed [nid]");
        goto end;
    }
    }

    plmn_snssai_local_var = OpenAPI_plmn_snssai_create (
        plmn_id_local_nonprim,
        s_nssai_listList,
        nid ? ogs_strdup(nid->valuestring) : NULL
    );

    return plmn_snssai_local_var;
end:
    return NULL;
}

OpenAPI_plmn_snssai_t *OpenAPI_plmn_snssai_copy(OpenAPI_plmn_snssai_t *dst, OpenAPI_plmn_snssai_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_plmn_snssai_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_plmn_snssai_convertToJSON() failed");
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

    OpenAPI_plmn_snssai_free(dst);
    dst = OpenAPI_plmn_snssai_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

