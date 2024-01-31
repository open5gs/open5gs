/*
 * sms_record_data.h
 *
 * Represents the information sent within a request message of the UplinkSMS service operation, for delivering SMS payload.
 */

#ifndef _OpenAPI_sms_record_data_H_
#define _OpenAPI_sms_record_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_type.h"
#include "ref_to_binary_data.h"
#include "user_location.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sms_record_data_s OpenAPI_sms_record_data_t;
typedef struct OpenAPI_sms_record_data_s {
    char *sms_record_id;
    struct OpenAPI_ref_to_binary_data_s *sms_payload;
    OpenAPI_access_type_e access_type;
    char *gpsi;
    char *pei;
    struct OpenAPI_user_location_s *ue_location;
    char *ue_time_zone;
} OpenAPI_sms_record_data_t;

OpenAPI_sms_record_data_t *OpenAPI_sms_record_data_create(
    char *sms_record_id,
    OpenAPI_ref_to_binary_data_t *sms_payload,
    OpenAPI_access_type_e access_type,
    char *gpsi,
    char *pei,
    OpenAPI_user_location_t *ue_location,
    char *ue_time_zone
);
void OpenAPI_sms_record_data_free(OpenAPI_sms_record_data_t *sms_record_data);
OpenAPI_sms_record_data_t *OpenAPI_sms_record_data_parseFromJSON(cJSON *sms_record_dataJSON);
cJSON *OpenAPI_sms_record_data_convertToJSON(OpenAPI_sms_record_data_t *sms_record_data);
OpenAPI_sms_record_data_t *OpenAPI_sms_record_data_copy(OpenAPI_sms_record_data_t *dst, OpenAPI_sms_record_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sms_record_data_H_ */

