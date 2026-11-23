/*
 * sm_address_domain.h
 *
 * 
 */

#ifndef _OpenAPI_sm_address_domain_H_
#define _OpenAPI_sm_address_domain_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_sm_address_domain_s OpenAPI_sm_address_domain_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_sm_address_domain_s {
    char *domain_name;
    char *_3_gppimsimccmnc;
};

OpenAPI_sm_address_domain_t *OpenAPI_sm_address_domain_create(
    char *domain_name,
    char *_3_gppimsimccmnc
);
void OpenAPI_sm_address_domain_free(OpenAPI_sm_address_domain_t *sm_address_domain);
OpenAPI_sm_address_domain_t *OpenAPI_sm_address_domain_parseFromJSON(cJSON *sm_address_domainJSON);
cJSON *OpenAPI_sm_address_domain_convertToJSON(OpenAPI_sm_address_domain_t *sm_address_domain);
OpenAPI_sm_address_domain_t *OpenAPI_sm_address_domain_copy(OpenAPI_sm_address_domain_t *dst, OpenAPI_sm_address_domain_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sm_address_domain_H_ */

