
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "supported_plmn.h"

OpenAPI_supported_plmn_t *OpenAPI_supported_plmn_create(
    OpenAPI_plmn_id_t *plmn_id,
    OpenAPI_list_t *ecsp_ids
)
{
    OpenAPI_supported_plmn_t *supported_plmn_local_var = ogs_malloc(sizeof(OpenAPI_supported_plmn_t));
    ogs_assert(supported_plmn_local_var);

    supported_plmn_local_var->plmn_id = plmn_id;
    supported_plmn_local_var->ecsp_ids = ecsp_ids;

    return supported_plmn_local_var;
}

void OpenAPI_supported_plmn_free(OpenAPI_supported_plmn_t *supported_plmn)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == supported_plmn) {
        return;
    }
    if (supported_plmn->plmn_id) {
        OpenAPI_plmn_id_free(supported_plmn->plmn_id);
        supported_plmn->plmn_id = NULL;
    }
    if (supported_plmn->ecsp_ids) {
        OpenAPI_list_for_each(supported_plmn->ecsp_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(supported_plmn->ecsp_ids);
        supported_plmn->ecsp_ids = NULL;
    }
    ogs_free(supported_plmn);
}

cJSON *OpenAPI_supported_plmn_convertToJSON(OpenAPI_supported_plmn_t *supported_plmn)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (supported_plmn == NULL) {
        ogs_error("OpenAPI_supported_plmn_convertToJSON() failed [SupportedPlmn]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!supported_plmn->plmn_id) {
        ogs_error("OpenAPI_supported_plmn_convertToJSON() failed [plmn_id]");
        return NULL;
    }
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(supported_plmn->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_supported_plmn_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_supported_plmn_convertToJSON() failed [plmn_id]");
        goto end;
    }

    if (supported_plmn->ecsp_ids) {
    cJSON *ecsp_idsList = cJSON_AddArrayToObject(item, "ecspIds");
    if (ecsp_idsList == NULL) {
        ogs_error("OpenAPI_supported_plmn_convertToJSON() failed [ecsp_ids]");
        goto end;
    }
    OpenAPI_list_for_each(supported_plmn->ecsp_ids, node) {
        if (cJSON_AddStringToObject(ecsp_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_supported_plmn_convertToJSON() failed [ecsp_ids]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_supported_plmn_t *OpenAPI_supported_plmn_parseFromJSON(cJSON *supported_plmnJSON)
{
    OpenAPI_supported_plmn_t *supported_plmn_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;
    cJSON *ecsp_ids = NULL;
    OpenAPI_list_t *ecsp_idsList = NULL;
    plmn_id = cJSON_GetObjectItemCaseSensitive(supported_plmnJSON, "plmnId");
    if (!plmn_id) {
        ogs_error("OpenAPI_supported_plmn_parseFromJSON() failed [plmn_id]");
        goto end;
    }
    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [plmn_id]");
        goto end;
    }

    ecsp_ids = cJSON_GetObjectItemCaseSensitive(supported_plmnJSON, "ecspIds");
    if (ecsp_ids) {
        cJSON *ecsp_ids_local = NULL;
        if (!cJSON_IsArray(ecsp_ids)) {
            ogs_error("OpenAPI_supported_plmn_parseFromJSON() failed [ecsp_ids]");
            goto end;
        }

        ecsp_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ecsp_ids_local, ecsp_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ecsp_ids_local)) {
                ogs_error("OpenAPI_supported_plmn_parseFromJSON() failed [ecsp_ids]");
                goto end;
            }
            OpenAPI_list_add(ecsp_idsList, ogs_strdup(ecsp_ids_local->valuestring));
        }
    }

    supported_plmn_local_var = OpenAPI_supported_plmn_create (
        plmn_id_local_nonprim,
        ecsp_ids ? ecsp_idsList : NULL
    );

    return supported_plmn_local_var;
end:
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
    if (ecsp_idsList) {
        OpenAPI_list_for_each(ecsp_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ecsp_idsList);
        ecsp_idsList = NULL;
    }
    return NULL;
}

OpenAPI_supported_plmn_t *OpenAPI_supported_plmn_copy(OpenAPI_supported_plmn_t *dst, OpenAPI_supported_plmn_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_supported_plmn_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_supported_plmn_convertToJSON() failed");
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

    OpenAPI_supported_plmn_free(dst);
    dst = OpenAPI_supported_plmn_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

