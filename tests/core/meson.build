# Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>

# This file is part of Open5GS.

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

testunit_core_sources = files('''
    list-test.c
    pool-test.c
    strings-test.c
    time-test.c
    conv-test.c
    log-test.c
    pkbuf-test.c
    memory-test.c
    rbtree-test.c
    timer-test.c
    thread-test.c
    socket-test.c
    queue-test.c
    poll-test.c
    tlv-test.c
    fsm-test.c
    hash-test.c
    uuid-test.c
    abts-main.c
'''.split())

testunit_core_cc_flags = []
if cc.get_id() == 'gcc' or cc.get_id() == 'clang'
    testunit_core_cc_flags += cc.get_supported_arguments([
                    '-Wno-missing-prototypes',
                    '-Wno-missing-declarations',
                    '-Wno-discarded-qualifiers',
                    '-Wno-incompatible-pointer-types',
                    '-Wno-unused-variable',
                    '-Wno-deprecated-declarations'])
endif

testunit_core_exe = executable('core',
    sources : testunit_core_sources,
    c_args : testunit_core_cc_flags,
    dependencies : libcore_dep)

test('core', testunit_core_exe, is_parallel : false, suite: 'unit')
