/*
 * scp_domain_info.h
 *
 * SCP Domain specific information
 */

#ifndef _OpenAPI_scp_domain_info_H_
#define _OpenAPI_scp_domain_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ip_end_point.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_scp_domain_info_s OpenAPI_scp_domain_info_t;
typedef struct OpenAPI_scp_domain_info_s {
    char *scp_fqdn;
    OpenAPI_list_t *scp_ip_end_points;
    char *scp_prefix;
    OpenAPI_list_t* scp_ports;
} OpenAPI_scp_domain_info_t;

OpenAPI_scp_domain_info_t *OpenAPI_scp_domain_info_create(
    char *scp_fqdn,
    OpenAPI_list_t *scp_ip_end_points,
    char *scp_prefix,
    OpenAPI_list_t* scp_ports
);
void OpenAPI_scp_domain_info_free(OpenAPI_scp_domain_info_t *scp_domain_info);
OpenAPI_scp_domain_info_t *OpenAPI_scp_domain_info_parseFromJSON(cJSON *scp_domain_infoJSON);
cJSON *OpenAPI_scp_domain_info_convertToJSON(OpenAPI_scp_domain_info_t *scp_domain_info);
OpenAPI_scp_domain_info_t *OpenAPI_scp_domain_info_copy(OpenAPI_scp_domain_info_t *dst, OpenAPI_scp_domain_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_scp_domain_info_H_ */

