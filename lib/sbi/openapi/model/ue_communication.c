
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_communication.h"

OpenAPI_ue_communication_t *OpenAPI_ue_communication_create(
    bool is_comm_dur,
    int comm_dur,
    bool is_comm_dur_variance,
    float comm_dur_variance,
    bool is_perio_time,
    int perio_time,
    bool is_perio_time_variance,
    float perio_time_variance,
    char *ts,
    bool is_ts_variance,
    float ts_variance,
    OpenAPI_scheduled_communication_time_1_t *recurring_time,
    OpenAPI_traffic_characterization_t *traf_char,
    bool is_ratio,
    int ratio,
    bool is_perio_comm_ind,
    int perio_comm_ind,
    bool is_confidence,
    int confidence,
    OpenAPI_app_list_for_ue_comm_t *ana_of_app_list,
    OpenAPI_sess_inact_timer_for_ue_comm_t *sess_inact_timer
)
{
    OpenAPI_ue_communication_t *ue_communication_local_var = ogs_malloc(sizeof(OpenAPI_ue_communication_t));
    ogs_assert(ue_communication_local_var);

    ue_communication_local_var->is_comm_dur = is_comm_dur;
    ue_communication_local_var->comm_dur = comm_dur;
    ue_communication_local_var->is_comm_dur_variance = is_comm_dur_variance;
    ue_communication_local_var->comm_dur_variance = comm_dur_variance;
    ue_communication_local_var->is_perio_time = is_perio_time;
    ue_communication_local_var->perio_time = perio_time;
    ue_communication_local_var->is_perio_time_variance = is_perio_time_variance;
    ue_communication_local_var->perio_time_variance = perio_time_variance;
    ue_communication_local_var->ts = ts;
    ue_communication_local_var->is_ts_variance = is_ts_variance;
    ue_communication_local_var->ts_variance = ts_variance;
    ue_communication_local_var->recurring_time = recurring_time;
    ue_communication_local_var->traf_char = traf_char;
    ue_communication_local_var->is_ratio = is_ratio;
    ue_communication_local_var->ratio = ratio;
    ue_communication_local_var->is_perio_comm_ind = is_perio_comm_ind;
    ue_communication_local_var->perio_comm_ind = perio_comm_ind;
    ue_communication_local_var->is_confidence = is_confidence;
    ue_communication_local_var->confidence = confidence;
    ue_communication_local_var->ana_of_app_list = ana_of_app_list;
    ue_communication_local_var->sess_inact_timer = sess_inact_timer;

    return ue_communication_local_var;
}

void OpenAPI_ue_communication_free(OpenAPI_ue_communication_t *ue_communication)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_communication) {
        return;
    }
    if (ue_communication->ts) {
        ogs_free(ue_communication->ts);
        ue_communication->ts = NULL;
    }
    if (ue_communication->recurring_time) {
        OpenAPI_scheduled_communication_time_1_free(ue_communication->recurring_time);
        ue_communication->recurring_time = NULL;
    }
    if (ue_communication->traf_char) {
        OpenAPI_traffic_characterization_free(ue_communication->traf_char);
        ue_communication->traf_char = NULL;
    }
    if (ue_communication->ana_of_app_list) {
        OpenAPI_app_list_for_ue_comm_free(ue_communication->ana_of_app_list);
        ue_communication->ana_of_app_list = NULL;
    }
    if (ue_communication->sess_inact_timer) {
        OpenAPI_sess_inact_timer_for_ue_comm_free(ue_communication->sess_inact_timer);
        ue_communication->sess_inact_timer = NULL;
    }
    ogs_free(ue_communication);
}

