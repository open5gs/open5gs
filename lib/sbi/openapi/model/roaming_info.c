
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "roaming_info.h"

OpenAPI_roaming_info_t *OpenAPI_roaming_info_create(
    OpenAPI_plmn_id_nid_t *plmn_id,
    OpenAPI_list_t *aois,
    OpenAPI_list_t *serving_nf_ids,
    OpenAPI_list_t *serving_nf_set_ids
)
{
    OpenAPI_roaming_info_t *roaming_info_local_var = ogs_malloc(sizeof(OpenAPI_roaming_info_t));
    ogs_assert(roaming_info_local_var);

    roaming_info_local_var->plmn_id = plmn_id;
    roaming_info_local_var->aois = aois;
    roaming_info_local_var->serving_nf_ids = serving_nf_ids;
    roaming_info_local_var->serving_nf_set_ids = serving_nf_set_ids;

    return roaming_info_local_var;
}

void OpenAPI_roaming_info_free(OpenAPI_roaming_info_t *roaming_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == roaming_info) {
        return;
    }
    if (roaming_info->plmn_id) {
        OpenAPI_plmn_id_nid_free(roaming_info->plmn_id);
        roaming_info->plmn_id = NULL;
    }
    if (roaming_info->aois) {
        OpenAPI_list_for_each(roaming_info->aois, node) {
            OpenAPI_geographical_area_free(node->data);
        }
        OpenAPI_list_free(roaming_info->aois);
        roaming_info->aois = NULL;
    }
    if (roaming_info->serving_nf_ids) {
        OpenAPI_list_for_each(roaming_info->serving_nf_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(roaming_info->serving_nf_ids);
        roaming_info->serving_nf_ids = NULL;
    }
    if (roaming_info->serving_nf_set_ids) {
        OpenAPI_list_for_each(roaming_info->serving_nf_set_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(roaming_info->serving_nf_set_ids);
        roaming_info->serving_nf_set_ids = NULL;
    }
    ogs_free(roaming_info);
}

cJSON *OpenAPI_roaming_info_convertToJSON(OpenAPI_roaming_info_t *roaming_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (roaming_info == NULL) {
        ogs_error("OpenAPI_roaming_info_convertToJSON() failed [RoamingInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (roaming_info->plmn_id) {
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_nid_convertToJSON(roaming_info->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_roaming_info_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_roaming_info_convertToJSON() failed [plmn_id]");
        goto end;
    }
    }

    if (roaming_info->aois) {
    cJSON *aoisList = cJSON_AddArrayToObject(item, "aois");
    if (aoisList == NULL) {
        ogs_error("OpenAPI_roaming_info_convertToJSON() failed [aois]");
        goto end;
    }
    OpenAPI_list_for_each(roaming_info->aois, node) {
        cJSON *itemLocal = OpenAPI_geographical_area_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_roaming_info_convertToJSON() failed [aois]");
            goto end;
        }
        cJSON_AddItemToArray(aoisList, itemLocal);
    }
    }

    if (roaming_info->serving_nf_ids) {
    cJSON *serving_nf_idsList = cJSON_AddArrayToObject(item, "servingNfIds");
    if (serving_nf_idsList == NULL) {
        ogs_error("OpenAPI_roaming_info_convertToJSON() failed [serving_nf_ids]");
        goto end;
    }
    OpenAPI_list_for_each(roaming_info->serving_nf_ids, node) {
        if (cJSON_AddStringToObject(serving_nf_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_roaming_info_convertToJSON() failed [serving_nf_ids]");
            goto end;
        }
    }
    }

    if (roaming_info->serving_nf_set_ids) {
    cJSON *serving_nf_set_idsList = cJSON_AddArrayToObject(item, "servingNfSetIds");
    if (serving_nf_set_idsList == NULL) {
        ogs_error("OpenAPI_roaming_info_convertToJSON() failed [serving_nf_set_ids]");
        goto end;
    }
    OpenAPI_list_for_each(roaming_info->serving_nf_set_ids, node) {
        if (cJSON_AddStringToObject(serving_nf_set_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_roaming_info_convertToJSON() failed [serving_nf_set_ids]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_roaming_info_t *OpenAPI_roaming_info_parseFromJSON(cJSON *roaming_infoJSON)
{
    OpenAPI_roaming_info_t *roaming_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_nid_t *plmn_id_local_nonprim = NULL;
    cJSON *aois = NULL;
    OpenAPI_list_t *aoisList = NULL;
    cJSON *serving_nf_ids = NULL;
    OpenAPI_list_t *serving_nf_idsList = NULL;
    cJSON *serving_nf_set_ids = NULL;
    OpenAPI_list_t *serving_nf_set_idsList = NULL;
    plmn_id = cJSON_GetObjectItemCaseSensitive(roaming_infoJSON, "plmnId");
    if (plmn_id) {
    plmn_id_local_nonprim = OpenAPI_plmn_id_nid_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_nid_parseFromJSON failed [plmn_id]");
        goto end;
    }
    }

    aois = cJSON_GetObjectItemCaseSensitive(roaming_infoJSON, "aois");
    if (aois) {
        cJSON *aois_local = NULL;
        if (!cJSON_IsArray(aois)) {
            ogs_error("OpenAPI_roaming_info_parseFromJSON() failed [aois]");
            goto end;
        }

        aoisList = OpenAPI_list_create();

        cJSON_ArrayForEach(aois_local, aois) {
            if (!cJSON_IsObject(aois_local)) {
                ogs_error("OpenAPI_roaming_info_parseFromJSON() failed [aois]");
                goto end;
            }
            OpenAPI_geographical_area_t *aoisItem = OpenAPI_geographical_area_parseFromJSON(aois_local);
            if (!aoisItem) {
                ogs_error("No aoisItem");
                goto end;
            }
            OpenAPI_list_add(aoisList, aoisItem);
        }
    }

    serving_nf_ids = cJSON_GetObjectItemCaseSensitive(roaming_infoJSON, "servingNfIds");
    if (serving_nf_ids) {
        cJSON *serving_nf_ids_local = NULL;
        if (!cJSON_IsArray(serving_nf_ids)) {
            ogs_error("OpenAPI_roaming_info_parseFromJSON() failed [serving_nf_ids]");
            goto end;
        }

        serving_nf_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(serving_nf_ids_local, serving_nf_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(serving_nf_ids_local)) {
                ogs_error("OpenAPI_roaming_info_parseFromJSON() failed [serving_nf_ids]");
                goto end;
            }
            OpenAPI_list_add(serving_nf_idsList, ogs_strdup(serving_nf_ids_local->valuestring));
        }
    }

    serving_nf_set_ids = cJSON_GetObjectItemCaseSensitive(roaming_infoJSON, "servingNfSetIds");
    if (serving_nf_set_ids) {
        cJSON *serving_nf_set_ids_local = NULL;
        if (!cJSON_IsArray(serving_nf_set_ids)) {
            ogs_error("OpenAPI_roaming_info_parseFromJSON() failed [serving_nf_set_ids]");
            goto end;
        }

        serving_nf_set_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(serving_nf_set_ids_local, serving_nf_set_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(serving_nf_set_ids_local)) {
                ogs_error("OpenAPI_roaming_info_parseFromJSON() failed [serving_nf_set_ids]");
                goto end;
            }
            OpenAPI_list_add(serving_nf_set_idsList, ogs_strdup(serving_nf_set_ids_local->valuestring));
        }
    }

    roaming_info_local_var = OpenAPI_roaming_info_create (
        plmn_id ? plmn_id_local_nonprim : NULL,
        aois ? aoisList : NULL,
        serving_nf_ids ? serving_nf_idsList : NULL,
        serving_nf_set_ids ? serving_nf_set_idsList : NULL
    );

    return roaming_info_local_var;
end:
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_nid_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
    if (aoisList) {
        OpenAPI_list_for_each(aoisList, node) {
            OpenAPI_geographical_area_free(node->data);
        }
        OpenAPI_list_free(aoisList);
        aoisList = NULL;
    }
    if (serving_nf_idsList) {
        OpenAPI_list_for_each(serving_nf_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(serving_nf_idsList);
        serving_nf_idsList = NULL;
    }
    if (serving_nf_set_idsList) {
        OpenAPI_list_for_each(serving_nf_set_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(serving_nf_set_idsList);
        serving_nf_set_idsList = NULL;
    }
    return NULL;
}

OpenAPI_roaming_info_t *OpenAPI_roaming_info_copy(OpenAPI_roaming_info_t *dst, OpenAPI_roaming_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_roaming_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_roaming_info_convertToJSON() failed");
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

    OpenAPI_roaming_info_free(dst);
    dst = OpenAPI_roaming_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

