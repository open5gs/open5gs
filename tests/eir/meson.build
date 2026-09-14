# Copyright (C) 2026 by Sukchan Lee <acetcom@gmail.com>
# SPDX-License-Identifier: AGPL-3.0-or-later

test_eir_integration_exe = executable('eir-integration',
    sources : files('''
        integration-main.c
        dbi-test.c
        service-test.c
        registration-test.c
    '''.split()),
    c_args : testunit_core_cc_flags,
    dependencies : [libtest5gc_dep, libcurl_dep])

test('eir-integration', test_eir_integration_exe,
    is_parallel : false,
    timeout : 180,
    suite : ['eir', '5gc'])
