
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mms_charging_information.h"

OpenAPI_mms_charging_information_t *OpenAPI_mms_charging_information_create(
    OpenAPI_mm_originator_info_t *mm_originator_info,
    OpenAPI_list_t *mm_recipient_info_list,
    OpenAPI_charging_user_location_t *user_locationinfo,
    char *uetime_zone,
    OpenAPI_rat_type_e r_at_type,
    char *correlation_information,
    char *submission_time,
    OpenAPI_mm_content_type_t *mm_content_type,
    OpenAPI_sm_priority_e mm_priority,
    char *message_id,
    char *message_type,
    bool is_message_size,
    int message_size,
    char *message_class,
    bool is_delivery_report_requested,
    int delivery_report_requested,
    bool is_read_reply_report_requested,
    int read_reply_report_requested,
    char *applic_id,
    char *reply_applic_id,
    char *aux_applic_info,
    char *content_class,
    bool is_d_rm_content,
    int d_rm_content,
    bool is_adaptations,
    int adaptations,
    char *vas_id,
    char *vasp_id
)
{
    OpenAPI_mms_charging_information_t *mms_charging_information_local_var = ogs_malloc(sizeof(OpenAPI_mms_charging_information_t));
    ogs_assert(mms_charging_information_local_var);

    mms_charging_information_local_var->mm_originator_info = mm_originator_info;
    mms_charging_information_local_var->mm_recipient_info_list = mm_recipient_info_list;
    mms_charging_information_local_var->user_locationinfo = user_locationinfo;
    mms_charging_information_local_var->uetime_zone = uetime_zone;
    mms_charging_information_local_var->r_at_type = r_at_type;
    mms_charging_information_local_var->correlation_information = correlation_information;
    mms_charging_information_local_var->submission_time = submission_time;
    mms_charging_information_local_var->mm_content_type = mm_content_type;
    mms_charging_information_local_var->mm_priority = mm_priority;
    mms_charging_information_local_var->message_id = message_id;
    mms_charging_information_local_var->message_type = message_type;
    mms_charging_information_local_var->is_message_size = is_message_size;
    mms_charging_information_local_var->message_size = message_size;
    mms_charging_information_local_var->message_class = message_class;
    mms_charging_information_local_var->is_delivery_report_requested = is_delivery_report_requested;
    mms_charging_information_local_var->delivery_report_requested = delivery_report_requested;
    mms_charging_information_local_var->is_read_reply_report_requested = is_read_reply_report_requested;
    mms_charging_information_local_var->read_reply_report_requested = read_reply_report_requested;
    mms_charging_information_local_var->applic_id = applic_id;
    mms_charging_information_local_var->reply_applic_id = reply_applic_id;
    mms_charging_information_local_var->aux_applic_info = aux_applic_info;
    mms_charging_information_local_var->content_class = content_class;
    mms_charging_information_local_var->is_d_rm_content = is_d_rm_content;
    mms_charging_information_local_var->d_rm_content = d_rm_content;
    mms_charging_information_local_var->is_adaptations = is_adaptations;
    mms_charging_information_local_var->adaptations = adaptations;
    mms_charging_information_local_var->vas_id = vas_id;
    mms_charging_information_local_var->vasp_id = vasp_id;

    return mms_charging_information_local_var;
}

