/*
 * civic_address.h
 *
 * 
 */

#ifndef _OpenAPI_civic_address_H_
#define _OpenAPI_civic_address_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_civic_address_s OpenAPI_civic_address_t;
typedef struct OpenAPI_civic_address_s {
    char *country;
    char *a1;
    char *a2;
    char *a3;
    char *a4;
    char *a5;
    char *a6;
    char *prd;
    char *pod;
    char *sts;
    char *hno;
    char *hns;
    char *lmk;
    char *loc;
    char *nam;
    char *pc;
    char *bld;
    char *unit;
    char *flr;
    char *room;
    char *plc;
    char *pcn;
    char *pobox;
    char *addcode;
    char *seat;
    char *rd;
    char *rdsec;
    char *rdbr;
    char *rdsubbr;
    char *prm;
    char *pom;
    char *usage_rules;
    char *method;
    char *provided_by;
} OpenAPI_civic_address_t;

OpenAPI_civic_address_t *OpenAPI_civic_address_create(
    char *country,
    char *a1,
    char *a2,
    char *a3,
    char *a4,
    char *a5,
    char *a6,
    char *prd,
    char *pod,
    char *sts,
    char *hno,
    char *hns,
    char *lmk,
    char *loc,
    char *nam,
    char *pc,
    char *bld,
    char *unit,
    char *flr,
    char *room,
    char *plc,
    char *pcn,
    char *pobox,
    char *addcode,
    char *seat,
    char *rd,
    char *rdsec,
    char *rdbr,
    char *rdsubbr,
    char *prm,
    char *pom,
    char *usage_rules,
    char *method,
    char *provided_by
);
void OpenAPI_civic_address_free(OpenAPI_civic_address_t *civic_address);
OpenAPI_civic_address_t *OpenAPI_civic_address_parseFromJSON(cJSON *civic_addressJSON);
cJSON *OpenAPI_civic_address_convertToJSON(OpenAPI_civic_address_t *civic_address);
OpenAPI_civic_address_t *OpenAPI_civic_address_copy(OpenAPI_civic_address_t *dst, OpenAPI_civic_address_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_civic_address_H_ */

