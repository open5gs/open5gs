/*
 * pcf_selection_assistance_info_1.h
 *
 * 
 */

#ifndef _OpenAPI_pcf_selection_assistance_info_1_H_
#define _OpenAPI_pcf_selection_assistance_info_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pcf_selection_assistance_info_1_s OpenAPI_pcf_selection_assistance_info_1_t;
typedef struct OpenAPI_pcf_selection_assistance_info_1_s {
    char *dnn;
    struct OpenAPI_snssai_s *single_nssai;
} OpenAPI_pcf_selection_assistance_info_1_t;

OpenAPI_pcf_selection_assistance_info_1_t *OpenAPI_pcf_selection_assistance_info_1_create(
    char *dnn,
    OpenAPI_snssai_t *single_nssai
);
void OpenAPI_pcf_selection_assistance_info_1_free(OpenAPI_pcf_selection_assistance_info_1_t *pcf_selection_assistance_info_1);
OpenAPI_pcf_selection_assistance_info_1_t *OpenAPI_pcf_selection_assistance_info_1_parseFromJSON(cJSON *pcf_selection_assistance_info_1JSON);
cJSON *OpenAPI_pcf_selection_assistance_info_1_convertToJSON(OpenAPI_pcf_selection_assistance_info_1_t *pcf_selection_assistance_info_1);
OpenAPI_pcf_selection_assistance_info_1_t *OpenAPI_pcf_selection_assistance_info_1_copy(OpenAPI_pcf_selection_assistance_info_1_t *dst, OpenAPI_pcf_selection_assistance_info_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pcf_selection_assistance_info_1_H_ */

