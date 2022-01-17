ies = []
ies.append({ "ie_type" : "Cause", "ie_value" : "Cause", "presence" : "M", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "Sender F-TEID for Control Plane", "presence" : "C", "instance" : "0", "comment" : "This IE shall be included by an SGW if the SGW receives a Sender F-TEID for Control Plane IE from an MME/SGSN in a Create Indirect Data Forwarding Tunnel Request message.See also NOTE 1 in Table 7.2.18-1."})
ies.append({ "ie_type" : "Bearer Context", "ie_value" : "Bearer Contexts", "presence" : "M", "instance" : "0", "comment" : "Several IEs with this type and instance value may be included as necessary to represent a list of Bearers"})
ies.append({ "ie_type" : "Recovery", "ie_value" : "Recovery", "presence" : "CO", "instance" : "0", "comment" : "This IE shall be included if contacting the peer for the first time"})
msg_list[key]["ies"] = ies
