#Open5GS MongoDB Schema Update Script
#Updates existing subscribers configured prior to the database change to the new format to work with more recent version of Open5GS
#Additional functionlality like PCC rules, static assignment etc, not tested. If it's not listed below it's probably not migrated by this script.
#Written by @nickvsnetworking 30/03/2021

import json
import sys
import random, string
import mongo
import pymongo

myclient = pymongo.MongoClient("mongodb://localhost:27017/")
mydb = myclient["open5gs"]
mycol = mydb["subscribers"]
subs_list = []
for x in mycol.find():
    if 'schema_version' not in x:
        print("Subscriber record "  + str(x['imsi']) + " needs updating")
        old_template_json = x
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

        #Write back to MongoDB
        myquery = { "imsi": str(old_template_json['imsi'])}
        newvalues = { "$set": old_template_json }
        mycol.update_one(myquery, newvalues)
        print("Updated OK")

