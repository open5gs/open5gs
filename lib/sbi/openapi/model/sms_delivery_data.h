/*
 * sms_delivery_data.h
 *
 * Information within response message invoking MtForwardSm service operation, for delivering MT SMS Delivery Report. 
 */

#ifndef _OpenAPI_sms_delivery_data_H_
#define _OpenAPI_sms_delivery_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ref_to_binary_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sms_delivery_data_s OpenAPI_sms_delivery_data_t;
typedef struct OpenAPI_sms_delivery_data_s {
    struct OpenAPI_ref_to_binary_data_s *sms_payload;
} OpenAPI_sms_delivery_data_t;

OpenAPI_sms_delivery_data_t *OpenAPI_sms_delivery_data_create(
    OpenAPI_ref_to_binary_data_t *sms_payload
);
void OpenAPI_sms_delivery_data_free(OpenAPI_sms_delivery_data_t *sms_delivery_data);
OpenAPI_sms_delivery_data_t *OpenAPI_sms_delivery_data_parseFromJSON(cJSON *sms_delivery_dataJSON);
cJSON *OpenAPI_sms_delivery_data_convertToJSON(OpenAPI_sms_delivery_data_t *sms_delivery_data);
OpenAPI_sms_delivery_data_t *OpenAPI_sms_delivery_data_copy(OpenAPI_sms_delivery_data_t *dst, OpenAPI_sms_delivery_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sms_delivery_data_H_ */

