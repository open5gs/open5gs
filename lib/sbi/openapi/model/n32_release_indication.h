/*
 * n32_release_indication.h
 *
 * N32-f connection/context release indication
 */

#ifndef _OpenAPI_n32_release_indication_H_
#define _OpenAPI_n32_release_indication_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_n32_release_indication_NULL = 0, OpenAPI_n32_release_indication_RELEASE_REESTABLISHMENT_ALLOWED, OpenAPI_n32_release_indication_RELEASE_REESTABLISHMENT_NOT_ALLOWED, OpenAPI_n32_release_indication_REESTABLISH } OpenAPI_n32_release_indication_e;

char* OpenAPI_n32_release_indication_ToString(OpenAPI_n32_release_indication_e n32_release_indication);

OpenAPI_n32_release_indication_e OpenAPI_n32_release_indication_FromString(char* n32_release_indication);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n32_release_indication_H_ */

