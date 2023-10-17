/*
 * n4_information.h
 *
 * N4 Information
 */

#ifndef _OpenAPI_n4_information_H_
#define _OpenAPI_n4_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "dnai_information.h"
#include "n4_message_type.h"
#include "ref_to_binary_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_n4_information_s OpenAPI_n4_information_t;
typedef struct OpenAPI_n4_information_s {
    OpenAPI_n4_message_type_e n4_message_type;
    struct OpenAPI_ref_to_binary_data_s *n4_message_payload;
    struct OpenAPI_dnai_information_s *n4_dnai_info;
    char *psa_upf_id;
    char *ul_cl_bp_id;
    OpenAPI_list_t *n9_ul_pdr_id_list;
} OpenAPI_n4_information_t;

OpenAPI_n4_information_t *OpenAPI_n4_information_create(
    OpenAPI_n4_message_type_e n4_message_type,
    OpenAPI_ref_to_binary_data_t *n4_message_payload,
    OpenAPI_dnai_information_t *n4_dnai_info,
    char *psa_upf_id,
    char *ul_cl_bp_id,
    OpenAPI_list_t *n9_ul_pdr_id_list
);
void OpenAPI_n4_information_free(OpenAPI_n4_information_t *n4_information);
OpenAPI_n4_information_t *OpenAPI_n4_information_parseFromJSON(cJSON *n4_informationJSON);
cJSON *OpenAPI_n4_information_convertToJSON(OpenAPI_n4_information_t *n4_information);
OpenAPI_n4_information_t *OpenAPI_n4_information_copy(OpenAPI_n4_information_t *dst, OpenAPI_n4_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n4_information_H_ */

