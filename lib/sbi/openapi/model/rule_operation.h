/*
 * rule_operation.h
 *
 * 
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

typedef enum { OpenAPI_rule_operation_NULL = 0, OpenAPI_rule_operation_CREATE_PCC_RULE, OpenAPI_rule_operation_DELETE_PCC_RULE, OpenAPI_rule_operation_MODIFY_PCC_RULE_AND_ADD_PACKET_FILTERS, OpenAPI_rule_operation_MODIFY_PCC_RULE_AND_REPLACE_PACKET_FILTERS, OpenAPI_rule_operation_MODIFY_PCC_RULE_AND_DELETE_PACKET_FILTERS, OpenAPI_rule_operation_MODIFY_PCC_RULE_WITHOUT_MODIFY_PACKET_FILTERS } OpenAPI_rule_operation_e;

char* OpenAPI_rule_operation_ToString(OpenAPI_rule_operation_e rule_operation);

OpenAPI_rule_operation_e OpenAPI_rule_operation_FromString(char* rule_operation);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_rule_operation_H_ */

