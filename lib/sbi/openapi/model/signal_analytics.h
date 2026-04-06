/*
 * signal_analytics.h
 *
 * The signalling storm analytics requirement information.
 */

#ifndef _OpenAPI_signal_analytics_H_
#define _OpenAPI_signal_analytics_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_signal_analytics_s OpenAPI_signal_analytics_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_signal_analytics_s {
    bool is_recv_sig_num;
    int recv_sig_num;
    bool is_sig_growth_rate;
    int sig_growth_rate;
    bool is_recv_sig_num_from_ue;
    int recv_sig_num_from_ue;
};

OpenAPI_signal_analytics_t *OpenAPI_signal_analytics_create(
    bool is_recv_sig_num,
    int recv_sig_num,
    bool is_sig_growth_rate,
    int sig_growth_rate,
    bool is_recv_sig_num_from_ue,
    int recv_sig_num_from_ue
);
void OpenAPI_signal_analytics_free(OpenAPI_signal_analytics_t *signal_analytics);
OpenAPI_signal_analytics_t *OpenAPI_signal_analytics_parseFromJSON(cJSON *signal_analyticsJSON);
cJSON *OpenAPI_signal_analytics_convertToJSON(OpenAPI_signal_analytics_t *signal_analytics);
OpenAPI_signal_analytics_t *OpenAPI_signal_analytics_copy(OpenAPI_signal_analytics_t *dst, OpenAPI_signal_analytics_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_signal_analytics_H_ */

