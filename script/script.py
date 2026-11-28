from pymongo import MongoClient

# Connect to MongoDB
client = MongoClient('mongodb://localhost:27017/')
db = client['open5gs']
collection = db['subscribers']

# Find the last document to get the current "imsi" value
last_document = collection.find_one(sort=[('_id', -1)])
current_imsi = int(last_document.get('imsi', '999700000000200'))

# Insert 1000 entries with an incrementing "imsi" value
for i in range(5):
    current_imsi += 1
    base_data = {
        'ambr': {'downlink': {'value': 1, 'unit': 3}, 'uplink': {'value': 1, 'unit': 3}},
        'schema_version': 1,
        'msisdn': [],
        'imeisv': [],
        'mme_host': [],
        'mme_realm': [],
        'purge_flag': [],
        'access_restriction_data': 32,
        'subscriber_status': 0,
        'network_access_mode': 0,
        'subscribed_rau_tau_timer': 12,
        'imsi': str(current_imsi),
        'security': {'k': '465B5CE8 B199B49F AA5F0A2E E238A6BC', 'amf': '8000', 'op': None, 'opc': 'E8ED289D EBA952E4 283B54E8 8E6183CA'},
        'slice': [{'sst': 1, 'default_indicator': True, 'session': [{'qos': {'arp': {'priority_level': 8, 'pre_emption_capability': 1, 'pre_emption_vulnerability': 1}, 'index': 9}, 'ambr': {'downlink': {'value': 1, 'unit': 3}, 'uplink': {'value': 1, 'unit': 3}}, '_id': 1, 'name': 'internet', 'type': 3, 'pcc_rule': []}]}],
        '__v': 0
    }
    collection.insert_one(base_data)


