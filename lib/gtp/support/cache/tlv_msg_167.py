ies = []
ies.append({ "ie_type" : "Cause", "ie_value" : "Cause", "presence" : "M", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "Sender F-TEID for Control Plane", "presence" : "C", "instance" : "0", "comment" : "This IE shall be included by an SGW if the SGW receives a Sender F-TEID for Control Plane IE from an MME/SGSN in a Create Indirect Data Forwarding Tunnel Request message.See also NOTE 1 in Table 7.2.18-1."})
ies.append({ "ie_type" : "Bearer Context", "ie_value" : "Bearer Context 0", "presence" : "M", "instance" : "0", "comment" : "Several IEs with this type and instance value may be included as necessary to represent a list of Bearers"})
ies.append({ "ie_type" : "Bearer Context", "ie_value" : "Bearer Context 1", "presence" : "O", "instance" : "1", "comment" : "Several IEs with this type and instance value may be included as necessary to represent a list of Bearers"})
type_list["Bearer Context"]["max_instance"] = "2"
ies.append({ "ie_type" : "Bearer Context", "ie_value" : "Bearer Context 2", "presence" : "O", "instance" : "2", "comment" : "Several IEs with this type and instance value may be included as necessary to represent a list of Bearers"})
type_list["Bearer Context"]["max_instance"] = "3"
ies.append({ "ie_type" : "Bearer Context", "ie_value" : "Bearer Context 3", "presence" : "O", "instance" : "3", "comment" : "Several IEs with this type and instance value may be included as necessary to represent a list of Bearers"})
type_list["Bearer Context"]["max_instance"] = "4"
ies.append({ "ie_type" : "Bearer Context", "ie_value" : "Bearer Context 4", "presence" : "O", "instance" : "4", "comment" : "Several IEs with this type and instance value may be included as necessary to represent a list of Bearers"})
type_list["Bearer Context"]["max_instance"] = "5"
ies.append({ "ie_type" : "Bearer Context", "ie_value" : "Bearer Context 5", "presence" : "O", "instance" : "5", "comment" : "Several IEs with this type and instance value may be included as necessary to represent a list of Bearers"})
type_list["Bearer Context"]["max_instance"] = "6"
ies.append({ "ie_type" : "Bearer Context", "ie_value" : "Bearer Context 6", "presence" : "O", "instance" : "6", "comment" : "Several IEs with this type and instance value may be included as necessary to represent a list of Bearers"})
type_list["Bearer Context"]["max_instance"] = "7"
ies.append({ "ie_type" : "Bearer Context", "ie_value" : "Bearer Context 7", "presence" : "O", "instance" : "7", "comment" : "Several IEs with this type and instance value may be included as necessary to represent a list of Bearers"})
type_list["Bearer Context"]["max_instance"] = "8"
ies.append({ "ie_type" : "Bearer Context", "ie_value" : "Bearer Context 8", "presence" : "O", "instance" : "8", "comment" : "Several IEs with this type and instance value may be included as necessary to represent a list of Bearers"})
type_list["Bearer Context"]["max_instance"] = "9"
ies.append({ "ie_type" : "Bearer Context", "ie_value" : "Bearer Context 9", "presence" : "O", "instance" : "9", "comment" : "Several IEs with this type and instance value may be included as necessary to represent a list of Bearers"})
type_list["Bearer Context"]["max_instance"] = "10"
ies.append({ "ie_type" : "Bearer Context", "ie_value" : "Bearer Context 10", "presence" : "O", "instance" : "10", "comment" : "Several IEs with this type and instance value may be included as necessary to represent a list of Bearers"})
ies.append({ "ie_type" : "Recovery", "ie_value" : "Recovery", "presence" : "CO", "instance" : "0", "comment" : "This IE shall be included if contacting the peer for the first time"})
msg_list[key]["ies"] = ies
