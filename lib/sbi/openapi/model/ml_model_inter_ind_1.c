
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ml_model_inter_ind_1.h"

OpenAPI_ml_model_inter_ind_1_t *OpenAPI_ml_model_inter_ind_1_create(
    OpenAPI_nwdaf_event_e analytics_id,
    OpenAPI_list_t *vendor_list
)
{
    OpenAPI_ml_model_inter_ind_1_t *ml_model_inter_ind_1_local_var = ogs_malloc(sizeof(OpenAPI_ml_model_inter_ind_1_t));
    ogs_assert(ml_model_inter_ind_1_local_var);

    ml_model_inter_ind_1_local_var->analytics_id = analytics_id;
    ml_model_inter_ind_1_local_var->vendor_list = vendor_list;

    return ml_model_inter_ind_1_local_var;
}

void OpenAPI_ml_model_inter_ind_1_free(OpenAPI_ml_model_inter_ind_1_t *ml_model_inter_ind_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ml_model_inter_ind_1) {
        return;
    }
    if (ml_model_inter_ind_1->vendor_list) {
        OpenAPI_list_for_each(ml_model_inter_ind_1->vendor_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ml_model_inter_ind_1->vendor_list);
        ml_model_inter_ind_1->vendor_list = NULL;
    }
    ogs_free(ml_model_inter_ind_1);
}

cJSON *OpenAPI_ml_model_inter_ind_1_convertToJSON(OpenAPI_ml_model_inter_ind_1_t *ml_model_inter_ind_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ml_model_inter_ind_1 == NULL) {
        ogs_error("OpenAPI_ml_model_inter_ind_1_convertToJSON() failed [MlModelInterInd_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ml_model_inter_ind_1->analytics_id == OpenAPI_nwdaf_event_NULL) {
        ogs_error("OpenAPI_ml_model_inter_ind_1_convertToJSON() failed [analytics_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "analyticsId", OpenAPI_nwdaf_event_ToString(ml_model_inter_ind_1->analytics_id)) == NULL) {
        ogs_error("OpenAPI_ml_model_inter_ind_1_convertToJSON() failed [analytics_id]");
        goto end;
    }

    if (!ml_model_inter_ind_1->vendor_list) {
        ogs_error("OpenAPI_ml_model_inter_ind_1_convertToJSON() failed [vendor_list]");
        return NULL;
    }
    cJSON *vendor_listList = cJSON_AddArrayToObject(item, "vendorList");
    if (vendor_listList == NULL) {
        ogs_error("OpenAPI_ml_model_inter_ind_1_convertToJSON() failed [vendor_list]");
        goto end;
    }
    OpenAPI_list_for_each(ml_model_inter_ind_1->vendor_list, node) {
        if (cJSON_AddStringToObject(vendor_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ml_model_inter_ind_1_convertToJSON() failed [vendor_list]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_ml_model_inter_ind_1_t *OpenAPI_ml_model_inter_ind_1_parseFromJSON(cJSON *ml_model_inter_ind_1JSON)
{
    OpenAPI_ml_model_inter_ind_1_t *ml_model_inter_ind_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *analytics_id = NULL;
    OpenAPI_nwdaf_event_e analytics_idVariable = 0;
    cJSON *vendor_list = NULL;
    OpenAPI_list_t *vendor_listList = NULL;
    analytics_id = cJSON_GetObjectItemCaseSensitive(ml_model_inter_ind_1JSON, "analyticsId");
    if (!analytics_id) {
        ogs_error("OpenAPI_ml_model_inter_ind_1_parseFromJSON() failed [analytics_id]");
        goto end;
    }
    if (!cJSON_IsString(analytics_id)) {
        ogs_error("OpenAPI_ml_model_inter_ind_1_parseFromJSON() failed [analytics_id]");
        goto end;
    }
    analytics_idVariable = OpenAPI_nwdaf_event_FromString(analytics_id->valuestring);

    vendor_list = cJSON_GetObjectItemCaseSensitive(ml_model_inter_ind_1JSON, "vendorList");
    if (!vendor_list) {
        ogs_error("OpenAPI_ml_model_inter_ind_1_parseFromJSON() failed [vendor_list]");
        goto end;
    }
        cJSON *vendor_list_local = NULL;
        if (!cJSON_IsArray(vendor_list)) {
            ogs_error("OpenAPI_ml_model_inter_ind_1_parseFromJSON() failed [vendor_list]");
            goto end;
        }

        vendor_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(vendor_list_local, vendor_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(vendor_list_local)) {
                ogs_error("OpenAPI_ml_model_inter_ind_1_parseFromJSON() failed [vendor_list]");
                goto end;
            }
            OpenAPI_list_add(vendor_listList, ogs_strdup(vendor_list_local->valuestring));
        }

    ml_model_inter_ind_1_local_var = OpenAPI_ml_model_inter_ind_1_create (
        analytics_idVariable,
        vendor_listList
    );

    return ml_model_inter_ind_1_local_var;
end:
    if (vendor_listList) {
        OpenAPI_list_for_each(vendor_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(vendor_listList);
        vendor_listList = NULL;
    }
    return NULL;
}

OpenAPI_ml_model_inter_ind_1_t *OpenAPI_ml_model_inter_ind_1_copy(OpenAPI_ml_model_inter_ind_1_t *dst, OpenAPI_ml_model_inter_ind_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ml_model_inter_ind_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ml_model_inter_ind_1_convertToJSON() failed");
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

    OpenAPI_ml_model_inter_ind_1_free(dst);
    dst = OpenAPI_ml_model_inter_ind_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

