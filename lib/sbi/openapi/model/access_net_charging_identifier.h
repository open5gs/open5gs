/*
 * access_net_charging_identifier.h
 *
 * Describes the access network charging identifier.
 */

#ifndef _OpenAPI_access_net_charging_identifier_H_
#define _OpenAPI_access_net_charging_identifier_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "flows.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_access_net_charging_identifier_s OpenAPI_access_net_charging_identifier_t;
typedef struct OpenAPI_access_net_charging_identifier_s {
    int acc_net_cha_id_value;
    OpenAPI_list_t *flows;
} OpenAPI_access_net_charging_identifier_t;

OpenAPI_access_net_charging_identifier_t *OpenAPI_access_net_charging_identifier_create(
    int acc_net_cha_id_value,
    OpenAPI_list_t *flows
);
void OpenAPI_access_net_charging_identifier_free(OpenAPI_access_net_charging_identifier_t *access_net_charging_identifier);
OpenAPI_access_net_charging_identifier_t *OpenAPI_access_net_charging_identifier_parseFromJSON(cJSON *access_net_charging_identifierJSON);
cJSON *OpenAPI_access_net_charging_identifier_convertToJSON(OpenAPI_access_net_charging_identifier_t *access_net_charging_identifier);
OpenAPI_access_net_charging_identifier_t *OpenAPI_access_net_charging_identifier_copy(OpenAPI_access_net_charging_identifier_t *dst, OpenAPI_access_net_charging_identifier_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_access_net_charging_identifier_H_ */

