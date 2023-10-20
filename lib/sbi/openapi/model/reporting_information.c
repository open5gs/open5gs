
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "reporting_information.h"

OpenAPI_reporting_information_t *OpenAPI_reporting_information_create(
    bool is_imm_rep,
    int imm_rep,
    OpenAPI_notification_method_1_t *notif_method,
    bool is_max_report_nbr,
    int max_report_nbr,
    char *mon_dur,
    bool is_rep_period,
    int rep_period,
    bool is_samp_ratio,
    int samp_ratio,
    OpenAPI_list_t *partition_criteria,
    bool is_grp_rep_time,
    int grp_rep_time,
    OpenAPI_notification_flag_e notif_flag
)
{
    OpenAPI_reporting_information_t *reporting_information_local_var = ogs_malloc(sizeof(OpenAPI_reporting_information_t));
    ogs_assert(reporting_information_local_var);

    reporting_information_local_var->is_imm_rep = is_imm_rep;
    reporting_information_local_var->imm_rep = imm_rep;
    reporting_information_local_var->notif_method = notif_method;
    reporting_information_local_var->is_max_report_nbr = is_max_report_nbr;
    reporting_information_local_var->max_report_nbr = max_report_nbr;
    reporting_information_local_var->mon_dur = mon_dur;
    reporting_information_local_var->is_rep_period = is_rep_period;
    reporting_information_local_var->rep_period = rep_period;
    reporting_information_local_var->is_samp_ratio = is_samp_ratio;
    reporting_information_local_var->samp_ratio = samp_ratio;
    reporting_information_local_var->partition_criteria = partition_criteria;
    reporting_information_local_var->is_grp_rep_time = is_grp_rep_time;
    reporting_information_local_var->grp_rep_time = grp_rep_time;
    reporting_information_local_var->notif_flag = notif_flag;

    return reporting_information_local_var;
}

void OpenAPI_reporting_information_free(OpenAPI_reporting_information_t *reporting_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == reporting_information) {
        return;
    }
    if (reporting_information->notif_method) {
        OpenAPI_notification_method_1_free(reporting_information->notif_method);
        reporting_information->notif_method = NULL;
    }
    if (reporting_information->mon_dur) {
        ogs_free(reporting_information->mon_dur);
        reporting_information->mon_dur = NULL;
    }
    if (reporting_information->partition_criteria) {
        OpenAPI_list_free(reporting_information->partition_criteria);
        reporting_information->partition_criteria = NULL;
    }
    ogs_free(reporting_information);
}

