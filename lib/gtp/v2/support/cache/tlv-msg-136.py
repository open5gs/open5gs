ies = []
ies.append({ "ie_type" : "Cause", "ie_value" : "Cause", "presence" : "M", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Recovery", "ie_value" : "Recovery", "presence" : "O", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Secondary RAT Usage Data Report", "ie_value" : "Secondary RAT Usage Data Report", "presence" : "CO", "instance" : "0", "comment" : ""})
type_list["Secondary RAT Usage Data Report"]["max_instance"] = "1"
ies.append({ "ie_type" : "Secondary RAT Usage Data Report", "ie_value" : "Secondary RAT Usage Data Report from NG-RAN", "presence" : "CO", "instance" : "1", "comment" : ""})
msg_list[key]["ies"] = ies
