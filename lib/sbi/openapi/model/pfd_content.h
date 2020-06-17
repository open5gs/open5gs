/*
 * pfd_content.h
 *
 *
 */

#ifndef _OpenAPI_pfd_content_H_
#define _OpenAPI_pfd_content_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "domain_name_protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pfd_content_s OpenAPI_pfd_content_t;
typedef struct OpenAPI_pfd_content_s {
    char *pfd_id;
    OpenAPI_list_t *flow_descriptions;
    OpenAPI_list_t *urls;
    OpenAPI_list_t *domain_names;
    struct OpenAPI_domain_name_protocol_s *dn_protocol;
} OpenAPI_pfd_content_t;

OpenAPI_pfd_content_t *OpenAPI_pfd_content_create(
    char *pfd_id,
    OpenAPI_list_t *flow_descriptions,
    OpenAPI_list_t *urls,
    OpenAPI_list_t *domain_names,
    OpenAPI_domain_name_protocol_t *dn_protocol
    );
void OpenAPI_pfd_content_free(OpenAPI_pfd_content_t *pfd_content);
OpenAPI_pfd_content_t *OpenAPI_pfd_content_parseFromJSON(cJSON *pfd_contentJSON);
cJSON *OpenAPI_pfd_content_convertToJSON(OpenAPI_pfd_content_t *pfd_content);
OpenAPI_pfd_content_t *OpenAPI_pfd_content_copy(OpenAPI_pfd_content_t *dst, OpenAPI_pfd_content_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pfd_content_H_ */

