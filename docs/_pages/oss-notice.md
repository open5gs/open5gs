---
layout: page
title: OSS Notice
subject: OSS Notice
permalink: /oss-notice/
---

This page records attribution and license information for third-party code and assets included in Open5GS and for libraries used by its builds.

If you have any questions or concerns, please contact us at <a href="mailto:acetcom@gmail.com">acetcom@gmail.com</a>.

---

### Libraries

##### OpenSSL
- [https://www.openssl.org/](https://www.openssl.org/)
- Copyright (C) The OpenSSL Project Authors
- OpenSSL 3.0 and later use [Apache-2.0](https://www.apache.org/licenses/LICENSE-2.0). Earlier OpenSSL releases use the [OpenSSL and SSLeay licenses](https://openssl-library.org/source/license/index.html).

##### GnuTLS
- [https://www.gnutls.org/](https://www.gnutls.org/)
- Copyright (C) Free Software Foundation, Inc.
- Required by the freeDiameter and SBI build definitions.
- LICENSE: [LGPL-2.1-or-later](https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html)

##### Libgcrypt
- [https://gnupg.org/software/libgcrypt/](https://gnupg.org/software/libgcrypt/)
- Copyright (C) The GnuPG Project
- Checked as a freeDiameter build dependency. Runtime use depends on the GnuTLS version.
- LICENSE: [LGPL-2.1-or-later](https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html)

##### GNU Libidn (C library)
- [https://www.gnu.org/software/libidn/](https://www.gnu.org/software/libidn/)
- Copyright (C) Simon Josefsson and Free Software Foundation, Inc.
- Used by freeDiameter
- Current C library license: [LGPL-3.0-or-later](https://www.gnu.org/licenses/lgpl-3.0.html) or [GPL-2.0-or-later](https://www.gnu.org/licenses/old-licenses/gpl-2.0.html). See the [upstream license information](https://github.com/libidn/libidn#license) for the library version used.

##### LibYAML
- [https://pyyaml.org/wiki/LibYAML](https://pyyaml.org/wiki/LibYAML)
- Copyright (c) 2017-2020 Ingy döt Net
- Copyright (c) 2006-2016 Kirill Simonov
- License: [MIT](https://opensource.org/licenses/mit-license.php)

##### prometheus-client-c
- [Open5GS build subproject](https://github.com/open5gs/prometheus-client-c/tree/a58ba25bf87a9b1b7c6be4e6f4c62047d620f402)
- Copyright (c) 2019-2020 DigitalOcean Inc. in the library source files.
- License: [Apache-2.0](https://www.apache.org/licenses/LICENSE-2.0). The subproject also carries its own file-level notices.

##### lksctp-tools SCTP library (libsctp)
- [https://lksctp.sourceforge.net/](https://lksctp.sourceforge.net/)
- Copyright 2002 La Monte H.P. Yarroll; Copyright 2002, 2004 IBM Corp.; Copyright 2010, 2013 Red Hat
- LICENSE: [LGPL-2.1-or-later](https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html)

##### libmongoc
- [https://mongoc.org/libmongoc/current/](https://mongoc.org/libmongoc/current/)
- Copyright 2017-present MongoDB, Inc.
- LICENSE: [Apache-2.0](https://www.apache.org/licenses/LICENSE-2.0)

##### libbson
- [http://mongoc.org/libbson/current/](http://mongoc.org/libbson/current/)
- Copyright 2017-present MongoDB, Inc.
- LICENSE: [Apache-2.0](https://www.apache.org/licenses/LICENSE-2.0)

##### freeDiameter
- [http://www.freediameter.net](http://www.freediameter.net)
- Copyright (c) 2008-2019, WIDE Project and NICT
- License: [BSD-3-Clause](https://github.com/open5gs/freeDiameter/blob/539ca5f2ded19413b756544ef8538755e9792877/LICENSE) by default. Individual files and extensions carry additional notices.

##### Nghttp2
- [https://nghttp2.org/](https://nghttp2.org/)
- Copyright (C) 2022 - Tatsuhiro Tsujikawa
- License: [MIT](https://opensource.org/licenses/mit-license.php)

##### libcurl
- [https://curl.se/libcurl/](https://curl.se/libcurl/)
- Copyright (C) 2019 - 2022, Daniel Stenberg, <daniel@haxx.se>, et al.
- License: [curl](https://curl.se/docs/copyright.html)

##### GNU Libmicrohttpd
- [https://www.gnu.org/software/libmicrohttpd/](https://www.gnu.org/software/libmicrohttpd/)
- Copyright (C) Free Software Foundation, Inc.
- LICENSE: [LGPL-2.1-or-later](https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html)

##### libtins
- [http://libtins.github.io/](http://libtins.github.io/)
- Copyright (c) 2012-2017, Matias Fontanini
- License: [BSD-2-Clause](https://opensource.org/licenses/BSD-2-Clause)

##### libtalloc
- [https://talloc.samba.org/talloc/doc/html/](https://talloc.samba.org/talloc/doc/html/)
- Copyright (C) Andrew Tridgell 2004-2005, Stefan Metzmacher 2006
- LICENSE: [LGPL-3.0-or-later](https://www.gnu.org/licenses/lgpl-3.0.html)

##### c-ares
- [https://c-ares.org/](https://c-ares.org/)
- Copyright (c) The c-ares project and its contributors
- LICENSE: [MIT](https://opensource.org/licenses/MIT)

##### usrsctp
- [https://github.com/sctplab/usrsctp](https://github.com/sctplab/usrsctp)
- Copyright (c) 2015, Randall Stewart and Michael Tuexen
- Used by the macOS SCTP build.
- License: [BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause)

---

### Source code included in this repository

##### Apache Portable Runtime (APR)
- Copyright The Apache Software Foundation.
- Modified APR code is included in `lib/core/` and in the Base64 codec in `lib/crypt/ogs-base64.c` and `lib/crypt/ogs-base64.h`.
- License: [Apache-2.0](https://www.apache.org/licenses/LICENSE-2.0).

##### musl
- `ogs_vasprintf_debug()` in `lib/core/ogs-strings.c` is adapted from [musl 1.2.5 `src/stdio/vasprintf.c`](https://git.musl-libc.org/cgit/musl/tree/src/stdio/vasprintf.c?id=0784374d561435f7c787a555aeab8ede699ed298).
- Copyright (c) 2005-2020 Rich Felker, et al.
- The musl-derived portion uses the MIT License, reproduced in the source file. The Open5GS portions retain their file-level notices.

##### aws-c-io (PEM parsing reference)
- `ogs_fbase64_decode()` in `lib/crypt/ogs-base64.c` credits [aws-c-io `source/pem.c`](https://github.com/awslabs/aws-c-io/blob/main/source/pem.c) as a reference for its PEM parsing approach.
- Reference implementation copyright Amazon.com, Inc. or its affiliates.
- The reference implementation uses [Apache-2.0](https://github.com/awslabs/aws-c-io/blob/main/LICENSE).

##### OpenSSL-derived cryptography
- Copyright (c) 2002-2020 The OpenSSL Project Authors.
- Included in the AES, CMAC, SHA and HMAC files under `lib/crypt/`. The OpenSSL-derived portions in these files carry [Apache-2.0](https://www.apache.org/licenses/LICENSE-2.0). File-specific notices also apply.

##### FreeBSD ipfw and portability code
- Included in `lib/ipfw/`. See the file headers for the applicable BSD and file-specific terms, including the NetBSD notice in `humanize_number.c`.
- [FreeBSD ipfw source](https://github.com/freebsd/freebsd-src/tree/main/sbin/ipfw).

##### freeDiameter-derived code and configuration
- `lib/diameter/common/dict.c`: Copyright (c) 2013 Thomas Klausner, [BSD-2-Clause](https://opensource.org/licenses/BSD-2-Clause).
- `lib/diameter/common/libapp_sip.c`: Copyright (c) 2010 Alexandre Westfahl, Teraoka Laboratory (Keio University), and the WIDE Project, [BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause).
- The `configs/freeDiameter/*.conf.in` templates are adapted from the [freeDiameter sample configuration](https://github.com/open5gs/freeDiameter/blob/539ca5f2ded19413b756544ef8538755e9792877/doc/freediameter.conf.sample) under the project’s BSD-3-Clause terms.

##### asn1c
- [asn1c source and license](https://github.com/mouse07410/asn1c/blob/b7926896e87934cb57bd3c53243a91ebd9fcec90/LICENSE)
- Copyright (c) Lev Walkin <vlm@lionet.info> and contributors.
- Runtime source code is included in `lib/asn1c/common/`.
- License: [BSD-2-Clause](https://opensource.org/licenses/BSD-2-Clause)

##### OpenAPI Generator templates
- The repository includes adapted [OpenAPI Generator templates](https://github.com/OpenAPITools/openapi-generator/tree/v7.20.0/modules/openapi-generator/src/main/resources/C-libcurl) under `lib/sbi/support/`.
- The upstream templates use [Apache-2.0](https://www.apache.org/licenses/LICENSE-2.0). The cJSON templates and 3GPP input specifications carry their respective notices.

##### cJSON
- [https://github.com/DaveGamble/cJSON](https://github.com/DaveGamble/cJSON)
- Copyright (c) 2009-2017 Dave Gamble and cJSON contributors
- Included in `lib/third-party/cjson/` and in the cJSON templates under `lib/sbi/support/`.
- License: [MIT](https://github.com/open5gs/open5gs/blob/main/lib/third-party/cjson/cJSON.licence)

##### multipart-parser-c
- [https://github.com/iafonov/multipart-parser-c](https://github.com/iafonov/multipart-parser-c)
- Copyright (c) 2012 Igor Afonov
- Included in lib/sbi/contrib
- License: [MIT](https://opensource.org/licenses/MIT)

##### yuarel
- [https://github.com/jacketizer/libyuarel](https://github.com/jacketizer/libyuarel)
- Copyright (C) 2016,2017 Jack Engqvist Johansson
- Included in lib/sbi/yuarel.c
- License: [MIT](https://opensource.org/licenses/MIT)

##### hostapd/wpa_supplicant (Milenage)
- [https://w1.fi/](https://w1.fi/)
- Copyright (c) 2006-2007 Jouni Malinen <j@w1.fi>
- Included in lib/crypt/milenage.c
- License: [BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause)

##### curve25519-donna
- [https://github.com/agl/curve25519-donna](https://github.com/agl/curve25519-donna)
- Copyright 2008, Google Inc.
- Included in lib/crypt/curve25519-donna.c
- License: [BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause)

##### easy-ecc
- [https://github.com/jestan/easy-ecc](https://github.com/jestan/easy-ecc)
- Copyright (c) 2013, Kenneth MacKay
- Included in lib/crypt/ecc.c
- License: [BSD-2-Clause](https://opensource.org/licenses/BSD-2-Clause)

##### SHA-2 and HMAC (Olivier Gay)
- Copyright (C) 2005, 2007 Olivier Gay <olivier.gay@a3.epfl.ch>
- Included in `lib/crypt/ogs-sha2.c` and `lib/crypt/ogs-sha2-hmac.c`.
- License: [BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause)

##### arc4random (OpenBSD, portable version from Libevent)
- [https://libevent.org/](https://libevent.org/)
- Copyright (c) 1996, David Mazieres; Copyright (c) 2008, Damien Miller
- Portable version: Copyright (c) 2010 Chris Davis, Niels Provos, and Nick Mathewson; Copyright (c) 2010-2012 Niels Provos and Nick Mathewson
- Included in lib/core/arc4random.c
- License: [ISC](https://opensource.org/licenses/ISC) (OpenBSD notice in the file); the Libevent portable version is distributed under the [Libevent license](https://github.com/libevent/libevent/blob/master/LICENSE) (BSD-3-Clause)

##### PostgreSQL (hash table)
- [https://www.postgresql.org/](https://www.postgresql.org/)
- Portions Copyright (c) 1996-2026, PostgreSQL Global Development Group; Portions Copyright (c) 1994, The Regents of the University of California
- Included in lib/core/ogs-hash.c
- License: [PostgreSQL](https://opensource.org/licenses/PostgreSQL)

##### SipHash
- [https://github.com/veorq/SipHash](https://github.com/veorq/SipHash)
- Copyright 2012-2024 JP Aumasson; Copyright (c) 2012-2014 Daniel J. Bernstein
- Included in lib/core/ogs-hash.c
- License: [MIT](https://opensource.org/licenses/MIT)

##### MongoDB and Boost.Chrono
- Time helpers in `lib/core/ogs-time.c` and `lib/core/ogs-time.h` include MongoDB code, Copyright (c) 2013 MongoDB, Inc., under [Apache-2.0](https://www.apache.org/licenses/LICENSE-2.0).
- The files also contain Boost code, Copyright Beman Dawes 2008 and Copyright Vicente J. Botet Escriba 2009-2010, under the [Boost Software License 1.0](https://www.boost.org/LICENSE_1_0.txt).

##### optparse and process.h
- Included in `lib/core/ogs-getopt.c` and `.h`, and in `lib/core/ogs-process.c` and `.h`.
- The files carry the [Unlicense](https://unlicense.org/) public-domain dedication.

##### SNOW 3G
- `lib/crypt/openssl/snow_core.c` contains the SNOW 3G keystream implementation and carries a public-domain dedication and disclaimer.
- `lib/crypt/openssl/snow3g.h` carries the legacy OpenSSL License. Copyright (c) 1998-2002 The OpenSSL Project.
- This product includes software developed by the OpenSSL Project for use in the OpenSSL Toolkit. ([http://www.openssl.org/](https://www.openssl.org/))

##### GmSSL ZUC-128
- Copyright (c) 2014-2024 The GmSSL Project.
- The ZUC-128 core and EIA3 MAC in `lib/crypt/zuc.c` are adapted from [GmSSL source revision `24ae4827`](https://github.com/guanzhi/GmSSL/blob/24ae482701a7b124826c382fffc55c19f76d475d/src/zuc.c).
- License: [Apache-2.0](https://github.com/guanzhi/GmSSL/blob/24ae482701a7b124826c382fffc55c19f76d475d/LICENSE). The file header records the adaptation.

### WebUI assets

##### Bootstrap and Glyphicons Halflings
- `webui/static/css/bootstrap.min.css` includes Bootstrap 3.3.7, Copyright (c) 2011-2016 Twitter, Inc., and normalize.css 3.0.3, Copyright (c) Nicolas Gallagher and Jonathan Neal. Both use the [MIT License](https://github.com/twbs/bootstrap/blob/v3.3.7/LICENSE).
- The CSS also includes print styles from [HTML5 Boilerplate](https://github.com/h5bp/html5-boilerplate/blob/main/LICENSE.txt) under the MIT License.
- `webui/static/fonts/glyphicons-halflings-regular.*` contains the Bootstrap-bundled Glyphicons Halflings font, Copyright (c) 2014 Jan Kovarik. This edition was released under [Bootstrap’s MIT License](https://glyphicons.com/blog/introducing-version-2-0/). See [Glyphicons](https://glyphicons.com/) and the [Bootstrap attribution guidance](https://getbootstrap.com/docs/3.3/components/#glyphicons).

##### NProgress
- `webui/static/css/nprogress.css`, Copyright (c) 2013-2014 Rico Sta. Cruz.
- License: [MIT](https://github.com/rstacruz/nprogress/blob/v0.2.0/License.md).

##### Roboto and Ubuntu fonts
- `webui/static/fonts/Roboto-Regular.ttf`, Copyright (c) 2011 Google Inc., [Apache-2.0](https://www.apache.org/licenses/LICENSE-2.0).
- `webui/static/fonts/Ubuntu-Regular.ttf`, Copyright (c) 2011 Canonical Ltd., [Ubuntu Font Licence 1.0](https://canonical.com/legal/font-licence).

##### React Icons and Material Design icons
- The WebUI uses the [React Icons](https://github.com/react-icons/react-icons) package, whose wrapper code is distributed under the MIT License.
- The WebUI imports Google's [Material Design icons](https://github.com/google/material-design-icons), which are distributed under [Apache-2.0](https://www.apache.org/licenses/LICENSE-2.0).
