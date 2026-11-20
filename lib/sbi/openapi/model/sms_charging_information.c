
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sms_charging_information.h"

OpenAPI_sms_charging_information_t *OpenAPI_sms_charging_information_create(
    OpenAPI_originator_info_t *originator_info,
    OpenAPI_list_t *recipient_info,
    char *user_equipment_info,
    OpenAPI_roamer_in_out_e roamer_in_out,
    OpenAPI_charging_user_location_t *user_locationinfo,
    char *uetime_zone,
    OpenAPI_rat_type_e r_at_type,
    char *s_msc_address,
    bool is_s_m_data_coding_scheme,
    int s_m_data_coding_scheme,
    OpenAPI_sm_message_type_e s_m_message_type,
    OpenAPI_reply_path_requested_e s_m_reply_path_requested,
    char *s_m_user_data_header,
    char *s_m_status,
    char *s_m_discharge_time,
    bool is_numberof_messages_sent,
    int numberof_messages_sent,
    OpenAPI_sm_service_type_e s_m_service_type,
    bool is_s_m_sequence_number,
    int s_m_sequence_number,
    bool is_s_m_sresult,
    int s_m_sresult,
    char *submission_time,
    OpenAPI_sm_priority_e s_m_priority,
    char *message_reference,
    bool is_message_size,
    int message_size,
    OpenAPI_message_class_t *message_class,
    OpenAPI_delivery_report_requested_e delivery_report_requested
)
{
    OpenAPI_sms_charging_information_t *sms_charging_information_local_var = ogs_malloc(sizeof(OpenAPI_sms_charging_information_t));
    ogs_assert(sms_charging_information_local_var);

    sms_charging_information_local_var->originator_info = originator_info;
    sms_charging_information_local_var->recipient_info = recipient_info;
    sms_charging_information_local_var->user_equipment_info = user_equipment_info;
    sms_charging_information_local_var->roamer_in_out = roamer_in_out;
    sms_charging_information_local_var->user_locationinfo = user_locationinfo;
    sms_charging_information_local_var->uetime_zone = uetime_zone;
    sms_charging_information_local_var->r_at_type = r_at_type;
    sms_charging_information_local_var->s_msc_address = s_msc_address;
    sms_charging_information_local_var->is_s_m_data_coding_scheme = is_s_m_data_coding_scheme;
    sms_charging_information_local_var->s_m_data_coding_scheme = s_m_data_coding_scheme;
    sms_charging_information_local_var->s_m_message_type = s_m_message_type;
    sms_charging_information_local_var->s_m_reply_path_requested = s_m_reply_path_requested;
    sms_charging_information_local_var->s_m_user_data_header = s_m_user_data_header;
    sms_charging_information_local_var->s_m_status = s_m_status;
    sms_charging_information_local_var->s_m_discharge_time = s_m_discharge_time;
    sms_charging_information_local_var->is_numberof_messages_sent = is_numberof_messages_sent;
    sms_charging_information_local_var->numberof_messages_sent = numberof_messages_sent;
    sms_charging_information_local_var->s_m_service_type = s_m_service_type;
    sms_charging_information_local_var->is_s_m_sequence_number = is_s_m_sequence_number;
    sms_charging_information_local_var->s_m_sequence_number = s_m_sequence_number;
    sms_charging_information_local_var->is_s_m_sresult = is_s_m_sresult;
    sms_charging_information_local_var->s_m_sresult = s_m_sresult;
    sms_charging_information_local_var->submission_time = submission_time;
    sms_charging_information_local_var->s_m_priority = s_m_priority;
    sms_charging_information_local_var->message_reference = message_reference;
    sms_charging_information_local_var->is_message_size = is_message_size;
    sms_charging_information_local_var->message_size = message_size;
    sms_charging_information_local_var->message_class = message_class;
    sms_charging_information_local_var->delivery_report_requested = delivery_report_requested;

    return sms_charging_information_local_var;
}

