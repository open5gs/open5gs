
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "plmn_operator_class.h"

OpenAPI_plmn_operator_class_t *OpenAPI_plmn_operator_class_create(
    OpenAPI_lcs_client_class_e lcs_client_class,
    OpenAPI_list_t *lcs_client_ids
)
{
    OpenAPI_plmn_operator_class_t *plmn_operator_class_local_var = ogs_malloc(sizeof(OpenAPI_plmn_operator_class_t));
    ogs_assert(plmn_operator_class_local_var);

    plmn_operator_class_local_var->lcs_client_class = lcs_client_class;
    plmn_operator_class_local_var->lcs_client_ids = lcs_client_ids;

    return plmn_operator_class_local_var;
}

void OpenAPI_plmn_operator_class_free(OpenAPI_plmn_operator_class_t *plmn_operator_class)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == plmn_operator_class) {
        return;
    }
    if (plmn_operator_class->lcs_client_ids) {
        OpenAPI_list_for_each(plmn_operator_class->lcs_client_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(plmn_operator_class->lcs_client_ids);
        plmn_operator_class->lcs_client_ids = NULL;
    }
    ogs_free(plmn_operator_class);
}

cJSON *OpenAPI_plmn_operator_class_convertToJSON(OpenAPI_plmn_operator_class_t *plmn_operator_class)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (plmn_operator_class == NULL) {
        ogs_error("OpenAPI_plmn_operator_class_convertToJSON() failed [PlmnOperatorClass]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (plmn_operator_class->lcs_client_class == OpenAPI_lcs_client_class_NULL) {
        ogs_error("OpenAPI_plmn_operator_class_convertToJSON() failed [lcs_client_class]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "lcsClientClass", OpenAPI_lcs_client_class_ToString(plmn_operator_class->lcs_client_class)) == NULL) {
        ogs_error("OpenAPI_plmn_operator_class_convertToJSON() failed [lcs_client_class]");
        goto end;
    }

    if (!plmn_operator_class->lcs_client_ids) {
        ogs_error("OpenAPI_plmn_operator_class_convertToJSON() failed [lcs_client_ids]");
        return NULL;
    }
    cJSON *lcs_client_idsList = cJSON_AddArrayToObject(item, "lcsClientIds");
    if (lcs_client_idsList == NULL) {
        ogs_error("OpenAPI_plmn_operator_class_convertToJSON() failed [lcs_client_ids]");
        goto end;
    }
    OpenAPI_list_for_each(plmn_operator_class->lcs_client_ids, node) {
        if (cJSON_AddStringToObject(lcs_client_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_plmn_operator_class_convertToJSON() failed [lcs_client_ids]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_plmn_operator_class_t *OpenAPI_plmn_operator_class_parseFromJSON(cJSON *plmn_operator_classJSON)
{
    OpenAPI_plmn_operator_class_t *plmn_operator_class_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *lcs_client_class = NULL;
    OpenAPI_lcs_client_class_e lcs_client_classVariable = 0;
    cJSON *lcs_client_ids = NULL;
    OpenAPI_list_t *lcs_client_idsList = NULL;
    lcs_client_class = cJSON_GetObjectItemCaseSensitive(plmn_operator_classJSON, "lcsClientClass");
    if (!lcs_client_class) {
        ogs_error("OpenAPI_plmn_operator_class_parseFromJSON() failed [lcs_client_class]");
        goto end;
    }
    if (!cJSON_IsString(lcs_client_class)) {
        ogs_error("OpenAPI_plmn_operator_class_parseFromJSON() failed [lcs_client_class]");
        goto end;
    }
    lcs_client_classVariable = OpenAPI_lcs_client_class_FromString(lcs_client_class->valuestring);

    lcs_client_ids = cJSON_GetObjectItemCaseSensitive(plmn_operator_classJSON, "lcsClientIds");
    if (!lcs_client_ids) {
        ogs_error("OpenAPI_plmn_operator_class_parseFromJSON() failed [lcs_client_ids]");
        goto end;
    }
        cJSON *lcs_client_ids_local = NULL;
        if (!cJSON_IsArray(lcs_client_ids)) {
            ogs_error("OpenAPI_plmn_operator_class_parseFromJSON() failed [lcs_client_ids]");
            goto end;
        }

        lcs_client_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(lcs_client_ids_local, lcs_client_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(lcs_client_ids_local)) {
                ogs_error("OpenAPI_plmn_operator_class_parseFromJSON() failed [lcs_client_ids]");
                goto end;
            }
            OpenAPI_list_add(lcs_client_idsList, ogs_strdup(lcs_client_ids_local->valuestring));
        }

    plmn_operator_class_local_var = OpenAPI_plmn_operator_class_create (
        lcs_client_classVariable,
        lcs_client_idsList
    );

    return plmn_operator_class_local_var;
end:
    if (lcs_client_idsList) {
        OpenAPI_list_for_each(lcs_client_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(lcs_client_idsList);
        lcs_client_idsList = NULL;
    }
    return NULL;
}

OpenAPI_plmn_operator_class_t *OpenAPI_plmn_operator_class_copy(OpenAPI_plmn_operator_class_t *dst, OpenAPI_plmn_operator_class_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_plmn_operator_class_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_plmn_operator_class_convertToJSON() failed");
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

    OpenAPI_plmn_operator_class_free(dst);
    dst = OpenAPI_plmn_operator_class_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

