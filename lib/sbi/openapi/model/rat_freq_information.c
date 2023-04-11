
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rat_freq_information.h"

OpenAPI_rat_freq_information_t *OpenAPI_rat_freq_information_create(
    bool is_all_freq,
    int all_freq,
    bool is_all_rat,
    int all_rat,
    bool is_freq,
    int freq,
    OpenAPI_rat_type_e rat_type,
    OpenAPI_threshold_level_t *svc_exp_threshold,
    OpenAPI_matching_direction_t *matching_dir
)
{
    OpenAPI_rat_freq_information_t *rat_freq_information_local_var = ogs_malloc(sizeof(OpenAPI_rat_freq_information_t));
    ogs_assert(rat_freq_information_local_var);

    rat_freq_information_local_var->is_all_freq = is_all_freq;
    rat_freq_information_local_var->all_freq = all_freq;
    rat_freq_information_local_var->is_all_rat = is_all_rat;
    rat_freq_information_local_var->all_rat = all_rat;
    rat_freq_information_local_var->is_freq = is_freq;
    rat_freq_information_local_var->freq = freq;
    rat_freq_information_local_var->rat_type = rat_type;
    rat_freq_information_local_var->svc_exp_threshold = svc_exp_threshold;
    rat_freq_information_local_var->matching_dir = matching_dir;

    return rat_freq_information_local_var;
}

void OpenAPI_rat_freq_information_free(OpenAPI_rat_freq_information_t *rat_freq_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == rat_freq_information) {
        return;
    }
    if (rat_freq_information->svc_exp_threshold) {
        OpenAPI_threshold_level_free(rat_freq_information->svc_exp_threshold);
        rat_freq_information->svc_exp_threshold = NULL;
    }
    if (rat_freq_information->matching_dir) {
        OpenAPI_matching_direction_free(rat_freq_information->matching_dir);
        rat_freq_information->matching_dir = NULL;
    }
    ogs_free(rat_freq_information);
}