void OpenAPI_sms_charging_information_free(OpenAPI_sms_charging_information_t *sms_charging_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sms_charging_information) {
        return;
    }
    if (sms_charging_information->originator_info) {
        OpenAPI_originator_info_free(sms_charging_information->originator_info);
        sms_charging_information->originator_info = NULL;
    }
    if (sms_charging_information->recipient_info) {
        OpenAPI_list_for_each(sms_charging_information->recipient_info, node) {
            OpenAPI_recipient_info_free(node->data);
        }
        OpenAPI_list_free(sms_charging_information->recipient_info);
        sms_charging_information->recipient_info = NULL;
    }
    if (sms_charging_information->user_equipment_info) {
        ogs_free(sms_charging_information->user_equipment_info);
        sms_charging_information->user_equipment_info = NULL;
    }
    if (sms_charging_information->user_locationinfo) {
        OpenAPI_charging_user_location_free(sms_charging_information->user_locationinfo);
        sms_charging_information->user_locationinfo = NULL;
    }
    if (sms_charging_information->uetime_zone) {
        ogs_free(sms_charging_information->uetime_zone);
        sms_charging_information->uetime_zone = NULL;
    }
    if (sms_charging_information->s_msc_address) {
        ogs_free(sms_charging_information->s_msc_address);
        sms_charging_information->s_msc_address = NULL;
    }
    if (sms_charging_information->s_m_user_data_header) {
        ogs_free(sms_charging_information->s_m_user_data_header);
        sms_charging_information->s_m_user_data_header = NULL;
    }
    if (sms_charging_information->s_m_status) {
        ogs_free(sms_charging_information->s_m_status);
        sms_charging_information->s_m_status = NULL;
    }
    if (sms_charging_information->s_m_discharge_time) {
        ogs_free(sms_charging_information->s_m_discharge_time);
        sms_charging_information->s_m_discharge_time = NULL;
    }
    if (sms_charging_information->submission_time) {
        ogs_free(sms_charging_information->submission_time);
        sms_charging_information->submission_time = NULL;
    }
    if (sms_charging_information->message_reference) {
        ogs_free(sms_charging_information->message_reference);
        sms_charging_information->message_reference = NULL;
    }
    if (sms_charging_information->message_class) {
        OpenAPI_message_class_free(sms_charging_information->message_class);
        sms_charging_information->message_class = NULL;
    }
    ogs_free(sms_charging_information);
}

