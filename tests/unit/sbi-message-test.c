/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "ogs-sbi.h"
#include "core/abts.h"

static void sbi_message_test1(abts_case *tc, void *data)
{
	cJSON *item = NULL;
    OpenAPI_lnode_t *entry;

	OpenAPI_nf_profile_t *nf_profile1;
	OpenAPI_nf_profile_t *nf_profile2;

    OpenAPI_udr_info_t *udr_info1;
    OpenAPI_udr_info_t *udr_info2;

    OpenAPI_identity_range_t *identity_range1;
    OpenAPI_identity_range_t *identity_range2;

    OpenAPI_plmn_id_t plmn_id1[3];
    OpenAPI_plmn_id_t *plmn_id2;

    char *nsi;

    nf_profile1 = ogs_calloc(1, sizeof(*nf_profile1));
    ABTS_PTR_NOTNULL(tc, nf_profile1);
	nf_profile1->nf_instance_id = "NF_INSTANCE_ID";
	nf_profile1->nf_type = OpenAPI_nf_type_SMF;
	nf_profile1->nf_status = OpenAPI_nf_status_REGISTERED;

    memset(plmn_id1, 0, sizeof(plmn_id1));

    nf_profile1->plmn_list = OpenAPI_list_create();
    ABTS_PTR_NOTNULL(tc, nf_profile1->plmn_list);

    plmn_id1[0].mcc = "111";
    plmn_id1[0].mnc = "222";
    OpenAPI_list_add(nf_profile1->plmn_list, &plmn_id1[0]);
    plmn_id1[1].mcc = "333";
    plmn_id1[1].mnc = "444";
    OpenAPI_list_add(nf_profile1->plmn_list, &plmn_id1[1]);
    plmn_id1[2].mcc = "555";
    plmn_id1[2].mnc = "666";
    OpenAPI_list_add(nf_profile1->plmn_list, &plmn_id1[2]);

    nf_profile1->nsi_list = OpenAPI_list_create();
    OpenAPI_list_add(nf_profile1->nsi_list, "aaa");
    OpenAPI_list_add(nf_profile1->nsi_list, "bbbbb");

    nf_profile1->is_priority = true;
    nf_profile1->priority = 30;

    udr_info1 = nf_profile1->udr_info = ogs_calloc(1, sizeof(*udr_info1));
    ABTS_PTR_NOTNULL(tc, udr_info1);
    udr_info1->group_id = "group";

    udr_info1->supi_ranges = OpenAPI_list_create();
    ABTS_PTR_NOTNULL(tc, udr_info1->supi_ranges);

    udr_info1->gpsi_ranges = OpenAPI_list_create();
    ABTS_PTR_NOTNULL(tc, udr_info1->gpsi_ranges);

    identity_range1 = ogs_calloc(1, sizeof(*identity_range1));
    ABTS_PTR_NOTNULL(tc, identity_range1);
    identity_range1->start = "1";
    identity_range1->end = "9";
    OpenAPI_list_add(udr_info1->gpsi_ranges, identity_range1);

	item = OpenAPI_nf_profile_convertToJSON(nf_profile1);
    ABTS_PTR_NOTNULL(tc, item);

#if 0
    {
        char *str = cJSON_Print(item);
        printf("%s", str);
        ogs_free(str);
    }
#endif

	nf_profile2 = OpenAPI_nf_profile_parseFromJSON(item);
    ABTS_PTR_NOTNULL(tc, nf_profile2);

    ABTS_STR_EQUAL(tc,
            nf_profile1->nf_instance_id, nf_profile2->nf_instance_id);
    ABTS_INT_EQUAL(tc, nf_profile1->nf_type, nf_profile2->nf_type);
    ABTS_INT_EQUAL(tc, nf_profile1->nf_status, nf_profile2->nf_status);

    ABTS_PTR_NOTNULL(tc, nf_profile2->plmn_list);

    entry = nf_profile2->plmn_list->first;
    ABTS_PTR_NOTNULL(tc, entry);
    plmn_id2 = entry->data;
    ABTS_PTR_NOTNULL(tc, plmn_id2);
    ABTS_STR_EQUAL(tc, "111", plmn_id2->mcc);
    ABTS_STR_EQUAL(tc, "222", plmn_id2->mnc);

    entry = entry->next;
    ABTS_PTR_NOTNULL(tc, entry);
    plmn_id2 = entry->data;
    ABTS_PTR_NOTNULL(tc, plmn_id2);
    ABTS_STR_EQUAL(tc, "333", plmn_id2->mcc);
    ABTS_STR_EQUAL(tc, "444", plmn_id2->mnc);

    entry = entry->next;
    ABTS_PTR_NOTNULL(tc, entry);
    plmn_id2 = entry->data;
    ABTS_PTR_NOTNULL(tc, plmn_id2);
    ABTS_STR_EQUAL(tc, "555", plmn_id2->mcc);
    ABTS_STR_EQUAL(tc, "666", plmn_id2->mnc);

    entry = entry->next;
    ABTS_PTR_EQUAL(tc, NULL, entry);

    entry = nf_profile2->nsi_list->first;
    ABTS_PTR_NOTNULL(tc, entry);
    nsi = entry->data;
    ABTS_PTR_NOTNULL(tc, nsi);
    ABTS_STR_EQUAL(tc, "aaa", nsi);

    entry = entry->next;
    ABTS_PTR_NOTNULL(tc, entry);
    nsi = entry->data;
    ABTS_PTR_NOTNULL(tc, nsi);
    ABTS_STR_EQUAL(tc, "bbbbb", nsi);

    ABTS_TRUE(tc, nf_profile2->is_priority);
    ABTS_INT_EQUAL(tc, 30, nf_profile2->priority);

    udr_info2 = nf_profile2->udr_info;
    ABTS_PTR_NOTNULL(tc, udr_info2);
    ABTS_STR_EQUAL(tc, "group", udr_info2->group_id);

    ABTS_PTR_NOTNULL(tc, udr_info2->supi_ranges);
    ABTS_INT_EQUAL(tc, 0, udr_info2->supi_ranges->count);

    ABTS_PTR_NOTNULL(tc, udr_info2->gpsi_ranges);
    entry = udr_info2->gpsi_ranges->first;
    ABTS_PTR_NOTNULL(tc, entry);
    identity_range2 = entry->data;
    ABTS_PTR_NOTNULL(tc, identity_range2);
    ABTS_STR_EQUAL(tc, "1", identity_range2->start);
    ABTS_STR_EQUAL(tc, "9", identity_range2->end);

    ogs_free(identity_range1);

    OpenAPI_list_free(udr_info1->gpsi_ranges);
    OpenAPI_list_free(udr_info1->supi_ranges);
    ogs_free(udr_info1);

    OpenAPI_list_free(nf_profile1->plmn_list);
    OpenAPI_list_free(nf_profile1->nsi_list);
    ogs_free(nf_profile1);

    cJSON_Delete(item);

	OpenAPI_nf_profile_free(nf_profile2);
}

