
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "guami_1.h"

OpenAPI_guami_1_t *OpenAPI_guami_1_create(
    OpenAPI_plmn_id_nid_1_t *plmn_id,
    char *amf_id
)
{
    OpenAPI_guami_1_t *guami_1_local_var = ogs_malloc(sizeof(OpenAPI_guami_1_t));
    ogs_assert(guami_1_local_var);

    guami_1_local_var->plmn_id = plmn_id;
    guami_1_local_var->amf_id = amf_id;

    return guami_1_local_var;
}

void OpenAPI_guami_1_free(OpenAPI_guami_1_t *guami_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == guami_1) {
        return;
    }
    if (guami_1->plmn_id) {
        OpenAPI_plmn_id_nid_1_free(guami_1->plmn_id);
        guami_1->plmn_id = NULL;
    }
    if (guami_1->amf_id) {
        ogs_free(guami_1->amf_id);
        guami_1->amf_id = NULL;
    }
    ogs_free(guami_1);
}

cJSON *OpenAPI_guami_1_convertToJSON(OpenAPI_guami_1_t *guami_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (guami_1 == NULL) {
        ogs_error("OpenAPI_guami_1_convertToJSON() failed [Guami_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!guami_1->plmn_id) {
        ogs_error("OpenAPI_guami_1_convertToJSON() failed [plmn_id]");
        return NULL;
    }
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_nid_1_convertToJSON(guami_1->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_guami_1_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_guami_1_convertToJSON() failed [plmn_id]");
        goto end;
    }

    if (!guami_1->amf_id) {
        ogs_error("OpenAPI_guami_1_convertToJSON() failed [amf_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "amfId", guami_1->amf_id) == NULL) {
        ogs_error("OpenAPI_guami_1_convertToJSON() failed [amf_id]");
        goto end;
    }

end:
    return item;
}

OpenAPI_guami_1_t *OpenAPI_guami_1_parseFromJSON(cJSON *guami_1JSON)
{
    OpenAPI_guami_1_t *guami_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_nid_1_t *plmn_id_local_nonprim = NULL;
    cJSON *amf_id = NULL;
    plmn_id = cJSON_GetObjectItemCaseSensitive(guami_1JSON, "plmnId");
    if (!plmn_id) {
        ogs_error("OpenAPI_guami_1_parseFromJSON() failed [plmn_id]");
        goto end;
    }
    plmn_id_local_nonprim = OpenAPI_plmn_id_nid_1_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_nid_1_parseFromJSON failed [plmn_id]");
        goto end;
    }

    amf_id = cJSON_GetObjectItemCaseSensitive(guami_1JSON, "amfId");
    if (!amf_id) {
        ogs_error("OpenAPI_guami_1_parseFromJSON() failed [amf_id]");
        goto end;
    }
    if (!cJSON_IsString(amf_id)) {
        ogs_error("OpenAPI_guami_1_parseFromJSON() failed [amf_id]");
        goto end;
    }

    guami_1_local_var = OpenAPI_guami_1_create (
        plmn_id_local_nonprim,
        ogs_strdup(amf_id->valuestring)
    );

    return guami_1_local_var;
end:
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_nid_1_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_guami_1_t *OpenAPI_guami_1_copy(OpenAPI_guami_1_t *dst, OpenAPI_guami_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_guami_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_guami_1_convertToJSON() failed");
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

    OpenAPI_guami_1_free(dst);
    dst = OpenAPI_guami_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

