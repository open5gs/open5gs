/*
 * n2_sm_information.h
 *
 * Represents the session management SMF related N2 information data part
 */

#ifndef _OpenAPI_n2_sm_information_H_
#define _OpenAPI_n2_sm_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "n2_info_content.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_n2_sm_information_s OpenAPI_n2_sm_information_t;
typedef struct OpenAPI_n2_sm_information_s {
    int pdu_session_id;
    struct OpenAPI_n2_info_content_s *n2_info_content;
    struct OpenAPI_snssai_s *s_nssai;
    struct OpenAPI_snssai_s *home_plmn_snssai;
    struct OpenAPI_snssai_s *iwk_snssai;
    bool is_subject_to_ho;
    int subject_to_ho;
} OpenAPI_n2_sm_information_t;

OpenAPI_n2_sm_information_t *OpenAPI_n2_sm_information_create(
    int pdu_session_id,
    OpenAPI_n2_info_content_t *n2_info_content,
    OpenAPI_snssai_t *s_nssai,
    OpenAPI_snssai_t *home_plmn_snssai,
    OpenAPI_snssai_t *iwk_snssai,
    bool is_subject_to_ho,
    int subject_to_ho
);
void OpenAPI_n2_sm_information_free(OpenAPI_n2_sm_information_t *n2_sm_information);
OpenAPI_n2_sm_information_t *OpenAPI_n2_sm_information_parseFromJSON(cJSON *n2_sm_informationJSON);
cJSON *OpenAPI_n2_sm_information_convertToJSON(OpenAPI_n2_sm_information_t *n2_sm_information);
OpenAPI_n2_sm_information_t *OpenAPI_n2_sm_information_copy(OpenAPI_n2_sm_information_t *dst, OpenAPI_n2_sm_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n2_sm_information_H_ */

