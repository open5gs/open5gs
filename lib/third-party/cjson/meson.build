# Copyright (C) 2026 by Sukchan Lee <acetcom@gmail.com>

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

# Vendored third-party library: cJSON (MIT, see cJSON.licence)
#
# Include as: #include "external/cjson/cJSON.h"
# The path is intentionally unique so that the vendored header can
# never be confused with a system-installed <cjson/cJSON.h>.

libogscjson_sources = files('cJSON.c')

# Vendored code is kept byte-identical to upstream where possible, so
# silence warnings we would otherwise have to fix by patching cJSON.c
# (print_number() intentionally compares doubles with ==).
libogscjson_cc_flags = cc.get_supported_arguments('-Wno-float-equal')

# The vendored copy carries Open5GS modifications (ogs_malloc()-based
# allocation hooks), hence the libcore dependency.
libogscjson = library('ogscjson',
    sources : libogscjson_sources,
    version : libogslib_version,
    c_args : libogscjson_cc_flags,
    include_directories : libinc,
    dependencies : libcore_dep,
    install : true)

libogscjson_dep = declare_dependency(
    link_with : libogscjson,
    include_directories : libinc,
    dependencies : libcore_dep)
