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

libs1ap_sources = files('''
    ogs-s1ap.h

    conv.h
    message.h 
    build.h

    conv.c 
    message.c
    build.c
'''.split())

libs1ap_inc = include_directories('.')

libs1ap = library('ogss1ap',
    sources : libs1ap_sources,
    version : libogslib_version,
    c_args : '-DOGS_S1AP_COMPILATION',
    include_directories : [libs1ap_inc, libinc],
    dependencies : libasn1c_s1ap_dep,
    install : true)

libs1ap_dep = declare_dependency(
    link_with : libs1ap,
    include_directories : [libs1ap_inc, libinc],
    dependencies : libasn1c_s1ap_dep)
