# Copyright (C) 2026 by LetMeConnect
# SPDX-License-Identifier: AGPL-3.0-or-later

testapp_s13_sources = files('''
    abts-main.c
    s13-test.c
'''.split())

testapp_s13_exe = executable('s13',
    sources : testapp_s13_sources,
    c_args : testunit_core_cc_flags,
    dependencies : libtestapp_dep)

test('s13', testapp_s13_exe, is_parallel : false, suite: ['s13', 'epc'])
