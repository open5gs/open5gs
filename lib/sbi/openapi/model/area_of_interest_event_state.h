/*
 * area_of_interest_event_state.h
 *
 * Event State of AoI event in old AMF
 */

#ifndef _OpenAPI_area_of_interest_event_state_H_
#define _OpenAPI_area_of_interest_event_state_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "presence_state.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_area_of_interest_event_state_s OpenAPI_area_of_interest_event_state_t;
typedef struct OpenAPI_area_of_interest_event_state_s {
    OpenAPI_presence_state_e presence;
    OpenAPI_list_t *individual_pra_id_list;
} OpenAPI_area_of_interest_event_state_t;

OpenAPI_area_of_interest_event_state_t *OpenAPI_area_of_interest_event_state_create(
    OpenAPI_presence_state_e presence,
    OpenAPI_list_t *individual_pra_id_list
);
void OpenAPI_area_of_interest_event_state_free(OpenAPI_area_of_interest_event_state_t *area_of_interest_event_state);
OpenAPI_area_of_interest_event_state_t *OpenAPI_area_of_interest_event_state_parseFromJSON(cJSON *area_of_interest_event_stateJSON);
cJSON *OpenAPI_area_of_interest_event_state_convertToJSON(OpenAPI_area_of_interest_event_state_t *area_of_interest_event_state);
OpenAPI_area_of_interest_event_state_t *OpenAPI_area_of_interest_event_state_copy(OpenAPI_area_of_interest_event_state_t *dst, OpenAPI_area_of_interest_event_state_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_area_of_interest_event_state_H_ */

