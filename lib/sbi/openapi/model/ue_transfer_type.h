/*
 * ue_transfer_type.h
 *
 * 
 */

#ifndef _OpenAPI_ue_transfer_type_H_
#define _OpenAPI_ue_transfer_type_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_ue_transfer_type_NULL = 0, OpenAPI_ue_transfer_type_INTRA_UE, OpenAPI_ue_transfer_type_INTER_UE } OpenAPI_ue_transfer_type_e;

char* OpenAPI_ue_transfer_type_ToString(OpenAPI_ue_transfer_type_e ue_transfer_type);

OpenAPI_ue_transfer_type_e OpenAPI_ue_transfer_type_FromString(char* ue_transfer_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_transfer_type_H_ */

