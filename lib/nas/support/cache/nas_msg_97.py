ies = []
ies.append({ "iei" : "43", "value" : "Full name for network", "type" : "Network name", "reference" : "9.9.3.24", "presence" : "O", "format" : "TLV", "length" : "3-n"})
ies.append({ "iei" : "45", "value" : "Short name for network", "type" : "Network name", "reference" : "9.9.3.24", "presence" : "O", "format" : "TLV", "length" : "3-n"})
ies.append({ "iei" : "46", "value" : "Local time zone", "type" : "Time zone", "reference" : "9.9.3.29", "presence" : "O", "format" : "TV", "length" : "2"})
ies.append({ "iei" : "47", "value" : "Universal time and local time zone", "type" : "Time zone and time", "reference" : "9.9.3.30", "presence" : "O", "format" : "TV", "length" : "8"})
ies.append({ "iei" : "49", "value" : "Network daylight saving time", "type" : "Daylight saving time", "reference" : "9.9.3.6", "presence" : "O", "format" : "TLV", "length" : "3"})
msg_list[key]["ies"] = ies
