
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "enhanced_coverage_restriction_data.h"

OpenAPI_enhanced_coverage_restriction_data_t *OpenAPI_enhanced_coverage_restriction_data_create(
    OpenAPI_list_t *plmn_ec_info_list
)
{
    OpenAPI_enhanced_coverage_restriction_data_t *enhanced_coverage_restriction_data_local_var = ogs_malloc(sizeof(OpenAPI_enhanced_coverage_restriction_data_t));
    ogs_assert(enhanced_coverage_restriction_data_local_var);

    enhanced_coverage_restriction_data_local_var->plmn_ec_info_list = plmn_ec_info_list;

    return enhanced_coverage_restriction_data_local_var;
}

void OpenAPI_enhanced_coverage_restriction_data_free(OpenAPI_enhanced_coverage_restriction_data_t *enhanced_coverage_restriction_data)
{
    if (NULL == enhanced_coverage_restriction_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(enhanced_coverage_restriction_data->plmn_ec_info_list, node) {
        OpenAPI_plmn_ec_info_free(node->data);
    }
    OpenAPI_list_free(enhanced_coverage_restriction_data->plmn_ec_info_list);
    ogs_free(enhanced_coverage_restriction_data);
}

cJSON *OpenAPI_enhanced_coverage_restriction_data_convertToJSON(OpenAPI_enhanced_coverage_restriction_data_t *enhanced_coverage_restriction_data)
{
    cJSON *item = NULL;

    if (enhanced_coverage_restriction_data == NULL) {
        ogs_error("OpenAPI_enhanced_coverage_restriction_data_convertToJSON() failed [EnhancedCoverageRestrictionData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (enhanced_coverage_restriction_data->plmn_ec_info_list) {
    cJSON *plmn_ec_info_listList = cJSON_AddArrayToObject(item, "plmnEcInfoList");
    if (plmn_ec_info_listList == NULL) {
        ogs_error("OpenAPI_enhanced_coverage_restriction_data_convertToJSON() failed [plmn_ec_info_list]");
        goto end;
    }

    OpenAPI_lnode_t *plmn_ec_info_list_node;
    if (enhanced_coverage_restriction_data->plmn_ec_info_list) {
        OpenAPI_list_for_each(enhanced_coverage_restriction_data->plmn_ec_info_list, plmn_ec_info_list_node) {
            cJSON *itemLocal = OpenAPI_plmn_ec_info_convertToJSON(plmn_ec_info_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_enhanced_coverage_restriction_data_convertToJSON() failed [plmn_ec_info_list]");
                goto end;
            }
            cJSON_AddItemToArray(plmn_ec_info_listList, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_enhanced_coverage_restriction_data_t *OpenAPI_enhanced_coverage_restriction_data_parseFromJSON(cJSON *enhanced_coverage_restriction_dataJSON)
{
    OpenAPI_enhanced_coverage_restriction_data_t *enhanced_coverage_restriction_data_local_var = NULL;
    cJSON *plmn_ec_info_list = cJSON_GetObjectItemCaseSensitive(enhanced_coverage_restriction_dataJSON, "plmnEcInfoList");

    OpenAPI_list_t *plmn_ec_info_listList;
    if (plmn_ec_info_list) {
    cJSON *plmn_ec_info_list_local_nonprimitive;
    if (!cJSON_IsArray(plmn_ec_info_list)){
        ogs_error("OpenAPI_enhanced_coverage_restriction_data_parseFromJSON() failed [plmn_ec_info_list]");
        goto end;
    }

    plmn_ec_info_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(plmn_ec_info_list_local_nonprimitive, plmn_ec_info_list ) {
        if (!cJSON_IsObject(plmn_ec_info_list_local_nonprimitive)) {
            ogs_error("OpenAPI_enhanced_coverage_restriction_data_parseFromJSON() failed [plmn_ec_info_list]");
            goto end;
        }
        OpenAPI_plmn_ec_info_t *plmn_ec_info_listItem = OpenAPI_plmn_ec_info_parseFromJSON(plmn_ec_info_list_local_nonprimitive);

        if (!plmn_ec_info_listItem) {
            ogs_error("No plmn_ec_info_listItem");
            OpenAPI_list_free(plmn_ec_info_listList);
            goto end;
        }

        OpenAPI_list_add(plmn_ec_info_listList, plmn_ec_info_listItem);
    }
    }

    enhanced_coverage_restriction_data_local_var = OpenAPI_enhanced_coverage_restriction_data_create (
        plmn_ec_info_list ? plmn_ec_info_listList : NULL
    );

    return enhanced_coverage_restriction_data_local_var;
end:
    return NULL;
}

OpenAPI_enhanced_coverage_restriction_data_t *OpenAPI_enhanced_coverage_restriction_data_copy(OpenAPI_enhanced_coverage_restriction_data_t *dst, OpenAPI_enhanced_coverage_restriction_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_enhanced_coverage_restriction_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_enhanced_coverage_restriction_data_convertToJSON() failed");
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

    OpenAPI_enhanced_coverage_restriction_data_free(dst);
    dst = OpenAPI_enhanced_coverage_restriction_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

