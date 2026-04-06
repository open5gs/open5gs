
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lcs_privacy_data.h"

OpenAPI_lcs_privacy_data_t *OpenAPI_lcs_privacy_data_create(
    OpenAPI_lpi_t *lpi,
    OpenAPI_unrelated_class_t *unrelated_class,
    OpenAPI_list_t *plmn_operator_classes,
    OpenAPI_geographic_area_t *evt_rpt_expected_area,
    OpenAPI_area_usage_ind_e area_usage_ind,
    OpenAPI_up_loc_rep_ind_af_e up_loc_rep_ind_af
)
{
    OpenAPI_lcs_privacy_data_t *lcs_privacy_data_local_var = ogs_malloc(sizeof(OpenAPI_lcs_privacy_data_t));
    ogs_assert(lcs_privacy_data_local_var);

    lcs_privacy_data_local_var->lpi = lpi;
    lcs_privacy_data_local_var->unrelated_class = unrelated_class;
    lcs_privacy_data_local_var->plmn_operator_classes = plmn_operator_classes;
    lcs_privacy_data_local_var->evt_rpt_expected_area = evt_rpt_expected_area;
    lcs_privacy_data_local_var->area_usage_ind = area_usage_ind;
    lcs_privacy_data_local_var->up_loc_rep_ind_af = up_loc_rep_ind_af;

    return lcs_privacy_data_local_var;
}

void OpenAPI_lcs_privacy_data_free(OpenAPI_lcs_privacy_data_t *lcs_privacy_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == lcs_privacy_data) {
        return;
    }
    if (lcs_privacy_data->lpi) {
        OpenAPI_lpi_free(lcs_privacy_data->lpi);
        lcs_privacy_data->lpi = NULL;
    }
    if (lcs_privacy_data->unrelated_class) {
        OpenAPI_unrelated_class_free(lcs_privacy_data->unrelated_class);
        lcs_privacy_data->unrelated_class = NULL;
    }
    if (lcs_privacy_data->plmn_operator_classes) {
        OpenAPI_list_for_each(lcs_privacy_data->plmn_operator_classes, node) {
            OpenAPI_plmn_operator_class_free(node->data);
        }
        OpenAPI_list_free(lcs_privacy_data->plmn_operator_classes);
        lcs_privacy_data->plmn_operator_classes = NULL;
    }
    if (lcs_privacy_data->evt_rpt_expected_area) {
        OpenAPI_geographic_area_free(lcs_privacy_data->evt_rpt_expected_area);
        lcs_privacy_data->evt_rpt_expected_area = NULL;
    }
    ogs_free(lcs_privacy_data);
}

