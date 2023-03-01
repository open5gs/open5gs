
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "authentication_vector.h"

OpenAPI_authentication_vector_t *OpenAPI_authentication_vector_create(
    OpenAPI_av_type_e av_type,
    char *rand,
    char *xres,
    char *autn,
    char *ck_prime,
    char *ik_prime,
    char *xres_star,
    char *kausf
)
{
    OpenAPI_authentication_vector_t *authentication_vector_local_var = ogs_malloc(sizeof(OpenAPI_authentication_vector_t));
    ogs_assert(authentication_vector_local_var);

    authentication_vector_local_var->av_type = av_type;
    authentication_vector_local_var->rand = rand;
    authentication_vector_local_var->xres = xres;
    authentication_vector_local_var->autn = autn;
    authentication_vector_local_var->ck_prime = ck_prime;
    authentication_vector_local_var->ik_prime = ik_prime;
    authentication_vector_local_var->xres_star = xres_star;
    authentication_vector_local_var->kausf = kausf;

    return authentication_vector_local_var;
}

void OpenAPI_authentication_vector_free(OpenAPI_authentication_vector_t *authentication_vector)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == authentication_vector) {
        return;
    }
    if (authentication_vector->rand) {
        ogs_free(authentication_vector->rand);
        authentication_vector->rand = NULL;
    }
    if (authentication_vector->xres) {
        ogs_free(authentication_vector->xres);
        authentication_vector->xres = NULL;
    }
    if (authentication_vector->autn) {
        ogs_free(authentication_vector->autn);
        authentication_vector->autn = NULL;
    }
    if (authentication_vector->ck_prime) {
        ogs_free(authentication_vector->ck_prime);
        authentication_vector->ck_prime = NULL;
    }
    if (authentication_vector->ik_prime) {
        ogs_free(authentication_vector->ik_prime);
        authentication_vector->ik_prime = NULL;
    }
    if (authentication_vector->xres_star) {
        ogs_free(authentication_vector->xres_star);
        authentication_vector->xres_star = NULL;
    }
    if (authentication_vector->kausf) {
        ogs_free(authentication_vector->kausf);
        authentication_vector->kausf = NULL;
    }
    ogs_free(authentication_vector);
}

