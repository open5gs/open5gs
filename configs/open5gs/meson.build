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

open5gs_sysconfdir = join_paths(sysconfdir, 'open5gs')
meson.add_install_script(python3_exe, '-c',
        mkdir_p.format(open5gs_sysconfdir))

open5gs_conf = '''
    mme.yaml
    sgwc.yaml
    sgwu.yaml
    smf.yaml
    amf.yaml
    upf.yaml
    hss.yaml
    pcrf.yaml
    nrf.yaml
    ausf.yaml
    udm.yaml
    udr.yaml
    pcf.yaml
'''.split()

foreach file : open5gs_conf
    gen = configure_file(
            input : file + '.in',
            output : file,
            configuration : conf_data)
    meson.add_install_script(python3_exe, '-c',
            install_conf.format(gen, open5gs_sysconfdir))
endforeach
