
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ranging_sl_privacy_data_1.h"

OpenAPI_ranging_sl_privacy_data_1_t *OpenAPI_ranging_sl_privacy_data_1_create(
    OpenAPI_rslppi_1_t *rslppi,
    OpenAPI_ranging_sl_unrelated_class_1_t *ranging_sl_unrelated_class,
    OpenAPI_list_t *ranging_sl_plmn_operator_classes,
    OpenAPI_geographic_area_t *ranging_sl_evt_rpt_expected_area
)
{
    OpenAPI_ranging_sl_privacy_data_1_t *ranging_sl_privacy_data_1_local_var = ogs_malloc(sizeof(OpenAPI_ranging_sl_privacy_data_1_t));
    ogs_assert(ranging_sl_privacy_data_1_local_var);

    ranging_sl_privacy_data_1_local_var->rslppi = rslppi;
    ranging_sl_privacy_data_1_local_var->ranging_sl_unrelated_class = ranging_sl_unrelated_class;
    ranging_sl_privacy_data_1_local_var->ranging_sl_plmn_operator_classes = ranging_sl_plmn_operator_classes;
    ranging_sl_privacy_data_1_local_var->ranging_sl_evt_rpt_expected_area = ranging_sl_evt_rpt_expected_area;

    return ranging_sl_privacy_data_1_local_var;
}

void OpenAPI_ranging_sl_privacy_data_1_free(OpenAPI_ranging_sl_privacy_data_1_t *ranging_sl_privacy_data_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ranging_sl_privacy_data_1) {
        return;
    }
    if (ranging_sl_privacy_data_1->rslppi) {
        OpenAPI_rslppi_1_free(ranging_sl_privacy_data_1->rslppi);
        ranging_sl_privacy_data_1->rslppi = NULL;
    }
    if (ranging_sl_privacy_data_1->ranging_sl_unrelated_class) {
        OpenAPI_ranging_sl_unrelated_class_1_free(ranging_sl_privacy_data_1->ranging_sl_unrelated_class);
        ranging_sl_privacy_data_1->ranging_sl_unrelated_class = NULL;
    }
    if (ranging_sl_privacy_data_1->ranging_sl_plmn_operator_classes) {
        OpenAPI_list_for_each(ranging_sl_privacy_data_1->ranging_sl_plmn_operator_classes, node) {
            OpenAPI_ranging_sl_plmn_operator_class_1_free(node->data);
        }
        OpenAPI_list_free(ranging_sl_privacy_data_1->ranging_sl_plmn_operator_classes);
        ranging_sl_privacy_data_1->ranging_sl_plmn_operator_classes = NULL;
    }
    if (ranging_sl_privacy_data_1->ranging_sl_evt_rpt_expected_area) {
        OpenAPI_geographic_area_free(ranging_sl_privacy_data_1->ranging_sl_evt_rpt_expected_area);
        ranging_sl_privacy_data_1->ranging_sl_evt_rpt_expected_area = NULL;
    }
    ogs_free(ranging_sl_privacy_data_1);
}

