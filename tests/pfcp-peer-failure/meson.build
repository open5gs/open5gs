# Copyright (C) 2026 by Daniel Brune <daniel.brune.so@outlook.com>

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


testapp_pfcp_peer_failure_sources = files('''
    abts-main.c
    pfcp-peer-failure-test.c
'''.split())

testapp_pfcp_peer_failure_exe = executable('pfcp-peer-failure',
    sources : testapp_pfcp_peer_failure_sources,
    c_args : testunit_core_cc_flags,
    dependencies : libtest5gc_dep)

test('pfcp-peer-failure',
    testapp_pfcp_peer_failure_exe,
    is_parallel : false,
    timeout : 600,
    suite: ['pfcp-peer-failure', '5gc'])
