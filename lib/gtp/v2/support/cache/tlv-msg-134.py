ies = []
ies.append({ "ie_type" : "Cause", "ie_value" : "Cause", "presence" : "M", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "Sender's F-TEID for Control Plane", "presence" : "C", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Indication", "ie_value" : "Indication Flags", "presence" : "C", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Bearer Context", "ie_value" : "List of Set-up Bearers", "presence" : "C", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Bearer Context", "ie_value" : "List of Set-up RABs", "presence" : "C", "instance" : "1", "comment" : ""})
type_list["Bearer Context"]["max_instance"] = "2"
ies.append({ "ie_type" : "Bearer Context", "ie_value" : "List of Set-up PFCs", "presence" : "O", "instance" : "2", "comment" : ""})
ies.append({ "ie_type" : "F-Cause", "ie_value" : "S1-AP Cause", "presence" : "C", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "F-Cause", "ie_value" : "RANAP Cause", "presence" : "C", "instance" : "1", "comment" : ""})
ies.append({ "ie_type" : "F-Cause", "ie_value" : "BSSGP Cause", "presence" : "C", "instance" : "2", "comment" : ""})
ies.append({ "ie_type" : "F-Container", "ie_value" : "E-UTRAN Transparent Container", "presence" : "C", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "F-Container", "ie_value" : "UTRAN Transparent Container", "presence" : "C", "instance" : "1", "comment" : ""})
ies.append({ "ie_type" : "F-Container", "ie_value" : "BSS Container", "presence" : "C", "instance" : "2", "comment" : ""})
ies.append({ "ie_type" : "LDN", "ie_value" : "MME/S4-SGSN LDN", "presence" : "O", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "FQDN", "ie_value" : "SGSN node name", "presence" : "CO", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "FQDN", "ie_value" : "MME node name", "presence" : "CO", "instance" : "1", "comment" : ""})
ies.append({ "ie_type" : "Node Number", "ie_value" : "SGSN Number", "presence" : "O", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Node Identifier", "ie_value" : "SGSN Identifier", "presence" : "O", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Node Identifier", "ie_value" : "MME Identifier", "presence" : "O", "instance" : "1", "comment" : ""})
ies.append({ "ie_type" : "Node Number", "ie_value" : "MME number for MT SMS", "presence" : "CO", "instance" : "1", "comment" : ""})
type_list["Node Identifier"]["max_instance"] = "2"
ies.append({ "ie_type" : "Node Identifier", "ie_value" : "SGSN Identifier for MT-SMS", "presence" : "CO", "instance" : "2", "comment" : ""})
type_list["Node Identifier"]["max_instance"] = "3"
ies.append({ "ie_type" : "Node Identifier", "ie_value" : "MME Identifier for MT SMS", "presence" : "CO", "instance" : "3", "comment" : ""})
type_list["Bearer Context"]["max_instance"] = "3"
ies.append({ "ie_type" : "Bearer Context", "ie_value" : "List of Set-up Bearers for SCEF PDN Connections", "presence" : "CO", "instance" : "3", "comment" : ""})
ies.append({ "ie_type" : "SRVCC Cause", "ie_value" : "(v)SRVCC rejected Cause", "presence" : "CO", "instance" : "0", "comment" : ""})
type_list["Node Number"]["max_instance"] = "2"
ies.append({ "ie_type" : "Node Number", "ie_value" : "MSC Number", "presence" : "O", "instance" : "2", "comment" : ""})
ies.append({ "ie_type" : "NF Instance ID", "ie_value" : "AMF Identifier", "presence" : "O", "instance" : "0", "comment" : ""})
msg_list[key]["ies"] = ies
