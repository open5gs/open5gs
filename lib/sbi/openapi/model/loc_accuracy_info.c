
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "loc_accuracy_info.h"

OpenAPI_loc_accuracy_info_t *OpenAPI_loc_accuracy_info_create(
    OpenAPI_list_t *loc_acc_per_meths,
    bool is_in_out_ue_pct,
    int in_out_ue_pct,
    bool is_in_out_ind,
    int in_out_ind,
    bool is_confidence,
    int confidence
)
{
    OpenAPI_loc_accuracy_info_t *loc_accuracy_info_local_var = ogs_malloc(sizeof(OpenAPI_loc_accuracy_info_t));
    ogs_assert(loc_accuracy_info_local_var);

    loc_accuracy_info_local_var->loc_acc_per_meths = loc_acc_per_meths;
    loc_accuracy_info_local_var->is_in_out_ue_pct = is_in_out_ue_pct;
    loc_accuracy_info_local_var->in_out_ue_pct = in_out_ue_pct;
    loc_accuracy_info_local_var->is_in_out_ind = is_in_out_ind;
    loc_accuracy_info_local_var->in_out_ind = in_out_ind;
    loc_accuracy_info_local_var->is_confidence = is_confidence;
    loc_accuracy_info_local_var->confidence = confidence;

    return loc_accuracy_info_local_var;
}

void OpenAPI_loc_accuracy_info_free(OpenAPI_loc_accuracy_info_t *loc_accuracy_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == loc_accuracy_info) {
        return;
    }
    if (loc_accuracy_info->loc_acc_per_meths) {
        OpenAPI_list_for_each(loc_accuracy_info->loc_acc_per_meths, node) {
            OpenAPI_loc_accuracy_per_method_free(node->data);
        }
        OpenAPI_list_free(loc_accuracy_info->loc_acc_per_meths);
        loc_accuracy_info->loc_acc_per_meths = NULL;
    }
    ogs_free(loc_accuracy_info);
}

