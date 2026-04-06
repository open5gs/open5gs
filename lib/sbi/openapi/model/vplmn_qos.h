/*
 * vplmn_qos.h
 *
 * VPLMN QoS
 */

#ifndef _OpenAPI_vplmn_qos_H_
#define _OpenAPI_vplmn_qos_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_vplmn_qos_s OpenAPI_vplmn_qos_t;
#include "ambr.h"
#include "arp.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_vplmn_qos_s {
    bool is__5qi;
    int _5qi;
    OpenAPI_list_t *add5_qi_list;
    struct OpenAPI_arp_s *arp;
    struct OpenAPI_ambr_s *session_ambr;
    char *max_fbr_dl;
    char *max_fbr_ul;
    char *gua_fbr_dl;
    char *gua_fbr_ul;
    bool is__5qi_pl;
    int _5qi_pl;
};

OpenAPI_vplmn_qos_t *OpenAPI_vplmn_qos_create(
    bool is__5qi,
    int _5qi,
    OpenAPI_list_t *add5_qi_list,
    OpenAPI_arp_t *arp,
    OpenAPI_ambr_t *session_ambr,
    char *max_fbr_dl,
    char *max_fbr_ul,
    char *gua_fbr_dl,
    char *gua_fbr_ul,
    bool is__5qi_pl,
    int _5qi_pl
);
void OpenAPI_vplmn_qos_free(OpenAPI_vplmn_qos_t *vplmn_qos);
OpenAPI_vplmn_qos_t *OpenAPI_vplmn_qos_parseFromJSON(cJSON *vplmn_qosJSON);
cJSON *OpenAPI_vplmn_qos_convertToJSON(OpenAPI_vplmn_qos_t *vplmn_qos);
OpenAPI_vplmn_qos_t *OpenAPI_vplmn_qos_copy(OpenAPI_vplmn_qos_t *dst, OpenAPI_vplmn_qos_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_vplmn_qos_H_ */

