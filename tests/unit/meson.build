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

testunit_unit_sources = files('''
    abts-main.c
    proto-message-test.c
    s1ap-message-test.c
    nas-message-test.c
    gtp-message-test.c
    ngap-message-test.c
    sbi-message-test.c
    sbi-server-test.c
    security-test.c
    nrf-discovery-test.c
    nrf-timer-test.c
    nnrf-subscription-test.c
    crash-test.c
    mme-dns-select-test.c
    mme-s13-test.c
    mme-apn-test.c
    mme-attach-test.c
    mme-enb-id-test.c
    amf-eir-test.c
    amf-registration-test.c
    amf-gnb-id-test.c
    amf-supi-test.c
    eir-config-test.c
    ipfw-test.c
    pfcp-rule-test.c
'''.split())

testunit_unit_exe = executable('unit',
    sources : testunit_unit_sources,
    c_args : [testunit_core_cc_flags, sbi_cc_flags],
    include_directories : srcinc,
    dependencies : [libs1ap_dep,
                    libgtp_dep,
                    libngap_dep,
                    libnas_eps_dep,
                    libsbi_dep,
                    libipfw_dep,
                    libpfcp_dep,
                    libnrf_dep,
                    libmme_dep,
                    libamf_dep])

test('unit', testunit_unit_exe, is_parallel : false, suite: 'unit')

# Exercise policy decisions and the real AMF/MME FSMs without peer processes.
test('eir-fsm', testunit_unit_exe,
    args : ['mme-s13-test', 'mme-attach-test',
            'amf-eir-test', 'amf-registration-test', 'eir-config-test'],
    is_parallel : false,
    suite : 'eir')

# AUSF context tests link libausf alone.
testunit_ausf_exe = executable('ausf-unit',
    sources : files('ausf-abts-main.c', 'ausf-context-test.c'),
    c_args : testunit_core_cc_flags,
    include_directories : [srcinc, include_directories('../../src/ausf')],
    dependencies : [libausf_dep])

test('ausf-unit', testunit_ausf_exe, is_parallel : false, suite: 'unit')

# UPF tests link libupf, whose metrics symbols clash with libamf.
testunit_upf_exe = executable('upf-unit',
    sources : files('upf-abts-main.c', 'upf-context-test.c'),
    c_args : testunit_core_cc_flags,
    include_directories : [srcinc, include_directories('../../src/upf')],
    dependencies : [libpfcp_dep, libupf_dep])

test('upf-unit', testunit_upf_exe, is_parallel : false, suite: 'unit')
