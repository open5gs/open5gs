/*
 * mri_transfer_method.h
 *
 * 
 */

#ifndef _OpenAPI_mri_transfer_method_H_
#define _OpenAPI_mri_transfer_method_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_mri_transfer_method_NULL = 0, OpenAPI_mri_transfer_method_UDP_OPTION } OpenAPI_mri_transfer_method_e;

char* OpenAPI_mri_transfer_method_ToString(OpenAPI_mri_transfer_method_e mri_transfer_method);

OpenAPI_mri_transfer_method_e OpenAPI_mri_transfer_method_FromString(char* mri_transfer_method);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mri_transfer_method_H_ */

