/*
 * upf_information.h
 *
 * Represents the ID/address/FQDN of the UPF.
 */

#ifndef _OpenAPI_upf_information_H_
#define _OpenAPI_upf_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_upf_information_s OpenAPI_upf_information_t;
#include "addr_fqdn.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_upf_information_s {
    char *upf_id;
    struct OpenAPI_addr_fqdn_s *upf_addr;
};

OpenAPI_upf_information_t *OpenAPI_upf_information_create(
    char *upf_id,
    OpenAPI_addr_fqdn_t *upf_addr
);
void OpenAPI_upf_information_free(OpenAPI_upf_information_t *upf_information);
OpenAPI_upf_information_t *OpenAPI_upf_information_parseFromJSON(cJSON *upf_informationJSON);
cJSON *OpenAPI_upf_information_convertToJSON(OpenAPI_upf_information_t *upf_information);
OpenAPI_upf_information_t *OpenAPI_upf_information_copy(OpenAPI_upf_information_t *dst, OpenAPI_upf_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_upf_information_H_ */

