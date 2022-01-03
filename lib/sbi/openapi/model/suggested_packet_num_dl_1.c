
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "suggested_packet_num_dl_1.h"

OpenAPI_suggested_packet_num_dl_1_t *OpenAPI_suggested_packet_num_dl_1_create(
    int suggested_packet_num_dl,
    char *validity_time
)
{
    OpenAPI_suggested_packet_num_dl_1_t *suggested_packet_num_dl_1_local_var = ogs_malloc(sizeof(OpenAPI_suggested_packet_num_dl_1_t));
    ogs_assert(suggested_packet_num_dl_1_local_var);

    suggested_packet_num_dl_1_local_var->suggested_packet_num_dl = suggested_packet_num_dl;
    suggested_packet_num_dl_1_local_var->validity_time = validity_time;

    return suggested_packet_num_dl_1_local_var;
}

void OpenAPI_suggested_packet_num_dl_1_free(OpenAPI_suggested_packet_num_dl_1_t *suggested_packet_num_dl_1)
{
    if (NULL == suggested_packet_num_dl_1) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(suggested_packet_num_dl_1->validity_time);
    ogs_free(suggested_packet_num_dl_1);
}

cJSON *OpenAPI_suggested_packet_num_dl_1_convertToJSON(OpenAPI_suggested_packet_num_dl_1_t *suggested_packet_num_dl_1)
{
    cJSON *item = NULL;

    if (suggested_packet_num_dl_1 == NULL) {
        ogs_error("OpenAPI_suggested_packet_num_dl_1_convertToJSON() failed [SuggestedPacketNumDl_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "suggestedPacketNumDl", suggested_packet_num_dl_1->suggested_packet_num_dl) == NULL) {
        ogs_error("OpenAPI_suggested_packet_num_dl_1_convertToJSON() failed [suggested_packet_num_dl]");
        goto end;
    }

    if (suggested_packet_num_dl_1->validity_time) {
    if (cJSON_AddStringToObject(item, "validityTime", suggested_packet_num_dl_1->validity_time) == NULL) {
        ogs_error("OpenAPI_suggested_packet_num_dl_1_convertToJSON() failed [validity_time]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_suggested_packet_num_dl_1_t *OpenAPI_suggested_packet_num_dl_1_parseFromJSON(cJSON *suggested_packet_num_dl_1JSON)
{
    OpenAPI_suggested_packet_num_dl_1_t *suggested_packet_num_dl_1_local_var = NULL;
    cJSON *suggested_packet_num_dl = cJSON_GetObjectItemCaseSensitive(suggested_packet_num_dl_1JSON, "suggestedPacketNumDl");
    if (!suggested_packet_num_dl) {
        ogs_error("OpenAPI_suggested_packet_num_dl_1_parseFromJSON() failed [suggested_packet_num_dl]");
        goto end;
    }

    if (!cJSON_IsNumber(suggested_packet_num_dl)) {
        ogs_error("OpenAPI_suggested_packet_num_dl_1_parseFromJSON() failed [suggested_packet_num_dl]");
        goto end;
    }

    cJSON *validity_time = cJSON_GetObjectItemCaseSensitive(suggested_packet_num_dl_1JSON, "validityTime");

    if (validity_time) {
    if (!cJSON_IsString(validity_time)) {
        ogs_error("OpenAPI_suggested_packet_num_dl_1_parseFromJSON() failed [validity_time]");
        goto end;
    }
    }

    suggested_packet_num_dl_1_local_var = OpenAPI_suggested_packet_num_dl_1_create (
        
        suggested_packet_num_dl->valuedouble,
        validity_time ? ogs_strdup(validity_time->valuestring) : NULL
    );

    return suggested_packet_num_dl_1_local_var;
end:
    return NULL;
}

OpenAPI_suggested_packet_num_dl_1_t *OpenAPI_suggested_packet_num_dl_1_copy(OpenAPI_suggested_packet_num_dl_1_t *dst, OpenAPI_suggested_packet_num_dl_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_suggested_packet_num_dl_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_suggested_packet_num_dl_1_convertToJSON() failed");
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

    OpenAPI_suggested_packet_num_dl_1_free(dst);
    dst = OpenAPI_suggested_packet_num_dl_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

