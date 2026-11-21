
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "announcement_information.h"

OpenAPI_announcement_information_t *OpenAPI_announcement_information_create(
    bool is_announcement_identifier,
    int announcement_identifier,
    char *announcement_reference,
    OpenAPI_list_t *variable_parts,
    bool is_time_to_play,
    int time_to_play,
    OpenAPI_quota_consumption_indicator_e quota_consumption_indicator,
    bool is_announcement_priority,
    int announcement_priority,
    OpenAPI_play_to_party_e play_to_party,
    OpenAPI_announcement_privacy_indicator_e announcement_privacy_indicator,
    char *language
)
{
    OpenAPI_announcement_information_t *announcement_information_local_var = ogs_malloc(sizeof(OpenAPI_announcement_information_t));
    ogs_assert(announcement_information_local_var);

    announcement_information_local_var->is_announcement_identifier = is_announcement_identifier;
    announcement_information_local_var->announcement_identifier = announcement_identifier;
    announcement_information_local_var->announcement_reference = announcement_reference;
    announcement_information_local_var->variable_parts = variable_parts;
    announcement_information_local_var->is_time_to_play = is_time_to_play;
    announcement_information_local_var->time_to_play = time_to_play;
    announcement_information_local_var->quota_consumption_indicator = quota_consumption_indicator;
    announcement_information_local_var->is_announcement_priority = is_announcement_priority;
    announcement_information_local_var->announcement_priority = announcement_priority;
    announcement_information_local_var->play_to_party = play_to_party;
    announcement_information_local_var->announcement_privacy_indicator = announcement_privacy_indicator;
    announcement_information_local_var->language = language;

    return announcement_information_local_var;
}

void OpenAPI_announcement_information_free(OpenAPI_announcement_information_t *announcement_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == announcement_information) {
        return;
    }
    if (announcement_information->announcement_reference) {
        ogs_free(announcement_information->announcement_reference);
        announcement_information->announcement_reference = NULL;
    }
    if (announcement_information->variable_parts) {
        OpenAPI_list_for_each(announcement_information->variable_parts, node) {
            OpenAPI_variable_part_free(node->data);
        }
        OpenAPI_list_free(announcement_information->variable_parts);
        announcement_information->variable_parts = NULL;
    }
    if (announcement_information->language) {
        ogs_free(announcement_information->language);
        announcement_information->language = NULL;
    }
    ogs_free(announcement_information);
}

