/*
 * rule_status.h
 *
 * 
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

typedef enum { OpenAPI_rule_status_NULL = 0, OpenAPI_rule_status_ACTIVE, OpenAPI_rule_status_INACTIVE } OpenAPI_rule_status_e;

char* OpenAPI_rule_status_ToString(OpenAPI_rule_status_e rule_status);

OpenAPI_rule_status_e OpenAPI_rule_status_FromString(char* rule_status);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_rule_status_H_ */

