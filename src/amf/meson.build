# Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>

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

libamf_sources = files('''
    context.c
    event.c
    timer.c

    nausf-build.c
    nausf-handler.c

    nudm-build.c
    nudm-handler.c

    nsmf-build.c
    nsmf-handler.c

    npcf-build.c
    npcf-handler.c

    nnrf-handler.c
    namf-handler.c
    sbi-path.c

    nf-sm.c

    ngap-sctp.c
    ngap-build.c
    ngap-handler.c
    ngap-path.c
    ngap-sm.c

    nas-security.c

    gmm-build.c
    gmm-handler.c
    nas-path.c
    gmm-sm.c

    amf-sm.c

    init.c
'''.split())

libamf = static_library('amf',
    sources : libamf_sources,
    link_with : libipfw,
    dependencies : [libapp_dep,
                    libsctp_dep,
                    libngap_dep,
                    libnas_5gs_dep,
                    libsbi_dep],
    install : false)

libamf_dep = declare_dependency(
    link_with : libamf,
    dependencies : [libapp_dep,
                    libsctp_dep,
                    libngap_dep,
                    libnas_5gs_dep,
                    libsbi_dep])

amf_sources = files('''
    app.c
    ../main.c
'''.split())

executable('open5gs-amfd',
    sources : amf_sources,
    c_args : '-DDEFAULT_CONFIG_FILENAME="@0@/amf.yaml"'.format(open5gs_sysconfdir),
    include_directories : srcinc,
    dependencies : libamf_dep,
    install_rpath : libdir,
    install : true)
