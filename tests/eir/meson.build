# Copyright (C) 2026 by Sukchan Lee <acetcom@gmail.com>
# SPDX-License-Identifier: AGPL-3.0-or-later

testapp_eir_exe = executable('eir',
    sources : files('''
        integration-main.c
        dbi-test.c
        service-test.c
        registration-test.c
        attach-test.c
        epc-guti-test.c
        amf-guti-test.c
    '''.split()),
    c_args : testunit_core_cc_flags,
    dependencies : [libtestapp_dep, libcurl_dep])

test('eir-4g', testapp_eir_exe,
    args : ['attach-test'],
    is_parallel : false,
    suite : ['eir', 'epc'])

test('eir-5g', testapp_eir_exe,
    args : ['dbi-test', 'service-test', 'registration-test'],
    is_parallel : false,
    suite : ['eir', '5gc'])

test('eir-s13', testapp_eir_exe,
    args : ['epc-guti-test'],
    is_parallel : false,
    suite : ['eir', 'epc'])

test('eir-n5g', testapp_eir_exe,
    args : ['amf-guti-test'],
    is_parallel : false,
    suite : ['eir', '5gc'])
