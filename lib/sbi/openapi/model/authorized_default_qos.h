/*
 * authorized_default_qos.h
 *
 * Represents the Authorized Default QoS.
 */

#ifndef _OpenAPI_authorized_default_qos_H_
#define _OpenAPI_authorized_default_qos_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "arp.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_authorized_default_qos_s OpenAPI_authorized_default_qos_t;
typedef struct OpenAPI_authorized_default_qos_s {
    bool is__5qi;
    int _5qi;
    struct OpenAPI_arp_s *arp;
    bool is_priority_level_null;
    bool is_priority_level;
    int priority_level;
    bool is_aver_window_null;
    bool is_aver_window;
    int aver_window;
    bool is_max_data_burst_vol_null;
    bool is_max_data_burst_vol;
    int max_data_burst_vol;
    bool is_maxbr_ul_null;
    char *maxbr_ul;
    bool is_maxbr_dl_null;
    char *maxbr_dl;
    bool is_gbr_ul_null;
    char *gbr_ul;
    bool is_gbr_dl_null;
    char *gbr_dl;
    bool is_ext_max_data_burst_vol_null;
    bool is_ext_max_data_burst_vol;
    int ext_max_data_burst_vol;
} OpenAPI_authorized_default_qos_t;

OpenAPI_authorized_default_qos_t *OpenAPI_authorized_default_qos_create(
    bool is__5qi,
    int _5qi,
    OpenAPI_arp_t *arp,
    bool is_priority_level_null,
    bool is_priority_level,
    int priority_level,
    bool is_aver_window_null,
    bool is_aver_window,
    int aver_window,
    bool is_max_data_burst_vol_null,
    bool is_max_data_burst_vol,
    int max_data_burst_vol,
    bool is_maxbr_ul_null,
    char *maxbr_ul,
    bool is_maxbr_dl_null,
    char *maxbr_dl,
    bool is_gbr_ul_null,
    char *gbr_ul,
    bool is_gbr_dl_null,
    char *gbr_dl,
    bool is_ext_max_data_burst_vol_null,
    bool is_ext_max_data_burst_vol,
    int ext_max_data_burst_vol
);
void OpenAPI_authorized_default_qos_free(OpenAPI_authorized_default_qos_t *authorized_default_qos);
OpenAPI_authorized_default_qos_t *OpenAPI_authorized_default_qos_parseFromJSON(cJSON *authorized_default_qosJSON);
cJSON *OpenAPI_authorized_default_qos_convertToJSON(OpenAPI_authorized_default_qos_t *authorized_default_qos);
OpenAPI_authorized_default_qos_t *OpenAPI_authorized_default_qos_copy(OpenAPI_authorized_default_qos_t *dst, OpenAPI_authorized_default_qos_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_authorized_default_qos_H_ */

