/*
 * pws_response_data.h
 *
 * 
 */

#ifndef _OpenAPI_pws_response_data_H_
#define _OpenAPI_pws_response_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "tai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pws_response_data_s OpenAPI_pws_response_data_t;
typedef struct OpenAPI_pws_response_data_s {
    int ngap_message_type;
    int serial_number;
    int message_identifier;
    OpenAPI_list_t *unknown_tai_list;
} OpenAPI_pws_response_data_t;

OpenAPI_pws_response_data_t *OpenAPI_pws_response_data_create(
    int ngap_message_type,
    int serial_number,
    int message_identifier,
    OpenAPI_list_t *unknown_tai_list
);
void OpenAPI_pws_response_data_free(OpenAPI_pws_response_data_t *pws_response_data);
OpenAPI_pws_response_data_t *OpenAPI_pws_response_data_parseFromJSON(cJSON *pws_response_dataJSON);
cJSON *OpenAPI_pws_response_data_convertToJSON(OpenAPI_pws_response_data_t *pws_response_data);
OpenAPI_pws_response_data_t *OpenAPI_pws_response_data_copy(OpenAPI_pws_response_data_t *dst, OpenAPI_pws_response_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pws_response_data_H_ */

