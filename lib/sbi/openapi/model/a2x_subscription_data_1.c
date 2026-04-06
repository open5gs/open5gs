
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "a2x_subscription_data_1.h"

OpenAPI_a2x_subscription_data_1_t *OpenAPI_a2x_subscription_data_1_create(
    OpenAPI_nr_a2x_auth_1_t *nr_a2x_services_auth,
    OpenAPI_lte_a2x_auth_1_t *lte_a2x_services_auth,
    char *nr_ue_pc5_ambr,
    char *lte_pc5_ambr
)
{
    OpenAPI_a2x_subscription_data_1_t *a2x_subscription_data_1_local_var = ogs_malloc(sizeof(OpenAPI_a2x_subscription_data_1_t));
    ogs_assert(a2x_subscription_data_1_local_var);

    a2x_subscription_data_1_local_var->nr_a2x_services_auth = nr_a2x_services_auth;
    a2x_subscription_data_1_local_var->lte_a2x_services_auth = lte_a2x_services_auth;
    a2x_subscription_data_1_local_var->nr_ue_pc5_ambr = nr_ue_pc5_ambr;
    a2x_subscription_data_1_local_var->lte_pc5_ambr = lte_pc5_ambr;

    return a2x_subscription_data_1_local_var;
}

void OpenAPI_a2x_subscription_data_1_free(OpenAPI_a2x_subscription_data_1_t *a2x_subscription_data_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == a2x_subscription_data_1) {
        return;
    }
    if (a2x_subscription_data_1->nr_a2x_services_auth) {
        OpenAPI_nr_a2x_auth_1_free(a2x_subscription_data_1->nr_a2x_services_auth);
        a2x_subscription_data_1->nr_a2x_services_auth = NULL;
    }
    if (a2x_subscription_data_1->lte_a2x_services_auth) {
        OpenAPI_lte_a2x_auth_1_free(a2x_subscription_data_1->lte_a2x_services_auth);
        a2x_subscription_data_1->lte_a2x_services_auth = NULL;
    }
    if (a2x_subscription_data_1->nr_ue_pc5_ambr) {
        ogs_free(a2x_subscription_data_1->nr_ue_pc5_ambr);
        a2x_subscription_data_1->nr_ue_pc5_ambr = NULL;
    }
    if (a2x_subscription_data_1->lte_pc5_ambr) {
        ogs_free(a2x_subscription_data_1->lte_pc5_ambr);
        a2x_subscription_data_1->lte_pc5_ambr = NULL;
    }
    ogs_free(a2x_subscription_data_1);
}

