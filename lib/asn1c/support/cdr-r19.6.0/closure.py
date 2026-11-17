#!/usr/bin/env python3
"""
Compute the transitive type closure of a set of ASN.1 roots across the
3GPP TS 32.298 modules, and emit one self-contained module.

Why: TS 32.298's CHFChargingDataTypes IMPORTS from seven sibling charging
modules, which in turn reach TS 29.002 MAP, CAMEL CAP and ITU-T
CMIP/ACSE/X.721.  Nothing on the 5G SA PDU-session path needs any of that,
so we take the closure of what the CHF record actually reaches and leave
the rest out.  Every retained definition is copied verbatim, so tags,
types and value ranges stay exactly as 3GPP published them.
"""
import re
import sys

DEF_RE = re.compile(r"^([A-Za-z][A-Za-z0-9-]*)\s*::=")
# A reference is any upper-case-initial identifier. The lookbehind stops a
# hyphenated enum value such as "eND-USER-IMSI" from being read as a
# reference to a type called "USER-IMSI". ASN.1 builtins and keywords are
# filtered out below.
REF_RE = re.compile(r"(?<![A-Za-z0-9-])([A-Z][A-Za-z0-9-]*)\b")

BUILTIN = {
    "SEQUENCE", "SET", "OF", "CHOICE", "ENUMERATED", "INTEGER", "BOOLEAN",
    "OCTET", "STRING", "BIT", "NULL", "OPTIONAL", "DEFAULT", "SIZE", "TRUE",
    "FALSE", "UTF8String", "IA5String", "PrintableString", "VisibleString",
    "NumericString", "GraphicString", "GeneralString", "TeletexString",
    "UniversalString", "BMPString", "UTCTime", "GeneralizedTime",
    "OBJECT", "IDENTIFIER", "ANY", "DEFINED", "BY", "COMPONENTS", "WITH",
    "FROM", "IMPORTS", "EXPORTS", "BEGIN", "END", "DEFINITIONS", "IMPLICIT",
    "EXPLICIT", "AUTOMATIC", "TAGS", "APPLICATION", "UNIVERSAL", "PRIVATE",
    "MIN", "MAX", "ABSENT", "PRESENT", "INCLUDES", "EXCEPT", "ALL",
    "REAL", "EMBEDDED", "PDV", "EXTERNAL", "RELATIVE-OID", "CLASS",
    "CONSTRAINED", "ENCODED", "CHARACTER", "ObjectDescriptor",
}


def strip_comments(text):
    return "\n".join(re.sub(r"--.*$", "", ln) for ln in text.split("\n"))


def parse_module(path):
    """name -> verbatim definition text (comments kept)."""
    with open(path, encoding="utf-8", errors="replace") as f:
        lines = f.read().split("\n")
    # Only the body between BEGIN and the final END holds assignments.
    starts = []
    for i, ln in enumerate(lines):
        m = DEF_RE.match(ln)
        if m:
            starts.append((i, m.group(1)))
    defs = {}
    for idx, (i, name) in enumerate(starts):
        end = starts[idx + 1][0] if idx + 1 < len(starts) else len(lines)
        body = lines[i:end]
        # Trim trailing blank lines and a trailing bare END.
        while body and (body[-1].strip() == "" or body[-1].strip() == "END"):
            body.pop()
        defs[name] = "\n".join(body)
    return defs


def refs_of(text):
    body = strip_comments(text)
    # Drop the "Name ::=" head so a definition doesn't reference itself.
    body = re.sub(r"^[A-Za-z][A-Za-z0-9-]*\s*::=", "", body, count=1)
    out = set()
    for m in REF_RE.finditer(body):
        tok = m.group(1)
        if tok in BUILTIN:
            continue
        out.add(tok)
    return out


def module_header(path):
    """The 3GPP module identifier and DEFINITIONS line, verbatim."""
    out = []
    with open(path, encoding="utf-8", errors="replace") as f:
        for ln in f:
            out.append(ln.rstrip("\n"))
            if ln.strip() == "BEGIN":
                break
            if len(out) > 20:            # header is a handful of lines
                break
    # Everything up to BEGIN, minus any IMPORTS (there are none to keep).
    keep = []
    for ln in out:
        if ln.strip().startswith("IMPORTS"):
            break
        keep.append(ln)
    if not any(l.strip() == "BEGIN" for l in keep):
        keep.append("")
        keep.append("BEGIN")
    return "\n".join(keep)


def main():
    roots = []
    paths = []
    overrides = []
    emit_from = None
    mode = None
    for a in sys.argv[1:]:
        if a == "--root":
            mode = "root"
        elif a == "--override":
            mode = "override"
        elif a == "--module":
            mode = "module"
        elif a == "--emit-module":
            mode = "emit"
        elif mode == "root":
            roots.append(a)
        elif mode == "override":
            overrides.append(a)
        elif mode == "module":
            paths.append(a)
        elif mode == "emit":
            emit_from = a

    defs = {}
    provenance = {}
    for p in paths:
        for k, v in parse_module(p).items():
            if k not in defs:          # first module wins
                defs[k] = v
                provenance[k] = p.split("/")[-1]

    # Overrides replace a 3GPP definition with our trimmed one.
    for p in overrides:
        for k, v in parse_module(p).items():
            defs[k] = v
            provenance[k] = "(trimmed) " + p.split("/")[-1]

    seen = set()
    missing = {}
    order = []
    stack = list(roots)
    while stack:
        name = stack.pop()
        if name in seen:
            continue
        if name not in defs:
            missing.setdefault(name, set())
            continue
        seen.add(name)
        order.append(name)
        for r in sorted(refs_of(defs[name])):
            if r not in seen:
                stack.append(r)
            if r not in defs:
                missing.setdefault(r, set()).add(name)

    sys.stderr.write("closure: %d types from %d roots\n" % (len(order), len(roots)))
    if missing:
        sys.stderr.write("UNRESOLVED (%d):\n" % len(missing))
        for k in sorted(missing):
            sys.stderr.write("  %-40s <- %s\n" % (k, ", ".join(sorted(missing[k])) or "root"))

    if emit_from:
        # A second "--" on a line closes an ASN.1 comment, so the text of
        # these must not contain one.
        print("-- Generated by closure.py. Do not edit.")
        print("-- Self-contained 5G SA subset of 3GPP TS 32.298 CHF CDRs.")
        print("-- Definitions marked (trimmed) come from patched/; every other")
        print("-- definition is copied verbatim from standard/.")
        print()
        print(module_header(emit_from))
        print()

    for name in sorted(order):
        print("-- [%s]" % provenance[name])
        print(defs[name])
        print()

    if emit_from:
        print("END")


if __name__ == "__main__":
    main()
