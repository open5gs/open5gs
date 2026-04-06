
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ranging_sl_privacy.h"

OpenAPI_ranging_sl_privacy_t *OpenAPI_ranging_sl_privacy_create(
    char *af_instance_id,
    bool is_reference_id,
    int reference_id,
    OpenAPI_rslppi_t *rslppi
)
{
    OpenAPI_ranging_sl_privacy_t *ranging_sl_privacy_local_var = ogs_malloc(sizeof(OpenAPI_ranging_sl_privacy_t));
    ogs_assert(ranging_sl_privacy_local_var);

    ranging_sl_privacy_local_var->af_instance_id = af_instance_id;
    ranging_sl_privacy_local_var->is_reference_id = is_reference_id;
    ranging_sl_privacy_local_var->reference_id = reference_id;
    ranging_sl_privacy_local_var->rslppi = rslppi;

    return ranging_sl_privacy_local_var;
}

void OpenAPI_ranging_sl_privacy_free(OpenAPI_ranging_sl_privacy_t *ranging_sl_privacy)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ranging_sl_privacy) {
        return;
    }
    if (ranging_sl_privacy->af_instance_id) {
        ogs_free(ranging_sl_privacy->af_instance_id);
        ranging_sl_privacy->af_instance_id = NULL;
    }
    if (ranging_sl_privacy->rslppi) {
        OpenAPI_rslppi_free(ranging_sl_privacy->rslppi);
        ranging_sl_privacy->rslppi = NULL;
    }
    ogs_free(ranging_sl_privacy);
}

cJSON *OpenAPI_ranging_sl_privacy_convertToJSON(OpenAPI_ranging_sl_privacy_t *ranging_sl_privacy)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ranging_sl_privacy == NULL) {
        ogs_error("OpenAPI_ranging_sl_privacy_convertToJSON() failed [RangingSlPrivacy]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ranging_sl_privacy->af_instance_id) {
    if (cJSON_AddStringToObject(item, "afInstanceId", ranging_sl_privacy->af_instance_id) == NULL) {
        ogs_error("OpenAPI_ranging_sl_privacy_convertToJSON() failed [af_instance_id]");
        goto end;
    }
    }

    if (ranging_sl_privacy->is_reference_id) {
    if (cJSON_AddNumberToObject(item, "referenceId", ranging_sl_privacy->reference_id) == NULL) {
        ogs_error("OpenAPI_ranging_sl_privacy_convertToJSON() failed [reference_id]");
        goto end;
    }
    }

    if (ranging_sl_privacy->rslppi) {
    cJSON *rslppi_local_JSON = OpenAPI_rslppi_convertToJSON(ranging_sl_privacy->rslppi);
    if (rslppi_local_JSON == NULL) {
        ogs_error("OpenAPI_ranging_sl_privacy_convertToJSON() failed [rslppi]");
        goto end;
    }
    cJSON_AddItemToObject(item, "rslppi", rslppi_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ranging_sl_privacy_convertToJSON() failed [rslppi]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ranging_sl_privacy_t *OpenAPI_ranging_sl_privacy_parseFromJSON(cJSON *ranging_sl_privacyJSON)
{
    OpenAPI_ranging_sl_privacy_t *ranging_sl_privacy_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *af_instance_id = NULL;
    cJSON *reference_id = NULL;
    cJSON *rslppi = NULL;
    OpenAPI_rslppi_t *rslppi_local_nonprim = NULL;
    af_instance_id = cJSON_GetObjectItemCaseSensitive(ranging_sl_privacyJSON, "afInstanceId");
    if (af_instance_id) {
    if (!cJSON_IsString(af_instance_id) && !cJSON_IsNull(af_instance_id)) {
        ogs_error("OpenAPI_ranging_sl_privacy_parseFromJSON() failed [af_instance_id]");
        goto end;
    }
    }

    reference_id = cJSON_GetObjectItemCaseSensitive(ranging_sl_privacyJSON, "referenceId");
    if (reference_id) {
    if (!cJSON_IsNumber(reference_id)) {
        ogs_error("OpenAPI_ranging_sl_privacy_parseFromJSON() failed [reference_id]");
        goto end;
    }
    }

    rslppi = cJSON_GetObjectItemCaseSensitive(ranging_sl_privacyJSON, "rslppi");
    if (rslppi) {
    rslppi_local_nonprim = OpenAPI_rslppi_parseFromJSON(rslppi);
    if (!rslppi_local_nonprim) {
        ogs_error("OpenAPI_rslppi_parseFromJSON failed [rslppi]");
        goto end;
    }
    }

    ranging_sl_privacy_local_var = OpenAPI_ranging_sl_privacy_create (
        af_instance_id && !cJSON_IsNull(af_instance_id) ? ogs_strdup(af_instance_id->valuestring) : NULL,
        reference_id ? true : false,
        reference_id ? reference_id->valuedouble : 0,
        rslppi ? rslppi_local_nonprim : NULL
    );

    return ranging_sl_privacy_local_var;
end:
    if (rslppi_local_nonprim) {
        OpenAPI_rslppi_free(rslppi_local_nonprim);
        rslppi_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_ranging_sl_privacy_t *OpenAPI_ranging_sl_privacy_copy(OpenAPI_ranging_sl_privacy_t *dst, OpenAPI_ranging_sl_privacy_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ranging_sl_privacy_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ranging_sl_privacy_convertToJSON() failed");
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

    OpenAPI_ranging_sl_privacy_free(dst);
    dst = OpenAPI_ranging_sl_privacy_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

