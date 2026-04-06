/*
 * qos_monitoring_information_rm.h
 *
 * This data type is defined in the same way as the QosMonitoringInformation data type, but with the OpenAPI nullable property set to true. 
 */

#ifndef _OpenAPI_qos_monitoring_information_rm_H_
#define _OpenAPI_qos_monitoring_information_rm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_qos_monitoring_information_rm_s OpenAPI_qos_monitoring_information_rm_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_qos_monitoring_information_rm_s {
    bool is_rep_thresh_dl_null;
    bool is_rep_thresh_dl;
    int rep_thresh_dl;
    bool is_rep_thresh_ul_null;
    bool is_rep_thresh_ul;
    int rep_thresh_ul;
    bool is_rep_thresh_rp_null;
    bool is_rep_thresh_rp;
    int rep_thresh_rp;
    bool is_rep_thresh_dat_rate_ul_null;
    char *rep_thresh_dat_rate_ul;
    bool is_rep_thresh_dat_rate_dl_null;
    char *rep_thresh_dat_rate_dl;
    bool is_con_thresh_dl_null;
    bool is_con_thresh_dl;
    int con_thresh_dl;
    bool is_con_thresh_ul_null;
    bool is_con_thresh_ul;
    int con_thresh_ul;
    OpenAPI_list_t *avl_bitrate_ul_thrs;
    OpenAPI_list_t *avl_bitrate_dl_thrs;
};

OpenAPI_qos_monitoring_information_rm_t *OpenAPI_qos_monitoring_information_rm_create(
    bool is_rep_thresh_dl_null,
    bool is_rep_thresh_dl,
    int rep_thresh_dl,
    bool is_rep_thresh_ul_null,
    bool is_rep_thresh_ul,
    int rep_thresh_ul,
    bool is_rep_thresh_rp_null,
    bool is_rep_thresh_rp,
    int rep_thresh_rp,
    bool is_rep_thresh_dat_rate_ul_null,
    char *rep_thresh_dat_rate_ul,
    bool is_rep_thresh_dat_rate_dl_null,
    char *rep_thresh_dat_rate_dl,
    bool is_con_thresh_dl_null,
    bool is_con_thresh_dl,
    int con_thresh_dl,
    bool is_con_thresh_ul_null,
    bool is_con_thresh_ul,
    int con_thresh_ul,
    OpenAPI_list_t *avl_bitrate_ul_thrs,
    OpenAPI_list_t *avl_bitrate_dl_thrs
);
void OpenAPI_qos_monitoring_information_rm_free(OpenAPI_qos_monitoring_information_rm_t *qos_monitoring_information_rm);
OpenAPI_qos_monitoring_information_rm_t *OpenAPI_qos_monitoring_information_rm_parseFromJSON(cJSON *qos_monitoring_information_rmJSON);
cJSON *OpenAPI_qos_monitoring_information_rm_convertToJSON(OpenAPI_qos_monitoring_information_rm_t *qos_monitoring_information_rm);
OpenAPI_qos_monitoring_information_rm_t *OpenAPI_qos_monitoring_information_rm_copy(OpenAPI_qos_monitoring_information_rm_t *dst, OpenAPI_qos_monitoring_information_rm_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_monitoring_information_rm_H_ */

