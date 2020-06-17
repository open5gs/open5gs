/*
 * nrppa_information.h
 *
 *
 */

#ifndef _OpenAPI_nrppa_information_H_
#define _OpenAPI_nrppa_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "n2_info_content.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nrppa_information_s OpenAPI_nrppa_information_t;
typedef struct OpenAPI_nrppa_information_s {
    char *nf_id;
    struct OpenAPI_n2_info_content_s *nrppa_pdu;
    char *service_instance_id;
} OpenAPI_nrppa_information_t;

OpenAPI_nrppa_information_t *OpenAPI_nrppa_information_create(
    char *nf_id,
    OpenAPI_n2_info_content_t *nrppa_pdu,
    char *service_instance_id
    );
void OpenAPI_nrppa_information_free(OpenAPI_nrppa_information_t *nrppa_information);
OpenAPI_nrppa_information_t *OpenAPI_nrppa_information_parseFromJSON(cJSON *nrppa_informationJSON);
cJSON *OpenAPI_nrppa_information_convertToJSON(OpenAPI_nrppa_information_t *nrppa_information);
OpenAPI_nrppa_information_t *OpenAPI_nrppa_information_copy(OpenAPI_nrppa_information_t *dst, OpenAPI_nrppa_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nrppa_information_H_ */

