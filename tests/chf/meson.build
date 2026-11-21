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

# ---------------------------------------------------------------------------
# CHF unit tests
#
# They run without a NRF, a MongoDB or a live CHF: everything here is
# either arithmetic or a file the test writes itself.  What they cover:
#
#   chf-quota-test.c    — quota math: the cycle counter, exhaustion
#                          boundaries, session independence, and the
#                          cross-session aggregate (TS 32.291 §5.2)
#   chf-cdr-test.c      — the CDR writer: path layout, JSON fields, the
#                          slice, cause strings, the disabled-flag guard,
#                          which sessions are worth a record at all, the
#                          json/asn1/both switch, and aggregated filing
#                          with its record and size thresholds
#   chf-cdr-asn1-test.c — the TS 32.298 record itself: which fields a
#                          session becomes, the [200] tag the encoding
#                          opens with, a BER round-trip through the same
#                          codec a mediation system would use, volumes
#                          past 2^32, all three closing causes, the
#                          SliceDifferentiator octets and the BCD TimeStamp
#   chf-metrics-test.c  — the metric spec tables: that every enum entry
#                          has a complete spec, and that instrumenting
#                          before the metrics context exists is a no-op,
#                          which every other suite here relies on.  METR-5
#                          is the exception: it starts a real context so
#                          that chf_metrics_final() is exercised at all,
#                          and leaves none running for the suites after it
#   chf-sbi-test.c      — ogs_sbi_check_chf_info_supi_range(): which CHF
#                          instance serves a SUPI (TS 29.510 §6.1.6.2.9)
#   chf-grant-test.c    — grant shaping from chf.yaml charging.grant: the
#                          slice size, the validityTime cap and the
#                          volumeQuotaThreshold floor, and what an
#                          allowance costs in Updates
#   chf-bucket-test.c   — which allowance a session is charged to: scope
#                          matching over slice, DNN, home PLMN and roaming,
#                          and selection by priority
#
# All seven register into one binary, so a failure names the test that
# failed rather than the file.
#
# Run:
#   meson test -C build --suite chf -v
#   build/tests/chf/chf-unit           # direct invocation
# ---------------------------------------------------------------------------

testchf_sources = files('''
    abts-main.c
    chf-quota-test.c
    chf-cdr-test.c
    chf-cdr-asn1-test.c
    chf-metrics-test.c
    chf-sbi-test.c
    chf-grant-test.c
    chf-bucket-test.c
'''.split())

testchf_exe = executable('chf-unit',
    sources : testchf_sources,
    c_args : testunit_core_cc_flags,
    include_directories : srcinc,
    dependencies : libchf_dep,
    install : false)

test('chf-unit',
    testchf_exe,
    is_parallel : false,
    suite : 'chf')

# ---------------------------------------------------------------------------
# JSON mirrors BER
#
# The C tests can only compare the CDR writer against itself.  This one
# compares its two encodings through something that shares no code with
# either: chf-cdr-emit writes both for the same sessions, and
# open5gs-chf-cdr-decode - generated from the TS 32.298 ASN.1 module,
# not from the CHF's C - decodes the BER back.  The two JSON documents
# then have to be equal, which is what makes "the JSON mirrors the
# ASN.1" a fact rather than an intention.
#
# It needs only python3, which the build already requires.
#
# `depends` needs meson >= 0.46, which like the install_mode on the decoder
# target is above the 0.43 the project still declares; meson warns and
# proceeds.  See the comment on that target in src/chf/meson.build.
# ---------------------------------------------------------------------------
testchf_emit_exe = executable('chf-cdr-emit',
    sources : files('chf-cdr-emit.c'),
    c_args : testunit_core_cc_flags,
    include_directories : srcinc,
    dependencies : libchf_dep,
    install : false)

test('chf-cdr-decode',
    find_program('test-cdr-decode.sh'),
    args : [testchf_emit_exe, chf_cdr_decode_tgt, python3_exe],
    depends : [testchf_emit_exe, chf_cdr_decode_tgt],
    is_parallel : false,
    suite : 'chf')

# ---------------------------------------------------------------------------
# CHF API test over the wire
#
# tests/chf/test-chf-api.sh drives a running CHF with curl over cleartext
# HTTP/2, in the request shape of TS 32.291.  It needs open5gs-chfd and
# MongoDB up, so it is deliberately NOT registered as a meson test: a
# plain `meson test` on a build machine has neither.
#
# Run manually against a running CHF:
#   CHF_PORT=7777 tests/chf/test-chf-api.sh
# ---------------------------------------------------------------------------
