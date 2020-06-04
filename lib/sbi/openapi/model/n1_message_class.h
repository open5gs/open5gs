/*
 * n1_message_class.h
 *
 *
 */

#ifndef _OpenAPI_n1_message_class_H_
#define _OpenAPI_n1_message_class_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_n1_message_class_NULL = 0, OpenAPI_n1_message_class_5GMM, OpenAPI_n1_message_class_SM, OpenAPI_n1_message_class_LPP, OpenAPI_n1_message_class_SMS, OpenAPI_n1_message_class_UPDP, OpenAPI_n1_message_class_LCS } OpenAPI_n1_message_class_e;

char* OpenAPI_n1_message_class_ToString(OpenAPI_n1_message_class_e n1_message_class);

OpenAPI_n1_message_class_e OpenAPI_n1_message_class_FromString(char* n1_message_class);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n1_message_class_H_ */

