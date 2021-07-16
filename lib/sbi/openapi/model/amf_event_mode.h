/*
 * amf_event_mode.h
 *
 * 
 */

#ifndef _OpenAPI_amf_event_mode_H_
#define _OpenAPI_amf_event_mode_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "amf_event_trigger.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_amf_event_mode_s OpenAPI_amf_event_mode_t;
typedef struct OpenAPI_amf_event_mode_s {
    struct OpenAPI_amf_event_trigger_s *trigger;
    bool is_max_reports;
    int max_reports;
    char *expiry;
    bool is_rep_period;
    int rep_period;
    bool is_samp_ratio;
    int samp_ratio;
} OpenAPI_amf_event_mode_t;

OpenAPI_amf_event_mode_t *OpenAPI_amf_event_mode_create(
    OpenAPI_amf_event_trigger_t *trigger,
    bool is_max_reports,
    int max_reports,
    char *expiry,
    bool is_rep_period,
    int rep_period,
    bool is_samp_ratio,
    int samp_ratio
);
void OpenAPI_amf_event_mode_free(OpenAPI_amf_event_mode_t *amf_event_mode);
OpenAPI_amf_event_mode_t *OpenAPI_amf_event_mode_parseFromJSON(cJSON *amf_event_modeJSON);
cJSON *OpenAPI_amf_event_mode_convertToJSON(OpenAPI_amf_event_mode_t *amf_event_mode);
OpenAPI_amf_event_mode_t *OpenAPI_amf_event_mode_copy(OpenAPI_amf_event_mode_t *dst, OpenAPI_amf_event_mode_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_amf_event_mode_H_ */

