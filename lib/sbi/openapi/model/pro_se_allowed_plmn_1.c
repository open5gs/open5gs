
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pro_se_allowed_plmn_1.h"

OpenAPI_pro_se_allowed_plmn_1_t *OpenAPI_pro_se_allowed_plmn_1_create(
    OpenAPI_plmn_id_t *visited_plmn,
    OpenAPI_list_t *prose_direct_allowed
)
{
    OpenAPI_pro_se_allowed_plmn_1_t *pro_se_allowed_plmn_1_local_var = ogs_malloc(sizeof(OpenAPI_pro_se_allowed_plmn_1_t));
    ogs_assert(pro_se_allowed_plmn_1_local_var);

    pro_se_allowed_plmn_1_local_var->visited_plmn = visited_plmn;
    pro_se_allowed_plmn_1_local_var->prose_direct_allowed = prose_direct_allowed;

    return pro_se_allowed_plmn_1_local_var;
}

void OpenAPI_pro_se_allowed_plmn_1_free(OpenAPI_pro_se_allowed_plmn_1_t *pro_se_allowed_plmn_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pro_se_allowed_plmn_1) {
        return;
    }
    if (pro_se_allowed_plmn_1->visited_plmn) {
        OpenAPI_plmn_id_free(pro_se_allowed_plmn_1->visited_plmn);
        pro_se_allowed_plmn_1->visited_plmn = NULL;
    }
    if (pro_se_allowed_plmn_1->prose_direct_allowed) {
        OpenAPI_list_free(pro_se_allowed_plmn_1->prose_direct_allowed);
        pro_se_allowed_plmn_1->prose_direct_allowed = NULL;
    }
    ogs_free(pro_se_allowed_plmn_1);
}

cJSON *OpenAPI_pro_se_allowed_plmn_1_convertToJSON(OpenAPI_pro_se_allowed_plmn_1_t *pro_se_allowed_plmn_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pro_se_allowed_plmn_1 == NULL) {
        ogs_error("OpenAPI_pro_se_allowed_plmn_1_convertToJSON() failed [ProSeAllowedPlmn_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!pro_se_allowed_plmn_1->visited_plmn) {
        ogs_error("OpenAPI_pro_se_allowed_plmn_1_convertToJSON() failed [visited_plmn]");
        return NULL;
    }
    cJSON *visited_plmn_local_JSON = OpenAPI_plmn_id_convertToJSON(pro_se_allowed_plmn_1->visited_plmn);
    if (visited_plmn_local_JSON == NULL) {
        ogs_error("OpenAPI_pro_se_allowed_plmn_1_convertToJSON() failed [visited_plmn]");
        goto end;
    }
    cJSON_AddItemToObject(item, "visitedPlmn", visited_plmn_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pro_se_allowed_plmn_1_convertToJSON() failed [visited_plmn]");
        goto end;
    }

    if (pro_se_allowed_plmn_1->prose_direct_allowed != OpenAPI_prose_direct_allowed_NULL) {
    cJSON *prose_direct_allowedList = cJSON_AddArrayToObject(item, "proseDirectAllowed");
    if (prose_direct_allowedList == NULL) {
        ogs_error("OpenAPI_pro_se_allowed_plmn_1_convertToJSON() failed [prose_direct_allowed]");
        goto end;
    }
    OpenAPI_list_for_each(pro_se_allowed_plmn_1->prose_direct_allowed, node) {
        if (cJSON_AddStringToObject(prose_direct_allowedList, "", OpenAPI_prose_direct_allowed_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_pro_se_allowed_plmn_1_convertToJSON() failed [prose_direct_allowed]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_pro_se_allowed_plmn_1_t *OpenAPI_pro_se_allowed_plmn_1_parseFromJSON(cJSON *pro_se_allowed_plmn_1JSON)
{
    OpenAPI_pro_se_allowed_plmn_1_t *pro_se_allowed_plmn_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *visited_plmn = NULL;
    OpenAPI_plmn_id_t *visited_plmn_local_nonprim = NULL;
    cJSON *prose_direct_allowed = NULL;
    OpenAPI_list_t *prose_direct_allowedList = NULL;
    visited_plmn = cJSON_GetObjectItemCaseSensitive(pro_se_allowed_plmn_1JSON, "visitedPlmn");
    if (!visited_plmn) {
        ogs_error("OpenAPI_pro_se_allowed_plmn_1_parseFromJSON() failed [visited_plmn]");
        goto end;
    }
    visited_plmn_local_nonprim = OpenAPI_plmn_id_parseFromJSON(visited_plmn);
    if (!visited_plmn_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [visited_plmn]");
        goto end;
    }

    prose_direct_allowed = cJSON_GetObjectItemCaseSensitive(pro_se_allowed_plmn_1JSON, "proseDirectAllowed");
    if (prose_direct_allowed) {
        cJSON *prose_direct_allowed_local = NULL;
        if (!cJSON_IsArray(prose_direct_allowed)) {
            ogs_error("OpenAPI_pro_se_allowed_plmn_1_parseFromJSON() failed [prose_direct_allowed]");
            goto end;
        }

        prose_direct_allowedList = OpenAPI_list_create();

        cJSON_ArrayForEach(prose_direct_allowed_local, prose_direct_allowed) {
            OpenAPI_prose_direct_allowed_e localEnum = OpenAPI_prose_direct_allowed_NULL;
            if (!cJSON_IsString(prose_direct_allowed_local)) {
                ogs_error("OpenAPI_pro_se_allowed_plmn_1_parseFromJSON() failed [prose_direct_allowed]");
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
            ogs_error("OpenAPI_pro_se_allowed_plmn_1_parseFromJSON() failed: Expected prose_direct_allowedList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    pro_se_allowed_plmn_1_local_var = OpenAPI_pro_se_allowed_plmn_1_create (
        visited_plmn_local_nonprim,
        prose_direct_allowed ? prose_direct_allowedList : NULL
    );

    return pro_se_allowed_plmn_1_local_var;
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

OpenAPI_pro_se_allowed_plmn_1_t *OpenAPI_pro_se_allowed_plmn_1_copy(OpenAPI_pro_se_allowed_plmn_1_t *dst, OpenAPI_pro_se_allowed_plmn_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pro_se_allowed_plmn_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pro_se_allowed_plmn_1_convertToJSON() failed");
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

    OpenAPI_pro_se_allowed_plmn_1_free(dst);
    dst = OpenAPI_pro_se_allowed_plmn_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

