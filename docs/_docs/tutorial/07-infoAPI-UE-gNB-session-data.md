---
title: Open5GS JSON infoAPI for accessing UE, gNB/eNB, and session data
---

## How it works

Open5GS has a lightweight HTTP server (already used for `/metrics`) embedded in some NFs.  
New optional JSON endpoints were added:

| NF  | Endpoint | Content |
| --- | --- | --- |
| SMF | /pdu-info | All currently connected UEs + their PDU sessions (IMSI/SUPI, DNN, IPs, S-NSSAI, QoS, state, etc.) |
| AMF | /gnb-info | All currently connected gNBs and their supported TAs, PLMNs, SCTP info, number of UEs |
| AMF | /ue-info | All currently connected NR UEs and their info, active gNB, tai, security, slices, am_policy |
| MME | /enb-info | All currently connected eNBs and their supported TAs, PLMNs, SCTP info, number of UEs |
| MME | /ue-info | All currently connected LTE UEs and their info, active eNB, tai, pdn info |

They are exposed on the same HTTP port used by Prometheus metrics (default `:9090`).

To reduce processor load when there are a large number of devices, the API includes a pager that limits output.

`/ue-info?page=0&page_size=100`

page in the range 0-n (0 is default),

`page=-1` to avoid paging

`page_size=100` (default and MAX)

---

## How to enable

You need to build Open5GS from the latest `main` branch:

```
git clone https://github.com/open5gs/open5gs.git
cd open5gsgit checkout mainmeson build --prefix=`pwd`/installninja -C build install
```

## How to use

### Get info about PDUs from all UEs connected to 5G NR and LTE basestation

contains session info as APN/DNN, IP addresses, slice info, QoS info, PDU/PDN_state and UE_activity status

```
curl -s "http://127.0.0.4:9090/pdu-info" |jq . 
```

##### Example response

```
{
  "items": [
    {
      "supi": "imsi-231510000114763",
      "pdu": [
        {
          "psi": 1,
          "dnn": "internet",
          "ipv4": "10.45.0.11",
          "snssai": {
            "sst": 1,
            "sd": "ffffff"
          },
          "qos_flows": [
            {
              "qfi": 1,
              "5qi": 9
            }
          ],
          "pdu_state": "inactive"
        }
      ],
      "ue_activity": "idle"
    }
  ],
  "pager": {
    "page": 0,
    "page_size": 100,
    "count": 1,
  }
}
```

### Get all 5G NR connected UEs (from AMF)

contains the UE_ID as SUPI, SUCI, PEI, GUTI, TMSI, GNB_ID to which the UE is connected, AMBR, slices ..

```
curl -s "http://127.0.0.5:9090/ue-info" |jq . 
```

##### Example response

```
{
  "items": [
    {
      "supi": "imsi-001010000056492",
      "suci": "suci-0-001-01-0000-1-1-85731c4241d466407311a85135e914987944f361550f13f4532e29bdc5859548a363cc1a81798bb4cc59f2b4db",
      "pei": "imeisv-3535938300494715",
      "cm_state": "connected",
      "guti": "00101-030080-CC00007AB",
      "m_tmsi": 3221227435,
      "gnb": {
        "ostream_id": 1,
        "amf_ue_ngap_id": 2,
        "ran_ue_ngap_id": 39,
        "gnb_id": 100,
        "cell_id": 1
      },
      "location": {
        "timestamp": 1758733375895512,
        "nr_tai": {
          "plmn": "99970",
          "tac_hex": "000001",
          "tac": 1
        },
        "nr_cgi": {
          "plmn": "99970",
          "nci": 1638401,
          "gnb_id": 100,
          "cell_id": 1
        },
        "last_visited_plmn_id": "000000"
      },
      "msisdn": [],
      "security": {
        "valid": 1,
        "enc": "nea2",
        "int": "nia2"
      },
      "ambr": {
        "downlink": 1000000000,
        "uplink": 1000000000
      },
      "pdu_sessions": [],
      "pdu_sessions_count": 0,
      "requested_slices": [
        {
          "sst": 1,
          "sd": "ffffff"
        }
      ],
      "allowed_slices": [
        {
          "sst": 1,
          "sd": "ffffff"
        }
      ],
      "requested_slices_count": 1,
      "allowed_slices_count": 1,
      "am_policy_features": 4,
      "am_policy_features_info": {
        "hex": "0x0000000000000004",
        "bits": [
          2
        ],
        "labels": [
          "QoS Policy Control"
        ]
      }
    }
  ],
  "pager": {
    "page": 0,
    "page_size": 100,
    "count": 2
  }
}
```