cJSON *OpenAPI_reporting_information_convertToJSON(OpenAPI_reporting_information_t *reporting_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (reporting_information == NULL) {
        ogs_error("OpenAPI_reporting_information_convertToJSON() failed [ReportingInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (reporting_information->is_imm_rep) {
    if (cJSON_AddBoolToObject(item, "immRep", reporting_information->imm_rep) == NULL) {
        ogs_error("OpenAPI_reporting_information_convertToJSON() failed [imm_rep]");
        goto end;
    }
    }

    if (reporting_information->notif_method) {
    cJSON *notif_method_local_JSON = OpenAPI_notification_method_1_convertToJSON(reporting_information->notif_method);
    if (notif_method_local_JSON == NULL) {
        ogs_error("OpenAPI_reporting_information_convertToJSON() failed [notif_method]");
        goto end;
    }
    cJSON_AddItemToObject(item, "notifMethod", notif_method_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_reporting_information_convertToJSON() failed [notif_method]");
        goto end;
    }
    }

    if (reporting_information->is_max_report_nbr) {
    if (cJSON_AddNumberToObject(item, "maxReportNbr", reporting_information->max_report_nbr) == NULL) {
        ogs_error("OpenAPI_reporting_information_convertToJSON() failed [max_report_nbr]");
        goto end;
    }
    }

    if (reporting_information->mon_dur) {
    if (cJSON_AddStringToObject(item, "monDur", reporting_information->mon_dur) == NULL) {
        ogs_error("OpenAPI_reporting_information_convertToJSON() failed [mon_dur]");
        goto end;
    }
    }

    if (reporting_information->is_rep_period) {
    if (cJSON_AddNumberToObject(item, "repPeriod", reporting_information->rep_period) == NULL) {
        ogs_error("OpenAPI_reporting_information_convertToJSON() failed [rep_period]");
        goto end;
    }
    }

    if (reporting_information->is_samp_ratio) {
    if (cJSON_AddNumberToObject(item, "sampRatio", reporting_information->samp_ratio) == NULL) {
        ogs_error("OpenAPI_reporting_information_convertToJSON() failed [samp_ratio]");
        goto end;
    }
    }

    if (reporting_information->partition_criteria != OpenAPI_partitioning_criteria_NULL) {
    cJSON *partition_criteriaList = cJSON_AddArrayToObject(item, "partitionCriteria");
    if (partition_criteriaList == NULL) {
        ogs_error("OpenAPI_reporting_information_convertToJSON() failed [partition_criteria]");
        goto end;
    }
    OpenAPI_list_for_each(reporting_information->partition_criteria, node) {
        if (cJSON_AddStringToObject(partition_criteriaList, "", OpenAPI_partitioning_criteria_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_reporting_information_convertToJSON() failed [partition_criteria]");
            goto end;
        }
    }
    }

    if (reporting_information->is_grp_rep_time) {
    if (cJSON_AddNumberToObject(item, "grpRepTime", reporting_information->grp_rep_time) == NULL) {
        ogs_error("OpenAPI_reporting_information_convertToJSON() failed [grp_rep_time]");
        goto end;
    }
    }

    if (reporting_information->notif_flag != OpenAPI_notification_flag_NULL) {
    if (cJSON_AddStringToObject(item, "notifFlag", OpenAPI_notification_flag_ToString(reporting_information->notif_flag)) == NULL) {
        ogs_error("OpenAPI_reporting_information_convertToJSON() failed [notif_flag]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_reporting_information_t *OpenAPI_reporting_information_parseFromJSON(cJSON *reporting_informationJSON)
{
    OpenAPI_reporting_information_t *reporting_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *imm_rep = NULL;
    cJSON *notif_method = NULL;
    OpenAPI_notification_method_1_t *notif_method_local_nonprim = NULL;
    cJSON *max_report_nbr = NULL;
    cJSON *mon_dur = NULL;
    cJSON *rep_period = NULL;
    cJSON *samp_ratio = NULL;
    cJSON *partition_criteria = NULL;
    OpenAPI_list_t *partition_criteriaList = NULL;
    cJSON *grp_rep_time = NULL;
    cJSON *notif_flag = NULL;
    OpenAPI_notification_flag_e notif_flagVariable = 0;
    imm_rep = cJSON_GetObjectItemCaseSensitive(reporting_informationJSON, "immRep");
    if (imm_rep) {
    if (!cJSON_IsBool(imm_rep)) {
        ogs_error("OpenAPI_reporting_information_parseFromJSON() failed [imm_rep]");
        goto end;
    }
    }

    notif_method = cJSON_GetObjectItemCaseSensitive(reporting_informationJSON, "notifMethod");
    if (notif_method) {
    notif_method_local_nonprim = OpenAPI_notification_method_1_parseFromJSON(notif_method);
    if (!notif_method_local_nonprim) {
        ogs_error("OpenAPI_notification_method_1_parseFromJSON failed [notif_method]");
        goto end;
    }
    }

    max_report_nbr = cJSON_GetObjectItemCaseSensitive(reporting_informationJSON, "maxReportNbr");
    if (max_report_nbr) {
    if (!cJSON_IsNumber(max_report_nbr)) {
        ogs_error("OpenAPI_reporting_information_parseFromJSON() failed [max_report_nbr]");
        goto end;
    }
    }

    mon_dur = cJSON_GetObjectItemCaseSensitive(reporting_informationJSON, "monDur");
    if (mon_dur) {
    if (!cJSON_IsString(mon_dur) && !cJSON_IsNull(mon_dur)) {
        ogs_error("OpenAPI_reporting_information_parseFromJSON() failed [mon_dur]");
        goto end;
    }
    }

    rep_period = cJSON_GetObjectItemCaseSensitive(reporting_informationJSON, "repPeriod");
    if (rep_period) {
    if (!cJSON_IsNumber(rep_period)) {
        ogs_error("OpenAPI_reporting_information_parseFromJSON() failed [rep_period]");
        goto end;
    }
    }

    samp_ratio = cJSON_GetObjectItemCaseSensitive(reporting_informationJSON, "sampRatio");
    if (samp_ratio) {
    if (!cJSON_IsNumber(samp_ratio)) {
        ogs_error("OpenAPI_reporting_information_parseFromJSON() failed [samp_ratio]");
        goto end;
    }
    }

    partition_criteria = cJSON_GetObjectItemCaseSensitive(reporting_informationJSON, "partitionCriteria");
    if (partition_criteria) {
        cJSON *partition_criteria_local = NULL;
        if (!cJSON_IsArray(partition_criteria)) {
            ogs_error("OpenAPI_reporting_information_parseFromJSON() failed [partition_criteria]");
            goto end;
        }

        partition_criteriaList = OpenAPI_list_create();

        cJSON_ArrayForEach(partition_criteria_local, partition_criteria) {
            OpenAPI_partitioning_criteria_e localEnum = OpenAPI_partitioning_criteria_NULL;
            if (!cJSON_IsString(partition_criteria_local)) {
                ogs_error("OpenAPI_reporting_information_parseFromJSON() failed [partition_criteria]");
                goto end;
            }
            localEnum = OpenAPI_partitioning_criteria_FromString(partition_criteria_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"partition_criteria\" is not supported. Ignoring it ...",
                         partition_criteria_local->valuestring);
            } else {
                OpenAPI_list_add(partition_criteriaList, (void *)localEnum);
            }
        }
        if (partition_criteriaList->count == 0) {
            ogs_error("OpenAPI_reporting_information_parseFromJSON() failed: Expected partition_criteriaList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    grp_rep_time = cJSON_GetObjectItemCaseSensitive(reporting_informationJSON, "grpRepTime");
    if (grp_rep_time) {
    if (!cJSON_IsNumber(grp_rep_time)) {
        ogs_error("OpenAPI_reporting_information_parseFromJSON() failed [grp_rep_time]");
        goto end;
    }
    }

    notif_flag = cJSON_GetObjectItemCaseSensitive(reporting_informationJSON, "notifFlag");
    if (notif_flag) {
    if (!cJSON_IsString(notif_flag)) {
        ogs_error("OpenAPI_reporting_information_parseFromJSON() failed [notif_flag]");
        goto end;
    }
    notif_flagVariable = OpenAPI_notification_flag_FromString(notif_flag->valuestring);
    }

    reporting_information_local_var = OpenAPI_reporting_information_create (
        imm_rep ? true : false,
        imm_rep ? imm_rep->valueint : 0,
        notif_method ? notif_method_local_nonprim : NULL,
        max_report_nbr ? true : false,
        max_report_nbr ? max_report_nbr->valuedouble : 0,
        mon_dur && !cJSON_IsNull(mon_dur) ? ogs_strdup(mon_dur->valuestring) : NULL,
        rep_period ? true : false,
        rep_period ? rep_period->valuedouble : 0,
        samp_ratio ? true : false,
        samp_ratio ? samp_ratio->valuedouble : 0,
        partition_criteria ? partition_criteriaList : NULL,
        grp_rep_time ? true : false,
        grp_rep_time ? grp_rep_time->valuedouble : 0,
        notif_flag ? notif_flagVariable : 0
    );

    return reporting_information_local_var;
end:
    if (notif_method_local_nonprim) {
        OpenAPI_notification_method_1_free(notif_method_local_nonprim);
        notif_method_local_nonprim = NULL;
    }
    if (partition_criteriaList) {
        OpenAPI_list_free(partition_criteriaList);
        partition_criteriaList = NULL;
    }
    return NULL;
}

OpenAPI_reporting_information_t *OpenAPI_reporting_information_copy(OpenAPI_reporting_information_t *dst, OpenAPI_reporting_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_reporting_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_reporting_information_convertToJSON() failed");
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

    OpenAPI_reporting_information_free(dst);
    dst = OpenAPI_reporting_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

