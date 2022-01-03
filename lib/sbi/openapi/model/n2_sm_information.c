
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n2_sm_information.h"

OpenAPI_n2_sm_information_t *OpenAPI_n2_sm_information_create(
    int pdu_session_id,
    OpenAPI_n2_info_content_t *n2_info_content,
    OpenAPI_snssai_t *s_nssai,
    OpenAPI_snssai_t *home_plmn_snssai,
    OpenAPI_snssai_t *iwk_snssai,
    bool is_subject_to_ho,
    int subject_to_ho
)
{
    OpenAPI_n2_sm_information_t *n2_sm_information_local_var = ogs_malloc(sizeof(OpenAPI_n2_sm_information_t));
    ogs_assert(n2_sm_information_local_var);

    n2_sm_information_local_var->pdu_session_id = pdu_session_id;
    n2_sm_information_local_var->n2_info_content = n2_info_content;
    n2_sm_information_local_var->s_nssai = s_nssai;
    n2_sm_information_local_var->home_plmn_snssai = home_plmn_snssai;
    n2_sm_information_local_var->iwk_snssai = iwk_snssai;
    n2_sm_information_local_var->is_subject_to_ho = is_subject_to_ho;
    n2_sm_information_local_var->subject_to_ho = subject_to_ho;

    return n2_sm_information_local_var;
}

void OpenAPI_n2_sm_information_free(OpenAPI_n2_sm_information_t *n2_sm_information)
{
    if (NULL == n2_sm_information) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_n2_info_content_free(n2_sm_information->n2_info_content);
    OpenAPI_snssai_free(n2_sm_information->s_nssai);
    OpenAPI_snssai_free(n2_sm_information->home_plmn_snssai);
    OpenAPI_snssai_free(n2_sm_information->iwk_snssai);
    ogs_free(n2_sm_information);
}

