/*
 * nf_identification.h
 *
 * 
 */

#ifndef _OpenAPI_nf_identification_H_
#define _OpenAPI_nf_identification_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_nf_identification_s OpenAPI_nf_identification_t;
#include "node_functionality.h"
#include "plmn_id.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_nf_identification_s {
    char *n_f_name;
    char *n_fipv4_address;
    char *n_fipv6_address;
    struct OpenAPI_plmn_id_s *n_fplmnid;
    OpenAPI_node_functionality_e node_functionality;
    char *n_f_fqdn;
};

OpenAPI_nf_identification_t *OpenAPI_nf_identification_create(
    char *n_f_name,
    char *n_fipv4_address,
    char *n_fipv6_address,
    OpenAPI_plmn_id_t *n_fplmnid,
    OpenAPI_node_functionality_e node_functionality,
    char *n_f_fqdn
);
void OpenAPI_nf_identification_free(OpenAPI_nf_identification_t *nf_identification);
OpenAPI_nf_identification_t *OpenAPI_nf_identification_parseFromJSON(cJSON *nf_identificationJSON);
cJSON *OpenAPI_nf_identification_convertToJSON(OpenAPI_nf_identification_t *nf_identification);
OpenAPI_nf_identification_t *OpenAPI_nf_identification_copy(OpenAPI_nf_identification_t *dst, OpenAPI_nf_identification_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nf_identification_H_ */

