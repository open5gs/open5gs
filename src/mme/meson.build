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

libmme_sources = files('''
    mme-event.h
    mme-timer.h
    mme-context.h
    s1ap-build.h
    s1ap-handler.h
    s1ap-path.h 
    sgsap-build.h
    sgsap-handler.h
    sgsap-conv.h
    sgsap-path.h 
    mme-fd-path.h
    mme-s6a-handler.h 
    nas-security.h
    nas-path.h 
    emm-handler.h
    emm-build.h 
    esm-handler.h
    esm-build.h 
    mme-gtp-path.h
    mme-s11-build.h
    mme-s11-handler.h 
    sbc-message.h
    sbc-handler.h
    mme-sm.h
    mme-path.h 

    mme-init.c
    mme-event.c
    mme-timer.c
    mme-context.c 
    s1ap-sm.c
    s1ap-build.c
    s1ap-handler.c
    s1ap-sctp.c
    s1ap-path.c 
    sgsap-sm.c
    sgsap-build.c
    sgsap-handler.c
    sgsap-conv.c
    sgsap-sctp.c
    sgsap-path.c
    mme-fd-path.c
    mme-s6a-handler.c 
    nas-security.c
    nas-path.c
    emm-sm.c
    emm-handler.c
    emm-build.c
    esm-sm.c
    esm-handler.c
    esm-build.c
    mme-gtp-path.c
    mme-s11-build.c
    mme-s11-handler.c
    mme-sm.c
    mme-path.c 
    sbc-handler.c 
'''.split())

libmme = static_library('mme',
    sources : libmme_sources,
    dependencies : [libapp_dep,
                    libsctp_dep,
                    libs1ap_dep,
                    libnas_eps_dep,
                    libdiameter_s6a_dep,
                    libgtp_dep],
    install : false)

libmme_dep = declare_dependency(
    link_with : libmme,
    dependencies : [libapp_dep,
                    libsctp_dep,
                    libs1ap_dep,
                    libnas_eps_dep,
                    libdiameter_s6a_dep,
                    libgtp_dep])

mme_sources = files('''
    app-init.c
    ../main.c
'''.split())

executable('open5gs-mmed',
    sources : mme_sources,
    c_args : '-DDEFAULT_CONFIG_FILENAME="@0@/mme.yaml"'.format(open5gs_sysconfdir),
    include_directories : srcinc,
    dependencies : libmme_dep,
    install_rpath : libdir,
    install : true)
