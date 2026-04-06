/*
 * accuracy_req.h
 *
 * Represents the analytics accuracy requirement information.
 */

#ifndef _OpenAPI_accuracy_req_H_
#define _OpenAPI_accuracy_req_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_accuracy_req_s OpenAPI_accuracy_req_t;
#include "time_window.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_accuracy_req_s {
    struct OpenAPI_time_window_s *accu_time_win;
    bool is_accu_period;
    int accu_period;
    bool is_accu_dev_thr;
    int accu_dev_thr;
    bool is_min_num;
    int min_num;
    bool is_updated_ana_flg;
    int updated_ana_flg;
    bool is_correction_interval;
    int correction_interval;
};

OpenAPI_accuracy_req_t *OpenAPI_accuracy_req_create(
    OpenAPI_time_window_t *accu_time_win,
    bool is_accu_period,
    int accu_period,
    bool is_accu_dev_thr,
    int accu_dev_thr,
    bool is_min_num,
    int min_num,
    bool is_updated_ana_flg,
    int updated_ana_flg,
    bool is_correction_interval,
    int correction_interval
);
void OpenAPI_accuracy_req_free(OpenAPI_accuracy_req_t *accuracy_req);
OpenAPI_accuracy_req_t *OpenAPI_accuracy_req_parseFromJSON(cJSON *accuracy_reqJSON);
cJSON *OpenAPI_accuracy_req_convertToJSON(OpenAPI_accuracy_req_t *accuracy_req);
OpenAPI_accuracy_req_t *OpenAPI_accuracy_req_copy(OpenAPI_accuracy_req_t *dst, OpenAPI_accuracy_req_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_accuracy_req_H_ */

