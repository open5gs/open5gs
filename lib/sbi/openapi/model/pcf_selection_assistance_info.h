/*
 * pcf_selection_assistance_info.h
 *
 * 
 */

#ifndef _OpenAPI_pcf_selection_assistance_info_H_
#define _OpenAPI_pcf_selection_assistance_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pcf_selection_assistance_info_s OpenAPI_pcf_selection_assistance_info_t;
typedef struct OpenAPI_pcf_selection_assistance_info_s {
    char *dnn;
    struct OpenAPI_snssai_s *single_nssai;
} OpenAPI_pcf_selection_assistance_info_t;

OpenAPI_pcf_selection_assistance_info_t *OpenAPI_pcf_selection_assistance_info_create(
    char *dnn,
    OpenAPI_snssai_t *single_nssai
);
void OpenAPI_pcf_selection_assistance_info_free(OpenAPI_pcf_selection_assistance_info_t *pcf_selection_assistance_info);
OpenAPI_pcf_selection_assistance_info_t *OpenAPI_pcf_selection_assistance_info_parseFromJSON(cJSON *pcf_selection_assistance_infoJSON);
cJSON *OpenAPI_pcf_selection_assistance_info_convertToJSON(OpenAPI_pcf_selection_assistance_info_t *pcf_selection_assistance_info);
OpenAPI_pcf_selection_assistance_info_t *OpenAPI_pcf_selection_assistance_info_copy(OpenAPI_pcf_selection_assistance_info_t *dst, OpenAPI_pcf_selection_assistance_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pcf_selection_assistance_info_H_ */

