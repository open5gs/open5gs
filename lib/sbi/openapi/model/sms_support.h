/*
 * sms_support.h
 *
 *
 */

#ifndef _OpenAPI_sms_support_H_
#define _OpenAPI_sms_support_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sms_support_s OpenAPI_sms_support_t;
typedef struct OpenAPI_sms_support_s {
} OpenAPI_sms_support_t;

OpenAPI_sms_support_t *OpenAPI_sms_support_create(
    );
void OpenAPI_sms_support_free(OpenAPI_sms_support_t *sms_support);
OpenAPI_sms_support_t *OpenAPI_sms_support_parseFromJSON(cJSON *sms_supportJSON);
cJSON *OpenAPI_sms_support_convertToJSON(OpenAPI_sms_support_t *sms_support);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sms_support_H_ */

