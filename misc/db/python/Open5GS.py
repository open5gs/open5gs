import pymongo
import random
import bson

class Open5GS:
    def __init__(self, server, port):
        self.server = server
        self.port = port

    def GetSubscribers(self):
        myclient = pymongo.MongoClient("mongodb://" + str(self.server) + ":" + str(self.port) + "/")
        mydb = myclient["open5gs"]
        mycol = mydb["subscribers"]
        subs_list = []
        for x in mycol.find():
            print(x)
            subs_list.append(x)
            pass

        return subs_list

    def GetSubscriber(self, imsi):
        myclient = pymongo.MongoClient("mongodb://" + str(self.server) + ":" + str(self.port) + "/")
        mydb = myclient["open5gs"]
        mycol = mydb["subscribers"]
        myquery = { "imsi": str(imsi)}
        mydoc = mycol.find(myquery)
        for x in mydoc:
            print(x)
            return x

    def AddSubscriber(self, sub_data):
        myclient = pymongo.MongoClient("mongodb://" + str(self.server) + ":" + str(self.port) + "/")
        mydb = myclient["open5gs"]
        mycol = mydb["subscribers"]

        x = mycol.insert_one(sub_data)
        print("Added subscriber with Inserted ID : " + str(x.inserted_id))
        return x.inserted_id

    def UpdateSubscriber(self, imsi, sub_data):
        myclient = pymongo.MongoClient("mongodb://" + str(self.server) + ":" + str(self.port) + "/")
        mydb = myclient["open5gs"]
        mycol = mydb["subscribers"]
        print("Attempting to update IMSI " + str(imsi))
        newvalues = { "$set": sub_data }
        myquery = { "imsi": str(imsi)}
        x = mycol.update_one(myquery, newvalues)
        print(x)
        return True

    def DeleteSubscriber(self, imsi):
        myclient = pymongo.MongoClient("mongodb://" + str(self.server) + ":" + str(self.port) + "/")
        mydb = myclient["open5gs"]
        mycol = mydb["subscribers"]
        myquery = { "imsi": str(imsi)}
        x = mycol.delete_many(myquery)
        print(x.deleted_count, " subscribers deleted.")
        return x.deleted_count
