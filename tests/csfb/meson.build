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

testapp_csfb_sources = files('''
    abts-main.c
    mo-idle-test.c
    mt-idle-test.c
    mo-active-test.c
    mt-active-test.c
    mo-sms-test.c
    mt-sms-test.c
    crash-test.c
'''.split())

testapp_csfb_exe = executable('csfb',
    sources : testapp_csfb_sources,
    c_args : testunit_core_cc_flags,
    dependencies : libtestepc_dep)

test('csfb', testapp_csfb_exe, is_parallel : false, suite: 'epc')
