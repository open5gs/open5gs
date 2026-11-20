
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sdp_time_stamps.h"

OpenAPI_sdp_time_stamps_t *OpenAPI_sdp_time_stamps_create(
    char *s_dp_offer_timestamp,
    char *s_dp_answer_timestamp
)
{
    OpenAPI_sdp_time_stamps_t *sdp_time_stamps_local_var = ogs_malloc(sizeof(OpenAPI_sdp_time_stamps_t));
    ogs_assert(sdp_time_stamps_local_var);

    sdp_time_stamps_local_var->s_dp_offer_timestamp = s_dp_offer_timestamp;
    sdp_time_stamps_local_var->s_dp_answer_timestamp = s_dp_answer_timestamp;

    return sdp_time_stamps_local_var;
}

void OpenAPI_sdp_time_stamps_free(OpenAPI_sdp_time_stamps_t *sdp_time_stamps)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sdp_time_stamps) {
        return;
    }
    if (sdp_time_stamps->s_dp_offer_timestamp) {
        ogs_free(sdp_time_stamps->s_dp_offer_timestamp);
        sdp_time_stamps->s_dp_offer_timestamp = NULL;
    }
    if (sdp_time_stamps->s_dp_answer_timestamp) {
        ogs_free(sdp_time_stamps->s_dp_answer_timestamp);
        sdp_time_stamps->s_dp_answer_timestamp = NULL;
    }
    ogs_free(sdp_time_stamps);
}

cJSON *OpenAPI_sdp_time_stamps_convertToJSON(OpenAPI_sdp_time_stamps_t *sdp_time_stamps)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sdp_time_stamps == NULL) {
        ogs_error("OpenAPI_sdp_time_stamps_convertToJSON() failed [SDPTimeStamps]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (sdp_time_stamps->s_dp_offer_timestamp) {
    if (cJSON_AddStringToObject(item, "sDPOfferTimestamp", sdp_time_stamps->s_dp_offer_timestamp) == NULL) {
        ogs_error("OpenAPI_sdp_time_stamps_convertToJSON() failed [s_dp_offer_timestamp]");
        goto end;
    }
    }

    if (sdp_time_stamps->s_dp_answer_timestamp) {
    if (cJSON_AddStringToObject(item, "sDPAnswerTimestamp", sdp_time_stamps->s_dp_answer_timestamp) == NULL) {
        ogs_error("OpenAPI_sdp_time_stamps_convertToJSON() failed [s_dp_answer_timestamp]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sdp_time_stamps_t *OpenAPI_sdp_time_stamps_parseFromJSON(cJSON *sdp_time_stampsJSON)
{
    OpenAPI_sdp_time_stamps_t *sdp_time_stamps_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *s_dp_offer_timestamp = NULL;
    cJSON *s_dp_answer_timestamp = NULL;
    s_dp_offer_timestamp = cJSON_GetObjectItemCaseSensitive(sdp_time_stampsJSON, "sDPOfferTimestamp");
    if (s_dp_offer_timestamp) {
    if (!cJSON_IsString(s_dp_offer_timestamp) && !cJSON_IsNull(s_dp_offer_timestamp)) {
        ogs_error("OpenAPI_sdp_time_stamps_parseFromJSON() failed [s_dp_offer_timestamp]");
        goto end;
    }
    }

    s_dp_answer_timestamp = cJSON_GetObjectItemCaseSensitive(sdp_time_stampsJSON, "sDPAnswerTimestamp");
    if (s_dp_answer_timestamp) {
    if (!cJSON_IsString(s_dp_answer_timestamp) && !cJSON_IsNull(s_dp_answer_timestamp)) {
        ogs_error("OpenAPI_sdp_time_stamps_parseFromJSON() failed [s_dp_answer_timestamp]");
        goto end;
    }
    }

    sdp_time_stamps_local_var = OpenAPI_sdp_time_stamps_create (
        s_dp_offer_timestamp && !cJSON_IsNull(s_dp_offer_timestamp) ? ogs_strdup(s_dp_offer_timestamp->valuestring) : NULL,
        s_dp_answer_timestamp && !cJSON_IsNull(s_dp_answer_timestamp) ? ogs_strdup(s_dp_answer_timestamp->valuestring) : NULL
    );

    return sdp_time_stamps_local_var;
end:
    return NULL;
}

OpenAPI_sdp_time_stamps_t *OpenAPI_sdp_time_stamps_copy(OpenAPI_sdp_time_stamps_t *dst, OpenAPI_sdp_time_stamps_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sdp_time_stamps_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sdp_time_stamps_convertToJSON() failed");
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

    OpenAPI_sdp_time_stamps_free(dst);
    dst = OpenAPI_sdp_time_stamps_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

