/*
 * dl_data_delivery_status.h
 *
 * Possible values are - BUFFERED: The first downlink data is buffered with extended buffering matching the source of the downlink traffic. - TRANSMITTED: The first downlink data matching the source of the downlink traffic is transmitted after previous buffering or discarding of corresponding packet(s) because the UE of the PDU Session becomes ACTIVE, and buffered data can be delivered to UE. - DISCARDED: The first downlink data matching the source of the downlink traffic is discarded because the Extended Buffering time, as determined by the SMF, expires or the amount of downlink data to be buffered is exceeded.
 */

#ifndef _OpenAPI_dl_data_delivery_status_H_
#define _OpenAPI_dl_data_delivery_status_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_dl_data_delivery_status_s OpenAPI_dl_data_delivery_status_t;
typedef struct OpenAPI_dl_data_delivery_status_s {
} OpenAPI_dl_data_delivery_status_t;

OpenAPI_dl_data_delivery_status_t *OpenAPI_dl_data_delivery_status_create(
    );
void OpenAPI_dl_data_delivery_status_free(OpenAPI_dl_data_delivery_status_t *dl_data_delivery_status);
OpenAPI_dl_data_delivery_status_t *OpenAPI_dl_data_delivery_status_parseFromJSON(cJSON *dl_data_delivery_statusJSON);
cJSON *OpenAPI_dl_data_delivery_status_convertToJSON(OpenAPI_dl_data_delivery_status_t *dl_data_delivery_status);
OpenAPI_dl_data_delivery_status_t *OpenAPI_dl_data_delivery_status_copy(OpenAPI_dl_data_delivery_status_t *dst, OpenAPI_dl_data_delivery_status_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dl_data_delivery_status_H_ */

