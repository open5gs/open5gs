/*
 * suggested_pfd_info.h
 *
 * Represents the suggested PFD information for the application identifier.
 */

#ifndef _OpenAPI_suggested_pfd_info_H_
#define _OpenAPI_suggested_pfd_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_suggested_pfd_info_s OpenAPI_suggested_pfd_info_t;
#include "domain_name_protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_suggested_pfd_info_s {
    char *pfd_id;
    OpenAPI_list_t *ip3_tuple_list;
    OpenAPI_list_t *urls;
    OpenAPI_list_t *domain_names;
    OpenAPI_domain_name_protocol_e dn_protocol;
    bool is_pfd_confidence;
    int pfd_confidence;
};

OpenAPI_suggested_pfd_info_t *OpenAPI_suggested_pfd_info_create(
    char *pfd_id,
    OpenAPI_list_t *ip3_tuple_list,
    OpenAPI_list_t *urls,
    OpenAPI_list_t *domain_names,
    OpenAPI_domain_name_protocol_e dn_protocol,
    bool is_pfd_confidence,
    int pfd_confidence
);
void OpenAPI_suggested_pfd_info_free(OpenAPI_suggested_pfd_info_t *suggested_pfd_info);
OpenAPI_suggested_pfd_info_t *OpenAPI_suggested_pfd_info_parseFromJSON(cJSON *suggested_pfd_infoJSON);
cJSON *OpenAPI_suggested_pfd_info_convertToJSON(OpenAPI_suggested_pfd_info_t *suggested_pfd_info);
OpenAPI_suggested_pfd_info_t *OpenAPI_suggested_pfd_info_copy(OpenAPI_suggested_pfd_info_t *dst, OpenAPI_suggested_pfd_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_suggested_pfd_info_H_ */

