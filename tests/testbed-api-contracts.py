#!/usr/bin/env python3

from __future__ import annotations

from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


def read(relative: str) -> str:
    return (ROOT / relative).read_text(encoding="utf-8")


def require(content: str, token: str, source: str) -> None:
    if token not in content:
        raise AssertionError(f"{source} is missing required contract token: {token}")


def forbid(content: str, token: str, source: str) -> None:
    if token in content:
        raise AssertionError(f"{source} still contains forbidden token: {token}")


def require_order(content: str, earlier: str, later: str, source: str) -> None:
    try:
        earlier_index = content.index(earlier)
        later_index = content.index(later)
    except ValueError as exc:
        raise AssertionError(f"{source} is missing required ordered token: {exc}") from exc
    if earlier_index >= later_index:
        raise AssertionError(f"{source} must keep {earlier!r} before {later!r}")


def main() -> int:
    pdu_info = read("src/smf/pdu-info.c")
    migration = read("src/smf/migration.c")
    namf_handler = read("src/smf/namf-handler.c")
    gsm_sm = read("src/smf/gsm-sm.c")
    metrics = read("lib/metrics/prometheus/context.c")

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
        "migration_id_str",
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
        "action_must_be_prepare_switch_drain_abort_or_status",
        'is_action(action, "status")',
        "prepare_already_active",
        "required_fields_are_non_empty_supi_integral_psi",
        "migration_id_required",
        "migration_id_mismatch",
        "source_upf_mismatch",
        "target_upf_mismatch",
        "target_cleanup_not_allowed_after_switch_sent",
        "migration_target_cleanup_allowed",
        "target_node == sess->migration.target_node",
        "prepare_requires_target_upf",
        "target_upf_draining",
        "target_is_source",
        "target_upf_not_eligible",
        "source_buffering",
        "source_releasing",
        "smf_migration_handle_source_buffering_response",
        "smf_migration_handle_source_release_response",
        "smf_migration_handle_pfcp_node_deassociated",
        "smf_migration_handle_pfcp_node_restoration",
        "source UPF cleanup dispatch failed after path switch response",
        "source UPF PFCP association lost during post-switch cleanup",
        "source UPF PFCP restoration during post-switch cleanup",
        "source UPF cleanup already in progress",
        "path_switch_committed_to_target",
        "OGS_PFCP_MODIFY_UPF_MIGRATION_SOURCE_BUFFER",
        "OGS_PFCP_MODIFY_UPF_MIGRATION_SOURCE_RELEASE",
    ):
        require(migration, token, "src/smf/migration.c")

    migration_handler = migration[migration.index("size_t smf_handle_pdu_migrate"):]

    for followup_action in (
        'if (is_action(action, "status")) {\n        cJSON_Delete(request);',
        'if (is_action(action, "switch")) {\n        const char *reject_reason',
        'if (is_action(action, "abort")) {\n        const char *reject_reason',
        'if (is_action(action, "drain")) {\n        const char *reject_reason',
    ):
        require_order(
            migration_handler,
            followup_action,
            "if (!migration_json_nonempty_string(target_upf))",
            "src/smf/migration.c",
        )

    for token in (
        '*status_code = 405;',
        'migration_json_add_string(root, "reason", "method_not_allowed")',
        '*status_code = 400;',
        'migration_json_add_string(root, "reason", "invalid_json_body")',
        '*status_code = 404;',
        'migration_json_add_string(root, "reason", "ue_not_found")',
        '*status_code = 409;',
        'migration_json_add_string(root, "reason", "migration_already_active")',
        '*status_code = 202;',
        'migration_json_add_string(root, "result", "accepted")',
        "migration_validate_followup_request(",
        "migration_json_add_migration_id(root, sess->migration.id)",
    ):
        require(migration, token, "src/smf/migration.c")

    forbid(migration, "strstr(active_target", "src/smf/migration.c")
    forbid(migration, "sess->migration.state = SMF_MIGRATION_STATE_FAILED;\n                smf_migration_send_target_deletion(sess);", "src/smf/migration.c")
    forbid(namf_handler, "sess->migration.state = SMF_MIGRATION_STATE_FAILED;\n                smf_migration_send_target_deletion(sess);", "src/smf/namf-handler.c")
    forbid(gsm_sm, "sess->migration.state = SMF_MIGRATION_STATE_FAILED;\n                smf_migration_send_target_deletion(sess);", "src/smf/gsm-sm.c")

    for token in (
        "node->request_handler",
        'node->request_handler, method, "", 0',
    ):
        require(metrics, token, "lib/metrics/prometheus/context.c")

    print("SMF testbed API contract behavior tokens are present")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