cJSON *OpenAPI_sms_charging_information_convertToJSON(OpenAPI_sms_charging_information_t *sms_charging_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sms_charging_information == NULL) {
        ogs_error("OpenAPI_sms_charging_information_convertToJSON() failed [SMSChargingInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (sms_charging_information->originator_info) {
    cJSON *originator_info_local_JSON = OpenAPI_originator_info_convertToJSON(sms_charging_information->originator_info);
    if (originator_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sms_charging_information_convertToJSON() failed [originator_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "originatorInfo", originator_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sms_charging_information_convertToJSON() failed [originator_info]");
        goto end;
    }
    }

    if (sms_charging_information->recipient_info) {
    cJSON *recipient_infoList = cJSON_AddArrayToObject(item, "recipientInfo");
    if (recipient_infoList == NULL) {
        ogs_error("OpenAPI_sms_charging_information_convertToJSON() failed [recipient_info]");
        goto end;
    }
    OpenAPI_list_for_each(sms_charging_information->recipient_info, node) {
        cJSON *itemLocal = OpenAPI_recipient_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sms_charging_information_convertToJSON() failed [recipient_info]");
            goto end;
        }
        cJSON_AddItemToArray(recipient_infoList, itemLocal);
    }
    }

    if (sms_charging_information->user_equipment_info) {
    if (cJSON_AddStringToObject(item, "userEquipmentInfo", sms_charging_information->user_equipment_info) == NULL) {
        ogs_error("OpenAPI_sms_charging_information_convertToJSON() failed [user_equipment_info]");
        goto end;
    }
    }

    if (sms_charging_information->roamer_in_out != OpenAPI_roamer_in_out_NULL) {
    if (cJSON_AddStringToObject(item, "roamerInOut", OpenAPI_roamer_in_out_ToString(sms_charging_information->roamer_in_out)) == NULL) {
        ogs_error("OpenAPI_sms_charging_information_convertToJSON() failed [roamer_in_out]");
        goto end;
    }
    }

    if (sms_charging_information->user_locationinfo) {
    cJSON *user_locationinfo_local_JSON = OpenAPI_charging_user_location_convertToJSON(sms_charging_information->user_locationinfo);
    if (user_locationinfo_local_JSON == NULL) {
        ogs_error("OpenAPI_sms_charging_information_convertToJSON() failed [user_locationinfo]");
        goto end;
    }
    cJSON_AddItemToObject(item, "userLocationinfo", user_locationinfo_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sms_charging_information_convertToJSON() failed [user_locationinfo]");
        goto end;
    }
    }

    if (sms_charging_information->uetime_zone) {
    if (cJSON_AddStringToObject(item, "uetimeZone", sms_charging_information->uetime_zone) == NULL) {
        ogs_error("OpenAPI_sms_charging_information_convertToJSON() failed [uetime_zone]");
        goto end;
    }
    }

    if (sms_charging_information->r_at_type != OpenAPI_rat_type_NULL) {
    if (cJSON_AddStringToObject(item, "rATType", OpenAPI_rat_type_ToString(sms_charging_information->r_at_type)) == NULL) {
        ogs_error("OpenAPI_sms_charging_information_convertToJSON() failed [r_at_type]");
        goto end;
    }
    }

    if (sms_charging_information->s_msc_address) {
    if (cJSON_AddStringToObject(item, "sMSCAddress", sms_charging_information->s_msc_address) == NULL) {
        ogs_error("OpenAPI_sms_charging_information_convertToJSON() failed [s_msc_address]");
        goto end;
    }
    }

    if (sms_charging_information->is_s_m_data_coding_scheme) {
    if (cJSON_AddNumberToObject(item, "sMDataCodingScheme", sms_charging_information->s_m_data_coding_scheme) == NULL) {
        ogs_error("OpenAPI_sms_charging_information_convertToJSON() failed [s_m_data_coding_scheme]");
        goto end;
    }
    }

    if (sms_charging_information->s_m_message_type != OpenAPI_sm_message_type_NULL) {
    if (cJSON_AddStringToObject(item, "sMMessageType", OpenAPI_sm_message_type_ToString(sms_charging_information->s_m_message_type)) == NULL) {
        ogs_error("OpenAPI_sms_charging_information_convertToJSON() failed [s_m_message_type]");
        goto end;
    }
    }

    if (sms_charging_information->s_m_reply_path_requested != OpenAPI_reply_path_requested_NULL) {
    if (cJSON_AddStringToObject(item, "sMReplyPathRequested", OpenAPI_reply_path_requested_ToString(sms_charging_information->s_m_reply_path_requested)) == NULL) {
        ogs_error("OpenAPI_sms_charging_information_convertToJSON() failed [s_m_reply_path_requested]");
        goto end;
    }
    }

    if (sms_charging_information->s_m_user_data_header) {
    if (cJSON_AddStringToObject(item, "sMUserDataHeader", sms_charging_information->s_m_user_data_header) == NULL) {
        ogs_error("OpenAPI_sms_charging_information_convertToJSON() failed [s_m_user_data_header]");
        goto end;
    }
    }

    if (sms_charging_information->s_m_status) {
    if (cJSON_AddStringToObject(item, "sMStatus", sms_charging_information->s_m_status) == NULL) {
        ogs_error("OpenAPI_sms_charging_information_convertToJSON() failed [s_m_status]");
        goto end;
    }
    }

    if (sms_charging_information->s_m_discharge_time) {
    if (cJSON_AddStringToObject(item, "sMDischargeTime", sms_charging_information->s_m_discharge_time) == NULL) {
        ogs_error("OpenAPI_sms_charging_information_convertToJSON() failed [s_m_discharge_time]");
        goto end;
    }
    }

    if (sms_charging_information->is_numberof_messages_sent) {
    if (cJSON_AddNumberToObject(item, "numberofMessagesSent", sms_charging_information->numberof_messages_sent) == NULL) {
        ogs_error("OpenAPI_sms_charging_information_convertToJSON() failed [numberof_messages_sent]");
        goto end;
    }
    }

    if (sms_charging_information->s_m_service_type != OpenAPI_sm_service_type_NULL) {
    if (cJSON_AddStringToObject(item, "sMServiceType", OpenAPI_sm_service_type_ToString(sms_charging_information->s_m_service_type)) == NULL) {
        ogs_error("OpenAPI_sms_charging_information_convertToJSON() failed [s_m_service_type]");
        goto end;
    }
    }

    if (sms_charging_information->is_s_m_sequence_number) {
    if (cJSON_AddNumberToObject(item, "sMSequenceNumber", sms_charging_information->s_m_sequence_number) == NULL) {
        ogs_error("OpenAPI_sms_charging_information_convertToJSON() failed [s_m_sequence_number]");
        goto end;
    }
    }

    if (sms_charging_information->is_s_m_sresult) {
    if (cJSON_AddNumberToObject(item, "sMSresult", sms_charging_information->s_m_sresult) == NULL) {
        ogs_error("OpenAPI_sms_charging_information_convertToJSON() failed [s_m_sresult]");
        goto end;
    }
    }

    if (sms_charging_information->submission_time) {
    if (cJSON_AddStringToObject(item, "submissionTime", sms_charging_information->submission_time) == NULL) {
        ogs_error("OpenAPI_sms_charging_information_convertToJSON() failed [submission_time]");
        goto end;
    }
    }

    if (sms_charging_information->s_m_priority != OpenAPI_sm_priority_NULL) {
    if (cJSON_AddStringToObject(item, "sMPriority", OpenAPI_sm_priority_ToString(sms_charging_information->s_m_priority)) == NULL) {
        ogs_error("OpenAPI_sms_charging_information_convertToJSON() failed [s_m_priority]");
        goto end;
    }
    }

    if (sms_charging_information->message_reference) {
    if (cJSON_AddStringToObject(item, "messageReference", sms_charging_information->message_reference) == NULL) {
        ogs_error("OpenAPI_sms_charging_information_convertToJSON() failed [message_reference]");
        goto end;
    }
    }

    if (sms_charging_information->is_message_size) {
    if (cJSON_AddNumberToObject(item, "messageSize", sms_charging_information->message_size) == NULL) {
        ogs_error("OpenAPI_sms_charging_information_convertToJSON() failed [message_size]");
        goto end;
    }
    }

    if (sms_charging_information->message_class) {
    cJSON *message_class_local_JSON = OpenAPI_message_class_convertToJSON(sms_charging_information->message_class);
    if (message_class_local_JSON == NULL) {
        ogs_error("OpenAPI_sms_charging_information_convertToJSON() failed [message_class]");
        goto end;
    }
    cJSON_AddItemToObject(item, "messageClass", message_class_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sms_charging_information_convertToJSON() failed [message_class]");
        goto end;
    }
    }

    if (sms_charging_information->delivery_report_requested != OpenAPI_delivery_report_requested_NULL) {
    if (cJSON_AddStringToObject(item, "deliveryReportRequested", OpenAPI_delivery_report_requested_ToString(sms_charging_information->delivery_report_requested)) == NULL) {
        ogs_error("OpenAPI_sms_charging_information_convertToJSON() failed [delivery_report_requested]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sms_charging_information_t *OpenAPI_sms_charging_information_parseFromJSON(cJSON *sms_charging_informationJSON)
{
    OpenAPI_sms_charging_information_t *sms_charging_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *originator_info = NULL;
    OpenAPI_originator_info_t *originator_info_local_nonprim = NULL;
    cJSON *recipient_info = NULL;
    OpenAPI_list_t *recipient_infoList = NULL;
    cJSON *user_equipment_info = NULL;
    cJSON *roamer_in_out = NULL;
    OpenAPI_roamer_in_out_e roamer_in_outVariable = 0;
    cJSON *user_locationinfo = NULL;
    OpenAPI_charging_user_location_t *user_locationinfo_local_nonprim = NULL;
    cJSON *uetime_zone = NULL;
    cJSON *r_at_type = NULL;
    OpenAPI_rat_type_e r_at_typeVariable = 0;
    cJSON *s_msc_address = NULL;
    cJSON *s_m_data_coding_scheme = NULL;
    cJSON *s_m_message_type = NULL;
    OpenAPI_sm_message_type_e s_m_message_typeVariable = 0;
    cJSON *s_m_reply_path_requested = NULL;
    OpenAPI_reply_path_requested_e s_m_reply_path_requestedVariable = 0;
    cJSON *s_m_user_data_header = NULL;
    cJSON *s_m_status = NULL;
    cJSON *s_m_discharge_time = NULL;
    cJSON *numberof_messages_sent = NULL;
    cJSON *s_m_service_type = NULL;
    OpenAPI_sm_service_type_e s_m_service_typeVariable = 0;
    cJSON *s_m_sequence_number = NULL;
    cJSON *s_m_sresult = NULL;
    cJSON *submission_time = NULL;
    cJSON *s_m_priority = NULL;
    OpenAPI_sm_priority_e s_m_priorityVariable = 0;
    cJSON *message_reference = NULL;
    cJSON *message_size = NULL;
    cJSON *message_class = NULL;
    OpenAPI_message_class_t *message_class_local_nonprim = NULL;
    cJSON *delivery_report_requested = NULL;
    OpenAPI_delivery_report_requested_e delivery_report_requestedVariable = 0;
    originator_info = cJSON_GetObjectItemCaseSensitive(sms_charging_informationJSON, "originatorInfo");
    if (originator_info) {
    originator_info_local_nonprim = OpenAPI_originator_info_parseFromJSON(originator_info);
    if (!originator_info_local_nonprim) {
        ogs_error("OpenAPI_originator_info_parseFromJSON failed [originator_info]");
        goto end;
    }
    }

    recipient_info = cJSON_GetObjectItemCaseSensitive(sms_charging_informationJSON, "recipientInfo");
    if (recipient_info) {
        cJSON *recipient_info_local = NULL;
        if (!cJSON_IsArray(recipient_info)) {
            ogs_error("OpenAPI_sms_charging_information_parseFromJSON() failed [recipient_info]");
            goto end;
        }

        recipient_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(recipient_info_local, recipient_info) {
            if (!cJSON_IsObject(recipient_info_local)) {
                ogs_error("OpenAPI_sms_charging_information_parseFromJSON() failed [recipient_info]");
                goto end;
            }
            OpenAPI_recipient_info_t *recipient_infoItem = OpenAPI_recipient_info_parseFromJSON(recipient_info_local);
            if (!recipient_infoItem) {
                ogs_error("No recipient_infoItem");
                goto end;
            }
            OpenAPI_list_add(recipient_infoList, recipient_infoItem);
        }
    }

    user_equipment_info = cJSON_GetObjectItemCaseSensitive(sms_charging_informationJSON, "userEquipmentInfo");
    if (user_equipment_info) {
    if (!cJSON_IsString(user_equipment_info) && !cJSON_IsNull(user_equipment_info)) {
        ogs_error("OpenAPI_sms_charging_information_parseFromJSON() failed [user_equipment_info]");
        goto end;
    }
    }

    roamer_in_out = cJSON_GetObjectItemCaseSensitive(sms_charging_informationJSON, "roamerInOut");
    if (roamer_in_out) {
    if (!cJSON_IsString(roamer_in_out)) {
        ogs_error("OpenAPI_sms_charging_information_parseFromJSON() failed [roamer_in_out]");
        goto end;
    }
    roamer_in_outVariable = OpenAPI_roamer_in_out_FromString(roamer_in_out->valuestring);
    }

    user_locationinfo = cJSON_GetObjectItemCaseSensitive(sms_charging_informationJSON, "userLocationinfo");
    if (user_locationinfo) {
    user_locationinfo_local_nonprim = OpenAPI_charging_user_location_parseFromJSON(user_locationinfo);
    if (!user_locationinfo_local_nonprim) {
        ogs_error("OpenAPI_charging_user_location_parseFromJSON failed [user_locationinfo]");
        goto end;
    }
    }

    uetime_zone = cJSON_GetObjectItemCaseSensitive(sms_charging_informationJSON, "uetimeZone");
    if (uetime_zone) {
    if (!cJSON_IsString(uetime_zone) && !cJSON_IsNull(uetime_zone)) {
        ogs_error("OpenAPI_sms_charging_information_parseFromJSON() failed [uetime_zone]");
        goto end;
    }
    }

    r_at_type = cJSON_GetObjectItemCaseSensitive(sms_charging_informationJSON, "rATType");
    if (r_at_type) {
    if (!cJSON_IsString(r_at_type)) {
        ogs_error("OpenAPI_sms_charging_information_parseFromJSON() failed [r_at_type]");
        goto end;
    }
    r_at_typeVariable = OpenAPI_rat_type_FromString(r_at_type->valuestring);
    }

    s_msc_address = cJSON_GetObjectItemCaseSensitive(sms_charging_informationJSON, "sMSCAddress");
    if (s_msc_address) {
    if (!cJSON_IsString(s_msc_address) && !cJSON_IsNull(s_msc_address)) {
        ogs_error("OpenAPI_sms_charging_information_parseFromJSON() failed [s_msc_address]");
        goto end;
    }
    }

    s_m_data_coding_scheme = cJSON_GetObjectItemCaseSensitive(sms_charging_informationJSON, "sMDataCodingScheme");
    if (s_m_data_coding_scheme) {
    if (!cJSON_IsNumber(s_m_data_coding_scheme)) {
        ogs_error("OpenAPI_sms_charging_information_parseFromJSON() failed [s_m_data_coding_scheme]");
        goto end;
    }
    }

    s_m_message_type = cJSON_GetObjectItemCaseSensitive(sms_charging_informationJSON, "sMMessageType");
    if (s_m_message_type) {
    if (!cJSON_IsString(s_m_message_type)) {
        ogs_error("OpenAPI_sms_charging_information_parseFromJSON() failed [s_m_message_type]");
        goto end;
    }
    s_m_message_typeVariable = OpenAPI_sm_message_type_FromString(s_m_message_type->valuestring);
    }

    s_m_reply_path_requested = cJSON_GetObjectItemCaseSensitive(sms_charging_informationJSON, "sMReplyPathRequested");
    if (s_m_reply_path_requested) {
    if (!cJSON_IsString(s_m_reply_path_requested)) {
        ogs_error("OpenAPI_sms_charging_information_parseFromJSON() failed [s_m_reply_path_requested]");
        goto end;
    }
    s_m_reply_path_requestedVariable = OpenAPI_reply_path_requested_FromString(s_m_reply_path_requested->valuestring);
    }

    s_m_user_data_header = cJSON_GetObjectItemCaseSensitive(sms_charging_informationJSON, "sMUserDataHeader");
    if (s_m_user_data_header) {
    if (!cJSON_IsString(s_m_user_data_header) && !cJSON_IsNull(s_m_user_data_header)) {
        ogs_error("OpenAPI_sms_charging_information_parseFromJSON() failed [s_m_user_data_header]");
        goto end;
    }
    }

    s_m_status = cJSON_GetObjectItemCaseSensitive(sms_charging_informationJSON, "sMStatus");
    if (s_m_status) {
    if (!cJSON_IsString(s_m_status) && !cJSON_IsNull(s_m_status)) {
        ogs_error("OpenAPI_sms_charging_information_parseFromJSON() failed [s_m_status]");
        goto end;
    }
    }

    s_m_discharge_time = cJSON_GetObjectItemCaseSensitive(sms_charging_informationJSON, "sMDischargeTime");
    if (s_m_discharge_time) {
    if (!cJSON_IsString(s_m_discharge_time) && !cJSON_IsNull(s_m_discharge_time)) {
        ogs_error("OpenAPI_sms_charging_information_parseFromJSON() failed [s_m_discharge_time]");
        goto end;
    }
    }

    numberof_messages_sent = cJSON_GetObjectItemCaseSensitive(sms_charging_informationJSON, "numberofMessagesSent");
    if (numberof_messages_sent) {
    if (!cJSON_IsNumber(numberof_messages_sent)) {
        ogs_error("OpenAPI_sms_charging_information_parseFromJSON() failed [numberof_messages_sent]");
        goto end;
    }
    }

    s_m_service_type = cJSON_GetObjectItemCaseSensitive(sms_charging_informationJSON, "sMServiceType");
    if (s_m_service_type) {
    if (!cJSON_IsString(s_m_service_type)) {
        ogs_error("OpenAPI_sms_charging_information_parseFromJSON() failed [s_m_service_type]");
        goto end;
    }
    s_m_service_typeVariable = OpenAPI_sm_service_type_FromString(s_m_service_type->valuestring);
    }

    s_m_sequence_number = cJSON_GetObjectItemCaseSensitive(sms_charging_informationJSON, "sMSequenceNumber");
    if (s_m_sequence_number) {
    if (!cJSON_IsNumber(s_m_sequence_number)) {
        ogs_error("OpenAPI_sms_charging_information_parseFromJSON() failed [s_m_sequence_number]");
        goto end;
    }
    }

    s_m_sresult = cJSON_GetObjectItemCaseSensitive(sms_charging_informationJSON, "sMSresult");
    if (s_m_sresult) {
    if (!cJSON_IsNumber(s_m_sresult)) {
        ogs_error("OpenAPI_sms_charging_information_parseFromJSON() failed [s_m_sresult]");
        goto end;
    }
    }

    submission_time = cJSON_GetObjectItemCaseSensitive(sms_charging_informationJSON, "submissionTime");
    if (submission_time) {
    if (!cJSON_IsString(submission_time) && !cJSON_IsNull(submission_time)) {
        ogs_error("OpenAPI_sms_charging_information_parseFromJSON() failed [submission_time]");
        goto end;
    }
    }

    s_m_priority = cJSON_GetObjectItemCaseSensitive(sms_charging_informationJSON, "sMPriority");
    if (s_m_priority) {
    if (!cJSON_IsString(s_m_priority)) {
        ogs_error("OpenAPI_sms_charging_information_parseFromJSON() failed [s_m_priority]");
        goto end;
    }
    s_m_priorityVariable = OpenAPI_sm_priority_FromString(s_m_priority->valuestring);
    }

    message_reference = cJSON_GetObjectItemCaseSensitive(sms_charging_informationJSON, "messageReference");
    if (message_reference) {
    if (!cJSON_IsString(message_reference) && !cJSON_IsNull(message_reference)) {
        ogs_error("OpenAPI_sms_charging_information_parseFromJSON() failed [message_reference]");
        goto end;
    }
    }

    message_size = cJSON_GetObjectItemCaseSensitive(sms_charging_informationJSON, "messageSize");
    if (message_size) {
    if (!cJSON_IsNumber(message_size)) {
        ogs_error("OpenAPI_sms_charging_information_parseFromJSON() failed [message_size]");
        goto end;
    }
    }

    message_class = cJSON_GetObjectItemCaseSensitive(sms_charging_informationJSON, "messageClass");
    if (message_class) {
    message_class_local_nonprim = OpenAPI_message_class_parseFromJSON(message_class);
    if (!message_class_local_nonprim) {
        ogs_error("OpenAPI_message_class_parseFromJSON failed [message_class]");
        goto end;
    }
    }

    delivery_report_requested = cJSON_GetObjectItemCaseSensitive(sms_charging_informationJSON, "deliveryReportRequested");
    if (delivery_report_requested) {
    if (!cJSON_IsString(delivery_report_requested)) {
        ogs_error("OpenAPI_sms_charging_information_parseFromJSON() failed [delivery_report_requested]");
        goto end;
    }
    delivery_report_requestedVariable = OpenAPI_delivery_report_requested_FromString(delivery_report_requested->valuestring);
    }

    sms_charging_information_local_var = OpenAPI_sms_charging_information_create (
        originator_info ? originator_info_local_nonprim : NULL,
        recipient_info ? recipient_infoList : NULL,
        user_equipment_info && !cJSON_IsNull(user_equipment_info) ? ogs_strdup(user_equipment_info->valuestring) : NULL,
        roamer_in_out ? roamer_in_outVariable : 0,
        user_locationinfo ? user_locationinfo_local_nonprim : NULL,
        uetime_zone && !cJSON_IsNull(uetime_zone) ? ogs_strdup(uetime_zone->valuestring) : NULL,
        r_at_type ? r_at_typeVariable : 0,
        s_msc_address && !cJSON_IsNull(s_msc_address) ? ogs_strdup(s_msc_address->valuestring) : NULL,
        s_m_data_coding_scheme ? true : false,
        s_m_data_coding_scheme ? s_m_data_coding_scheme->valuedouble : 0,
        s_m_message_type ? s_m_message_typeVariable : 0,
        s_m_reply_path_requested ? s_m_reply_path_requestedVariable : 0,
        s_m_user_data_header && !cJSON_IsNull(s_m_user_data_header) ? ogs_strdup(s_m_user_data_header->valuestring) : NULL,
        s_m_status && !cJSON_IsNull(s_m_status) ? ogs_strdup(s_m_status->valuestring) : NULL,
        s_m_discharge_time && !cJSON_IsNull(s_m_discharge_time) ? ogs_strdup(s_m_discharge_time->valuestring) : NULL,
        numberof_messages_sent ? true : false,
        numberof_messages_sent ? numberof_messages_sent->valuedouble : 0,
        s_m_service_type ? s_m_service_typeVariable : 0,
        s_m_sequence_number ? true : false,
        s_m_sequence_number ? s_m_sequence_number->valuedouble : 0,
        s_m_sresult ? true : false,
        s_m_sresult ? s_m_sresult->valuedouble : 0,
        submission_time && !cJSON_IsNull(submission_time) ? ogs_strdup(submission_time->valuestring) : NULL,
        s_m_priority ? s_m_priorityVariable : 0,
        message_reference && !cJSON_IsNull(message_reference) ? ogs_strdup(message_reference->valuestring) : NULL,
        message_size ? true : false,
        message_size ? message_size->valuedouble : 0,
        message_class ? message_class_local_nonprim : NULL,
        delivery_report_requested ? delivery_report_requestedVariable : 0
    );

    return sms_charging_information_local_var;
end:
    if (originator_info_local_nonprim) {
        OpenAPI_originator_info_free(originator_info_local_nonprim);
        originator_info_local_nonprim = NULL;
    }
    if (recipient_infoList) {
        OpenAPI_list_for_each(recipient_infoList, node) {
            OpenAPI_recipient_info_free(node->data);
        }
        OpenAPI_list_free(recipient_infoList);
        recipient_infoList = NULL;
    }
    if (user_locationinfo_local_nonprim) {
        OpenAPI_charging_user_location_free(user_locationinfo_local_nonprim);
        user_locationinfo_local_nonprim = NULL;
    }
    if (message_class_local_nonprim) {
        OpenAPI_message_class_free(message_class_local_nonprim);
        message_class_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_sms_charging_information_t *OpenAPI_sms_charging_information_copy(OpenAPI_sms_charging_information_t *dst, OpenAPI_sms_charging_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sms_charging_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sms_charging_information_convertToJSON() failed");
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

    OpenAPI_sms_charging_information_free(dst);
    dst = OpenAPI_sms_charging_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

