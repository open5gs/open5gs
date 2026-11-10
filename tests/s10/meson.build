# SPDX-License-Identifier: AGPL-3.0-or-later
s10_exe = executable('s10',
    sources: files('abts-main.c', 'mobility-test.c', 'connected-test.c'),
    c_args: testunit_core_cc_flags,
    dependencies: libtestepc_dep)
test('s10', s10_exe, is_parallel: false, suite: ['s10', 'epc'])
