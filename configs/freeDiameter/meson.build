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

freediameter_sysconfdir = join_paths(sysconfdir, 'freeDiameter')
meson.add_install_script(python3_exe, '-c',
        mkdir_p.format(freediameter_sysconfdir))

freediameter_conf = '''
    mme.conf
    hss.conf
    smf.conf
    pcrf.conf
'''.split()

foreach file : freediameter_conf
    gen = configure_file(
            input : file + '.in',
            output : file,
            configuration : conf_data)
    meson.add_install_script(python3_exe, '-c',
            install_conf.format(gen, freediameter_sysconfdir))
endforeach

freediameter_pem = '''
    cacert.pem
    mme.cert.pem
    mme.key.pem
    hss.cert.pem
    hss.key.pem
    smf.cert.pem
    smf.key.pem
    pcrf.cert.pem
    pcrf.key.pem
'''.split()

foreach file : freediameter_pem
    gen = configure_file(
            input : file,
            output : file,
            configuration : conf_data)
    meson.add_install_script(python3_exe, '-c',
            install_conf.format(gen, freediameter_sysconfdir))
endforeach
