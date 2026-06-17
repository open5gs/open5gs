#!/usr/bin/env python3

from __future__ import annotations

from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


def read(relative: str) -> str:
    return (ROOT / relative).read_text(encoding="utf-8")


def require(content: str, token: str, source: str) -> None:
    if token not in content:
        raise AssertionError(f"{source} is missing required contract token: {token}")


def main() -> int:
    pdu_info = read("src/smf/pdu-info.c")
    migration = read("src/smf/migration.c")

    for token in (
        "SMF_TESTBED_API_VERSION",
        "5g3e.smf.pdu-info/v1",
        "5g3e.smf.upf-info/v1",
        "api_version",
        "schema",
        "endpoint",
        "integer_encoding",
        "ue_location_timestamp_us",
        "target_upf_n4_seid_str",
    ):
        require(pdu_info, token, "src/smf/pdu-info.c")

    for token in (
        "SMF_TESTBED_API_VERSION",
        "5g3e.smf.pdu-migrate/v1",
        "api_version",
        "schema",
        "endpoint",
        "result",
        "reason",
        "migration_state",
        "target_upf_not_associated",
        "target_upf_ftup_required",
        "migration_already_active",
    ):
        require(migration, token, "src/smf/migration.c")

    print("SMF testbed API contract tokens are present")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
