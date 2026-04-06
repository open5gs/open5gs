/*
 * condition_group.h
 *
 * List (array) of conditions (joined by the \&quot;and\&quot; or \&quot;or\&quot; logical relationship), under which an NF Instance with an NFStatus or NFServiceStatus value set to, \&quot;CANARY_RELEASE\&quot;, or with a \&quot;canaryRelease\&quot; attribute set to true, shall be selected by an NF Service Consumer. 
 */

#ifndef _OpenAPI_condition_group_H_
#define _OpenAPI_condition_group_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_condition_group_s OpenAPI_condition_group_t;
#include "selection_conditions.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_condition_group_s {
    OpenAPI_list_t *_and;
    OpenAPI_list_t *_or;
};

OpenAPI_condition_group_t *OpenAPI_condition_group_create(
    OpenAPI_list_t *_and,
    OpenAPI_list_t *_or
);
void OpenAPI_condition_group_free(OpenAPI_condition_group_t *condition_group);
OpenAPI_condition_group_t *OpenAPI_condition_group_parseFromJSON(cJSON *condition_groupJSON);
cJSON *OpenAPI_condition_group_convertToJSON(OpenAPI_condition_group_t *condition_group);
OpenAPI_condition_group_t *OpenAPI_condition_group_copy(OpenAPI_condition_group_t *dst, OpenAPI_condition_group_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_condition_group_H_ */

