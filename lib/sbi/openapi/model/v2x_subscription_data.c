
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "v2x_subscription_data.h"

OpenAPI_v2x_subscription_data_t *OpenAPI_v2x_subscription_data_create(
    OpenAPI_nr_v2x_auth_t *nr_v2x_services_auth,
    OpenAPI_lte_v2x_auth_t *lte_v2x_services_auth,
    char *nr_ue_pc5_ambr,
    char *lte_pc5_ambr
)
{
    OpenAPI_v2x_subscription_data_t *v2x_subscription_data_local_var = ogs_malloc(sizeof(OpenAPI_v2x_subscription_data_t));
    ogs_assert(v2x_subscription_data_local_var);

    v2x_subscription_data_local_var->nr_v2x_services_auth = nr_v2x_services_auth;
    v2x_subscription_data_local_var->lte_v2x_services_auth = lte_v2x_services_auth;
    v2x_subscription_data_local_var->nr_ue_pc5_ambr = nr_ue_pc5_ambr;
    v2x_subscription_data_local_var->lte_pc5_ambr = lte_pc5_ambr;

    return v2x_subscription_data_local_var;
}

void OpenAPI_v2x_subscription_data_free(OpenAPI_v2x_subscription_data_t *v2x_subscription_data)
{
    if (NULL == v2x_subscription_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_nr_v2x_auth_free(v2x_subscription_data->nr_v2x_services_auth);
    OpenAPI_lte_v2x_auth_free(v2x_subscription_data->lte_v2x_services_auth);
    ogs_free(v2x_subscription_data->nr_ue_pc5_ambr);
    ogs_free(v2x_subscription_data->lte_pc5_ambr);
    ogs_free(v2x_subscription_data);
}

cJSON *OpenAPI_v2x_subscription_data_convertToJSON(OpenAPI_v2x_subscription_data_t *v2x_subscription_data)
{
    cJSON *item = NULL;

    if (v2x_subscription_data == NULL) {
        ogs_error("OpenAPI_v2x_subscription_data_convertToJSON() failed [V2xSubscriptionData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (v2x_subscription_data->nr_v2x_services_auth) {
    cJSON *nr_v2x_services_auth_local_JSON = OpenAPI_nr_v2x_auth_convertToJSON(v2x_subscription_data->nr_v2x_services_auth);
    if (nr_v2x_services_auth_local_JSON == NULL) {
        ogs_error("OpenAPI_v2x_subscription_data_convertToJSON() failed [nr_v2x_services_auth]");
        goto end;
    }
    cJSON_AddItemToObject(item, "nrV2xServicesAuth", nr_v2x_services_auth_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_v2x_subscription_data_convertToJSON() failed [nr_v2x_services_auth]");
        goto end;
    }
    }

    if (v2x_subscription_data->lte_v2x_services_auth) {
    cJSON *lte_v2x_services_auth_local_JSON = OpenAPI_lte_v2x_auth_convertToJSON(v2x_subscription_data->lte_v2x_services_auth);
    if (lte_v2x_services_auth_local_JSON == NULL) {
        ogs_error("OpenAPI_v2x_subscription_data_convertToJSON() failed [lte_v2x_services_auth]");
        goto end;
    }
    cJSON_AddItemToObject(item, "lteV2xServicesAuth", lte_v2x_services_auth_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_v2x_subscription_data_convertToJSON() failed [lte_v2x_services_auth]");
        goto end;
    }
    }

    if (v2x_subscription_data->nr_ue_pc5_ambr) {
    if (cJSON_AddStringToObject(item, "nrUePc5Ambr", v2x_subscription_data->nr_ue_pc5_ambr) == NULL) {
        ogs_error("OpenAPI_v2x_subscription_data_convertToJSON() failed [nr_ue_pc5_ambr]");
        goto end;
    }
    }

    if (v2x_subscription_data->lte_pc5_ambr) {
    if (cJSON_AddStringToObject(item, "ltePc5Ambr", v2x_subscription_data->lte_pc5_ambr) == NULL) {
        ogs_error("OpenAPI_v2x_subscription_data_convertToJSON() failed [lte_pc5_ambr]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_v2x_subscription_data_t *OpenAPI_v2x_subscription_data_parseFromJSON(cJSON *v2x_subscription_dataJSON)
{
    OpenAPI_v2x_subscription_data_t *v2x_subscription_data_local_var = NULL;
    cJSON *nr_v2x_services_auth = cJSON_GetObjectItemCaseSensitive(v2x_subscription_dataJSON, "nrV2xServicesAuth");

    OpenAPI_nr_v2x_auth_t *nr_v2x_services_auth_local_nonprim = NULL;
    if (nr_v2x_services_auth) {
    nr_v2x_services_auth_local_nonprim = OpenAPI_nr_v2x_auth_parseFromJSON(nr_v2x_services_auth);
    }

    cJSON *lte_v2x_services_auth = cJSON_GetObjectItemCaseSensitive(v2x_subscription_dataJSON, "lteV2xServicesAuth");

    OpenAPI_lte_v2x_auth_t *lte_v2x_services_auth_local_nonprim = NULL;
    if (lte_v2x_services_auth) {
    lte_v2x_services_auth_local_nonprim = OpenAPI_lte_v2x_auth_parseFromJSON(lte_v2x_services_auth);
    }

    cJSON *nr_ue_pc5_ambr = cJSON_GetObjectItemCaseSensitive(v2x_subscription_dataJSON, "nrUePc5Ambr");

    if (nr_ue_pc5_ambr) {
    if (!cJSON_IsString(nr_ue_pc5_ambr)) {
        ogs_error("OpenAPI_v2x_subscription_data_parseFromJSON() failed [nr_ue_pc5_ambr]");
        goto end;
    }
    }

    cJSON *lte_pc5_ambr = cJSON_GetObjectItemCaseSensitive(v2x_subscription_dataJSON, "ltePc5Ambr");

    if (lte_pc5_ambr) {
    if (!cJSON_IsString(lte_pc5_ambr)) {
        ogs_error("OpenAPI_v2x_subscription_data_parseFromJSON() failed [lte_pc5_ambr]");
        goto end;
    }
    }

    v2x_subscription_data_local_var = OpenAPI_v2x_subscription_data_create (
        nr_v2x_services_auth ? nr_v2x_services_auth_local_nonprim : NULL,
        lte_v2x_services_auth ? lte_v2x_services_auth_local_nonprim : NULL,
        nr_ue_pc5_ambr ? ogs_strdup(nr_ue_pc5_ambr->valuestring) : NULL,
        lte_pc5_ambr ? ogs_strdup(lte_pc5_ambr->valuestring) : NULL
    );

    return v2x_subscription_data_local_var;
end:
    return NULL;
}

OpenAPI_v2x_subscription_data_t *OpenAPI_v2x_subscription_data_copy(OpenAPI_v2x_subscription_data_t *dst, OpenAPI_v2x_subscription_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_v2x_subscription_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_v2x_subscription_data_convertToJSON() failed");
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

    OpenAPI_v2x_subscription_data_free(dst);
    dst = OpenAPI_v2x_subscription_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

