
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "serving_nf_identity.h"

OpenAPI_serving_nf_identity_t *OpenAPI_serving_nf_identity_create(
    char *serv_nf_inst_id,
    OpenAPI_guami_t *guami,
    OpenAPI_an_gw_address_t *an_gw_addr,
    OpenAPI_sgsn_address_t *sgsn_addr
)
{
    OpenAPI_serving_nf_identity_t *serving_nf_identity_local_var = ogs_malloc(sizeof(OpenAPI_serving_nf_identity_t));
    ogs_assert(serving_nf_identity_local_var);

    serving_nf_identity_local_var->serv_nf_inst_id = serv_nf_inst_id;
    serving_nf_identity_local_var->guami = guami;
    serving_nf_identity_local_var->an_gw_addr = an_gw_addr;
    serving_nf_identity_local_var->sgsn_addr = sgsn_addr;

    return serving_nf_identity_local_var;
}

void OpenAPI_serving_nf_identity_free(OpenAPI_serving_nf_identity_t *serving_nf_identity)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == serving_nf_identity) {
        return;
    }
    if (serving_nf_identity->serv_nf_inst_id) {
        ogs_free(serving_nf_identity->serv_nf_inst_id);
        serving_nf_identity->serv_nf_inst_id = NULL;
    }
    if (serving_nf_identity->guami) {
        OpenAPI_guami_free(serving_nf_identity->guami);
        serving_nf_identity->guami = NULL;
    }
    if (serving_nf_identity->an_gw_addr) {
        OpenAPI_an_gw_address_free(serving_nf_identity->an_gw_addr);
        serving_nf_identity->an_gw_addr = NULL;
    }
    if (serving_nf_identity->sgsn_addr) {
        OpenAPI_sgsn_address_free(serving_nf_identity->sgsn_addr);
        serving_nf_identity->sgsn_addr = NULL;
    }
    ogs_free(serving_nf_identity);
}

