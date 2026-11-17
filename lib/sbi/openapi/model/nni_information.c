
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nni_information.h"

OpenAPI_nni_information_t *OpenAPI_nni_information_create(
    OpenAPI_nni_session_direction_e session_direction,
    OpenAPI_nni_type_e n_ni_type,
    OpenAPI_nni_relationship_mode_e relationship_mode,
    bool is_neighbour_node_address_null,
    OpenAPI_ims_address_t *neighbour_node_address
)
{
    OpenAPI_nni_information_t *nni_information_local_var = ogs_malloc(sizeof(OpenAPI_nni_information_t));
    ogs_assert(nni_information_local_var);

    nni_information_local_var->session_direction = session_direction;
    nni_information_local_var->n_ni_type = n_ni_type;
    nni_information_local_var->relationship_mode = relationship_mode;
    nni_information_local_var->is_neighbour_node_address_null = is_neighbour_node_address_null;
    nni_information_local_var->neighbour_node_address = neighbour_node_address;

    return nni_information_local_var;
}

void OpenAPI_nni_information_free(OpenAPI_nni_information_t *nni_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nni_information) {
        return;
    }
    if (nni_information->neighbour_node_address) {
        OpenAPI_ims_address_free(nni_information->neighbour_node_address);
        nni_information->neighbour_node_address = NULL;
    }
    ogs_free(nni_information);
}

cJSON *OpenAPI_nni_information_convertToJSON(OpenAPI_nni_information_t *nni_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nni_information == NULL) {
        ogs_error("OpenAPI_nni_information_convertToJSON() failed [NNIInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (nni_information->session_direction != OpenAPI_nni_session_direction_NULL) {
    if (cJSON_AddStringToObject(item, "sessionDirection", OpenAPI_nni_session_direction_ToString(nni_information->session_direction)) == NULL) {
        ogs_error("OpenAPI_nni_information_convertToJSON() failed [session_direction]");
        goto end;
    }
    }

    if (nni_information->n_ni_type != OpenAPI_nni_type_NULL) {
    if (cJSON_AddStringToObject(item, "nNIType", OpenAPI_nni_type_ToString(nni_information->n_ni_type)) == NULL) {
        ogs_error("OpenAPI_nni_information_convertToJSON() failed [n_ni_type]");
        goto end;
    }
    }

    if (nni_information->relationship_mode != OpenAPI_nni_relationship_mode_NULL) {
    if (cJSON_AddStringToObject(item, "relationshipMode", OpenAPI_nni_relationship_mode_ToString(nni_information->relationship_mode)) == NULL) {
        ogs_error("OpenAPI_nni_information_convertToJSON() failed [relationship_mode]");
        goto end;
    }
    }

    if (nni_information->neighbour_node_address) {
    cJSON *neighbour_node_address_local_JSON = OpenAPI_ims_address_convertToJSON(nni_information->neighbour_node_address);
    if (neighbour_node_address_local_JSON == NULL) {
        ogs_error("OpenAPI_nni_information_convertToJSON() failed [neighbour_node_address]");
        goto end;
    }
    cJSON_AddItemToObject(item, "neighbourNodeAddress", neighbour_node_address_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nni_information_convertToJSON() failed [neighbour_node_address]");
        goto end;
    }
    } else if (nni_information->is_neighbour_node_address_null) {
        if (cJSON_AddNullToObject(item, "neighbourNodeAddress") == NULL) {
            ogs_error("OpenAPI_nni_information_convertToJSON() failed [neighbour_node_address]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_nni_information_t *OpenAPI_nni_information_parseFromJSON(cJSON *nni_informationJSON)
{
    OpenAPI_nni_information_t *nni_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *session_direction = NULL;
    OpenAPI_nni_session_direction_e session_directionVariable = 0;
    cJSON *n_ni_type = NULL;
    OpenAPI_nni_type_e n_ni_typeVariable = 0;
    cJSON *relationship_mode = NULL;
    OpenAPI_nni_relationship_mode_e relationship_modeVariable = 0;
    cJSON *neighbour_node_address = NULL;
    OpenAPI_ims_address_t *neighbour_node_address_local_nonprim = NULL;
    session_direction = cJSON_GetObjectItemCaseSensitive(nni_informationJSON, "sessionDirection");
    if (session_direction) {
    if (!cJSON_IsString(session_direction)) {
        ogs_error("OpenAPI_nni_information_parseFromJSON() failed [session_direction]");
        goto end;
    }
    session_directionVariable = OpenAPI_nni_session_direction_FromString(session_direction->valuestring);
    }

    n_ni_type = cJSON_GetObjectItemCaseSensitive(nni_informationJSON, "nNIType");
    if (n_ni_type) {
    if (!cJSON_IsString(n_ni_type)) {
        ogs_error("OpenAPI_nni_information_parseFromJSON() failed [n_ni_type]");
        goto end;
    }
    n_ni_typeVariable = OpenAPI_nni_type_FromString(n_ni_type->valuestring);
    }

    relationship_mode = cJSON_GetObjectItemCaseSensitive(nni_informationJSON, "relationshipMode");
    if (relationship_mode) {
    if (!cJSON_IsString(relationship_mode)) {
        ogs_error("OpenAPI_nni_information_parseFromJSON() failed [relationship_mode]");
        goto end;
    }
    relationship_modeVariable = OpenAPI_nni_relationship_mode_FromString(relationship_mode->valuestring);
    }

    neighbour_node_address = cJSON_GetObjectItemCaseSensitive(nni_informationJSON, "neighbourNodeAddress");
    if (neighbour_node_address) {
    if (!cJSON_IsNull(neighbour_node_address)) {
    neighbour_node_address_local_nonprim = OpenAPI_ims_address_parseFromJSON(neighbour_node_address);
    if (!neighbour_node_address_local_nonprim) {
        ogs_error("OpenAPI_ims_address_parseFromJSON failed [neighbour_node_address]");
        goto end;
    }
    }
    }

    nni_information_local_var = OpenAPI_nni_information_create (
        session_direction ? session_directionVariable : 0,
        n_ni_type ? n_ni_typeVariable : 0,
        relationship_mode ? relationship_modeVariable : 0,
        neighbour_node_address && cJSON_IsNull(neighbour_node_address) ? true : false,
        neighbour_node_address ? neighbour_node_address_local_nonprim : NULL
    );

    return nni_information_local_var;
end:
    if (neighbour_node_address_local_nonprim) {
        OpenAPI_ims_address_free(neighbour_node_address_local_nonprim);
        neighbour_node_address_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_nni_information_t *OpenAPI_nni_information_copy(OpenAPI_nni_information_t *dst, OpenAPI_nni_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nni_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nni_information_convertToJSON() failed");
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

    OpenAPI_nni_information_free(dst);
    dst = OpenAPI_nni_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

