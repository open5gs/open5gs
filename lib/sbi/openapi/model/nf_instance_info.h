/*
 * nf_instance_info.h
 *
 * Contains information on an NF profile matching a discovery request
 */

#ifndef _OpenAPI_nf_instance_info_H_
#define _OpenAPI_nf_instance_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "preferred_search.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nf_instance_info_s OpenAPI_nf_instance_info_t;
typedef struct OpenAPI_nf_instance_info_s {
    char *nrf_disc_api_uri;
    struct OpenAPI_preferred_search_s *preferred_search;
    OpenAPI_list_t* nrf_altered_priorities;
} OpenAPI_nf_instance_info_t;

OpenAPI_nf_instance_info_t *OpenAPI_nf_instance_info_create(
    char *nrf_disc_api_uri,
    OpenAPI_preferred_search_t *preferred_search,
    OpenAPI_list_t* nrf_altered_priorities
);
void OpenAPI_nf_instance_info_free(OpenAPI_nf_instance_info_t *nf_instance_info);
OpenAPI_nf_instance_info_t *OpenAPI_nf_instance_info_parseFromJSON(cJSON *nf_instance_infoJSON);
cJSON *OpenAPI_nf_instance_info_convertToJSON(OpenAPI_nf_instance_info_t *nf_instance_info);
OpenAPI_nf_instance_info_t *OpenAPI_nf_instance_info_copy(OpenAPI_nf_instance_info_t *dst, OpenAPI_nf_instance_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nf_instance_info_H_ */

