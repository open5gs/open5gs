ies = []
ies.append({ "ie_type" : "Cause", "ie_value" : "Cause", "presence" : "M", "instance" : "0", "comment" : "This IE shall indicate if the deletion of indirect tunnel is successful, and if not, gives information on the reason."})
ies.append({ "ie_type" : "Recovery", "ie_value" : "Recovery", "presence" : "C", "instance" : "0", "comment" : "This IE shall be included if contacting the peer for the first time."})
msg_list[key]["ies"] = ies