static void sbi_message_test2(abts_case *tc, void *data)
{
	cJSON *item = NULL;
    OpenAPI_lnode_t *entry;

    OpenAPI_nf_group_cond_t *nf_group_cond1;
    OpenAPI_nf_group_cond_t *nf_group_cond2;

	OpenAPI_smf_info_t *smf_info1;
	OpenAPI_smf_info_t *smf_info2;
    OpenAPI_access_type_e access_type;

    nf_group_cond1 = ogs_calloc(1, sizeof(*nf_group_cond1));
    ABTS_PTR_NOTNULL(tc, nf_group_cond1);
	nf_group_cond1->nf_type = OpenAPI_nf_group_cond_NFTYPE_UDR;
	nf_group_cond1->nf_group_id = "group";

	item = OpenAPI_nf_group_cond_convertToJSON(nf_group_cond1);
    ABTS_PTR_NOTNULL(tc, item);

#if 0
    {
        char *str = cJSON_Print(item);
        printf("%s", str);
        ogs_free(str);
    }
#endif

	nf_group_cond2 = OpenAPI_nf_group_cond_parseFromJSON(item);
    ABTS_PTR_NOTNULL(tc, nf_group_cond2);

    ABTS_STR_EQUAL(tc,
            nf_group_cond1->nf_group_id, nf_group_cond2->nf_group_id);
    ogs_free(nf_group_cond1);

    ABTS_INT_EQUAL(tc,
            OpenAPI_nf_group_cond_NFTYPE_UDR, nf_group_cond2->nf_type);

    cJSON_Delete(item);

    OpenAPI_nf_group_cond_free(nf_group_cond2);

    smf_info1 = ogs_calloc(1, sizeof(*smf_info1));
    ABTS_PTR_NOTNULL(tc, smf_info1);
    smf_info1->s_nssai_smf_info_list = OpenAPI_list_create();
	smf_info1->pgw_fqdn = "PGW_FQDN";
    smf_info1->access_type = OpenAPI_list_create();
    ABTS_PTR_NOTNULL(tc, smf_info1->access_type);

    OpenAPI_list_add(smf_info1->access_type,
            (void*)OpenAPI_access_type_3GPP_ACCESS);
    OpenAPI_list_add(smf_info1->access_type,
            (void*)OpenAPI_access_type_NON_3GPP_ACCESS);
	item = OpenAPI_smf_info_convertToJSON(smf_info1);
    ABTS_PTR_NOTNULL(tc, item);

#if 0
    {
        char *str = cJSON_Print(item);
        printf("%s", str);
        ogs_free(str);
    }
#endif

	smf_info2 = OpenAPI_smf_info_parseFromJSON(item);
    ABTS_PTR_NOTNULL(tc, smf_info2);

    ABTS_STR_EQUAL(tc,
            smf_info1->pgw_fqdn, smf_info2->pgw_fqdn);
    ABTS_PTR_NOTNULL(tc, smf_info2->s_nssai_smf_info_list);
    ABTS_INT_EQUAL(tc, smf_info1->s_nssai_smf_info_list->count,
                        smf_info2->s_nssai_smf_info_list->count);
    ABTS_PTR_NOTNULL(tc, smf_info2->access_type);
    ABTS_INT_EQUAL(tc, smf_info1->access_type->count,
                        smf_info2->access_type->count);

    entry = smf_info2->access_type->first;
    ABTS_PTR_NOTNULL(tc, entry);
    access_type = (intptr_t)entry->data;
    ABTS_INT_EQUAL(tc, OpenAPI_access_type_3GPP_ACCESS, access_type);

    entry = entry->next;
    ABTS_PTR_NOTNULL(tc, entry);
    access_type = (intptr_t)entry->data;
    ABTS_INT_EQUAL(tc, OpenAPI_access_type_NON_3GPP_ACCESS, access_type);

    OpenAPI_list_free(smf_info1->s_nssai_smf_info_list);
    OpenAPI_list_free(smf_info1->access_type);
    ogs_free(smf_info1);

    cJSON_Delete(item);

    OpenAPI_smf_info_free(smf_info2);
}

