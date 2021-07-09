/*
 * hss_auth_type_in_uri.h
 *
 * 
 */

#ifndef _OpenAPI_hss_auth_type_in_uri_H_
#define _OpenAPI_hss_auth_type_in_uri_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_hss_auth_type_in_uri_NULL = 0, OpenAPI_hss_auth_type_in_uri_eps_aka, OpenAPI_hss_auth_type_in_uri_eap_aka, OpenAPI_hss_auth_type_in_uri_eap_aka_prime, OpenAPI_hss_auth_type_in_uri_ims_aka, OpenAPI_hss_auth_type_in_uri_gba_aka } OpenAPI_hss_auth_type_in_uri_e;

char* OpenAPI_hss_auth_type_in_uri_ToString(OpenAPI_hss_auth_type_in_uri_e hss_auth_type_in_uri);

OpenAPI_hss_auth_type_in_uri_e OpenAPI_hss_auth_type_in_uri_FromString(char* hss_auth_type_in_uri);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_hss_auth_type_in_uri_H_ */

