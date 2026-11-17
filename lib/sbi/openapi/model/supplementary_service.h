/*
 * supplementary_service.h
 *
 * 
 */

#ifndef _OpenAPI_supplementary_service_H_
#define _OpenAPI_supplementary_service_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_supplementary_service_s OpenAPI_supplementary_service_t;
#include "participant_action_type.h"
#include "supplementary_service_mode.h"
#include "supplementary_service_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_supplementary_service_s {
    OpenAPI_supplementary_service_type_e supplementary_service_type;
    OpenAPI_supplementary_service_mode_e supplementary_service_mode;
    bool is_number_of_diversions;
    int number_of_diversions;
    char *associated_party_address;
    char *conference_id;
    OpenAPI_participant_action_type_e participant_action_type;
    char *change_time;
    bool is_number_of_participants;
    int number_of_participants;
    char *c_ug_information;
};

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
);
void OpenAPI_supplementary_service_free(OpenAPI_supplementary_service_t *supplementary_service);
OpenAPI_supplementary_service_t *OpenAPI_supplementary_service_parseFromJSON(cJSON *supplementary_serviceJSON);
cJSON *OpenAPI_supplementary_service_convertToJSON(OpenAPI_supplementary_service_t *supplementary_service);
OpenAPI_supplementary_service_t *OpenAPI_supplementary_service_copy(OpenAPI_supplementary_service_t *dst, OpenAPI_supplementary_service_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_supplementary_service_H_ */

