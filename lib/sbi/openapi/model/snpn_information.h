/*
 * snpn_information.h
 *
 * 
 */

#ifndef _OpenAPI_snpn_information_H_
#define _OpenAPI_snpn_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_snpn_information_s OpenAPI_snpn_information_t;
#include "access_type.h"
#include "plmn_id_nid.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_snpn_information_s {
    struct OpenAPI_plmn_id_nid_s *s_npnid;
    OpenAPI_access_type_e access_type;
    char *n3_iwf_fqdn;
};

OpenAPI_snpn_information_t *OpenAPI_snpn_information_create(
    OpenAPI_plmn_id_nid_t *s_npnid,
    OpenAPI_access_type_e access_type,
    char *n3_iwf_fqdn
);
void OpenAPI_snpn_information_free(OpenAPI_snpn_information_t *snpn_information);
OpenAPI_snpn_information_t *OpenAPI_snpn_information_parseFromJSON(cJSON *snpn_informationJSON);
cJSON *OpenAPI_snpn_information_convertToJSON(OpenAPI_snpn_information_t *snpn_information);
OpenAPI_snpn_information_t *OpenAPI_snpn_information_copy(OpenAPI_snpn_information_t *dst, OpenAPI_snpn_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_snpn_information_H_ */

