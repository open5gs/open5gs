
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "recur_time_1.h"

OpenAPI_recur_time_1_t *OpenAPI_recur_time_1_create(
    OpenAPI_valid_time_period_1_t *recur_time_window,
    OpenAPI_recur_type_e recur_type,
    OpenAPI_list_t *recur_month,
    OpenAPI_list_t *recur_week,
    OpenAPI_list_t *recur_day,
    OpenAPI_list_t *recur_date,
    char *recur_end_time
)
{
    OpenAPI_recur_time_1_t *recur_time_1_local_var = ogs_malloc(sizeof(OpenAPI_recur_time_1_t));
    ogs_assert(recur_time_1_local_var);

    recur_time_1_local_var->recur_time_window = recur_time_window;
    recur_time_1_local_var->recur_type = recur_type;
    recur_time_1_local_var->recur_month = recur_month;
    recur_time_1_local_var->recur_week = recur_week;
    recur_time_1_local_var->recur_day = recur_day;
    recur_time_1_local_var->recur_date = recur_date;
    recur_time_1_local_var->recur_end_time = recur_end_time;

    return recur_time_1_local_var;
}

void OpenAPI_recur_time_1_free(OpenAPI_recur_time_1_t *recur_time_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == recur_time_1) {
        return;
    }
    if (recur_time_1->recur_time_window) {
        OpenAPI_valid_time_period_1_free(recur_time_1->recur_time_window);
        recur_time_1->recur_time_window = NULL;
    }
    if (recur_time_1->recur_month) {
        OpenAPI_list_for_each(recur_time_1->recur_month, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(recur_time_1->recur_month);
        recur_time_1->recur_month = NULL;
    }
    if (recur_time_1->recur_week) {
        OpenAPI_list_for_each(recur_time_1->recur_week, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(recur_time_1->recur_week);
        recur_time_1->recur_week = NULL;
    }
    if (recur_time_1->recur_day) {
        OpenAPI_list_for_each(recur_time_1->recur_day, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(recur_time_1->recur_day);
        recur_time_1->recur_day = NULL;
    }
    if (recur_time_1->recur_date) {
        OpenAPI_list_for_each(recur_time_1->recur_date, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(recur_time_1->recur_date);
        recur_time_1->recur_date = NULL;
    }
    if (recur_time_1->recur_end_time) {
        ogs_free(recur_time_1->recur_end_time);
        recur_time_1->recur_end_time = NULL;
    }
    ogs_free(recur_time_1);
}

cJSON *OpenAPI_recur_time_1_convertToJSON(OpenAPI_recur_time_1_t *recur_time_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (recur_time_1 == NULL) {
        ogs_error("OpenAPI_recur_time_1_convertToJSON() failed [RecurTime_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (recur_time_1->recur_time_window) {
    cJSON *recur_time_window_local_JSON = OpenAPI_valid_time_period_1_convertToJSON(recur_time_1->recur_time_window);
    if (recur_time_window_local_JSON == NULL) {
        ogs_error("OpenAPI_recur_time_1_convertToJSON() failed [recur_time_window]");
        goto end;
    }
    cJSON_AddItemToObject(item, "recurTimeWindow", recur_time_window_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_recur_time_1_convertToJSON() failed [recur_time_window]");
        goto end;
    }
    }

    if (recur_time_1->recur_type != OpenAPI_recur_type_NULL) {
    if (cJSON_AddStringToObject(item, "recurType", OpenAPI_recur_type_ToString(recur_time_1->recur_type)) == NULL) {
        ogs_error("OpenAPI_recur_time_1_convertToJSON() failed [recur_type]");
        goto end;
    }
    }

    if (recur_time_1->recur_month) {
    cJSON *recur_monthList = cJSON_AddArrayToObject(item, "recurMonth");
    if (recur_monthList == NULL) {
        ogs_error("OpenAPI_recur_time_1_convertToJSON() failed [recur_month]");
        goto end;
    }
    OpenAPI_list_for_each(recur_time_1->recur_month, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_recur_time_1_convertToJSON() failed [recur_month]");
            goto end;
        }
        if (cJSON_AddNumberToObject(recur_monthList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_recur_time_1_convertToJSON() failed [recur_month]");
            goto end;
        }
    }
    }

    if (recur_time_1->recur_week) {
    cJSON *recur_weekList = cJSON_AddArrayToObject(item, "recurWeek");
    if (recur_weekList == NULL) {
        ogs_error("OpenAPI_recur_time_1_convertToJSON() failed [recur_week]");
        goto end;
    }
    OpenAPI_list_for_each(recur_time_1->recur_week, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_recur_time_1_convertToJSON() failed [recur_week]");
            goto end;
        }
        if (cJSON_AddNumberToObject(recur_weekList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_recur_time_1_convertToJSON() failed [recur_week]");
            goto end;
        }
    }
    }

    if (recur_time_1->recur_day) {
    cJSON *recur_dayList = cJSON_AddArrayToObject(item, "recurDay");
    if (recur_dayList == NULL) {
        ogs_error("OpenAPI_recur_time_1_convertToJSON() failed [recur_day]");
        goto end;
    }
    OpenAPI_list_for_each(recur_time_1->recur_day, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_recur_time_1_convertToJSON() failed [recur_day]");
            goto end;
        }
        if (cJSON_AddNumberToObject(recur_dayList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_recur_time_1_convertToJSON() failed [recur_day]");
            goto end;
        }
    }
    }

    if (recur_time_1->recur_date) {
    cJSON *recur_dateList = cJSON_AddArrayToObject(item, "recurDate");
    if (recur_dateList == NULL) {
        ogs_error("OpenAPI_recur_time_1_convertToJSON() failed [recur_date]");
        goto end;
    }
    OpenAPI_list_for_each(recur_time_1->recur_date, node) {
    }
    }

    if (recur_time_1->recur_end_time) {
    if (cJSON_AddStringToObject(item, "recurEndTime", recur_time_1->recur_end_time) == NULL) {
        ogs_error("OpenAPI_recur_time_1_convertToJSON() failed [recur_end_time]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_recur_time_1_t *OpenAPI_recur_time_1_parseFromJSON(cJSON *recur_time_1JSON)
{
    OpenAPI_recur_time_1_t *recur_time_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *recur_time_window = NULL;
    OpenAPI_valid_time_period_1_t *recur_time_window_local_nonprim = NULL;
    cJSON *recur_type = NULL;
    OpenAPI_recur_type_e recur_typeVariable = 0;
    cJSON *recur_month = NULL;
    OpenAPI_list_t *recur_monthList = NULL;
    cJSON *recur_week = NULL;
    OpenAPI_list_t *recur_weekList = NULL;
    cJSON *recur_day = NULL;
    OpenAPI_list_t *recur_dayList = NULL;
    cJSON *recur_date = NULL;
    OpenAPI_list_t *recur_dateList = NULL;
    cJSON *recur_end_time = NULL;
    recur_time_window = cJSON_GetObjectItemCaseSensitive(recur_time_1JSON, "recurTimeWindow");
    if (recur_time_window) {
    recur_time_window_local_nonprim = OpenAPI_valid_time_period_1_parseFromJSON(recur_time_window);
    if (!recur_time_window_local_nonprim) {
        ogs_error("OpenAPI_valid_time_period_1_parseFromJSON failed [recur_time_window]");
        goto end;
    }
    }

    recur_type = cJSON_GetObjectItemCaseSensitive(recur_time_1JSON, "recurType");
    if (recur_type) {
    if (!cJSON_IsString(recur_type)) {
        ogs_error("OpenAPI_recur_time_1_parseFromJSON() failed [recur_type]");
        goto end;
    }
    recur_typeVariable = OpenAPI_recur_type_FromString(recur_type->valuestring);
    }

    recur_month = cJSON_GetObjectItemCaseSensitive(recur_time_1JSON, "recurMonth");
    if (recur_month) {
        cJSON *recur_month_local = NULL;
        if (!cJSON_IsArray(recur_month)) {
            ogs_error("OpenAPI_recur_time_1_parseFromJSON() failed [recur_month]");
            goto end;
        }

        recur_monthList = OpenAPI_list_create();

        cJSON_ArrayForEach(recur_month_local, recur_month) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(recur_month_local)) {
                ogs_error("OpenAPI_recur_time_1_parseFromJSON() failed [recur_month]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_recur_time_1_parseFromJSON() failed [recur_month]");
                goto end;
            }
            *localDouble = recur_month_local->valuedouble;
            OpenAPI_list_add(recur_monthList, localDouble);
        }
    }

    recur_week = cJSON_GetObjectItemCaseSensitive(recur_time_1JSON, "recurWeek");
    if (recur_week) {
        cJSON *recur_week_local = NULL;
        if (!cJSON_IsArray(recur_week)) {
            ogs_error("OpenAPI_recur_time_1_parseFromJSON() failed [recur_week]");
            goto end;
        }

        recur_weekList = OpenAPI_list_create();

        cJSON_ArrayForEach(recur_week_local, recur_week) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(recur_week_local)) {
                ogs_error("OpenAPI_recur_time_1_parseFromJSON() failed [recur_week]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_recur_time_1_parseFromJSON() failed [recur_week]");
                goto end;
            }
            *localDouble = recur_week_local->valuedouble;
            OpenAPI_list_add(recur_weekList, localDouble);
        }
    }

    recur_day = cJSON_GetObjectItemCaseSensitive(recur_time_1JSON, "recurDay");
    if (recur_day) {
        cJSON *recur_day_local = NULL;
        if (!cJSON_IsArray(recur_day)) {
            ogs_error("OpenAPI_recur_time_1_parseFromJSON() failed [recur_day]");
            goto end;
        }

        recur_dayList = OpenAPI_list_create();

        cJSON_ArrayForEach(recur_day_local, recur_day) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(recur_day_local)) {
                ogs_error("OpenAPI_recur_time_1_parseFromJSON() failed [recur_day]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_recur_time_1_parseFromJSON() failed [recur_day]");
                goto end;
            }
            *localDouble = recur_day_local->valuedouble;
            OpenAPI_list_add(recur_dayList, localDouble);
        }
    }

    recur_date = cJSON_GetObjectItemCaseSensitive(recur_time_1JSON, "recurDate");
    if (recur_date) {
        cJSON *recur_date_local = NULL;
        if (!cJSON_IsArray(recur_date)) {
            ogs_error("OpenAPI_recur_time_1_parseFromJSON() failed [recur_date]");
            goto end;
        }

        recur_dateList = OpenAPI_list_create();

        cJSON_ArrayForEach(recur_date_local, recur_date) {
            double *localDouble = NULL;
            int *localInt = NULL;
        }
    }

    recur_end_time = cJSON_GetObjectItemCaseSensitive(recur_time_1JSON, "recurEndTime");
    if (recur_end_time) {
    if (!cJSON_IsString(recur_end_time) && !cJSON_IsNull(recur_end_time)) {
        ogs_error("OpenAPI_recur_time_1_parseFromJSON() failed [recur_end_time]");
        goto end;
    }
    }

    recur_time_1_local_var = OpenAPI_recur_time_1_create (
        recur_time_window ? recur_time_window_local_nonprim : NULL,
        recur_type ? recur_typeVariable : 0,
        recur_month ? recur_monthList : NULL,
        recur_week ? recur_weekList : NULL,
        recur_day ? recur_dayList : NULL,
        recur_date ? recur_dateList : NULL,
        recur_end_time && !cJSON_IsNull(recur_end_time) ? ogs_strdup(recur_end_time->valuestring) : NULL
    );

    return recur_time_1_local_var;
end:
    if (recur_time_window_local_nonprim) {
        OpenAPI_valid_time_period_1_free(recur_time_window_local_nonprim);
        recur_time_window_local_nonprim = NULL;
    }
    if (recur_monthList) {
        OpenAPI_list_for_each(recur_monthList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(recur_monthList);
        recur_monthList = NULL;
    }
    if (recur_weekList) {
        OpenAPI_list_for_each(recur_weekList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(recur_weekList);
        recur_weekList = NULL;
    }
    if (recur_dayList) {
        OpenAPI_list_for_each(recur_dayList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(recur_dayList);
        recur_dayList = NULL;
    }
    if (recur_dateList) {
        OpenAPI_list_for_each(recur_dateList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(recur_dateList);
        recur_dateList = NULL;
    }
    return NULL;
}

OpenAPI_recur_time_1_t *OpenAPI_recur_time_1_copy(OpenAPI_recur_time_1_t *dst, OpenAPI_recur_time_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_recur_time_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_recur_time_1_convertToJSON() failed");
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

    OpenAPI_recur_time_1_free(dst);
    dst = OpenAPI_recur_time_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

