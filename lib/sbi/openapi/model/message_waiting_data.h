/*
 * message_waiting_data.h
 *
 * Message Waiting Data list.
 */

#ifndef _OpenAPI_message_waiting_data_H_
#define _OpenAPI_message_waiting_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "smsc_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_message_waiting_data_s OpenAPI_message_waiting_data_t;
typedef struct OpenAPI_message_waiting_data_s {
    OpenAPI_list_t *mwd_list;
} OpenAPI_message_waiting_data_t;

OpenAPI_message_waiting_data_t *OpenAPI_message_waiting_data_create(
    OpenAPI_list_t *mwd_list
);
void OpenAPI_message_waiting_data_free(OpenAPI_message_waiting_data_t *message_waiting_data);
OpenAPI_message_waiting_data_t *OpenAPI_message_waiting_data_parseFromJSON(cJSON *message_waiting_dataJSON);
cJSON *OpenAPI_message_waiting_data_convertToJSON(OpenAPI_message_waiting_data_t *message_waiting_data);
OpenAPI_message_waiting_data_t *OpenAPI_message_waiting_data_copy(OpenAPI_message_waiting_data_t *dst, OpenAPI_message_waiting_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_message_waiting_data_H_ */