cJSON *OpenAPI_authentication_vector_convertToJSON(OpenAPI_authentication_vector_t *authentication_vector)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (authentication_vector == NULL) {
        ogs_error("OpenAPI_authentication_vector_convertToJSON() failed [AuthenticationVector]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (authentication_vector->av_type == OpenAPI_av_type_NULL) {
        ogs_error("OpenAPI_authentication_vector_convertToJSON() failed [av_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "avType", OpenAPI_av_type_ToString(authentication_vector->av_type)) == NULL) {
        ogs_error("OpenAPI_authentication_vector_convertToJSON() failed [av_type]");
        goto end;
    }

    if (!authentication_vector->rand) {
        ogs_error("OpenAPI_authentication_vector_convertToJSON() failed [rand]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "rand", authentication_vector->rand) == NULL) {
        ogs_error("OpenAPI_authentication_vector_convertToJSON() failed [rand]");
        goto end;
    }

    if (authentication_vector->xres) {
    if (cJSON_AddStringToObject(item, "xres", authentication_vector->xres) == NULL) {
        ogs_error("OpenAPI_authentication_vector_convertToJSON() failed [xres]");
        goto end;
    }
    }

    if (!authentication_vector->autn) {
        ogs_error("OpenAPI_authentication_vector_convertToJSON() failed [autn]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "autn", authentication_vector->autn) == NULL) {
        ogs_error("OpenAPI_authentication_vector_convertToJSON() failed [autn]");
        goto end;
    }

    if (authentication_vector->ck_prime) {
    if (cJSON_AddStringToObject(item, "ckPrime", authentication_vector->ck_prime) == NULL) {
        ogs_error("OpenAPI_authentication_vector_convertToJSON() failed [ck_prime]");
        goto end;
    }
    }

    if (authentication_vector->ik_prime) {
    if (cJSON_AddStringToObject(item, "ikPrime", authentication_vector->ik_prime) == NULL) {
        ogs_error("OpenAPI_authentication_vector_convertToJSON() failed [ik_prime]");
        goto end;
    }
    }

    if (authentication_vector->xres_star) {
    if (cJSON_AddStringToObject(item, "xresStar", authentication_vector->xres_star) == NULL) {
        ogs_error("OpenAPI_authentication_vector_convertToJSON() failed [xres_star]");
        goto end;
    }
    }

    if (authentication_vector->kausf) {
    if (cJSON_AddStringToObject(item, "kausf", authentication_vector->kausf) == NULL) {
        ogs_error("OpenAPI_authentication_vector_convertToJSON() failed [kausf]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_authentication_vector_t *OpenAPI_authentication_vector_parseFromJSON(cJSON *authentication_vectorJSON)
{
    OpenAPI_authentication_vector_t *authentication_vector_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *av_type = NULL;
    OpenAPI_av_type_e av_typeVariable = 0;
    cJSON *rand = NULL;
    cJSON *xres = NULL;
    cJSON *autn = NULL;
    cJSON *ck_prime = NULL;
    cJSON *ik_prime = NULL;
    cJSON *xres_star = NULL;
    cJSON *kausf = NULL;
    av_type = cJSON_GetObjectItemCaseSensitive(authentication_vectorJSON, "avType");
    if (!av_type) {
        ogs_error("OpenAPI_authentication_vector_parseFromJSON() failed [av_type]");
        goto end;
    }
    if (!cJSON_IsString(av_type)) {
        ogs_error("OpenAPI_authentication_vector_parseFromJSON() failed [av_type]");
        goto end;
    }
    av_typeVariable = OpenAPI_av_type_FromString(av_type->valuestring);

    rand = cJSON_GetObjectItemCaseSensitive(authentication_vectorJSON, "rand");
    if (!rand) {
        ogs_error("OpenAPI_authentication_vector_parseFromJSON() failed [rand]");
        goto end;
    }
    if (!cJSON_IsString(rand)) {
        ogs_error("OpenAPI_authentication_vector_parseFromJSON() failed [rand]");
        goto end;
    }

    xres = cJSON_GetObjectItemCaseSensitive(authentication_vectorJSON, "xres");
    if (xres) {
    if (!cJSON_IsString(xres) && !cJSON_IsNull(xres)) {
        ogs_error("OpenAPI_authentication_vector_parseFromJSON() failed [xres]");
        goto end;
    }
    }

    autn = cJSON_GetObjectItemCaseSensitive(authentication_vectorJSON, "autn");
    if (!autn) {
        ogs_error("OpenAPI_authentication_vector_parseFromJSON() failed [autn]");
        goto end;
    }
    if (!cJSON_IsString(autn)) {
        ogs_error("OpenAPI_authentication_vector_parseFromJSON() failed [autn]");
        goto end;
    }

    ck_prime = cJSON_GetObjectItemCaseSensitive(authentication_vectorJSON, "ckPrime");
    if (ck_prime) {
    if (!cJSON_IsString(ck_prime) && !cJSON_IsNull(ck_prime)) {
        ogs_error("OpenAPI_authentication_vector_parseFromJSON() failed [ck_prime]");
        goto end;
    }
    }

    ik_prime = cJSON_GetObjectItemCaseSensitive(authentication_vectorJSON, "ikPrime");
    if (ik_prime) {
    if (!cJSON_IsString(ik_prime) && !cJSON_IsNull(ik_prime)) {
        ogs_error("OpenAPI_authentication_vector_parseFromJSON() failed [ik_prime]");
        goto end;
    }
    }

    xres_star = cJSON_GetObjectItemCaseSensitive(authentication_vectorJSON, "xresStar");
    if (xres_star) {
    if (!cJSON_IsString(xres_star) && !cJSON_IsNull(xres_star)) {
        ogs_error("OpenAPI_authentication_vector_parseFromJSON() failed [xres_star]");
        goto end;
    }
    }

    kausf = cJSON_GetObjectItemCaseSensitive(authentication_vectorJSON, "kausf");
    if (kausf) {
    if (!cJSON_IsString(kausf) && !cJSON_IsNull(kausf)) {
        ogs_error("OpenAPI_authentication_vector_parseFromJSON() failed [kausf]");
        goto end;
    }
    }

    authentication_vector_local_var = OpenAPI_authentication_vector_create (
        av_typeVariable,
        ogs_strdup(rand->valuestring),
        xres && !cJSON_IsNull(xres) ? ogs_strdup(xres->valuestring) : NULL,
        ogs_strdup(autn->valuestring),
        ck_prime && !cJSON_IsNull(ck_prime) ? ogs_strdup(ck_prime->valuestring) : NULL,
        ik_prime && !cJSON_IsNull(ik_prime) ? ogs_strdup(ik_prime->valuestring) : NULL,
        xres_star && !cJSON_IsNull(xres_star) ? ogs_strdup(xres_star->valuestring) : NULL,
        kausf && !cJSON_IsNull(kausf) ? ogs_strdup(kausf->valuestring) : NULL
    );

    return authentication_vector_local_var;
end:
    return NULL;
}

OpenAPI_authentication_vector_t *OpenAPI_authentication_vector_copy(OpenAPI_authentication_vector_t *dst, OpenAPI_authentication_vector_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_authentication_vector_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_authentication_vector_convertToJSON() failed");
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

    OpenAPI_authentication_vector_free(dst);
    dst = OpenAPI_authentication_vector_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

