/*
 * n2_information_transfer_result.h
 *
 * Describes the result of N2 information transfer by AMF to the AN
 */

#ifndef _OpenAPI_n2_information_transfer_result_H_
#define _OpenAPI_n2_information_transfer_result_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_n2_information_transfer_result_NULL = 0, OpenAPI_n2_information_transfer_result_N2_INFO_TRANSFER_INITIATED, OpenAPI_n2_information_transfer_result_PARTIAL_N2_INFO_TRANSFER } OpenAPI_n2_information_transfer_result_e;

char* OpenAPI_n2_information_transfer_result_ToString(OpenAPI_n2_information_transfer_result_e n2_information_transfer_result);

OpenAPI_n2_information_transfer_result_e OpenAPI_n2_information_transfer_result_FromString(char* n2_information_transfer_result);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n2_information_transfer_result_H_ */

