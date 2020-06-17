
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lte_v2x_auth.h"

OpenAPI_lte_v2x_auth_t *OpenAPI_lte_v2x_auth_create(
    OpenAPI_ue_auth_t *vehicle_ue_auth,
    OpenAPI_ue_auth_t *pedestrian_ue_auth
    )
{
    OpenAPI_lte_v2x_auth_t *lte_v2x_auth_local_var = OpenAPI_malloc(sizeof(OpenAPI_lte_v2x_auth_t));
    if (!lte_v2x_auth_local_var) {
        return NULL;
    }
    lte_v2x_auth_local_var->vehicle_ue_auth = vehicle_ue_auth;
    lte_v2x_auth_local_var->pedestrian_ue_auth = pedestrian_ue_auth;

    return lte_v2x_auth_local_var;
}

void OpenAPI_lte_v2x_auth_free(OpenAPI_lte_v2x_auth_t *lte_v2x_auth)
{
    if (NULL == lte_v2x_auth) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_ue_auth_free(lte_v2x_auth->vehicle_ue_auth);
    OpenAPI_ue_auth_free(lte_v2x_auth->pedestrian_ue_auth);
    ogs_free(lte_v2x_auth);
}

cJSON *OpenAPI_lte_v2x_auth_convertToJSON(OpenAPI_lte_v2x_auth_t *lte_v2x_auth)
{
    cJSON *item = NULL;

    if (lte_v2x_auth == NULL) {
        ogs_error("OpenAPI_lte_v2x_auth_convertToJSON() failed [LteV2xAuth]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (lte_v2x_auth->vehicle_ue_auth) {
        cJSON *vehicle_ue_auth_local_JSON = OpenAPI_ue_auth_convertToJSON(lte_v2x_auth->vehicle_ue_auth);
        if (vehicle_ue_auth_local_JSON == NULL) {
            ogs_error("OpenAPI_lte_v2x_auth_convertToJSON() failed [vehicle_ue_auth]");
            goto end;
        }
        cJSON_AddItemToObject(item, "vehicleUeAuth", vehicle_ue_auth_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_lte_v2x_auth_convertToJSON() failed [vehicle_ue_auth]");
            goto end;
        }
    }

    if (lte_v2x_auth->pedestrian_ue_auth) {
        cJSON *pedestrian_ue_auth_local_JSON = OpenAPI_ue_auth_convertToJSON(lte_v2x_auth->pedestrian_ue_auth);
        if (pedestrian_ue_auth_local_JSON == NULL) {
            ogs_error("OpenAPI_lte_v2x_auth_convertToJSON() failed [pedestrian_ue_auth]");
            goto end;
        }
        cJSON_AddItemToObject(item, "pedestrianUeAuth", pedestrian_ue_auth_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_lte_v2x_auth_convertToJSON() failed [pedestrian_ue_auth]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_lte_v2x_auth_t *OpenAPI_lte_v2x_auth_parseFromJSON(cJSON *lte_v2x_authJSON)
{
    OpenAPI_lte_v2x_auth_t *lte_v2x_auth_local_var = NULL;
    cJSON *vehicle_ue_auth = cJSON_GetObjectItemCaseSensitive(lte_v2x_authJSON, "vehicleUeAuth");

    OpenAPI_ue_auth_t *vehicle_ue_auth_local_nonprim = NULL;
    if (vehicle_ue_auth) {
        vehicle_ue_auth_local_nonprim = OpenAPI_ue_auth_parseFromJSON(vehicle_ue_auth);
    }

    cJSON *pedestrian_ue_auth = cJSON_GetObjectItemCaseSensitive(lte_v2x_authJSON, "pedestrianUeAuth");

    OpenAPI_ue_auth_t *pedestrian_ue_auth_local_nonprim = NULL;
    if (pedestrian_ue_auth) {
        pedestrian_ue_auth_local_nonprim = OpenAPI_ue_auth_parseFromJSON(pedestrian_ue_auth);
    }

    lte_v2x_auth_local_var = OpenAPI_lte_v2x_auth_create (
        vehicle_ue_auth ? vehicle_ue_auth_local_nonprim : NULL,
        pedestrian_ue_auth ? pedestrian_ue_auth_local_nonprim : NULL
        );

    return lte_v2x_auth_local_var;
end:
    return NULL;
}

OpenAPI_lte_v2x_auth_t *OpenAPI_lte_v2x_auth_copy(OpenAPI_lte_v2x_auth_t *dst, OpenAPI_lte_v2x_auth_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_lte_v2x_auth_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_lte_v2x_auth_convertToJSON() failed");
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

    OpenAPI_lte_v2x_auth_free(dst);
    dst = OpenAPI_lte_v2x_auth_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

