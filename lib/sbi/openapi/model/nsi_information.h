/*
 * nsi_information.h
 *
 * Contains the API URIs of NRF services to be used to discover NFs/services, subscribe to NF status changes and/or request access tokens within the selected Network Slice instance and optional the Identifier of the selected Network Slice instance 
 */

#ifndef _OpenAPI_nsi_information_H_
#define _OpenAPI_nsi_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nsi_information_s OpenAPI_nsi_information_t;
typedef struct OpenAPI_nsi_information_s {
    char *nrf_id;
    char *nsi_id;
    char *nrf_nf_mgt_uri;
    char *nrf_access_token_uri;
    OpenAPI_list_t* nrf_oauth2_required;
} OpenAPI_nsi_information_t;

OpenAPI_nsi_information_t *OpenAPI_nsi_information_create(
    char *nrf_id,
    char *nsi_id,
    char *nrf_nf_mgt_uri,
    char *nrf_access_token_uri,
    OpenAPI_list_t* nrf_oauth2_required
);
void OpenAPI_nsi_information_free(OpenAPI_nsi_information_t *nsi_information);
OpenAPI_nsi_information_t *OpenAPI_nsi_information_parseFromJSON(cJSON *nsi_informationJSON);
cJSON *OpenAPI_nsi_information_convertToJSON(OpenAPI_nsi_information_t *nsi_information);
OpenAPI_nsi_information_t *OpenAPI_nsi_information_copy(OpenAPI_nsi_information_t *dst, OpenAPI_nsi_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nsi_information_H_ */

