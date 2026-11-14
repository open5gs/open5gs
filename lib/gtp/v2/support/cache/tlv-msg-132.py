ies = []
ies.append({ "ie_type" : "Cause", "ie_value" : "Cause", "presence" : "M", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Indication", "ie_value" : "Indication flags", "presence" : "C", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "Forwarding F-TEID", "presence" : "CO", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Bearer Context", "ie_value" : "Bearer Contexts", "presence" : "CO", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Node Number", "ie_value" : "SGSN Number", "presence" : "CO", "instance" : "0", "comment" : ""})
type_list["Node Number"]["max_instance"] = "1"
ies.append({ "ie_type" : "Node Number", "ie_value" : "MME number for MT SMS", "presence" : "CO", "instance" : "1", "comment" : ""})
ies.append({ "ie_type" : "Node Identifier", "ie_value" : "SGSN Identifier for MT SMS", "presence" : "CO", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Node Identifier", "ie_value" : "MME Identifier for MT SMS", "presence" : "CO", "instance" : "1", "comment" : ""})
msg_list[key]["ies"] = ies
