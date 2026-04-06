/*
 * header_handling_action_request_1.h
 *
 * Represents the header handling action request. 
 */

#ifndef _OpenAPI_header_handling_action_request_1_H_
#define _OpenAPI_header_handling_action_request_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_header_handling_action_request_1_s OpenAPI_header_handling_action_request_1_t;
#include "header_handling_action.h"
#include "header_handling_cond.h"
#include "header_handling_reporting_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_header_handling_action_request_1_s {
    char *h_hndlg_ctrl_ref;
    OpenAPI_list_t *h_hndlg_action;
    char *h_info;
    char *h_val;
    OpenAPI_header_handling_cond_e h_hndlg_cond;
    struct OpenAPI_header_handling_reporting_1_s *h_hndlg_rep;
};

OpenAPI_header_handling_action_request_1_t *OpenAPI_header_handling_action_request_1_create(
    char *h_hndlg_ctrl_ref,
    OpenAPI_list_t *h_hndlg_action,
    char *h_info,
    char *h_val,
    OpenAPI_header_handling_cond_e h_hndlg_cond,
    OpenAPI_header_handling_reporting_1_t *h_hndlg_rep
);
void OpenAPI_header_handling_action_request_1_free(OpenAPI_header_handling_action_request_1_t *header_handling_action_request_1);
OpenAPI_header_handling_action_request_1_t *OpenAPI_header_handling_action_request_1_parseFromJSON(cJSON *header_handling_action_request_1JSON);
cJSON *OpenAPI_header_handling_action_request_1_convertToJSON(OpenAPI_header_handling_action_request_1_t *header_handling_action_request_1);
OpenAPI_header_handling_action_request_1_t *OpenAPI_header_handling_action_request_1_copy(OpenAPI_header_handling_action_request_1_t *dst, OpenAPI_header_handling_action_request_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_header_handling_action_request_1_H_ */

