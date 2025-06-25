# Copyright (C) 2019-2025 by Sukchan Lee <acetcom@gmail.com>

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

libpwsiwf_sai_sources = files('''
    context.h
    context.c
    pwsiwf_sai-event.h
    pwsiwf_sai-event.c
    sbi-path.h
    sbi-path.c
    ngap-path.h
    ngap-path.c
    ngap-build.h
    ngap-build.c
    namf-build.h
    namf-build.c
    namf-handler.h
    namf-handler.c
    sbcap-path.h
    sbcap-path.c
    sbcap-handler.h
    sbcap-handler.c
    sbc-message.h
    sbc-handler.h
    sbc-handler.c
    init.h
    init.c
'''.split())

asn1c_sbcap_inc = include_directories('../../lib/asn1c/sbcap')

libpwsiwf_sai = static_library('pwsiwf_sai',
    sources : libpwsiwf_sai_sources,
    dependencies : [libsctp_dep,
                    libngap_dep,
                    libnas_5gs_dep,
                    libsbi_dep,
                    libasn1c_sbcap_dep],
    include_directories : [asn1c_sbcap_inc],
    install : false)

libpwsiwf_sai_dep = declare_dependency(
    link_with : libpwsiwf_sai,
    dependencies : [libsctp_dep,
                    libngap_dep,
                    libnas_5gs_dep,
                    libsbi_dep,
                    libasn1c_sbcap_dep])

pwsiwf_sai_sources = files('''
    app.c
    ../main.c
'''.split())

executable('open5gs-pwsiwf_said',
    sources : pwsiwf_sai_sources,
    c_args : '-DDEFAULT_CONFIG_FILENAME="@0@/pwsiwf_sai.yaml"'.format(open5gs_sysconfdir),
    include_directories : [srcinc, asn1c_sbcap_inc],
    dependencies : libpwsiwf_sai_dep,
    install_rpath : libdir,
    install : true) 