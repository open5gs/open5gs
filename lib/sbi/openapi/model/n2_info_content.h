/*
 * n2_info_content.h
 *
 * Represents a transparent N2 information content to be relayed by AMF
 */

#ifndef _OpenAPI_n2_info_content_H_
#define _OpenAPI_n2_info_content_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ngap_ie_type.h"
#include "ref_to_binary_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_n2_info_content_s OpenAPI_n2_info_content_t;
typedef struct OpenAPI_n2_info_content_s {
    bool is_ngap_message_type;
    int ngap_message_type;
    OpenAPI_ngap_ie_type_e ngap_ie_type;
    struct OpenAPI_ref_to_binary_data_s *ngap_data;


        //added by sairisan
    uint16_t message_identifier;
    uint16_t serial_number;
    // uint8_t warning_type;
    uint32_t no_of_tai;        
    uint32_t repetition_period;
    uint32_t number_of_broadcast;
    uint8_t data_coding_scheme;
    uint32_t message_length;
    uint8_t message_contents[1024]; /* TODO: max 9600 */

    
} OpenAPI_n2_info_content_t;

OpenAPI_n2_info_content_t *OpenAPI_n2_info_content_create(
    bool is_ngap_message_type,
    int ngap_message_type,
    OpenAPI_ngap_ie_type_e ngap_ie_type,
    OpenAPI_ref_to_binary_data_t *ngap_data
);
void OpenAPI_n2_info_content_free(OpenAPI_n2_info_content_t *n2_info_content);
OpenAPI_n2_info_content_t *OpenAPI_n2_info_content_parseFromJSON(cJSON *n2_info_contentJSON);
cJSON *OpenAPI_n2_info_content_convertToJSON(OpenAPI_n2_info_content_t *n2_info_content);
OpenAPI_n2_info_content_t *OpenAPI_n2_info_content_copy(OpenAPI_n2_info_content_t *dst, OpenAPI_n2_info_content_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n2_info_content_H_ */

