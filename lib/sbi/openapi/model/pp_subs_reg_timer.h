/*
 * pp_subs_reg_timer.h
 *
 *
 */

#ifndef _OpenAPI_pp_subs_reg_timer_H_
#define _OpenAPI_pp_subs_reg_timer_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pp_subs_reg_timer_s OpenAPI_pp_subs_reg_timer_t;
typedef struct OpenAPI_pp_subs_reg_timer_s {
    int subs_reg_timer;
    char *af_instance_id;
    int reference_id;
    char *validity_time;
} OpenAPI_pp_subs_reg_timer_t;

OpenAPI_pp_subs_reg_timer_t *OpenAPI_pp_subs_reg_timer_create(
    int subs_reg_timer,
    char *af_instance_id,
    int reference_id,
    char *validity_time
    );
void OpenAPI_pp_subs_reg_timer_free(OpenAPI_pp_subs_reg_timer_t *pp_subs_reg_timer);
OpenAPI_pp_subs_reg_timer_t *OpenAPI_pp_subs_reg_timer_parseFromJSON(cJSON *pp_subs_reg_timerJSON);
cJSON *OpenAPI_pp_subs_reg_timer_convertToJSON(OpenAPI_pp_subs_reg_timer_t *pp_subs_reg_timer);
OpenAPI_pp_subs_reg_timer_t *OpenAPI_pp_subs_reg_timer_copy(OpenAPI_pp_subs_reg_timer_t *dst, OpenAPI_pp_subs_reg_timer_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pp_subs_reg_timer_H_ */

