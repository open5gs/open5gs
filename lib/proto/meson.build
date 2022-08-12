# Copyright (C) 2019-2022 by Sukchan Lee <acetcom@gmail.com>

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

libproto_sources = files('''
    ogs-proto.h

    types.h
    conv.h
    event.h
    timer.h

    types.c
    conv.c
    event.c
    timer.c
'''.split())

libproto_inc = include_directories('.')

libproto = library('ogsproto',
    sources : libproto_sources,
    version : libogslib_version,
    c_args : '-DOGS_PROTO_COMPILATION',
    include_directories : [libproto_inc, libinc],
    dependencies : libcore_dep,
    install : true)

libproto_dep = declare_dependency(
    link_with : libproto,
    include_directories : [libproto_inc, libinc],
    dependencies : libcore_dep)
