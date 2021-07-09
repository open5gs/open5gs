/*
 * dl_data_delivery_status.h
 *
 * 
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

typedef enum { OpenAPI_dl_data_delivery_status_NULL = 0, OpenAPI_dl_data_delivery_status_BUFFERED, OpenAPI_dl_data_delivery_status_TRANSMITTED, OpenAPI_dl_data_delivery_status_DISCARDED } OpenAPI_dl_data_delivery_status_e;

char* OpenAPI_dl_data_delivery_status_ToString(OpenAPI_dl_data_delivery_status_e dl_data_delivery_status);

OpenAPI_dl_data_delivery_status_e OpenAPI_dl_data_delivery_status_FromString(char* dl_data_delivery_status);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dl_data_delivery_status_H_ */

