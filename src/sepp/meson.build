# Copyright (C) 2023 by Sukchan Lee <acetcom@gmail.com>

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

libsepp_sources = files('''
    context.c
    event.c
    timer.c

    n32c-build.c
    n32c-handler.c

    handshake-sm.c

    sbi-path.c
    sepp-sm.c

    init.c
'''.split())

libsepp = static_library('sepp',
    sources : libsepp_sources,
    dependencies : [libcrypt_dep,
                    libsbi_dep],
    install : false)

libsepp_dep = declare_dependency(
    link_with : libsepp,
    dependencies : [libcrypt_dep,
                    libsbi_dep])

sepp_sources = files('''
    app.c
    ../main.c
'''.split())

executable('open5gs-seppd',
    sources : sepp_sources,
    c_args : '-DDEFAULT_CONFIG_FILENAME="@0@/sepp.yaml"'.format(open5gs_sysconfdir),
    include_directories : srcinc,
    dependencies : libsepp_dep,
    install_rpath : libdir,
    install : true)