cJSON *OpenAPI_serving_nf_identity_convertToJSON(OpenAPI_serving_nf_identity_t *serving_nf_identity)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (serving_nf_identity == NULL) {
        ogs_error("OpenAPI_serving_nf_identity_convertToJSON() failed [ServingNfIdentity]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (serving_nf_identity->serv_nf_inst_id) {
    if (cJSON_AddStringToObject(item, "servNfInstId", serving_nf_identity->serv_nf_inst_id) == NULL) {
        ogs_error("OpenAPI_serving_nf_identity_convertToJSON() failed [serv_nf_inst_id]");
        goto end;
    }
    }

    if (serving_nf_identity->guami) {
    cJSON *guami_local_JSON = OpenAPI_guami_convertToJSON(serving_nf_identity->guami);
    if (guami_local_JSON == NULL) {
        ogs_error("OpenAPI_serving_nf_identity_convertToJSON() failed [guami]");
        goto end;
    }
    cJSON_AddItemToObject(item, "guami", guami_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_serving_nf_identity_convertToJSON() failed [guami]");
        goto end;
    }
    }

    if (serving_nf_identity->an_gw_addr) {
    cJSON *an_gw_addr_local_JSON = OpenAPI_an_gw_address_convertToJSON(serving_nf_identity->an_gw_addr);
    if (an_gw_addr_local_JSON == NULL) {
        ogs_error("OpenAPI_serving_nf_identity_convertToJSON() failed [an_gw_addr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "anGwAddr", an_gw_addr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_serving_nf_identity_convertToJSON() failed [an_gw_addr]");
        goto end;
    }
    }

    if (serving_nf_identity->sgsn_addr) {
    cJSON *sgsn_addr_local_JSON = OpenAPI_sgsn_address_convertToJSON(serving_nf_identity->sgsn_addr);
    if (sgsn_addr_local_JSON == NULL) {
        ogs_error("OpenAPI_serving_nf_identity_convertToJSON() failed [sgsn_addr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sgsnAddr", sgsn_addr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_serving_nf_identity_convertToJSON() failed [sgsn_addr]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_serving_nf_identity_t *OpenAPI_serving_nf_identity_parseFromJSON(cJSON *serving_nf_identityJSON)
{
    OpenAPI_serving_nf_identity_t *serving_nf_identity_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *serv_nf_inst_id = NULL;
    cJSON *guami = NULL;
    OpenAPI_guami_t *guami_local_nonprim = NULL;
    cJSON *an_gw_addr = NULL;
    OpenAPI_an_gw_address_t *an_gw_addr_local_nonprim = NULL;
    cJSON *sgsn_addr = NULL;
    OpenAPI_sgsn_address_t *sgsn_addr_local_nonprim = NULL;
    serv_nf_inst_id = cJSON_GetObjectItemCaseSensitive(serving_nf_identityJSON, "servNfInstId");
    if (serv_nf_inst_id) {
    if (!cJSON_IsString(serv_nf_inst_id) && !cJSON_IsNull(serv_nf_inst_id)) {
        ogs_error("OpenAPI_serving_nf_identity_parseFromJSON() failed [serv_nf_inst_id]");
        goto end;
    }
    }

    guami = cJSON_GetObjectItemCaseSensitive(serving_nf_identityJSON, "guami");
    if (guami) {
    guami_local_nonprim = OpenAPI_guami_parseFromJSON(guami);
    if (!guami_local_nonprim) {
        ogs_error("OpenAPI_guami_parseFromJSON failed [guami]");
        goto end;
    }
    }

    an_gw_addr = cJSON_GetObjectItemCaseSensitive(serving_nf_identityJSON, "anGwAddr");
    if (an_gw_addr) {
    an_gw_addr_local_nonprim = OpenAPI_an_gw_address_parseFromJSON(an_gw_addr);
    if (!an_gw_addr_local_nonprim) {
        ogs_error("OpenAPI_an_gw_address_parseFromJSON failed [an_gw_addr]");
        goto end;
    }
    }

    sgsn_addr = cJSON_GetObjectItemCaseSensitive(serving_nf_identityJSON, "sgsnAddr");
    if (sgsn_addr) {
    sgsn_addr_local_nonprim = OpenAPI_sgsn_address_parseFromJSON(sgsn_addr);
    if (!sgsn_addr_local_nonprim) {
        ogs_error("OpenAPI_sgsn_address_parseFromJSON failed [sgsn_addr]");
        goto end;
    }
    }

    serving_nf_identity_local_var = OpenAPI_serving_nf_identity_create (
        serv_nf_inst_id && !cJSON_IsNull(serv_nf_inst_id) ? ogs_strdup(serv_nf_inst_id->valuestring) : NULL,
        guami ? guami_local_nonprim : NULL,
        an_gw_addr ? an_gw_addr_local_nonprim : NULL,
        sgsn_addr ? sgsn_addr_local_nonprim : NULL
    );

    return serving_nf_identity_local_var;
end:
    if (guami_local_nonprim) {
        OpenAPI_guami_free(guami_local_nonprim);
        guami_local_nonprim = NULL;
    }
    if (an_gw_addr_local_nonprim) {
        OpenAPI_an_gw_address_free(an_gw_addr_local_nonprim);
        an_gw_addr_local_nonprim = NULL;
    }
    if (sgsn_addr_local_nonprim) {
        OpenAPI_sgsn_address_free(sgsn_addr_local_nonprim);
        sgsn_addr_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_serving_nf_identity_t *OpenAPI_serving_nf_identity_copy(OpenAPI_serving_nf_identity_t *dst, OpenAPI_serving_nf_identity_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_serving_nf_identity_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_serving_nf_identity_convertToJSON() failed");
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

    OpenAPI_serving_nf_identity_free(dst);
    dst = OpenAPI_serving_nf_identity_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

