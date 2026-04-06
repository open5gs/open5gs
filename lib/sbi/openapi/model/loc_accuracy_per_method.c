
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "loc_accuracy_per_method.h"

OpenAPI_loc_accuracy_per_method_t *OpenAPI_loc_accuracy_per_method_create(
    OpenAPI_positioning_method_e pos_method,
    int loc_acc,
    bool is_los_nlos_pct,
    int los_nlos_pct,
    bool is_los_nlos_ind,
    int los_nlos_ind
)
{
    OpenAPI_loc_accuracy_per_method_t *loc_accuracy_per_method_local_var = ogs_malloc(sizeof(OpenAPI_loc_accuracy_per_method_t));
    ogs_assert(loc_accuracy_per_method_local_var);

    loc_accuracy_per_method_local_var->pos_method = pos_method;
    loc_accuracy_per_method_local_var->loc_acc = loc_acc;
    loc_accuracy_per_method_local_var->is_los_nlos_pct = is_los_nlos_pct;
    loc_accuracy_per_method_local_var->los_nlos_pct = los_nlos_pct;
    loc_accuracy_per_method_local_var->is_los_nlos_ind = is_los_nlos_ind;
    loc_accuracy_per_method_local_var->los_nlos_ind = los_nlos_ind;

    return loc_accuracy_per_method_local_var;
}

void OpenAPI_loc_accuracy_per_method_free(OpenAPI_loc_accuracy_per_method_t *loc_accuracy_per_method)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == loc_accuracy_per_method) {
        return;
    }
    ogs_free(loc_accuracy_per_method);
}

cJSON *OpenAPI_loc_accuracy_per_method_convertToJSON(OpenAPI_loc_accuracy_per_method_t *loc_accuracy_per_method)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (loc_accuracy_per_method == NULL) {
        ogs_error("OpenAPI_loc_accuracy_per_method_convertToJSON() failed [LocAccuracyPerMethod]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (loc_accuracy_per_method->pos_method == OpenAPI_positioning_method_NULL) {
        ogs_error("OpenAPI_loc_accuracy_per_method_convertToJSON() failed [pos_method]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "posMethod", OpenAPI_positioning_method_ToString(loc_accuracy_per_method->pos_method)) == NULL) {
        ogs_error("OpenAPI_loc_accuracy_per_method_convertToJSON() failed [pos_method]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "locAcc", loc_accuracy_per_method->loc_acc) == NULL) {
        ogs_error("OpenAPI_loc_accuracy_per_method_convertToJSON() failed [loc_acc]");
        goto end;
    }

    if (loc_accuracy_per_method->is_los_nlos_pct) {
    if (cJSON_AddNumberToObject(item, "losNlosPct", loc_accuracy_per_method->los_nlos_pct) == NULL) {
        ogs_error("OpenAPI_loc_accuracy_per_method_convertToJSON() failed [los_nlos_pct]");
        goto end;
    }
    }

    if (loc_accuracy_per_method->is_los_nlos_ind) {
    if (cJSON_AddBoolToObject(item, "losNlosInd", loc_accuracy_per_method->los_nlos_ind) == NULL) {
        ogs_error("OpenAPI_loc_accuracy_per_method_convertToJSON() failed [los_nlos_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_loc_accuracy_per_method_t *OpenAPI_loc_accuracy_per_method_parseFromJSON(cJSON *loc_accuracy_per_methodJSON)
{
    OpenAPI_loc_accuracy_per_method_t *loc_accuracy_per_method_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *pos_method = NULL;
    OpenAPI_positioning_method_e pos_methodVariable = 0;
    cJSON *loc_acc = NULL;
    cJSON *los_nlos_pct = NULL;
    cJSON *los_nlos_ind = NULL;
    pos_method = cJSON_GetObjectItemCaseSensitive(loc_accuracy_per_methodJSON, "posMethod");
    if (!pos_method) {
        ogs_error("OpenAPI_loc_accuracy_per_method_parseFromJSON() failed [pos_method]");
        goto end;
    }
    if (!cJSON_IsString(pos_method)) {
        ogs_error("OpenAPI_loc_accuracy_per_method_parseFromJSON() failed [pos_method]");
        goto end;
    }
    pos_methodVariable = OpenAPI_positioning_method_FromString(pos_method->valuestring);

    loc_acc = cJSON_GetObjectItemCaseSensitive(loc_accuracy_per_methodJSON, "locAcc");
    if (!loc_acc) {
        ogs_error("OpenAPI_loc_accuracy_per_method_parseFromJSON() failed [loc_acc]");
        goto end;
    }
    if (!cJSON_IsNumber(loc_acc)) {
        ogs_error("OpenAPI_loc_accuracy_per_method_parseFromJSON() failed [loc_acc]");
        goto end;
    }

    los_nlos_pct = cJSON_GetObjectItemCaseSensitive(loc_accuracy_per_methodJSON, "losNlosPct");
    if (los_nlos_pct) {
    if (!cJSON_IsNumber(los_nlos_pct)) {
        ogs_error("OpenAPI_loc_accuracy_per_method_parseFromJSON() failed [los_nlos_pct]");
        goto end;
    }
    }

    los_nlos_ind = cJSON_GetObjectItemCaseSensitive(loc_accuracy_per_methodJSON, "losNlosInd");
    if (los_nlos_ind) {
    if (!cJSON_IsBool(los_nlos_ind)) {
        ogs_error("OpenAPI_loc_accuracy_per_method_parseFromJSON() failed [los_nlos_ind]");
        goto end;
    }
    }

    loc_accuracy_per_method_local_var = OpenAPI_loc_accuracy_per_method_create (
        pos_methodVariable,
        
        loc_acc->valuedouble,
        los_nlos_pct ? true : false,
        los_nlos_pct ? los_nlos_pct->valuedouble : 0,
        los_nlos_ind ? true : false,
        los_nlos_ind ? los_nlos_ind->valueint : 0
    );

    return loc_accuracy_per_method_local_var;
end:
    return NULL;
}

OpenAPI_loc_accuracy_per_method_t *OpenAPI_loc_accuracy_per_method_copy(OpenAPI_loc_accuracy_per_method_t *dst, OpenAPI_loc_accuracy_per_method_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_loc_accuracy_per_method_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_loc_accuracy_per_method_convertToJSON() failed");
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

    OpenAPI_loc_accuracy_per_method_free(dst);
    dst = OpenAPI_loc_accuracy_per_method_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

