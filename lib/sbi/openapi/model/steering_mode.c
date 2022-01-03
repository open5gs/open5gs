
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "steering_mode.h"

OpenAPI_steering_mode_t *OpenAPI_steering_mode_create(
    OpenAPI_steer_mode_value_e steer_mode_value,
    OpenAPI_access_type_e active,
    OpenAPI_access_type_rm_t *standby,
    bool is__3g_load,
    int _3g_load,
    OpenAPI_access_type_e prio_acc
)
{
    OpenAPI_steering_mode_t *steering_mode_local_var = ogs_malloc(sizeof(OpenAPI_steering_mode_t));
    ogs_assert(steering_mode_local_var);

    steering_mode_local_var->steer_mode_value = steer_mode_value;
    steering_mode_local_var->active = active;
    steering_mode_local_var->standby = standby;
    steering_mode_local_var->is__3g_load = is__3g_load;
    steering_mode_local_var->_3g_load = _3g_load;
    steering_mode_local_var->prio_acc = prio_acc;

    return steering_mode_local_var;
}

void OpenAPI_steering_mode_free(OpenAPI_steering_mode_t *steering_mode)
{
    if (NULL == steering_mode) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_access_type_rm_free(steering_mode->standby);
    ogs_free(steering_mode);
}

cJSON *OpenAPI_steering_mode_convertToJSON(OpenAPI_steering_mode_t *steering_mode)
{
    cJSON *item = NULL;

    if (steering_mode == NULL) {
        ogs_error("OpenAPI_steering_mode_convertToJSON() failed [SteeringMode]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "steerModeValue", OpenAPI_steer_mode_value_ToString(steering_mode->steer_mode_value)) == NULL) {
        ogs_error("OpenAPI_steering_mode_convertToJSON() failed [steer_mode_value]");
        goto end;
    }

    if (steering_mode->active) {
    if (cJSON_AddStringToObject(item, "active", OpenAPI_access_type_ToString(steering_mode->active)) == NULL) {
        ogs_error("OpenAPI_steering_mode_convertToJSON() failed [active]");
        goto end;
    }
    }

    if (steering_mode->standby) {
    cJSON *standby_local_JSON = OpenAPI_access_type_rm_convertToJSON(steering_mode->standby);
    if (standby_local_JSON == NULL) {
        ogs_error("OpenAPI_steering_mode_convertToJSON() failed [standby]");
        goto end;
    }
    cJSON_AddItemToObject(item, "standby", standby_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_steering_mode_convertToJSON() failed [standby]");
        goto end;
    }
    }

    if (steering_mode->is__3g_load) {
    if (cJSON_AddNumberToObject(item, "3gLoad", steering_mode->_3g_load) == NULL) {
        ogs_error("OpenAPI_steering_mode_convertToJSON() failed [_3g_load]");
        goto end;
    }
    }

    if (steering_mode->prio_acc) {
    if (cJSON_AddStringToObject(item, "prioAcc", OpenAPI_access_type_ToString(steering_mode->prio_acc)) == NULL) {
        ogs_error("OpenAPI_steering_mode_convertToJSON() failed [prio_acc]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_steering_mode_t *OpenAPI_steering_mode_parseFromJSON(cJSON *steering_modeJSON)
{
    OpenAPI_steering_mode_t *steering_mode_local_var = NULL;
    cJSON *steer_mode_value = cJSON_GetObjectItemCaseSensitive(steering_modeJSON, "steerModeValue");
    if (!steer_mode_value) {
        ogs_error("OpenAPI_steering_mode_parseFromJSON() failed [steer_mode_value]");
        goto end;
    }

    OpenAPI_steer_mode_value_e steer_mode_valueVariable;
    if (!cJSON_IsString(steer_mode_value)) {
        ogs_error("OpenAPI_steering_mode_parseFromJSON() failed [steer_mode_value]");
        goto end;
    }
    steer_mode_valueVariable = OpenAPI_steer_mode_value_FromString(steer_mode_value->valuestring);

    cJSON *active = cJSON_GetObjectItemCaseSensitive(steering_modeJSON, "active");

    OpenAPI_access_type_e activeVariable;
    if (active) {
    if (!cJSON_IsString(active)) {
        ogs_error("OpenAPI_steering_mode_parseFromJSON() failed [active]");
        goto end;
    }
    activeVariable = OpenAPI_access_type_FromString(active->valuestring);
    }

    cJSON *standby = cJSON_GetObjectItemCaseSensitive(steering_modeJSON, "standby");

    OpenAPI_access_type_rm_t *standby_local_nonprim = NULL;
    if (standby) {
    standby_local_nonprim = OpenAPI_access_type_rm_parseFromJSON(standby);
    }

    cJSON *_3g_load = cJSON_GetObjectItemCaseSensitive(steering_modeJSON, "3gLoad");

    if (_3g_load) {
    if (!cJSON_IsNumber(_3g_load)) {
        ogs_error("OpenAPI_steering_mode_parseFromJSON() failed [_3g_load]");
        goto end;
    }
    }

    cJSON *prio_acc = cJSON_GetObjectItemCaseSensitive(steering_modeJSON, "prioAcc");

    OpenAPI_access_type_e prio_accVariable;
    if (prio_acc) {
    if (!cJSON_IsString(prio_acc)) {
        ogs_error("OpenAPI_steering_mode_parseFromJSON() failed [prio_acc]");
        goto end;
    }
    prio_accVariable = OpenAPI_access_type_FromString(prio_acc->valuestring);
    }

    steering_mode_local_var = OpenAPI_steering_mode_create (
        steer_mode_valueVariable,
        active ? activeVariable : 0,
        standby ? standby_local_nonprim : NULL,
        _3g_load ? true : false,
        _3g_load ? _3g_load->valuedouble : 0,
        prio_acc ? prio_accVariable : 0
    );

    return steering_mode_local_var;
end:
    return NULL;
}

OpenAPI_steering_mode_t *OpenAPI_steering_mode_copy(OpenAPI_steering_mode_t *dst, OpenAPI_steering_mode_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_steering_mode_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_steering_mode_convertToJSON() failed");
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

    OpenAPI_steering_mode_free(dst);
    dst = OpenAPI_steering_mode_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

