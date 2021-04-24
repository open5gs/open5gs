#Open5GS MongoDB Schema Update Script
#Updates existing subscribers configured prior to the database change to the new format to work with more recent version of Open5GS
#Additional functionlality like PCC rules, static assignment etc, not tested. If it's not listed below it's probably not migrated by this script.
#Written by @nickvsnetworking 30/03/2021

import copy
import pymongo


def migrate_all_subscribers(mycol):
    """Migrates all subscribers in the mycol collection from schema version 0 to version 1
    """
    subs_list = []
    for x in mycol.find():
        if 'schema_version' not in x:
            imsi = x['imsi']
            print("Subscriber record "  + str(imsi) + " needs updating")

            new_subscriber = create_v1_from_v0(x)

            #Write back to MongoDB
            myquery = { "imsi": str(imsi) }
            newvalues = { "$set": new_subscriber }
            mycol.update_one(myquery, newvalues)
            print("Updated OK")


def create_v1_from_v0(old_sub):
    """Create a v1 subscriber from an existing v0 subscriber
    """
    # Make a copy to avoid mutating the existing subscriber object so it can be
    # re-used for other parts of the migration.
    old_template_json = copy.deepcopy(old_sub)
    print(old_template_json)
    #Set AMBR Values to new format (Old format is in bits per second)
    try:
        uplink = old_template_json['ambr']['uplink']
        old_template_json['ambr']['uplink'] = {}
        old_template_json['ambr']['uplink']['value'] = uplink
        old_template_json['ambr']['uplink']['unit'] = 0
    except Exception as e:
        print(e)
        print("Failed to set Uplink AMBR values")

    try:
        downlink = old_template_json['ambr']['downlink']
        old_template_json['ambr']['downlink'] = {}
        old_template_json['ambr']['downlink']['value'] = downlink
        old_template_json['ambr']['downlink']['unit'] = 0
    except Exception as e:
        print(e)
        print("Failed to set Downlink AMBR values")

    #Propogate APN / DDN Slice Details
    old_template_json['slice'] = []
    old_template_json['slice'].append({"sst": 1, "default_indicator" : True, "session" : []})

    i = 0
    while i < len(old_template_json['pdn']):
        ddn_dict = {}
        ddn_dict['name'] = old_template_json['pdn'][i]['apn']
        ddn_dict['type'] = old_template_json['pdn'][i]['type']
        ddn_dict['pcc_rule'] = old_template_json['pdn'][i]['pcc_rule']
        ddn_dict['qos'] = old_template_json['pdn'][i]['qos']
        ddn_dict['qos']['index'] = old_template_json['pdn'][i]['qos']['qci']
        ddn_dict['qos']['arp'] = old_template_json['pdn'][i]['qos']['arp']
        ddn_dict['ambr'] = {"uplink": {"value": old_template_json['pdn'][i]['ambr']['uplink'], "unit": 0}, "downlink": {"value": old_template_json['pdn'][i]['ambr']['downlink'], "unit": 0}}
        i += 1
        old_template_json['slice'][0]['session'].append(ddn_dict)

    #Remove old PDN info
    #del old_template_json['pdn']

    #Add "schema_version" feild
    old_template_json['schema_version'] = 1

    return old_template_json


if __name__ == "__main__":
    myclient = pymongo.MongoClient("mongodb://localhost:27017/")
    mydb = myclient["open5gs"]

    migrate_all_subscribers(mycol=mydb["subscribers"])
