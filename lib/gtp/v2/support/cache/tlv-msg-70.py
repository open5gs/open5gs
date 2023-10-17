ies = []
ies.append({ "ie_type" : "Cause", "ie_value" : "Cause", "presence" : "M", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Node Type", "ie_value" : "Originating Node", "presence" : "CO", "instance" : "0", "comment" : "This IE shall be included on the S4/S11 interface if the ISR associated GTP entities i.e. MME, S4-SGSN, send this message to the SGW during the Network Triggered Service Request procedure to denote the type of the node originating the message."})
ies.append({ "ie_type" : "IMSI", "ie_value" : "IMSI", "presence" : "CO", "instance" : "0", "comment" : "3GPPTS23.007[17] specifies conditions for sending this IE on the S11/S4 interface as part of the network triggered service restoration procedure, if both the SGW and the MME/S4-SGSN support this optional feature."})
msg_list[key]["ies"] = ies
