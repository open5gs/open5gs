/*
 * nwdaf_data.h
 *
 * Indicates the list of Analytic ID(s) per NWDAF instance ID used for the PDU Session consumed by the SMF.
 */

#ifndef _OpenAPI_nwdaf_data_H_
#define _OpenAPI_nwdaf_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "nwdaf_event.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nwdaf_data_s OpenAPI_nwdaf_data_t;
typedef struct OpenAPI_nwdaf_data_s {
    char *nwdaf_instance_id;
    OpenAPI_list_t *nwdaf_events;
} OpenAPI_nwdaf_data_t;

OpenAPI_nwdaf_data_t *OpenAPI_nwdaf_data_create(
    char *nwdaf_instance_id,
    OpenAPI_list_t *nwdaf_events
);
void OpenAPI_nwdaf_data_free(OpenAPI_nwdaf_data_t *nwdaf_data);
OpenAPI_nwdaf_data_t *OpenAPI_nwdaf_data_parseFromJSON(cJSON *nwdaf_dataJSON);
cJSON *OpenAPI_nwdaf_data_convertToJSON(OpenAPI_nwdaf_data_t *nwdaf_data);
OpenAPI_nwdaf_data_t *OpenAPI_nwdaf_data_copy(OpenAPI_nwdaf_data_t *dst, OpenAPI_nwdaf_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nwdaf_data_H_ */

