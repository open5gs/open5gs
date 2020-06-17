
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "plmn_operator_class.h"

OpenAPI_plmn_operator_class_t *OpenAPI_plmn_operator_class_create(
    OpenAPI_lcs_client_class_e lcs_client_class,
    OpenAPI_list_t *lcs_client_ids
    )
{
    OpenAPI_plmn_operator_class_t *plmn_operator_class_local_var = OpenAPI_malloc(sizeof(OpenAPI_plmn_operator_class_t));
    if (!plmn_operator_class_local_var) {
        return NULL;
    }
    plmn_operator_class_local_var->lcs_client_class = lcs_client_class;
    plmn_operator_class_local_var->lcs_client_ids = lcs_client_ids;

    return plmn_operator_class_local_var;
}

void OpenAPI_plmn_operator_class_free(OpenAPI_plmn_operator_class_t *plmn_operator_class)
{
    if (NULL == plmn_operator_class) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(plmn_operator_class->lcs_client_ids, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(plmn_operator_class->lcs_client_ids);
    ogs_free(plmn_operator_class);
}

cJSON *OpenAPI_plmn_operator_class_convertToJSON(OpenAPI_plmn_operator_class_t *plmn_operator_class)
{
    cJSON *item = NULL;

    if (plmn_operator_class == NULL) {
        ogs_error("OpenAPI_plmn_operator_class_convertToJSON() failed [PlmnOperatorClass]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!plmn_operator_class->lcs_client_class) {
        ogs_error("OpenAPI_plmn_operator_class_convertToJSON() failed [lcs_client_class]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "lcsClientClass", OpenAPI_lcs_client_class_ToString(plmn_operator_class->lcs_client_class)) == NULL) {
        ogs_error("OpenAPI_plmn_operator_class_convertToJSON() failed [lcs_client_class]");
        goto end;
    }

    if (!plmn_operator_class->lcs_client_ids) {
        ogs_error("OpenAPI_plmn_operator_class_convertToJSON() failed [lcs_client_ids]");
        goto end;
    }
    cJSON *lcs_client_ids = cJSON_AddArrayToObject(item, "lcsClientIds");
    if (lcs_client_ids == NULL) {
        ogs_error("OpenAPI_plmn_operator_class_convertToJSON() failed [lcs_client_ids]");
        goto end;
    }

    OpenAPI_lnode_t *lcs_client_ids_node;
    OpenAPI_list_for_each(plmn_operator_class->lcs_client_ids, lcs_client_ids_node)  {
        if (cJSON_AddStringToObject(lcs_client_ids, "", (char*)lcs_client_ids_node->data) == NULL) {
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
    cJSON *lcs_client_class = cJSON_GetObjectItemCaseSensitive(plmn_operator_classJSON, "lcsClientClass");
    if (!lcs_client_class) {
        ogs_error("OpenAPI_plmn_operator_class_parseFromJSON() failed [lcs_client_class]");
        goto end;
    }

    OpenAPI_lcs_client_class_e lcs_client_classVariable;

    if (!cJSON_IsString(lcs_client_class)) {
        ogs_error("OpenAPI_plmn_operator_class_parseFromJSON() failed [lcs_client_class]");
        goto end;
    }
    lcs_client_classVariable = OpenAPI_lcs_client_class_FromString(lcs_client_class->valuestring);

    cJSON *lcs_client_ids = cJSON_GetObjectItemCaseSensitive(plmn_operator_classJSON, "lcsClientIds");
    if (!lcs_client_ids) {
        ogs_error("OpenAPI_plmn_operator_class_parseFromJSON() failed [lcs_client_ids]");
        goto end;
    }

    OpenAPI_list_t *lcs_client_idsList;

    cJSON *lcs_client_ids_local;
    if (!cJSON_IsArray(lcs_client_ids)) {
        ogs_error("OpenAPI_plmn_operator_class_parseFromJSON() failed [lcs_client_ids]");
        goto end;
    }
    lcs_client_idsList = OpenAPI_list_create();

    cJSON_ArrayForEach(lcs_client_ids_local, lcs_client_ids) {
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