void OpenAPI_mms_charging_information_free(OpenAPI_mms_charging_information_t *mms_charging_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == mms_charging_information) {
        return;
    }
    if (mms_charging_information->mm_originator_info) {
        OpenAPI_mm_originator_info_free(mms_charging_information->mm_originator_info);
        mms_charging_information->mm_originator_info = NULL;
    }
    if (mms_charging_information->mm_recipient_info_list) {
        OpenAPI_list_for_each(mms_charging_information->mm_recipient_info_list, node) {
            OpenAPI_mm_recipient_info_free(node->data);
        }
        OpenAPI_list_free(mms_charging_information->mm_recipient_info_list);
        mms_charging_information->mm_recipient_info_list = NULL;
    }
    if (mms_charging_information->user_locationinfo) {
        OpenAPI_charging_user_location_free(mms_charging_information->user_locationinfo);
        mms_charging_information->user_locationinfo = NULL;
    }
    if (mms_charging_information->uetime_zone) {
        ogs_free(mms_charging_information->uetime_zone);
        mms_charging_information->uetime_zone = NULL;
    }
    if (mms_charging_information->correlation_information) {
        ogs_free(mms_charging_information->correlation_information);
        mms_charging_information->correlation_information = NULL;
    }
    if (mms_charging_information->submission_time) {
        ogs_free(mms_charging_information->submission_time);
        mms_charging_information->submission_time = NULL;
    }
    if (mms_charging_information->mm_content_type) {
        OpenAPI_mm_content_type_free(mms_charging_information->mm_content_type);
        mms_charging_information->mm_content_type = NULL;
    }
    if (mms_charging_information->message_id) {
        ogs_free(mms_charging_information->message_id);
        mms_charging_information->message_id = NULL;
    }
    if (mms_charging_information->message_type) {
        ogs_free(mms_charging_information->message_type);
        mms_charging_information->message_type = NULL;
    }
    if (mms_charging_information->message_class) {
        ogs_free(mms_charging_information->message_class);
        mms_charging_information->message_class = NULL;
    }
    if (mms_charging_information->applic_id) {
        ogs_free(mms_charging_information->applic_id);
        mms_charging_information->applic_id = NULL;
    }
    if (mms_charging_information->reply_applic_id) {
        ogs_free(mms_charging_information->reply_applic_id);
        mms_charging_information->reply_applic_id = NULL;
    }
    if (mms_charging_information->aux_applic_info) {
        ogs_free(mms_charging_information->aux_applic_info);
        mms_charging_information->aux_applic_info = NULL;
    }
    if (mms_charging_information->content_class) {
        ogs_free(mms_charging_information->content_class);
        mms_charging_information->content_class = NULL;
    }
    if (mms_charging_information->vas_id) {
        ogs_free(mms_charging_information->vas_id);
        mms_charging_information->vas_id = NULL;
    }
    if (mms_charging_information->vasp_id) {
        ogs_free(mms_charging_information->vasp_id);
        mms_charging_information->vasp_id = NULL;
    }
    ogs_free(mms_charging_information);
}

