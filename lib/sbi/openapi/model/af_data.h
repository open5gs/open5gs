/*
 * af_data.h
 *
 * Contains information supported by the trusted AF
 */

#ifndef _OpenAPI_af_data_H_
#define _OpenAPI_af_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_af_data_s OpenAPI_af_data_t;
#include "af_event.h"
#include "tai.h"
#include "tai_range.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_af_data_s {
    OpenAPI_list_t *af_events;
    OpenAPI_list_t *tai_list;
    OpenAPI_list_t *tai_range_list;
};

OpenAPI_af_data_t *OpenAPI_af_data_create(
    OpenAPI_list_t *af_events,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list
);
void OpenAPI_af_data_free(OpenAPI_af_data_t *af_data);
OpenAPI_af_data_t *OpenAPI_af_data_parseFromJSON(cJSON *af_dataJSON);
cJSON *OpenAPI_af_data_convertToJSON(OpenAPI_af_data_t *af_data);
OpenAPI_af_data_t *OpenAPI_af_data_copy(OpenAPI_af_data_t *dst, OpenAPI_af_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_af_data_H_ */

