/*
 * observed_redundant_trans_exp.h
 *
 * Represents the observed redundant transmission experience related information.
 */

#ifndef _OpenAPI_observed_redundant_trans_exp_H_
#define _OpenAPI_observed_redundant_trans_exp_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_observed_redundant_trans_exp_s OpenAPI_observed_redundant_trans_exp_t;
typedef struct OpenAPI_observed_redundant_trans_exp_s {
    bool is_avg_pkt_drop_rate_ul;
    int avg_pkt_drop_rate_ul;
    bool is_var_pkt_drop_rate_ul;
    float var_pkt_drop_rate_ul;
    bool is_avg_pkt_drop_rate_dl;
    int avg_pkt_drop_rate_dl;
    bool is_var_pkt_drop_rate_dl;
    float var_pkt_drop_rate_dl;
    bool is_avg_pkt_delay_ul;
    int avg_pkt_delay_ul;
    bool is_var_pkt_delay_ul;
    float var_pkt_delay_ul;
    bool is_avg_pkt_delay_dl;
    int avg_pkt_delay_dl;
    bool is_var_pkt_delay_dl;
    float var_pkt_delay_dl;
} OpenAPI_observed_redundant_trans_exp_t;

OpenAPI_observed_redundant_trans_exp_t *OpenAPI_observed_redundant_trans_exp_create(
    bool is_avg_pkt_drop_rate_ul,
    int avg_pkt_drop_rate_ul,
    bool is_var_pkt_drop_rate_ul,
    float var_pkt_drop_rate_ul,
    bool is_avg_pkt_drop_rate_dl,
    int avg_pkt_drop_rate_dl,
    bool is_var_pkt_drop_rate_dl,
    float var_pkt_drop_rate_dl,
    bool is_avg_pkt_delay_ul,
    int avg_pkt_delay_ul,
    bool is_var_pkt_delay_ul,
    float var_pkt_delay_ul,
    bool is_avg_pkt_delay_dl,
    int avg_pkt_delay_dl,
    bool is_var_pkt_delay_dl,
    float var_pkt_delay_dl
);
void OpenAPI_observed_redundant_trans_exp_free(OpenAPI_observed_redundant_trans_exp_t *observed_redundant_trans_exp);
OpenAPI_observed_redundant_trans_exp_t *OpenAPI_observed_redundant_trans_exp_parseFromJSON(cJSON *observed_redundant_trans_expJSON);
cJSON *OpenAPI_observed_redundant_trans_exp_convertToJSON(OpenAPI_observed_redundant_trans_exp_t *observed_redundant_trans_exp);
OpenAPI_observed_redundant_trans_exp_t *OpenAPI_observed_redundant_trans_exp_copy(OpenAPI_observed_redundant_trans_exp_t *dst, OpenAPI_observed_redundant_trans_exp_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_observed_redundant_trans_exp_H_ */

