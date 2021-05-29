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

systemd_conf_in = '''
    open5gs-mmed.service
    open5gs-sgwcd.service
    open5gs-smfd.service
    open5gs-amfd.service
    open5gs-sgwud.service
    open5gs-upfd.service
    open5gs-hssd.service
    open5gs-pcrfd.service
    open5gs-nrfd.service
    open5gs-ausfd.service
    open5gs-udmd.service
    open5gs-pcfd.service
    open5gs-nssfd.service
    open5gs-bsfd.service
    open5gs-udrd.service
'''.split()

foreach file : systemd_conf_in
    configure_file(
            input : file + '.in',
            output : file,
            configuration : conf_data)
endforeach

systemd_conf = '''
    99-open5gs.netdev
    99-open5gs.network        
'''.split()

foreach file : systemd_conf
    configure_file(
            input : file,
            output : file,
            configuration : conf_data)
endforeach
