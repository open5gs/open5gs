# Tutorial: Dynamically Adding and Removing PLMNs in the AMF

This tutorial describes the different OAM (Operations, Administration and Maintenance) management API calls for the AMF (Access and Mobility Management Function) in Open5GS. The goal is to dynamically add and remove PLMNs without restarting the AMF.  
The feature is implemented by extending the SBI, so the API uses HTTP/2.0.

## Objective

Implement a RESTful API that allows you to:

1. Query the configuration of PLMNs and slices (S-NSSAI)
2. Dynamically add new PLMNs without restarting the AMF
3. Dynamically delete PLMNs without restarting the AMF and deregister the associated UEs
4. Automatically notify gNBs about configuration changes

## API Calls

| Method | URL                                                      | Description                                                                                          |
|--------|-----------------------------------------------------------|------------------------------------------------------------------------------------------------------|
| GET    | `http://<AMFIP>:7777/namf-oam/v1/plmns`                  | Returns all PLMNs in the current AMF context.                                                       |
| GET    | `http://<AMFIP>:7777/namf-oam/v1/plmns/{plmnid}`         | Returns detailed information for the PLMN identified by `{plmnid}` (composed of MCC and MNC).      |
| POST   | `http://<AMFIP>:7777/namf-oam/v1/plmns`                  | Adds a new PLMN to the list or a new `sst`/`sd` combination if the PLMN already exists; notifies gNBs. |
| DELETE | `http://<AMFIP>:7777/namf-oam/v1/plmns/{plmnid}`         | Deletes the specified PLMN, deregisters the associated UEs, and notifies the gNBs.                  |

## How to use

### Get all PLMNs in the AMF
Retrieve all PLMNs (mcc, mnc, s_nssai with stt and sd)
```
curl --http2-prior-knowledge -X GET http://<AMFIP>:7777/namf-oam/v1/plmns
```

##### Example response
```
{
        "plmns":        [{
                        "plmn_id":      "99970",
                        "mcc":  999,
                        "mnc":  70,
                        "s_nssai":      [{
                                        "sst":  1,
                                        "sd":   "333333"
                                }, {
                                        "sst":  3
                                }]
                }],
        "total_plmns":  1,
        "connected_gnbs":       0,
        "registered_ues":       0
}
```


### Get specific PLMN
Retrieve all entries for the specific PLMN (mcc, mnc, s_nssai with stt and sd) using the PLMN id which is formed using the mcc+mc
```
curl --http2-prior-knowledge -X GET http://<AMFIP>:7777/namf-oam/v1/plmns/99970
```

##### Example response
```
{
        "plmn_id":      "99970",
        "mcc":  999,
        "mnc":  70,
        "s_nssai":      [{
                        "sst":  1,
                        "sd":   "333333"
                }, {
                        "sst":  3
                }]
}
```


### Add new PLMN in the AMF
Add a new PLMN to the AMF and notify gnBs (send AMFConfigurationUpdate)
```
curl --http2-prior-knowledge -X POST http://<AMFIP>:7777/namf-oam/v1/plmns \
  -H "Content-Type: application/json" \
  -d '{
    "plmn_id": {
      "mcc": "999",
      "mnc": "70"
    },
    "s_nssai": [
      {"sst": 1, "sd": "333333"},
      {"sst": 3}
    ]
  }'
```

##### Example response
```
{
    "status":       "success",
    "message":      "PLMN added successfully",
    "total_plmns":  1
}
```

### Remove PLMN
Remove a PLMN of the AMF and notify the gnBs (send AMFConfigurationUpdate) and unregister the UEs to avoid the go through the CORE 5G

```
curl --http2-prior-knowledge -X DELETE http://10.244.0.216:7777/namf-oam/v1/plmns/99970
```


##### Example response

```
{
        "status":       "success",
        "message":      "PLMN deleted successfully",
        "deleted_plmn_id":      "99970",
        "deleted_entries":      1,
        "remaining_plmns":      0
}
```