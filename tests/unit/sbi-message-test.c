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
#include "contrib/multipart_parser.h"
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
    ogs_s_nssai_t s_nssai1, s_nssai2;
    char *string = NULL;
    bool rc = false;

    memset(&s_nssai1, 0, sizeof(ogs_s_nssai_t));

    s_nssai1.sst = 255;
    s_nssai1.sd.v = 0x19cde0;

    string = ogs_sbi_s_nssai_to_string(&s_nssai1);
    ABTS_PTR_NOTNULL(tc, string);

    rc = ogs_sbi_s_nssai_from_string(&s_nssai2, string);
    ABTS_TRUE(tc, rc == true);

    ogs_free(string);

    ABTS_INT_EQUAL(tc, s_nssai1.sst, s_nssai2.sst);
    ABTS_INT_EQUAL(tc, s_nssai1.sd.v, s_nssai2.sd.v);

    s_nssai1.sst = 1;
    s_nssai1.sd.v = OGS_S_NSSAI_NO_SD_VALUE;

    string = ogs_sbi_s_nssai_to_string(&s_nssai1);
    ABTS_PTR_NOTNULL(tc, string);

    rc = ogs_sbi_s_nssai_from_string(&s_nssai2, string);
    ABTS_TRUE(tc, rc == true);

    ogs_free(string);

    ABTS_INT_EQUAL(tc, s_nssai1.sst, s_nssai2.sst);
    ABTS_INT_EQUAL(tc, s_nssai1.sd.v, s_nssai2.sd.v);
}

static void sbi_message_test7(abts_case *tc, void *data)
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
        s_nssai.sst = 255;
        s_nssai.sd.v = 0x19cde0;

        SubscribedSnssaiInfoMap = OpenAPI_map_create(
                ogs_sbi_s_nssai_to_string(&s_nssai), SubscribedSnssaiInfo);
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
            ABTS_INT_EQUAL(tc, 255, s_nssai.sst);
            ABTS_INT_EQUAL(tc, 0x19cde0, s_nssai.sd.v);
        }
    }

    OpenAPI_smf_selection_subscription_data_free(r1);

    content = cJSON_Print(item);
    cJSON_Delete(item);
    ogs_assert(content);
    ABTS_INT_EQUAL(tc, 139, (int)strlen(content));
    ogs_free(content);
}

