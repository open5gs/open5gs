ies = []
ies.append({ "ie_type" : "GUTI", "ie_value" : "GUTI", "presence" : "C", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "ULI", "ie_value" : "Routeing Area Identity", "presence" : "C", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "P-TMSI", "ie_value" : "Packet TMSI", "presence" : "C", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "P-TMSI Signature", "ie_value" : "P-TMSI Signature", "presence" : "C", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Complete Request Message", "ie_value" : "Complete Attach Request Message", "presence" : "C", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "IP Address", "ie_value" : "Address for Control Plane", "presence" : "O", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Port Number", "ie_value" : "UDP Source Port Number", "presence" : "C", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Hop Counter", "ie_value" : "Hop Counter", "presence" : "O", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Serving Network", "ie_value" : "Target PLMN ID", "presence" : "CO", "instance" : "0", "comment" : ""})
msg_list[key]["ies"] = ies
