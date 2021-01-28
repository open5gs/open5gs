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

testapp_handover_sources = files('''
    abts-main.c
    epc-x2-test.c
    epc-s1-test.c
    5gc-xn-test.c
    5gc-n2-test.c
'''.split())

testapp_handover_exe = executable('handover',
    sources : testapp_handover_sources,
    c_args : testunit_core_cc_flags,
    dependencies : libtestapp_dep)

test('handover', testapp_handover_exe, is_parallel : false, suite: 'app')
