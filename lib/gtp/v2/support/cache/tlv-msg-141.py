ies = []
ies.append({ "ie_type" : "F-Container", "ie_value" : "E-UTRAN Transparent Container / EN-DC Container / Inter-system SON Container", "presence" : "M", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Target Identification", "ie_value" : "Target eNodeB ID / en-gNB ID / gnB ID", "presence" : "M", "instance" : "0", "comment" : ""})
type_list["Target Identification"]["max_instance"] = "1"
ies.append({ "ie_type" : "Target Identification", "ie_value" : "Connected Target eNodeB ID", "presence" : "CO", "instance" : "1", "comment" : ""})
msg_list[key]["ies"] = ies
