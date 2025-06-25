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

libpwsiws_sources = files('''
    context.h
    context.c
    pwsiws-event.h
    pwsiws-event.c
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

libpwsiws = static_library('pwsiws',
    sources : libpwsiws_sources,
    dependencies : [libsctp_dep,
                    libngap_dep,
                    libnas_5gs_dep,
                    libsbi_dep,
                    libasn1c_sbcap_dep],
    include_directories : [asn1c_sbcap_inc],
    install : false)

libpwsiws_dep = declare_dependency(
    link_with : libpwsiws,
    dependencies : [libsctp_dep,
                    libngap_dep,
                    libnas_5gs_dep,
                    libsbi_dep,
                    libasn1c_sbcap_dep])

pwsiws_sources = files('''
    app.c
    ../main.c
'''.split())

executable('open5gs-pwsiwsd',
    sources : pwsiws_sources,
    c_args : '-DDEFAULT_CONFIG_FILENAME="@0@/pwsiws.yaml"'.format(open5gs_sysconfdir),
    include_directories : [srcinc, asn1c_sbcap_inc],
    dependencies : libpwsiws_dep,
    install_rpath : libdir,
    install : true) 