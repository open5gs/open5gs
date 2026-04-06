
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ranging_sl_external_unrelated_class.h"

OpenAPI_ranging_sl_external_unrelated_class_t *OpenAPI_ranging_sl_external_unrelated_class_create(
    OpenAPI_list_t *ranging_sl_lcs_client_externals,
    OpenAPI_list_t *ranging_sl_af_externals
)
{
    OpenAPI_ranging_sl_external_unrelated_class_t *ranging_sl_external_unrelated_class_local_var = ogs_malloc(sizeof(OpenAPI_ranging_sl_external_unrelated_class_t));
    ogs_assert(ranging_sl_external_unrelated_class_local_var);

    ranging_sl_external_unrelated_class_local_var->ranging_sl_lcs_client_externals = ranging_sl_lcs_client_externals;
    ranging_sl_external_unrelated_class_local_var->ranging_sl_af_externals = ranging_sl_af_externals;

    return ranging_sl_external_unrelated_class_local_var;
}

void OpenAPI_ranging_sl_external_unrelated_class_free(OpenAPI_ranging_sl_external_unrelated_class_t *ranging_sl_external_unrelated_class)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ranging_sl_external_unrelated_class) {
        return;
    }
    if (ranging_sl_external_unrelated_class->ranging_sl_lcs_client_externals) {
        OpenAPI_list_for_each(ranging_sl_external_unrelated_class->ranging_sl_lcs_client_externals, node) {
            OpenAPI_ranging_sl_lcs_client_external_free(node->data);
        }
        OpenAPI_list_free(ranging_sl_external_unrelated_class->ranging_sl_lcs_client_externals);
        ranging_sl_external_unrelated_class->ranging_sl_lcs_client_externals = NULL;
    }
    if (ranging_sl_external_unrelated_class->ranging_sl_af_externals) {
        OpenAPI_list_for_each(ranging_sl_external_unrelated_class->ranging_sl_af_externals, node) {
            OpenAPI_ranging_sl_af_external_free(node->data);
        }
        OpenAPI_list_free(ranging_sl_external_unrelated_class->ranging_sl_af_externals);
        ranging_sl_external_unrelated_class->ranging_sl_af_externals = NULL;
    }
    ogs_free(ranging_sl_external_unrelated_class);
}

