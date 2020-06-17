/*
 * vplmn_qos.h
 *
 *
 */

#ifndef _OpenAPI_vplmn_qos_H_
#define _OpenAPI_vplmn_qos_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ambr.h"
#include "arp.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_vplmn_qos_s OpenAPI_vplmn_qos_t;
typedef struct OpenAPI_vplmn_qos_s {
    int _5qi;
    struct OpenAPI_arp_s *arp;
    struct OpenAPI_ambr_s *session_ambr;
    char *max_fbr_dl;
    char *max_fbr_ul;
    char *gua_fbr_dl;
    char *gua_fbr_ul;
} OpenAPI_vplmn_qos_t;

OpenAPI_vplmn_qos_t *OpenAPI_vplmn_qos_create(
    int _5qi,
    OpenAPI_arp_t *arp,
    OpenAPI_ambr_t *session_ambr,
    char *max_fbr_dl,
    char *max_fbr_ul,
    char *gua_fbr_dl,
    char *gua_fbr_ul
    );
void OpenAPI_vplmn_qos_free(OpenAPI_vplmn_qos_t *vplmn_qos);
OpenAPI_vplmn_qos_t *OpenAPI_vplmn_qos_parseFromJSON(cJSON *vplmn_qosJSON);
cJSON *OpenAPI_vplmn_qos_convertToJSON(OpenAPI_vplmn_qos_t *vplmn_qos);
OpenAPI_vplmn_qos_t *OpenAPI_vplmn_qos_copy(OpenAPI_vplmn_qos_t *dst, OpenAPI_vplmn_qos_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_vplmn_qos_H_ */

