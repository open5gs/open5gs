# Copyright (C) 2024 by Sukchan Lee <acetcom@gmail.com>

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

libsmsf_sources = files('''
    context.c
    event.c

    nsmsf-handler.c

    nnrf-handler.c

    namf-handler.c
    namf-build.c

    nudm-handler.c
    nudm-build.c
    ue-sm.c

    sms.c
    sbi-path.c
    smsf-sm.c

    init.c
'''.split())

libsmsf = static_library('smsf',
    sources : libsmsf_sources,
    dependencies : libsbi_dep,
    install : false)

libsmsf_dep = declare_dependency(
    link_with : libsmsf,
    dependencies : libsbi_dep)

smsf_sources = files('''
    app.c
    ../main.c
'''.split())

executable('open5gs-smsfd',
    sources : smsf_sources,
    c_args : '-DDEFAULT_CONFIG_FILENAME="@0@/smsf.yaml"'.format(open5gs_sysconfdir),
    include_directories : srcinc,
    dependencies : libsmsf_dep,
    install_rpath : libdir,
    install : true)
