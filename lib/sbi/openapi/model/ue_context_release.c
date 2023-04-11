
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_context_release.h"

OpenAPI_ue_context_release_t *OpenAPI_ue_context_release_create(
    char *supi,
    bool is_unauthenticated_supi,
    int unauthenticated_supi,
    OpenAPI_ng_ap_cause_t *ngap_cause
)
{
    OpenAPI_ue_context_release_t *ue_context_release_local_var = ogs_malloc(sizeof(OpenAPI_ue_context_release_t));
    ogs_assert(ue_context_release_local_var);

    ue_context_release_local_var->supi = supi;
    ue_context_release_local_var->is_unauthenticated_supi = is_unauthenticated_supi;
    ue_context_release_local_var->unauthenticated_supi = unauthenticated_supi;
    ue_context_release_local_var->ngap_cause = ngap_cause;

    return ue_context_release_local_var;
}

void OpenAPI_ue_context_release_free(OpenAPI_ue_context_release_t *ue_context_release)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_context_release) {
        return;
    }
    if (ue_context_release->supi) {
        ogs_free(ue_context_release->supi);
        ue_context_release->supi = NULL;
    }
    if (ue_context_release->ngap_cause) {
        OpenAPI_ng_ap_cause_free(ue_context_release->ngap_cause);
        ue_context_release->ngap_cause = NULL;
    }
    ogs_free(ue_context_release);
}

cJSON *OpenAPI_ue_context_release_convertToJSON(OpenAPI_ue_context_release_t *ue_context_release)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ue_context_release == NULL) {
        ogs_error("OpenAPI_ue_context_release_convertToJSON() failed [UEContextRelease]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ue_context_release->supi) {
    if (cJSON_AddStringToObject(item, "supi", ue_context_release->supi) == NULL) {
        ogs_error("OpenAPI_ue_context_release_convertToJSON() failed [supi]");
        goto end;
    }
    }

    if (ue_context_release->is_unauthenticated_supi) {
    if (cJSON_AddBoolToObject(item, "unauthenticatedSupi", ue_context_release->unauthenticated_supi) == NULL) {
        ogs_error("OpenAPI_ue_context_release_convertToJSON() failed [unauthenticated_supi]");
        goto end;
    }
    }

    if (!ue_context_release->ngap_cause) {
        ogs_error("OpenAPI_ue_context_release_convertToJSON() failed [ngap_cause]");
        return NULL;
    }
    cJSON *ngap_cause_local_JSON = OpenAPI_ng_ap_cause_convertToJSON(ue_context_release->ngap_cause);
    if (ngap_cause_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_release_convertToJSON() failed [ngap_cause]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ngapCause", ngap_cause_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_release_convertToJSON() failed [ngap_cause]");
        goto end;
    }

end:
    return item;
}

OpenAPI_ue_context_release_t *OpenAPI_ue_context_release_parseFromJSON(cJSON *ue_context_releaseJSON)
{
    OpenAPI_ue_context_release_t *ue_context_release_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *supi = NULL;
    cJSON *unauthenticated_supi = NULL;
    cJSON *ngap_cause = NULL;
    OpenAPI_ng_ap_cause_t *ngap_cause_local_nonprim = NULL;
    supi = cJSON_GetObjectItemCaseSensitive(ue_context_releaseJSON, "supi");
    if (supi) {
    if (!cJSON_IsString(supi) && !cJSON_IsNull(supi)) {
        ogs_error("OpenAPI_ue_context_release_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    unauthenticated_supi = cJSON_GetObjectItemCaseSensitive(ue_context_releaseJSON, "unauthenticatedSupi");
    if (unauthenticated_supi) {
    if (!cJSON_IsBool(unauthenticated_supi)) {
        ogs_error("OpenAPI_ue_context_release_parseFromJSON() failed [unauthenticated_supi]");
        goto end;
    }
    }

    ngap_cause = cJSON_GetObjectItemCaseSensitive(ue_context_releaseJSON, "ngapCause");
    if (!ngap_cause) {
        ogs_error("OpenAPI_ue_context_release_parseFromJSON() failed [ngap_cause]");
        goto end;
    }
    ngap_cause_local_nonprim = OpenAPI_ng_ap_cause_parseFromJSON(ngap_cause);
    if (!ngap_cause_local_nonprim) {
        ogs_error("OpenAPI_ng_ap_cause_parseFromJSON failed [ngap_cause]");
        goto end;
    }

    ue_context_release_local_var = OpenAPI_ue_context_release_create (
        supi && !cJSON_IsNull(supi) ? ogs_strdup(supi->valuestring) : NULL,
        unauthenticated_supi ? true : false,
        unauthenticated_supi ? unauthenticated_supi->valueint : 0,
        ngap_cause_local_nonprim
    );

    return ue_context_release_local_var;
end:
    if (ngap_cause_local_nonprim) {
        OpenAPI_ng_ap_cause_free(ngap_cause_local_nonprim);
        ngap_cause_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_ue_context_release_t *OpenAPI_ue_context_release_copy(OpenAPI_ue_context_release_t *dst, OpenAPI_ue_context_release_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_context_release_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_context_release_convertToJSON() failed");
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

    OpenAPI_ue_context_release_free(dst);
    dst = OpenAPI_ue_context_release_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

