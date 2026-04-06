/*
 * snssai_part_rejected.h
 *
 * Represents the list of the S-NSSAI(s) rejected partially in the RA.
 */

#ifndef _OpenAPI_snssai_part_rejected_H_
#define _OpenAPI_snssai_part_rejected_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_snssai_part_rejected_s OpenAPI_snssai_part_rejected_t;
#include "snssai.h"
#include "tai.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_snssai_part_rejected_s {
    struct OpenAPI_snssai_s *snssai;
    OpenAPI_list_t *allowed_tai_list;
    OpenAPI_list_t *rejected_tai_list;
};

OpenAPI_snssai_part_rejected_t *OpenAPI_snssai_part_rejected_create(
    OpenAPI_snssai_t *snssai,
    OpenAPI_list_t *allowed_tai_list,
    OpenAPI_list_t *rejected_tai_list
);
void OpenAPI_snssai_part_rejected_free(OpenAPI_snssai_part_rejected_t *snssai_part_rejected);
OpenAPI_snssai_part_rejected_t *OpenAPI_snssai_part_rejected_parseFromJSON(cJSON *snssai_part_rejectedJSON);
cJSON *OpenAPI_snssai_part_rejected_convertToJSON(OpenAPI_snssai_part_rejected_t *snssai_part_rejected);
OpenAPI_snssai_part_rejected_t *OpenAPI_snssai_part_rejected_copy(OpenAPI_snssai_part_rejected_t *dst, OpenAPI_snssai_part_rejected_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_snssai_part_rejected_H_ */

