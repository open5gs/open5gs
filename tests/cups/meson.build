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

testcups_sources = files('''
    pcscf-fd-path.h
    pcscf-fd-path.c

    abts-main.c
    cups-test.c
'''.split())

testcups_exe = executable('cups',
    sources : testcups_sources,
    c_args : [testcore_cc_flags,
        '-DFD_EXT_DIR="@0@"'.format(freediameter_extensions_builddir)],
    dependencies : libtestapp_dep)

test('cups', testcups_exe, is_parallel : false, suite: '5gc')
