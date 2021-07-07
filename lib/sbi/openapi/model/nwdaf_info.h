/*
 * nwdaf_info.h
 *
 * Information of a NWDAF NF Instance
 */

#ifndef _OpenAPI_nwdaf_info_H_
#define _OpenAPI_nwdaf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "event_id.h"
#include "nwdaf_event.h"
#include "tai.h"
#include "tai_range.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nwdaf_info_s OpenAPI_nwdaf_info_t;
typedef struct OpenAPI_nwdaf_info_s {
    OpenAPI_list_t *event_ids;
    OpenAPI_list_t *nwdaf_events;
    OpenAPI_list_t *tai_list;
    OpenAPI_list_t *tai_range_list;
} OpenAPI_nwdaf_info_t;

OpenAPI_nwdaf_info_t *OpenAPI_nwdaf_info_create(
    OpenAPI_list_t *event_ids,
    OpenAPI_list_t *nwdaf_events,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list
);
void OpenAPI_nwdaf_info_free(OpenAPI_nwdaf_info_t *nwdaf_info);
OpenAPI_nwdaf_info_t *OpenAPI_nwdaf_info_parseFromJSON(cJSON *nwdaf_infoJSON);
cJSON *OpenAPI_nwdaf_info_convertToJSON(OpenAPI_nwdaf_info_t *nwdaf_info);
OpenAPI_nwdaf_info_t *OpenAPI_nwdaf_info_copy(OpenAPI_nwdaf_info_t *dst, OpenAPI_nwdaf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nwdaf_info_H_ */

