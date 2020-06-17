/*
 * notif_condition.h
 *
 * Condition (list of attributes in the NF Profile) to determine whether a notification must be sent by NRF
 */

#ifndef _OpenAPI_notif_condition_H_
#define _OpenAPI_notif_condition_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_notif_condition_s OpenAPI_notif_condition_t;
typedef struct OpenAPI_notif_condition_s {
    OpenAPI_list_t *monitored_attributes;
    OpenAPI_list_t *unmonitored_attributes;
} OpenAPI_notif_condition_t;

OpenAPI_notif_condition_t *OpenAPI_notif_condition_create(
    OpenAPI_list_t *monitored_attributes,
    OpenAPI_list_t *unmonitored_attributes
    );
void OpenAPI_notif_condition_free(OpenAPI_notif_condition_t *notif_condition);
OpenAPI_notif_condition_t *OpenAPI_notif_condition_parseFromJSON(cJSON *notif_conditionJSON);
cJSON *OpenAPI_notif_condition_convertToJSON(OpenAPI_notif_condition_t *notif_condition);
OpenAPI_notif_condition_t *OpenAPI_notif_condition_copy(OpenAPI_notif_condition_t *dst, OpenAPI_notif_condition_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_notif_condition_H_ */