cJSON *OpenAPI_ue_communication_convertToJSON(OpenAPI_ue_communication_t *ue_communication)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ue_communication == NULL) {
        ogs_error("OpenAPI_ue_communication_convertToJSON() failed [UeCommunication]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ue_communication->is_comm_dur) {
    if (cJSON_AddNumberToObject(item, "commDur", ue_communication->comm_dur) == NULL) {
        ogs_error("OpenAPI_ue_communication_convertToJSON() failed [comm_dur]");
        goto end;
    }
    }

    if (ue_communication->is_comm_dur_variance) {
    if (cJSON_AddNumberToObject(item, "commDurVariance", ue_communication->comm_dur_variance) == NULL) {
        ogs_error("OpenAPI_ue_communication_convertToJSON() failed [comm_dur_variance]");
        goto end;
    }
    }

    if (ue_communication->is_perio_time) {
    if (cJSON_AddNumberToObject(item, "perioTime", ue_communication->perio_time) == NULL) {
        ogs_error("OpenAPI_ue_communication_convertToJSON() failed [perio_time]");
        goto end;
    }
    }

    if (ue_communication->is_perio_time_variance) {
    if (cJSON_AddNumberToObject(item, "perioTimeVariance", ue_communication->perio_time_variance) == NULL) {
        ogs_error("OpenAPI_ue_communication_convertToJSON() failed [perio_time_variance]");
        goto end;
    }
    }

    if (ue_communication->ts) {
    if (cJSON_AddStringToObject(item, "ts", ue_communication->ts) == NULL) {
        ogs_error("OpenAPI_ue_communication_convertToJSON() failed [ts]");
        goto end;
    }
    }

    if (ue_communication->is_ts_variance) {
    if (cJSON_AddNumberToObject(item, "tsVariance", ue_communication->ts_variance) == NULL) {
        ogs_error("OpenAPI_ue_communication_convertToJSON() failed [ts_variance]");
        goto end;
    }
    }

    if (ue_communication->recurring_time) {
    cJSON *recurring_time_local_JSON = OpenAPI_scheduled_communication_time_1_convertToJSON(ue_communication->recurring_time);
    if (recurring_time_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_communication_convertToJSON() failed [recurring_time]");
        goto end;
    }
    cJSON_AddItemToObject(item, "recurringTime", recurring_time_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_communication_convertToJSON() failed [recurring_time]");
        goto end;
    }
    }

    if (ue_communication->traf_char) {
    cJSON *traf_char_local_JSON = OpenAPI_traffic_characterization_convertToJSON(ue_communication->traf_char);
    if (traf_char_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_communication_convertToJSON() failed [traf_char]");
        goto end;
    }
    cJSON_AddItemToObject(item, "trafChar", traf_char_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_communication_convertToJSON() failed [traf_char]");
        goto end;
    }
    }

    if (ue_communication->is_ratio) {
    if (cJSON_AddNumberToObject(item, "ratio", ue_communication->ratio) == NULL) {
        ogs_error("OpenAPI_ue_communication_convertToJSON() failed [ratio]");
        goto end;
    }
    }

    if (ue_communication->is_perio_comm_ind) {
    if (cJSON_AddBoolToObject(item, "perioCommInd", ue_communication->perio_comm_ind) == NULL) {
        ogs_error("OpenAPI_ue_communication_convertToJSON() failed [perio_comm_ind]");
        goto end;
    }
    }

    if (ue_communication->is_confidence) {
    if (cJSON_AddNumberToObject(item, "confidence", ue_communication->confidence) == NULL) {
        ogs_error("OpenAPI_ue_communication_convertToJSON() failed [confidence]");
        goto end;
    }
    }

    if (ue_communication->ana_of_app_list) {
    cJSON *ana_of_app_list_local_JSON = OpenAPI_app_list_for_ue_comm_convertToJSON(ue_communication->ana_of_app_list);
    if (ana_of_app_list_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_communication_convertToJSON() failed [ana_of_app_list]");
        goto end;
    }
    cJSON_AddItemToObject(item, "anaOfAppList", ana_of_app_list_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_communication_convertToJSON() failed [ana_of_app_list]");
        goto end;
    }
    }

    if (ue_communication->sess_inact_timer) {
    cJSON *sess_inact_timer_local_JSON = OpenAPI_sess_inact_timer_for_ue_comm_convertToJSON(ue_communication->sess_inact_timer);
    if (sess_inact_timer_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_communication_convertToJSON() failed [sess_inact_timer]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sessInactTimer", sess_inact_timer_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_communication_convertToJSON() failed [sess_inact_timer]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ue_communication_t *OpenAPI_ue_communication_parseFromJSON(cJSON *ue_communicationJSON)
{
    OpenAPI_ue_communication_t *ue_communication_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *comm_dur = NULL;
    cJSON *comm_dur_variance = NULL;
    cJSON *perio_time = NULL;
    cJSON *perio_time_variance = NULL;
    cJSON *ts = NULL;
    cJSON *ts_variance = NULL;
    cJSON *recurring_time = NULL;
    OpenAPI_scheduled_communication_time_1_t *recurring_time_local_nonprim = NULL;
    cJSON *traf_char = NULL;
    OpenAPI_traffic_characterization_t *traf_char_local_nonprim = NULL;
    cJSON *ratio = NULL;
    cJSON *perio_comm_ind = NULL;
    cJSON *confidence = NULL;
    cJSON *ana_of_app_list = NULL;
    OpenAPI_app_list_for_ue_comm_t *ana_of_app_list_local_nonprim = NULL;
    cJSON *sess_inact_timer = NULL;
    OpenAPI_sess_inact_timer_for_ue_comm_t *sess_inact_timer_local_nonprim = NULL;
    comm_dur = cJSON_GetObjectItemCaseSensitive(ue_communicationJSON, "commDur");
    if (comm_dur) {
    if (!cJSON_IsNumber(comm_dur)) {
        ogs_error("OpenAPI_ue_communication_parseFromJSON() failed [comm_dur]");
        goto end;
    }
    }

    comm_dur_variance = cJSON_GetObjectItemCaseSensitive(ue_communicationJSON, "commDurVariance");
    if (comm_dur_variance) {
    if (!cJSON_IsNumber(comm_dur_variance)) {
        ogs_error("OpenAPI_ue_communication_parseFromJSON() failed [comm_dur_variance]");
        goto end;
    }
    }

    perio_time = cJSON_GetObjectItemCaseSensitive(ue_communicationJSON, "perioTime");
    if (perio_time) {
    if (!cJSON_IsNumber(perio_time)) {
        ogs_error("OpenAPI_ue_communication_parseFromJSON() failed [perio_time]");
        goto end;
    }
    }

    perio_time_variance = cJSON_GetObjectItemCaseSensitive(ue_communicationJSON, "perioTimeVariance");
    if (perio_time_variance) {
    if (!cJSON_IsNumber(perio_time_variance)) {
        ogs_error("OpenAPI_ue_communication_parseFromJSON() failed [perio_time_variance]");
        goto end;
    }
    }

    ts = cJSON_GetObjectItemCaseSensitive(ue_communicationJSON, "ts");
    if (ts) {
    if (!cJSON_IsString(ts) && !cJSON_IsNull(ts)) {
        ogs_error("OpenAPI_ue_communication_parseFromJSON() failed [ts]");
        goto end;
    }
    }

    ts_variance = cJSON_GetObjectItemCaseSensitive(ue_communicationJSON, "tsVariance");
    if (ts_variance) {
    if (!cJSON_IsNumber(ts_variance)) {
        ogs_error("OpenAPI_ue_communication_parseFromJSON() failed [ts_variance]");
        goto end;
    }
    }

    recurring_time = cJSON_GetObjectItemCaseSensitive(ue_communicationJSON, "recurringTime");
    if (recurring_time) {
    recurring_time_local_nonprim = OpenAPI_scheduled_communication_time_1_parseFromJSON(recurring_time);
    if (!recurring_time_local_nonprim) {
        ogs_error("OpenAPI_scheduled_communication_time_1_parseFromJSON failed [recurring_time]");
        goto end;
    }
    }

    traf_char = cJSON_GetObjectItemCaseSensitive(ue_communicationJSON, "trafChar");
    if (traf_char) {
    traf_char_local_nonprim = OpenAPI_traffic_characterization_parseFromJSON(traf_char);
    if (!traf_char_local_nonprim) {
        ogs_error("OpenAPI_traffic_characterization_parseFromJSON failed [traf_char]");
        goto end;
    }
    }

    ratio = cJSON_GetObjectItemCaseSensitive(ue_communicationJSON, "ratio");
    if (ratio) {
    if (!cJSON_IsNumber(ratio)) {
        ogs_error("OpenAPI_ue_communication_parseFromJSON() failed [ratio]");
        goto end;
    }
    }

    perio_comm_ind = cJSON_GetObjectItemCaseSensitive(ue_communicationJSON, "perioCommInd");
    if (perio_comm_ind) {
    if (!cJSON_IsBool(perio_comm_ind)) {
        ogs_error("OpenAPI_ue_communication_parseFromJSON() failed [perio_comm_ind]");
        goto end;
    }
    }

    confidence = cJSON_GetObjectItemCaseSensitive(ue_communicationJSON, "confidence");
    if (confidence) {
    if (!cJSON_IsNumber(confidence)) {
        ogs_error("OpenAPI_ue_communication_parseFromJSON() failed [confidence]");
        goto end;
    }
    }

    ana_of_app_list = cJSON_GetObjectItemCaseSensitive(ue_communicationJSON, "anaOfAppList");
    if (ana_of_app_list) {
    ana_of_app_list_local_nonprim = OpenAPI_app_list_for_ue_comm_parseFromJSON(ana_of_app_list);
    if (!ana_of_app_list_local_nonprim) {
        ogs_error("OpenAPI_app_list_for_ue_comm_parseFromJSON failed [ana_of_app_list]");
        goto end;
    }
    }

    sess_inact_timer = cJSON_GetObjectItemCaseSensitive(ue_communicationJSON, "sessInactTimer");
    if (sess_inact_timer) {
    sess_inact_timer_local_nonprim = OpenAPI_sess_inact_timer_for_ue_comm_parseFromJSON(sess_inact_timer);
    if (!sess_inact_timer_local_nonprim) {
        ogs_error("OpenAPI_sess_inact_timer_for_ue_comm_parseFromJSON failed [sess_inact_timer]");
        goto end;
    }
    }

    ue_communication_local_var = OpenAPI_ue_communication_create (
        comm_dur ? true : false,
        comm_dur ? comm_dur->valuedouble : 0,
        comm_dur_variance ? true : false,
        comm_dur_variance ? comm_dur_variance->valuedouble : 0,
        perio_time ? true : false,
        perio_time ? perio_time->valuedouble : 0,
        perio_time_variance ? true : false,
        perio_time_variance ? perio_time_variance->valuedouble : 0,
        ts && !cJSON_IsNull(ts) ? ogs_strdup(ts->valuestring) : NULL,
        ts_variance ? true : false,
        ts_variance ? ts_variance->valuedouble : 0,
        recurring_time ? recurring_time_local_nonprim : NULL,
        traf_char ? traf_char_local_nonprim : NULL,
        ratio ? true : false,
        ratio ? ratio->valuedouble : 0,
        perio_comm_ind ? true : false,
        perio_comm_ind ? perio_comm_ind->valueint : 0,
        confidence ? true : false,
        confidence ? confidence->valuedouble : 0,
        ana_of_app_list ? ana_of_app_list_local_nonprim : NULL,
        sess_inact_timer ? sess_inact_timer_local_nonprim : NULL
    );

    return ue_communication_local_var;
end:
    if (recurring_time_local_nonprim) {
        OpenAPI_scheduled_communication_time_1_free(recurring_time_local_nonprim);
        recurring_time_local_nonprim = NULL;
    }
    if (traf_char_local_nonprim) {
        OpenAPI_traffic_characterization_free(traf_char_local_nonprim);
        traf_char_local_nonprim = NULL;
    }
    if (ana_of_app_list_local_nonprim) {
        OpenAPI_app_list_for_ue_comm_free(ana_of_app_list_local_nonprim);
        ana_of_app_list_local_nonprim = NULL;
    }
    if (sess_inact_timer_local_nonprim) {
        OpenAPI_sess_inact_timer_for_ue_comm_free(sess_inact_timer_local_nonprim);
        sess_inact_timer_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_ue_communication_t *OpenAPI_ue_communication_copy(OpenAPI_ue_communication_t *dst, OpenAPI_ue_communication_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_communication_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_communication_convertToJSON() failed");
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

    OpenAPI_ue_communication_free(dst);
    dst = OpenAPI_ue_communication_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

