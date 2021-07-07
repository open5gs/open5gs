/*
 * af_event_exposure_data.h
 *
 * AF Event Exposure data managed by a given NEF Instance
 */

#ifndef _OpenAPI_af_event_exposure_data_H_
#define _OpenAPI_af_event_exposure_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "af_event.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_af_event_exposure_data_s OpenAPI_af_event_exposure_data_t;
typedef struct OpenAPI_af_event_exposure_data_s {
    OpenAPI_list_t *af_events;
    OpenAPI_list_t *af_ids;
    OpenAPI_list_t *app_ids;
} OpenAPI_af_event_exposure_data_t;

OpenAPI_af_event_exposure_data_t *OpenAPI_af_event_exposure_data_create(
    OpenAPI_list_t *af_events,
    OpenAPI_list_t *af_ids,
    OpenAPI_list_t *app_ids
);
void OpenAPI_af_event_exposure_data_free(OpenAPI_af_event_exposure_data_t *af_event_exposure_data);
OpenAPI_af_event_exposure_data_t *OpenAPI_af_event_exposure_data_parseFromJSON(cJSON *af_event_exposure_dataJSON);
cJSON *OpenAPI_af_event_exposure_data_convertToJSON(OpenAPI_af_event_exposure_data_t *af_event_exposure_data);
OpenAPI_af_event_exposure_data_t *OpenAPI_af_event_exposure_data_copy(OpenAPI_af_event_exposure_data_t *dst, OpenAPI_af_event_exposure_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_af_event_exposure_data_H_ */

