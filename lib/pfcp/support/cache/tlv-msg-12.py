ies = []
ies.append({ "ie_type" : "Node ID", "ie_value" : "Node ID", "presence" : "M", "tlv_more" : "0", "comment" : "This IE shall contain the unique identifier of the sending Node."})
ies.append({ "ie_type" : "Node Report Type", "ie_value" : "Node Report Type", "presence" : "M", "tlv_more" : "0", "comment" : "This IE shall indicate the type of the report."})
ies.append({ "ie_type" : "User Plane Path Failure Report", "ie_value" : "User Plane Path Failure Report", "presence" : "C", "tlv_more" : "0", "comment" : "This IE shall be present if the Node Report Type indicates a User Plane Path Failure Report."})
ies.append({ "ie_type" : "User Plane Path Recovery Report", "ie_value" : "User Plane Path Recovery Report", "presence" : "C", "tlv_more" : "0", "comment" : "This IE shall be present if the Node Report Type indicates a User Plane Path Recovery Report."})
ies.append({ "ie_type" : "Clock Drift Report", "ie_value" : "Clock Drift Report", "presence" : "C", "tlv_more" : "0", "comment" : "This IE shall be present if the Node Report Type indicates a Clock Drift Report.More than one IE with this type may be included to send Clock Drift Reports for different TSN Time Domain Numbers."})
ies.append({ "ie_type" : "GTP-U Path QoS Report PFCP Node Report Request", "ie_value" : "GTP-U Path QoS Report", "presence" : "C", "tlv_more" : "0", "comment" : "This IE shall be present if the Node Report Type indicates a GTP-U Path QoS Report.More than one IE with this type may be included to represent multiple remote GTP-U peers for which QoS information is reported."})
msg_list[key]["ies"] = ies
