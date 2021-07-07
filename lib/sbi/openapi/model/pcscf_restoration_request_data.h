/*
 * pcscf_restoration_request_data.h
 *
 * Indicates P-CSCF restoration.
 */

#ifndef _OpenAPI_pcscf_restoration_request_data_H_
#define _OpenAPI_pcscf_restoration_request_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pcscf_restoration_request_data_s OpenAPI_pcscf_restoration_request_data_t;
typedef struct OpenAPI_pcscf_restoration_request_data_s {
    char *dnn;
    char *ip_domain;
    struct OpenAPI_snssai_s *slice_info;
    char *supi;
    char *ue_ipv4;
    char *ue_ipv6;
} OpenAPI_pcscf_restoration_request_data_t;

OpenAPI_pcscf_restoration_request_data_t *OpenAPI_pcscf_restoration_request_data_create(
    char *dnn,
    char *ip_domain,
    OpenAPI_snssai_t *slice_info,
    char *supi,
    char *ue_ipv4,
    char *ue_ipv6
);
void OpenAPI_pcscf_restoration_request_data_free(OpenAPI_pcscf_restoration_request_data_t *pcscf_restoration_request_data);
OpenAPI_pcscf_restoration_request_data_t *OpenAPI_pcscf_restoration_request_data_parseFromJSON(cJSON *pcscf_restoration_request_dataJSON);
cJSON *OpenAPI_pcscf_restoration_request_data_convertToJSON(OpenAPI_pcscf_restoration_request_data_t *pcscf_restoration_request_data);
OpenAPI_pcscf_restoration_request_data_t *OpenAPI_pcscf_restoration_request_data_copy(OpenAPI_pcscf_restoration_request_data_t *dst, OpenAPI_pcscf_restoration_request_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pcscf_restoration_request_data_H_ */

