# Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>

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

libchf_sources = files('''
    context.c
    event.c

    db-handler.c
    cdr-record.c
    cdr-json.c
    cdr-path.c
    notify-path.c

    metrics.c

    nnrf-handler.c
    nchf-handler.c

    sbi-path.c
    chf-sm.c

    init.c
'''.split())

libchf = static_library('chf',
    sources : libchf_sources,
    dependencies : [libdbi_dep,
                    libmetrics_dep,
                    libsbi_dep,
                    libasn1c_cdr_dep],
    install : false)

libchf_dep = declare_dependency(
    link_with : libchf,
    dependencies : [libdbi_dep,
                    libmetrics_dep,
                    libsbi_dep,
                    libasn1c_cdr_dep])

# ---------------------------------------------------------------------
# open5gs-chf-cdr-decode
#
# The CDR decoder is generated at build time from the same ASN.1 module
# that asn1c generates the CHF's encoder from, so it cannot drift from
# what the CHF writes.  asn2py.py turns the module into a schema table and
# substitutes it into cdr-decode.py.in, giving one self-contained script
# that needs only the Python standard library - it can be copied to a
# billing host on its own.
#
# install_mode needs meson >= 0.47 (the project still declares 0.43, which
# predates much of what it already relies on); without it the script would
# install unreadable as an executable.
# ---------------------------------------------------------------------
chf_cdr_asn2py = files('../../lib/asn1c/support/cdr-r19.6.0/asn2py.py')
chf_cdr_asn = files(
    '../../lib/asn1c/support/cdr-r19.6.0/modified/32298-j60-chf-5gsa.asn')

chf_cdr_decode_tgt = custom_target('open5gs-chf-cdr-decode',
    input : files('cdr-decode.py.in'),
    output : 'open5gs-chf-cdr-decode',
    command : [python3_exe, chf_cdr_asn2py,
               '--asn', chf_cdr_asn,
               '--template', '@INPUT@',
               '--out', '@OUTPUT@'],
    depend_files : [chf_cdr_asn2py, chf_cdr_asn],
    build_by_default : true,
    install : true,
    install_dir : get_option('bindir'),
    install_mode : 'rwxr-xr-x')

chf_sources = files('''
    app.c
    ../main.c
'''.split())

executable('open5gs-chfd',
    sources : chf_sources,
    c_args : '-DDEFAULT_CONFIG_FILENAME="@0@/chf.yaml"'.format(open5gs_sysconfdir),
    include_directories : srcinc,
    dependencies : libchf_dep,
    install_rpath : libdir,
    install : true)