### Get all LTE connected UEs ( from MME )

contains the ENB_ID to which the UE is connected

```
curl -s "http://127.0.0.2:9090/ue-info?" |jq . 
```

##### Example response

```
{
  "items": [
    {
      "supi": "999700000021632",
      "domain": "EPS",
      "rat": "E-UTRA",
      "cm_state": "connected",
      "enb": {
        "ostream_id": 3,
        "mme_ue_ngap_id": 3,
        "ran_ue_ngap_id": 9,
        "enb_id": 264040,
        "cell_id": 67594275
      },
      "location": {
        "tai": {
          "plmn": "99970",
          "tac_hex": "0001",
          "tac": 1
        }
      },
      "ambr": {
        "downlink": 1000000000,
        "uplink": 1000000000
      },
      "pdn": [
        {
          "apn": "internet",
          "qos_flows": [
            {
              "ebi": 5
            }
          ],
          "qci": 9,
          "ebi": 5,
          "bearer_count": 1,
          "pdu_state": "active"
        }
      ],
      "pdn_count": 1
    }
  ],
  "pager": {
    "page": 0,
    "page_size": 100,
    "count": 1
  }
}
```

### Get all connected gNBs (from AMF)

contains gNB info and number of RRC connected UEs

```
curl -s "http://127.0.0.5:9090/gnb-info?" |jq . 
```

##### Example response

```
{
  "items": [
    {
      "gnb_id": 100,
      "plmn": "99970",
      "network": {
        "amf_name": "efire-amf0",
        "ngap_port": 38412
      },
      "ng": {
        "sctp": {
          "peer": "[192.168.168.100]:60110",
          "max_out_streams": 2,
          "next_ostream_id": 1
        },
        "setup_success": true
      },
      "supported_ta_list": [
        {
          "tac": "000001",
          "bplmns": [
            {
              "plmn": "99970",
              "snssai": [
                {
                  "sst": 1,
                  "sd": "ffffff"
                }
              ]
            },
            {
              "plmn": "99971",
              "snssai": [
                {
                  "sst": 2,
                  "sd": "000000"
                }
              ]
            }
          ]
        }
      ],
      "num_connected_ues": 2
    }
  ],
  "pager": {
    "page": 0,
    "page_size": 100,
    "count": 1
  }
}
```

### Get all connected eNBs (from MME)

contains eNB info and number of connected UEs

```
curl -s "http://127.0.0.2:9090/enb-info?" |jq . 
```

##### Example response

```
{
  "items": [
    {
      "enb_id": 264040,
      "plmn": "99970",
      "network": {
        "mme_name": "efire-mme0"
      },
      "s1": {
        "sctp": {
          "peer": "[192.168.168.254]:36412",
          "max_out_streams": 10,
          "next_ostream_id": 3
        },
        "setup_success": true
      },
      "supported_ta_list": [
        {
          "tac": "0001",
          "plmn": "99970"
        }
      ],
      "num_connected_ues": 1
    }
  ],
  "pager": {
    "page": 0,
    "page_size": 100,
    "count": 1
  }
}
```

Using a suitable filter in jq, the output can be simplified and adapted to the desired form.

AMF/ue-info sorted on supi ( page=-1 to avoid paging and show all UEs )

```
./amf_ue_sort.sh 
supi                  cm-state   gnb_id  dnn
imsi-001010000056492  connected  100     internet
imsi-231510000114763  connected  100     internet
imsi-999700000021630  connected  100     internet
imsi-999700000083810  idle       100     internet
imsi-999700000114762  idle       100     internet
```

```
curl -s "http://127.0.0.5:9090/ue-info?page=-1" \
| jq -r '
  # Unwrap common list keys or accept a bare array/object
  ( .ue_infos // .items // .data // .results // .list // . )
  | (if type=="array" then . else [.] end)
| sort_by(.supi // "")| group_by(.supi)| map({supi: (.[0].supi // "-"),
  cm: (if any(.[]; .cm_state == "connected") then "connected"
       else (.[-1].cm_state // "-") end),
  gnb_id: (
    ( [ .[] | select(.cm_state=="connected")
          | (.gnb.gnb_id? // .location.nr_cgi.gnb_id?) ]
      | map(select(.!=null)) | .[0] )
    // ( [ .[] | (.gnb.gnb_id? // .location.nr_cgi.gnb_id?) ]
         | map(select(.!=null)) | .[0] )
    // "-"
  ),
  dnns: (
    [ .[].pdu_sessions[]?.dnn ]
    | map(select(. != null and . != ""))
    | unique | sort
    | if length==0 then ["-"] else . end
  )
})
| sort_by( ( .supi | tostring | sub("^imsi-"; "") | tonumber? ) // 0 )
```

