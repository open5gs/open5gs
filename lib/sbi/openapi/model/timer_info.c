
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "timer_info.h"

OpenAPI_timer_info_t *OpenAPI_timer_info_create(
    char *timer_type,
    bool is_timer_dur,
    int timer_dur
)
{
    OpenAPI_timer_info_t *timer_info_local_var = ogs_malloc(sizeof(OpenAPI_timer_info_t));
    ogs_assert(timer_info_local_var);

    timer_info_local_var->timer_type = timer_type;
    timer_info_local_var->is_timer_dur = is_timer_dur;
    timer_info_local_var->timer_dur = timer_dur;

    return timer_info_local_var;
}

void OpenAPI_timer_info_free(OpenAPI_timer_info_t *timer_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == timer_info) {
        return;
    }
    if (timer_info->timer_type) {
        ogs_free(timer_info->timer_type);
        timer_info->timer_type = NULL;
    }
    ogs_free(timer_info);
}

cJSON *OpenAPI_timer_info_convertToJSON(OpenAPI_timer_info_t *timer_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (timer_info == NULL) {
        ogs_error("OpenAPI_timer_info_convertToJSON() failed [TimerInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (timer_info->timer_type) {
    if (cJSON_AddStringToObject(item, "timerType", timer_info->timer_type) == NULL) {
        ogs_error("OpenAPI_timer_info_convertToJSON() failed [timer_type]");
        goto end;
    }
    }

    if (timer_info->is_timer_dur) {
    if (cJSON_AddNumberToObject(item, "timerDur", timer_info->timer_dur) == NULL) {
        ogs_error("OpenAPI_timer_info_convertToJSON() failed [timer_dur]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_timer_info_t *OpenAPI_timer_info_parseFromJSON(cJSON *timer_infoJSON)
{
    OpenAPI_timer_info_t *timer_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *timer_type = NULL;
    cJSON *timer_dur = NULL;
    timer_type = cJSON_GetObjectItemCaseSensitive(timer_infoJSON, "timerType");
    if (timer_type) {
    if (!cJSON_IsString(timer_type) && !cJSON_IsNull(timer_type)) {
        ogs_error("OpenAPI_timer_info_parseFromJSON() failed [timer_type]");
        goto end;
    }
    }

    timer_dur = cJSON_GetObjectItemCaseSensitive(timer_infoJSON, "timerDur");
    if (timer_dur) {
    if (!cJSON_IsNumber(timer_dur)) {
        ogs_error("OpenAPI_timer_info_parseFromJSON() failed [timer_dur]");
        goto end;
    }
    }

    timer_info_local_var = OpenAPI_timer_info_create (
        timer_type && !cJSON_IsNull(timer_type) ? ogs_strdup(timer_type->valuestring) : NULL,
        timer_dur ? true : false,
        timer_dur ? timer_dur->valuedouble : 0
    );

    return timer_info_local_var;
end:
    return NULL;
}

OpenAPI_timer_info_t *OpenAPI_timer_info_copy(OpenAPI_timer_info_t *dst, OpenAPI_timer_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_timer_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_timer_info_convertToJSON() failed");
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

    OpenAPI_timer_info_free(dst);
    dst = OpenAPI_timer_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

