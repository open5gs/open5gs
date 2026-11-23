
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "charging_pdu_session_info.h"

OpenAPI_charging_pdu_session_info_t *OpenAPI_charging_pdu_session_info_create(
    OpenAPI_pdu_session_type_e pdu_sess_type,
    OpenAPI_ssc_mode_e ssc_mode,
    OpenAPI_list_t *access_types
)
{
    OpenAPI_charging_pdu_session_info_t *charging_pdu_session_info_local_var = ogs_malloc(sizeof(OpenAPI_charging_pdu_session_info_t));
    ogs_assert(charging_pdu_session_info_local_var);

    charging_pdu_session_info_local_var->pdu_sess_type = pdu_sess_type;
    charging_pdu_session_info_local_var->ssc_mode = ssc_mode;
    charging_pdu_session_info_local_var->access_types = access_types;

    return charging_pdu_session_info_local_var;
}

void OpenAPI_charging_pdu_session_info_free(OpenAPI_charging_pdu_session_info_t *charging_pdu_session_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == charging_pdu_session_info) {
        return;
    }
    if (charging_pdu_session_info->access_types) {
        OpenAPI_list_free(charging_pdu_session_info->access_types);
        charging_pdu_session_info->access_types = NULL;
    }
    ogs_free(charging_pdu_session_info);
}

cJSON *OpenAPI_charging_pdu_session_info_convertToJSON(OpenAPI_charging_pdu_session_info_t *charging_pdu_session_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (charging_pdu_session_info == NULL) {
        ogs_error("OpenAPI_charging_pdu_session_info_convertToJSON() failed [ChargingPduSessionInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (charging_pdu_session_info->pdu_sess_type != OpenAPI_pdu_session_type_NULL) {
    if (cJSON_AddStringToObject(item, "pduSessType", OpenAPI_pdu_session_type_ToString(charging_pdu_session_info->pdu_sess_type)) == NULL) {
        ogs_error("OpenAPI_charging_pdu_session_info_convertToJSON() failed [pdu_sess_type]");
        goto end;
    }
    }

    if (charging_pdu_session_info->ssc_mode != OpenAPI_ssc_mode_NULL) {
    if (cJSON_AddStringToObject(item, "sscMode", OpenAPI_ssc_mode_ToString(charging_pdu_session_info->ssc_mode)) == NULL) {
        ogs_error("OpenAPI_charging_pdu_session_info_convertToJSON() failed [ssc_mode]");
        goto end;
    }
    }

    if (charging_pdu_session_info->access_types != OpenAPI_access_type_NULL) {
    cJSON *access_typesList = cJSON_AddArrayToObject(item, "accessTypes");
    if (access_typesList == NULL) {
        ogs_error("OpenAPI_charging_pdu_session_info_convertToJSON() failed [access_types]");
        goto end;
    }
    OpenAPI_list_for_each(charging_pdu_session_info->access_types, node) {
        if (cJSON_AddStringToObject(access_typesList, "", OpenAPI_access_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_charging_pdu_session_info_convertToJSON() failed [access_types]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_charging_pdu_session_info_t *OpenAPI_charging_pdu_session_info_parseFromJSON(cJSON *charging_pdu_session_infoJSON)
{
    OpenAPI_charging_pdu_session_info_t *charging_pdu_session_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *pdu_sess_type = NULL;
    OpenAPI_pdu_session_type_e pdu_sess_typeVariable = 0;
    cJSON *ssc_mode = NULL;
    OpenAPI_ssc_mode_e ssc_modeVariable = 0;
    cJSON *access_types = NULL;
    OpenAPI_list_t *access_typesList = NULL;
    pdu_sess_type = cJSON_GetObjectItemCaseSensitive(charging_pdu_session_infoJSON, "pduSessType");
    if (pdu_sess_type) {
    if (!cJSON_IsString(pdu_sess_type)) {
        ogs_error("OpenAPI_charging_pdu_session_info_parseFromJSON() failed [pdu_sess_type]");
        goto end;
    }
    pdu_sess_typeVariable = OpenAPI_pdu_session_type_FromString(pdu_sess_type->valuestring);
    }

    ssc_mode = cJSON_GetObjectItemCaseSensitive(charging_pdu_session_infoJSON, "sscMode");
    if (ssc_mode) {
    if (!cJSON_IsString(ssc_mode)) {
        ogs_error("OpenAPI_charging_pdu_session_info_parseFromJSON() failed [ssc_mode]");
        goto end;
    }
    ssc_modeVariable = OpenAPI_ssc_mode_FromString(ssc_mode->valuestring);
    }

    access_types = cJSON_GetObjectItemCaseSensitive(charging_pdu_session_infoJSON, "accessTypes");
    if (access_types) {
        cJSON *access_types_local = NULL;
        if (!cJSON_IsArray(access_types)) {
            ogs_error("OpenAPI_charging_pdu_session_info_parseFromJSON() failed [access_types]");
            goto end;
        }

        access_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(access_types_local, access_types) {
            OpenAPI_access_type_e localEnum = OpenAPI_access_type_NULL;
            if (!cJSON_IsString(access_types_local)) {
                ogs_error("OpenAPI_charging_pdu_session_info_parseFromJSON() failed [access_types]");
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
            ogs_error("OpenAPI_charging_pdu_session_info_parseFromJSON() failed: Expected access_typesList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    charging_pdu_session_info_local_var = OpenAPI_charging_pdu_session_info_create (
        pdu_sess_type ? pdu_sess_typeVariable : 0,
        ssc_mode ? ssc_modeVariable : 0,
        access_types ? access_typesList : NULL
    );

    return charging_pdu_session_info_local_var;
end:
    if (access_typesList) {
        OpenAPI_list_free(access_typesList);
        access_typesList = NULL;
    }
    return NULL;
}

OpenAPI_charging_pdu_session_info_t *OpenAPI_charging_pdu_session_info_copy(OpenAPI_charging_pdu_session_info_t *dst, OpenAPI_charging_pdu_session_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_charging_pdu_session_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_charging_pdu_session_info_convertToJSON() failed");
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

    OpenAPI_charging_pdu_session_info_free(dst);
    dst = OpenAPI_charging_pdu_session_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

