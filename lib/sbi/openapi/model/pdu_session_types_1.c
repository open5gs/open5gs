
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pdu_session_types_1.h"

OpenAPI_pdu_session_types_1_t *OpenAPI_pdu_session_types_1_create(
    OpenAPI_pdu_session_type_e default_session_type,
    OpenAPI_list_t *allowed_session_types
)
{
    OpenAPI_pdu_session_types_1_t *pdu_session_types_1_local_var = ogs_malloc(sizeof(OpenAPI_pdu_session_types_1_t));
    ogs_assert(pdu_session_types_1_local_var);

    pdu_session_types_1_local_var->default_session_type = default_session_type;
    pdu_session_types_1_local_var->allowed_session_types = allowed_session_types;

    return pdu_session_types_1_local_var;
}

void OpenAPI_pdu_session_types_1_free(OpenAPI_pdu_session_types_1_t *pdu_session_types_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pdu_session_types_1) {
        return;
    }
    if (pdu_session_types_1->allowed_session_types) {
        OpenAPI_list_free(pdu_session_types_1->allowed_session_types);
        pdu_session_types_1->allowed_session_types = NULL;
    }
    ogs_free(pdu_session_types_1);
}

cJSON *OpenAPI_pdu_session_types_1_convertToJSON(OpenAPI_pdu_session_types_1_t *pdu_session_types_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pdu_session_types_1 == NULL) {
        ogs_error("OpenAPI_pdu_session_types_1_convertToJSON() failed [PduSessionTypes_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pdu_session_types_1->default_session_type != OpenAPI_pdu_session_type_NULL) {
    if (cJSON_AddStringToObject(item, "defaultSessionType", OpenAPI_pdu_session_type_ToString(pdu_session_types_1->default_session_type)) == NULL) {
        ogs_error("OpenAPI_pdu_session_types_1_convertToJSON() failed [default_session_type]");
        goto end;
    }
    }

    if (pdu_session_types_1->allowed_session_types != OpenAPI_pdu_session_type_NULL) {
    cJSON *allowed_session_typesList = cJSON_AddArrayToObject(item, "allowedSessionTypes");
    if (allowed_session_typesList == NULL) {
        ogs_error("OpenAPI_pdu_session_types_1_convertToJSON() failed [allowed_session_types]");
        goto end;
    }
    OpenAPI_list_for_each(pdu_session_types_1->allowed_session_types, node) {
        if (cJSON_AddStringToObject(allowed_session_typesList, "", OpenAPI_pdu_session_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_pdu_session_types_1_convertToJSON() failed [allowed_session_types]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_pdu_session_types_1_t *OpenAPI_pdu_session_types_1_parseFromJSON(cJSON *pdu_session_types_1JSON)
{
    OpenAPI_pdu_session_types_1_t *pdu_session_types_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *default_session_type = NULL;
    OpenAPI_pdu_session_type_e default_session_typeVariable = 0;
    cJSON *allowed_session_types = NULL;
    OpenAPI_list_t *allowed_session_typesList = NULL;
    default_session_type = cJSON_GetObjectItemCaseSensitive(pdu_session_types_1JSON, "defaultSessionType");
    if (default_session_type) {
    if (!cJSON_IsString(default_session_type)) {
        ogs_error("OpenAPI_pdu_session_types_1_parseFromJSON() failed [default_session_type]");
        goto end;
    }
    default_session_typeVariable = OpenAPI_pdu_session_type_FromString(default_session_type->valuestring);
    }

    allowed_session_types = cJSON_GetObjectItemCaseSensitive(pdu_session_types_1JSON, "allowedSessionTypes");
    if (allowed_session_types) {
        cJSON *allowed_session_types_local = NULL;
        if (!cJSON_IsArray(allowed_session_types)) {
            ogs_error("OpenAPI_pdu_session_types_1_parseFromJSON() failed [allowed_session_types]");
            goto end;
        }

        allowed_session_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_session_types_local, allowed_session_types) {
            OpenAPI_pdu_session_type_e localEnum = OpenAPI_pdu_session_type_NULL;
            if (!cJSON_IsString(allowed_session_types_local)) {
                ogs_error("OpenAPI_pdu_session_types_1_parseFromJSON() failed [allowed_session_types]");
                goto end;
            }
            localEnum = OpenAPI_pdu_session_type_FromString(allowed_session_types_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"allowed_session_types\" is not supported. Ignoring it ...",
                         allowed_session_types_local->valuestring);
            } else {
                OpenAPI_list_add(allowed_session_typesList, (void *)localEnum);
            }
        }
        if (allowed_session_typesList->count == 0) {
            ogs_error("OpenAPI_pdu_session_types_1_parseFromJSON() failed: Expected allowed_session_typesList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    pdu_session_types_1_local_var = OpenAPI_pdu_session_types_1_create (
        default_session_type ? default_session_typeVariable : 0,
        allowed_session_types ? allowed_session_typesList : NULL
    );

    return pdu_session_types_1_local_var;
end:
    if (allowed_session_typesList) {
        OpenAPI_list_free(allowed_session_typesList);
        allowed_session_typesList = NULL;
    }
    return NULL;
}

OpenAPI_pdu_session_types_1_t *OpenAPI_pdu_session_types_1_copy(OpenAPI_pdu_session_types_1_t *dst, OpenAPI_pdu_session_types_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pdu_session_types_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pdu_session_types_1_convertToJSON() failed");
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

    OpenAPI_pdu_session_types_1_free(dst);
    dst = OpenAPI_pdu_session_types_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

