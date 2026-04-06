/*
 * easdf_info.h
 *
 * Information of an EASDF NF Instance
 */

#ifndef _OpenAPI_easdf_info_H_
#define _OpenAPI_easdf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_easdf_info_s OpenAPI_easdf_info_t;
#include "dns_security_protocol.h"
#include "interface_upf_info_item.h"
#include "ip_addr.h"
#include "snssai_easdf_info_item.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_easdf_info_s {
    OpenAPI_list_t *s_nssai_easdf_info_list;
    OpenAPI_list_t *easdf_n6_ip_address_list;
    OpenAPI_list_t *upf_n6_ip_address_list;
    OpenAPI_list_t* n6_tunnel_info_list;
    OpenAPI_list_t *dns_security_protocols;
};

OpenAPI_easdf_info_t *OpenAPI_easdf_info_create(
    OpenAPI_list_t *s_nssai_easdf_info_list,
    OpenAPI_list_t *easdf_n6_ip_address_list,
    OpenAPI_list_t *upf_n6_ip_address_list,
    OpenAPI_list_t* n6_tunnel_info_list,
    OpenAPI_list_t *dns_security_protocols
);
void OpenAPI_easdf_info_free(OpenAPI_easdf_info_t *easdf_info);
OpenAPI_easdf_info_t *OpenAPI_easdf_info_parseFromJSON(cJSON *easdf_infoJSON);
cJSON *OpenAPI_easdf_info_convertToJSON(OpenAPI_easdf_info_t *easdf_info);
OpenAPI_easdf_info_t *OpenAPI_easdf_info_copy(OpenAPI_easdf_info_t *dst, OpenAPI_easdf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_easdf_info_H_ */

