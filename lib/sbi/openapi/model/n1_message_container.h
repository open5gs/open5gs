/*
 * n1_message_container.h
 *
 * N1 Message container
 */

#ifndef _OpenAPI_n1_message_container_H_
#define _OpenAPI_n1_message_container_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "n1_message_class.h"
#include "ref_to_binary_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_n1_message_container_s OpenAPI_n1_message_container_t;
typedef struct OpenAPI_n1_message_container_s {
    OpenAPI_n1_message_class_e n1_message_class;
    struct OpenAPI_ref_to_binary_data_s *n1_message_content;
    char *nf_id;
    char *service_instance_id;
} OpenAPI_n1_message_container_t;

OpenAPI_n1_message_container_t *OpenAPI_n1_message_container_create(
    OpenAPI_n1_message_class_e n1_message_class,
    OpenAPI_ref_to_binary_data_t *n1_message_content,
    char *nf_id,
    char *service_instance_id
);
void OpenAPI_n1_message_container_free(OpenAPI_n1_message_container_t *n1_message_container);
OpenAPI_n1_message_container_t *OpenAPI_n1_message_container_parseFromJSON(cJSON *n1_message_containerJSON);
cJSON *OpenAPI_n1_message_container_convertToJSON(OpenAPI_n1_message_container_t *n1_message_container);
OpenAPI_n1_message_container_t *OpenAPI_n1_message_container_copy(OpenAPI_n1_message_container_t *dst, OpenAPI_n1_message_container_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n1_message_container_H_ */