static void sbi_message_test8(abts_case *tc, void *data)
{
    ABTS_INT_EQUAL(tc, OpenAPI_nf_type_NRF,
        ogs_sbi_service_type_to_nf_type(OGS_SBI_SERVICE_TYPE_NNRF_NFM));
    ABTS_STR_EQUAL(tc, OGS_SBI_SERVICE_NAME_NNRF_NFM,
        ogs_sbi_service_type_to_name(OGS_SBI_SERVICE_TYPE_NNRF_NFM));
    ABTS_INT_EQUAL(tc, OGS_SBI_SERVICE_TYPE_NNRF_NFM,
        ogs_sbi_service_type_from_name(OGS_SBI_SERVICE_NAME_NNRF_NFM));
    ABTS_INT_EQUAL(tc, OpenAPI_nf_type_UDM,
        ogs_sbi_service_type_to_nf_type(OGS_SBI_SERVICE_TYPE_NUDM_SDM));
    ABTS_STR_EQUAL(tc, OGS_SBI_SERVICE_NAME_NUDM_SDM,
        ogs_sbi_service_type_to_name(OGS_SBI_SERVICE_TYPE_NUDM_SDM));
    ABTS_INT_EQUAL(tc, OGS_SBI_SERVICE_TYPE_NUDM_SDM,
        ogs_sbi_service_type_from_name(OGS_SBI_SERVICE_NAME_NUDM_SDM));
    ABTS_INT_EQUAL(tc, OpenAPI_nf_type_AMF,
        ogs_sbi_service_type_to_nf_type(OGS_SBI_SERVICE_TYPE_NAMF_COMM));
    ABTS_STR_EQUAL(tc, OGS_SBI_SERVICE_NAME_NAMF_COMM,
        ogs_sbi_service_type_to_name(OGS_SBI_SERVICE_TYPE_NAMF_COMM));
    ABTS_INT_EQUAL(tc, OGS_SBI_SERVICE_TYPE_NAMF_COMM,
        ogs_sbi_service_type_from_name(OGS_SBI_SERVICE_NAME_NAMF_COMM));
    ABTS_INT_EQUAL(tc, OpenAPI_nf_type_SMF,
        ogs_sbi_service_type_to_nf_type(OGS_SBI_SERVICE_TYPE_NSMF_PDUSESSION));
    ABTS_STR_EQUAL(tc, OGS_SBI_SERVICE_NAME_NSMF_PDUSESSION,
        ogs_sbi_service_type_to_name(OGS_SBI_SERVICE_TYPE_NSMF_PDUSESSION));
    ABTS_INT_EQUAL(tc, OGS_SBI_SERVICE_TYPE_NSMF_PDUSESSION,
        ogs_sbi_service_type_from_name(OGS_SBI_SERVICE_NAME_NSMF_PDUSESSION));
    ABTS_INT_EQUAL(tc, OpenAPI_nf_type_AUSF,
        ogs_sbi_service_type_to_nf_type(OGS_SBI_SERVICE_TYPE_NAUSF_AUTH));
    ABTS_STR_EQUAL(tc, OGS_SBI_SERVICE_NAME_NAUSF_AUTH,
        ogs_sbi_service_type_to_name(OGS_SBI_SERVICE_TYPE_NAUSF_AUTH));
    ABTS_INT_EQUAL(tc, OGS_SBI_SERVICE_TYPE_NAUSF_AUTH,
        ogs_sbi_service_type_from_name(OGS_SBI_SERVICE_NAME_NAUSF_AUTH));
    ABTS_INT_EQUAL(tc, OpenAPI_nf_type_NEF,
        ogs_sbi_service_type_to_nf_type(
            OGS_SBI_SERVICE_TYPE_NNEF_PFDMANAGEMENT));
    ABTS_STR_EQUAL(tc, OGS_SBI_SERVICE_NAME_NNEF_PFDMANAGEMENT,
        ogs_sbi_service_type_to_name(OGS_SBI_SERVICE_TYPE_NNEF_PFDMANAGEMENT));
    ABTS_INT_EQUAL(tc, OGS_SBI_SERVICE_TYPE_NNEF_PFDMANAGEMENT,
        ogs_sbi_service_type_from_name(
            OGS_SBI_SERVICE_NAME_NNEF_PFDMANAGEMENT));
    ABTS_INT_EQUAL(tc, OpenAPI_nf_type_PCF,
        ogs_sbi_service_type_to_nf_type(
            OGS_SBI_SERVICE_TYPE_NPCF_AM_POLICY_CONTROL));
    ABTS_STR_EQUAL(tc, OGS_SBI_SERVICE_NAME_NPCF_AM_POLICY_CONTROL,
        ogs_sbi_service_type_to_name(
            OGS_SBI_SERVICE_TYPE_NPCF_AM_POLICY_CONTROL));
    ABTS_INT_EQUAL(tc, OGS_SBI_SERVICE_TYPE_NPCF_AM_POLICY_CONTROL,
        ogs_sbi_service_type_from_name(
            OGS_SBI_SERVICE_NAME_NPCF_AM_POLICY_CONTROL));
    ABTS_INT_EQUAL(tc, OpenAPI_nf_type_SMSF,
        ogs_sbi_service_type_to_nf_type(OGS_SBI_SERVICE_TYPE_NSMSF_SMS));
    ABTS_STR_EQUAL(tc, OGS_SBI_SERVICE_NAME_NSMSF_SMS,
        ogs_sbi_service_type_to_name(OGS_SBI_SERVICE_TYPE_NSMSF_SMS));
    ABTS_INT_EQUAL(tc, OGS_SBI_SERVICE_TYPE_NSMSF_SMS,
        ogs_sbi_service_type_from_name(OGS_SBI_SERVICE_NAME_NSMSF_SMS));
    ABTS_INT_EQUAL(tc, OpenAPI_nf_type_NSSF,
        ogs_sbi_service_type_to_nf_type(
            OGS_SBI_SERVICE_TYPE_NNSSF_NSSELECTION));
    ABTS_STR_EQUAL(tc, OGS_SBI_SERVICE_NAME_NNSSF_NSSELECTION,
        ogs_sbi_service_type_to_name(OGS_SBI_SERVICE_TYPE_NNSSF_NSSELECTION));
    ABTS_INT_EQUAL(tc, OGS_SBI_SERVICE_TYPE_NNSSF_NSSELECTION,
        ogs_sbi_service_type_from_name(OGS_SBI_SERVICE_NAME_NNSSF_NSSELECTION));
    ABTS_INT_EQUAL(tc, OpenAPI_nf_type_UDR,
        ogs_sbi_service_type_to_nf_type(OGS_SBI_SERVICE_TYPE_NUDR_DR));
    ABTS_STR_EQUAL(tc, OGS_SBI_SERVICE_NAME_NUDR_DR,
        ogs_sbi_service_type_to_name(OGS_SBI_SERVICE_TYPE_NUDR_DR));
    ABTS_INT_EQUAL(tc, OGS_SBI_SERVICE_TYPE_NUDR_DR,
        ogs_sbi_service_type_from_name(OGS_SBI_SERVICE_NAME_NUDR_DR));
    ABTS_INT_EQUAL(tc, OpenAPI_nf_type_LMF,
        ogs_sbi_service_type_to_nf_type(OGS_SBI_SERVICE_TYPE_NLMF_LOC));
    ABTS_STR_EQUAL(tc, OGS_SBI_SERVICE_NAME_NLMF_LOC,
        ogs_sbi_service_type_to_name(OGS_SBI_SERVICE_TYPE_NLMF_LOC));
    ABTS_INT_EQUAL(tc, OGS_SBI_SERVICE_TYPE_NLMF_LOC,
        ogs_sbi_service_type_from_name(OGS_SBI_SERVICE_NAME_NLMF_LOC));
    ABTS_INT_EQUAL(tc, OpenAPI_nf_type_5G_EIR,
        ogs_sbi_service_type_to_nf_type(OGS_SBI_SERVICE_TYPE_N5G_EIR_EIC));
    ABTS_STR_EQUAL(tc, OGS_SBI_SERVICE_NAME_N5G_EIR_EIC,
        ogs_sbi_service_type_to_name(OGS_SBI_SERVICE_TYPE_N5G_EIR_EIC));
    ABTS_INT_EQUAL(tc, OGS_SBI_SERVICE_TYPE_N5G_EIR_EIC,
        ogs_sbi_service_type_from_name(OGS_SBI_SERVICE_NAME_N5G_EIR_EIC));
    ABTS_INT_EQUAL(tc, OpenAPI_nf_type_BSF,
        ogs_sbi_service_type_to_nf_type(OGS_SBI_SERVICE_TYPE_NBSF_MANAGEMENT));
    ABTS_STR_EQUAL(tc, OGS_SBI_SERVICE_NAME_NBSF_MANAGEMENT,
        ogs_sbi_service_type_to_name(OGS_SBI_SERVICE_TYPE_NBSF_MANAGEMENT));
    ABTS_INT_EQUAL(tc, OGS_SBI_SERVICE_TYPE_NBSF_MANAGEMENT,
        ogs_sbi_service_type_from_name(OGS_SBI_SERVICE_NAME_NBSF_MANAGEMENT));
    ABTS_INT_EQUAL(tc, OpenAPI_nf_type_CHF,
        ogs_sbi_service_type_to_nf_type(
            OGS_SBI_SERVICE_TYPE_NCHF_SPENDINGLIMITCONTROL));
    ABTS_STR_EQUAL(tc, OGS_SBI_SERVICE_NAME_NCHF_SPENDINGLIMITCONTROL,
        ogs_sbi_service_type_to_name(
            OGS_SBI_SERVICE_TYPE_NCHF_SPENDINGLIMITCONTROL));
    ABTS_INT_EQUAL(tc, OGS_SBI_SERVICE_TYPE_NCHF_SPENDINGLIMITCONTROL,
        ogs_sbi_service_type_from_name(
            OGS_SBI_SERVICE_NAME_NCHF_SPENDINGLIMITCONTROL));
    ABTS_INT_EQUAL(tc, OpenAPI_nf_type_NWDAF,
        ogs_sbi_service_type_to_nf_type(
            OGS_SBI_SERVICE_TYPE_NNWDAF_EVENTSSUBSCRIPTION));
    ABTS_STR_EQUAL(tc, OGS_SBI_SERVICE_NAME_NNWDAF_EVENTSSUBSCRIPTION,
        ogs_sbi_service_type_to_name(
            OGS_SBI_SERVICE_TYPE_NNWDAF_EVENTSSUBSCRIPTION));
    ABTS_INT_EQUAL(tc, OGS_SBI_SERVICE_TYPE_NNWDAF_EVENTSSUBSCRIPTION,
        ogs_sbi_service_type_from_name(
            OGS_SBI_SERVICE_NAME_NNWDAF_EVENTSSUBSCRIPTION));
    ABTS_INT_EQUAL(tc, OpenAPI_nf_type_GMLC,
        ogs_sbi_service_type_to_nf_type(OGS_SBI_SERVICE_TYPE_NGMLC_LOC));
    ABTS_STR_EQUAL(tc, OGS_SBI_SERVICE_NAME_NGMLC_LOC,
        ogs_sbi_service_type_to_name(OGS_SBI_SERVICE_TYPE_NGMLC_LOC));
    ABTS_INT_EQUAL(tc, OGS_SBI_SERVICE_TYPE_NGMLC_LOC,
        ogs_sbi_service_type_from_name(OGS_SBI_SERVICE_NAME_NGMLC_LOC));
    ABTS_INT_EQUAL(tc, OpenAPI_nf_type_UCMF,
        ogs_sbi_service_type_to_nf_type(
            OGS_SBI_SERVICE_TYPE_NUCMF_PROVISIONING));
    ABTS_STR_EQUAL(tc, OGS_SBI_SERVICE_NAME_NUCMF_PROVISIONING,
        ogs_sbi_service_type_to_name(OGS_SBI_SERVICE_TYPE_NUCMF_PROVISIONING));
    ABTS_INT_EQUAL(tc, OGS_SBI_SERVICE_TYPE_NUCMF_PROVISIONING,
        ogs_sbi_service_type_from_name(
            OGS_SBI_SERVICE_NAME_NUCMF_PROVISIONING));
    ABTS_INT_EQUAL(tc, OpenAPI_nf_type_HSS,
        ogs_sbi_service_type_to_nf_type(OGS_SBI_SERVICE_TYPE_NHSS_SDM));
    ABTS_STR_EQUAL(tc, OGS_SBI_SERVICE_NAME_NHSS_SDM,
        ogs_sbi_service_type_to_name(OGS_SBI_SERVICE_TYPE_NHSS_SDM));
    ABTS_INT_EQUAL(tc, OGS_SBI_SERVICE_TYPE_NHSS_SDM,
        ogs_sbi_service_type_from_name(OGS_SBI_SERVICE_NAME_NHSS_SDM));
    ABTS_INT_EQUAL(tc, OpenAPI_nf_type_SEPP,
        ogs_sbi_service_type_to_nf_type(OGS_SBI_SERVICE_TYPE_NSEPP_TELESCOPIC));
    ABTS_STR_EQUAL(tc, OGS_SBI_SERVICE_NAME_NSEPP_TELESCOPIC,
        ogs_sbi_service_type_to_name(OGS_SBI_SERVICE_TYPE_NSEPP_TELESCOPIC));
    ABTS_INT_EQUAL(tc, OGS_SBI_SERVICE_TYPE_NSEPP_TELESCOPIC,
        ogs_sbi_service_type_from_name(OGS_SBI_SERVICE_NAME_NSEPP_TELESCOPIC));
    ABTS_INT_EQUAL(tc, OpenAPI_nf_type_SOR_AF,
        ogs_sbi_service_type_to_nf_type(OGS_SBI_SERVICE_TYPE_NSORAF_SOR));
    ABTS_STR_EQUAL(tc, OGS_SBI_SERVICE_NAME_NSORAF_SOR,
        ogs_sbi_service_type_to_name(OGS_SBI_SERVICE_TYPE_NSORAF_SOR));
    ABTS_INT_EQUAL(tc, OGS_SBI_SERVICE_TYPE_NSORAF_SOR,
        ogs_sbi_service_type_from_name(OGS_SBI_SERVICE_NAME_NSORAF_SOR));
    ABTS_INT_EQUAL(tc, OpenAPI_nf_type_SPAF,
        ogs_sbi_service_type_to_nf_type(
            OGS_SBI_SERVICE_TYPE_NSPAF_SECURED_PACKET));
    ABTS_STR_EQUAL(tc, OGS_SBI_SERVICE_NAME_NSPAF_SECURED_PACKET,
        ogs_sbi_service_type_to_name(
            OGS_SBI_SERVICE_TYPE_NSPAF_SECURED_PACKET));
    ABTS_INT_EQUAL(tc, OGS_SBI_SERVICE_TYPE_NSPAF_SECURED_PACKET,
        ogs_sbi_service_type_from_name(
            OGS_SBI_SERVICE_NAME_NSPAF_SECURED_PACKET));
    ABTS_INT_EQUAL(tc, OpenAPI_nf_type_UDSF,
        ogs_sbi_service_type_to_nf_type(OGS_SBI_SERVICE_TYPE_NUDSF_DR));
    ABTS_STR_EQUAL(tc, OGS_SBI_SERVICE_NAME_NUDSF_DR,
        ogs_sbi_service_type_to_name(OGS_SBI_SERVICE_TYPE_NUDSF_DR));
    ABTS_INT_EQUAL(tc, OGS_SBI_SERVICE_TYPE_NUDSF_DR,
        ogs_sbi_service_type_from_name(OGS_SBI_SERVICE_NAME_NUDSF_DR));
    ABTS_INT_EQUAL(tc, OpenAPI_nf_type_NSSAAF,
        ogs_sbi_service_type_to_nf_type(OGS_SBI_SERVICE_TYPE_NNSSAAF_NSSAA));
    ABTS_STR_EQUAL(tc, OGS_SBI_SERVICE_NAME_NNSSAAF_NSSAA,
        ogs_sbi_service_type_to_name(OGS_SBI_SERVICE_TYPE_NNSSAAF_NSSAA));
    ABTS_INT_EQUAL(tc, OGS_SBI_SERVICE_TYPE_NNSSAAF_NSSAA,
        ogs_sbi_service_type_from_name(OGS_SBI_SERVICE_NAME_NNSSAAF_NSSAA));
}

