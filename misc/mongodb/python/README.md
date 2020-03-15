## Open5GS Python Library

Basic Python library to interface with MongoDB subscriber DB in Open5GS HSS / PCRF. Requires Python 3+, mongo, pymongo and bson. (All available through PIP)

If you are planning to run this on a different machine other than localhost (the machine hosting the MongoDB service) you will need to enable remote access to MongoDB by binding it's IP to 0.0.0.0:

This is done by editing ```/etc/mongodb.conf``` and changing the bind IP to:
``` bind_ip = 0.0.0.0 ```

Restart MongoDB for changes to take effect.

``` $ /etc/init.d/mongodb restart ```


Basic Example:
```
import Open5GS
Open5GS_1 = Open5GS("10.0.1.118", 27017)

pdn = [{'apn': 'internet', 'pcc_rule': [], 'ambr': {'downlink': 1234, 'uplink': 1234}, 'qos': {'qci': 9, 'arp': {'priority_level': 8, 'pre_emption_vulnerability': 1, 'pre_emption_capability': 1}}, 'type': 2}]
sub_data = {'imsi': '891012222222300', \
             'pdn': pdn, \
             'ambr': {'downlink': 1024000, 'uplink': 1024001}, \
             'subscribed_rau_tau_timer': 12, \
             'network_access_mode': 2, \
             'subscriber_status': 0, \
             'access_restriction_data': 32, \
             'security': {'k': '465B5CE8 B199B49F AA5F0A2E E238A6BC', 'amf': '8000', 'op': None, 'opc': 'E8ED289D EBA952E4 283B54E8 8E6183CA'}, '__v': 0}

print(Open5GS_1.AddSubscriber(sub_data))                        #Add Subscriber using dict of sub_data

print(Open5GS_1.GetSubscriber('891012222222300'))               #Get added Subscriber's details

print(Open5GS_1.UpdateSubscriber('891012222222300', sub_data))  #Update subscriber

print(Open5GS_1.DeleteSubscriber('891012222222300'))            #Delete Subscriber

Subscriber_List = Open5GS_1.GetSubscribers()
for subscribers in Subscriber_List:
  print(subscribers['imsi'])

```