static void sbi_message_test3(abts_case *tc, void *data)
{
	cJSON *item = NULL;
    OpenAPI_lnode_t *entry;

	OpenAPI_nrf_info_t *nrf_info1;
	OpenAPI_nrf_info_t *nrf_info2;

    OpenAPI_map_t *served_pcf_info1 = NULL;
    OpenAPI_map_t *served_pcf_info2 = NULL;
    OpenAPI_pcf_info_t *pcf_info1;
    OpenAPI_pcf_info_t *pcf_info2;

    pcf_info1 = ogs_calloc(1, sizeof(*pcf_info1));
    ABTS_PTR_NOTNULL(tc, pcf_info1);
    pcf_info1->group_id = "pcf_group_id";
    pcf_info2 = ogs_calloc(1, sizeof(*pcf_info2));
    ABTS_PTR_NOTNULL(tc, pcf_info2);
    pcf_info2->group_id = "pcf_group_id2";

    served_pcf_info1 = OpenAPI_map_create("nfInstanceId", pcf_info1);
    ABTS_PTR_NOTNULL(tc, served_pcf_info1);
    served_pcf_info2 = OpenAPI_map_create("nfInstanceId2", pcf_info2);
    ABTS_PTR_NOTNULL(tc, served_pcf_info2);

    nrf_info1 = ogs_calloc(1, sizeof(*nrf_info1));
    ABTS_PTR_NOTNULL(tc, nrf_info1);

    nrf_info1->served_pcf_info = OpenAPI_list_create();
    ABTS_PTR_NOTNULL(tc, nrf_info1->served_pcf_info);
    OpenAPI_list_add(nrf_info1->served_pcf_info, served_pcf_info1);
    OpenAPI_list_add(nrf_info1->served_pcf_info, served_pcf_info2);

	item = OpenAPI_nrf_info_convertToJSON(nrf_info1);
    ABTS_PTR_NOTNULL(tc, item);

#if 0
    {
        char *str = cJSON_Print(item);
        printf("%s", str);
        ogs_free(str);
    }
#endif

    OpenAPI_map_free(served_pcf_info1);
    OpenAPI_map_free(served_pcf_info2);
    ogs_free(pcf_info1);
    ogs_free(pcf_info2);

	nrf_info2 = OpenAPI_nrf_info_parseFromJSON(item);
    ABTS_PTR_NOTNULL(tc, nrf_info2);

    ABTS_INT_EQUAL(tc, nrf_info1->served_pcf_info->count,
                        nrf_info2->served_pcf_info->count);

    OpenAPI_list_free(nrf_info1->served_pcf_info);
    ogs_free(nrf_info1);

    entry = nrf_info2->served_pcf_info->first;
    ABTS_PTR_NOTNULL(tc, entry);
    served_pcf_info1 = entry->data;
    ABTS_PTR_NOTNULL(tc, served_pcf_info1);
    ABTS_STR_EQUAL(tc, served_pcf_info1->key, "nfInstanceId");
    pcf_info1 = served_pcf_info1->value;
    ABTS_PTR_NOTNULL(tc, pcf_info1);
    ABTS_STR_EQUAL(tc, pcf_info1->group_id, "pcf_group_id");

    entry = entry->next;
    ABTS_PTR_NOTNULL(tc, entry);
    served_pcf_info1 = entry->data;
    ABTS_STR_EQUAL(tc, served_pcf_info1->key, "nfInstanceId2");
    pcf_info1 = served_pcf_info1->value;
    ABTS_PTR_NOTNULL(tc, pcf_info1);
    ABTS_STR_EQUAL(tc, pcf_info1->group_id, "pcf_group_id2");

    cJSON_Delete(item);
    OpenAPI_nrf_info_free(nrf_info2);
}

