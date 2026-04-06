
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "loc_accuracy_req.h"

OpenAPI_loc_accuracy_req_t *OpenAPI_loc_accuracy_req_create(
    bool is_acc_thres,
    int acc_thres,
    OpenAPI_matching_direction_e acc_thres_match_dir,
    bool is_in_out_thres,
    int in_out_thres,
    OpenAPI_matching_direction_e in_out_thres_match_dir,
    OpenAPI_positioning_method_e pos_method
)
{
    OpenAPI_loc_accuracy_req_t *loc_accuracy_req_local_var = ogs_malloc(sizeof(OpenAPI_loc_accuracy_req_t));
    ogs_assert(loc_accuracy_req_local_var);

    loc_accuracy_req_local_var->is_acc_thres = is_acc_thres;
    loc_accuracy_req_local_var->acc_thres = acc_thres;
    loc_accuracy_req_local_var->acc_thres_match_dir = acc_thres_match_dir;
    loc_accuracy_req_local_var->is_in_out_thres = is_in_out_thres;
    loc_accuracy_req_local_var->in_out_thres = in_out_thres;
    loc_accuracy_req_local_var->in_out_thres_match_dir = in_out_thres_match_dir;
    loc_accuracy_req_local_var->pos_method = pos_method;

    return loc_accuracy_req_local_var;
}

void OpenAPI_loc_accuracy_req_free(OpenAPI_loc_accuracy_req_t *loc_accuracy_req)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == loc_accuracy_req) {
        return;
    }
    ogs_free(loc_accuracy_req);
}

cJSON *OpenAPI_loc_accuracy_req_convertToJSON(OpenAPI_loc_accuracy_req_t *loc_accuracy_req)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (loc_accuracy_req == NULL) {
        ogs_error("OpenAPI_loc_accuracy_req_convertToJSON() failed [LocAccuracyReq]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (loc_accuracy_req->is_acc_thres) {
    if (cJSON_AddNumberToObject(item, "accThres", loc_accuracy_req->acc_thres) == NULL) {
        ogs_error("OpenAPI_loc_accuracy_req_convertToJSON() failed [acc_thres]");
        goto end;
    }
    }

    if (loc_accuracy_req->acc_thres_match_dir != OpenAPI_matching_direction_NULL) {
    if (cJSON_AddStringToObject(item, "accThresMatchDir", OpenAPI_matching_direction_ToString(loc_accuracy_req->acc_thres_match_dir)) == NULL) {
        ogs_error("OpenAPI_loc_accuracy_req_convertToJSON() failed [acc_thres_match_dir]");
        goto end;
    }
    }

    if (loc_accuracy_req->is_in_out_thres) {
    if (cJSON_AddNumberToObject(item, "inOutThres", loc_accuracy_req->in_out_thres) == NULL) {
        ogs_error("OpenAPI_loc_accuracy_req_convertToJSON() failed [in_out_thres]");
        goto end;
    }
    }

    if (loc_accuracy_req->in_out_thres_match_dir != OpenAPI_matching_direction_NULL) {
    if (cJSON_AddStringToObject(item, "inOutThresMatchDir", OpenAPI_matching_direction_ToString(loc_accuracy_req->in_out_thres_match_dir)) == NULL) {
        ogs_error("OpenAPI_loc_accuracy_req_convertToJSON() failed [in_out_thres_match_dir]");
        goto end;
    }
    }

    if (loc_accuracy_req->pos_method != OpenAPI_positioning_method_NULL) {
    if (cJSON_AddStringToObject(item, "posMethod", OpenAPI_positioning_method_ToString(loc_accuracy_req->pos_method)) == NULL) {
        ogs_error("OpenAPI_loc_accuracy_req_convertToJSON() failed [pos_method]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_loc_accuracy_req_t *OpenAPI_loc_accuracy_req_parseFromJSON(cJSON *loc_accuracy_reqJSON)
{
    OpenAPI_loc_accuracy_req_t *loc_accuracy_req_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *acc_thres = NULL;
    cJSON *acc_thres_match_dir = NULL;
    OpenAPI_matching_direction_e acc_thres_match_dirVariable = 0;
    cJSON *in_out_thres = NULL;
    cJSON *in_out_thres_match_dir = NULL;
    OpenAPI_matching_direction_e in_out_thres_match_dirVariable = 0;
    cJSON *pos_method = NULL;
    OpenAPI_positioning_method_e pos_methodVariable = 0;
    acc_thres = cJSON_GetObjectItemCaseSensitive(loc_accuracy_reqJSON, "accThres");
    if (acc_thres) {
    if (!cJSON_IsNumber(acc_thres)) {
        ogs_error("OpenAPI_loc_accuracy_req_parseFromJSON() failed [acc_thres]");
        goto end;
    }
    }

    acc_thres_match_dir = cJSON_GetObjectItemCaseSensitive(loc_accuracy_reqJSON, "accThresMatchDir");
    if (acc_thres_match_dir) {
    if (!cJSON_IsString(acc_thres_match_dir)) {
        ogs_error("OpenAPI_loc_accuracy_req_parseFromJSON() failed [acc_thres_match_dir]");
        goto end;
    }
    acc_thres_match_dirVariable = OpenAPI_matching_direction_FromString(acc_thres_match_dir->valuestring);
    }

    in_out_thres = cJSON_GetObjectItemCaseSensitive(loc_accuracy_reqJSON, "inOutThres");
    if (in_out_thres) {
    if (!cJSON_IsNumber(in_out_thres)) {
        ogs_error("OpenAPI_loc_accuracy_req_parseFromJSON() failed [in_out_thres]");
        goto end;
    }
    }

    in_out_thres_match_dir = cJSON_GetObjectItemCaseSensitive(loc_accuracy_reqJSON, "inOutThresMatchDir");
    if (in_out_thres_match_dir) {
    if (!cJSON_IsString(in_out_thres_match_dir)) {
        ogs_error("OpenAPI_loc_accuracy_req_parseFromJSON() failed [in_out_thres_match_dir]");
        goto end;
    }
    in_out_thres_match_dirVariable = OpenAPI_matching_direction_FromString(in_out_thres_match_dir->valuestring);
    }

    pos_method = cJSON_GetObjectItemCaseSensitive(loc_accuracy_reqJSON, "posMethod");
    if (pos_method) {
    if (!cJSON_IsString(pos_method)) {
        ogs_error("OpenAPI_loc_accuracy_req_parseFromJSON() failed [pos_method]");
        goto end;
    }
    pos_methodVariable = OpenAPI_positioning_method_FromString(pos_method->valuestring);
    }

    loc_accuracy_req_local_var = OpenAPI_loc_accuracy_req_create (
        acc_thres ? true : false,
        acc_thres ? acc_thres->valuedouble : 0,
        acc_thres_match_dir ? acc_thres_match_dirVariable : 0,
        in_out_thres ? true : false,
        in_out_thres ? in_out_thres->valuedouble : 0,
        in_out_thres_match_dir ? in_out_thres_match_dirVariable : 0,
        pos_method ? pos_methodVariable : 0
    );

    return loc_accuracy_req_local_var;
end:
    return NULL;
}

OpenAPI_loc_accuracy_req_t *OpenAPI_loc_accuracy_req_copy(OpenAPI_loc_accuracy_req_t *dst, OpenAPI_loc_accuracy_req_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_loc_accuracy_req_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_loc_accuracy_req_convertToJSON() failed");
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

    OpenAPI_loc_accuracy_req_free(dst);
    dst = OpenAPI_loc_accuracy_req_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

