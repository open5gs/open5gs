/*
 * nssaa_status.h
 *
 * 
 */

#ifndef _OpenAPI_nssaa_status_H_
#define _OpenAPI_nssaa_status_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "auth_status.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nssaa_status_s OpenAPI_nssaa_status_t;
typedef struct OpenAPI_nssaa_status_s {
    struct OpenAPI_snssai_s *snssai;
    OpenAPI_auth_status_e status;
} OpenAPI_nssaa_status_t;

OpenAPI_nssaa_status_t *OpenAPI_nssaa_status_create(
    OpenAPI_snssai_t *snssai,
    OpenAPI_auth_status_e status
);
void OpenAPI_nssaa_status_free(OpenAPI_nssaa_status_t *nssaa_status);
OpenAPI_nssaa_status_t *OpenAPI_nssaa_status_parseFromJSON(cJSON *nssaa_statusJSON);
cJSON *OpenAPI_nssaa_status_convertToJSON(OpenAPI_nssaa_status_t *nssaa_status);
OpenAPI_nssaa_status_t *OpenAPI_nssaa_status_copy(OpenAPI_nssaa_status_t *dst, OpenAPI_nssaa_status_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nssaa_status_H_ */

