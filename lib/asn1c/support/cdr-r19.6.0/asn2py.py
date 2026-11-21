#!/usr/bin/env python3
#
# Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
#
# This file is part of Open5GS.
#
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
"""
Turn the TS 32.298 ASN.1 module into a schema table for the CDR decoder.

Run at build time, so the decoder's idea of the record comes from the same
file asn1c generates the C encoder from.  Moving to a newer release means
re-running generator.sh; the decoder follows without being edited.

The table it emits is deliberately small - just enough to walk a BER
encoding and name what it finds:

    {"TypeName": {"kind": "SET"|"SEQUENCE"|"CHOICE",
                  "members": [[tag, "fieldName", "TypeName", is_seq_of], ...]},
     "TypeName": {"kind": "INTEGER"|"ENUMERATED", "values": {n: "name"}},
     "TypeName": {"kind": "OCTET STRING"|"IA5String"|"UTF8String"}}

Usage:
    asn2py.py --asn modified/<module>.asn --template <in> --out <out>

The template is copied through with the line containing the marker
"@@SCHEMA@@" replaced by the generated table.
"""
import argparse
import re
import sys

MARKER = "@@SCHEMA@@"

# "fieldName   [12] TypeName OPTIONAL," with any spacing, and the type
# possibly being "SEQUENCE OF Thing".
MEMBER_RE = re.compile(
    r"^\s*([a-zA-Z][\w-]*)\s*"          # field name
    r"\[(\d+)\]\s*"                     # [tag]
    r"(SEQUENCE\s+OF\s+)?"              # SEQUENCE OF?
    r"([A-Za-z][\w-]*(?:\s+STRING)?)"   # type (incl. "OCTET STRING")
    r"\s*(OPTIONAL)?\s*,?\s*$"
)

# "name  (12)," inside an INTEGER or ENUMERATED value list.
VALUE_RE = re.compile(r"^\s*([a-zA-Z][\w-]*)\s*\((-?\d+)\)\s*,?\s*$")

DEF_RE = re.compile(r"^([A-Za-z][\w-]*)\s*::=\s*(.*)$")

STRING_KINDS = ("OCTET STRING", "IA5String", "UTF8String", "GraphicString",
                "PrintableString", "VisibleString", "NumericString")


def strip_comment(line):
    """Drop an ASN.1 comment. A second '--' closes one, so honour that."""
    i = line.find("--")
    if i < 0:
        return line
    rest = line[i + 2:]
    j = rest.find("--")
    if j >= 0:
        return line[:i] + rest[j + 2:]
    return line[:i]


def parse(path):
    with open(path, encoding="utf-8", errors="replace") as f:
        raw = f.read().split("\n")

    lines = [strip_comment(ln) for ln in raw]

    types = {}
    i = 0
    while i < len(lines):
        m = DEF_RE.match(lines[i])
        if not m:
            i += 1
            continue

        name, tail = m.group(1), m.group(2).strip()
        i += 1

        # Constructed: the body runs to the closing brace at column 0, or
        # to a lone "}".
        if tail.startswith(("SET", "SEQUENCE", "CHOICE")) and "OF" not in tail:
            kind = tail.split()[0]
            members = []
            while i < len(lines):
                ln = lines[i]
                i += 1
                if ln.strip().startswith("}"):
                    break
                mm = MEMBER_RE.match(ln)
                if mm:
                    members.append([
                        int(mm.group(2)),
                        mm.group(1),
                        re.sub(r"\s+", " ", mm.group(4)).strip(),
                        bool(mm.group(3)),
                    ])
            types[name] = {"kind": kind, "members": members}
            continue

        if tail.startswith(("INTEGER", "ENUMERATED")):
            kind = "ENUMERATED" if tail.startswith("ENUMERATED") else "INTEGER"
            values = {}

            # The named-value list may open on this line, or after blank
            # and comment lines - TS 32.298 puts a paragraph of commentary
            # between "RecordType ::= INTEGER" and its "{".  Scan forward
            # for the brace, but stop at the next definition so a type
            # with no names does not adopt the following type's list.
            opened = tail.rstrip().endswith("{")
            if not opened:
                j = i
                while j < len(lines):
                    s = lines[j].strip()
                    if not s:
                        j += 1
                        continue
                    if s == "{":
                        opened = True
                        i = j + 1
                    break

            if opened:
                while i < len(lines):
                    ln = lines[i]
                    i += 1
                    if ln.strip().startswith("}"):
                        break
                    vm = VALUE_RE.match(ln)
                    if vm:
                        values[int(vm.group(2))] = vm.group(1)

            types[name] = {"kind": kind}
            if values:
                types[name]["values"] = values
            continue

        for sk in STRING_KINDS:
            if tail.startswith(sk):
                types[name] = {"kind": sk}
                break
        else:
            # A type we do not model: record the kind so the decoder can
            # say so rather than guess.
            if tail:
                types[name] = {"kind": tail.split()[0]}

    return types


def emit(types):
    out = ["SCHEMA = {"]
    for name in sorted(types):
        t = types[name]
        if "members" in t:
            out.append("    %r: {" % name)
            out.append("        'kind': %r," % t["kind"])
            out.append("        'members': [")
            for tag, field, typ, seqof in t["members"]:
                out.append("            (%d, %r, %r, %s)," %
                           (tag, field, typ, seqof))
            out.append("        ],")
            out.append("    },")
        elif "values" in t:
            out.append("    %r: {'kind': %r, 'values': {%s}}," % (
                name, t["kind"],
                ", ".join("%d: %r" % (k, v)
                          for k, v in sorted(t["values"].items()))))
        else:
            out.append("    %r: {'kind': %r}," % (name, t["kind"]))
    out.append("}")
    return "\n".join(out)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--asn", required=True)
    ap.add_argument("--template", required=True)
    ap.add_argument("--out", required=True)
    args = ap.parse_args()

    types = parse(args.asn)
    if "CHFRecord" not in types or "ChargingRecord" not in types:
        sys.stderr.write(
            "asn2py: %s has no CHFRecord/ChargingRecord - wrong module?\n"
            % args.asn)
        return 1

    table = emit(types)

    with open(args.template, encoding="utf-8") as f:
        template = f.read()

    if MARKER not in template:
        sys.stderr.write("asn2py: %s has no %s marker\n"
                         % (args.template, MARKER))
        return 1

    # Replace the whole line holding the marker.
    lines = template.split("\n")
    for n, ln in enumerate(lines):
        if MARKER in ln:
            lines[n] = table
            break

    with open(args.out, "w", encoding="utf-8") as f:
        f.write("\n".join(lines))

    sys.stderr.write("asn2py: %d types -> %s\n" % (len(types), args.out))
    return 0


if __name__ == "__main__":
    sys.exit(main())
