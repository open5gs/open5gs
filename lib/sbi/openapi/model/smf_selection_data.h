/*
 * smf_selection_data.h
 *
 * Represents the SMF Selection information that may be replaced by the PCF.
 */

#ifndef _OpenAPI_smf_selection_data_H_
#define _OpenAPI_smf_selection_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "candidate_for_replacement.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_smf_selection_data_s OpenAPI_smf_selection_data_t;
typedef struct OpenAPI_smf_selection_data_s {
    bool is_unsupp_dnn;
    int unsupp_dnn;
    bool is_candidates_null;
    OpenAPI_list_t* candidates;
    struct OpenAPI_snssai_s *snssai;
    struct OpenAPI_snssai_s *mapping_snssai;
    char *dnn;
} OpenAPI_smf_selection_data_t;

OpenAPI_smf_selection_data_t *OpenAPI_smf_selection_data_create(
    bool is_unsupp_dnn,
    int unsupp_dnn,
    bool is_candidates_null,
    OpenAPI_list_t* candidates,
    OpenAPI_snssai_t *snssai,
    OpenAPI_snssai_t *mapping_snssai,
    char *dnn
);
void OpenAPI_smf_selection_data_free(OpenAPI_smf_selection_data_t *smf_selection_data);
OpenAPI_smf_selection_data_t *OpenAPI_smf_selection_data_parseFromJSON(cJSON *smf_selection_dataJSON);
cJSON *OpenAPI_smf_selection_data_convertToJSON(OpenAPI_smf_selection_data_t *smf_selection_data);
OpenAPI_smf_selection_data_t *OpenAPI_smf_selection_data_copy(OpenAPI_smf_selection_data_t *dst, OpenAPI_smf_selection_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_smf_selection_data_H_ */

