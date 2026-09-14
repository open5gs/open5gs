/* Copyright (C) 2026 by Sukchan Lee <acetcom@gmail.com>
 * SPDX-License-Identifier: AGPL-3.0-or-later */

#ifndef TEST_EIR_INTEGRATION_H
#define TEST_EIR_INTEGRATION_H

#include "test-app.h"

/* Addresses come from the same configuration passed to every child NF. */
const char *test_eir_sbi_uri(void);
const char *test_eir_nrf_uri(void);
bool test_eir_wait_ready(void);

abts_suite *test_eir_dbi(abts_suite *suite);
abts_suite *test_eir_service(abts_suite *suite);
abts_suite *test_eir_registration(abts_suite *suite);

#endif /* TEST_EIR_INTEGRATION_H */
