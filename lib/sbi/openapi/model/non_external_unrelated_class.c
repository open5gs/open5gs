
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "non_external_unrelated_class.h"

OpenAPI_non_external_unrelated_class_t *OpenAPI_non_external_unrelated_class_create(
    OpenAPI_list_t *lcs_client_non_externals,
    OpenAPI_list_t *af_non_externals
    )
{
    OpenAPI_non_external_unrelated_class_t *non_external_unrelated_class_local_var = OpenAPI_malloc(sizeof(OpenAPI_non_external_unrelated_class_t));
    if (!non_external_unrelated_class_local_var) {
        return NULL;
    }
    non_external_unrelated_class_local_var->lcs_client_non_externals = lcs_client_non_externals;
    non_external_unrelated_class_local_var->af_non_externals = af_non_externals;

    return non_external_unrelated_class_local_var;
}

void OpenAPI_non_external_unrelated_class_free(OpenAPI_non_external_unrelated_class_t *non_external_unrelated_class)
{
    if (NULL == non_external_unrelated_class) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(non_external_unrelated_class->lcs_client_non_externals, node) {
        OpenAPI_lcs_client_non_external_free(node->data);
    }
    OpenAPI_list_free(non_external_unrelated_class->lcs_client_non_externals);
    OpenAPI_list_for_each(non_external_unrelated_class->af_non_externals, node) {
        OpenAPI_af_non_external_free(node->data);
    }
    OpenAPI_list_free(non_external_unrelated_class->af_non_externals);
    ogs_free(non_external_unrelated_class);
}

cJSON *OpenAPI_non_external_unrelated_class_convertToJSON(OpenAPI_non_external_unrelated_class_t *non_external_unrelated_class)
{
    cJSON *item = NULL;

    if (non_external_unrelated_class == NULL) {
        ogs_error("OpenAPI_non_external_unrelated_class_convertToJSON() failed [NonExternalUnrelatedClass]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (non_external_unrelated_class->lcs_client_non_externals) {
        cJSON *lcs_client_non_externalsList = cJSON_AddArrayToObject(item, "lcsClientNonExternals");
        if (lcs_client_non_externalsList == NULL) {
            ogs_error("OpenAPI_non_external_unrelated_class_convertToJSON() failed [lcs_client_non_externals]");
            goto end;
        }

        OpenAPI_lnode_t *lcs_client_non_externals_node;
        if (non_external_unrelated_class->lcs_client_non_externals) {
            OpenAPI_list_for_each(non_external_unrelated_class->lcs_client_non_externals, lcs_client_non_externals_node) {
                cJSON *itemLocal = OpenAPI_lcs_client_non_external_convertToJSON(lcs_client_non_externals_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_non_external_unrelated_class_convertToJSON() failed [lcs_client_non_externals]");
                    goto end;
                }
                cJSON_AddItemToArray(lcs_client_non_externalsList, itemLocal);
            }
        }
    }

    if (non_external_unrelated_class->af_non_externals) {
        cJSON *af_non_externalsList = cJSON_AddArrayToObject(item, "afNonExternals");
        if (af_non_externalsList == NULL) {
            ogs_error("OpenAPI_non_external_unrelated_class_convertToJSON() failed [af_non_externals]");
            goto end;
        }

        OpenAPI_lnode_t *af_non_externals_node;
        if (non_external_unrelated_class->af_non_externals) {
            OpenAPI_list_for_each(non_external_unrelated_class->af_non_externals, af_non_externals_node) {
                cJSON *itemLocal = OpenAPI_af_non_external_convertToJSON(af_non_externals_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_non_external_unrelated_class_convertToJSON() failed [af_non_externals]");
                    goto end;
                }
                cJSON_AddItemToArray(af_non_externalsList, itemLocal);
            }
        }
    }

end:
    return item;
}

OpenAPI_non_external_unrelated_class_t *OpenAPI_non_external_unrelated_class_parseFromJSON(cJSON *non_external_unrelated_classJSON)
{
    OpenAPI_non_external_unrelated_class_t *non_external_unrelated_class_local_var = NULL;
    cJSON *lcs_client_non_externals = cJSON_GetObjectItemCaseSensitive(non_external_unrelated_classJSON, "lcsClientNonExternals");

    OpenAPI_list_t *lcs_client_non_externalsList;
    if (lcs_client_non_externals) {
        cJSON *lcs_client_non_externals_local_nonprimitive;
        if (!cJSON_IsArray(lcs_client_non_externals)) {
            ogs_error("OpenAPI_non_external_unrelated_class_parseFromJSON() failed [lcs_client_non_externals]");
            goto end;
        }

        lcs_client_non_externalsList = OpenAPI_list_create();

        cJSON_ArrayForEach(lcs_client_non_externals_local_nonprimitive, lcs_client_non_externals ) {
            if (!cJSON_IsObject(lcs_client_non_externals_local_nonprimitive)) {
                ogs_error("OpenAPI_non_external_unrelated_class_parseFromJSON() failed [lcs_client_non_externals]");
                goto end;
            }
            OpenAPI_lcs_client_non_external_t *lcs_client_non_externalsItem = OpenAPI_lcs_client_non_external_parseFromJSON(lcs_client_non_externals_local_nonprimitive);

            OpenAPI_list_add(lcs_client_non_externalsList, lcs_client_non_externalsItem);
        }
    }

    cJSON *af_non_externals = cJSON_GetObjectItemCaseSensitive(non_external_unrelated_classJSON, "afNonExternals");

    OpenAPI_list_t *af_non_externalsList;
    if (af_non_externals) {
        cJSON *af_non_externals_local_nonprimitive;
        if (!cJSON_IsArray(af_non_externals)) {
            ogs_error("OpenAPI_non_external_unrelated_class_parseFromJSON() failed [af_non_externals]");
            goto end;
        }

        af_non_externalsList = OpenAPI_list_create();

        cJSON_ArrayForEach(af_non_externals_local_nonprimitive, af_non_externals ) {
            if (!cJSON_IsObject(af_non_externals_local_nonprimitive)) {
                ogs_error("OpenAPI_non_external_unrelated_class_parseFromJSON() failed [af_non_externals]");
                goto end;
            }
            OpenAPI_af_non_external_t *af_non_externalsItem = OpenAPI_af_non_external_parseFromJSON(af_non_externals_local_nonprimitive);

            OpenAPI_list_add(af_non_externalsList, af_non_externalsItem);
        }
    }

    non_external_unrelated_class_local_var = OpenAPI_non_external_unrelated_class_create (
        lcs_client_non_externals ? lcs_client_non_externalsList : NULL,
        af_non_externals ? af_non_externalsList : NULL
        );

    return non_external_unrelated_class_local_var;
end:
    return NULL;
}

OpenAPI_non_external_unrelated_class_t *OpenAPI_non_external_unrelated_class_copy(OpenAPI_non_external_unrelated_class_t *dst, OpenAPI_non_external_unrelated_class_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_non_external_unrelated_class_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_non_external_unrelated_class_convertToJSON() failed");
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

    OpenAPI_non_external_unrelated_class_free(dst);
    dst = OpenAPI_non_external_unrelated_class_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

