/*
 * loc_accuracy_info.h
 *
 * Contains location accuracy analytics. 
 */

#ifndef _OpenAPI_loc_accuracy_info_H_
#define _OpenAPI_loc_accuracy_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_loc_accuracy_info_s OpenAPI_loc_accuracy_info_t;
#include "loc_accuracy_per_method.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_loc_accuracy_info_s {
    OpenAPI_list_t *loc_acc_per_meths;
    bool is_in_out_ue_pct;
    int in_out_ue_pct;
    bool is_in_out_ind;
    int in_out_ind;
    bool is_confidence;
    int confidence;
};

OpenAPI_loc_accuracy_info_t *OpenAPI_loc_accuracy_info_create(
    OpenAPI_list_t *loc_acc_per_meths,
    bool is_in_out_ue_pct,
    int in_out_ue_pct,
    bool is_in_out_ind,
    int in_out_ind,
    bool is_confidence,
    int confidence
);
void OpenAPI_loc_accuracy_info_free(OpenAPI_loc_accuracy_info_t *loc_accuracy_info);
OpenAPI_loc_accuracy_info_t *OpenAPI_loc_accuracy_info_parseFromJSON(cJSON *loc_accuracy_infoJSON);
cJSON *OpenAPI_loc_accuracy_info_convertToJSON(OpenAPI_loc_accuracy_info_t *loc_accuracy_info);
OpenAPI_loc_accuracy_info_t *OpenAPI_loc_accuracy_info_copy(OpenAPI_loc_accuracy_info_t *dst, OpenAPI_loc_accuracy_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_loc_accuracy_info_H_ */

