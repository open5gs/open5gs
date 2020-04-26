ies = []
ies.append({ "ie_type" : "Node ID", "ie_value" : "Node ID", "presence" : "M", "tlv_more" : "0", "comment" : "This IE shall contain the node identity of the originating node of the message."})
ies.append({ "ie_type" : "FQ-CSID", "ie_value" : "SGW-C FQ-CSID", "presence" : "C", "tlv_more" : "0", "comment" : "This IE shall be included according to the requirements in clause 23 of 3GPPTS23.007[24]."})
ies.append({ "ie_type" : "FQ-CSID", "ie_value" : "PGW-C FQ-CSID", "presence" : "C", "tlv_more" : "0", "comment" : "This IE shall be included according to the requirements in clause 23 of 3GPPTS23.007[24]."})
ies.append({ "ie_type" : "FQ-CSID", "ie_value" : "SGW-U FQ-CSID", "presence" : "C", "tlv_more" : "0", "comment" : "This IE shall be included according to the requirements in clause 23 of 3GPPTS23.007[24]."})
ies.append({ "ie_type" : "FQ-CSID", "ie_value" : "PGW-U FQ-CSID", "presence" : "C", "tlv_more" : "0", "comment" : "This IE shall be included according to the requirements in clause 23 of 3GPPTS23.007[24]."})
ies.append({ "ie_type" : "FQ-CSID", "ie_value" : "TWAN FQ-CSID", "presence" : "C", "tlv_more" : "0", "comment" : "This IE shall be included according to the requirements in clause 23 of 3GPPTS23.007[24]."})
ies.append({ "ie_type" : "FQ-CSID", "ie_value" : "ePDG FQ-CSID", "presence" : "C", "tlv_more" : "0", "comment" : "This IE shall be included according to the requirements in clause 23 of 3GPPTS23.007[24]."})
ies.append({ "ie_type" : "FQ-CSID", "ie_value" : "MME FQ-CSID", "presence" : "C", "tlv_more" : "0", "comment" : "This IE shall be included according to the requirements in clause 23 of 3GPPTS23.007[24]."})
msg_list[key]["ies"] = ies