cJSON *OpenAPI_announcement_information_convertToJSON(OpenAPI_announcement_information_t *announcement_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (announcement_information == NULL) {
        ogs_error("OpenAPI_announcement_information_convertToJSON() failed [AnnouncementInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (announcement_information->is_announcement_identifier) {
    if (cJSON_AddNumberToObject(item, "announcementIdentifier", announcement_information->announcement_identifier) == NULL) {
        ogs_error("OpenAPI_announcement_information_convertToJSON() failed [announcement_identifier]");
        goto end;
    }
    }

    if (announcement_information->announcement_reference) {
    if (cJSON_AddStringToObject(item, "announcementReference", announcement_information->announcement_reference) == NULL) {
        ogs_error("OpenAPI_announcement_information_convertToJSON() failed [announcement_reference]");
        goto end;
    }
    }

    if (announcement_information->variable_parts) {
    cJSON *variable_partsList = cJSON_AddArrayToObject(item, "variableParts");
    if (variable_partsList == NULL) {
        ogs_error("OpenAPI_announcement_information_convertToJSON() failed [variable_parts]");
        goto end;
    }
    OpenAPI_list_for_each(announcement_information->variable_parts, node) {
        cJSON *itemLocal = OpenAPI_variable_part_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_announcement_information_convertToJSON() failed [variable_parts]");
            goto end;
        }
        cJSON_AddItemToArray(variable_partsList, itemLocal);
    }
    }

    if (announcement_information->is_time_to_play) {
    if (cJSON_AddNumberToObject(item, "timeToPlay", announcement_information->time_to_play) == NULL) {
        ogs_error("OpenAPI_announcement_information_convertToJSON() failed [time_to_play]");
        goto end;
    }
    }

    if (announcement_information->quota_consumption_indicator != OpenAPI_quota_consumption_indicator_NULL) {
    if (cJSON_AddStringToObject(item, "quotaConsumptionIndicator", OpenAPI_quota_consumption_indicator_ToString(announcement_information->quota_consumption_indicator)) == NULL) {
        ogs_error("OpenAPI_announcement_information_convertToJSON() failed [quota_consumption_indicator]");
        goto end;
    }
    }

    if (announcement_information->is_announcement_priority) {
    if (cJSON_AddNumberToObject(item, "announcementPriority", announcement_information->announcement_priority) == NULL) {
        ogs_error("OpenAPI_announcement_information_convertToJSON() failed [announcement_priority]");
        goto end;
    }
    }

    if (announcement_information->play_to_party != OpenAPI_play_to_party_NULL) {
    if (cJSON_AddStringToObject(item, "playToParty", OpenAPI_play_to_party_ToString(announcement_information->play_to_party)) == NULL) {
        ogs_error("OpenAPI_announcement_information_convertToJSON() failed [play_to_party]");
        goto end;
    }
    }

    if (announcement_information->announcement_privacy_indicator != OpenAPI_announcement_privacy_indicator_NULL) {
    if (cJSON_AddStringToObject(item, "announcementPrivacyIndicator", OpenAPI_announcement_privacy_indicator_ToString(announcement_information->announcement_privacy_indicator)) == NULL) {
        ogs_error("OpenAPI_announcement_information_convertToJSON() failed [announcement_privacy_indicator]");
        goto end;
    }
    }

    if (announcement_information->language) {
    if (cJSON_AddStringToObject(item, "Language", announcement_information->language) == NULL) {
        ogs_error("OpenAPI_announcement_information_convertToJSON() failed [language]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_announcement_information_t *OpenAPI_announcement_information_parseFromJSON(cJSON *announcement_informationJSON)
{
    OpenAPI_announcement_information_t *announcement_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *announcement_identifier = NULL;
    cJSON *announcement_reference = NULL;
    cJSON *variable_parts = NULL;
    OpenAPI_list_t *variable_partsList = NULL;
    cJSON *time_to_play = NULL;
    cJSON *quota_consumption_indicator = NULL;
    OpenAPI_quota_consumption_indicator_e quota_consumption_indicatorVariable = 0;
    cJSON *announcement_priority = NULL;
    cJSON *play_to_party = NULL;
    OpenAPI_play_to_party_e play_to_partyVariable = 0;
    cJSON *announcement_privacy_indicator = NULL;
    OpenAPI_announcement_privacy_indicator_e announcement_privacy_indicatorVariable = 0;
    cJSON *language = NULL;
    announcement_identifier = cJSON_GetObjectItemCaseSensitive(announcement_informationJSON, "announcementIdentifier");
    if (announcement_identifier) {
    if (!cJSON_IsNumber(announcement_identifier)) {
        ogs_error("OpenAPI_announcement_information_parseFromJSON() failed [announcement_identifier]");
        goto end;
    }
    }

    announcement_reference = cJSON_GetObjectItemCaseSensitive(announcement_informationJSON, "announcementReference");
    if (announcement_reference) {
    if (!cJSON_IsString(announcement_reference) && !cJSON_IsNull(announcement_reference)) {
        ogs_error("OpenAPI_announcement_information_parseFromJSON() failed [announcement_reference]");
        goto end;
    }
    }

    variable_parts = cJSON_GetObjectItemCaseSensitive(announcement_informationJSON, "variableParts");
    if (variable_parts) {
        cJSON *variable_parts_local = NULL;
        if (!cJSON_IsArray(variable_parts)) {
            ogs_error("OpenAPI_announcement_information_parseFromJSON() failed [variable_parts]");
            goto end;
        }

        variable_partsList = OpenAPI_list_create();

        cJSON_ArrayForEach(variable_parts_local, variable_parts) {
            if (!cJSON_IsObject(variable_parts_local)) {
                ogs_error("OpenAPI_announcement_information_parseFromJSON() failed [variable_parts]");
                goto end;
            }
            OpenAPI_variable_part_t *variable_partsItem = OpenAPI_variable_part_parseFromJSON(variable_parts_local);
            if (!variable_partsItem) {
                ogs_error("No variable_partsItem");
                goto end;
            }
            OpenAPI_list_add(variable_partsList, variable_partsItem);
        }
    }

    time_to_play = cJSON_GetObjectItemCaseSensitive(announcement_informationJSON, "timeToPlay");
    if (time_to_play) {
    if (!cJSON_IsNumber(time_to_play)) {
        ogs_error("OpenAPI_announcement_information_parseFromJSON() failed [time_to_play]");
        goto end;
    }
    }

    quota_consumption_indicator = cJSON_GetObjectItemCaseSensitive(announcement_informationJSON, "quotaConsumptionIndicator");
    if (quota_consumption_indicator) {
    if (!cJSON_IsString(quota_consumption_indicator)) {
        ogs_error("OpenAPI_announcement_information_parseFromJSON() failed [quota_consumption_indicator]");
        goto end;
    }
    quota_consumption_indicatorVariable = OpenAPI_quota_consumption_indicator_FromString(quota_consumption_indicator->valuestring);
    }

    announcement_priority = cJSON_GetObjectItemCaseSensitive(announcement_informationJSON, "announcementPriority");
    if (announcement_priority) {
    if (!cJSON_IsNumber(announcement_priority)) {
        ogs_error("OpenAPI_announcement_information_parseFromJSON() failed [announcement_priority]");
        goto end;
    }
    }

    play_to_party = cJSON_GetObjectItemCaseSensitive(announcement_informationJSON, "playToParty");
    if (play_to_party) {
    if (!cJSON_IsString(play_to_party)) {
        ogs_error("OpenAPI_announcement_information_parseFromJSON() failed [play_to_party]");
        goto end;
    }
    play_to_partyVariable = OpenAPI_play_to_party_FromString(play_to_party->valuestring);
    }

    announcement_privacy_indicator = cJSON_GetObjectItemCaseSensitive(announcement_informationJSON, "announcementPrivacyIndicator");
    if (announcement_privacy_indicator) {
    if (!cJSON_IsString(announcement_privacy_indicator)) {
        ogs_error("OpenAPI_announcement_information_parseFromJSON() failed [announcement_privacy_indicator]");
        goto end;
    }
    announcement_privacy_indicatorVariable = OpenAPI_announcement_privacy_indicator_FromString(announcement_privacy_indicator->valuestring);
    }

    language = cJSON_GetObjectItemCaseSensitive(announcement_informationJSON, "Language");
    if (language) {
    if (!cJSON_IsString(language) && !cJSON_IsNull(language)) {
        ogs_error("OpenAPI_announcement_information_parseFromJSON() failed [language]");
        goto end;
    }
    }

    announcement_information_local_var = OpenAPI_announcement_information_create (
        announcement_identifier ? true : false,
        announcement_identifier ? announcement_identifier->valuedouble : 0,
        announcement_reference && !cJSON_IsNull(announcement_reference) ? ogs_strdup(announcement_reference->valuestring) : NULL,
        variable_parts ? variable_partsList : NULL,
        time_to_play ? true : false,
        time_to_play ? time_to_play->valuedouble : 0,
        quota_consumption_indicator ? quota_consumption_indicatorVariable : 0,
        announcement_priority ? true : false,
        announcement_priority ? announcement_priority->valuedouble : 0,
        play_to_party ? play_to_partyVariable : 0,
        announcement_privacy_indicator ? announcement_privacy_indicatorVariable : 0,
        language && !cJSON_IsNull(language) ? ogs_strdup(language->valuestring) : NULL
    );

    return announcement_information_local_var;
end:
    if (variable_partsList) {
        OpenAPI_list_for_each(variable_partsList, node) {
            OpenAPI_variable_part_free(node->data);
        }
        OpenAPI_list_free(variable_partsList);
        variable_partsList = NULL;
    }
    return NULL;
}

OpenAPI_announcement_information_t *OpenAPI_announcement_information_copy(OpenAPI_announcement_information_t *dst, OpenAPI_announcement_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_announcement_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_announcement_information_convertToJSON() failed");
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

    OpenAPI_announcement_information_free(dst);
    dst = OpenAPI_announcement_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

