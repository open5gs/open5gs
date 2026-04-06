
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ranging_sl_plmn_operator_class_1.h"

OpenAPI_ranging_sl_plmn_operator_class_1_t *OpenAPI_ranging_sl_plmn_operator_class_1_create(
    OpenAPI_lcs_client_class_e ranging_sl_lcs_client_class,
    OpenAPI_list_t *lcs_client_ids
)
{
    OpenAPI_ranging_sl_plmn_operator_class_1_t *ranging_sl_plmn_operator_class_1_local_var = ogs_malloc(sizeof(OpenAPI_ranging_sl_plmn_operator_class_1_t));
    ogs_assert(ranging_sl_plmn_operator_class_1_local_var);

    ranging_sl_plmn_operator_class_1_local_var->ranging_sl_lcs_client_class = ranging_sl_lcs_client_class;
    ranging_sl_plmn_operator_class_1_local_var->lcs_client_ids = lcs_client_ids;

    return ranging_sl_plmn_operator_class_1_local_var;
}

void OpenAPI_ranging_sl_plmn_operator_class_1_free(OpenAPI_ranging_sl_plmn_operator_class_1_t *ranging_sl_plmn_operator_class_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ranging_sl_plmn_operator_class_1) {
        return;
    }
    if (ranging_sl_plmn_operator_class_1->lcs_client_ids) {
        OpenAPI_list_for_each(ranging_sl_plmn_operator_class_1->lcs_client_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ranging_sl_plmn_operator_class_1->lcs_client_ids);
        ranging_sl_plmn_operator_class_1->lcs_client_ids = NULL;
    }
    ogs_free(ranging_sl_plmn_operator_class_1);
}

cJSON *OpenAPI_ranging_sl_plmn_operator_class_1_convertToJSON(OpenAPI_ranging_sl_plmn_operator_class_1_t *ranging_sl_plmn_operator_class_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ranging_sl_plmn_operator_class_1 == NULL) {
        ogs_error("OpenAPI_ranging_sl_plmn_operator_class_1_convertToJSON() failed [RangingSlPlmnOperatorClass_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ranging_sl_plmn_operator_class_1->ranging_sl_lcs_client_class == OpenAPI_lcs_client_class_NULL) {
        ogs_error("OpenAPI_ranging_sl_plmn_operator_class_1_convertToJSON() failed [ranging_sl_lcs_client_class]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "rangingSlLcsClientClass", OpenAPI_lcs_client_class_ToString(ranging_sl_plmn_operator_class_1->ranging_sl_lcs_client_class)) == NULL) {
        ogs_error("OpenAPI_ranging_sl_plmn_operator_class_1_convertToJSON() failed [ranging_sl_lcs_client_class]");
        goto end;
    }

    if (!ranging_sl_plmn_operator_class_1->lcs_client_ids) {
        ogs_error("OpenAPI_ranging_sl_plmn_operator_class_1_convertToJSON() failed [lcs_client_ids]");
        return NULL;
    }
    cJSON *lcs_client_idsList = cJSON_AddArrayToObject(item, "lcsClientIds");
    if (lcs_client_idsList == NULL) {
        ogs_error("OpenAPI_ranging_sl_plmn_operator_class_1_convertToJSON() failed [lcs_client_ids]");
        goto end;
    }
    OpenAPI_list_for_each(ranging_sl_plmn_operator_class_1->lcs_client_ids, node) {
        if (cJSON_AddStringToObject(lcs_client_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ranging_sl_plmn_operator_class_1_convertToJSON() failed [lcs_client_ids]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_ranging_sl_plmn_operator_class_1_t *OpenAPI_ranging_sl_plmn_operator_class_1_parseFromJSON(cJSON *ranging_sl_plmn_operator_class_1JSON)
{
    OpenAPI_ranging_sl_plmn_operator_class_1_t *ranging_sl_plmn_operator_class_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ranging_sl_lcs_client_class = NULL;
    OpenAPI_lcs_client_class_e ranging_sl_lcs_client_classVariable = 0;
    cJSON *lcs_client_ids = NULL;
    OpenAPI_list_t *lcs_client_idsList = NULL;
    ranging_sl_lcs_client_class = cJSON_GetObjectItemCaseSensitive(ranging_sl_plmn_operator_class_1JSON, "rangingSlLcsClientClass");
    if (!ranging_sl_lcs_client_class) {
        ogs_error("OpenAPI_ranging_sl_plmn_operator_class_1_parseFromJSON() failed [ranging_sl_lcs_client_class]");
        goto end;
    }
    if (!cJSON_IsString(ranging_sl_lcs_client_class)) {
        ogs_error("OpenAPI_ranging_sl_plmn_operator_class_1_parseFromJSON() failed [ranging_sl_lcs_client_class]");
        goto end;
    }
    ranging_sl_lcs_client_classVariable = OpenAPI_lcs_client_class_FromString(ranging_sl_lcs_client_class->valuestring);

    lcs_client_ids = cJSON_GetObjectItemCaseSensitive(ranging_sl_plmn_operator_class_1JSON, "lcsClientIds");
    if (!lcs_client_ids) {
        ogs_error("OpenAPI_ranging_sl_plmn_operator_class_1_parseFromJSON() failed [lcs_client_ids]");
        goto end;
    }
        cJSON *lcs_client_ids_local = NULL;
        if (!cJSON_IsArray(lcs_client_ids)) {
            ogs_error("OpenAPI_ranging_sl_plmn_operator_class_1_parseFromJSON() failed [lcs_client_ids]");
            goto end;
        }

        lcs_client_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(lcs_client_ids_local, lcs_client_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(lcs_client_ids_local)) {
                ogs_error("OpenAPI_ranging_sl_plmn_operator_class_1_parseFromJSON() failed [lcs_client_ids]");
                goto end;
            }
            OpenAPI_list_add(lcs_client_idsList, ogs_strdup(lcs_client_ids_local->valuestring));
        }

    ranging_sl_plmn_operator_class_1_local_var = OpenAPI_ranging_sl_plmn_operator_class_1_create (
        ranging_sl_lcs_client_classVariable,
        lcs_client_idsList
    );

    return ranging_sl_plmn_operator_class_1_local_var;
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

OpenAPI_ranging_sl_plmn_operator_class_1_t *OpenAPI_ranging_sl_plmn_operator_class_1_copy(OpenAPI_ranging_sl_plmn_operator_class_1_t *dst, OpenAPI_ranging_sl_plmn_operator_class_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ranging_sl_plmn_operator_class_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ranging_sl_plmn_operator_class_1_convertToJSON() failed");
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

    OpenAPI_ranging_sl_plmn_operator_class_1_free(dst);
    dst = OpenAPI_ranging_sl_plmn_operator_class_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

