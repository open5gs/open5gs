/*
 * dccf_event.h
 *
 * Identifies the (event exposure or analytics) event that the processing instructions  shall apply to. Contains all event IDs related to DCCF. 
 */

#ifndef _OpenAPI_dccf_event_H_
#define _OpenAPI_dccf_event_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_dccf_event_s OpenAPI_dccf_event_t;
#include "af_event.h"
#include "amf_event_type.h"
#include "event_notify_data_type.h"
#include "event_type.h"
#include "event_type_1.h"
#include "nef_event.h"
#include "notification_event_type.h"
#include "nwdaf_event.h"
#include "pc_event.h"
#include "sac_event.h"
#include "smf_event.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_dccf_event_s {
    OpenAPI_nwdaf_event_e nwdaf_event;
    OpenAPI_smf_event_e smf_event;
    OpenAPI_amf_event_type_e amf_event;
    OpenAPI_nef_event_e nef_event;
    OpenAPI_event_type_e udm_event;
    OpenAPI_af_event_e af_event;
    struct OpenAPI_sac_event_s *sac_event;
    OpenAPI_notification_event_type_e nrf_event;
    OpenAPI_event_notify_data_type_e gmlc_event;
    OpenAPI_event_type_1_e upf_event;
    OpenAPI_pc_event_e pcf_event;
};

OpenAPI_dccf_event_t *OpenAPI_dccf_event_create(
    OpenAPI_nwdaf_event_e nwdaf_event,
    OpenAPI_smf_event_e smf_event,
    OpenAPI_amf_event_type_e amf_event,
    OpenAPI_nef_event_e nef_event,
    OpenAPI_event_type_e udm_event,
    OpenAPI_af_event_e af_event,
    OpenAPI_sac_event_t *sac_event,
    OpenAPI_notification_event_type_e nrf_event,
    OpenAPI_event_notify_data_type_e gmlc_event,
    OpenAPI_event_type_1_e upf_event,
    OpenAPI_pc_event_e pcf_event
);
void OpenAPI_dccf_event_free(OpenAPI_dccf_event_t *dccf_event);
OpenAPI_dccf_event_t *OpenAPI_dccf_event_parseFromJSON(cJSON *dccf_eventJSON);
cJSON *OpenAPI_dccf_event_convertToJSON(OpenAPI_dccf_event_t *dccf_event);
OpenAPI_dccf_event_t *OpenAPI_dccf_event_copy(OpenAPI_dccf_event_t *dst, OpenAPI_dccf_event_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dccf_event_H_ */

