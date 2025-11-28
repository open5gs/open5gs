# Tutorial for add and delete PLMNs dynamically in the AMF

Este tutorial muestra las diferentes llamadas de la API de gestión OAM (Operations, Administration and Maintenance) para el AMF (Access and Mobility Management Function) en Open5GS, con el objetivo de eliminar y crear PLMNs de forma dinámica sin tener que reiniciar el AMF. Se ha implementando extendiendo el SBI por lo que se usa HTTP/2.0.

## Objetivo

Implementar una API RESTful que permita:
1. Consultar la configuración de PLMNs y slices (S-NSSAI)
2. Añadir dinámicamente nuevos PLMNs sin reiniciar el AMF
3. Eliminar dinámicamente PLMNs sin reiniciar el AMF y deregistrar los UEs asociados
4. Notificar automáticamente a los gNBs sobre cambios en la configuración

## Llamadas API

 - GET 
     - http://<AMFIP>:7777/namf-oam/v1/plmns 
         - Muestra todos los PLMNs del contexto actual
     - http://<AMFIP>:7777/namf-oam/v1/plmns/{plmnid}
         - Muestra la información del PLMN id, PLMNid está formado por el mcc y mnc
 - POST
      - http://<AMFIP>:7777/namf-oam/v1/plmns 
        - Añade un nuevo PLMN a la lista o un nuevo sst y sd, si ya existiese. Notifica a los gnBs
 - DELETE
      - http://<AMFIP>:7777/namf-oam/v1/plmns/{plmnid}
        - Elimina el PLMN y desregistra a los UEs asociados y notifica a los gnBs

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