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

testapp_non3gpp_sources = files('''
    test-fd-path.h
    test-fd-path.c
    diameter-swx-path.c
    diameter-s6b-path.c
    gtp-path.c

    s2b-build.c
    s2b-handler.c

    abts-main.c
    epdg-test.c
'''.split())

testapp_non3gpp_exe = executable('non3gpp',
    sources : testapp_non3gpp_sources,
    c_args : [testunit_core_cc_flags,
        '-DFD_EXT_DIR="@0@"'.format(build_subprojects_freeDiameter_extensions_dir)],
    dependencies : libtestepc_dep)

test('non3gpp', testapp_non3gpp_exe, is_parallel : false, suite: 'epc')
