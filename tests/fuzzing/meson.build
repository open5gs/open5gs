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


# Get the lib_fuzzing_engine build option.
lib_fuzzing_engine = get_option('lib_fuzzing_engine')

# All fuzzer sources.
gtp_message_source = files('gtp-message-fuzz.c')
nas_message_source = files('nas-message-fuzz.c')
ngap_message_source = files('ngap-message-fuzz.c')
s1ap_message_source = files('s1ap-message-fuzz.c')
pfcp_message_source = files('pfcp-message-fuzz.c')
nas_5gs_message_source = files('nas-5gs-message-fuzz.c')

# Build all executable
executable(
    'gtp_message_fuzz',
    sources : gtp_message_source,
    c_args : [testunit_core_cc_flags, sbi_cc_flags],
    dependencies : [libgtp_dep],
    link_args: lib_fuzzing_engine
)

executable(
    'nas_message_fuzz',
    sources : nas_message_source,
    c_args : [testunit_core_cc_flags, sbi_cc_flags],
    dependencies : [libnas_eps_dep],
    link_args: lib_fuzzing_engine
)

executable(
    'ngap_message_fuzz',
    sources : ngap_message_source,
    c_args : [testunit_core_cc_flags, sbi_cc_flags],
    dependencies : [libngap_dep, libcore_dep],
    link_args: lib_fuzzing_engine
)

executable(
    's1ap_message_fuzz',
    sources : s1ap_message_source,
    c_args : [testunit_core_cc_flags, sbi_cc_flags],
    dependencies : [libs1ap_dep, libcore_dep],
    link_args: lib_fuzzing_engine
)

executable(
    'pfcp_message_fuzz',
    sources : pfcp_message_source,
    c_args : [testunit_core_cc_flags, sbi_cc_flags],
    dependencies : [libpfcp_dep, libcore_dep],
    link_args: lib_fuzzing_engine
)

executable(
    'nas_5gs_message_fuzz',
    sources : nas_5gs_message_source,
    c_args : [testunit_core_cc_flags, sbi_cc_flags],
    dependencies : [libnas_5gs_dep, libcore_dep],
    link_args: lib_fuzzing_engine
)
