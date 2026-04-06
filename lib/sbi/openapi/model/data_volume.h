/*
 * data_volume.h
 *
 * Data Volume including UL/DL.
 */

#ifndef _OpenAPI_data_volume_H_
#define _OpenAPI_data_volume_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_data_volume_s OpenAPI_data_volume_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_data_volume_s {
    bool is_uplink_volume;
    long uplink_volume;
    bool is_downlink_volume;
    long downlink_volume;
};

OpenAPI_data_volume_t *OpenAPI_data_volume_create(
    bool is_uplink_volume,
    long uplink_volume,
    bool is_downlink_volume,
    long downlink_volume
);
void OpenAPI_data_volume_free(OpenAPI_data_volume_t *data_volume);
OpenAPI_data_volume_t *OpenAPI_data_volume_parseFromJSON(cJSON *data_volumeJSON);
cJSON *OpenAPI_data_volume_convertToJSON(OpenAPI_data_volume_t *data_volume);
OpenAPI_data_volume_t *OpenAPI_data_volume_copy(OpenAPI_data_volume_t *dst, OpenAPI_data_volume_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_data_volume_H_ */