cJSON *OpenAPI_n2_sm_information_convertToJSON(OpenAPI_n2_sm_information_t *n2_sm_information)
{
    cJSON *item = NULL;

    if (n2_sm_information == NULL) {
        ogs_error("OpenAPI_n2_sm_information_convertToJSON() failed [N2SmInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "pduSessionId", n2_sm_information->pdu_session_id) == NULL) {
        ogs_error("OpenAPI_n2_sm_information_convertToJSON() failed [pdu_session_id]");
        goto end;
    }

    if (n2_sm_information->n2_info_content) {
    cJSON *n2_info_content_local_JSON = OpenAPI_n2_info_content_convertToJSON(n2_sm_information->n2_info_content);
    if (n2_info_content_local_JSON == NULL) {
        ogs_error("OpenAPI_n2_sm_information_convertToJSON() failed [n2_info_content]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n2InfoContent", n2_info_content_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n2_sm_information_convertToJSON() failed [n2_info_content]");
        goto end;
    }
    }

    if (n2_sm_information->s_nssai) {
    cJSON *s_nssai_local_JSON = OpenAPI_snssai_convertToJSON(n2_sm_information->s_nssai);
    if (s_nssai_local_JSON == NULL) {
        ogs_error("OpenAPI_n2_sm_information_convertToJSON() failed [s_nssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sNssai", s_nssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n2_sm_information_convertToJSON() failed [s_nssai]");
        goto end;
    }
    }

    if (n2_sm_information->home_plmn_snssai) {
    cJSON *home_plmn_snssai_local_JSON = OpenAPI_snssai_convertToJSON(n2_sm_information->home_plmn_snssai);
    if (home_plmn_snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_n2_sm_information_convertToJSON() failed [home_plmn_snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "homePlmnSnssai", home_plmn_snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n2_sm_information_convertToJSON() failed [home_plmn_snssai]");
        goto end;
    }
    }

    if (n2_sm_information->iwk_snssai) {
    cJSON *iwk_snssai_local_JSON = OpenAPI_snssai_convertToJSON(n2_sm_information->iwk_snssai);
    if (iwk_snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_n2_sm_information_convertToJSON() failed [iwk_snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "iwkSnssai", iwk_snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n2_sm_information_convertToJSON() failed [iwk_snssai]");
        goto end;
    }
    }

    if (n2_sm_information->is_subject_to_ho) {
    if (cJSON_AddBoolToObject(item, "subjectToHo", n2_sm_information->subject_to_ho) == NULL) {
        ogs_error("OpenAPI_n2_sm_information_convertToJSON() failed [subject_to_ho]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_n2_sm_information_t *OpenAPI_n2_sm_information_parseFromJSON(cJSON *n2_sm_informationJSON)
{
    OpenAPI_n2_sm_information_t *n2_sm_information_local_var = NULL;
    cJSON *pdu_session_id = cJSON_GetObjectItemCaseSensitive(n2_sm_informationJSON, "pduSessionId");
    if (!pdu_session_id) {
        ogs_error("OpenAPI_n2_sm_information_parseFromJSON() failed [pdu_session_id]");
        goto end;
    }

    if (!cJSON_IsNumber(pdu_session_id)) {
        ogs_error("OpenAPI_n2_sm_information_parseFromJSON() failed [pdu_session_id]");
        goto end;
    }

    cJSON *n2_info_content = cJSON_GetObjectItemCaseSensitive(n2_sm_informationJSON, "n2InfoContent");

    OpenAPI_n2_info_content_t *n2_info_content_local_nonprim = NULL;
    if (n2_info_content) {
    n2_info_content_local_nonprim = OpenAPI_n2_info_content_parseFromJSON(n2_info_content);
    }

    cJSON *s_nssai = cJSON_GetObjectItemCaseSensitive(n2_sm_informationJSON, "sNssai");

    OpenAPI_snssai_t *s_nssai_local_nonprim = NULL;
    if (s_nssai) {
    s_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(s_nssai);
    }

    cJSON *home_plmn_snssai = cJSON_GetObjectItemCaseSensitive(n2_sm_informationJSON, "homePlmnSnssai");

    OpenAPI_snssai_t *home_plmn_snssai_local_nonprim = NULL;
    if (home_plmn_snssai) {
    home_plmn_snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(home_plmn_snssai);
    }

    cJSON *iwk_snssai = cJSON_GetObjectItemCaseSensitive(n2_sm_informationJSON, "iwkSnssai");

    OpenAPI_snssai_t *iwk_snssai_local_nonprim = NULL;
    if (iwk_snssai) {
    iwk_snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(iwk_snssai);
    }

    cJSON *subject_to_ho = cJSON_GetObjectItemCaseSensitive(n2_sm_informationJSON, "subjectToHo");

    if (subject_to_ho) {
    if (!cJSON_IsBool(subject_to_ho)) {
        ogs_error("OpenAPI_n2_sm_information_parseFromJSON() failed [subject_to_ho]");
        goto end;
    }
    }

    n2_sm_information_local_var = OpenAPI_n2_sm_information_create (
        
        pdu_session_id->valuedouble,
        n2_info_content ? n2_info_content_local_nonprim : NULL,
        s_nssai ? s_nssai_local_nonprim : NULL,
        home_plmn_snssai ? home_plmn_snssai_local_nonprim : NULL,
        iwk_snssai ? iwk_snssai_local_nonprim : NULL,
        subject_to_ho ? true : false,
        subject_to_ho ? subject_to_ho->valueint : 0
    );

    return n2_sm_information_local_var;
end:
    return NULL;
}

OpenAPI_n2_sm_information_t *OpenAPI_n2_sm_information_copy(OpenAPI_n2_sm_information_t *dst, OpenAPI_n2_sm_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_n2_sm_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_n2_sm_information_convertToJSON() failed");
        return NULL;
    }

    content = cJSON_Print(item);
    cJSON_Delete(item);

    if (!content) {
        ogs_error("cJSON_Print() failed");
        return NULL;
    }

    item = cJSON_Parse(content);
    ogs_free(content);
    if (!item) {
        ogs_error("cJSON_Parse() failed");
        return NULL;
    }

    OpenAPI_n2_sm_information_free(dst);
    dst = OpenAPI_n2_sm_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

