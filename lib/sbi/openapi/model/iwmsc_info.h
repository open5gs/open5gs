/*
 * iwmsc_info.h
 *
 * Information of an SMS-IWMSC NF Instance
 */

#ifndef _OpenAPI_iwmsc_info_H_
#define _OpenAPI_iwmsc_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "identity_range.h"
#include "supi_range.h"
#include "tai_range.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_iwmsc_info_s OpenAPI_iwmsc_info_t;
typedef struct OpenAPI_iwmsc_info_s {
    OpenAPI_list_t *msisdn_ranges;
    OpenAPI_list_t *supi_ranges;
    OpenAPI_list_t *tai_range_list;
    char *sc_number;
} OpenAPI_iwmsc_info_t;

OpenAPI_iwmsc_info_t *OpenAPI_iwmsc_info_create(
    OpenAPI_list_t *msisdn_ranges,
    OpenAPI_list_t *supi_ranges,
    OpenAPI_list_t *tai_range_list,
    char *sc_number
);
void OpenAPI_iwmsc_info_free(OpenAPI_iwmsc_info_t *iwmsc_info);
OpenAPI_iwmsc_info_t *OpenAPI_iwmsc_info_parseFromJSON(cJSON *iwmsc_infoJSON);
cJSON *OpenAPI_iwmsc_info_convertToJSON(OpenAPI_iwmsc_info_t *iwmsc_info);
OpenAPI_iwmsc_info_t *OpenAPI_iwmsc_info_copy(OpenAPI_iwmsc_info_t *dst, OpenAPI_iwmsc_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_iwmsc_info_H_ */

