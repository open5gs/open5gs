---
title: PCC Rules in the PCF and PCRF
---

This page explains how Open5GS builds PCC rules: where they come from, which keys are accepted, and what the rules are called on the wire. It applies to the PCF (5GC, N7) and the PCRF (EPC, Gx).

## 1. Where the rules come from

The PCF and PCRF look for a subscriber's policy in two places, and use only one of them:

1. **The `policy:` section in `pcf.yaml` / `pcrf.yaml`.** The first `policy:` entry that matches the subscriber is used, and the session and its PCC rules come from the YAML. The PCF matches on `supi_range`, `plmn_id`, or both (an entry with both must match both). The PCRF matches on `supi_range` only.
2. **The subscriber database (WebUI / MongoDB).** Used only when no `policy:` entry matches.

So if you add PCC rules to a subscriber in the WebUI but a `policy:` entry in the YAML also covers that subscriber, the WebUI rules are not used. There is no fallback either: if the matching entry has no session for the requested DNN/APN (or, on the PCF, no matching slice), the request fails with `No SESSION` (or `No SLICE`) in the log instead of using the WebUI data.

## 2. Configuring rules in YAML

Rules sit under a session (DNN/APN) in `pcc_rule`. Each rule accepts two keys, `qos` and `flow`. The layout above the session differs between the two functions.

**PCF (`pcf.yaml`)**: sessions sit under a `slice:`.

```yaml
policy:
  - supi_range:
      - 999700000000001-999709999999999
    slice:
      - sst: 1
        default_indicator: true
        session:
          - name: internet
            type: 3
            ambr:
              downlink: { value: 1, unit: 3 }
              uplink: { value: 1, unit: 3 }
            qos:
              index: 9
              arp: { priority_level: 8, pre_emption_vulnerability: 1, pre_emption_capability: 1 }
            pcc_rule:
              - qos:
                  index: 1
                  arp: { priority_level: 1, pre_emption_vulnerability: 1, pre_emption_capability: 1 }
                  mbr:
                    downlink: { value: 82, unit: 1 }
                    uplink: { value: 82, unit: 1 }
                  gbr:
                    downlink: { value: 82, unit: 1 }
                    uplink: { value: 82, unit: 1 }
                flow:
                  - direction: 2
                    description: "permit out udp from 10.200.136.98/32 23455 to assigned 1-65535"
                  - direction: 1
                    description: "permit out udp from 10.200.136.98/32 1-65535 to assigned 50021"
```

**PCRF (`pcrf.yaml`)**: there is no `slice:` level. `session:` sits directly under the policy entry (the PCRF creates one default slice itself). The same session written for the PCRF:

```yaml
policy:
  - supi_range:
      - 999700000000001-999709999999999
    session:
      - name: internet
        type: 3
        ambr:
          downlink: { value: 1, unit: 3 }
          uplink: { value: 1, unit: 3 }
        qos:
          index: 9
          arp: { priority_level: 8, pre_emption_vulnerability: 1, pre_emption_capability: 1 }
        pcc_rule:
          - qos:
              index: 1
              arp: { priority_level: 1, pre_emption_vulnerability: 1, pre_emption_capability: 1 }
              mbr:
                downlink: { value: 82, unit: 1 }
                uplink: { value: 82, unit: 1 }
              gbr:
                downlink: { value: 82, unit: 1 }
                uplink: { value: 82, unit: 1 }
            flow:
              - direction: 2
                description: "permit out udp from 10.200.136.98/32 23455 to assigned 1-65535"
              - direction: 1
                description: "permit out udp from 10.200.136.98/32 1-65535 to assigned 50021"
```

If you copy the PCF layout into `pcrf.yaml`, the `slice:` key is ignored, the policy entry still matches but has no sessions, and every Gx request for those subscribers fails with `No SESSION`.

Things to know:

- **Only `qos` and `flow` are read.** Any other key in a rule, for example `name`, is skipped and the log shows `unknown key`.
- **QCI and all three ARP values are mandatory.** If `qos.index`, `arp.priority_level`, `arp.pre_emption_capability` or `arp.pre_emption_vulnerability` is missing, the rule is not installed and the log shows `Mandatory is MISSING - QCI[..], ARP[..:..:..]`.
- **`direction` must be 1 (downlink) or 2 (uplink).** Any other value stops the configuration from loading with `Unknown Direction`. A flow without both `direction` and `description` is ignored.
- **Limits:** up to 8 PCC rules per session. A 9th rule in the YAML is not skipped: it fails an assertion and the daemon aborts at startup.

## 3. How the rules are named

Rule names are generated, not configured. They are built from the session (DNN/APN) name and the rule's position in the list:

| Interface | Field | Name |
| --- | --- | --- |
| EPC, Gx | Charging-Rule-Name | `<dnn>-g<N>` |
| 5GC, N7 | PCC-Rule-Id | `<dnn>-n<N>` |

The precedence is `N` as well. For session `internet`, the first rule is `internet-g1` on Gx and `internet-n1` on N7, the second `internet-g2` / `internet-n2`, and so on. A rule skipped because of a missing mandatory value does not use up a number.

Rules provisioned per subscriber in the WebUI are named the same way.

If you need a specific rule name, for example to match a predefined rule on a gateway, that is not configurable today.
