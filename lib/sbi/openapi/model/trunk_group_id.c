
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "trunk_group_id.h"

OpenAPI_trunk_group_id_t *OpenAPI_trunk_group_id_create(
    char *incoming_trunk_group_id,
    char *outgoing_trunk_group_id
)
{
    OpenAPI_trunk_group_id_t *trunk_group_id_local_var = ogs_malloc(sizeof(OpenAPI_trunk_group_id_t));
    ogs_assert(trunk_group_id_local_var);

    trunk_group_id_local_var->incoming_trunk_group_id = incoming_trunk_group_id;
    trunk_group_id_local_var->outgoing_trunk_group_id = outgoing_trunk_group_id;

    return trunk_group_id_local_var;
}

void OpenAPI_trunk_group_id_free(OpenAPI_trunk_group_id_t *trunk_group_id)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == trunk_group_id) {
        return;
    }
    if (trunk_group_id->incoming_trunk_group_id) {
        ogs_free(trunk_group_id->incoming_trunk_group_id);
        trunk_group_id->incoming_trunk_group_id = NULL;
    }
    if (trunk_group_id->outgoing_trunk_group_id) {
        ogs_free(trunk_group_id->outgoing_trunk_group_id);
        trunk_group_id->outgoing_trunk_group_id = NULL;
    }
    ogs_free(trunk_group_id);
}

cJSON *OpenAPI_trunk_group_id_convertToJSON(OpenAPI_trunk_group_id_t *trunk_group_id)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (trunk_group_id == NULL) {
        ogs_error("OpenAPI_trunk_group_id_convertToJSON() failed [TrunkGroupID]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (trunk_group_id->incoming_trunk_group_id) {
    if (cJSON_AddStringToObject(item, "incomingTrunkGroupID", trunk_group_id->incoming_trunk_group_id) == NULL) {
        ogs_error("OpenAPI_trunk_group_id_convertToJSON() failed [incoming_trunk_group_id]");
        goto end;
    }
    }

    if (trunk_group_id->outgoing_trunk_group_id) {
    if (cJSON_AddStringToObject(item, "outgoingTrunkGroupID", trunk_group_id->outgoing_trunk_group_id) == NULL) {
        ogs_error("OpenAPI_trunk_group_id_convertToJSON() failed [outgoing_trunk_group_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_trunk_group_id_t *OpenAPI_trunk_group_id_parseFromJSON(cJSON *trunk_group_idJSON)
{
    OpenAPI_trunk_group_id_t *trunk_group_id_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *incoming_trunk_group_id = NULL;
    cJSON *outgoing_trunk_group_id = NULL;
    incoming_trunk_group_id = cJSON_GetObjectItemCaseSensitive(trunk_group_idJSON, "incomingTrunkGroupID");
    if (incoming_trunk_group_id) {
    if (!cJSON_IsString(incoming_trunk_group_id) && !cJSON_IsNull(incoming_trunk_group_id)) {
        ogs_error("OpenAPI_trunk_group_id_parseFromJSON() failed [incoming_trunk_group_id]");
        goto end;
    }
    }

    outgoing_trunk_group_id = cJSON_GetObjectItemCaseSensitive(trunk_group_idJSON, "outgoingTrunkGroupID");
    if (outgoing_trunk_group_id) {
    if (!cJSON_IsString(outgoing_trunk_group_id) && !cJSON_IsNull(outgoing_trunk_group_id)) {
        ogs_error("OpenAPI_trunk_group_id_parseFromJSON() failed [outgoing_trunk_group_id]");
        goto end;
    }
    }

    trunk_group_id_local_var = OpenAPI_trunk_group_id_create (
        incoming_trunk_group_id && !cJSON_IsNull(incoming_trunk_group_id) ? ogs_strdup(incoming_trunk_group_id->valuestring) : NULL,
        outgoing_trunk_group_id && !cJSON_IsNull(outgoing_trunk_group_id) ? ogs_strdup(outgoing_trunk_group_id->valuestring) : NULL
    );

    return trunk_group_id_local_var;
end:
    return NULL;
}

OpenAPI_trunk_group_id_t *OpenAPI_trunk_group_id_copy(OpenAPI_trunk_group_id_t *dst, OpenAPI_trunk_group_id_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_trunk_group_id_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_trunk_group_id_convertToJSON() failed");
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

    OpenAPI_trunk_group_id_free(dst);
    dst = OpenAPI_trunk_group_id_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

