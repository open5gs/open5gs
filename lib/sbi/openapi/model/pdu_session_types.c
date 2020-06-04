
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pdu_session_types.h"

OpenAPI_pdu_session_types_t *OpenAPI_pdu_session_types_create(
    OpenAPI_pdu_session_type_t *default_session_type,
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
    OpenAPI_pdu_session_type_free(pdu_session_types->default_session_type);
    OpenAPI_list_for_each(pdu_session_types->allowed_session_types, node) {
        OpenAPI_pdu_session_type_free(node->data);
    }
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
    cJSON *default_session_type_local_JSON = OpenAPI_pdu_session_type_convertToJSON(pdu_session_types->default_session_type);
    if (default_session_type_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_types_convertToJSON() failed [default_session_type]");
        goto end;
    }
    cJSON_AddItemToObject(item, "defaultSessionType", default_session_type_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_types_convertToJSON() failed [default_session_type]");
        goto end;
    }

    if (pdu_session_types->allowed_session_types) {
        cJSON *allowed_session_typesList = cJSON_AddArrayToObject(item, "allowedSessionTypes");
        if (allowed_session_typesList == NULL) {
            ogs_error("OpenAPI_pdu_session_types_convertToJSON() failed [allowed_session_types]");
            goto end;
        }

        OpenAPI_lnode_t *allowed_session_types_node;
        if (pdu_session_types->allowed_session_types) {
            OpenAPI_list_for_each(pdu_session_types->allowed_session_types, allowed_session_types_node) {
                cJSON *itemLocal = OpenAPI_pdu_session_type_convertToJSON(allowed_session_types_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_pdu_session_types_convertToJSON() failed [allowed_session_types]");
                    goto end;
                }
                cJSON_AddItemToArray(allowed_session_typesList, itemLocal);
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

    OpenAPI_pdu_session_type_t *default_session_type_local_nonprim = NULL;

    default_session_type_local_nonprim = OpenAPI_pdu_session_type_parseFromJSON(default_session_type);

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
            if (!cJSON_IsObject(allowed_session_types_local_nonprimitive)) {
                ogs_error("OpenAPI_pdu_session_types_parseFromJSON() failed [allowed_session_types]");
                goto end;
            }
            OpenAPI_pdu_session_type_t *allowed_session_typesItem = OpenAPI_pdu_session_type_parseFromJSON(allowed_session_types_local_nonprimitive);

            OpenAPI_list_add(allowed_session_typesList, allowed_session_typesItem);
        }
    }

    pdu_session_types_local_var = OpenAPI_pdu_session_types_create (
        default_session_type_local_nonprim,
        allowed_session_types ? allowed_session_typesList : NULL
        );

    return pdu_session_types_local_var;
end:
    return NULL;
}

