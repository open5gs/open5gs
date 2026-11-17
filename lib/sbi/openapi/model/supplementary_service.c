
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "supplementary_service.h"

OpenAPI_supplementary_service_t *OpenAPI_supplementary_service_create(
    OpenAPI_supplementary_service_type_e supplementary_service_type,
    OpenAPI_supplementary_service_mode_e supplementary_service_mode,
    bool is_number_of_diversions,
    int number_of_diversions,
    char *associated_party_address,
    char *conference_id,
    OpenAPI_participant_action_type_e participant_action_type,
    char *change_time,
    bool is_number_of_participants,
    int number_of_participants,
    char *c_ug_information
)
{
    OpenAPI_supplementary_service_t *supplementary_service_local_var = ogs_malloc(sizeof(OpenAPI_supplementary_service_t));
    ogs_assert(supplementary_service_local_var);

    supplementary_service_local_var->supplementary_service_type = supplementary_service_type;
    supplementary_service_local_var->supplementary_service_mode = supplementary_service_mode;
    supplementary_service_local_var->is_number_of_diversions = is_number_of_diversions;
    supplementary_service_local_var->number_of_diversions = number_of_diversions;
    supplementary_service_local_var->associated_party_address = associated_party_address;
    supplementary_service_local_var->conference_id = conference_id;
    supplementary_service_local_var->participant_action_type = participant_action_type;
    supplementary_service_local_var->change_time = change_time;
    supplementary_service_local_var->is_number_of_participants = is_number_of_participants;
    supplementary_service_local_var->number_of_participants = number_of_participants;
    supplementary_service_local_var->c_ug_information = c_ug_information;

    return supplementary_service_local_var;
}

void OpenAPI_supplementary_service_free(OpenAPI_supplementary_service_t *supplementary_service)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == supplementary_service) {
        return;
    }
    if (supplementary_service->associated_party_address) {
        ogs_free(supplementary_service->associated_party_address);
        supplementary_service->associated_party_address = NULL;
    }
    if (supplementary_service->conference_id) {
        ogs_free(supplementary_service->conference_id);
        supplementary_service->conference_id = NULL;
    }
    if (supplementary_service->change_time) {
        ogs_free(supplementary_service->change_time);
        supplementary_service->change_time = NULL;
    }
    if (supplementary_service->c_ug_information) {
        ogs_free(supplementary_service->c_ug_information);
        supplementary_service->c_ug_information = NULL;
    }
    ogs_free(supplementary_service);
}

