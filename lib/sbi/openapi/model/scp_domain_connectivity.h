/*
 * scp_domain_connectivity.h
 *
 * SCP Domain Connectivity Information
 */

#ifndef _OpenAPI_scp_domain_connectivity_H_
#define _OpenAPI_scp_domain_connectivity_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_scp_domain_connectivity_s OpenAPI_scp_domain_connectivity_t;
typedef struct OpenAPI_scp_domain_connectivity_s {
    OpenAPI_list_t *connected_scp_domain_list;
} OpenAPI_scp_domain_connectivity_t;

OpenAPI_scp_domain_connectivity_t *OpenAPI_scp_domain_connectivity_create(
    OpenAPI_list_t *connected_scp_domain_list
);
void OpenAPI_scp_domain_connectivity_free(OpenAPI_scp_domain_connectivity_t *scp_domain_connectivity);
OpenAPI_scp_domain_connectivity_t *OpenAPI_scp_domain_connectivity_parseFromJSON(cJSON *scp_domain_connectivityJSON);
cJSON *OpenAPI_scp_domain_connectivity_convertToJSON(OpenAPI_scp_domain_connectivity_t *scp_domain_connectivity);
OpenAPI_scp_domain_connectivity_t *OpenAPI_scp_domain_connectivity_copy(OpenAPI_scp_domain_connectivity_t *dst, OpenAPI_scp_domain_connectivity_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_scp_domain_connectivity_H_ */

