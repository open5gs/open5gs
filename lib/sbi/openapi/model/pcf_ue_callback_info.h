/*
 * pcf_ue_callback_info.h
 *
 * Contains the PCF for the UE information necessary for the PCF for the PDU session to send  SM Policy Association Establishment and Termination events. 
 */

#ifndef _OpenAPI_pcf_ue_callback_info_H_
#define _OpenAPI_pcf_ue_callback_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pcf_ue_callback_info_s OpenAPI_pcf_ue_callback_info_t;
typedef struct OpenAPI_pcf_ue_callback_info_s {
    char *callback_uri;
    char *binding_info;
} OpenAPI_pcf_ue_callback_info_t;

OpenAPI_pcf_ue_callback_info_t *OpenAPI_pcf_ue_callback_info_create(
    char *callback_uri,
    char *binding_info
);
void OpenAPI_pcf_ue_callback_info_free(OpenAPI_pcf_ue_callback_info_t *pcf_ue_callback_info);
OpenAPI_pcf_ue_callback_info_t *OpenAPI_pcf_ue_callback_info_parseFromJSON(cJSON *pcf_ue_callback_infoJSON);
cJSON *OpenAPI_pcf_ue_callback_info_convertToJSON(OpenAPI_pcf_ue_callback_info_t *pcf_ue_callback_info);
OpenAPI_pcf_ue_callback_info_t *OpenAPI_pcf_ue_callback_info_copy(OpenAPI_pcf_ue_callback_info_t *dst, OpenAPI_pcf_ue_callback_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pcf_ue_callback_info_H_ */

