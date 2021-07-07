/*
 * acceptable_service_info.h
 *
 * Indicates the maximum bandwidth that shall be authorized by the PCF.
 */

#ifndef _OpenAPI_acceptable_service_info_H_
#define _OpenAPI_acceptable_service_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "media_component.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_acceptable_service_info_s OpenAPI_acceptable_service_info_t;
typedef struct OpenAPI_acceptable_service_info_s {
    OpenAPI_list_t* acc_bw_med_comps;
    char *mar_bw_ul;
    char *mar_bw_dl;
} OpenAPI_acceptable_service_info_t;

OpenAPI_acceptable_service_info_t *OpenAPI_acceptable_service_info_create(
    OpenAPI_list_t* acc_bw_med_comps,
    char *mar_bw_ul,
    char *mar_bw_dl
);
void OpenAPI_acceptable_service_info_free(OpenAPI_acceptable_service_info_t *acceptable_service_info);
OpenAPI_acceptable_service_info_t *OpenAPI_acceptable_service_info_parseFromJSON(cJSON *acceptable_service_infoJSON);
cJSON *OpenAPI_acceptable_service_info_convertToJSON(OpenAPI_acceptable_service_info_t *acceptable_service_info);
OpenAPI_acceptable_service_info_t *OpenAPI_acceptable_service_info_copy(OpenAPI_acceptable_service_info_t *dst, OpenAPI_acceptable_service_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_acceptable_service_info_H_ */

