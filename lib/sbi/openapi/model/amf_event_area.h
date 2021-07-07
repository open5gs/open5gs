/*
 * amf_event_area.h
 *
 * 
 */

#ifndef _OpenAPI_amf_event_area_H_
#define _OpenAPI_amf_event_area_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ladn_info.h"
#include "presence_info.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_amf_event_area_s OpenAPI_amf_event_area_t;
typedef struct OpenAPI_amf_event_area_s {
    struct OpenAPI_presence_info_s *presence_info;
    struct OpenAPI_ladn_info_s *ladn_info;
    struct OpenAPI_snssai_s *s_nssai;
    char *nsi_id;
} OpenAPI_amf_event_area_t;

OpenAPI_amf_event_area_t *OpenAPI_amf_event_area_create(
    OpenAPI_presence_info_t *presence_info,
    OpenAPI_ladn_info_t *ladn_info,
    OpenAPI_snssai_t *s_nssai,
    char *nsi_id
);
void OpenAPI_amf_event_area_free(OpenAPI_amf_event_area_t *amf_event_area);
OpenAPI_amf_event_area_t *OpenAPI_amf_event_area_parseFromJSON(cJSON *amf_event_areaJSON);
cJSON *OpenAPI_amf_event_area_convertToJSON(OpenAPI_amf_event_area_t *amf_event_area);
OpenAPI_amf_event_area_t *OpenAPI_amf_event_area_copy(OpenAPI_amf_event_area_t *dst, OpenAPI_amf_event_area_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_amf_event_area_H_ */

