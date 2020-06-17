/*
 * time_window.h
 *
 *
 */

#ifndef _OpenAPI_time_window_H_
#define _OpenAPI_time_window_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_time_window_s OpenAPI_time_window_t;
typedef struct OpenAPI_time_window_s {
    char *start_time;
    char *stop_time;
} OpenAPI_time_window_t;

OpenAPI_time_window_t *OpenAPI_time_window_create(
    char *start_time,
    char *stop_time
    );
void OpenAPI_time_window_free(OpenAPI_time_window_t *time_window);
OpenAPI_time_window_t *OpenAPI_time_window_parseFromJSON(cJSON *time_windowJSON);
cJSON *OpenAPI_time_window_convertToJSON(OpenAPI_time_window_t *time_window);
OpenAPI_time_window_t *OpenAPI_time_window_copy(OpenAPI_time_window_t *dst, OpenAPI_time_window_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_time_window_H_ */

