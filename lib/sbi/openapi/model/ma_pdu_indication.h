/*
 * ma_pdu_indication.h
 *
 *
 */

#ifndef _OpenAPI_ma_pdu_indication_H_
#define _OpenAPI_ma_pdu_indication_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ma_pdu_indication_s OpenAPI_ma_pdu_indication_t;
typedef struct OpenAPI_ma_pdu_indication_s {
} OpenAPI_ma_pdu_indication_t;

OpenAPI_ma_pdu_indication_t *OpenAPI_ma_pdu_indication_create(
    );
void OpenAPI_ma_pdu_indication_free(OpenAPI_ma_pdu_indication_t *ma_pdu_indication);
OpenAPI_ma_pdu_indication_t *OpenAPI_ma_pdu_indication_parseFromJSON(cJSON *ma_pdu_indicationJSON);
cJSON *OpenAPI_ma_pdu_indication_convertToJSON(OpenAPI_ma_pdu_indication_t *ma_pdu_indication);
OpenAPI_ma_pdu_indication_t *OpenAPI_ma_pdu_indication_copy(OpenAPI_ma_pdu_indication_t *dst, OpenAPI_ma_pdu_indication_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ma_pdu_indication_H_ */

