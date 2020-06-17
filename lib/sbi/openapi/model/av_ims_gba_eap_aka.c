
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "av_ims_gba_eap_aka.h"

OpenAPI_av_ims_gba_eap_aka_t *OpenAPI_av_ims_gba_eap_aka_create(
    OpenAPI_hss_av_type_t *av_type,
    char *rand,
    char *xres,
    char *autn,
    char *ck,
    char *ik
    )
{
    OpenAPI_av_ims_gba_eap_aka_t *av_ims_gba_eap_aka_local_var = OpenAPI_malloc(sizeof(OpenAPI_av_ims_gba_eap_aka_t));
    if (!av_ims_gba_eap_aka_local_var) {
        return NULL;
    }
    av_ims_gba_eap_aka_local_var->av_type = av_type;
    av_ims_gba_eap_aka_local_var->rand = rand;
    av_ims_gba_eap_aka_local_var->xres = xres;
    av_ims_gba_eap_aka_local_var->autn = autn;
    av_ims_gba_eap_aka_local_var->ck = ck;
    av_ims_gba_eap_aka_local_var->ik = ik;

    return av_ims_gba_eap_aka_local_var;
}

void OpenAPI_av_ims_gba_eap_aka_free(OpenAPI_av_ims_gba_eap_aka_t *av_ims_gba_eap_aka)
{
    if (NULL == av_ims_gba_eap_aka) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_hss_av_type_free(av_ims_gba_eap_aka->av_type);
    ogs_free(av_ims_gba_eap_aka->rand);
    ogs_free(av_ims_gba_eap_aka->xres);
    ogs_free(av_ims_gba_eap_aka->autn);
    ogs_free(av_ims_gba_eap_aka->ck);
    ogs_free(av_ims_gba_eap_aka->ik);
    ogs_free(av_ims_gba_eap_aka);
}