cJSON *OpenAPI_ranging_sl_external_unrelated_class_convertToJSON(OpenAPI_ranging_sl_external_unrelated_class_t *ranging_sl_external_unrelated_class)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ranging_sl_external_unrelated_class == NULL) {
        ogs_error("OpenAPI_ranging_sl_external_unrelated_class_convertToJSON() failed [RangingSlExternalUnrelatedClass]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ranging_sl_external_unrelated_class->ranging_sl_lcs_client_externals) {
    cJSON *ranging_sl_lcs_client_externalsList = cJSON_AddArrayToObject(item, "rangingSlLcsClientExternals");
    if (ranging_sl_lcs_client_externalsList == NULL) {
        ogs_error("OpenAPI_ranging_sl_external_unrelated_class_convertToJSON() failed [ranging_sl_lcs_client_externals]");
        goto end;
    }
    OpenAPI_list_for_each(ranging_sl_external_unrelated_class->ranging_sl_lcs_client_externals, node) {
        cJSON *itemLocal = OpenAPI_ranging_sl_lcs_client_external_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ranging_sl_external_unrelated_class_convertToJSON() failed [ranging_sl_lcs_client_externals]");
            goto end;
        }
        cJSON_AddItemToArray(ranging_sl_lcs_client_externalsList, itemLocal);
    }
    }

    if (ranging_sl_external_unrelated_class->ranging_sl_af_externals) {
    cJSON *ranging_sl_af_externalsList = cJSON_AddArrayToObject(item, "rangingSlAfExternals");
    if (ranging_sl_af_externalsList == NULL) {
        ogs_error("OpenAPI_ranging_sl_external_unrelated_class_convertToJSON() failed [ranging_sl_af_externals]");
        goto end;
    }
    OpenAPI_list_for_each(ranging_sl_external_unrelated_class->ranging_sl_af_externals, node) {
        cJSON *itemLocal = OpenAPI_ranging_sl_af_external_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ranging_sl_external_unrelated_class_convertToJSON() failed [ranging_sl_af_externals]");
            goto end;
        }
        cJSON_AddItemToArray(ranging_sl_af_externalsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_ranging_sl_external_unrelated_class_t *OpenAPI_ranging_sl_external_unrelated_class_parseFromJSON(cJSON *ranging_sl_external_unrelated_classJSON)
{
    OpenAPI_ranging_sl_external_unrelated_class_t *ranging_sl_external_unrelated_class_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ranging_sl_lcs_client_externals = NULL;
    OpenAPI_list_t *ranging_sl_lcs_client_externalsList = NULL;
    cJSON *ranging_sl_af_externals = NULL;
    OpenAPI_list_t *ranging_sl_af_externalsList = NULL;
    ranging_sl_lcs_client_externals = cJSON_GetObjectItemCaseSensitive(ranging_sl_external_unrelated_classJSON, "rangingSlLcsClientExternals");
    if (ranging_sl_lcs_client_externals) {
        cJSON *ranging_sl_lcs_client_externals_local = NULL;
        if (!cJSON_IsArray(ranging_sl_lcs_client_externals)) {
            ogs_error("OpenAPI_ranging_sl_external_unrelated_class_parseFromJSON() failed [ranging_sl_lcs_client_externals]");
            goto end;
        }

        ranging_sl_lcs_client_externalsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ranging_sl_lcs_client_externals_local, ranging_sl_lcs_client_externals) {
            if (!cJSON_IsObject(ranging_sl_lcs_client_externals_local)) {
                ogs_error("OpenAPI_ranging_sl_external_unrelated_class_parseFromJSON() failed [ranging_sl_lcs_client_externals]");
                goto end;
            }
            OpenAPI_ranging_sl_lcs_client_external_t *ranging_sl_lcs_client_externalsItem = OpenAPI_ranging_sl_lcs_client_external_parseFromJSON(ranging_sl_lcs_client_externals_local);
            if (!ranging_sl_lcs_client_externalsItem) {
                ogs_error("No ranging_sl_lcs_client_externalsItem");
                goto end;
            }
            OpenAPI_list_add(ranging_sl_lcs_client_externalsList, ranging_sl_lcs_client_externalsItem);
        }
    }

    ranging_sl_af_externals = cJSON_GetObjectItemCaseSensitive(ranging_sl_external_unrelated_classJSON, "rangingSlAfExternals");
    if (ranging_sl_af_externals) {
        cJSON *ranging_sl_af_externals_local = NULL;
        if (!cJSON_IsArray(ranging_sl_af_externals)) {
            ogs_error("OpenAPI_ranging_sl_external_unrelated_class_parseFromJSON() failed [ranging_sl_af_externals]");
            goto end;
        }

        ranging_sl_af_externalsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ranging_sl_af_externals_local, ranging_sl_af_externals) {
            if (!cJSON_IsObject(ranging_sl_af_externals_local)) {
                ogs_error("OpenAPI_ranging_sl_external_unrelated_class_parseFromJSON() failed [ranging_sl_af_externals]");
                goto end;
            }
            OpenAPI_ranging_sl_af_external_t *ranging_sl_af_externalsItem = OpenAPI_ranging_sl_af_external_parseFromJSON(ranging_sl_af_externals_local);
            if (!ranging_sl_af_externalsItem) {
                ogs_error("No ranging_sl_af_externalsItem");
                goto end;
            }
            OpenAPI_list_add(ranging_sl_af_externalsList, ranging_sl_af_externalsItem);
        }
    }

    ranging_sl_external_unrelated_class_local_var = OpenAPI_ranging_sl_external_unrelated_class_create (
        ranging_sl_lcs_client_externals ? ranging_sl_lcs_client_externalsList : NULL,
        ranging_sl_af_externals ? ranging_sl_af_externalsList : NULL
    );

    return ranging_sl_external_unrelated_class_local_var;
end:
    if (ranging_sl_lcs_client_externalsList) {
        OpenAPI_list_for_each(ranging_sl_lcs_client_externalsList, node) {
            OpenAPI_ranging_sl_lcs_client_external_free(node->data);
        }
        OpenAPI_list_free(ranging_sl_lcs_client_externalsList);
        ranging_sl_lcs_client_externalsList = NULL;
    }
    if (ranging_sl_af_externalsList) {
        OpenAPI_list_for_each(ranging_sl_af_externalsList, node) {
            OpenAPI_ranging_sl_af_external_free(node->data);
        }
        OpenAPI_list_free(ranging_sl_af_externalsList);
        ranging_sl_af_externalsList = NULL;
    }
    return NULL;
}

OpenAPI_ranging_sl_external_unrelated_class_t *OpenAPI_ranging_sl_external_unrelated_class_copy(OpenAPI_ranging_sl_external_unrelated_class_t *dst, OpenAPI_ranging_sl_external_unrelated_class_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ranging_sl_external_unrelated_class_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ranging_sl_external_unrelated_class_convertToJSON() failed");
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

    OpenAPI_ranging_sl_external_unrelated_class_free(dst);
    dst = OpenAPI_ranging_sl_external_unrelated_class_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

