# Copyright (C) 2022 by Sukchan Lee <acetcom@gmail.com>

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

tls_sysconfdir = join_paths(open5gs_sysconfdir, 'tls')
meson.add_install_script(python3_exe, '-c',
        mkdir_p.format(tls_sysconfdir))

tls_security = '''
    ca.crt
    amf.key
    ausf.key
    bsf.key
    hss.key
    mme.key
    nrf.key
    nssf.key
    pcf.key
    pcrf.key
    scp.key
    smf.key
    udm.key
    udr.key
    amf.crt
    ausf.crt
    bsf.crt
    hss.crt
    mme.crt
    nrf.crt
    nssf.crt
    pcf.crt
    pcrf.crt
    scp.crt
    smf.crt
    udm.crt
    udr.crt
    sepp1.key
    sepp1.crt
    sepp2.key
    sepp2.crt
    sepp3.key
    sepp3.crt
'''.split()

foreach file : tls_security
    gen = configure_file(
            input : file,
            output : file,
            configuration : conf_data)
    meson.add_install_script(python3_exe, '-c',
            install_conf.format(gen, tls_sysconfdir))
endforeach
