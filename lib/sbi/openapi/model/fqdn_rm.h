/*
 * fqdn_rm.h
 *
 * Fully Qualified Domain Name, but it also allows the null value
 */

#ifndef _OpenAPI_fqdn_rm_H_
#define _OpenAPI_fqdn_rm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "null_value.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_fqdn_rm_s OpenAPI_fqdn_rm_t;
typedef struct OpenAPI_fqdn_rm_s {
} OpenAPI_fqdn_rm_t;

OpenAPI_fqdn_rm_t *OpenAPI_fqdn_rm_create(
);
void OpenAPI_fqdn_rm_free(OpenAPI_fqdn_rm_t *fqdn_rm);
OpenAPI_fqdn_rm_t *OpenAPI_fqdn_rm_parseFromJSON(cJSON *fqdn_rmJSON);
cJSON *OpenAPI_fqdn_rm_convertToJSON(OpenAPI_fqdn_rm_t *fqdn_rm);
OpenAPI_fqdn_rm_t *OpenAPI_fqdn_rm_copy(OpenAPI_fqdn_rm_t *dst, OpenAPI_fqdn_rm_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_fqdn_rm_H_ */

