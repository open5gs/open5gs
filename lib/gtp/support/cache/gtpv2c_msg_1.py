ies = []
ies.append({ "ie_type" : "Recovery", "ie_value" : "Recovery", "presence" : "M", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Node Features", "ie_value" : "Sending Node Features", "presence" : "CO", "instance" : "0", "comment" : "This IE shall be sent towards a peer node on any GTPv2 interface if the sending node supports at least one feature on this interface or if the sending node supports at least one feature and does not know the interface type towards the peer node. This IE may be present otherwise. "})
msg_list[key]["ies"] = ies
