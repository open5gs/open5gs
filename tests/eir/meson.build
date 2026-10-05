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
    env : ['OPEN5GS_EIR_TEST_MODE=integration', 'OPEN5GS_EIR_TEST_POLICY=reject'],
    args : ['attach-test'],
    is_parallel : false,
    suite : ['eir', 'epc'])

test('eir-4g-allow', testapp_eir_exe,
    env : ['OPEN5GS_EIR_TEST_MODE=integration', 'OPEN5GS_EIR_TEST_POLICY=allow'],
    args : ['attach-test'],
    is_parallel : false,
    suite : ['eir', 'epc'])

test('eir-5g', testapp_eir_exe,
    env : ['OPEN5GS_EIR_TEST_MODE=integration', 'OPEN5GS_EIR_TEST_POLICY=reject'],
    args : ['dbi-test', 'service-test', 'registration-test'],
    is_parallel : false,
    suite : ['eir', '5gc'])

test('eir-5g-allow', testapp_eir_exe,
    env : ['OPEN5GS_EIR_TEST_MODE=integration', 'OPEN5GS_EIR_TEST_POLICY=allow'],
    args : ['registration-test'],
    is_parallel : false,
    suite : ['eir', '5gc'])

test('eir-epc', testapp_eir_exe,
    env : ['OPEN5GS_EIR_TEST_MODE=epc-guti'],
    is_parallel : false,
    suite : ['eir', 'epc'])

test('eir-s13', testapp_eir_exe,
    env : ['OPEN5GS_EIR_TEST_MODE=epc-guti'],
    args : ['-c', join_paths(build_configs_dir, 'eir.yaml')],
    is_parallel : false,
    suite : ['eir', 'epc'])

test('eir-amf', testapp_eir_exe,
    env : ['OPEN5GS_EIR_TEST_MODE=amf-guti'],
    is_parallel : false,
    suite : ['eir', '5gc'])

test('eir-n5g', testapp_eir_exe,
    env : ['OPEN5GS_EIR_TEST_MODE=amf-guti'],
    args : ['-c', join_paths(build_configs_dir, 'eir.yaml')],
    is_parallel : false,
    suite : ['eir', '5gc'])
