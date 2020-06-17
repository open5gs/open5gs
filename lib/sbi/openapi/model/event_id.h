/*
 * event_id.h
 *
 * Possible values are - LOAD_LEVEL_INFORMATION: Represent the analytics of load level information of corresponding network slice instance. - NETWORK_PERFORMANCE: Represent the analytics of network performance information. - NF_LOAD: Indicates that the event subscribed is NF Load. - SERVICE_EXPERIENCE: Represent the analytics of service experience information of the specific applications. - UE_MOBILITY: Represent the analytics of UE mobility. - UE_COMMUNICATION: Represent the analytics of UE communication. - QOS_SUSTAINABILITY: Represent the analytics of QoS sustainability information in the certain area.  - ABNORMAL_BEHAVIOUR: Indicates that the event subscribed is abnormal behaviour information. - USER_DATA_CONGESTION: Represent the analytics of the user data congestion in the certain area.
 */

#ifndef _OpenAPI_event_id_H_
#define _OpenAPI_event_id_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_event_id_s OpenAPI_event_id_t;
typedef struct OpenAPI_event_id_s {
} OpenAPI_event_id_t;

OpenAPI_event_id_t *OpenAPI_event_id_create(
    );
void OpenAPI_event_id_free(OpenAPI_event_id_t *event_id);
OpenAPI_event_id_t *OpenAPI_event_id_parseFromJSON(cJSON *event_idJSON);
cJSON *OpenAPI_event_id_convertToJSON(OpenAPI_event_id_t *event_id);
OpenAPI_event_id_t *OpenAPI_event_id_copy(OpenAPI_event_id_t *dst, OpenAPI_event_id_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_event_id_H_ */

