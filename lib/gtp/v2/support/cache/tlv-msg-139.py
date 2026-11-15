ies = []
ies.append({ "ie_type" : "IMSI", "ie_value" : "IMSI", "presence" : "C", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "MEI", "ie_value" : "ME Identity", "presence" : "C", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Indication", "ie_value" : "Indication Flags", "presence" : "CO", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "F-Cause", "ie_value" : "RANAP Cause", "presence" : "C", "instance" : "0", "comment" : ""})
msg_list[key]["ies"] = ies
