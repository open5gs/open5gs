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

libnas_eps_sources = files('''
    types.c
    conv.c
    ies.c
    decoder.c
    encoder.c
'''.split())

libnas_eps_inc = include_directories('.')

libnas_eps = library('ogsnas-eps',
    sources : libnas_eps_sources,
    version : libogslib_version,
    c_args : '-DOGS_NAS_COMPILATION',
    include_directories : libnas_eps_inc,
    dependencies : libnas_common_dep,
    install : true)

libnas_eps_dep = declare_dependency(
    link_with : libnas_eps,
    include_directories : libnas_eps_inc,
    dependencies : libnas_common_dep)