cJSON *OpenAPI_mms_charging_information_convertToJSON(OpenAPI_mms_charging_information_t *mms_charging_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (mms_charging_information == NULL) {
        ogs_error("OpenAPI_mms_charging_information_convertToJSON() failed [MMSChargingInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (mms_charging_information->mm_originator_info) {
    cJSON *mm_originator_info_local_JSON = OpenAPI_mm_originator_info_convertToJSON(mms_charging_information->mm_originator_info);
    if (mm_originator_info_local_JSON == NULL) {
        ogs_error("OpenAPI_mms_charging_information_convertToJSON() failed [mm_originator_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "mmOriginatorInfo", mm_originator_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_mms_charging_information_convertToJSON() failed [mm_originator_info]");
        goto end;
    }
    }

    if (mms_charging_information->mm_recipient_info_list) {
    cJSON *mm_recipient_info_listList = cJSON_AddArrayToObject(item, "mmRecipientInfoList");
    if (mm_recipient_info_listList == NULL) {
        ogs_error("OpenAPI_mms_charging_information_convertToJSON() failed [mm_recipient_info_list]");
        goto end;
    }
    OpenAPI_list_for_each(mms_charging_information->mm_recipient_info_list, node) {
        cJSON *itemLocal = OpenAPI_mm_recipient_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_mms_charging_information_convertToJSON() failed [mm_recipient_info_list]");
            goto end;
        }
        cJSON_AddItemToArray(mm_recipient_info_listList, itemLocal);
    }
    }

    if (mms_charging_information->user_locationinfo) {
    cJSON *user_locationinfo_local_JSON = OpenAPI_charging_user_location_convertToJSON(mms_charging_information->user_locationinfo);
    if (user_locationinfo_local_JSON == NULL) {
        ogs_error("OpenAPI_mms_charging_information_convertToJSON() failed [user_locationinfo]");
        goto end;
    }
    cJSON_AddItemToObject(item, "userLocationinfo", user_locationinfo_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_mms_charging_information_convertToJSON() failed [user_locationinfo]");
        goto end;
    }
    }

    if (mms_charging_information->uetime_zone) {
    if (cJSON_AddStringToObject(item, "uetimeZone", mms_charging_information->uetime_zone) == NULL) {
        ogs_error("OpenAPI_mms_charging_information_convertToJSON() failed [uetime_zone]");
        goto end;
    }
    }

    if (mms_charging_information->r_at_type != OpenAPI_rat_type_NULL) {
    if (cJSON_AddStringToObject(item, "rATType", OpenAPI_rat_type_ToString(mms_charging_information->r_at_type)) == NULL) {
        ogs_error("OpenAPI_mms_charging_information_convertToJSON() failed [r_at_type]");
        goto end;
    }
    }

    if (mms_charging_information->correlation_information) {
    if (cJSON_AddStringToObject(item, "correlationInformation", mms_charging_information->correlation_information) == NULL) {
        ogs_error("OpenAPI_mms_charging_information_convertToJSON() failed [correlation_information]");
        goto end;
    }
    }

    if (mms_charging_information->submission_time) {
    if (cJSON_AddStringToObject(item, "submissionTime", mms_charging_information->submission_time) == NULL) {
        ogs_error("OpenAPI_mms_charging_information_convertToJSON() failed [submission_time]");
        goto end;
    }
    }

    if (mms_charging_information->mm_content_type) {
    cJSON *mm_content_type_local_JSON = OpenAPI_mm_content_type_convertToJSON(mms_charging_information->mm_content_type);
    if (mm_content_type_local_JSON == NULL) {
        ogs_error("OpenAPI_mms_charging_information_convertToJSON() failed [mm_content_type]");
        goto end;
    }
    cJSON_AddItemToObject(item, "mmContentType", mm_content_type_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_mms_charging_information_convertToJSON() failed [mm_content_type]");
        goto end;
    }
    }

    if (mms_charging_information->mm_priority != OpenAPI_sm_priority_NULL) {
    if (cJSON_AddStringToObject(item, "mmPriority", OpenAPI_sm_priority_ToString(mms_charging_information->mm_priority)) == NULL) {
        ogs_error("OpenAPI_mms_charging_information_convertToJSON() failed [mm_priority]");
        goto end;
    }
    }

    if (mms_charging_information->message_id) {
    if (cJSON_AddStringToObject(item, "messageID", mms_charging_information->message_id) == NULL) {
        ogs_error("OpenAPI_mms_charging_information_convertToJSON() failed [message_id]");
        goto end;
    }
    }

    if (mms_charging_information->message_type) {
    if (cJSON_AddStringToObject(item, "messageType", mms_charging_information->message_type) == NULL) {
        ogs_error("OpenAPI_mms_charging_information_convertToJSON() failed [message_type]");
        goto end;
    }
    }

    if (mms_charging_information->is_message_size) {
    if (cJSON_AddNumberToObject(item, "messageSize", mms_charging_information->message_size) == NULL) {
        ogs_error("OpenAPI_mms_charging_information_convertToJSON() failed [message_size]");
        goto end;
    }
    }

    if (mms_charging_information->message_class) {
    if (cJSON_AddStringToObject(item, "messageClass", mms_charging_information->message_class) == NULL) {
        ogs_error("OpenAPI_mms_charging_information_convertToJSON() failed [message_class]");
        goto end;
    }
    }

    if (mms_charging_information->is_delivery_report_requested) {
    if (cJSON_AddBoolToObject(item, "deliveryReportRequested", mms_charging_information->delivery_report_requested) == NULL) {
        ogs_error("OpenAPI_mms_charging_information_convertToJSON() failed [delivery_report_requested]");
        goto end;
    }
    }

    if (mms_charging_information->is_read_reply_report_requested) {
    if (cJSON_AddBoolToObject(item, "readReplyReportRequested", mms_charging_information->read_reply_report_requested) == NULL) {
        ogs_error("OpenAPI_mms_charging_information_convertToJSON() failed [read_reply_report_requested]");
        goto end;
    }
    }

    if (mms_charging_information->applic_id) {
    if (cJSON_AddStringToObject(item, "applicID", mms_charging_information->applic_id) == NULL) {
        ogs_error("OpenAPI_mms_charging_information_convertToJSON() failed [applic_id]");
        goto end;
    }
    }

    if (mms_charging_information->reply_applic_id) {
    if (cJSON_AddStringToObject(item, "replyApplicID", mms_charging_information->reply_applic_id) == NULL) {
        ogs_error("OpenAPI_mms_charging_information_convertToJSON() failed [reply_applic_id]");
        goto end;
    }
    }

    if (mms_charging_information->aux_applic_info) {
    if (cJSON_AddStringToObject(item, "auxApplicInfo", mms_charging_information->aux_applic_info) == NULL) {
        ogs_error("OpenAPI_mms_charging_information_convertToJSON() failed [aux_applic_info]");
        goto end;
    }
    }

    if (mms_charging_information->content_class) {
    if (cJSON_AddStringToObject(item, "contentClass", mms_charging_information->content_class) == NULL) {
        ogs_error("OpenAPI_mms_charging_information_convertToJSON() failed [content_class]");
        goto end;
    }
    }

    if (mms_charging_information->is_d_rm_content) {
    if (cJSON_AddBoolToObject(item, "dRMContent", mms_charging_information->d_rm_content) == NULL) {
        ogs_error("OpenAPI_mms_charging_information_convertToJSON() failed [d_rm_content]");
        goto end;
    }
    }

    if (mms_charging_information->is_adaptations) {
    if (cJSON_AddBoolToObject(item, "adaptations", mms_charging_information->adaptations) == NULL) {
        ogs_error("OpenAPI_mms_charging_information_convertToJSON() failed [adaptations]");
        goto end;
    }
    }

    if (mms_charging_information->vas_id) {
    if (cJSON_AddStringToObject(item, "vasID", mms_charging_information->vas_id) == NULL) {
        ogs_error("OpenAPI_mms_charging_information_convertToJSON() failed [vas_id]");
        goto end;
    }
    }

    if (mms_charging_information->vasp_id) {
    if (cJSON_AddStringToObject(item, "vaspID", mms_charging_information->vasp_id) == NULL) {
        ogs_error("OpenAPI_mms_charging_information_convertToJSON() failed [vasp_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_mms_charging_information_t *OpenAPI_mms_charging_information_parseFromJSON(cJSON *mms_charging_informationJSON)
{
    OpenAPI_mms_charging_information_t *mms_charging_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *mm_originator_info = NULL;
    OpenAPI_mm_originator_info_t *mm_originator_info_local_nonprim = NULL;
    cJSON *mm_recipient_info_list = NULL;
    OpenAPI_list_t *mm_recipient_info_listList = NULL;
    cJSON *user_locationinfo = NULL;
    OpenAPI_charging_user_location_t *user_locationinfo_local_nonprim = NULL;
    cJSON *uetime_zone = NULL;
    cJSON *r_at_type = NULL;
    OpenAPI_rat_type_e r_at_typeVariable = 0;
    cJSON *correlation_information = NULL;
    cJSON *submission_time = NULL;
    cJSON *mm_content_type = NULL;
    OpenAPI_mm_content_type_t *mm_content_type_local_nonprim = NULL;
    cJSON *mm_priority = NULL;
    OpenAPI_sm_priority_e mm_priorityVariable = 0;
    cJSON *message_id = NULL;
    cJSON *message_type = NULL;
    cJSON *message_size = NULL;
    cJSON *message_class = NULL;
    cJSON *delivery_report_requested = NULL;
    cJSON *read_reply_report_requested = NULL;
    cJSON *applic_id = NULL;
    cJSON *reply_applic_id = NULL;
    cJSON *aux_applic_info = NULL;
    cJSON *content_class = NULL;
    cJSON *d_rm_content = NULL;
    cJSON *adaptations = NULL;
    cJSON *vas_id = NULL;
    cJSON *vasp_id = NULL;
    mm_originator_info = cJSON_GetObjectItemCaseSensitive(mms_charging_informationJSON, "mmOriginatorInfo");
    if (mm_originator_info) {
    mm_originator_info_local_nonprim = OpenAPI_mm_originator_info_parseFromJSON(mm_originator_info);
    if (!mm_originator_info_local_nonprim) {
        ogs_error("OpenAPI_mm_originator_info_parseFromJSON failed [mm_originator_info]");
        goto end;
    }
    }

    mm_recipient_info_list = cJSON_GetObjectItemCaseSensitive(mms_charging_informationJSON, "mmRecipientInfoList");
    if (mm_recipient_info_list) {
        cJSON *mm_recipient_info_list_local = NULL;
        if (!cJSON_IsArray(mm_recipient_info_list)) {
            ogs_error("OpenAPI_mms_charging_information_parseFromJSON() failed [mm_recipient_info_list]");
            goto end;
        }

        mm_recipient_info_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(mm_recipient_info_list_local, mm_recipient_info_list) {
            if (!cJSON_IsObject(mm_recipient_info_list_local)) {
                ogs_error("OpenAPI_mms_charging_information_parseFromJSON() failed [mm_recipient_info_list]");
                goto end;
            }
            OpenAPI_mm_recipient_info_t *mm_recipient_info_listItem = OpenAPI_mm_recipient_info_parseFromJSON(mm_recipient_info_list_local);
            if (!mm_recipient_info_listItem) {
                ogs_error("No mm_recipient_info_listItem");
                goto end;
            }
            OpenAPI_list_add(mm_recipient_info_listList, mm_recipient_info_listItem);
        }
    }

    user_locationinfo = cJSON_GetObjectItemCaseSensitive(mms_charging_informationJSON, "userLocationinfo");
    if (user_locationinfo) {
    user_locationinfo_local_nonprim = OpenAPI_charging_user_location_parseFromJSON(user_locationinfo);
    if (!user_locationinfo_local_nonprim) {
        ogs_error("OpenAPI_charging_user_location_parseFromJSON failed [user_locationinfo]");
        goto end;
    }
    }

    uetime_zone = cJSON_GetObjectItemCaseSensitive(mms_charging_informationJSON, "uetimeZone");
    if (uetime_zone) {
    if (!cJSON_IsString(uetime_zone) && !cJSON_IsNull(uetime_zone)) {
        ogs_error("OpenAPI_mms_charging_information_parseFromJSON() failed [uetime_zone]");
        goto end;
    }
    }

    r_at_type = cJSON_GetObjectItemCaseSensitive(mms_charging_informationJSON, "rATType");
    if (r_at_type) {
    if (!cJSON_IsString(r_at_type)) {
        ogs_error("OpenAPI_mms_charging_information_parseFromJSON() failed [r_at_type]");
        goto end;
    }
    r_at_typeVariable = OpenAPI_rat_type_FromString(r_at_type->valuestring);
    }

    correlation_information = cJSON_GetObjectItemCaseSensitive(mms_charging_informationJSON, "correlationInformation");
    if (correlation_information) {
    if (!cJSON_IsString(correlation_information) && !cJSON_IsNull(correlation_information)) {
        ogs_error("OpenAPI_mms_charging_information_parseFromJSON() failed [correlation_information]");
        goto end;
    }
    }

    submission_time = cJSON_GetObjectItemCaseSensitive(mms_charging_informationJSON, "submissionTime");
    if (submission_time) {
    if (!cJSON_IsString(submission_time) && !cJSON_IsNull(submission_time)) {
        ogs_error("OpenAPI_mms_charging_information_parseFromJSON() failed [submission_time]");
        goto end;
    }
    }

    mm_content_type = cJSON_GetObjectItemCaseSensitive(mms_charging_informationJSON, "mmContentType");
    if (mm_content_type) {
    mm_content_type_local_nonprim = OpenAPI_mm_content_type_parseFromJSON(mm_content_type);
    if (!mm_content_type_local_nonprim) {
        ogs_error("OpenAPI_mm_content_type_parseFromJSON failed [mm_content_type]");
        goto end;
    }
    }

    mm_priority = cJSON_GetObjectItemCaseSensitive(mms_charging_informationJSON, "mmPriority");
    if (mm_priority) {
    if (!cJSON_IsString(mm_priority)) {
        ogs_error("OpenAPI_mms_charging_information_parseFromJSON() failed [mm_priority]");
        goto end;
    }
    mm_priorityVariable = OpenAPI_sm_priority_FromString(mm_priority->valuestring);
    }

    message_id = cJSON_GetObjectItemCaseSensitive(mms_charging_informationJSON, "messageID");
    if (message_id) {
    if (!cJSON_IsString(message_id) && !cJSON_IsNull(message_id)) {
        ogs_error("OpenAPI_mms_charging_information_parseFromJSON() failed [message_id]");
        goto end;
    }
    }

    message_type = cJSON_GetObjectItemCaseSensitive(mms_charging_informationJSON, "messageType");
    if (message_type) {
    if (!cJSON_IsString(message_type) && !cJSON_IsNull(message_type)) {
        ogs_error("OpenAPI_mms_charging_information_parseFromJSON() failed [message_type]");
        goto end;
    }
    }

    message_size = cJSON_GetObjectItemCaseSensitive(mms_charging_informationJSON, "messageSize");
    if (message_size) {
    if (!cJSON_IsNumber(message_size)) {
        ogs_error("OpenAPI_mms_charging_information_parseFromJSON() failed [message_size]");
        goto end;
    }
    }

    message_class = cJSON_GetObjectItemCaseSensitive(mms_charging_informationJSON, "messageClass");
    if (message_class) {
    if (!cJSON_IsString(message_class) && !cJSON_IsNull(message_class)) {
        ogs_error("OpenAPI_mms_charging_information_parseFromJSON() failed [message_class]");
        goto end;
    }
    }

    delivery_report_requested = cJSON_GetObjectItemCaseSensitive(mms_charging_informationJSON, "deliveryReportRequested");
    if (delivery_report_requested) {
    if (!cJSON_IsBool(delivery_report_requested)) {
        ogs_error("OpenAPI_mms_charging_information_parseFromJSON() failed [delivery_report_requested]");
        goto end;
    }
    }

    read_reply_report_requested = cJSON_GetObjectItemCaseSensitive(mms_charging_informationJSON, "readReplyReportRequested");
    if (read_reply_report_requested) {
    if (!cJSON_IsBool(read_reply_report_requested)) {
        ogs_error("OpenAPI_mms_charging_information_parseFromJSON() failed [read_reply_report_requested]");
        goto end;
    }
    }

    applic_id = cJSON_GetObjectItemCaseSensitive(mms_charging_informationJSON, "applicID");
    if (applic_id) {
    if (!cJSON_IsString(applic_id) && !cJSON_IsNull(applic_id)) {
        ogs_error("OpenAPI_mms_charging_information_parseFromJSON() failed [applic_id]");
        goto end;
    }
    }

    reply_applic_id = cJSON_GetObjectItemCaseSensitive(mms_charging_informationJSON, "replyApplicID");
    if (reply_applic_id) {
    if (!cJSON_IsString(reply_applic_id) && !cJSON_IsNull(reply_applic_id)) {
        ogs_error("OpenAPI_mms_charging_information_parseFromJSON() failed [reply_applic_id]");
        goto end;
    }
    }

    aux_applic_info = cJSON_GetObjectItemCaseSensitive(mms_charging_informationJSON, "auxApplicInfo");
    if (aux_applic_info) {
    if (!cJSON_IsString(aux_applic_info) && !cJSON_IsNull(aux_applic_info)) {
        ogs_error("OpenAPI_mms_charging_information_parseFromJSON() failed [aux_applic_info]");
        goto end;
    }
    }

    content_class = cJSON_GetObjectItemCaseSensitive(mms_charging_informationJSON, "contentClass");
    if (content_class) {
    if (!cJSON_IsString(content_class) && !cJSON_IsNull(content_class)) {
        ogs_error("OpenAPI_mms_charging_information_parseFromJSON() failed [content_class]");
        goto end;
    }
    }

    d_rm_content = cJSON_GetObjectItemCaseSensitive(mms_charging_informationJSON, "dRMContent");
    if (d_rm_content) {
    if (!cJSON_IsBool(d_rm_content)) {
        ogs_error("OpenAPI_mms_charging_information_parseFromJSON() failed [d_rm_content]");
        goto end;
    }
    }

    adaptations = cJSON_GetObjectItemCaseSensitive(mms_charging_informationJSON, "adaptations");
    if (adaptations) {
    if (!cJSON_IsBool(adaptations)) {
        ogs_error("OpenAPI_mms_charging_information_parseFromJSON() failed [adaptations]");
        goto end;
    }
    }

    vas_id = cJSON_GetObjectItemCaseSensitive(mms_charging_informationJSON, "vasID");
    if (vas_id) {
    if (!cJSON_IsString(vas_id) && !cJSON_IsNull(vas_id)) {
        ogs_error("OpenAPI_mms_charging_information_parseFromJSON() failed [vas_id]");
        goto end;
    }
    }

    vasp_id = cJSON_GetObjectItemCaseSensitive(mms_charging_informationJSON, "vaspID");
    if (vasp_id) {
    if (!cJSON_IsString(vasp_id) && !cJSON_IsNull(vasp_id)) {
        ogs_error("OpenAPI_mms_charging_information_parseFromJSON() failed [vasp_id]");
        goto end;
    }
    }

    mms_charging_information_local_var = OpenAPI_mms_charging_information_create (
        mm_originator_info ? mm_originator_info_local_nonprim : NULL,
        mm_recipient_info_list ? mm_recipient_info_listList : NULL,
        user_locationinfo ? user_locationinfo_local_nonprim : NULL,
        uetime_zone && !cJSON_IsNull(uetime_zone) ? ogs_strdup(uetime_zone->valuestring) : NULL,
        r_at_type ? r_at_typeVariable : 0,
        correlation_information && !cJSON_IsNull(correlation_information) ? ogs_strdup(correlation_information->valuestring) : NULL,
        submission_time && !cJSON_IsNull(submission_time) ? ogs_strdup(submission_time->valuestring) : NULL,
        mm_content_type ? mm_content_type_local_nonprim : NULL,
        mm_priority ? mm_priorityVariable : 0,
        message_id && !cJSON_IsNull(message_id) ? ogs_strdup(message_id->valuestring) : NULL,
        message_type && !cJSON_IsNull(message_type) ? ogs_strdup(message_type->valuestring) : NULL,
        message_size ? true : false,
        message_size ? message_size->valuedouble : 0,
        message_class && !cJSON_IsNull(message_class) ? ogs_strdup(message_class->valuestring) : NULL,
        delivery_report_requested ? true : false,
        delivery_report_requested ? delivery_report_requested->valueint : 0,
        read_reply_report_requested ? true : false,
        read_reply_report_requested ? read_reply_report_requested->valueint : 0,
        applic_id && !cJSON_IsNull(applic_id) ? ogs_strdup(applic_id->valuestring) : NULL,
        reply_applic_id && !cJSON_IsNull(reply_applic_id) ? ogs_strdup(reply_applic_id->valuestring) : NULL,
        aux_applic_info && !cJSON_IsNull(aux_applic_info) ? ogs_strdup(aux_applic_info->valuestring) : NULL,
        content_class && !cJSON_IsNull(content_class) ? ogs_strdup(content_class->valuestring) : NULL,
        d_rm_content ? true : false,
        d_rm_content ? d_rm_content->valueint : 0,
        adaptations ? true : false,
        adaptations ? adaptations->valueint : 0,
        vas_id && !cJSON_IsNull(vas_id) ? ogs_strdup(vas_id->valuestring) : NULL,
        vasp_id && !cJSON_IsNull(vasp_id) ? ogs_strdup(vasp_id->valuestring) : NULL
    );

    return mms_charging_information_local_var;
end:
    if (mm_originator_info_local_nonprim) {
        OpenAPI_mm_originator_info_free(mm_originator_info_local_nonprim);
        mm_originator_info_local_nonprim = NULL;
    }
    if (mm_recipient_info_listList) {
        OpenAPI_list_for_each(mm_recipient_info_listList, node) {
            OpenAPI_mm_recipient_info_free(node->data);
        }
        OpenAPI_list_free(mm_recipient_info_listList);
        mm_recipient_info_listList = NULL;
    }
    if (user_locationinfo_local_nonprim) {
        OpenAPI_charging_user_location_free(user_locationinfo_local_nonprim);
        user_locationinfo_local_nonprim = NULL;
    }
    if (mm_content_type_local_nonprim) {
        OpenAPI_mm_content_type_free(mm_content_type_local_nonprim);
        mm_content_type_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_mms_charging_information_t *OpenAPI_mms_charging_information_copy(OpenAPI_mms_charging_information_t *dst, OpenAPI_mms_charging_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mms_charging_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mms_charging_information_convertToJSON() failed");
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

    OpenAPI_mms_charging_information_free(dst);
    dst = OpenAPI_mms_charging_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

