/*
 * ebi_arp_mapping.h
 *
 *
 */

#ifndef _OpenAPI_ebi_arp_mapping_H_
#define _OpenAPI_ebi_arp_mapping_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "arp.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ebi_arp_mapping_s OpenAPI_ebi_arp_mapping_t;
typedef struct OpenAPI_ebi_arp_mapping_s {
    int eps_bearer_id;
    struct OpenAPI_arp_s *arp;
} OpenAPI_ebi_arp_mapping_t;

OpenAPI_ebi_arp_mapping_t *OpenAPI_ebi_arp_mapping_create(
    int eps_bearer_id,
    OpenAPI_arp_t *arp
    );
void OpenAPI_ebi_arp_mapping_free(OpenAPI_ebi_arp_mapping_t *ebi_arp_mapping);
OpenAPI_ebi_arp_mapping_t *OpenAPI_ebi_arp_mapping_parseFromJSON(cJSON *ebi_arp_mappingJSON);
cJSON *OpenAPI_ebi_arp_mapping_convertToJSON(OpenAPI_ebi_arp_mapping_t *ebi_arp_mapping);
OpenAPI_ebi_arp_mapping_t *OpenAPI_ebi_arp_mapping_copy(OpenAPI_ebi_arp_mapping_t *dst, OpenAPI_ebi_arp_mapping_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ebi_arp_mapping_H_ */