cJSON *OpenAPI_a2x_subscription_data_1_convertToJSON(OpenAPI_a2x_subscription_data_1_t *a2x_subscription_data_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (a2x_subscription_data_1 == NULL) {
        ogs_error("OpenAPI_a2x_subscription_data_1_convertToJSON() failed [A2xSubscriptionData_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (a2x_subscription_data_1->nr_a2x_services_auth) {
    cJSON *nr_a2x_services_auth_local_JSON = OpenAPI_nr_a2x_auth_1_convertToJSON(a2x_subscription_data_1->nr_a2x_services_auth);
    if (nr_a2x_services_auth_local_JSON == NULL) {
        ogs_error("OpenAPI_a2x_subscription_data_1_convertToJSON() failed [nr_a2x_services_auth]");
        goto end;
    }
    cJSON_AddItemToObject(item, "nrA2xServicesAuth", nr_a2x_services_auth_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_a2x_subscription_data_1_convertToJSON() failed [nr_a2x_services_auth]");
        goto end;
    }
    }

    if (a2x_subscription_data_1->lte_a2x_services_auth) {
    cJSON *lte_a2x_services_auth_local_JSON = OpenAPI_lte_a2x_auth_1_convertToJSON(a2x_subscription_data_1->lte_a2x_services_auth);
    if (lte_a2x_services_auth_local_JSON == NULL) {
        ogs_error("OpenAPI_a2x_subscription_data_1_convertToJSON() failed [lte_a2x_services_auth]");
        goto end;
    }
    cJSON_AddItemToObject(item, "lteA2xServicesAuth", lte_a2x_services_auth_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_a2x_subscription_data_1_convertToJSON() failed [lte_a2x_services_auth]");
        goto end;
    }
    }

    if (a2x_subscription_data_1->nr_ue_pc5_ambr) {
    if (cJSON_AddStringToObject(item, "nrUePc5Ambr", a2x_subscription_data_1->nr_ue_pc5_ambr) == NULL) {
        ogs_error("OpenAPI_a2x_subscription_data_1_convertToJSON() failed [nr_ue_pc5_ambr]");
        goto end;
    }
    }

    if (a2x_subscription_data_1->lte_pc5_ambr) {
    if (cJSON_AddStringToObject(item, "ltePc5Ambr", a2x_subscription_data_1->lte_pc5_ambr) == NULL) {
        ogs_error("OpenAPI_a2x_subscription_data_1_convertToJSON() failed [lte_pc5_ambr]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_a2x_subscription_data_1_t *OpenAPI_a2x_subscription_data_1_parseFromJSON(cJSON *a2x_subscription_data_1JSON)
{
    OpenAPI_a2x_subscription_data_1_t *a2x_subscription_data_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *nr_a2x_services_auth = NULL;
    OpenAPI_nr_a2x_auth_1_t *nr_a2x_services_auth_local_nonprim = NULL;
    cJSON *lte_a2x_services_auth = NULL;
    OpenAPI_lte_a2x_auth_1_t *lte_a2x_services_auth_local_nonprim = NULL;
    cJSON *nr_ue_pc5_ambr = NULL;
    cJSON *lte_pc5_ambr = NULL;
    nr_a2x_services_auth = cJSON_GetObjectItemCaseSensitive(a2x_subscription_data_1JSON, "nrA2xServicesAuth");
    if (nr_a2x_services_auth) {
    nr_a2x_services_auth_local_nonprim = OpenAPI_nr_a2x_auth_1_parseFromJSON(nr_a2x_services_auth);
    if (!nr_a2x_services_auth_local_nonprim) {
        ogs_error("OpenAPI_nr_a2x_auth_1_parseFromJSON failed [nr_a2x_services_auth]");
        goto end;
    }
    }

    lte_a2x_services_auth = cJSON_GetObjectItemCaseSensitive(a2x_subscription_data_1JSON, "lteA2xServicesAuth");
    if (lte_a2x_services_auth) {
    lte_a2x_services_auth_local_nonprim = OpenAPI_lte_a2x_auth_1_parseFromJSON(lte_a2x_services_auth);
    if (!lte_a2x_services_auth_local_nonprim) {
        ogs_error("OpenAPI_lte_a2x_auth_1_parseFromJSON failed [lte_a2x_services_auth]");
        goto end;
    }
    }

    nr_ue_pc5_ambr = cJSON_GetObjectItemCaseSensitive(a2x_subscription_data_1JSON, "nrUePc5Ambr");
    if (nr_ue_pc5_ambr) {
    if (!cJSON_IsString(nr_ue_pc5_ambr) && !cJSON_IsNull(nr_ue_pc5_ambr)) {
        ogs_error("OpenAPI_a2x_subscription_data_1_parseFromJSON() failed [nr_ue_pc5_ambr]");
        goto end;
    }
    }

    lte_pc5_ambr = cJSON_GetObjectItemCaseSensitive(a2x_subscription_data_1JSON, "ltePc5Ambr");
    if (lte_pc5_ambr) {
    if (!cJSON_IsString(lte_pc5_ambr) && !cJSON_IsNull(lte_pc5_ambr)) {
        ogs_error("OpenAPI_a2x_subscription_data_1_parseFromJSON() failed [lte_pc5_ambr]");
        goto end;
    }
    }

    a2x_subscription_data_1_local_var = OpenAPI_a2x_subscription_data_1_create (
        nr_a2x_services_auth ? nr_a2x_services_auth_local_nonprim : NULL,
        lte_a2x_services_auth ? lte_a2x_services_auth_local_nonprim : NULL,
        nr_ue_pc5_ambr && !cJSON_IsNull(nr_ue_pc5_ambr) ? ogs_strdup(nr_ue_pc5_ambr->valuestring) : NULL,
        lte_pc5_ambr && !cJSON_IsNull(lte_pc5_ambr) ? ogs_strdup(lte_pc5_ambr->valuestring) : NULL
    );

    return a2x_subscription_data_1_local_var;
end:
    if (nr_a2x_services_auth_local_nonprim) {
        OpenAPI_nr_a2x_auth_1_free(nr_a2x_services_auth_local_nonprim);
        nr_a2x_services_auth_local_nonprim = NULL;
    }
    if (lte_a2x_services_auth_local_nonprim) {
        OpenAPI_lte_a2x_auth_1_free(lte_a2x_services_auth_local_nonprim);
        lte_a2x_services_auth_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_a2x_subscription_data_1_t *OpenAPI_a2x_subscription_data_1_copy(OpenAPI_a2x_subscription_data_1_t *dst, OpenAPI_a2x_subscription_data_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_a2x_subscription_data_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_a2x_subscription_data_1_convertToJSON() failed");
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

    OpenAPI_a2x_subscription_data_1_free(dst);
    dst = OpenAPI_a2x_subscription_data_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