cJSON *OpenAPI_supplementary_service_convertToJSON(OpenAPI_supplementary_service_t *supplementary_service)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (supplementary_service == NULL) {
        ogs_error("OpenAPI_supplementary_service_convertToJSON() failed [SupplementaryService]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (supplementary_service->supplementary_service_type != OpenAPI_supplementary_service_type_NULL) {
    if (cJSON_AddStringToObject(item, "supplementaryServiceType", OpenAPI_supplementary_service_type_ToString(supplementary_service->supplementary_service_type)) == NULL) {
        ogs_error("OpenAPI_supplementary_service_convertToJSON() failed [supplementary_service_type]");
        goto end;
    }
    }

    if (supplementary_service->supplementary_service_mode != OpenAPI_supplementary_service_mode_NULL) {
    if (cJSON_AddStringToObject(item, "supplementaryServiceMode", OpenAPI_supplementary_service_mode_ToString(supplementary_service->supplementary_service_mode)) == NULL) {
        ogs_error("OpenAPI_supplementary_service_convertToJSON() failed [supplementary_service_mode]");
        goto end;
    }
    }

    if (supplementary_service->is_number_of_diversions) {
    if (cJSON_AddNumberToObject(item, "numberOfDiversions", supplementary_service->number_of_diversions) == NULL) {
        ogs_error("OpenAPI_supplementary_service_convertToJSON() failed [number_of_diversions]");
        goto end;
    }
    }

    if (supplementary_service->associated_party_address) {
    if (cJSON_AddStringToObject(item, "associatedPartyAddress", supplementary_service->associated_party_address) == NULL) {
        ogs_error("OpenAPI_supplementary_service_convertToJSON() failed [associated_party_address]");
        goto end;
    }
    }

    if (supplementary_service->conference_id) {
    if (cJSON_AddStringToObject(item, "conferenceId", supplementary_service->conference_id) == NULL) {
        ogs_error("OpenAPI_supplementary_service_convertToJSON() failed [conference_id]");
        goto end;
    }
    }

    if (supplementary_service->participant_action_type != OpenAPI_participant_action_type_NULL) {
    if (cJSON_AddStringToObject(item, "participantActionType", OpenAPI_participant_action_type_ToString(supplementary_service->participant_action_type)) == NULL) {
        ogs_error("OpenAPI_supplementary_service_convertToJSON() failed [participant_action_type]");
        goto end;
    }
    }

    if (supplementary_service->change_time) {
    if (cJSON_AddStringToObject(item, "changeTime", supplementary_service->change_time) == NULL) {
        ogs_error("OpenAPI_supplementary_service_convertToJSON() failed [change_time]");
        goto end;
    }
    }

    if (supplementary_service->is_number_of_participants) {
    if (cJSON_AddNumberToObject(item, "numberOfParticipants", supplementary_service->number_of_participants) == NULL) {
        ogs_error("OpenAPI_supplementary_service_convertToJSON() failed [number_of_participants]");
        goto end;
    }
    }

    if (supplementary_service->c_ug_information) {
    if (cJSON_AddStringToObject(item, "cUGInformation", supplementary_service->c_ug_information) == NULL) {
        ogs_error("OpenAPI_supplementary_service_convertToJSON() failed [c_ug_information]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_supplementary_service_t *OpenAPI_supplementary_service_parseFromJSON(cJSON *supplementary_serviceJSON)
{
    OpenAPI_supplementary_service_t *supplementary_service_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *supplementary_service_type = NULL;
    OpenAPI_supplementary_service_type_e supplementary_service_typeVariable = 0;
    cJSON *supplementary_service_mode = NULL;
    OpenAPI_supplementary_service_mode_e supplementary_service_modeVariable = 0;
    cJSON *number_of_diversions = NULL;
    cJSON *associated_party_address = NULL;
    cJSON *conference_id = NULL;
    cJSON *participant_action_type = NULL;
    OpenAPI_participant_action_type_e participant_action_typeVariable = 0;
    cJSON *change_time = NULL;
    cJSON *number_of_participants = NULL;
    cJSON *c_ug_information = NULL;
    supplementary_service_type = cJSON_GetObjectItemCaseSensitive(supplementary_serviceJSON, "supplementaryServiceType");
    if (supplementary_service_type) {
    if (!cJSON_IsString(supplementary_service_type)) {
        ogs_error("OpenAPI_supplementary_service_parseFromJSON() failed [supplementary_service_type]");
        goto end;
    }
    supplementary_service_typeVariable = OpenAPI_supplementary_service_type_FromString(supplementary_service_type->valuestring);
    }

    supplementary_service_mode = cJSON_GetObjectItemCaseSensitive(supplementary_serviceJSON, "supplementaryServiceMode");
    if (supplementary_service_mode) {
    if (!cJSON_IsString(supplementary_service_mode)) {
        ogs_error("OpenAPI_supplementary_service_parseFromJSON() failed [supplementary_service_mode]");
        goto end;
    }
    supplementary_service_modeVariable = OpenAPI_supplementary_service_mode_FromString(supplementary_service_mode->valuestring);
    }

    number_of_diversions = cJSON_GetObjectItemCaseSensitive(supplementary_serviceJSON, "numberOfDiversions");
    if (number_of_diversions) {
    if (!cJSON_IsNumber(number_of_diversions)) {
        ogs_error("OpenAPI_supplementary_service_parseFromJSON() failed [number_of_diversions]");
        goto end;
    }
    }

    associated_party_address = cJSON_GetObjectItemCaseSensitive(supplementary_serviceJSON, "associatedPartyAddress");
    if (associated_party_address) {
    if (!cJSON_IsString(associated_party_address) && !cJSON_IsNull(associated_party_address)) {
        ogs_error("OpenAPI_supplementary_service_parseFromJSON() failed [associated_party_address]");
        goto end;
    }
    }

    conference_id = cJSON_GetObjectItemCaseSensitive(supplementary_serviceJSON, "conferenceId");
    if (conference_id) {
    if (!cJSON_IsString(conference_id) && !cJSON_IsNull(conference_id)) {
        ogs_error("OpenAPI_supplementary_service_parseFromJSON() failed [conference_id]");
        goto end;
    }
    }

    participant_action_type = cJSON_GetObjectItemCaseSensitive(supplementary_serviceJSON, "participantActionType");
    if (participant_action_type) {
    if (!cJSON_IsString(participant_action_type)) {
        ogs_error("OpenAPI_supplementary_service_parseFromJSON() failed [participant_action_type]");
        goto end;
    }
    participant_action_typeVariable = OpenAPI_participant_action_type_FromString(participant_action_type->valuestring);
    }

    change_time = cJSON_GetObjectItemCaseSensitive(supplementary_serviceJSON, "changeTime");
    if (change_time) {
    if (!cJSON_IsString(change_time) && !cJSON_IsNull(change_time)) {
        ogs_error("OpenAPI_supplementary_service_parseFromJSON() failed [change_time]");
        goto end;
    }
    }

    number_of_participants = cJSON_GetObjectItemCaseSensitive(supplementary_serviceJSON, "numberOfParticipants");
    if (number_of_participants) {
    if (!cJSON_IsNumber(number_of_participants)) {
        ogs_error("OpenAPI_supplementary_service_parseFromJSON() failed [number_of_participants]");
        goto end;
    }
    }

    c_ug_information = cJSON_GetObjectItemCaseSensitive(supplementary_serviceJSON, "cUGInformation");
    if (c_ug_information) {
    if (!cJSON_IsString(c_ug_information) && !cJSON_IsNull(c_ug_information)) {
        ogs_error("OpenAPI_supplementary_service_parseFromJSON() failed [c_ug_information]");
        goto end;
    }
    }

    supplementary_service_local_var = OpenAPI_supplementary_service_create (
        supplementary_service_type ? supplementary_service_typeVariable : 0,
        supplementary_service_mode ? supplementary_service_modeVariable : 0,
        number_of_diversions ? true : false,
        number_of_diversions ? number_of_diversions->valuedouble : 0,
        associated_party_address && !cJSON_IsNull(associated_party_address) ? ogs_strdup(associated_party_address->valuestring) : NULL,
        conference_id && !cJSON_IsNull(conference_id) ? ogs_strdup(conference_id->valuestring) : NULL,
        participant_action_type ? participant_action_typeVariable : 0,
        change_time && !cJSON_IsNull(change_time) ? ogs_strdup(change_time->valuestring) : NULL,
        number_of_participants ? true : false,
        number_of_participants ? number_of_participants->valuedouble : 0,
        c_ug_information && !cJSON_IsNull(c_ug_information) ? ogs_strdup(c_ug_information->valuestring) : NULL
    );

    return supplementary_service_local_var;
end:
    return NULL;
}

OpenAPI_supplementary_service_t *OpenAPI_supplementary_service_copy(OpenAPI_supplementary_service_t *dst, OpenAPI_supplementary_service_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_supplementary_service_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_supplementary_service_convertToJSON() failed");
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

    OpenAPI_supplementary_service_free(dst);
    dst = OpenAPI_supplementary_service_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

