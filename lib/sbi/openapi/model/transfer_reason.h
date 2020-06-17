/*
 * transfer_reason.h
 *
 *
 */

#ifndef _OpenAPI_transfer_reason_H_
#define _OpenAPI_transfer_reason_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_transfer_reason_NULL = 0, OpenAPI_transfer_reason_INIT_REG, OpenAPI_transfer_reason_MOBI_REG, OpenAPI_transfer_reason_MOBI_REG_UE_VALIDATED } OpenAPI_transfer_reason_e;

char* OpenAPI_transfer_reason_ToString(OpenAPI_transfer_reason_e transfer_reason);

OpenAPI_transfer_reason_e OpenAPI_transfer_reason_FromString(char* transfer_reason);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_transfer_reason_H_ */

