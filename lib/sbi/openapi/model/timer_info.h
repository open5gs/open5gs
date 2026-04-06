/*
 * timer_info.h
 *
 * The signalling storm analytics requirement information.
 */

#ifndef _OpenAPI_timer_info_H_
#define _OpenAPI_timer_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_timer_info_s OpenAPI_timer_info_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_timer_info_s {
    char *timer_type;
    bool is_timer_dur;
    int timer_dur;
};

OpenAPI_timer_info_t *OpenAPI_timer_info_create(
    char *timer_type,
    bool is_timer_dur,
    int timer_dur
);
void OpenAPI_timer_info_free(OpenAPI_timer_info_t *timer_info);
OpenAPI_timer_info_t *OpenAPI_timer_info_parseFromJSON(cJSON *timer_infoJSON);
cJSON *OpenAPI_timer_info_convertToJSON(OpenAPI_timer_info_t *timer_info);
OpenAPI_timer_info_t *OpenAPI_timer_info_copy(OpenAPI_timer_info_t *dst, OpenAPI_timer_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_timer_info_H_ */

