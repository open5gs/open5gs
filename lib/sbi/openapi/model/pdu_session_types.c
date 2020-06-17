
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pdu_session_types.h"

OpenAPI_pdu_session_types_t *OpenAPI_pdu_session_types_create(
    OpenAPI_pdu_session_type_e default_session_type,
    OpenAPI_list_t *allowed_session_types
    )
{
    OpenAPI_pdu_session_types_t *pdu_session_types_local_var = OpenAPI_malloc(sizeof(OpenAPI_pdu_session_types_t));
    if (!pdu_session_types_local_var) {
        return NULL;
    }
    pdu_session_types_local_var->default_session_type = default_session_type;
    pdu_session_types_local_var->allowed_session_types = allowed_session_types;

    return pdu_session_types_local_var;
}

void OpenAPI_pdu_session_types_free(OpenAPI_pdu_session_types_t *pdu_session_types)
{
    if (NULL == pdu_session_types) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_free(pdu_session_types->allowed_session_types);
    ogs_free(pdu_session_types);
}

cJSON *OpenAPI_pdu_session_types_convertToJSON(OpenAPI_pdu_session_types_t *pdu_session_types)
{
    cJSON *item = NULL;

    if (pdu_session_types == NULL) {
        ogs_error("OpenAPI_pdu_session_types_convertToJSON() failed [PduSessionTypes]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!pdu_session_types->default_session_type) {
        ogs_error("OpenAPI_pdu_session_types_convertToJSON() failed [default_session_type]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "defaultSessionType", OpenAPI_pdu_session_type_ToString(pdu_session_types->default_session_type)) == NULL) {
        ogs_error("OpenAPI_pdu_session_types_convertToJSON() failed [default_session_type]");
        goto end;
    }

    if (pdu_session_types->allowed_session_types) {
        cJSON *allowed_session_types = cJSON_AddArrayToObject(item, "allowedSessionTypes");
        if (allowed_session_types == NULL) {
            ogs_error("OpenAPI_pdu_session_types_convertToJSON() failed [allowed_session_types]");
            goto end;
        }
        OpenAPI_lnode_t *allowed_session_types_node;
        OpenAPI_list_for_each(pdu_session_types->allowed_session_types, allowed_session_types_node) {
            if (cJSON_AddStringToObject(allowed_session_types, "", OpenAPI_pdu_session_type_ToString((OpenAPI_pdu_session_type_e)allowed_session_types_node->data)) == NULL) {
                ogs_error("OpenAPI_pdu_session_types_convertToJSON() failed [allowed_session_types]");
                goto end;
            }
        }
    }

end:
    return item;
}

OpenAPI_pdu_session_types_t *OpenAPI_pdu_session_types_parseFromJSON(cJSON *pdu_session_typesJSON)
{
    OpenAPI_pdu_session_types_t *pdu_session_types_local_var = NULL;
    cJSON *default_session_type = cJSON_GetObjectItemCaseSensitive(pdu_session_typesJSON, "defaultSessionType");
    if (!default_session_type) {
        ogs_error("OpenAPI_pdu_session_types_parseFromJSON() failed [default_session_type]");
        goto end;
    }

    OpenAPI_pdu_session_type_e default_session_typeVariable;

    if (!cJSON_IsString(default_session_type)) {
        ogs_error("OpenAPI_pdu_session_types_parseFromJSON() failed [default_session_type]");
        goto end;
    }
    default_session_typeVariable = OpenAPI_pdu_session_type_FromString(default_session_type->valuestring);

    cJSON *allowed_session_types = cJSON_GetObjectItemCaseSensitive(pdu_session_typesJSON, "allowedSessionTypes");

    OpenAPI_list_t *allowed_session_typesList;
    if (allowed_session_types) {
        cJSON *allowed_session_types_local_nonprimitive;
        if (!cJSON_IsArray(allowed_session_types)) {
            ogs_error("OpenAPI_pdu_session_types_parseFromJSON() failed [allowed_session_types]");
            goto end;
        }

        allowed_session_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_session_types_local_nonprimitive, allowed_session_types ) {
            if (!cJSON_IsString(allowed_session_types_local_nonprimitive)) {
                ogs_error("OpenAPI_pdu_session_types_parseFromJSON() failed [allowed_session_types]");
                goto end;
            }

            OpenAPI_list_add(allowed_session_typesList, (void *)OpenAPI_pdu_session_type_FromString(allowed_session_types_local_nonprimitive->valuestring));
        }
    }

    pdu_session_types_local_var = OpenAPI_pdu_session_types_create (
        default_session_typeVariable,
        allowed_session_types ? allowed_session_typesList : NULL
        );

    return pdu_session_types_local_var;
end:
    return NULL;
}

OpenAPI_pdu_session_types_t *OpenAPI_pdu_session_types_copy(OpenAPI_pdu_session_types_t *dst, OpenAPI_pdu_session_types_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pdu_session_types_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pdu_session_types_convertToJSON() failed");
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

    OpenAPI_pdu_session_types_free(dst);
    dst = OpenAPI_pdu_session_types_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

