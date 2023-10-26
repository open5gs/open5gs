
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pro_se_allowed_plmn.h"

OpenAPI_pro_se_allowed_plmn_t *OpenAPI_pro_se_allowed_plmn_create(
    OpenAPI_plmn_id_t *visited_plmn,
    OpenAPI_list_t *prose_direct_allowed
)
{
    OpenAPI_pro_se_allowed_plmn_t *pro_se_allowed_plmn_local_var = ogs_malloc(sizeof(OpenAPI_pro_se_allowed_plmn_t));
    ogs_assert(pro_se_allowed_plmn_local_var);

    pro_se_allowed_plmn_local_var->visited_plmn = visited_plmn;
    pro_se_allowed_plmn_local_var->prose_direct_allowed = prose_direct_allowed;

    return pro_se_allowed_plmn_local_var;
}

void OpenAPI_pro_se_allowed_plmn_free(OpenAPI_pro_se_allowed_plmn_t *pro_se_allowed_plmn)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pro_se_allowed_plmn) {
        return;
    }
    if (pro_se_allowed_plmn->visited_plmn) {
        OpenAPI_plmn_id_free(pro_se_allowed_plmn->visited_plmn);
        pro_se_allowed_plmn->visited_plmn = NULL;
    }
    if (pro_se_allowed_plmn->prose_direct_allowed) {
        OpenAPI_list_free(pro_se_allowed_plmn->prose_direct_allowed);
        pro_se_allowed_plmn->prose_direct_allowed = NULL;
    }
    ogs_free(pro_se_allowed_plmn);
}

cJSON *OpenAPI_pro_se_allowed_plmn_convertToJSON(OpenAPI_pro_se_allowed_plmn_t *pro_se_allowed_plmn)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pro_se_allowed_plmn == NULL) {
        ogs_error("OpenAPI_pro_se_allowed_plmn_convertToJSON() failed [ProSeAllowedPlmn]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!pro_se_allowed_plmn->visited_plmn) {
        ogs_error("OpenAPI_pro_se_allowed_plmn_convertToJSON() failed [visited_plmn]");
        return NULL;
    }
    cJSON *visited_plmn_local_JSON = OpenAPI_plmn_id_convertToJSON(pro_se_allowed_plmn->visited_plmn);
    if (visited_plmn_local_JSON == NULL) {
        ogs_error("OpenAPI_pro_se_allowed_plmn_convertToJSON() failed [visited_plmn]");
        goto end;
    }
    cJSON_AddItemToObject(item, "visitedPlmn", visited_plmn_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pro_se_allowed_plmn_convertToJSON() failed [visited_plmn]");
        goto end;
    }

    if (pro_se_allowed_plmn->prose_direct_allowed != OpenAPI_prose_direct_allowed_NULL) {
    cJSON *prose_direct_allowedList = cJSON_AddArrayToObject(item, "proseDirectAllowed");
    if (prose_direct_allowedList == NULL) {
        ogs_error("OpenAPI_pro_se_allowed_plmn_convertToJSON() failed [prose_direct_allowed]");
        goto end;
    }
    OpenAPI_list_for_each(pro_se_allowed_plmn->prose_direct_allowed, node) {
        if (cJSON_AddStringToObject(prose_direct_allowedList, "", OpenAPI_prose_direct_allowed_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_pro_se_allowed_plmn_convertToJSON() failed [prose_direct_allowed]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_pro_se_allowed_plmn_t *OpenAPI_pro_se_allowed_plmn_parseFromJSON(cJSON *pro_se_allowed_plmnJSON)
{
    OpenAPI_pro_se_allowed_plmn_t *pro_se_allowed_plmn_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *visited_plmn = NULL;
    OpenAPI_plmn_id_t *visited_plmn_local_nonprim = NULL;
    cJSON *prose_direct_allowed = NULL;
    OpenAPI_list_t *prose_direct_allowedList = NULL;
    visited_plmn = cJSON_GetObjectItemCaseSensitive(pro_se_allowed_plmnJSON, "visitedPlmn");
    if (!visited_plmn) {
        ogs_error("OpenAPI_pro_se_allowed_plmn_parseFromJSON() failed [visited_plmn]");
        goto end;
    }
    visited_plmn_local_nonprim = OpenAPI_plmn_id_parseFromJSON(visited_plmn);
    if (!visited_plmn_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [visited_plmn]");
        goto end;
    }

    prose_direct_allowed = cJSON_GetObjectItemCaseSensitive(pro_se_allowed_plmnJSON, "proseDirectAllowed");
    if (prose_direct_allowed) {
        cJSON *prose_direct_allowed_local = NULL;
        if (!cJSON_IsArray(prose_direct_allowed)) {
            ogs_error("OpenAPI_pro_se_allowed_plmn_parseFromJSON() failed [prose_direct_allowed]");
            goto end;
        }

        prose_direct_allowedList = OpenAPI_list_create();

        cJSON_ArrayForEach(prose_direct_allowed_local, prose_direct_allowed) {
            OpenAPI_prose_direct_allowed_e localEnum = OpenAPI_prose_direct_allowed_NULL;
            if (!cJSON_IsString(prose_direct_allowed_local)) {
                ogs_error("OpenAPI_pro_se_allowed_plmn_parseFromJSON() failed [prose_direct_allowed]");
                goto end;
            }
            localEnum = OpenAPI_prose_direct_allowed_FromString(prose_direct_allowed_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"prose_direct_allowed\" is not supported. Ignoring it ...",
                         prose_direct_allowed_local->valuestring);
            } else {
                OpenAPI_list_add(prose_direct_allowedList, (void *)localEnum);
            }
        }
        if (prose_direct_allowedList->count == 0) {
            ogs_error("OpenAPI_pro_se_allowed_plmn_parseFromJSON() failed: Expected prose_direct_allowedList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    pro_se_allowed_plmn_local_var = OpenAPI_pro_se_allowed_plmn_create (
        visited_plmn_local_nonprim,
        prose_direct_allowed ? prose_direct_allowedList : NULL
    );

    return pro_se_allowed_plmn_local_var;
end:
    if (visited_plmn_local_nonprim) {
        OpenAPI_plmn_id_free(visited_plmn_local_nonprim);
        visited_plmn_local_nonprim = NULL;
    }
    if (prose_direct_allowedList) {
        OpenAPI_list_free(prose_direct_allowedList);
        prose_direct_allowedList = NULL;
    }
    return NULL;
}

OpenAPI_pro_se_allowed_plmn_t *OpenAPI_pro_se_allowed_plmn_copy(OpenAPI_pro_se_allowed_plmn_t *dst, OpenAPI_pro_se_allowed_plmn_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pro_se_allowed_plmn_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pro_se_allowed_plmn_convertToJSON() failed");
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

    OpenAPI_pro_se_allowed_plmn_free(dst);
    dst = OpenAPI_pro_se_allowed_plmn_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

