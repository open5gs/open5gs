
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "gnb_id.h"

OpenAPI_gnb_id_t *OpenAPI_gnb_id_create(
    int bit_length,
    char *g_nb_value
)
{
    OpenAPI_gnb_id_t *gnb_id_local_var = ogs_malloc(sizeof(OpenAPI_gnb_id_t));
    ogs_assert(gnb_id_local_var);

    gnb_id_local_var->bit_length = bit_length;
    gnb_id_local_var->g_nb_value = g_nb_value;

    return gnb_id_local_var;
}

void OpenAPI_gnb_id_free(OpenAPI_gnb_id_t *gnb_id)
{
    if (NULL == gnb_id) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(gnb_id->g_nb_value);
    ogs_free(gnb_id);
}

cJSON *OpenAPI_gnb_id_convertToJSON(OpenAPI_gnb_id_t *gnb_id)
{
    cJSON *item = NULL;

    if (gnb_id == NULL) {
        ogs_error("OpenAPI_gnb_id_convertToJSON() failed [GNbId]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "bitLength", gnb_id->bit_length) == NULL) {
        ogs_error("OpenAPI_gnb_id_convertToJSON() failed [bit_length]");
        goto end;
    }

    if (cJSON_AddStringToObject(item, "gNBValue", gnb_id->g_nb_value) == NULL) {
        ogs_error("OpenAPI_gnb_id_convertToJSON() failed [g_nb_value]");
        goto end;
    }

end:
    return item;
}

OpenAPI_gnb_id_t *OpenAPI_gnb_id_parseFromJSON(cJSON *gnb_idJSON)
{
    OpenAPI_gnb_id_t *gnb_id_local_var = NULL;
    cJSON *bit_length = cJSON_GetObjectItemCaseSensitive(gnb_idJSON, "bitLength");
    if (!bit_length) {
        ogs_error("OpenAPI_gnb_id_parseFromJSON() failed [bit_length]");
        goto end;
    }

    if (!cJSON_IsNumber(bit_length)) {
        ogs_error("OpenAPI_gnb_id_parseFromJSON() failed [bit_length]");
        goto end;
    }

    cJSON *g_nb_value = cJSON_GetObjectItemCaseSensitive(gnb_idJSON, "gNBValue");
    if (!g_nb_value) {
        ogs_error("OpenAPI_gnb_id_parseFromJSON() failed [g_nb_value]");
        goto end;
    }

    if (!cJSON_IsString(g_nb_value)) {
        ogs_error("OpenAPI_gnb_id_parseFromJSON() failed [g_nb_value]");
        goto end;
    }

    gnb_id_local_var = OpenAPI_gnb_id_create (
        
        bit_length->valuedouble,
        ogs_strdup(g_nb_value->valuestring)
    );

    return gnb_id_local_var;
end:
    return NULL;
}

OpenAPI_gnb_id_t *OpenAPI_gnb_id_copy(OpenAPI_gnb_id_t *dst, OpenAPI_gnb_id_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_gnb_id_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_gnb_id_convertToJSON() failed");
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

    OpenAPI_gnb_id_free(dst);
    dst = OpenAPI_gnb_id_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

