ies = []
ies.append({ "ie_type" : "Node ID", "ie_value" : "Node ID", "presence" : "M", "tlv_more" : "0", "comment" : "This IE shall contain the unique identifier of the sending Node."})
ies.append({ "ie_type" : "Node Report Type", "ie_value" : "Node Report Type", "presence" : "M", "tlv_more" : "0", "comment" : "This IE shall indicate the type of the report."})
ies.append({ "ie_type" : "User Plane Path Failure Report", "ie_value" : "User Plane Path Failure Report", "presence" : "C", "tlv_more" : "0", "comment" : "This IE shall be present if the Node Report Type indicates a User Plane Path Failure Report."})
msg_list[key]["ies"] = ies
