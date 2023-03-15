
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nsi_id_info.h"

OpenAPI_nsi_id_info_t *OpenAPI_nsi_id_info_create(
    OpenAPI_snssai_t *snssai,
    OpenAPI_list_t *nsi_ids
)
{
    OpenAPI_nsi_id_info_t *nsi_id_info_local_var = ogs_malloc(sizeof(OpenAPI_nsi_id_info_t));
    ogs_assert(nsi_id_info_local_var);

    nsi_id_info_local_var->snssai = snssai;
    nsi_id_info_local_var->nsi_ids = nsi_ids;

    return nsi_id_info_local_var;
}

void OpenAPI_nsi_id_info_free(OpenAPI_nsi_id_info_t *nsi_id_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nsi_id_info) {
        return;
    }
    if (nsi_id_info->snssai) {
        OpenAPI_snssai_free(nsi_id_info->snssai);
        nsi_id_info->snssai = NULL;
    }
    if (nsi_id_info->nsi_ids) {
        OpenAPI_list_for_each(nsi_id_info->nsi_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nsi_id_info->nsi_ids);
        nsi_id_info->nsi_ids = NULL;
    }
    ogs_free(nsi_id_info);
}

cJSON *OpenAPI_nsi_id_info_convertToJSON(OpenAPI_nsi_id_info_t *nsi_id_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nsi_id_info == NULL) {
        ogs_error("OpenAPI_nsi_id_info_convertToJSON() failed [NsiIdInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!nsi_id_info->snssai) {
        ogs_error("OpenAPI_nsi_id_info_convertToJSON() failed [snssai]");
        return NULL;
    }
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(nsi_id_info->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_nsi_id_info_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nsi_id_info_convertToJSON() failed [snssai]");
        goto end;
    }

    if (nsi_id_info->nsi_ids) {
    cJSON *nsi_idsList = cJSON_AddArrayToObject(item, "nsiIds");
    if (nsi_idsList == NULL) {
        ogs_error("OpenAPI_nsi_id_info_convertToJSON() failed [nsi_ids]");
        goto end;
    }
    OpenAPI_list_for_each(nsi_id_info->nsi_ids, node) {
        if (cJSON_AddStringToObject(nsi_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_nsi_id_info_convertToJSON() failed [nsi_ids]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_nsi_id_info_t *OpenAPI_nsi_id_info_parseFromJSON(cJSON *nsi_id_infoJSON)
{
    OpenAPI_nsi_id_info_t *nsi_id_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *nsi_ids = NULL;
    OpenAPI_list_t *nsi_idsList = NULL;
    snssai = cJSON_GetObjectItemCaseSensitive(nsi_id_infoJSON, "snssai");
    if (!snssai) {
        ogs_error("OpenAPI_nsi_id_info_parseFromJSON() failed [snssai]");
        goto end;
    }
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }

    nsi_ids = cJSON_GetObjectItemCaseSensitive(nsi_id_infoJSON, "nsiIds");
    if (nsi_ids) {
        cJSON *nsi_ids_local = NULL;
        if (!cJSON_IsArray(nsi_ids)) {
            ogs_error("OpenAPI_nsi_id_info_parseFromJSON() failed [nsi_ids]");
            goto end;
        }

        nsi_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(nsi_ids_local, nsi_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(nsi_ids_local)) {
                ogs_error("OpenAPI_nsi_id_info_parseFromJSON() failed [nsi_ids]");
                goto end;
            }
            OpenAPI_list_add(nsi_idsList, ogs_strdup(nsi_ids_local->valuestring));
        }
    }

    nsi_id_info_local_var = OpenAPI_nsi_id_info_create (
        snssai_local_nonprim,
        nsi_ids ? nsi_idsList : NULL
    );

    return nsi_id_info_local_var;
end:
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    if (nsi_idsList) {
        OpenAPI_list_for_each(nsi_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nsi_idsList);
        nsi_idsList = NULL;
    }
    return NULL;
}

OpenAPI_nsi_id_info_t *OpenAPI_nsi_id_info_copy(OpenAPI_nsi_id_info_t *dst, OpenAPI_nsi_id_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nsi_id_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nsi_id_info_convertToJSON() failed");
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

    OpenAPI_nsi_id_info_free(dst);
    dst = OpenAPI_nsi_id_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

