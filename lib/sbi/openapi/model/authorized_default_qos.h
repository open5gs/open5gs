/*
 * authorized_default_qos.h
 *
 *
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
    int _5qi;
    struct OpenAPI_arp_s *arp;
    int priority_level;
    int aver_window;
    int max_data_burst_vol;
    char *maxbr_ul;
    char *maxbr_dl;
    char *gbr_ul;
    char *gbr_dl;
    int qnc;
    int ext_max_data_burst_vol;
} OpenAPI_authorized_default_qos_t;

OpenAPI_authorized_default_qos_t *OpenAPI_authorized_default_qos_create(
    int _5qi,
    OpenAPI_arp_t *arp,
    int priority_level,
    int aver_window,
    int max_data_burst_vol,
    char *maxbr_ul,
    char *maxbr_dl,
    char *gbr_ul,
    char *gbr_dl,
    int qnc,
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

