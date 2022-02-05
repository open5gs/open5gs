
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lcs_privacy_data_1.h"

OpenAPI_lcs_privacy_data_1_t *OpenAPI_lcs_privacy_data_1_create(
    OpenAPI_lpi_1_t *lpi,
    OpenAPI_unrelated_class_1_t *unrelated_class,
    OpenAPI_list_t *plmn_operator_classes
)
{
    OpenAPI_lcs_privacy_data_1_t *lcs_privacy_data_1_local_var = ogs_malloc(sizeof(OpenAPI_lcs_privacy_data_1_t));
    ogs_assert(lcs_privacy_data_1_local_var);

    lcs_privacy_data_1_local_var->lpi = lpi;
    lcs_privacy_data_1_local_var->unrelated_class = unrelated_class;
    lcs_privacy_data_1_local_var->plmn_operator_classes = plmn_operator_classes;

    return lcs_privacy_data_1_local_var;
}

void OpenAPI_lcs_privacy_data_1_free(OpenAPI_lcs_privacy_data_1_t *lcs_privacy_data_1)
{
    if (NULL == lcs_privacy_data_1) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_lpi_1_free(lcs_privacy_data_1->lpi);
    OpenAPI_unrelated_class_1_free(lcs_privacy_data_1->unrelated_class);
    OpenAPI_list_for_each(lcs_privacy_data_1->plmn_operator_classes, node) {
        OpenAPI_plmn_operator_class_1_free(node->data);
    }
    OpenAPI_list_free(lcs_privacy_data_1->plmn_operator_classes);
    ogs_free(lcs_privacy_data_1);
}

cJSON *OpenAPI_lcs_privacy_data_1_convertToJSON(OpenAPI_lcs_privacy_data_1_t *lcs_privacy_data_1)
{
    cJSON *item = NULL;

    if (lcs_privacy_data_1 == NULL) {
        ogs_error("OpenAPI_lcs_privacy_data_1_convertToJSON() failed [LcsPrivacyData_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (lcs_privacy_data_1->lpi) {
    cJSON *lpi_local_JSON = OpenAPI_lpi_1_convertToJSON(lcs_privacy_data_1->lpi);
    if (lpi_local_JSON == NULL) {
        ogs_error("OpenAPI_lcs_privacy_data_1_convertToJSON() failed [lpi]");
        goto end;
    }
    cJSON_AddItemToObject(item, "lpi", lpi_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_lcs_privacy_data_1_convertToJSON() failed [lpi]");
        goto end;
    }
    }

    if (lcs_privacy_data_1->unrelated_class) {
    cJSON *unrelated_class_local_JSON = OpenAPI_unrelated_class_1_convertToJSON(lcs_privacy_data_1->unrelated_class);
    if (unrelated_class_local_JSON == NULL) {
        ogs_error("OpenAPI_lcs_privacy_data_1_convertToJSON() failed [unrelated_class]");
        goto end;
    }
    cJSON_AddItemToObject(item, "unrelatedClass", unrelated_class_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_lcs_privacy_data_1_convertToJSON() failed [unrelated_class]");
        goto end;
    }
    }

    if (lcs_privacy_data_1->plmn_operator_classes) {
    cJSON *plmn_operator_classesList = cJSON_AddArrayToObject(item, "plmnOperatorClasses");
    if (plmn_operator_classesList == NULL) {
        ogs_error("OpenAPI_lcs_privacy_data_1_convertToJSON() failed [plmn_operator_classes]");
        goto end;
    }

    OpenAPI_lnode_t *plmn_operator_classes_node;
    if (lcs_privacy_data_1->plmn_operator_classes) {
        OpenAPI_list_for_each(lcs_privacy_data_1->plmn_operator_classes, plmn_operator_classes_node) {
            cJSON *itemLocal = OpenAPI_plmn_operator_class_1_convertToJSON(plmn_operator_classes_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_lcs_privacy_data_1_convertToJSON() failed [plmn_operator_classes]");
                goto end;
            }
            cJSON_AddItemToArray(plmn_operator_classesList, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_lcs_privacy_data_1_t *OpenAPI_lcs_privacy_data_1_parseFromJSON(cJSON *lcs_privacy_data_1JSON)
{
    OpenAPI_lcs_privacy_data_1_t *lcs_privacy_data_1_local_var = NULL;
    cJSON *lpi = cJSON_GetObjectItemCaseSensitive(lcs_privacy_data_1JSON, "lpi");

    OpenAPI_lpi_1_t *lpi_local_nonprim = NULL;
    if (lpi) {
    lpi_local_nonprim = OpenAPI_lpi_1_parseFromJSON(lpi);
    }

    cJSON *unrelated_class = cJSON_GetObjectItemCaseSensitive(lcs_privacy_data_1JSON, "unrelatedClass");

    OpenAPI_unrelated_class_1_t *unrelated_class_local_nonprim = NULL;
    if (unrelated_class) {
    unrelated_class_local_nonprim = OpenAPI_unrelated_class_1_parseFromJSON(unrelated_class);
    }

    cJSON *plmn_operator_classes = cJSON_GetObjectItemCaseSensitive(lcs_privacy_data_1JSON, "plmnOperatorClasses");

    OpenAPI_list_t *plmn_operator_classesList;
    if (plmn_operator_classes) {
    cJSON *plmn_operator_classes_local_nonprimitive;
    if (!cJSON_IsArray(plmn_operator_classes)){
        ogs_error("OpenAPI_lcs_privacy_data_1_parseFromJSON() failed [plmn_operator_classes]");
        goto end;
    }

    plmn_operator_classesList = OpenAPI_list_create();

    cJSON_ArrayForEach(plmn_operator_classes_local_nonprimitive, plmn_operator_classes ) {
        if (!cJSON_IsObject(plmn_operator_classes_local_nonprimitive)) {
            ogs_error("OpenAPI_lcs_privacy_data_1_parseFromJSON() failed [plmn_operator_classes]");
            goto end;
        }
        OpenAPI_plmn_operator_class_1_t *plmn_operator_classesItem = OpenAPI_plmn_operator_class_1_parseFromJSON(plmn_operator_classes_local_nonprimitive);

        if (!plmn_operator_classesItem) {
            ogs_error("No plmn_operator_classesItem");
            OpenAPI_list_free(plmn_operator_classesList);
            goto end;
        }

        OpenAPI_list_add(plmn_operator_classesList, plmn_operator_classesItem);
    }
    }

    lcs_privacy_data_1_local_var = OpenAPI_lcs_privacy_data_1_create (
        lpi ? lpi_local_nonprim : NULL,
        unrelated_class ? unrelated_class_local_nonprim : NULL,
        plmn_operator_classes ? plmn_operator_classesList : NULL
    );

    return lcs_privacy_data_1_local_var;
end:
    return NULL;
}

OpenAPI_lcs_privacy_data_1_t *OpenAPI_lcs_privacy_data_1_copy(OpenAPI_lcs_privacy_data_1_t *dst, OpenAPI_lcs_privacy_data_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_lcs_privacy_data_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_lcs_privacy_data_1_convertToJSON() failed");
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

    OpenAPI_lcs_privacy_data_1_free(dst);
    dst = OpenAPI_lcs_privacy_data_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

