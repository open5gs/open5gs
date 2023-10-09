## Open5GS Python Library

Basic Python library to interface with MongoDB subscriber DB in Open5GS HSS / PCRF. Requires Python 3+, pymongo and bson. (All available through PIP)

If you are planning to run this on a different machine other than localhost (the machine hosting the MongoDB service) you will need to enable remote access to MongoDB by binding it's IP to 0.0.0.0:

This is done by editing ```/etc/mongodb.conf``` and changing the bind IP to:
``` bind_ip = 0.0.0.0 ```

Restart MongoDB for changes to take effect.

``` $ /etc/init.d/mongodb restart ```


Basic Example:
```
import Open5GS
Open5GS_1 = Open5GS("10.0.1.118", 27017)

slice_data = [
    {
      "sst": 1,
      "default_indicator": True,
      "session": [
        {
          "name": "internet",
          "type": 3, "pcc_rule": [], "ambr": {"uplink": {"value": 1, "unit": 3}, "downlink": {"value": 1, "unit": 3}},
          "qos": {
            "index": 9,
            "arp": {"priority_level": 8, "pre_emption_capability": 1, "pre_emption_vulnerability": 1}
          }
        }
      ]
    }
  ]

sub_data = {
  "imsi": "001010000000004",
  "subscribed_rau_tau_timer": 12,
  "network_access_mode": 0,
  "subscriber_status": 0,
  "operator_determined_barring": 0,
  "access_restriction_data": 32,
  "slice" : slice_data,
  "ambr": {"uplink": {"value": 1, "unit": 3}, "downlink": {"value": 1, "unit": 3}},
  "security": {
    "k": "465B5CE8 B199B49F AA5F0A2E E238A6BC",
    "amf": "8000",
    'op': None,
    "opc": "E8ED289D EBA952E4 283B54E8 8E6183CA"
  },
  "schema_version": 1,
  "__v": 0
}

print(Open5GS_1.AddSubscriber(sub_data))                        #Add Subscriber using dict of sub_data

print(Open5GS_1.GetSubscriber('891012222222300'))               #Get added Subscriber's details

print(Open5GS_1.UpdateSubscriber('891012222222300', sub_data))  #Update subscriber

print(Open5GS_1.DeleteSubscriber('891012222222300'))            #Delete Subscriber

Subscriber_List = Open5GS_1.GetSubscribers()
for subscribers in Subscriber_List:
  print(subscribers['imsi'])

```
