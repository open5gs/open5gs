# Copyright (C) 2026 by Sukchan Lee <acetcom@gmail.com>
# SPDX-License-Identifier: AGPL-3.0-or-later

testapp_eir_exe = executable('eir',
    sources : files('''
        integration-main.c
        dbi-test.c
        service-test.c
        registration-test.c
        attach-test.c
    '''.split()),
    c_args : testunit_core_cc_flags,
    dependencies : [libtestapp_dep, libcurl_dep])

test('eir', testapp_eir_exe,
    is_parallel : false,
    timeout : 180,
    suite : ['eir', 'app'])
