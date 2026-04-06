/*
 * nrppa_trans_failure.h
 *
 * Cause for NRPPa transfer failure
 */

#ifndef _OpenAPI_nrppa_trans_failure_H_
#define _OpenAPI_nrppa_trans_failure_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_nrppa_trans_failure_NULL = 0, OpenAPI_nrppa_trans_failure_NG_RAN_NODE_NOT_REACHABLE, OpenAPI_nrppa_trans_failure_NG_RAN_NODE_UNKNONWN } OpenAPI_nrppa_trans_failure_e;

char* OpenAPI_nrppa_trans_failure_ToString(OpenAPI_nrppa_trans_failure_e nrppa_trans_failure);

OpenAPI_nrppa_trans_failure_e OpenAPI_nrppa_trans_failure_FromString(char* nrppa_trans_failure);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nrppa_trans_failure_H_ */

