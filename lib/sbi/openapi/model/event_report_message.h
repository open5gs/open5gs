/*
 * event_report_message.h
 *
 * Indicates an event report message.
 */

#ifndef _OpenAPI_event_report_message_H_
#define _OpenAPI_event_report_message_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "event_class.h"
#include "ref_to_binary_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_event_report_message_s OpenAPI_event_report_message_t;
typedef struct OpenAPI_event_report_message_s {
    struct OpenAPI_event_class_s *event_class;
    struct OpenAPI_ref_to_binary_data_s *event_content;
} OpenAPI_event_report_message_t;

OpenAPI_event_report_message_t *OpenAPI_event_report_message_create(
    OpenAPI_event_class_t *event_class,
    OpenAPI_ref_to_binary_data_t *event_content
);
void OpenAPI_event_report_message_free(OpenAPI_event_report_message_t *event_report_message);
OpenAPI_event_report_message_t *OpenAPI_event_report_message_parseFromJSON(cJSON *event_report_messageJSON);
cJSON *OpenAPI_event_report_message_convertToJSON(OpenAPI_event_report_message_t *event_report_message);
OpenAPI_event_report_message_t *OpenAPI_event_report_message_copy(OpenAPI_event_report_message_t *dst, OpenAPI_event_report_message_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_event_report_message_H_ */

