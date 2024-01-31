/*
 * sms_record_delivery_data.h
 *
 * Represents information on the result of invoking the UplinkSMS service operation.
 */

#ifndef _OpenAPI_sms_record_delivery_data_H_
#define _OpenAPI_sms_record_delivery_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "sms_delivery_status.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sms_record_delivery_data_s OpenAPI_sms_record_delivery_data_t;
typedef struct OpenAPI_sms_record_delivery_data_s {
    char *sms_record_id;
    OpenAPI_sms_delivery_status_e delivery_status;
} OpenAPI_sms_record_delivery_data_t;

OpenAPI_sms_record_delivery_data_t *OpenAPI_sms_record_delivery_data_create(
    char *sms_record_id,
    OpenAPI_sms_delivery_status_e delivery_status
);
void OpenAPI_sms_record_delivery_data_free(OpenAPI_sms_record_delivery_data_t *sms_record_delivery_data);
OpenAPI_sms_record_delivery_data_t *OpenAPI_sms_record_delivery_data_parseFromJSON(cJSON *sms_record_delivery_dataJSON);
cJSON *OpenAPI_sms_record_delivery_data_convertToJSON(OpenAPI_sms_record_delivery_data_t *sms_record_delivery_data);
OpenAPI_sms_record_delivery_data_t *OpenAPI_sms_record_delivery_data_copy(OpenAPI_sms_record_delivery_data_t *dst, OpenAPI_sms_record_delivery_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sms_record_delivery_data_H_ */