AMF/gnb-info

```
./amf_gnb_sort.sh 
gnb_id  peer                     plmn   num_connected_ues
100     [192.168.168.100]:60110  99970  1
411     [192.168.168.99]:34349   99970  0
```

```
curl -s "http://127.0.0.5:9090/gnb-info" \
| jq -r '
  ( .items // .gnbs // .data // .results // .list // . )
  | (if type=="array" then . else [.] end)
  | sort_by(.gnb_id // 0)
  | (["gnb_id","peer","plmn","num_connected_ues"]),
    ( .[] |
      [ (.gnb_id // "-"),
        (.ng?.sctp?.peer // "-"),
        (.plmn // "-"),
        ((.num_connected_ues // 0) | tostring)
      ])
  | @tsv
' | column -s $'\t' -t
```

MME/enb-info

```
./mme_enb_sort.sh 
enb_id  peer                     plmn   num_connected_ues
264040  [192.168.168.254]:36412  99970  1
```

```
curl -s "http://127.0.0.2:9090/enb-info" \
| jq -r '
  ( .items // .enbs // .data // .results // .list // . )
  | (if type=="array" then . else [.] end)
  | sort_by(.enb_id // 0)
  | (["enb_id","peer","plmn","num_connected_ues"]),
    ( .[] |
      [ (.enb_id // "-"),
        (.s1?.sctp?.peer // "-"),
        (.plmn // "-"),
        ((.num_connected_ues // 0) | tostring)
      ])
  | @tsv
' | column -s $'\t' -t
```

MME/ue-info

```
./mme_ue_sort.sh 
supi             cm-state   enb_id  apn
001010000056492  connected  264040  internet
999700000021632  connected  264040  internet
999700000083811  connected  264040  internet
```

```
curl -s "http://127.0.0.2:9090/ue-info?page=-1" \
| jq -r '
  ( .items // .ue_infos // .data // .results // .list // . )
  | (if type=="array" then . else [.] end)
| map(select(((.rat? // "") | test("^(E-?UTRA|LTE)$"; "i")) or((.domain? // "") | test("^EPS$"; "i"))))
| sort_by(.supi // "")| group_by(.supi)| map({supi: (.[0].supi // "-"),cm: (if any(.[]; .cm_state == "connected") then "connected"else (.[-1].cm_state // "-") end),enb_id: (( [ .[] | select(.cm_state=="connected") | .enb.enb_id? ]| map(select(.!=null)) | .[0] )// ( [ .[] | .enb.enb_id? ] | map(select(.!=null)) | .[0] )// "-"),apns: ([ .[].pdn[]?.apn ]| map(select(. != null and . != ""))| unique | sort| if length==0 then ["-"] else . end)})
| sort_by( ( .supi | tostring | sub("^imsi-"; "") | tonumber? ) // 0 )
```

SMF/pdu-info

```
./smf_pdu_sort.sh 
supi                  ue_activity  dnn       ipv4/ipv6      pdu_state
imsi-001010000056492  active       internet  10.45.0.59     active
imsi-231510000114763  idle         internet  10.45.0.52     inactive
imsi-999700000021630  idle         internet  10.45.250.250  inactive
999700000021632       unknown      internet  10.45.0.8      unknown
imsi-999700000083810  idle         internet  10.45.0.12     inactive
imsi-999700000114762  idle         internet  10.45.0.10     inactive
```

```
curl -s "http://127.0.0.4:9090/pdu-info?page=-1" \
| jq -r '
  ( .items // .ue_infos // .data // .results // .list // . )
  | (if type=="array" then . else [.] end) as $arr
| ($arr | map((.pdu // []) | length) | max // 0) as $maxp
| ( ["supi","ue_activity"]+ ([ range(0; $maxp) | "dnn","ipv4/ipv6","pdu_state" ]) ),
# Rows
( $arr
  | sort_by( ( .supi | tostring | sub("^imsi-"; "") | tonumber? ) // 0 )
  | .[]
  | . as $ue
  | ( $ue.pdu // [] | sort_by(.psi // .ebi // 0) ) as $pdus
  | [ ($ue.supi // "-"), ($ue.ue_activity // "-") ]
    + (
        $pdus
        | map([
            (.dnn // .apn // "-"),
            ( ([.ipv4?, .ipv6?] | map(select(.!=null and .!="")) | join("/") )
              | if .=="" then "-" else . end ),
            (.pdu_state // "-")
          ])
        | add // []
      )
    + ( [ range( ( ($pdus|length) * 3 ); ($maxp * 3) ) | "-" ] )
)
```
