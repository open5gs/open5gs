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

libcrypt_sources = files('''
    ogs-crypt.h

    ogs-aes.h
    ogs-aes-cmac.h
    ogs-sha1.h
    ogs-sha1-hmac.h
    ogs-sha2.h
    ogs-sha2-hmac.h
    milenage.h
    snow-3g.h
    zuc.h
    kasumi.h
    ogs-kdf.h
    ecc.h

    ogs-aes.c
    ogs-aes-cmac.c
    ogs-sha1.c
    ogs-sha1-hmac.c
    ogs-sha2.c
    ogs-sha2-hmac.c
    milenage.c
    snow-3g.c
    zuc.c
    kasumi.c

    ogs-kdf.c
    ogs-base64.c

    curve25519-donna.c
    ecc.c

    openssl/snow3g.h
    openssl/snow_core.c
'''.split())

libcrypt_inc = include_directories('.')

libcrypt = library('ogscrypt',
    sources : libcrypt_sources,
    version : libogslib_version,
    c_args : '-DOGS_CRYPT_COMPILATION',
    include_directories : [libcrypt_inc, libinc],
    dependencies : libproto_dep,
    install : true)

libcrypt_dep = declare_dependency(
    link_with : libcrypt,
    include_directories : [libcrypt_inc, libinc],
    dependencies : libproto_dep)
