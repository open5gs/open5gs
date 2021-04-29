#Open5GS MongoDB Schema Update Script
#Updates existing subscribers configured prior to the database change to the new format to work with more recent version of Open5GS
#Additional functionlality like PCC rules, static assignment etc, not tested. If it's not listed below it's probably not migrated by this script.
#Written by @nickvsnetworking 30/03/2021

import copy
import pymongo


def migrate_all_subscribers(mycol):
    """Migrates all subscribers in the mycol collection from schema version 0 to version 1
    """
    for x in mycol.find():
        if 'schema_version' not in x:
            imsi = x['imsi']
            print("Subscriber record "  + str(imsi) + " needs updating")

            print("Current value:", x)
            new_subscriber = create_v1_from_v0(x)
            print("Migrated value:", new_subscriber)

            #Write back to MongoDB
            myquery = { "imsi": str(imsi) }
            newvalues = {
                "$set": new_subscriber,
                "$unset": {"pdn": 1}
                }
            mycol.update_one(myquery, newvalues)
            print("Updated OK")


def create_v1_from_v0(old_sub):
    """Create a v1 subscriber from an existing v0 subscriber
    """
    # Make a copy to avoid mutating the existing subscriber object so it can be
    # re-used for other parts of the migration.
    new_sub = copy.deepcopy(old_sub)

    # Remove old PDN info
    del new_sub['pdn']

    # Set AMBR Values to new format (Old format is in kbps per second)
    new_sub['ambr']['uplink'] = {}
    new_sub['ambr']['uplink']['value'] = old_sub['ambr']['uplink']
    new_sub['ambr']['uplink']['unit'] = 1

    new_sub['ambr']['downlink'] = {}
    new_sub['ambr']['downlink']['value'] = old_sub['ambr']['downlink']
    new_sub['ambr']['downlink']['unit'] = 1

    #Propogate APN / DDN Slice Details
    new_sub['slice'] = []
    new_sub['slice'].append({"sst": 1, "default_indicator" : True, "session" : []})

    for pdn_entry in old_sub["pdn"]:
        session = _create_session_from_pdn(pdn_entry)
        new_sub['slice'][0]['session'].append(session)

    #Add "schema_version" feild
    new_sub['schema_version'] = 1

    return new_sub


def _create_session_from_pdn(pdn):
    """Builds a new session object from an existing PDN"""
    session = {}
    session['name'] = pdn['apn']

    if pdn['type'] in {1, 2, 3}:
        session['type'] = pdn['type']
    else:
        # Default to IPv4 for old networks being upgraded with an invalid type
        session['type'] = 1

    session['ambr'] = {
        "uplink": {
            "value": pdn['ambr']['uplink'],
            "unit": 1
        },
        "downlink": {
            "value": pdn['ambr']['downlink'],
            "unit": 1
        }
    }

    if "qos" in pdn:
        session["qos"] = {
            "index": pdn["qos"]["qci"],
            "arp": pdn["qos"]["arp"]
        }
    if "smf" in pdn:
        session["smf"] = pdn["smf"]
    if "ue" in pdn:
        session["ue"] = pdn["ue"]

    if ("pcc_rule" in pdn) and (len(pdn['pcc_rule']) != 0):
        raise NotImplementedError("PCC Rule Migration Not Implemented")
    else:
        session["pcc_rule"] = []

    return session


if __name__ == "__main__":
    myclient = pymongo.MongoClient("mongodb://localhost:27017/")
    mydb = myclient["open5gs"]

    migrate_all_subscribers(mycol=mydb["subscribers"])
