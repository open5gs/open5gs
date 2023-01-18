# Copyright (C) 2023 by Ryan Dimsey <ryan@omnitouch.com.au>

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

testapp_mme_sources = files('''
    abts-main.c
    mme-s13-handler-test.c
'''.split())

testapp_mme_exe = executable('mme',
    sources : testapp_mme_sources,
    c_args : testunit_core_cc_flags,
    dependencies : [libtestapp_dep, libmme_dep])

# Disabled as logs cause failures
# test('mme', testapp_mme_exe, is_parallel : false, suite: 'app')