cJSON *OpenAPI_av_ims_gba_eap_aka_convertToJSON(OpenAPI_av_ims_gba_eap_aka_t *av_ims_gba_eap_aka)
{
    cJSON *item = NULL;

    if (av_ims_gba_eap_aka == NULL) {
        ogs_error("OpenAPI_av_ims_gba_eap_aka_convertToJSON() failed [AvImsGbaEapAka]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!av_ims_gba_eap_aka->av_type) {
        ogs_error("OpenAPI_av_ims_gba_eap_aka_convertToJSON() failed [av_type]");
        goto end;
    }
    cJSON *av_type_local_JSON = OpenAPI_hss_av_type_convertToJSON(av_ims_gba_eap_aka->av_type);
    if (av_type_local_JSON == NULL) {
        ogs_error("OpenAPI_av_ims_gba_eap_aka_convertToJSON() failed [av_type]");
        goto end;
    }
    cJSON_AddItemToObject(item, "avType", av_type_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_av_ims_gba_eap_aka_convertToJSON() failed [av_type]");
        goto end;
    }

    if (!av_ims_gba_eap_aka->rand) {
        ogs_error("OpenAPI_av_ims_gba_eap_aka_convertToJSON() failed [rand]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "rand", av_ims_gba_eap_aka->rand) == NULL) {
        ogs_error("OpenAPI_av_ims_gba_eap_aka_convertToJSON() failed [rand]");
        goto end;
    }

    if (!av_ims_gba_eap_aka->xres) {
        ogs_error("OpenAPI_av_ims_gba_eap_aka_convertToJSON() failed [xres]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "xres", av_ims_gba_eap_aka->xres) == NULL) {
        ogs_error("OpenAPI_av_ims_gba_eap_aka_convertToJSON() failed [xres]");
        goto end;
    }

    if (!av_ims_gba_eap_aka->autn) {
        ogs_error("OpenAPI_av_ims_gba_eap_aka_convertToJSON() failed [autn]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "autn", av_ims_gba_eap_aka->autn) == NULL) {
        ogs_error("OpenAPI_av_ims_gba_eap_aka_convertToJSON() failed [autn]");
        goto end;
    }

    if (!av_ims_gba_eap_aka->ck) {
        ogs_error("OpenAPI_av_ims_gba_eap_aka_convertToJSON() failed [ck]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "ck", av_ims_gba_eap_aka->ck) == NULL) {
        ogs_error("OpenAPI_av_ims_gba_eap_aka_convertToJSON() failed [ck]");
        goto end;
    }

    if (!av_ims_gba_eap_aka->ik) {
        ogs_error("OpenAPI_av_ims_gba_eap_aka_convertToJSON() failed [ik]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "ik", av_ims_gba_eap_aka->ik) == NULL) {
        ogs_error("OpenAPI_av_ims_gba_eap_aka_convertToJSON() failed [ik]");
        goto end;
    }

end:
    return item;
}

OpenAPI_av_ims_gba_eap_aka_t *OpenAPI_av_ims_gba_eap_aka_parseFromJSON(cJSON *av_ims_gba_eap_akaJSON)
{
    OpenAPI_av_ims_gba_eap_aka_t *av_ims_gba_eap_aka_local_var = NULL;
    cJSON *av_type = cJSON_GetObjectItemCaseSensitive(av_ims_gba_eap_akaJSON, "avType");
    if (!av_type) {
        ogs_error("OpenAPI_av_ims_gba_eap_aka_parseFromJSON() failed [av_type]");
        goto end;
    }

    OpenAPI_hss_av_type_t *av_type_local_nonprim = NULL;

    av_type_local_nonprim = OpenAPI_hss_av_type_parseFromJSON(av_type);

    cJSON *rand = cJSON_GetObjectItemCaseSensitive(av_ims_gba_eap_akaJSON, "rand");
    if (!rand) {
        ogs_error("OpenAPI_av_ims_gba_eap_aka_parseFromJSON() failed [rand]");
        goto end;
    }


    if (!cJSON_IsString(rand)) {
        ogs_error("OpenAPI_av_ims_gba_eap_aka_parseFromJSON() failed [rand]");
        goto end;
    }

    cJSON *xres = cJSON_GetObjectItemCaseSensitive(av_ims_gba_eap_akaJSON, "xres");
    if (!xres) {
        ogs_error("OpenAPI_av_ims_gba_eap_aka_parseFromJSON() failed [xres]");
        goto end;
    }


    if (!cJSON_IsString(xres)) {
        ogs_error("OpenAPI_av_ims_gba_eap_aka_parseFromJSON() failed [xres]");
        goto end;
    }

    cJSON *autn = cJSON_GetObjectItemCaseSensitive(av_ims_gba_eap_akaJSON, "autn");
    if (!autn) {
        ogs_error("OpenAPI_av_ims_gba_eap_aka_parseFromJSON() failed [autn]");
        goto end;
    }


    if (!cJSON_IsString(autn)) {
        ogs_error("OpenAPI_av_ims_gba_eap_aka_parseFromJSON() failed [autn]");
        goto end;
    }

    cJSON *ck = cJSON_GetObjectItemCaseSensitive(av_ims_gba_eap_akaJSON, "ck");
    if (!ck) {
        ogs_error("OpenAPI_av_ims_gba_eap_aka_parseFromJSON() failed [ck]");
        goto end;
    }


    if (!cJSON_IsString(ck)) {
        ogs_error("OpenAPI_av_ims_gba_eap_aka_parseFromJSON() failed [ck]");
        goto end;
    }

    cJSON *ik = cJSON_GetObjectItemCaseSensitive(av_ims_gba_eap_akaJSON, "ik");
    if (!ik) {
        ogs_error("OpenAPI_av_ims_gba_eap_aka_parseFromJSON() failed [ik]");
        goto end;
    }


    if (!cJSON_IsString(ik)) {
        ogs_error("OpenAPI_av_ims_gba_eap_aka_parseFromJSON() failed [ik]");
        goto end;
    }

    av_ims_gba_eap_aka_local_var = OpenAPI_av_ims_gba_eap_aka_create (
        av_type_local_nonprim,
        ogs_strdup(rand->valuestring),
        ogs_strdup(xres->valuestring),
        ogs_strdup(autn->valuestring),
        ogs_strdup(ck->valuestring),
        ogs_strdup(ik->valuestring)
        );

    return av_ims_gba_eap_aka_local_var;
end:
    return NULL;
}

OpenAPI_av_ims_gba_eap_aka_t *OpenAPI_av_ims_gba_eap_aka_copy(OpenAPI_av_ims_gba_eap_aka_t *dst, OpenAPI_av_ims_gba_eap_aka_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_av_ims_gba_eap_aka_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_av_ims_gba_eap_aka_convertToJSON() failed");
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

    OpenAPI_av_ims_gba_eap_aka_free(dst);
    dst = OpenAPI_av_ims_gba_eap_aka_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

