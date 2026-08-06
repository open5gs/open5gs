# Incident: AMF SBI `nausf-auth` HTTP 400 / NAS reject cause 95

## Summary

During AMF testing, the `nausf-auth` SBI call returned `HTTP 400 Bad Request` from AUSF. This failure caused the AMF to map the SBI error into a NAS 5GMM reject with cause `95: Semantically Incorrect Message`, preventing UE registration completion.

## Affected area

- AMF SBI client flow for `nausf-auth`
- AUSF SBI server response handling
- NAS 5GMM error translation path

## Error observed

- `HTTP 400` returned from AUSF on AMF SBI call
- AMF logged SBI response failure and generated a NAS reject
- UE registration stopped in the authentication phase

## Files reviewed

- `src/amf/sbi-path.c`
  - SBI transaction context
  - request header stamping
  - request/response SBI logging path
- `src/amf/context.c`
  - AMF config initialization
  - `sbi_correlation_enabled` default value
- `src/amf/nausf-build.c`
  - construction of `AuthenticationInfo` request payload
- `src/amf/gmm-sm.c`
  - handling of `nausf-auth` response events
  - translation of non-200/201 into NAS reject
- `src/amf/nas-path.c`
  - HTTP status to NAS 5GMM cause mapping
- `lib/sbi/client.c`
  - libcurl SBI client send/receive logic
  - debug logging of outgoing request bodies and incoming response bodies
- `configs/open5gs/amf.yaml.in`
- `configs/open5gs/ausf.yaml.in`
- `configs/open5gs/scp.yaml.in`

## Investigation

### What was confirmed

- The AMF change under review included SBI correlation header support.
- Correlation headers are injected by `src/amf/sbi-path.c` and are extra HTTP metadata.
- Request payload generation remains in `src/amf/nausf-build.c`.
- `lib/sbi/client.c` already supports detailed SBI body logging at debug level.
- The AMF currently maps HTTP 400 to NAS cause `SEMANTICALLY_INCORRECT_MESSAGE` in `src/amf/nas-path.c`.

### What changed in config

- Enabled debug logging in `configs/open5gs/amf.yaml.in`
- Enabled debug logging in `configs/open5gs/ausf.yaml.in`
- Enabled debug logging in `configs/open5gs/scp.yaml.in`

This ensures the runtime can capture:
- SBI outgoing request body
- SBI incoming response body
- HTTP headers and transport-level details

## Root Cause Analysis

### Likely root cause

The 400 error is likely triggered by the new AMF SBI correlation headers being emitted on the wire by default. The `nausf-auth` JSON payload builder was not changed by the logging work, while the Phase 7 path now mutates every outbound AMF SBI request immediately before send.

### Supporting evidence

- Correlation header support is implemented as additional HTTP request headers, so the JSON payload structure is not directly changed.
- The regression started after the Phase 7 SBI send-path changes.
- `src/amf/nausf-build.c` still builds the same `AuthenticationInfo` body, while `src/amf/sbi-path.c` now stamps extra headers before `ogs_sbi_send_request_to_nf_instance()`.
- Strict peer NFs, proxies, SCP routing, or generated server validation can reject unexpected custom headers even when the body is valid.

### Hypotheses

1. AUSF/SCP rejects one of the experimental custom AMF headers.
2. Mixed-case custom header names interact badly with HTTP/2 or peer validation.
3. `AuthenticationInfo` request content is malformed or missing required fields.
4. SUPI/SUCI or `serving_network_name` is built incorrectly in `src/amf/nausf-build.c`.

## Immediate conclusion

The immediate mitigation is to make AMF custom SBI wire headers opt-in by default. AMF structured logs can still carry local `trace.id`, `span.id`, and `correlation.id`; only the outbound HTTP headers are disabled unless `amf.sbi_correlation_enabled: true` is configured.

## Recommended next steps

1. Re-run registration with `amf.sbi_correlation_enabled: false`.
2. If registration succeeds, keep wire headers disabled by default and treat cross-NF correlation headers as an interoperability follow-up.
3. If registration still fails, inspect the exact `SENDING...` `nausf-auth` JSON payload from AMF and the exact HTTP 400 response body from AUSF.
4. Compare the request contents against the 3GPP `AuthenticationInfo` schema.
5. Identify the missing/invalid field and fix payload generation in `src/amf/nausf-build.c` or the corresponding AUSF validation expectation.

## Notes

- If the runtime debug logs show the `nausf-auth` payload is correct, further investigation should focus on AUSF request validation details or transport-layer mismatch.
- The current workflow in this workspace has prepared observability but has not yet captured the exact live request/response body pair.
