/*
 * n2_information_class.h
 *
 *
 */

#ifndef _OpenAPI_n2_information_class_H_
#define _OpenAPI_n2_information_class_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_n2_information_class_NULL = 0, OpenAPI_n2_information_class_SM, OpenAPI_n2_information_class_NRPPa, OpenAPI_n2_information_class_PWS, OpenAPI_n2_information_class_PWS_BCAL, OpenAPI_n2_information_class_PWS_RF, OpenAPI_n2_information_class_RAN } OpenAPI_n2_information_class_e;

char* OpenAPI_n2_information_class_ToString(OpenAPI_n2_information_class_e n2_information_class);

OpenAPI_n2_information_class_e OpenAPI_n2_information_class_FromString(char* n2_information_class);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n2_information_class_H_ */

