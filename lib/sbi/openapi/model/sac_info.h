/*
 * sac_info.h
 *
 * Represents threshold(s) to control the triggering of network slice reporting notifications or the information contained in the network slice reporting notification. 
 */

#ifndef _OpenAPI_sac_info_H_
#define _OpenAPI_sac_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_sac_info_s OpenAPI_sac_info_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_sac_info_s {
    bool is_numeric_val_num_ues;
    int numeric_val_num_ues;
    bool is_numeric_val_num_pdu_sess;
    int numeric_val_num_pdu_sess;
    bool is_perc_value_num_ues;
    int perc_value_num_ues;
    bool is_perc_value_num_pdu_sess;
    int perc_value_num_pdu_sess;
    bool is_ues_with_pdu_session_ind;
    int ues_with_pdu_session_ind;
};

OpenAPI_sac_info_t *OpenAPI_sac_info_create(
    bool is_numeric_val_num_ues,
    int numeric_val_num_ues,
    bool is_numeric_val_num_pdu_sess,
    int numeric_val_num_pdu_sess,
    bool is_perc_value_num_ues,
    int perc_value_num_ues,
    bool is_perc_value_num_pdu_sess,
    int perc_value_num_pdu_sess,
    bool is_ues_with_pdu_session_ind,
    int ues_with_pdu_session_ind
);
void OpenAPI_sac_info_free(OpenAPI_sac_info_t *sac_info);
OpenAPI_sac_info_t *OpenAPI_sac_info_parseFromJSON(cJSON *sac_infoJSON);
cJSON *OpenAPI_sac_info_convertToJSON(OpenAPI_sac_info_t *sac_info);
OpenAPI_sac_info_t *OpenAPI_sac_info_copy(OpenAPI_sac_info_t *dst, OpenAPI_sac_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sac_info_H_ */

