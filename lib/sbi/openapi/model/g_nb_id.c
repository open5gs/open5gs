
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "g_nb_id.h"

OpenAPI_g_nb_id_t *OpenAPI_g_nb_id_create(
    int bit_length,
    char *g_nb_value
    )
{
    OpenAPI_g_nb_id_t *g_nb_id_local_var = OpenAPI_malloc(sizeof(OpenAPI_g_nb_id_t));
    if (!g_nb_id_local_var) {
        return NULL;
    }
    g_nb_id_local_var->bit_length = bit_length;
    g_nb_id_local_var->g_nb_value = g_nb_value;

    return g_nb_id_local_var;
}

void OpenAPI_g_nb_id_free(OpenAPI_g_nb_id_t *g_nb_id)
{
    if (NULL == g_nb_id) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(g_nb_id->g_nb_value);
    ogs_free(g_nb_id);
}

cJSON *OpenAPI_g_nb_id_convertToJSON(OpenAPI_g_nb_id_t *g_nb_id)
{
    cJSON *item = NULL;

    if (g_nb_id == NULL) {
        ogs_error("OpenAPI_g_nb_id_convertToJSON() failed [GNbId]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!g_nb_id->bit_length) {
        ogs_error("OpenAPI_g_nb_id_convertToJSON() failed [bit_length]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "bitLength", g_nb_id->bit_length) == NULL) {
        ogs_error("OpenAPI_g_nb_id_convertToJSON() failed [bit_length]");
        goto end;
    }

    if (!g_nb_id->g_nb_value) {
        ogs_error("OpenAPI_g_nb_id_convertToJSON() failed [g_nb_value]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "gNBValue", g_nb_id->g_nb_value) == NULL) {
        ogs_error("OpenAPI_g_nb_id_convertToJSON() failed [g_nb_value]");
        goto end;
    }

end:
    return item;
}

OpenAPI_g_nb_id_t *OpenAPI_g_nb_id_parseFromJSON(cJSON *g_nb_idJSON)
{
    OpenAPI_g_nb_id_t *g_nb_id_local_var = NULL;
    cJSON *bit_length = cJSON_GetObjectItemCaseSensitive(g_nb_idJSON, "bitLength");
    if (!bit_length) {
        ogs_error("OpenAPI_g_nb_id_parseFromJSON() failed [bit_length]");
        goto end;
    }


    if (!cJSON_IsNumber(bit_length)) {
        ogs_error("OpenAPI_g_nb_id_parseFromJSON() failed [bit_length]");
        goto end;
    }

    cJSON *g_nb_value = cJSON_GetObjectItemCaseSensitive(g_nb_idJSON, "gNBValue");
    if (!g_nb_value) {
        ogs_error("OpenAPI_g_nb_id_parseFromJSON() failed [g_nb_value]");
        goto end;
    }


    if (!cJSON_IsString(g_nb_value)) {
        ogs_error("OpenAPI_g_nb_id_parseFromJSON() failed [g_nb_value]");
        goto end;
    }

    g_nb_id_local_var = OpenAPI_g_nb_id_create (
        bit_length->valuedouble,
        ogs_strdup(g_nb_value->valuestring)
        );

    return g_nb_id_local_var;
end:
    return NULL;
}

OpenAPI_g_nb_id_t *OpenAPI_g_nb_id_copy(OpenAPI_g_nb_id_t *dst, OpenAPI_g_nb_id_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_g_nb_id_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_g_nb_id_convertToJSON() failed");
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

    OpenAPI_g_nb_id_free(dst);
    dst = OpenAPI_g_nb_id_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

