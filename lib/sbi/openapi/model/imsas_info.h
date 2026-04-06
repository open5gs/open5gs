/*
 * imsas_info.h
 *
 * Information of an IMS AS NF Instance
 */

#ifndef _OpenAPI_imsas_info_H_
#define _OpenAPI_imsas_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_imsas_info_s OpenAPI_imsas_info_t;
#include "ims_event.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_imsas_info_s {
    OpenAPI_list_t *ims_event_list;
};

OpenAPI_imsas_info_t *OpenAPI_imsas_info_create(
    OpenAPI_list_t *ims_event_list
);
void OpenAPI_imsas_info_free(OpenAPI_imsas_info_t *imsas_info);
OpenAPI_imsas_info_t *OpenAPI_imsas_info_parseFromJSON(cJSON *imsas_infoJSON);
cJSON *OpenAPI_imsas_info_convertToJSON(OpenAPI_imsas_info_t *imsas_info);
OpenAPI_imsas_info_t *OpenAPI_imsas_info_copy(OpenAPI_imsas_info_t *dst, OpenAPI_imsas_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_imsas_info_H_ */

