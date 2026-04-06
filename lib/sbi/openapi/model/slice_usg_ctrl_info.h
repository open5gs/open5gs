/*
 * slice_usg_ctrl_info.h
 *
 * Represents network slice usage control information.
 */

#ifndef _OpenAPI_slice_usg_ctrl_info_H_
#define _OpenAPI_slice_usg_ctrl_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_slice_usg_ctrl_info_s OpenAPI_slice_usg_ctrl_info_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_slice_usg_ctrl_info_s {
    bool is_pdu_sess_inactiv_timer_null;
    bool is_pdu_sess_inactiv_timer;
    int pdu_sess_inactiv_timer;
};

OpenAPI_slice_usg_ctrl_info_t *OpenAPI_slice_usg_ctrl_info_create(
    bool is_pdu_sess_inactiv_timer_null,
    bool is_pdu_sess_inactiv_timer,
    int pdu_sess_inactiv_timer
);
void OpenAPI_slice_usg_ctrl_info_free(OpenAPI_slice_usg_ctrl_info_t *slice_usg_ctrl_info);
OpenAPI_slice_usg_ctrl_info_t *OpenAPI_slice_usg_ctrl_info_parseFromJSON(cJSON *slice_usg_ctrl_infoJSON);
cJSON *OpenAPI_slice_usg_ctrl_info_convertToJSON(OpenAPI_slice_usg_ctrl_info_t *slice_usg_ctrl_info);
OpenAPI_slice_usg_ctrl_info_t *OpenAPI_slice_usg_ctrl_info_copy(OpenAPI_slice_usg_ctrl_info_t *dst, OpenAPI_slice_usg_ctrl_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_slice_usg_ctrl_info_H_ */

