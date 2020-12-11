/*
 * rule_status.h
 *
 * Possible values are - ACTIVE: Indicates that the PCC rule(s) are successfully installed (for those provisioned from PCF) or activated (for those pre-defined in SMF), or the session rule(s) are successfully installed  - INACTIVE: Indicates that the PCC rule(s) are removed (for those provisioned from PCF) or inactive (for those pre-defined in SMF) or the session rule(s) are removed.
 */

#ifndef _OpenAPI_rule_status_H_
#define _OpenAPI_rule_status_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_rule_status_s OpenAPI_rule_status_t;
typedef struct OpenAPI_rule_status_s {
} OpenAPI_rule_status_t;

OpenAPI_rule_status_t *OpenAPI_rule_status_create(
    );
void OpenAPI_rule_status_free(OpenAPI_rule_status_t *rule_status);
OpenAPI_rule_status_t *OpenAPI_rule_status_parseFromJSON(cJSON *rule_statusJSON);
cJSON *OpenAPI_rule_status_convertToJSON(OpenAPI_rule_status_t *rule_status);
OpenAPI_rule_status_t *OpenAPI_rule_status_copy(OpenAPI_rule_status_t *dst, OpenAPI_rule_status_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_rule_status_H_ */

