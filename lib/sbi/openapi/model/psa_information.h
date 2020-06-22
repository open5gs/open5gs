/*
 * psa_information.h
 *
 *
 */

#ifndef _OpenAPI_psa_information_H_
#define _OpenAPI_psa_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "psa_indication.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_psa_information_s OpenAPI_psa_information_t;
typedef struct OpenAPI_psa_information_s {
    OpenAPI_psa_indication_e psa_ind;
    OpenAPI_list_t *dnai_list;
    char *ue_ipv6_prefix;
    char *psa_upf_id;
} OpenAPI_psa_information_t;

OpenAPI_psa_information_t *OpenAPI_psa_information_create(
    OpenAPI_psa_indication_e psa_ind,
    OpenAPI_list_t *dnai_list,
    char *ue_ipv6_prefix,
    char *psa_upf_id
    );
void OpenAPI_psa_information_free(OpenAPI_psa_information_t *psa_information);
OpenAPI_psa_information_t *OpenAPI_psa_information_parseFromJSON(cJSON *psa_informationJSON);
cJSON *OpenAPI_psa_information_convertToJSON(OpenAPI_psa_information_t *psa_information);
OpenAPI_psa_information_t *OpenAPI_psa_information_copy(OpenAPI_psa_information_t *dst, OpenAPI_psa_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_psa_information_H_ */

