/*
 * lte_m_ind.h
 *
 * LTE-M Indication.
 */

#ifndef _OpenAPI_lte_m_ind_H_
#define _OpenAPI_lte_m_ind_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_lte_m_ind_s OpenAPI_lte_m_ind_t;
typedef struct OpenAPI_lte_m_ind_s {
    int lte_cat_m_ind;
} OpenAPI_lte_m_ind_t;

OpenAPI_lte_m_ind_t *OpenAPI_lte_m_ind_create(
    int lte_cat_m_ind
);
void OpenAPI_lte_m_ind_free(OpenAPI_lte_m_ind_t *lte_m_ind);
OpenAPI_lte_m_ind_t *OpenAPI_lte_m_ind_parseFromJSON(cJSON *lte_m_indJSON);
cJSON *OpenAPI_lte_m_ind_convertToJSON(OpenAPI_lte_m_ind_t *lte_m_ind);
OpenAPI_lte_m_ind_t *OpenAPI_lte_m_ind_copy(OpenAPI_lte_m_ind_t *dst, OpenAPI_lte_m_ind_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_lte_m_ind_H_ */

