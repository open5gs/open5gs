
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "header_handling_reporting.h"

OpenAPI_header_handling_reporting_t *OpenAPI_header_handling_reporting_create(
    bool is_notif_flag,
    int notif_flag,
    OpenAPI_reporting_suggestion_information_t *rep_sugg_info,
    bool is_one_time_ind,
    int one_time_ind
)
{
    OpenAPI_header_handling_reporting_t *header_handling_reporting_local_var = ogs_malloc(sizeof(OpenAPI_header_handling_reporting_t));
    ogs_assert(header_handling_reporting_local_var);

    header_handling_reporting_local_var->is_notif_flag = is_notif_flag;
    header_handling_reporting_local_var->notif_flag = notif_flag;
    header_handling_reporting_local_var->rep_sugg_info = rep_sugg_info;
    header_handling_reporting_local_var->is_one_time_ind = is_one_time_ind;
    header_handling_reporting_local_var->one_time_ind = one_time_ind;

    return header_handling_reporting_local_var;
}

void OpenAPI_header_handling_reporting_free(OpenAPI_header_handling_reporting_t *header_handling_reporting)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == header_handling_reporting) {
        return;
    }
    if (header_handling_reporting->rep_sugg_info) {
        OpenAPI_reporting_suggestion_information_free(header_handling_reporting->rep_sugg_info);
        header_handling_reporting->rep_sugg_info = NULL;
    }
    ogs_free(header_handling_reporting);
}

cJSON *OpenAPI_header_handling_reporting_convertToJSON(OpenAPI_header_handling_reporting_t *header_handling_reporting)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (header_handling_reporting == NULL) {
        ogs_error("OpenAPI_header_handling_reporting_convertToJSON() failed [HeaderHandlingReporting]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (header_handling_reporting->is_notif_flag) {
    if (cJSON_AddBoolToObject(item, "notifFlag", header_handling_reporting->notif_flag) == NULL) {
        ogs_error("OpenAPI_header_handling_reporting_convertToJSON() failed [notif_flag]");
        goto end;
    }
    }

    if (header_handling_reporting->rep_sugg_info) {
    cJSON *rep_sugg_info_local_JSON = OpenAPI_reporting_suggestion_information_convertToJSON(header_handling_reporting->rep_sugg_info);
    if (rep_sugg_info_local_JSON == NULL) {
        ogs_error("OpenAPI_header_handling_reporting_convertToJSON() failed [rep_sugg_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "repSuggInfo", rep_sugg_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_header_handling_reporting_convertToJSON() failed [rep_sugg_info]");
        goto end;
    }
    }

    if (header_handling_reporting->is_one_time_ind) {
    if (cJSON_AddBoolToObject(item, "oneTimeInd", header_handling_reporting->one_time_ind) == NULL) {
        ogs_error("OpenAPI_header_handling_reporting_convertToJSON() failed [one_time_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_header_handling_reporting_t *OpenAPI_header_handling_reporting_parseFromJSON(cJSON *header_handling_reportingJSON)
{
    OpenAPI_header_handling_reporting_t *header_handling_reporting_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *notif_flag = NULL;
    cJSON *rep_sugg_info = NULL;
    OpenAPI_reporting_suggestion_information_t *rep_sugg_info_local_nonprim = NULL;
    cJSON *one_time_ind = NULL;
    notif_flag = cJSON_GetObjectItemCaseSensitive(header_handling_reportingJSON, "notifFlag");
    if (notif_flag) {
    if (!cJSON_IsBool(notif_flag)) {
        ogs_error("OpenAPI_header_handling_reporting_parseFromJSON() failed [notif_flag]");
        goto end;
    }
    }

    rep_sugg_info = cJSON_GetObjectItemCaseSensitive(header_handling_reportingJSON, "repSuggInfo");
    if (rep_sugg_info) {
    rep_sugg_info_local_nonprim = OpenAPI_reporting_suggestion_information_parseFromJSON(rep_sugg_info);
    if (!rep_sugg_info_local_nonprim) {
        ogs_error("OpenAPI_reporting_suggestion_information_parseFromJSON failed [rep_sugg_info]");
        goto end;
    }
    }

    one_time_ind = cJSON_GetObjectItemCaseSensitive(header_handling_reportingJSON, "oneTimeInd");
    if (one_time_ind) {
    if (!cJSON_IsBool(one_time_ind)) {
        ogs_error("OpenAPI_header_handling_reporting_parseFromJSON() failed [one_time_ind]");
        goto end;
    }
    }

    header_handling_reporting_local_var = OpenAPI_header_handling_reporting_create (
        notif_flag ? true : false,
        notif_flag ? notif_flag->valueint : 0,
        rep_sugg_info ? rep_sugg_info_local_nonprim : NULL,
        one_time_ind ? true : false,
        one_time_ind ? one_time_ind->valueint : 0
    );

    return header_handling_reporting_local_var;
end:
    if (rep_sugg_info_local_nonprim) {
        OpenAPI_reporting_suggestion_information_free(rep_sugg_info_local_nonprim);
        rep_sugg_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_header_handling_reporting_t *OpenAPI_header_handling_reporting_copy(OpenAPI_header_handling_reporting_t *dst, OpenAPI_header_handling_reporting_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_header_handling_reporting_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_header_handling_reporting_convertToJSON() failed");
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

    OpenAPI_header_handling_reporting_free(dst);
    dst = OpenAPI_header_handling_reporting_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

