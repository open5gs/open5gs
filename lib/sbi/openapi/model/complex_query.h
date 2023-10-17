/*
 * complex_query.h
 *
 * The ComplexQuery data type is either a conjunctive normal form or a disjunctive normal form.  The attribute names \&quot;cnfUnits\&quot; and \&quot;dnfUnits\&quot; (see clause 5.2.4.11 and clause 5.2.4.12)  serve as discriminator. 
 */

#ifndef _OpenAPI_complex_query_H_
#define _OpenAPI_complex_query_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "cnf.h"
#include "cnf_unit.h"
#include "dnf.h"
#include "dnf_unit.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_complex_query_s OpenAPI_complex_query_t;
typedef struct OpenAPI_complex_query_s {
    OpenAPI_list_t *cnf_units;
    OpenAPI_list_t *dnf_units;
} OpenAPI_complex_query_t;

OpenAPI_complex_query_t *OpenAPI_complex_query_create(
    OpenAPI_list_t *cnf_units,
    OpenAPI_list_t *dnf_units
);
void OpenAPI_complex_query_free(OpenAPI_complex_query_t *complex_query);
OpenAPI_complex_query_t *OpenAPI_complex_query_parseFromJSON(cJSON *complex_queryJSON);
cJSON *OpenAPI_complex_query_convertToJSON(OpenAPI_complex_query_t *complex_query);
OpenAPI_complex_query_t *OpenAPI_complex_query_copy(OpenAPI_complex_query_t *dst, OpenAPI_complex_query_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_complex_query_H_ */