cJSON *OpenAPI_lcs_privacy_data_convertToJSON(OpenAPI_lcs_privacy_data_t *lcs_privacy_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (lcs_privacy_data == NULL) {
        ogs_error("OpenAPI_lcs_privacy_data_convertToJSON() failed [LcsPrivacyData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (lcs_privacy_data->lpi) {
    cJSON *lpi_local_JSON = OpenAPI_lpi_convertToJSON(lcs_privacy_data->lpi);
    if (lpi_local_JSON == NULL) {
        ogs_error("OpenAPI_lcs_privacy_data_convertToJSON() failed [lpi]");
        goto end;
    }
    cJSON_AddItemToObject(item, "lpi", lpi_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_lcs_privacy_data_convertToJSON() failed [lpi]");
        goto end;
    }
    }

    if (lcs_privacy_data->unrelated_class) {
    cJSON *unrelated_class_local_JSON = OpenAPI_unrelated_class_convertToJSON(lcs_privacy_data->unrelated_class);
    if (unrelated_class_local_JSON == NULL) {
        ogs_error("OpenAPI_lcs_privacy_data_convertToJSON() failed [unrelated_class]");
        goto end;
    }
    cJSON_AddItemToObject(item, "unrelatedClass", unrelated_class_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_lcs_privacy_data_convertToJSON() failed [unrelated_class]");
        goto end;
    }
    }

    if (lcs_privacy_data->plmn_operator_classes) {
    cJSON *plmn_operator_classesList = cJSON_AddArrayToObject(item, "plmnOperatorClasses");
    if (plmn_operator_classesList == NULL) {
        ogs_error("OpenAPI_lcs_privacy_data_convertToJSON() failed [plmn_operator_classes]");
        goto end;
    }
    OpenAPI_list_for_each(lcs_privacy_data->plmn_operator_classes, node) {
        cJSON *itemLocal = OpenAPI_plmn_operator_class_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_lcs_privacy_data_convertToJSON() failed [plmn_operator_classes]");
            goto end;
        }
        cJSON_AddItemToArray(plmn_operator_classesList, itemLocal);
    }
    }

    if (lcs_privacy_data->evt_rpt_expected_area) {
    cJSON *evt_rpt_expected_area_local_JSON = OpenAPI_geographic_area_convertToJSON(lcs_privacy_data->evt_rpt_expected_area);
    if (evt_rpt_expected_area_local_JSON == NULL) {
        ogs_error("OpenAPI_lcs_privacy_data_convertToJSON() failed [evt_rpt_expected_area]");
        goto end;
    }
    cJSON_AddItemToObject(item, "evtRptExpectedArea", evt_rpt_expected_area_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_lcs_privacy_data_convertToJSON() failed [evt_rpt_expected_area]");
        goto end;
    }
    }

    if (lcs_privacy_data->area_usage_ind != OpenAPI_area_usage_ind_NULL) {
    if (cJSON_AddStringToObject(item, "areaUsageInd", OpenAPI_area_usage_ind_ToString(lcs_privacy_data->area_usage_ind)) == NULL) {
        ogs_error("OpenAPI_lcs_privacy_data_convertToJSON() failed [area_usage_ind]");
        goto end;
    }
    }

    if (lcs_privacy_data->up_loc_rep_ind_af != OpenAPI_up_loc_rep_ind_af_NULL) {
    if (cJSON_AddStringToObject(item, "upLocRepIndAf", OpenAPI_up_loc_rep_ind_af_ToString(lcs_privacy_data->up_loc_rep_ind_af)) == NULL) {
        ogs_error("OpenAPI_lcs_privacy_data_convertToJSON() failed [up_loc_rep_ind_af]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_lcs_privacy_data_t *OpenAPI_lcs_privacy_data_parseFromJSON(cJSON *lcs_privacy_dataJSON)
{
    OpenAPI_lcs_privacy_data_t *lcs_privacy_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *lpi = NULL;
    OpenAPI_lpi_t *lpi_local_nonprim = NULL;
    cJSON *unrelated_class = NULL;
    OpenAPI_unrelated_class_t *unrelated_class_local_nonprim = NULL;
    cJSON *plmn_operator_classes = NULL;
    OpenAPI_list_t *plmn_operator_classesList = NULL;
    cJSON *evt_rpt_expected_area = NULL;
    OpenAPI_geographic_area_t *evt_rpt_expected_area_local_nonprim = NULL;
    cJSON *area_usage_ind = NULL;
    OpenAPI_area_usage_ind_e area_usage_indVariable = 0;
    cJSON *up_loc_rep_ind_af = NULL;
    OpenAPI_up_loc_rep_ind_af_e up_loc_rep_ind_afVariable = 0;
    lpi = cJSON_GetObjectItemCaseSensitive(lcs_privacy_dataJSON, "lpi");
    if (lpi) {
    lpi_local_nonprim = OpenAPI_lpi_parseFromJSON(lpi);
    if (!lpi_local_nonprim) {
        ogs_error("OpenAPI_lpi_parseFromJSON failed [lpi]");
        goto end;
    }
    }

    unrelated_class = cJSON_GetObjectItemCaseSensitive(lcs_privacy_dataJSON, "unrelatedClass");
    if (unrelated_class) {
    unrelated_class_local_nonprim = OpenAPI_unrelated_class_parseFromJSON(unrelated_class);
    if (!unrelated_class_local_nonprim) {
        ogs_error("OpenAPI_unrelated_class_parseFromJSON failed [unrelated_class]");
        goto end;
    }
    }

    plmn_operator_classes = cJSON_GetObjectItemCaseSensitive(lcs_privacy_dataJSON, "plmnOperatorClasses");
    if (plmn_operator_classes) {
        cJSON *plmn_operator_classes_local = NULL;
        if (!cJSON_IsArray(plmn_operator_classes)) {
            ogs_error("OpenAPI_lcs_privacy_data_parseFromJSON() failed [plmn_operator_classes]");
            goto end;
        }

        plmn_operator_classesList = OpenAPI_list_create();

        cJSON_ArrayForEach(plmn_operator_classes_local, plmn_operator_classes) {
            if (!cJSON_IsObject(plmn_operator_classes_local)) {
                ogs_error("OpenAPI_lcs_privacy_data_parseFromJSON() failed [plmn_operator_classes]");
                goto end;
            }
            OpenAPI_plmn_operator_class_t *plmn_operator_classesItem = OpenAPI_plmn_operator_class_parseFromJSON(plmn_operator_classes_local);
            if (!plmn_operator_classesItem) {
                ogs_error("No plmn_operator_classesItem");
                goto end;
            }
            OpenAPI_list_add(plmn_operator_classesList, plmn_operator_classesItem);
        }
    }

    evt_rpt_expected_area = cJSON_GetObjectItemCaseSensitive(lcs_privacy_dataJSON, "evtRptExpectedArea");
    if (evt_rpt_expected_area) {
    evt_rpt_expected_area_local_nonprim = OpenAPI_geographic_area_parseFromJSON(evt_rpt_expected_area);
    if (!evt_rpt_expected_area_local_nonprim) {
        ogs_error("OpenAPI_geographic_area_parseFromJSON failed [evt_rpt_expected_area]");
        goto end;
    }
    }

    area_usage_ind = cJSON_GetObjectItemCaseSensitive(lcs_privacy_dataJSON, "areaUsageInd");
    if (area_usage_ind) {
    if (!cJSON_IsString(area_usage_ind)) {
        ogs_error("OpenAPI_lcs_privacy_data_parseFromJSON() failed [area_usage_ind]");
        goto end;
    }
    area_usage_indVariable = OpenAPI_area_usage_ind_FromString(area_usage_ind->valuestring);
    }

    up_loc_rep_ind_af = cJSON_GetObjectItemCaseSensitive(lcs_privacy_dataJSON, "upLocRepIndAf");
    if (up_loc_rep_ind_af) {
    if (!cJSON_IsString(up_loc_rep_ind_af)) {
        ogs_error("OpenAPI_lcs_privacy_data_parseFromJSON() failed [up_loc_rep_ind_af]");
        goto end;
    }
    up_loc_rep_ind_afVariable = OpenAPI_up_loc_rep_ind_af_FromString(up_loc_rep_ind_af->valuestring);
    }

    lcs_privacy_data_local_var = OpenAPI_lcs_privacy_data_create (
        lpi ? lpi_local_nonprim : NULL,
        unrelated_class ? unrelated_class_local_nonprim : NULL,
        plmn_operator_classes ? plmn_operator_classesList : NULL,
        evt_rpt_expected_area ? evt_rpt_expected_area_local_nonprim : NULL,
        area_usage_ind ? area_usage_indVariable : 0,
        up_loc_rep_ind_af ? up_loc_rep_ind_afVariable : 0
    );

    return lcs_privacy_data_local_var;
end:
    if (lpi_local_nonprim) {
        OpenAPI_lpi_free(lpi_local_nonprim);
        lpi_local_nonprim = NULL;
    }
    if (unrelated_class_local_nonprim) {
        OpenAPI_unrelated_class_free(unrelated_class_local_nonprim);
        unrelated_class_local_nonprim = NULL;
    }
    if (plmn_operator_classesList) {
        OpenAPI_list_for_each(plmn_operator_classesList, node) {
            OpenAPI_plmn_operator_class_free(node->data);
        }
        OpenAPI_list_free(plmn_operator_classesList);
        plmn_operator_classesList = NULL;
    }
    if (evt_rpt_expected_area_local_nonprim) {
        OpenAPI_geographic_area_free(evt_rpt_expected_area_local_nonprim);
        evt_rpt_expected_area_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_lcs_privacy_data_t *OpenAPI_lcs_privacy_data_copy(OpenAPI_lcs_privacy_data_t *dst, OpenAPI_lcs_privacy_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_lcs_privacy_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_lcs_privacy_data_convertToJSON() failed");
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

    OpenAPI_lcs_privacy_data_free(dst);
    dst = OpenAPI_lcs_privacy_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

