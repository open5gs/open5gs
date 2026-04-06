/*
 * establishment_rejection_cause.h
 *
 * 
 */

#ifndef _OpenAPI_establishment_rejection_cause_H_
#define _OpenAPI_establishment_rejection_cause_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_establishment_rejection_cause_NULL = 0, OpenAPI_establishment_rejection_cause_OPERATOR_DETERMINED_BARRING } OpenAPI_establishment_rejection_cause_e;

char* OpenAPI_establishment_rejection_cause_ToString(OpenAPI_establishment_rejection_cause_e establishment_rejection_cause);

OpenAPI_establishment_rejection_cause_e OpenAPI_establishment_rejection_cause_FromString(char* establishment_rejection_cause);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_establishment_rejection_cause_H_ */

