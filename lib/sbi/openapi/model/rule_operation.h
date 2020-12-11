/*
 * rule_operation.h
 *
 * Possible values are - CREATE_PCC_RULE: Indicates to create a new PCC rule to reserve the resource requested by the UE.  - DELETE_PCC_RULE: Indicates to delete a PCC rule corresponding to reserve the resource requested by the UE.. - MODIFY_PCC_RULE_AND_ADD_PACKET_FILTERS: Indicates to modify the PCC rule by adding new packet filter(s). - MODIFY_ PCC_RULE_AND_REPLACE_PACKET_FILTERS: Indicates to modify the PCC rule by replacing the existing packet filter(s). - MODIFY_ PCC_RULE_AND_DELETE_PACKET_FILTERS: Indicates to modify the PCC rule by deleting the existing packet filter(s). - MODIFY_PCC_RULE_WITHOUT_MODIFY_PACKET_FILTERS: Indicates to modify the PCC rule by modifying the QoS of the PCC rule.
 */

#ifndef _OpenAPI_rule_operation_H_
#define _OpenAPI_rule_operation_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_rule_operation_s OpenAPI_rule_operation_t;
typedef struct OpenAPI_rule_operation_s {
} OpenAPI_rule_operation_t;

OpenAPI_rule_operation_t *OpenAPI_rule_operation_create(
    );
void OpenAPI_rule_operation_free(OpenAPI_rule_operation_t *rule_operation);
OpenAPI_rule_operation_t *OpenAPI_rule_operation_parseFromJSON(cJSON *rule_operationJSON);
cJSON *OpenAPI_rule_operation_convertToJSON(OpenAPI_rule_operation_t *rule_operation);
OpenAPI_rule_operation_t *OpenAPI_rule_operation_copy(OpenAPI_rule_operation_t *dst, OpenAPI_rule_operation_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_rule_operation_H_ */

