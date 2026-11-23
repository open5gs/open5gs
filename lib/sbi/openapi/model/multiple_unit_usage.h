/*
 * multiple_unit_usage.h
 *
 * 
 */

#ifndef _OpenAPI_multiple_unit_usage_H_
#define _OpenAPI_multiple_unit_usage_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_multiple_unit_usage_s OpenAPI_multiple_unit_usage_t;
#include "allocate_unit.h"
#include "allocated_unit.h"
#include "pdu_address.h"
#include "requested_unit.h"
#include "used_unit_container.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_multiple_unit_usage_s {
    int rating_group;
    struct OpenAPI_requested_unit_s *requested_unit;
    struct OpenAPI_allocate_unit_s *allocate_unit;
    OpenAPI_list_t *used_unit_container;
    struct OpenAPI_allocated_unit_s *allocated_unit;
    char *u_pfid;
    struct OpenAPI_pdu_address_s *multihomed_pdu_address;
    char *m_bupfid;
};

OpenAPI_multiple_unit_usage_t *OpenAPI_multiple_unit_usage_create(
    int rating_group,
    OpenAPI_requested_unit_t *requested_unit,
    OpenAPI_allocate_unit_t *allocate_unit,
    OpenAPI_list_t *used_unit_container,
    OpenAPI_allocated_unit_t *allocated_unit,
    char *u_pfid,
    OpenAPI_pdu_address_t *multihomed_pdu_address,
    char *m_bupfid
);
void OpenAPI_multiple_unit_usage_free(OpenAPI_multiple_unit_usage_t *multiple_unit_usage);
OpenAPI_multiple_unit_usage_t *OpenAPI_multiple_unit_usage_parseFromJSON(cJSON *multiple_unit_usageJSON);
cJSON *OpenAPI_multiple_unit_usage_convertToJSON(OpenAPI_multiple_unit_usage_t *multiple_unit_usage);
OpenAPI_multiple_unit_usage_t *OpenAPI_multiple_unit_usage_copy(OpenAPI_multiple_unit_usage_t *dst, OpenAPI_multiple_unit_usage_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_multiple_unit_usage_H_ */

