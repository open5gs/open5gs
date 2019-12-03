ies = []
ies.append({ "ie_type" : "Cause", "ie_value" : "Cause", "presence" : "M", "instance" : "0", "comment" : "This IE shall indicate the acceptance or the rejection of the corresponding request message."})
ies.append({ "ie_type" : "Offending IE", "ie_value" : "Offending IE", "presence" : "C", "instance" : "0", "comment" : "This IE shall be included if the rejection is due to an conditional or mandatory IE missing or faulty."})
ies.append({ "ie_type" : "Update BAR PFCP Session Report Response", "ie_value" : "Update BAR", "presence" : "C", "instance" : "0", "comment" : "This IE shall be present if a BAR previously created for the PFCP session needs to be modified. A previously created BAR that is not modified shall not be included. See Table 7.5.9.2-1."})
ies.append({ "ie_type" : "PFCPSRRsp-Flags", "ie_value" : "PFCPSRRsp-Flags", "presence" : "C", "instance" : "0", "comment" : "This IE shall be included if at least one of the flags is set to 1."})
msg_list[key]["ies"] = ies
