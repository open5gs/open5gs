ies = []
ies.append({ "ie_type" : "Node ID", "ie_value" : "Node ID", "presence" : "M", "instance" : "0", "comment" : "This IE shall contain the node identity of the originating node of the message."})
ies.append({ "ie_type" : "FQ-CSIDp", "ie_value" : "SGW-C FQ-CSID", "presence" : "C", "instance" : "0", "comment" : "This IE shall be included according to the requirements in clause 23 of 3GPP TS 23.007 [24]."})
msg_list[key]["ies"] = ies
