/*
 * af_header_handling_control_info_1.h
 *
 * Describes AF requirements on handling of payload headers.
 */

#ifndef _OpenAPI_af_header_handling_control_info_1_H_
#define _OpenAPI_af_header_handling_control_info_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_af_header_handling_control_info_1_s OpenAPI_af_header_handling_control_info_1_t;
#include "header_handling_action_request_1.h"
#include "spatial_validity_rm_1.h"
#include "temporal_validity_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_af_header_handling_control_info_1_s {
    char *h_detection_reference;
    bool is_h_detection_supp_info_null;
    char *h_detection_supp_info;
    bool is_notif_uri_null;
    char *notif_uri;
    bool is_notif_id_null;
    char *notif_id;
    bool is_sp_val_null;
    struct OpenAPI_spatial_validity_rm_1_s *sp_val;
    bool is_temp_vals_null;
    OpenAPI_list_t *temp_vals;
    bool is_h_hndlg_ul_null;
    OpenAPI_list_t *h_hndlg_ul;
    bool is_h_hndlg_dl_null;
    OpenAPI_list_t *h_hndlg_dl;
};

OpenAPI_af_header_handling_control_info_1_t *OpenAPI_af_header_handling_control_info_1_create(
    char *h_detection_reference,
    bool is_h_detection_supp_info_null,
    char *h_detection_supp_info,
    bool is_notif_uri_null,
    char *notif_uri,
    bool is_notif_id_null,
    char *notif_id,
    bool is_sp_val_null,
    OpenAPI_spatial_validity_rm_1_t *sp_val,
    bool is_temp_vals_null,
    OpenAPI_list_t *temp_vals,
    bool is_h_hndlg_ul_null,
    OpenAPI_list_t *h_hndlg_ul,
    bool is_h_hndlg_dl_null,
    OpenAPI_list_t *h_hndlg_dl
);
void OpenAPI_af_header_handling_control_info_1_free(OpenAPI_af_header_handling_control_info_1_t *af_header_handling_control_info_1);
OpenAPI_af_header_handling_control_info_1_t *OpenAPI_af_header_handling_control_info_1_parseFromJSON(cJSON *af_header_handling_control_info_1JSON);
cJSON *OpenAPI_af_header_handling_control_info_1_convertToJSON(OpenAPI_af_header_handling_control_info_1_t *af_header_handling_control_info_1);
OpenAPI_af_header_handling_control_info_1_t *OpenAPI_af_header_handling_control_info_1_copy(OpenAPI_af_header_handling_control_info_1_t *dst, OpenAPI_af_header_handling_control_info_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_af_header_handling_control_info_1_H_ */

