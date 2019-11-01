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

fd_ext_sources = ('''
    dbg_msg_dumps
    dict_rfc5777
    dict_mip6i 
    dict_nasreq
    dict_nas_mipv6 
    dict_dcca
    dict_dcca_3gpp
    dict_s6a 
'''.split())

foreach s : fd_ext_sources
    shared_module(s,
            sources : [files(join_paths(s, s+'.c'))],
            include_directories : freeDiameter_includes,
            dependencies : [libfdcore_dep],
            name_prefix : '',
            name_suffix : 'fdx',
            install : true,
            install_dir : freeDiameter_extensionsdir)
endforeach
