/*
 * scp_domain_cond.h
 *
 * Subscription to a set of NF or SCP instances belonging to certain SCP domains
 */

#ifndef _OpenAPI_scp_domain_cond_H_
#define _OpenAPI_scp_domain_cond_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_scp_domain_cond_s OpenAPI_scp_domain_cond_t;
typedef struct OpenAPI_scp_domain_cond_s {
    OpenAPI_list_t *scp_domains;
} OpenAPI_scp_domain_cond_t;

OpenAPI_scp_domain_cond_t *OpenAPI_scp_domain_cond_create(
    OpenAPI_list_t *scp_domains
);
void OpenAPI_scp_domain_cond_free(OpenAPI_scp_domain_cond_t *scp_domain_cond);
OpenAPI_scp_domain_cond_t *OpenAPI_scp_domain_cond_parseFromJSON(cJSON *scp_domain_condJSON);
cJSON *OpenAPI_scp_domain_cond_convertToJSON(OpenAPI_scp_domain_cond_t *scp_domain_cond);
OpenAPI_scp_domain_cond_t *OpenAPI_scp_domain_cond_copy(OpenAPI_scp_domain_cond_t *dst, OpenAPI_scp_domain_cond_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_scp_domain_cond_H_ */

