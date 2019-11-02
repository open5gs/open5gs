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

libdiameter_rx_sources = files('''
    ogs-diameter-rx.h
    
    message.h

    dict.c
    message.c
'''.split())

libdiameter_rx_inc = include_directories('.')

libdiameter_rx = library('ogsdiameter-rx',
    sources : libdiameter_rx_sources,
    version : libogslib_version,
    c_args : libdiameter_common_cc_flags,
    include_directories : libdiameter_rx_inc,
    dependencies : libdiameter_common_dep,
    install : true)

libdiameter_rx_dep = declare_dependency(
    link_with : libdiameter_rx,
    include_directories : libdiameter_rx_inc,
    dependencies : libdiameter_common_dep)
