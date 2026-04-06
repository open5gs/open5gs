/*
 * dnai_eas_info.h
 *
 * Contains EAS information for a DNAI.
 */

#ifndef _OpenAPI_dnai_eas_info_H_
#define _OpenAPI_dnai_eas_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_dnai_eas_info_s OpenAPI_dnai_eas_info_t;
#include "fqdn_pattern_matching_rule.h"
#include "ip_addr_1.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_dnai_eas_info_s {
    char *dnn;
    struct OpenAPI_snssai_s *snssai;
    OpenAPI_list_t *dnais;
    OpenAPI_list_t *eas_ip_addrs;
    OpenAPI_list_t *fqdns;
};

OpenAPI_dnai_eas_info_t *OpenAPI_dnai_eas_info_create(
    char *dnn,
    OpenAPI_snssai_t *snssai,
    OpenAPI_list_t *dnais,
    OpenAPI_list_t *eas_ip_addrs,
    OpenAPI_list_t *fqdns
);
void OpenAPI_dnai_eas_info_free(OpenAPI_dnai_eas_info_t *dnai_eas_info);
OpenAPI_dnai_eas_info_t *OpenAPI_dnai_eas_info_parseFromJSON(cJSON *dnai_eas_infoJSON);
cJSON *OpenAPI_dnai_eas_info_convertToJSON(OpenAPI_dnai_eas_info_t *dnai_eas_info);
OpenAPI_dnai_eas_info_t *OpenAPI_dnai_eas_info_copy(OpenAPI_dnai_eas_info_t *dst, OpenAPI_dnai_eas_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dnai_eas_info_H_ */

