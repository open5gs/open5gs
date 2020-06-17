/*
 * n2_interface_amf_info.h
 *
 * AMF N2 interface information
 */

#ifndef _OpenAPI_n2_interface_amf_info_H_
#define _OpenAPI_n2_interface_amf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_n2_interface_amf_info_s OpenAPI_n2_interface_amf_info_t;
typedef struct OpenAPI_n2_interface_amf_info_s {
    OpenAPI_list_t *ipv4_endpoint_address;
    OpenAPI_list_t *ipv6_endpoint_address;
    char *amf_name;
} OpenAPI_n2_interface_amf_info_t;

OpenAPI_n2_interface_amf_info_t *OpenAPI_n2_interface_amf_info_create(
    OpenAPI_list_t *ipv4_endpoint_address,
    OpenAPI_list_t *ipv6_endpoint_address,
    char *amf_name
    );
void OpenAPI_n2_interface_amf_info_free(OpenAPI_n2_interface_amf_info_t *n2_interface_amf_info);
OpenAPI_n2_interface_amf_info_t *OpenAPI_n2_interface_amf_info_parseFromJSON(cJSON *n2_interface_amf_infoJSON);
cJSON *OpenAPI_n2_interface_amf_info_convertToJSON(OpenAPI_n2_interface_amf_info_t *n2_interface_amf_info);
OpenAPI_n2_interface_amf_info_t *OpenAPI_n2_interface_amf_info_copy(OpenAPI_n2_interface_amf_info_t *dst, OpenAPI_n2_interface_amf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n2_interface_amf_info_H_ */