cJSON *OpenAPI_rat_freq_information_convertToJSON(OpenAPI_rat_freq_information_t *rat_freq_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (rat_freq_information == NULL) {
        ogs_error("OpenAPI_rat_freq_information_convertToJSON() failed [RatFreqInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (rat_freq_information->is_all_freq) {
    if (cJSON_AddBoolToObject(item, "allFreq", rat_freq_information->all_freq) == NULL) {
        ogs_error("OpenAPI_rat_freq_information_convertToJSON() failed [all_freq]");
        goto end;
    }
    }

    if (rat_freq_information->is_all_rat) {
    if (cJSON_AddBoolToObject(item, "allRat", rat_freq_information->all_rat) == NULL) {
        ogs_error("OpenAPI_rat_freq_information_convertToJSON() failed [all_rat]");
        goto end;
    }
    }

    if (rat_freq_information->is_freq) {
    if (cJSON_AddNumberToObject(item, "freq", rat_freq_information->freq) == NULL) {
        ogs_error("OpenAPI_rat_freq_information_convertToJSON() failed [freq]");
        goto end;
    }
    }

    if (rat_freq_information->rat_type != OpenAPI_rat_type_NULL) {
    if (cJSON_AddStringToObject(item, "ratType", OpenAPI_rat_type_ToString(rat_freq_information->rat_type)) == NULL) {
        ogs_error("OpenAPI_rat_freq_information_convertToJSON() failed [rat_type]");
        goto end;
    }
    }

    if (rat_freq_information->svc_exp_threshold) {
    cJSON *svc_exp_threshold_local_JSON = OpenAPI_threshold_level_convertToJSON(rat_freq_information->svc_exp_threshold);
    if (svc_exp_threshold_local_JSON == NULL) {
        ogs_error("OpenAPI_rat_freq_information_convertToJSON() failed [svc_exp_threshold]");
        goto end;
    }
    cJSON_AddItemToObject(item, "svcExpThreshold", svc_exp_threshold_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_rat_freq_information_convertToJSON() failed [svc_exp_threshold]");
        goto end;
    }
    }

    if (rat_freq_information->matching_dir) {
    cJSON *matching_dir_local_JSON = OpenAPI_matching_direction_convertToJSON(rat_freq_information->matching_dir);
    if (matching_dir_local_JSON == NULL) {
        ogs_error("OpenAPI_rat_freq_information_convertToJSON() failed [matching_dir]");
        goto end;
    }
    cJSON_AddItemToObject(item, "matchingDir", matching_dir_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_rat_freq_information_convertToJSON() failed [matching_dir]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_rat_freq_information_t *OpenAPI_rat_freq_information_parseFromJSON(cJSON *rat_freq_informationJSON)
{
    OpenAPI_rat_freq_information_t *rat_freq_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *all_freq = NULL;
    cJSON *all_rat = NULL;
    cJSON *freq = NULL;
    cJSON *rat_type = NULL;
    OpenAPI_rat_type_e rat_typeVariable = 0;
    cJSON *svc_exp_threshold = NULL;
    OpenAPI_threshold_level_t *svc_exp_threshold_local_nonprim = NULL;
    cJSON *matching_dir = NULL;
    OpenAPI_matching_direction_t *matching_dir_local_nonprim = NULL;
    all_freq = cJSON_GetObjectItemCaseSensitive(rat_freq_informationJSON, "allFreq");
    if (all_freq) {
    if (!cJSON_IsBool(all_freq)) {
        ogs_error("OpenAPI_rat_freq_information_parseFromJSON() failed [all_freq]");
        goto end;
    }
    }

    all_rat = cJSON_GetObjectItemCaseSensitive(rat_freq_informationJSON, "allRat");
    if (all_rat) {
    if (!cJSON_IsBool(all_rat)) {
        ogs_error("OpenAPI_rat_freq_information_parseFromJSON() failed [all_rat]");
        goto end;
    }
    }

    freq = cJSON_GetObjectItemCaseSensitive(rat_freq_informationJSON, "freq");
    if (freq) {
    if (!cJSON_IsNumber(freq)) {
        ogs_error("OpenAPI_rat_freq_information_parseFromJSON() failed [freq]");
        goto end;
    }
    }

    rat_type = cJSON_GetObjectItemCaseSensitive(rat_freq_informationJSON, "ratType");
    if (rat_type) {
    if (!cJSON_IsString(rat_type)) {
        ogs_error("OpenAPI_rat_freq_information_parseFromJSON() failed [rat_type]");
        goto end;
    }
    rat_typeVariable = OpenAPI_rat_type_FromString(rat_type->valuestring);
    }

    svc_exp_threshold = cJSON_GetObjectItemCaseSensitive(rat_freq_informationJSON, "svcExpThreshold");
    if (svc_exp_threshold) {
    svc_exp_threshold_local_nonprim = OpenAPI_threshold_level_parseFromJSON(svc_exp_threshold);
    if (!svc_exp_threshold_local_nonprim) {
        ogs_error("OpenAPI_threshold_level_parseFromJSON failed [svc_exp_threshold]");
        goto end;
    }
    }

    matching_dir = cJSON_GetObjectItemCaseSensitive(rat_freq_informationJSON, "matchingDir");
    if (matching_dir) {
    matching_dir_local_nonprim = OpenAPI_matching_direction_parseFromJSON(matching_dir);
    if (!matching_dir_local_nonprim) {
        ogs_error("OpenAPI_matching_direction_parseFromJSON failed [matching_dir]");
        goto end;
    }
    }

    rat_freq_information_local_var = OpenAPI_rat_freq_information_create (
        all_freq ? true : false,
        all_freq ? all_freq->valueint : 0,
        all_rat ? true : false,
        all_rat ? all_rat->valueint : 0,
        freq ? true : false,
        freq ? freq->valuedouble : 0,
        rat_type ? rat_typeVariable : 0,
        svc_exp_threshold ? svc_exp_threshold_local_nonprim : NULL,
        matching_dir ? matching_dir_local_nonprim : NULL
    );

    return rat_freq_information_local_var;
end:
    if (svc_exp_threshold_local_nonprim) {
        OpenAPI_threshold_level_free(svc_exp_threshold_local_nonprim);
        svc_exp_threshold_local_nonprim = NULL;
    }
    if (matching_dir_local_nonprim) {
        OpenAPI_matching_direction_free(matching_dir_local_nonprim);
        matching_dir_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_rat_freq_information_t *OpenAPI_rat_freq_information_copy(OpenAPI_rat_freq_information_t *dst, OpenAPI_rat_freq_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_rat_freq_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_rat_freq_information_convertToJSON() failed");
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

    OpenAPI_rat_freq_information_free(dst);
    dst = OpenAPI_rat_freq_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

