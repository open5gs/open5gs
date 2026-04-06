/*
 * snssai_replace_info.h
 *
 * Indicates the status of an S-NSSAI and an alternative S-NSSAI optionally.
 */

#ifndef _OpenAPI_snssai_replace_info_H_
#define _OpenAPI_snssai_replace_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_snssai_replace_info_s OpenAPI_snssai_replace_info_t;
#include "mitigation_info.h"
#include "plmn_id.h"
#include "snssai.h"
#include "snssai_status.h"
#include "termination_indication.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_snssai_replace_info_s {
    struct OpenAPI_snssai_s *snssai;
    OpenAPI_snssai_status_e status;
    struct OpenAPI_snssai_s *alt_snssai;
    OpenAPI_termination_indication_e ns_repl_termin_ind;
    struct OpenAPI_plmn_id_s *plmn_id;
    struct OpenAPI_mitigation_info_s *mitigation_info;
};

OpenAPI_snssai_replace_info_t *OpenAPI_snssai_replace_info_create(
    OpenAPI_snssai_t *snssai,
    OpenAPI_snssai_status_e status,
    OpenAPI_snssai_t *alt_snssai,
    OpenAPI_termination_indication_e ns_repl_termin_ind,
    OpenAPI_plmn_id_t *plmn_id,
    OpenAPI_mitigation_info_t *mitigation_info
);
void OpenAPI_snssai_replace_info_free(OpenAPI_snssai_replace_info_t *snssai_replace_info);
OpenAPI_snssai_replace_info_t *OpenAPI_snssai_replace_info_parseFromJSON(cJSON *snssai_replace_infoJSON);
cJSON *OpenAPI_snssai_replace_info_convertToJSON(OpenAPI_snssai_replace_info_t *snssai_replace_info);
OpenAPI_snssai_replace_info_t *OpenAPI_snssai_replace_info_copy(OpenAPI_snssai_replace_info_t *dst, OpenAPI_snssai_replace_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_snssai_replace_info_H_ */