static void sbi_message_test4(abts_case *tc, void *data)
{
    ogs_time_t t;
    struct tm tm;
    bool rc;

    rc = ogs_sbi_time_from_string(&t, "1985-04-12T23:20:50.52");
    ABTS_INT_EQUAL(tc, 1, rc);

    ogs_gmtime(ogs_time_sec(t), &tm);
    ABTS_INT_EQUAL(tc, 85, tm.tm_year);
    ABTS_INT_EQUAL(tc, 3, tm.tm_mon);
    ABTS_INT_EQUAL(tc, 12, tm.tm_mday);
    ABTS_INT_EQUAL(tc, 23, tm.tm_hour);
    ABTS_INT_EQUAL(tc, 20, tm.tm_min);
    ABTS_INT_EQUAL(tc, 50, tm.tm_sec);
    ABTS_INT_EQUAL(tc, 0, tm.tm_isdst);
    ABTS_INT_EQUAL(tc, 0, tm.tm_gmtoff);
    ABTS_TRUE(tc, t == 482196050520000LL);

    rc = ogs_sbi_time_from_string(&t, "1996-12-19T16:39:57-0800");
    ABTS_INT_EQUAL(tc, 1, rc);

    ogs_gmtime(ogs_time_sec(t), &tm);
    ABTS_INT_EQUAL(tc, 96, tm.tm_year);
    ABTS_INT_EQUAL(tc, 11, tm.tm_mon);
    ABTS_INT_EQUAL(tc, 20, tm.tm_mday);
    ABTS_INT_EQUAL(tc, 0, tm.tm_hour);
    ABTS_INT_EQUAL(tc, 39, tm.tm_min);
    ABTS_INT_EQUAL(tc, 57, tm.tm_sec);
    ABTS_INT_EQUAL(tc, 0, tm.tm_isdst);
    ABTS_INT_EQUAL(tc, 0, tm.tm_gmtoff);
    ABTS_TRUE(tc, t == 851042397000000LL);

    tm.tm_mday = 19;
    tm.tm_hour = 16;
    tm.tm_gmtoff = -28800;

    rc = ogs_sbi_time_from_string(&t, "1996-12-19T16:39:57-08:00");
    ABTS_INT_EQUAL(tc, 1, rc);

    ogs_gmtime(ogs_time_sec(t), &tm);
    ABTS_INT_EQUAL(tc, 96, tm.tm_year);
    ABTS_INT_EQUAL(tc, 11, tm.tm_mon);
    ABTS_INT_EQUAL(tc, 20, tm.tm_mday);
    ABTS_INT_EQUAL(tc, 0, tm.tm_hour);
    ABTS_INT_EQUAL(tc, 39, tm.tm_min);
    ABTS_INT_EQUAL(tc, 57, tm.tm_sec);
    ABTS_INT_EQUAL(tc, 0, tm.tm_isdst);
    ABTS_INT_EQUAL(tc, 0, tm.tm_gmtoff);
    ABTS_TRUE(tc, t == 851042397000000LL);

    rc = ogs_sbi_time_from_string(&t, "1990-12-31T23:59:60Z");
    ABTS_INT_EQUAL(tc, 1, rc);

    ogs_gmtime(ogs_time_sec(t), &tm);
    ABTS_INT_EQUAL(tc, 91, tm.tm_year);
    ABTS_INT_EQUAL(tc, 0, tm.tm_mon);
    ABTS_INT_EQUAL(tc, 1, tm.tm_mday);
    ABTS_INT_EQUAL(tc, 0, tm.tm_hour);
    ABTS_INT_EQUAL(tc, 0, tm.tm_min);
    ABTS_INT_EQUAL(tc, 0, tm.tm_sec);
    ABTS_INT_EQUAL(tc, 0, tm.tm_isdst);
    ABTS_INT_EQUAL(tc, 0, tm.tm_gmtoff);
    ABTS_TRUE(tc, t == 662688000000000LL);

    rc = ogs_sbi_time_from_string(&t, "1990-12-31T15:59:60-0800");
    ABTS_INT_EQUAL(tc, 1, rc);

    ogs_gmtime(ogs_time_sec(t), &tm);
    ABTS_INT_EQUAL(tc, 91, tm.tm_year);
    ABTS_INT_EQUAL(tc, 0, tm.tm_mon);
    ABTS_INT_EQUAL(tc, 1, tm.tm_mday);
    ABTS_INT_EQUAL(tc, 0, tm.tm_hour);
    ABTS_INT_EQUAL(tc, 0, tm.tm_min);
    ABTS_INT_EQUAL(tc, 0, tm.tm_sec);
    ABTS_INT_EQUAL(tc, 0, tm.tm_isdst);
    ABTS_INT_EQUAL(tc, 0, tm.tm_gmtoff);
    ABTS_TRUE(tc, t == 662688000000000LL);

    rc = ogs_sbi_time_from_string(&t, "1990-12-31T15:59:60-08:00");
    ABTS_INT_EQUAL(tc, 1, rc);

    ogs_gmtime(ogs_time_sec(t), &tm);
    ABTS_INT_EQUAL(tc, 91, tm.tm_year);
    ABTS_INT_EQUAL(tc, 0, tm.tm_mon);
    ABTS_INT_EQUAL(tc, 1, tm.tm_mday);
    ABTS_INT_EQUAL(tc, 0, tm.tm_hour);
    ABTS_INT_EQUAL(tc, 0, tm.tm_min);
    ABTS_INT_EQUAL(tc, 0, tm.tm_sec);
    ABTS_INT_EQUAL(tc, 0, tm.tm_isdst);
    ABTS_INT_EQUAL(tc, 0, tm.tm_gmtoff);
    ABTS_TRUE(tc, t == 662688000000000LL);

    rc = ogs_sbi_time_from_string(&t, "1987-01-01T12:00:27.87+0020");
    ABTS_INT_EQUAL(tc, 1, rc);

    ogs_gmtime(ogs_time_sec(t), &tm);
    ABTS_INT_EQUAL(tc, 87, tm.tm_year);
    ABTS_INT_EQUAL(tc, 0, tm.tm_mon);
    ABTS_INT_EQUAL(tc, 1, tm.tm_mday);
    ABTS_INT_EQUAL(tc, 11, tm.tm_hour);
    ABTS_INT_EQUAL(tc, 40, tm.tm_min);
    ABTS_INT_EQUAL(tc, 27, tm.tm_sec);
    ABTS_INT_EQUAL(tc, 0, tm.tm_isdst);
    ABTS_INT_EQUAL(tc, 0, tm.tm_gmtoff);
    ABTS_TRUE(tc, t == 536499627870000LL);

    rc = ogs_sbi_time_from_string(&t, "1987-01-01T12:00:27.87+0020");
    ABTS_INT_EQUAL(tc, 1, rc);

    ogs_gmtime(ogs_time_sec(t), &tm);
    ABTS_INT_EQUAL(tc, 87, tm.tm_year);
    ABTS_INT_EQUAL(tc, 0, tm.tm_mon);
    ABTS_INT_EQUAL(tc, 1, tm.tm_mday);
    ABTS_INT_EQUAL(tc, 11, tm.tm_hour);
    ABTS_INT_EQUAL(tc, 40, tm.tm_min);
    ABTS_INT_EQUAL(tc, 27, tm.tm_sec);
    ABTS_INT_EQUAL(tc, 0, tm.tm_isdst);
    ABTS_INT_EQUAL(tc, 0, tm.tm_gmtoff);
    ABTS_TRUE(tc, t == 536499627870000LL);
}

