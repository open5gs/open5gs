/*
 * psa_indication.h
 *
 *
 */

#ifndef _OpenAPI_psa_indication_H_
#define _OpenAPI_psa_indication_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_psa_indication_NULL = 0, OpenAPI_psa_indication_PSA_INSERTED, OpenAPI_psa_indication_PSA_REMOVED } OpenAPI_psa_indication_e;

char* OpenAPI_psa_indication_ToString(OpenAPI_psa_indication_e psa_indication);

OpenAPI_psa_indication_e OpenAPI_psa_indication_FromString(char* psa_indication);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_psa_indication_H_ */

