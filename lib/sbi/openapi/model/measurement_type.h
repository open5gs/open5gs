/*
 * measurement_type.h
 *
 * 
 */

#ifndef _OpenAPI_measurement_type_H_
#define _OpenAPI_measurement_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_measurement_type_NULL = 0, OpenAPI_measurement_type_GTP_DELAYDLPSAUPFUEMEAN_SNSSAI_QFI, OpenAPI_measurement_type_GTP_DELAYULPSAUPFUEMEANEXCD1_SNSSAI_QFI, OpenAPI_measurement_type_GTP_DELAYDLPSAUPFUEMEANINCD1_SNSSAI_QFI, OpenAPI_measurement_type_GTP_DELAYULPSAUPFNGRANMEAN_SNSSAI_QFI, OpenAPI_measurement_type_GTP_DELAYDLPSAUPFNGRANMEAN_SNSSAI_QFI, OpenAPI_measurement_type_GTP_RTTDELAYPSAUPFNGRANMEAN, OpenAPI_measurement_type_GTP_RTTDELAYPSAUPFNGRANMEAN_SNSSAI, OpenAPI_measurement_type_GTP_RTTDELAYPSAUPFNGRANMEAN_QFI, OpenAPI_measurement_type_GTP_RTTDELAYPSAUPFNGRANMEAN_SNSSAI_QFI } OpenAPI_measurement_type_e;

char* OpenAPI_measurement_type_ToString(OpenAPI_measurement_type_e measurement_type);

OpenAPI_measurement_type_e OpenAPI_measurement_type_FromString(char* measurement_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_measurement_type_H_ */

