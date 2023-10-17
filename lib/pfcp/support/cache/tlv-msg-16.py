ies = []
ies.append({ "ie_type" : "Node ID", "ie_value" : "Node ID", "presence" : "M", "tlv_more" : "0", "comment" : "This IE shall contain the node identity of the originating node of the message."})
ies.append({ "ie_type" : "PFCP Session Change Info", "ie_value" : "PFCP Session Change Info", "presence" : "M", "tlv_more" : "0", "comment" : "This IE shall identify the FQ-CSID(s), Group Id(s) or CP IP address(es) of the PFCP sessions for which the PGW-U/UPF shall send subsequent PFCP Session Report Request messages to an Alternative SMF/PGW-C IP Address.Several IEs with the same IE type may be present to request the PGW-U/UPF to move PFCP sessions associated with different FQ-CSIDs, Group Ids or CP IP addresses to different PGW-C/SMFs."})
msg_list[key]["ies"] = ies
