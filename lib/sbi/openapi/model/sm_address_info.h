/*
 * sm_address_info.h
 *
 * 
 */

#ifndef _OpenAPI_sm_address_info_H_
#define _OpenAPI_sm_address_info_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_sm_address_info_s OpenAPI_sm_address_info_t;
#include "sm_address_domain.h"
#include "sm_address_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_sm_address_info_s {
    OpenAPI_sm_address_type_e s_maddress_type;
    char *s_maddress_data;
    struct OpenAPI_sm_address_domain_s *s_maddress_domain;
};

OpenAPI_sm_address_info_t *OpenAPI_sm_address_info_create(
    OpenAPI_sm_address_type_e s_maddress_type,
    char *s_maddress_data,
    OpenAPI_sm_address_domain_t *s_maddress_domain
);
void OpenAPI_sm_address_info_free(OpenAPI_sm_address_info_t *sm_address_info);
OpenAPI_sm_address_info_t *OpenAPI_sm_address_info_parseFromJSON(cJSON *sm_address_infoJSON);
cJSON *OpenAPI_sm_address_info_convertToJSON(OpenAPI_sm_address_info_t *sm_address_info);
OpenAPI_sm_address_info_t *OpenAPI_sm_address_info_copy(OpenAPI_sm_address_info_t *dst, OpenAPI_sm_address_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sm_address_info_H_ */

