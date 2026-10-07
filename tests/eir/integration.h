/* Copyright (C) 2026 by Sukchan Lee <acetcom@gmail.com>
 * SPDX-License-Identifier: AGPL-3.0-or-later */

#ifndef TEST_EIR_INTEGRATION_H
#define TEST_EIR_INTEGRATION_H

#include "test-app.h"

abts_suite *test_eir_dbi(abts_suite *suite);
abts_suite *test_eir_service(abts_suite *suite);
abts_suite *test_eir_registration(abts_suite *suite);
abts_suite *test_eir_attach(abts_suite *suite);
abts_suite *test_guti_epc(abts_suite *suite);
abts_suite *test_guti_amf(abts_suite *suite);

#endif /* TEST_EIR_INTEGRATION_H */
