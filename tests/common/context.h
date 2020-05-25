/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

#if !defined(OGS_TEST_INSIDE) && !defined(OGS_TEST_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef TEST_COMMON_CONTEXT_H
#define TEST_COMMON_CONTEXT_H

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_NUM_OF_SERVED_GUAMI     8

typedef struct test_context_s {
    /* Served GUMME */
    uint8_t num_of_served_guami;
    struct {
        ogs_plmn_id_t plmn_id;
        ogs_amf_id_t amf_id;
    } served_guami[MAX_NUM_OF_SERVED_GUAMI];

    /* Served TAI */
    uint8_t num_of_served_tai;
    struct {
        ogs_5gs_tai0_list_t list0;
        ogs_5gs_tai2_list_t list2;
    } served_tai[OGS_MAX_NUM_OF_SERVED_TAI];

    /* PLMN Support */
    uint8_t num_of_plmn_support;
    struct {
        ogs_plmn_id_t plmn_id;
        int num_of_s_nssai;
        ogs_s_nssai_t s_nssai[OGS_MAX_NUM_OF_S_NSSAI];
    } plmn_support[OGS_MAX_NUM_OF_PLMN];

} test_context_t;

void test_context_init(void);
void test_context_final(void);
test_context_t *test_self(void);

int test_context_parse_config(void);

#ifdef __cplusplus
}
#endif

#endif /* TEST_COMMON_CONTEXT_H */
