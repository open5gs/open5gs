/*
 * loc_accuracy_req.h
 *
 * Contains location accuracy analytics requirements. 
 */

#ifndef _OpenAPI_loc_accuracy_req_H_
#define _OpenAPI_loc_accuracy_req_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_loc_accuracy_req_s OpenAPI_loc_accuracy_req_t;
#include "matching_direction.h"
#include "positioning_method.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_loc_accuracy_req_s {
    bool is_acc_thres;
    int acc_thres;
    OpenAPI_matching_direction_e acc_thres_match_dir;
    bool is_in_out_thres;
    int in_out_thres;
    OpenAPI_matching_direction_e in_out_thres_match_dir;
    OpenAPI_positioning_method_e pos_method;
};

OpenAPI_loc_accuracy_req_t *OpenAPI_loc_accuracy_req_create(
    bool is_acc_thres,
    int acc_thres,
    OpenAPI_matching_direction_e acc_thres_match_dir,
    bool is_in_out_thres,
    int in_out_thres,
    OpenAPI_matching_direction_e in_out_thres_match_dir,
    OpenAPI_positioning_method_e pos_method
);
void OpenAPI_loc_accuracy_req_free(OpenAPI_loc_accuracy_req_t *loc_accuracy_req);
OpenAPI_loc_accuracy_req_t *OpenAPI_loc_accuracy_req_parseFromJSON(cJSON *loc_accuracy_reqJSON);
cJSON *OpenAPI_loc_accuracy_req_convertToJSON(OpenAPI_loc_accuracy_req_t *loc_accuracy_req);
OpenAPI_loc_accuracy_req_t *OpenAPI_loc_accuracy_req_copy(OpenAPI_loc_accuracy_req_t *dst, OpenAPI_loc_accuracy_req_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_loc_accuracy_req_H_ */

