/*
 * xr_device_with2_rx.h
 *
 * Information of the XR Device with 2Rx
 */

#ifndef _OpenAPI_xr_device_with2_rx_H_
#define _OpenAPI_xr_device_with2_rx_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_xr_device_with2_rx_NULL = 0, OpenAPI_xr_device_with2_rx__true } OpenAPI_xr_device_with2_rx_e;

char* OpenAPI_xr_device_with2_rx_ToString(OpenAPI_xr_device_with2_rx_e xr_device_with2_rx);

OpenAPI_xr_device_with2_rx_e OpenAPI_xr_device_with2_rx_FromString(char* xr_device_with2_rx);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_xr_device_with2_rx_H_ */

