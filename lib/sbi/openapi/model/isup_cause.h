/*
 * isup_cause.h
 *
 * 
 */

#ifndef _OpenAPI_isup_cause_H_
#define _OpenAPI_isup_cause_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_isup_cause_s OpenAPI_isup_cause_t;
#include "ran_nas_rel_cause.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_isup_cause_s {
    bool is_i_sup_cause_location;
    int i_sup_cause_location;
    bool is_i_sup_cause_value;
    int i_sup_cause_value;
    char *i_sup_cause_diagnostics;
    OpenAPI_list_t *enhanced_diagnostics;
};

OpenAPI_isup_cause_t *OpenAPI_isup_cause_create(
    bool is_i_sup_cause_location,
    int i_sup_cause_location,
    bool is_i_sup_cause_value,
    int i_sup_cause_value,
    char *i_sup_cause_diagnostics,
    OpenAPI_list_t *enhanced_diagnostics
);
void OpenAPI_isup_cause_free(OpenAPI_isup_cause_t *isup_cause);
OpenAPI_isup_cause_t *OpenAPI_isup_cause_parseFromJSON(cJSON *isup_causeJSON);
cJSON *OpenAPI_isup_cause_convertToJSON(OpenAPI_isup_cause_t *isup_cause);
OpenAPI_isup_cause_t *OpenAPI_isup_cause_copy(OpenAPI_isup_cause_t *dst, OpenAPI_isup_cause_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_isup_cause_H_ */

