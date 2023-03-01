/*
 * tscai_input_container.h
 *
 * Indicates TSC Traffic pattern.
 */

#ifndef _OpenAPI_tscai_input_container_H_
#define _OpenAPI_tscai_input_container_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_tscai_input_container_s OpenAPI_tscai_input_container_t;
typedef struct OpenAPI_tscai_input_container_s {
    bool is_periodicity;
    int periodicity;
    char *burst_arrival_time;
    bool is_sur_time_in_num_msg;
    int sur_time_in_num_msg;
    bool is_sur_time_in_time;
    int sur_time_in_time;
} OpenAPI_tscai_input_container_t;

OpenAPI_tscai_input_container_t *OpenAPI_tscai_input_container_create(
    bool is_periodicity,
    int periodicity,
    char *burst_arrival_time,
    bool is_sur_time_in_num_msg,
    int sur_time_in_num_msg,
    bool is_sur_time_in_time,
    int sur_time_in_time
);
void OpenAPI_tscai_input_container_free(OpenAPI_tscai_input_container_t *tscai_input_container);
OpenAPI_tscai_input_container_t *OpenAPI_tscai_input_container_parseFromJSON(cJSON *tscai_input_containerJSON);
cJSON *OpenAPI_tscai_input_container_convertToJSON(OpenAPI_tscai_input_container_t *tscai_input_container);
OpenAPI_tscai_input_container_t *OpenAPI_tscai_input_container_copy(OpenAPI_tscai_input_container_t *dst, OpenAPI_tscai_input_container_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_tscai_input_container_H_ */

