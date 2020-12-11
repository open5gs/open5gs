/*
 * credit_management_status.h
 *
 *
 */

#ifndef _OpenAPI_credit_management_status_H_
#define _OpenAPI_credit_management_status_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_credit_management_status_s OpenAPI_credit_management_status_t;
typedef struct OpenAPI_credit_management_status_s {
} OpenAPI_credit_management_status_t;

OpenAPI_credit_management_status_t *OpenAPI_credit_management_status_create(
    );
void OpenAPI_credit_management_status_free(OpenAPI_credit_management_status_t *credit_management_status);
OpenAPI_credit_management_status_t *OpenAPI_credit_management_status_parseFromJSON(cJSON *credit_management_statusJSON);
cJSON *OpenAPI_credit_management_status_convertToJSON(OpenAPI_credit_management_status_t *credit_management_status);
OpenAPI_credit_management_status_t *OpenAPI_credit_management_status_copy(OpenAPI_credit_management_status_t *dst, OpenAPI_credit_management_status_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_credit_management_status_H_ */