cJSON *OpenAPI_ranging_sl_privacy_data_1_convertToJSON(OpenAPI_ranging_sl_privacy_data_1_t *ranging_sl_privacy_data_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ranging_sl_privacy_data_1 == NULL) {
        ogs_error("OpenAPI_ranging_sl_privacy_data_1_convertToJSON() failed [RangingSlPrivacyData_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ranging_sl_privacy_data_1->rslppi) {
    cJSON *rslppi_local_JSON = OpenAPI_rslppi_1_convertToJSON(ranging_sl_privacy_data_1->rslppi);
    if (rslppi_local_JSON == NULL) {
        ogs_error("OpenAPI_ranging_sl_privacy_data_1_convertToJSON() failed [rslppi]");
        goto end;
    }
    cJSON_AddItemToObject(item, "rslppi", rslppi_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ranging_sl_privacy_data_1_convertToJSON() failed [rslppi]");
        goto end;
    }
    }

    if (ranging_sl_privacy_data_1->ranging_sl_unrelated_class) {
    cJSON *ranging_sl_unrelated_class_local_JSON = OpenAPI_ranging_sl_unrelated_class_1_convertToJSON(ranging_sl_privacy_data_1->ranging_sl_unrelated_class);
    if (ranging_sl_unrelated_class_local_JSON == NULL) {
        ogs_error("OpenAPI_ranging_sl_privacy_data_1_convertToJSON() failed [ranging_sl_unrelated_class]");
        goto end;
    }
    cJSON_AddItemToObject(item, "rangingSlUnrelatedClass", ranging_sl_unrelated_class_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ranging_sl_privacy_data_1_convertToJSON() failed [ranging_sl_unrelated_class]");
        goto end;
    }
    }

    if (ranging_sl_privacy_data_1->ranging_sl_plmn_operator_classes) {
    cJSON *ranging_sl_plmn_operator_classesList = cJSON_AddArrayToObject(item, "rangingSlPlmnOperatorClasses");
    if (ranging_sl_plmn_operator_classesList == NULL) {
        ogs_error("OpenAPI_ranging_sl_privacy_data_1_convertToJSON() failed [ranging_sl_plmn_operator_classes]");
        goto end;
    }
    OpenAPI_list_for_each(ranging_sl_privacy_data_1->ranging_sl_plmn_operator_classes, node) {
        cJSON *itemLocal = OpenAPI_ranging_sl_plmn_operator_class_1_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ranging_sl_privacy_data_1_convertToJSON() failed [ranging_sl_plmn_operator_classes]");
            goto end;
        }
        cJSON_AddItemToArray(ranging_sl_plmn_operator_classesList, itemLocal);
    }
    }

    if (ranging_sl_privacy_data_1->ranging_sl_evt_rpt_expected_area) {
    cJSON *ranging_sl_evt_rpt_expected_area_local_JSON = OpenAPI_geographic_area_convertToJSON(ranging_sl_privacy_data_1->ranging_sl_evt_rpt_expected_area);
    if (ranging_sl_evt_rpt_expected_area_local_JSON == NULL) {
        ogs_error("OpenAPI_ranging_sl_privacy_data_1_convertToJSON() failed [ranging_sl_evt_rpt_expected_area]");
        goto end;
    }
    cJSON_AddItemToObject(item, "rangingSlEvtRptExpectedArea", ranging_sl_evt_rpt_expected_area_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ranging_sl_privacy_data_1_convertToJSON() failed [ranging_sl_evt_rpt_expected_area]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ranging_sl_privacy_data_1_t *OpenAPI_ranging_sl_privacy_data_1_parseFromJSON(cJSON *ranging_sl_privacy_data_1JSON)
{
    OpenAPI_ranging_sl_privacy_data_1_t *ranging_sl_privacy_data_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *rslppi = NULL;
    OpenAPI_rslppi_1_t *rslppi_local_nonprim = NULL;
    cJSON *ranging_sl_unrelated_class = NULL;
    OpenAPI_ranging_sl_unrelated_class_1_t *ranging_sl_unrelated_class_local_nonprim = NULL;
    cJSON *ranging_sl_plmn_operator_classes = NULL;
    OpenAPI_list_t *ranging_sl_plmn_operator_classesList = NULL;
    cJSON *ranging_sl_evt_rpt_expected_area = NULL;
    OpenAPI_geographic_area_t *ranging_sl_evt_rpt_expected_area_local_nonprim = NULL;
    rslppi = cJSON_GetObjectItemCaseSensitive(ranging_sl_privacy_data_1JSON, "rslppi");
    if (rslppi) {
    rslppi_local_nonprim = OpenAPI_rslppi_1_parseFromJSON(rslppi);
    if (!rslppi_local_nonprim) {
        ogs_error("OpenAPI_rslppi_1_parseFromJSON failed [rslppi]");
        goto end;
    }
    }

    ranging_sl_unrelated_class = cJSON_GetObjectItemCaseSensitive(ranging_sl_privacy_data_1JSON, "rangingSlUnrelatedClass");
    if (ranging_sl_unrelated_class) {
    ranging_sl_unrelated_class_local_nonprim = OpenAPI_ranging_sl_unrelated_class_1_parseFromJSON(ranging_sl_unrelated_class);
    if (!ranging_sl_unrelated_class_local_nonprim) {
        ogs_error("OpenAPI_ranging_sl_unrelated_class_1_parseFromJSON failed [ranging_sl_unrelated_class]");
        goto end;
    }
    }

    ranging_sl_plmn_operator_classes = cJSON_GetObjectItemCaseSensitive(ranging_sl_privacy_data_1JSON, "rangingSlPlmnOperatorClasses");
    if (ranging_sl_plmn_operator_classes) {
        cJSON *ranging_sl_plmn_operator_classes_local = NULL;
        if (!cJSON_IsArray(ranging_sl_plmn_operator_classes)) {
            ogs_error("OpenAPI_ranging_sl_privacy_data_1_parseFromJSON() failed [ranging_sl_plmn_operator_classes]");
            goto end;
        }

        ranging_sl_plmn_operator_classesList = OpenAPI_list_create();

        cJSON_ArrayForEach(ranging_sl_plmn_operator_classes_local, ranging_sl_plmn_operator_classes) {
            if (!cJSON_IsObject(ranging_sl_plmn_operator_classes_local)) {
                ogs_error("OpenAPI_ranging_sl_privacy_data_1_parseFromJSON() failed [ranging_sl_plmn_operator_classes]");
                goto end;
            }
            OpenAPI_ranging_sl_plmn_operator_class_1_t *ranging_sl_plmn_operator_classesItem = OpenAPI_ranging_sl_plmn_operator_class_1_parseFromJSON(ranging_sl_plmn_operator_classes_local);
            if (!ranging_sl_plmn_operator_classesItem) {
                ogs_error("No ranging_sl_plmn_operator_classesItem");
                goto end;
            }
            OpenAPI_list_add(ranging_sl_plmn_operator_classesList, ranging_sl_plmn_operator_classesItem);
        }
    }

    ranging_sl_evt_rpt_expected_area = cJSON_GetObjectItemCaseSensitive(ranging_sl_privacy_data_1JSON, "rangingSlEvtRptExpectedArea");
    if (ranging_sl_evt_rpt_expected_area) {
    ranging_sl_evt_rpt_expected_area_local_nonprim = OpenAPI_geographic_area_parseFromJSON(ranging_sl_evt_rpt_expected_area);
    if (!ranging_sl_evt_rpt_expected_area_local_nonprim) {
        ogs_error("OpenAPI_geographic_area_parseFromJSON failed [ranging_sl_evt_rpt_expected_area]");
        goto end;
    }
    }

    ranging_sl_privacy_data_1_local_var = OpenAPI_ranging_sl_privacy_data_1_create (
        rslppi ? rslppi_local_nonprim : NULL,
        ranging_sl_unrelated_class ? ranging_sl_unrelated_class_local_nonprim : NULL,
        ranging_sl_plmn_operator_classes ? ranging_sl_plmn_operator_classesList : NULL,
        ranging_sl_evt_rpt_expected_area ? ranging_sl_evt_rpt_expected_area_local_nonprim : NULL
    );

    return ranging_sl_privacy_data_1_local_var;
end:
    if (rslppi_local_nonprim) {
        OpenAPI_rslppi_1_free(rslppi_local_nonprim);
        rslppi_local_nonprim = NULL;
    }
    if (ranging_sl_unrelated_class_local_nonprim) {
        OpenAPI_ranging_sl_unrelated_class_1_free(ranging_sl_unrelated_class_local_nonprim);
        ranging_sl_unrelated_class_local_nonprim = NULL;
    }
    if (ranging_sl_plmn_operator_classesList) {
        OpenAPI_list_for_each(ranging_sl_plmn_operator_classesList, node) {
            OpenAPI_ranging_sl_plmn_operator_class_1_free(node->data);
        }
        OpenAPI_list_free(ranging_sl_plmn_operator_classesList);
        ranging_sl_plmn_operator_classesList = NULL;
    }
    if (ranging_sl_evt_rpt_expected_area_local_nonprim) {
        OpenAPI_geographic_area_free(ranging_sl_evt_rpt_expected_area_local_nonprim);
        ranging_sl_evt_rpt_expected_area_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_ranging_sl_privacy_data_1_t *OpenAPI_ranging_sl_privacy_data_1_copy(OpenAPI_ranging_sl_privacy_data_1_t *dst, OpenAPI_ranging_sl_privacy_data_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ranging_sl_privacy_data_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ranging_sl_privacy_data_1_convertToJSON() failed");
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

    OpenAPI_ranging_sl_privacy_data_1_free(dst);
    dst = OpenAPI_ranging_sl_privacy_data_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