cJSON *OpenAPI_loc_accuracy_info_convertToJSON(OpenAPI_loc_accuracy_info_t *loc_accuracy_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (loc_accuracy_info == NULL) {
        ogs_error("OpenAPI_loc_accuracy_info_convertToJSON() failed [LocAccuracyInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!loc_accuracy_info->loc_acc_per_meths) {
        ogs_error("OpenAPI_loc_accuracy_info_convertToJSON() failed [loc_acc_per_meths]");
        return NULL;
    }
    cJSON *loc_acc_per_methsList = cJSON_AddArrayToObject(item, "locAccPerMeths");
    if (loc_acc_per_methsList == NULL) {
        ogs_error("OpenAPI_loc_accuracy_info_convertToJSON() failed [loc_acc_per_meths]");
        goto end;
    }
    OpenAPI_list_for_each(loc_accuracy_info->loc_acc_per_meths, node) {
        cJSON *itemLocal = OpenAPI_loc_accuracy_per_method_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_loc_accuracy_info_convertToJSON() failed [loc_acc_per_meths]");
            goto end;
        }
        cJSON_AddItemToArray(loc_acc_per_methsList, itemLocal);
    }

    if (loc_accuracy_info->is_in_out_ue_pct) {
    if (cJSON_AddNumberToObject(item, "inOutUePct", loc_accuracy_info->in_out_ue_pct) == NULL) {
        ogs_error("OpenAPI_loc_accuracy_info_convertToJSON() failed [in_out_ue_pct]");
        goto end;
    }
    }

    if (loc_accuracy_info->is_in_out_ind) {
    if (cJSON_AddBoolToObject(item, "inOutInd", loc_accuracy_info->in_out_ind) == NULL) {
        ogs_error("OpenAPI_loc_accuracy_info_convertToJSON() failed [in_out_ind]");
        goto end;
    }
    }

    if (loc_accuracy_info->is_confidence) {
    if (cJSON_AddNumberToObject(item, "confidence", loc_accuracy_info->confidence) == NULL) {
        ogs_error("OpenAPI_loc_accuracy_info_convertToJSON() failed [confidence]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_loc_accuracy_info_t *OpenAPI_loc_accuracy_info_parseFromJSON(cJSON *loc_accuracy_infoJSON)
{
    OpenAPI_loc_accuracy_info_t *loc_accuracy_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *loc_acc_per_meths = NULL;
    OpenAPI_list_t *loc_acc_per_methsList = NULL;
    cJSON *in_out_ue_pct = NULL;
    cJSON *in_out_ind = NULL;
    cJSON *confidence = NULL;
    loc_acc_per_meths = cJSON_GetObjectItemCaseSensitive(loc_accuracy_infoJSON, "locAccPerMeths");
    if (!loc_acc_per_meths) {
        ogs_error("OpenAPI_loc_accuracy_info_parseFromJSON() failed [loc_acc_per_meths]");
        goto end;
    }
        cJSON *loc_acc_per_meths_local = NULL;
        if (!cJSON_IsArray(loc_acc_per_meths)) {
            ogs_error("OpenAPI_loc_accuracy_info_parseFromJSON() failed [loc_acc_per_meths]");
            goto end;
        }

        loc_acc_per_methsList = OpenAPI_list_create();

        cJSON_ArrayForEach(loc_acc_per_meths_local, loc_acc_per_meths) {
            if (!cJSON_IsObject(loc_acc_per_meths_local)) {
                ogs_error("OpenAPI_loc_accuracy_info_parseFromJSON() failed [loc_acc_per_meths]");
                goto end;
            }
            OpenAPI_loc_accuracy_per_method_t *loc_acc_per_methsItem = OpenAPI_loc_accuracy_per_method_parseFromJSON(loc_acc_per_meths_local);
            if (!loc_acc_per_methsItem) {
                ogs_error("No loc_acc_per_methsItem");
                goto end;
            }
            OpenAPI_list_add(loc_acc_per_methsList, loc_acc_per_methsItem);
        }

    in_out_ue_pct = cJSON_GetObjectItemCaseSensitive(loc_accuracy_infoJSON, "inOutUePct");
    if (in_out_ue_pct) {
    if (!cJSON_IsNumber(in_out_ue_pct)) {
        ogs_error("OpenAPI_loc_accuracy_info_parseFromJSON() failed [in_out_ue_pct]");
        goto end;
    }
    }

    in_out_ind = cJSON_GetObjectItemCaseSensitive(loc_accuracy_infoJSON, "inOutInd");
    if (in_out_ind) {
    if (!cJSON_IsBool(in_out_ind)) {
        ogs_error("OpenAPI_loc_accuracy_info_parseFromJSON() failed [in_out_ind]");
        goto end;
    }
    }

    confidence = cJSON_GetObjectItemCaseSensitive(loc_accuracy_infoJSON, "confidence");
    if (confidence) {
    if (!cJSON_IsNumber(confidence)) {
        ogs_error("OpenAPI_loc_accuracy_info_parseFromJSON() failed [confidence]");
        goto end;
    }
    }

    loc_accuracy_info_local_var = OpenAPI_loc_accuracy_info_create (
        loc_acc_per_methsList,
        in_out_ue_pct ? true : false,
        in_out_ue_pct ? in_out_ue_pct->valuedouble : 0,
        in_out_ind ? true : false,
        in_out_ind ? in_out_ind->valueint : 0,
        confidence ? true : false,
        confidence ? confidence->valuedouble : 0
    );

    return loc_accuracy_info_local_var;
end:
    if (loc_acc_per_methsList) {
        OpenAPI_list_for_each(loc_acc_per_methsList, node) {
            OpenAPI_loc_accuracy_per_method_free(node->data);
        }
        OpenAPI_list_free(loc_acc_per_methsList);
        loc_acc_per_methsList = NULL;
    }
    return NULL;
}

OpenAPI_loc_accuracy_info_t *OpenAPI_loc_accuracy_info_copy(OpenAPI_loc_accuracy_info_t *dst, OpenAPI_loc_accuracy_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_loc_accuracy_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_loc_accuracy_info_convertToJSON() failed");
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

    OpenAPI_loc_accuracy_info_free(dst);
    dst = OpenAPI_loc_accuracy_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