static void sbi_message_test5(abts_case *tc, void *data)
{
    struct tm tm;

#define MAX_TIMESTR_LEN 128
    char datetime[MAX_TIMESTR_LEN];
    char timezone[MAX_TIMESTR_LEN];
    int len;

    char *str = NULL;

    str = ogs_sbi_timezone_string(-18000);
    ABTS_STR_EQUAL(tc, "-05:00", str);
    ogs_free(str);

    str = ogs_sbi_timezone_string(28800);
    ABTS_STR_EQUAL(tc, "+08:00", str);
    ogs_free(str);

    str = ogs_sbi_timezone_string(12600);
    ABTS_STR_EQUAL(tc, "+03:30", str);
    ogs_free(str);

    str = ogs_sbi_timezone_string(0);
    ABTS_STR_EQUAL(tc, "+00:00", str);
    ogs_free(str);

    ogs_localtime(ogs_time_sec(ogs_time_now()), &tm);
    ogs_strftime(datetime, sizeof datetime, "%Y-%m-%dT%H:%M:%S", &tm);

    len = ogs_strftimezone(timezone, MAX_TIMESTR_LEN, tm.tm_gmtoff);
    ABTS_INT_EQUAL(tc, 6, len);
}

static void sbi_message_test6(abts_case *tc, void *data)
{
    cJSON *item = NULL, *item2 = NULL;
    char *content = NULL;
    OpenAPI_smf_selection_subscription_data_t SmfSelectionSubscriptionData;
    OpenAPI_smf_selection_subscription_data_t *r1, *s1 = NULL;

    ogs_s_nssai_t s_nssai;

    OpenAPI_list_t *SubscribedSnssaiInfoList = NULL;
    OpenAPI_map_t *SubscribedSnssaiInfoMap = NULL;
    OpenAPI_snssai_info_t *SubscribedSnssaiInfo = NULL;

    OpenAPI_list_t *DnnInfoList = NULL;
    OpenAPI_dnn_info_t *DnnInfo = NULL;

    OpenAPI_lnode_t *node = NULL, *node2 = NULL;

    memset(&SmfSelectionSubscriptionData, 0,
            sizeof(SmfSelectionSubscriptionData));

    SubscribedSnssaiInfoList = OpenAPI_list_create();
    ogs_assert(SubscribedSnssaiInfoList);

    {
        DnnInfoList = OpenAPI_list_create();
        ogs_assert(DnnInfoList);

        {
            DnnInfo = ogs_calloc(1, sizeof(*DnnInfo));
            ogs_assert(DnnInfo);

            DnnInfo->dnn = "internet";

            DnnInfo->is_default_dnn_indicator = true;
            DnnInfo->default_dnn_indicator = true;

            OpenAPI_list_add(DnnInfoList, DnnInfo);
        }

        SubscribedSnssaiInfo = ogs_calloc(1, sizeof(*SubscribedSnssaiInfo));
        ogs_assert(SubscribedSnssaiInfo);

        if (DnnInfoList->count)
            SubscribedSnssaiInfo->dnn_infos = DnnInfoList;
        else
            OpenAPI_list_free(DnnInfoList);

        memset(&s_nssai, 0, sizeof(s_nssai));
        s_nssai.sst = 1;
        s_nssai.sd.v = OGS_S_NSSAI_NO_SD_VALUE;

        SubscribedSnssaiInfoMap = OpenAPI_map_create(
                ogs_sbi_s_nssai_to_string(&s_nssai),
                SubscribedSnssaiInfo);
        ogs_assert(SubscribedSnssaiInfoMap);
        ogs_assert(SubscribedSnssaiInfoMap->key);

        OpenAPI_list_add(SubscribedSnssaiInfoList, SubscribedSnssaiInfoMap);
    }

    if (SubscribedSnssaiInfoList->count)
        SmfSelectionSubscriptionData.subscribed_snssai_infos =
            SubscribedSnssaiInfoList;
    else
        OpenAPI_list_free(SubscribedSnssaiInfoList);

    item = OpenAPI_smf_selection_subscription_data_convertToJSON(
                &SmfSelectionSubscriptionData);

    SubscribedSnssaiInfoList =
        SmfSelectionSubscriptionData.subscribed_snssai_infos;
    OpenAPI_list_for_each(SubscribedSnssaiInfoList, node) {
        SubscribedSnssaiInfoMap = node->data;
        if (SubscribedSnssaiInfoMap) {
            SubscribedSnssaiInfo = SubscribedSnssaiInfoMap->value;
            if (SubscribedSnssaiInfo) {
                DnnInfoList = SubscribedSnssaiInfo->dnn_infos;
                if (DnnInfoList) {
                    OpenAPI_list_for_each(DnnInfoList, node2) {
                        DnnInfo = node2->data;
                        if (DnnInfo) {
                            ogs_free(DnnInfo);
                        }
                    }
                    OpenAPI_list_free(DnnInfoList);
                }
                ogs_free(SubscribedSnssaiInfo);
            }
            if (SubscribedSnssaiInfoMap->key)
                ogs_free(SubscribedSnssaiInfoMap->key);
            ogs_free(SubscribedSnssaiInfoMap);
        }
    }
    OpenAPI_list_free(SubscribedSnssaiInfoList);

    r1 = OpenAPI_smf_selection_subscription_data_parseFromJSON(item);
    cJSON_Delete(item);

    item = OpenAPI_smf_selection_subscription_data_convertToJSON(r1);

    SubscribedSnssaiInfoList = r1->subscribed_snssai_infos;
    OpenAPI_list_for_each(SubscribedSnssaiInfoList, node) {
        SubscribedSnssaiInfoMap = node->data;
        if (SubscribedSnssaiInfoMap) {
            memset(&s_nssai, 0, sizeof(s_nssai));
            ogs_sbi_s_nssai_from_string(&s_nssai, SubscribedSnssaiInfoMap->key);
            ABTS_INT_EQUAL(tc, 1, s_nssai.sst);
            ABTS_INT_EQUAL(tc, OGS_S_NSSAI_NO_SD_VALUE, s_nssai.sd.v);
        }
    }

    OpenAPI_smf_selection_subscription_data_free(r1);

    content = cJSON_Print(item);
    cJSON_Delete(item);
    ogs_assert(content);
    ABTS_INT_EQUAL(tc, 148, (int)strlen(content));
    ogs_free(content);
}

abts_suite *test_sbi_message(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, sbi_message_test1, NULL);
    abts_run_test(suite, sbi_message_test2, NULL);
    abts_run_test(suite, sbi_message_test3, NULL);
    abts_run_test(suite, sbi_message_test4, NULL);
    abts_run_test(suite, sbi_message_test5, NULL);
    abts_run_test(suite, sbi_message_test6, NULL);

    return suite;
}