static void sbi_message_test9(abts_case *tc, void *data)
{
    const char *original = "{\"sst\": 1, \"sd\": \"A08923\"}";
    char *encoded = ogs_sbi_url_encode(original);
    char *decoded = ogs_sbi_url_decode(encoded);

    ABTS_STR_EQUAL(tc,
            "%7B%22sst%22%3A 1%2C %22sd%22%3A %22A08923%22%7D", encoded);
    ABTS_STR_EQUAL(tc, original, decoded);

    ogs_free(encoded);
    ogs_free(decoded);
}

typedef struct multipart_parser_data_s {
    int num_of_part;
    struct {
        char *content_type;
        char *content_id;
        char *content;
        size_t content_length;
    } part[OGS_SBI_MAX_NUM_OF_PART];

    char *header_field;
} multipart_parser_data_t;

static int on_header_field(
        multipart_parser *parser, const char *at, size_t length)
{
    return 0;
}

static int on_header_value(
        multipart_parser *parser, const char *at, size_t length)
{
    return 0;
}

static int on_part_data(
        multipart_parser *parser, const char *at, size_t length)
{
    return 0;
}

static int on_part_data_end(multipart_parser *parser)
{
    multipart_parser_data_t *data = NULL;

    ogs_assert(parser);
    data = multipart_parser_get_data(parser);
    ogs_assert(data);

    if (data->num_of_part < OGS_SBI_MAX_NUM_OF_PART) {
        data->num_of_part++;
    }

    return 0;
}
static void sbi_message_test10(abts_case *tc, void *param)
{
#define TEST_SBI_MAX_MESSAGE 16
    ogs_pkbuf_t *pkbuf = NULL;
    const char *payload[TEST_SBI_MAX_MESSAGE] = {
        /* No Preamble CRLF */
        "2d2d3d2d42"
        "5876585878326357 4276755951577237 45573477513d3d0d 0a436f6e74656e74"
        "2d547970653a2061 70706c6963617469 6f6e2f6a736f6e0d 0a0d0a7b0a092273"
        "757069223a092269 6d73692d32303630 3135353030303030 303030222c0a0922"
        "706569223a092269 6d656973762d3433 3730383136313235 383136313531222c"
        "0a09227064755365 7373696f6e496422 3a09312c0a092264 6e6e223a09226465"
        "6d6f2e6e6f6b6961 2e6d6e633030312e 6d63633230362e67 707273222c0a0922"
        "734e73736169223a 097b0a0909227373 74223a09312c0a09 09227364223a0922"
        "616263646566220a 097d2c0a09227365 7276696e674e6649 64223a0922333037"
        "63653332342d6434 32372d343165652d 393235392d353339 6262383833363465"
        "32222c0a09226775 616d69223a097b0a 090922706c6d6e49 64223a097b0a0909"
        "09226d6363223a09 22323036222c0a09 0909226d6e63223a 09223031220a0909"
        "7d2c0a090922616d 664964223a092230 3230303430220a09 7d2c0a0922736572"
        "76696e674e657477 6f726b223a097b0a 0909226d6363223a 0922323036222c0a"
        "0909226d6e63223a 09223031220a097d 2c0a09226e31536d 4d7367223a097b0a"
        "090922636f6e7465 6e744964223a0922 35676e61732d736d 220a097d2c0a0922"
        "616e54797065223a 0922334750505f41 4343455353222c0a 0922726174547970"
        "65223a09224e5222 2c0a092275654c6f 636174696f6e223a 097b0a0909226e72"
        "4c6f636174696f6e 223a097b0a090909 22746169223a097b 0a0909090922706c"
        "6d6e4964223a097b 0a0909090909226d 6363223a09223230 36222c0a09090909"
        "09226d6e63223a09 223031220a090909 097d2c0a09090909 22746163223a0922"
        "303030303032220a 0909097d2c0a0909 09226e636769223a 097b0a0909090922"
        "706c6d6e4964223a 097b0a0909090909 226d6363223a0922 323036222c0a0909"
        "090909226d6e6322 3a09223031220a09 0909097d2c0a0909 0909226e7243656c"
        "6c4964223a092230 3030303030303130 220a0909097d2c0a 0909092275654c6f"
        "636174696f6e5469 6d657374616d7022 3a0922323032342d 30322d3235543231"
        "3a34363a35332e31 34383839305a220a 09097d0a097d2c0a 0922756554696d65"
        "5a6f6e65223a0922 2b30303a3030222c 0a0922736d436f6e 7465787453746174"
        "7573557269223a09 22687474703a2f2f 31302e35302e312e 323a383038302f6e"
        "616d662d63616c6c 6261636b2f76312f 696d73692d323036 3031353530303030"
        "303030302f736d2d 636f6e746578742d 7374617475732f31 222c0a0922706366"
        "4964223a09226238 3666613934652d64 3432352d34316565 2d623030302d3133"
        "6265303937343265 3539220a7d0d0a2d 2d3d2d4258765858 7832635742767559"
        "5157723745573477 513d3d0d0a436f6e 74656e742d49643a 2035676e61732d73"
        "6d0d0a436f6e7465 6e742d547970653a 206170706c696361 74696f6e2f766e64"
        "2e336770702e3567 6e61730d0a0d0a2e 0101c1ffff91a128 01007b000780000a"
        "00000d000d0a2d2d 3d2d425876585878 3263574276755951 5772374557347751"
        "3d3d2d2d0d0a",

        "0d0a" /* WITH Preamble CRLF */
        "2d2d67"
        "6330704a7130386a 55353334630d0a43 6f6e74656e742d54 7970653a20617070"
        "6c69636174696f6e 2f6a736f6e0d0a0d 0a7b226e314d6573 73616765436f6e74"
        "61696e6572223a7b 226e314d65737361 6765436c61737322 3a22534d222c226e"
        "314d657373616765 436f6e74656e7422 3a7b22636f6e7465 6e744964223a226e"
        "31436f6e74656e74 496431227d7d2c22 6e32496e666f436f 6e7461696e657222"
        "3a7b226e32496e66 6f726d6174696f6e 436c617373223a22 534d222c22736d49"
        "6e666f223a7b2270 647553657373696f 6e4964223a312c22 6e32496e666f436f"
        "6e74656e74223a7b 226e676170496554 797065223a225044 555f5245535f5345"
        "5455505f52455122 2c226e6761704461 7461223a7b22636f 6e74656e74496422"
        "3a226e32436f6e74 656e74496431227d 7d2c22734e737361 69223a7b22737374"
        "223a312c22736422 3a22414243444546 227d7d7d2c226c61 73744d7367496e64"
        "69636174696f6e22 3a66616c73652c22 7064755365737369 6f6e4964223a312c"
        "226e316e32466169 6c7572655478664e 6f74696655524922 3a22687474703a2f"
        "2f312e312e312e31 3a36353532302f6e 616d662d636f6d6d 2f76312f75652d63"
        "6f6e74657874732f 3030303135313230 222c22736d665265 616c6c6f63617469"
        "6f6e496e64223a66 616c73657d0d0a2d 2d676330704a7130 386a55353334630d"
        "0a436f6e74656e74 2d547970653a2061 70706c6963617469 6f6e2f766e642e33"
        "6770702e35676e61 730d0a436f6e7465 6e742d49643a206e 31436f6e74656e74"
        "4964310d0a0d0a2e 0101c21100090100 0631310101fe0106 0b00010b00012905"
        "012b000801220401 abcdef7900060120 410101097b000f80 000d04d043fefe00"
        "0d04d043ffff251e 0464656d6f056e6f 6b6961066d6e6330 3031066d63633230"
        "3604677072730d0a 2d2d676330704a71 30386a5535333463 0d0a436f6e74656e"
        "742d547970653a20 6170706c69636174 696f6e2f766e642e 336770702e6e6761"
        "700d0a436f6e7465 6e742d49643a206e 32436f6e74656e74 4964310d0a0d0a00"
        "00040082000a0c3b 9aca00303b9aca00 008b000a01f00101 0168003800000086"
        "0001000088000700 010000091c000d0a 2d2d676330704a71 30386a5535333463"
        "2d2d0d0a"
        "",

    };
    uint16_t len[TEST_SBI_MAX_MESSAGE] = {
        1163,
        841,
        0,
    };
    int num_of_part[TEST_SBI_MAX_MESSAGE] = {
        2,
        3,
        0,
    };
    char hexbuf[OGS_HUGE_LEN];

    char *boundary = NULL;
    int i, num;

    multipart_parser_settings settings;
    multipart_parser_data_t data;

    multipart_parser *parser = NULL;

    for (num = 0; num < 2; num++) {
        int preamble = 0;
        pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
        ogs_assert(pkbuf);
        ogs_pkbuf_put_data(pkbuf,
            ogs_hex_from_string(payload[num], hexbuf, sizeof(hexbuf)), len[num]);

        memset(&settings, 0, sizeof(settings));
        settings.on_header_field = &on_header_field;
        settings.on_header_value = &on_header_value;
        settings.on_part_data = &on_part_data;
        settings.on_part_data_end = &on_part_data_end;

        if (pkbuf->data[0] == '\r' && pkbuf->data[1] == '\n')
            preamble = 2;

        for (i = preamble; i < (pkbuf->len-preamble); i++) {
            if (pkbuf->data[i] == '\r' && pkbuf->data[i+1] == '\n')
                break;
        }

        ogs_assert(i < pkbuf->len);

        boundary = ogs_strndup((char *)pkbuf->data+preamble, i-preamble);
        ogs_assert(boundary);

        parser = multipart_parser_init(boundary, &settings);
        ogs_assert(parser);

        memset(&data, 0, sizeof(data));
        multipart_parser_set_data(parser, &data);
        multipart_parser_execute(parser,
                (char *)pkbuf->data+preamble, pkbuf->len-preamble);

        multipart_parser_free(parser);
        ogs_free(boundary);

        ogs_assert(data.num_of_part <= OGS_SBI_MAX_NUM_OF_PART);

        ABTS_INT_EQUAL(tc, num_of_part[num], data.num_of_part);

        ogs_pkbuf_free(pkbuf);
    }
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
    abts_run_test(suite, sbi_message_test7, NULL);
    abts_run_test(suite, sbi_message_test8, NULL);
    abts_run_test(suite, sbi_message_test9, NULL);
    abts_run_test(suite, sbi_message_test10, NULL);

    return suite;
}
