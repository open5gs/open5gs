/*
 * on_path_n6_sig_info.h
 *
 * Represents the on path N6 signaling information. 
 */

#ifndef _OpenAPI_on_path_n6_sig_info_H_
#define _OpenAPI_on_path_n6_sig_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_on_path_n6_sig_info_s OpenAPI_on_path_n6_sig_info_t;
#include "ip_addr.h"
#include "on_path_n6_method.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_on_path_n6_sig_info_s {
    OpenAPI_on_path_n6_method_e on_path_n6_method;
    struct OpenAPI_ip_addr_s *as_proxy_addr;
};

OpenAPI_on_path_n6_sig_info_t *OpenAPI_on_path_n6_sig_info_create(
    OpenAPI_on_path_n6_method_e on_path_n6_method,
    OpenAPI_ip_addr_t *as_proxy_addr
);
void OpenAPI_on_path_n6_sig_info_free(OpenAPI_on_path_n6_sig_info_t *on_path_n6_sig_info);
OpenAPI_on_path_n6_sig_info_t *OpenAPI_on_path_n6_sig_info_parseFromJSON(cJSON *on_path_n6_sig_infoJSON);
cJSON *OpenAPI_on_path_n6_sig_info_convertToJSON(OpenAPI_on_path_n6_sig_info_t *on_path_n6_sig_info);
OpenAPI_on_path_n6_sig_info_t *OpenAPI_on_path_n6_sig_info_copy(OpenAPI_on_path_n6_sig_info_t *dst, OpenAPI_on_path_n6_sig_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_on_path_n6_sig_info_H_ */

