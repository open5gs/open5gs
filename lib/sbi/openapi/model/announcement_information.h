/*
 * announcement_information.h
 *
 * 
 */

#ifndef _OpenAPI_announcement_information_H_
#define _OpenAPI_announcement_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_announcement_information_s OpenAPI_announcement_information_t;
#include "announcement_privacy_indicator.h"
#include "play_to_party.h"
#include "quota_consumption_indicator.h"
#include "variable_part.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_announcement_information_s {
    bool is_announcement_identifier;
    int announcement_identifier;
    char *announcement_reference;
    OpenAPI_list_t *variable_parts;
    bool is_time_to_play;
    int time_to_play;
    OpenAPI_quota_consumption_indicator_e quota_consumption_indicator;
    bool is_announcement_priority;
    int announcement_priority;
    OpenAPI_play_to_party_e play_to_party;
    OpenAPI_announcement_privacy_indicator_e announcement_privacy_indicator;
    char *language;
};

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
);
void OpenAPI_announcement_information_free(OpenAPI_announcement_information_t *announcement_information);
OpenAPI_announcement_information_t *OpenAPI_announcement_information_parseFromJSON(cJSON *announcement_informationJSON);
cJSON *OpenAPI_announcement_information_convertToJSON(OpenAPI_announcement_information_t *announcement_information);
OpenAPI_announcement_information_t *OpenAPI_announcement_information_copy(OpenAPI_announcement_information_t *dst, OpenAPI_announcement_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_announcement_information_H_ */

