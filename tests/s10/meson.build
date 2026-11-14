# SPDX-License-Identifier: AGPL-3.0-or-later
s10_exe = executable('s10',
    sources: files('abts-main.c', 'mobility-test.c', 'connected-test.c'),
    c_args: testunit_core_cc_flags,
    dependencies: libtestepc_dep)
test('s10', s10_exe, is_parallel: false, timeout: 120, suite: ['s10', 'epc'])

s10_sgw_exe = executable('s10-sgw',
    sources: files('abts-main.c', 'mobility-test.c', 'connected-test.c'),
    c_args: [testunit_core_cc_flags, '-DTEST_S10_SGW_RELOCATION'],
    dependencies: libtestepc_dep)
test('s10-sgw', s10_sgw_exe, is_parallel: false, timeout: 120, suite: ['s10', 'epc'])

foreach mode : ['failure', 'timeout']
  failure_args = [testunit_core_cc_flags, '-DTEST_S10_SGW_RELOCATION', '-DTEST_S10_SGW_FAILURE']
  if mode == 'timeout'
    failure_args += '-DTEST_S10_SGW_TIMEOUT'
  endif
  failure_exe = executable('s10-sgw-' + mode,
      sources: files('abts-main.c', 'mobility-test.c', 'connected-test.c'),
      c_args: failure_args, dependencies: libtestepc_dep)
  test('s10-sgw-' + mode, failure_exe, is_parallel: false, timeout: 120, suite: ['s10', 'epc'])
endforeach
