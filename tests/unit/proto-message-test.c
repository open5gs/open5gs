/*
 * Copyright (C) 2023 by Sukchan Lee <acetcom@gmail.com>
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

static void proto_message_test1(abts_case *tc, void *data)
{
    char *fqdn = NULL;
    ogs_plmn_id_t plmn_id1, plmn_id2;

    ogs_plmn_id_build(&plmn_id1, 456, 123, 3);
    fqdn = ogs_home_network_domain_from_plmn_id(&plmn_id1);
    ABTS_STR_EQUAL(tc, "5gc.mnc123.mcc456.3gppnetwork.org", fqdn);
    ABTS_INT_EQUAL(tc, 456, ogs_plmn_id_mcc_from_fqdn(fqdn));
    ABTS_INT_EQUAL(tc, 123, ogs_plmn_id_mnc_from_fqdn(fqdn));
    ogs_free(fqdn);

    ogs_plmn_id_build(&plmn_id1, 1, 1, 2);
    fqdn = ogs_home_network_domain_from_plmn_id(&plmn_id1);
    ABTS_STR_EQUAL(tc, "5gc.mnc001.mcc001.3gppnetwork.org", fqdn);
    ABTS_INT_EQUAL(tc, 1, ogs_plmn_id_mcc_from_fqdn(fqdn));
    ABTS_INT_EQUAL(tc, 1, ogs_plmn_id_mnc_from_fqdn(fqdn));
    ogs_free(fqdn);

    ogs_plmn_id_build(&plmn_id1, 310, 14, 3);
    fqdn = ogs_home_network_domain_from_plmn_id(&plmn_id1);
    ABTS_STR_EQUAL(tc, "5gc.mnc014.mcc310.3gppnetwork.org", fqdn);
    ABTS_INT_EQUAL(tc, 310, ogs_plmn_id_mcc_from_fqdn(fqdn));
    ABTS_INT_EQUAL(tc, 14, ogs_plmn_id_mnc_from_fqdn(fqdn));
    ogs_free(fqdn);
}

static void proto_message_test2(abts_case *tc, void *data)
{
    char *home_network_domain = NULL;
    char *full_dnn = NULL;
    char dnn_ni[OGS_MAX_DNN_LEN+1];
    ogs_plmn_id_t plmn_id1, plmn_id2;

    ogs_plmn_id_build(&plmn_id1, 456, 123, 3);
    home_network_domain = ogs_home_network_domain_from_plmn_id(&plmn_id1);
    ABTS_STR_EQUAL(tc,
            "5gc.mnc123.mcc456.3gppnetwork.org", home_network_domain);
    full_dnn = ogs_msprintf("internet.realm.%s", home_network_domain);
    ABTS_STR_EQUAL(tc,
            "internet.realm.5gc.mnc123.mcc456.3gppnetwork.org", full_dnn);
    ABTS_STR_EQUAL(tc,
            home_network_domain, ogs_home_network_domain_from_fqdn(full_dnn));

    ogs_cpystrn(dnn_ni, full_dnn,
            ogs_min(OGS_MAX_DNN_LEN,
                ogs_home_network_domain_from_fqdn(full_dnn) - full_dnn));
    ABTS_STR_EQUAL(tc, "internet.realm", dnn_ni);

    ABTS_INT_EQUAL(tc, 456, ogs_plmn_id_mcc_from_fqdn(full_dnn));
    ABTS_INT_EQUAL(tc, 123, ogs_plmn_id_mnc_from_fqdn(full_dnn));
    ogs_free(home_network_domain);
    ogs_free(full_dnn);
}

abts_suite *test_proto_message(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, proto_message_test1, NULL);
    abts_run_test(suite, proto_message_test2, NULL);

    return suite;
}
