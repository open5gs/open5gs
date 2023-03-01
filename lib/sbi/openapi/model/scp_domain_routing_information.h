/*
 * scp_domain_routing_information.h
 *
 * SCP Domain Routing Information
 */

#ifndef _OpenAPI_scp_domain_routing_information_H_
#define _OpenAPI_scp_domain_routing_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "scp_domain_connectivity.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_scp_domain_routing_information_s OpenAPI_scp_domain_routing_information_t;
typedef struct OpenAPI_scp_domain_routing_information_s {
    OpenAPI_list_t* scp_domain_list;
} OpenAPI_scp_domain_routing_information_t;

OpenAPI_scp_domain_routing_information_t *OpenAPI_scp_domain_routing_information_create(
    OpenAPI_list_t* scp_domain_list
);
void OpenAPI_scp_domain_routing_information_free(OpenAPI_scp_domain_routing_information_t *scp_domain_routing_information);
OpenAPI_scp_domain_routing_information_t *OpenAPI_scp_domain_routing_information_parseFromJSON(cJSON *scp_domain_routing_informationJSON);
cJSON *OpenAPI_scp_domain_routing_information_convertToJSON(OpenAPI_scp_domain_routing_information_t *scp_domain_routing_information);
OpenAPI_scp_domain_routing_information_t *OpenAPI_scp_domain_routing_information_copy(OpenAPI_scp_domain_routing_information_t *dst, OpenAPI_scp_domain_routing_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_scp_domain_routing_information_H_ */

