
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pdu_session_info_1.h"

OpenAPI_pdu_session_info_1_t *OpenAPI_pdu_session_info_1_create(
    OpenAPI_pdu_session_type_e pdu_sess_type,
    OpenAPI_ssc_mode_e ssc_mode,
    OpenAPI_list_t *access_types
)
{
    OpenAPI_pdu_session_info_1_t *pdu_session_info_1_local_var = ogs_malloc(sizeof(OpenAPI_pdu_session_info_1_t));
    ogs_assert(pdu_session_info_1_local_var);

    pdu_session_info_1_local_var->pdu_sess_type = pdu_sess_type;
    pdu_session_info_1_local_var->ssc_mode = ssc_mode;
    pdu_session_info_1_local_var->access_types = access_types;

    return pdu_session_info_1_local_var;
}

void OpenAPI_pdu_session_info_1_free(OpenAPI_pdu_session_info_1_t *pdu_session_info_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pdu_session_info_1) {
        return;
    }
    if (pdu_session_info_1->access_types) {
        OpenAPI_list_free(pdu_session_info_1->access_types);
        pdu_session_info_1->access_types = NULL;
    }
    ogs_free(pdu_session_info_1);
}

cJSON *OpenAPI_pdu_session_info_1_convertToJSON(OpenAPI_pdu_session_info_1_t *pdu_session_info_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pdu_session_info_1 == NULL) {
        ogs_error("OpenAPI_pdu_session_info_1_convertToJSON() failed [PduSessionInfo_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pdu_session_info_1->pdu_sess_type != OpenAPI_pdu_session_type_NULL) {
    if (cJSON_AddStringToObject(item, "pduSessType", OpenAPI_pdu_session_type_ToString(pdu_session_info_1->pdu_sess_type)) == NULL) {
        ogs_error("OpenAPI_pdu_session_info_1_convertToJSON() failed [pdu_sess_type]");
        goto end;
    }
    }

    if (pdu_session_info_1->ssc_mode != OpenAPI_ssc_mode_NULL) {
    if (cJSON_AddStringToObject(item, "sscMode", OpenAPI_ssc_mode_ToString(pdu_session_info_1->ssc_mode)) == NULL) {
        ogs_error("OpenAPI_pdu_session_info_1_convertToJSON() failed [ssc_mode]");
        goto end;
    }
    }

    if (pdu_session_info_1->access_types != OpenAPI_access_type_NULL) {
    cJSON *access_typesList = cJSON_AddArrayToObject(item, "accessTypes");
    if (access_typesList == NULL) {
        ogs_error("OpenAPI_pdu_session_info_1_convertToJSON() failed [access_types]");
        goto end;
    }
    OpenAPI_list_for_each(pdu_session_info_1->access_types, node) {
        if (cJSON_AddStringToObject(access_typesList, "", OpenAPI_access_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_pdu_session_info_1_convertToJSON() failed [access_types]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_pdu_session_info_1_t *OpenAPI_pdu_session_info_1_parseFromJSON(cJSON *pdu_session_info_1JSON)
{
    OpenAPI_pdu_session_info_1_t *pdu_session_info_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *pdu_sess_type = NULL;
    OpenAPI_pdu_session_type_e pdu_sess_typeVariable = 0;
    cJSON *ssc_mode = NULL;
    OpenAPI_ssc_mode_e ssc_modeVariable = 0;
    cJSON *access_types = NULL;
    OpenAPI_list_t *access_typesList = NULL;
    pdu_sess_type = cJSON_GetObjectItemCaseSensitive(pdu_session_info_1JSON, "pduSessType");
    if (pdu_sess_type) {
    if (!cJSON_IsString(pdu_sess_type)) {
        ogs_error("OpenAPI_pdu_session_info_1_parseFromJSON() failed [pdu_sess_type]");
        goto end;
    }
    pdu_sess_typeVariable = OpenAPI_pdu_session_type_FromString(pdu_sess_type->valuestring);
    }

    ssc_mode = cJSON_GetObjectItemCaseSensitive(pdu_session_info_1JSON, "sscMode");
    if (ssc_mode) {
    if (!cJSON_IsString(ssc_mode)) {
        ogs_error("OpenAPI_pdu_session_info_1_parseFromJSON() failed [ssc_mode]");
        goto end;
    }
    ssc_modeVariable = OpenAPI_ssc_mode_FromString(ssc_mode->valuestring);
    }

    access_types = cJSON_GetObjectItemCaseSensitive(pdu_session_info_1JSON, "accessTypes");
    if (access_types) {
        cJSON *access_types_local = NULL;
        if (!cJSON_IsArray(access_types)) {
            ogs_error("OpenAPI_pdu_session_info_1_parseFromJSON() failed [access_types]");
            goto end;
        }

        access_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(access_types_local, access_types) {
            OpenAPI_access_type_e localEnum = OpenAPI_access_type_NULL;
            if (!cJSON_IsString(access_types_local)) {
                ogs_error("OpenAPI_pdu_session_info_1_parseFromJSON() failed [access_types]");
                goto end;
            }
            localEnum = OpenAPI_access_type_FromString(access_types_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"access_types\" is not supported. Ignoring it ...",
                         access_types_local->valuestring);
            } else {
                OpenAPI_list_add(access_typesList, (void *)localEnum);
            }
        }
        if (access_typesList->count == 0) {
            ogs_error("OpenAPI_pdu_session_info_1_parseFromJSON() failed: Expected access_typesList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    pdu_session_info_1_local_var = OpenAPI_pdu_session_info_1_create (
        pdu_sess_type ? pdu_sess_typeVariable : 0,
        ssc_mode ? ssc_modeVariable : 0,
        access_types ? access_typesList : NULL
    );

    return pdu_session_info_1_local_var;
end:
    if (access_typesList) {
        OpenAPI_list_free(access_typesList);
        access_typesList = NULL;
    }
    return NULL;
}

OpenAPI_pdu_session_info_1_t *OpenAPI_pdu_session_info_1_copy(OpenAPI_pdu_session_info_1_t *dst, OpenAPI_pdu_session_info_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pdu_session_info_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pdu_session_info_1_convertToJSON() failed");
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

    OpenAPI_pdu_session_info_1_free(dst);
    dst = OpenAPI_pdu_session_info_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

