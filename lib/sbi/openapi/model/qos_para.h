/*
 * qos_para.h
 *
 * The values of the QoS parameters.
 */

#ifndef _OpenAPI_qos_para_H_
#define _OpenAPI_qos_para_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_qos_para_s OpenAPI_qos_para_t;
#include "qos_resource_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_qos_para_s {
    char *qos_param_set_id;
    bool is__5qi;
    int _5qi;
    bool is_priority_lvl;
    int priority_lvl;
    OpenAPI_qos_resource_type_e rsc_type;
    bool is_pdb;
    int pdb;
    char *per;
    char *gbr_ul;
    char *gbr_dl;
    char *mbr_ul;
    char *mbr_dl;
    bool is_max_plr_ul;
    int max_plr_ul;
    bool is_max_plr_dl;
    int max_plr_dl;
    bool is_avg_win;
    int avg_win;
    bool is_max_data_burst_vol;
    int max_data_burst_vol;
    bool is_ext_max_data_burst_vol;
    int ext_max_data_burst_vol;
};

OpenAPI_qos_para_t *OpenAPI_qos_para_create(
    char *qos_param_set_id,
    bool is__5qi,
    int _5qi,
    bool is_priority_lvl,
    int priority_lvl,
    OpenAPI_qos_resource_type_e rsc_type,
    bool is_pdb,
    int pdb,
    char *per,
    char *gbr_ul,
    char *gbr_dl,
    char *mbr_ul,
    char *mbr_dl,
    bool is_max_plr_ul,
    int max_plr_ul,
    bool is_max_plr_dl,
    int max_plr_dl,
    bool is_avg_win,
    int avg_win,
    bool is_max_data_burst_vol,
    int max_data_burst_vol,
    bool is_ext_max_data_burst_vol,
    int ext_max_data_burst_vol
);
void OpenAPI_qos_para_free(OpenAPI_qos_para_t *qos_para);
OpenAPI_qos_para_t *OpenAPI_qos_para_parseFromJSON(cJSON *qos_paraJSON);
cJSON *OpenAPI_qos_para_convertToJSON(OpenAPI_qos_para_t *qos_para);
OpenAPI_qos_para_t *OpenAPI_qos_para_copy(OpenAPI_qos_para_t *dst, OpenAPI_qos_para_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_para_H_ */

