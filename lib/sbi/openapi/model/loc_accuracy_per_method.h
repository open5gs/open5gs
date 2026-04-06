/*
 * loc_accuracy_per_method.h
 *
 * Contains location accuracy analytics per positioning method. 
 */

#ifndef _OpenAPI_loc_accuracy_per_method_H_
#define _OpenAPI_loc_accuracy_per_method_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_loc_accuracy_per_method_s OpenAPI_loc_accuracy_per_method_t;
#include "positioning_method.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_loc_accuracy_per_method_s {
    OpenAPI_positioning_method_e pos_method;
    int loc_acc;
    bool is_los_nlos_pct;
    int los_nlos_pct;
    bool is_los_nlos_ind;
    int los_nlos_ind;
};

OpenAPI_loc_accuracy_per_method_t *OpenAPI_loc_accuracy_per_method_create(
    OpenAPI_positioning_method_e pos_method,
    int loc_acc,
    bool is_los_nlos_pct,
    int los_nlos_pct,
    bool is_los_nlos_ind,
    int los_nlos_ind
);
void OpenAPI_loc_accuracy_per_method_free(OpenAPI_loc_accuracy_per_method_t *loc_accuracy_per_method);
OpenAPI_loc_accuracy_per_method_t *OpenAPI_loc_accuracy_per_method_parseFromJSON(cJSON *loc_accuracy_per_methodJSON);
cJSON *OpenAPI_loc_accuracy_per_method_convertToJSON(OpenAPI_loc_accuracy_per_method_t *loc_accuracy_per_method);
OpenAPI_loc_accuracy_per_method_t *OpenAPI_loc_accuracy_per_method_copy(OpenAPI_loc_accuracy_per_method_t *dst, OpenAPI_loc_accuracy_per_method_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_loc_accuracy_per_method_H_ */

