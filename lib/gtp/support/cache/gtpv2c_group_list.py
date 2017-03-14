type_list["Bearer Context"] = { "type" : "93" }
group = []
group.append({ "ie_type" : "EBI", "ie_value" : "EPS Bearer ID", "presence" : "M", "instance" : "0", "comment" : ""})
group.append({ "ie_type" : "Bearer TFT", "ie_value" : "TFT", "presence" : "O", "instance" : "0", "comment" : "This IE may be included on the S4/S11 interfaces."})
group.append({ "ie_type" : "F-TEID", "ie_value" : "S1-U eNodeB F-TEID", "presence" : "C", "instance" : "0", "comment" : "This IE shall be included on the S11 interface for X2-based handover with SGW relocation."})
group.append({ "ie_type" : "F-TEID", "ie_value" : "S4-U SGSN F-TEID", "presence" : "C", "instance" : "1", "comment" : "This IE shall be included on the S4 interface if the S4-U interface is used."})
group.append({ "ie_type" : "F-TEID", "ie_value" : "S5/S8-U SGW F-TEID", "presence" : "C", "instance" : "2", "comment" : "This IE shall be included on the S5/S8 interface for an E-UTRAN Initial Attach, a Handover from Trusted or Untrusted Non-3GPP IP Access to E-UTRAN, a PDP Context Activation, a Handover from Trusted or Untrusted Non-3GPP IP Access to UTRAN/GERAN or a UE Requested PDN Connectivity."})
group.append({ "ie_type" : "F-TEID", "ie_value" : "S5/S8-U PGW F-TEID", "presence" : "C", "instance" : "3", "comment" : "This IE shall be included on the S4 and S11 interfaces for the TAU/RAU/Handover cases when the GTP-based S5/S8 is used."})
group.append({ "ie_type" : "F-TEID", "ie_value" : "S5/S8-U PGW F-TEID", "presence" : "CO", "instance" : "3", "comment" : "For PMIP-based S5/S8, this IE shall be included on the S11/S4 interface for the TAU/RAU/Handover cases if the PGW provided an alternate address for user plane, i.e. an IP address for user plane which is different from the IP address for control plane.When present, this IE shall contain the alternate IP address for user plane and the uplink GRE key.See NOTE 1."})
group.append({ "ie_type" : "F-TEID", "ie_value" : "S12 RNC F-TEID", "presence" : "CO", "instance" : "4", "comment" : "This IE shall be included on the S4 interface if the S12 interface is used in the Enhanced serving RNS relocation with SGW relocation procedure."})
group.append({ "ie_type" : "F-TEID", "ie_value" : "S2b-U ePDG F-TEID", "presence" : "C", "instance" : "5", "comment" : "This IE shall be included on the S2b interface for an Attach with GTP on S2b, a UE initiated Connectivity to Additional PDN with GTP on S2b, a Handover to Untrusted Non-3GPP IP Access with GTP on S2b and an Initial Attach for emergency session (GTP on S2b)."})
group.append({ "ie_type" : "F-TEID", "ie_value" : "S2a-U TWAN F-TEID", "presence" : "C", "instance" : "6", "comment" : "This IE shall be included on the S2a interface for an Initial Attach in WLAN on GTP S2a, a UE initiated Connectivity to Additional PDN with GTP on S2a and a Handover to TWAN with GTP on S2a."})
group.append({ "ie_type" : "Bearer QoS", "ie_value" : "Bearer Level QoS", "presence" : "M", "instance" : "0", "comment" : ""})
group.append({ "ie_type" : "F-TEID", "ie_value" : "S11-U MME F-TEID", "presence" : "CO", "instance" : "7", "comment" : "This IE shall be sent on the S11 interface, if S11-U is being used, during the E-UTRAN Initial Attach and UE requested PDN connectivity procedures. This IE may also be sent on the S11 interface, if S11-U is being used, during a Tracking Area Update procedure with Serving GW change, if the MME needs to establish the S11-U tunnel. See NOTE 2."})
group_list["Bearer Context"] = { "group" : group }
type_list["Overload Control Information"] = { "type" : "180" }
group = []
group.append({ "ie_type" : "Sequence Number", "ie_value" : "Overload Control Sequence Number", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.1 for the description and use of this parameter."})
group.append({ "ie_type" : "Metric", "ie_value" : "Overload Reduction Metric", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.3 for the description and use of this parameter."})
group.append({ "ie_type" : "EPC Timer", "ie_value" : "Period of Validity", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.2 for the description and use of this parameter.This IE should be set to 0 if the Overload Reduction Metric is null. This IE shall be ignored by the receiver if the Overload Reduction Metric is null."})
group_list["Overload Control Information"] = { "group" : group }
type_list["Remote UE Context"] = { "type" : "191" }
group = []
group.append({ "ie_type" : "Remote User ID", "ie_value" : "Remote User ID", "presence" : "M", "instance" : "0", "comment" : "See subclause 8.123 for the description and use of this parameter"})
group.append({ "ie_type" : "Remote UE IP Information", "ie_value" : "Remote UE IP Information", "presence" : "M", "instance" : "0", "comment" : "See subclause 8.124 for the description and use of this parameter"})
group_list["Remote UE Context"] = { "group" : group }
group.append({ "ie_type" : "Cause", "ie_value" : "Cause", "presence" : "M", "instance" : "0", "comment" : "This IE shall indicate if the bearer handling was successful, and if not, it gives information on the reason. (NOTE 1, NOTE 2, NOTE 3)"})
group.append({ "ie_type" : "Charging ID", "ie_value" : "Charging Id", "presence" : "C", "instance" : "0", "comment" : "This IE shall be included on the S5/S8 interface for an E-UTRAN initial attach, a Handover from Trusted or Untrusted Non-3GPP IP Access to E-UTRAN, a PDP Context Activation, a Handover from Trusted or Untrusted Non-3GPP IP Access to UTRAN/GERAN and a UE requested PDN connectivity."})
group.append({ "ie_type" : "Bearer Flags", "ie_value" : "Bearer Flags", "presence" : "O", "instance" : "0", "comment" : "Applicable flags are:PPC (Prohibit Payload Compression) : this flag may be set on the S5/S8 and S4 interfaces."})
group_list["Bearer Context"] = { "group" : group }
group.append({ "ie_type" : "EBI", "ie_value" : "EPS Bearer ID", "presence" : "M", "instance" : "0", "comment" : ""})
group_list["Bearer Context"] = { "group" : group }
type_list["Load Control Information"] = { "type" : "181" }
group = []
group.append({ "ie_type" : "Sequence Number", "ie_value" : "Load Control Sequence Number", "presence" : "M", "instance" : "0", "comment" : "See clause 12.2.5.1.2.1 for the description and use of this parameter."})
group.append({ "ie_type" : "Metric", "ie_value" : "Load Metric", "presence" : "M", "instance" : "0", "comment" : "See clauses 12.2.5.1.2.2 and 12.2.5.1.2.3 for the description and use of this parameter."})
group.append({ "ie_type" : "APN and Relative Capacity", "ie_value" : "List of APN and Relative Capacity", "presence" : "CO", "instance" : "0", "comment" : "The IE shall (only) be present in the PGWs APN level Load Control Information IE.For indicating the APN level load, the PGW shall include one or more instances of this IE, up to maximum of 10, with the same type and instance value, representing a list of APN(s) & its respective Relative Capacity (sharing the same Load Metric).See clause 12.2.5.1.2.3 for the description and use of this parameter.See NOTE 1."})
group_list["Load Control Information"] = { "group" : group }
group.append({ "ie_type" : "APN", "ie_value" : "List of Access Point Name (APN)", "presence" : "CO", "instance" : "0", "comment" : "The IE may (only) be present in the PGWs Overload Control Information IE.For indicating the APN level overload, the PGW shall include one or more instances of this IE, up to maximum of 10, with the same type and instance value, representing a list of APN(s) (sharing the same Overload Reduction Metric and Period of Validity). See NOTE 1."})
group_list["Overload Control Information"] = { "group" : group }
group.append({ "ie_type" : "Bearer TFT", "ie_value" : "TFT", "presence" : "M", "instance" : "0", "comment" : "This IE can contain both uplink and downlink packet filters to be sent to the UE or the TWAN/ePDG."})
group.append({ "ie_type" : "F-TEID", "ie_value" : "S1-U SGW F-TEID", "presence" : "C", "instance" : "0", "comment" : "This IE shall be sent on the S11 interface if the S1-U interface is used. If SGW supports both IPv4 and IPv6, it shall send both an IPv4 address and an IPv6 address within the S1-U SGW F-TEID IE.See NOTE 1."})
group.append({ "ie_type" : "F-TEID", "ie_value" : "S5/8-U PGW F-TEID", "presence" : "C", "instance" : "1", "comment" : "This IE shall be sent on the S4, S5/S8 and S11 interfaces for GTP-based S5/S8 interface. The MME/SGSN shall ignore the IE on S11/S4 for PMIP-based S5/S8 interface."})
group.append({ "ie_type" : "F-TEID", "ie_value" : "S12 SGW F-TEID", "presence" : "C", "instance" : "2", "comment" : "This IE shall be sent on the S4 interface if the S12 interface is used. See NOTE 1."})
group.append({ "ie_type" : "F-TEID", "ie_value" : "S4-U SGW F-TEID", "presence" : "C", "instance" : "3", "comment" : "This IE shall be sent on the S4 interface if the S4-U interface is used. See NOTE 1."})
group.append({ "ie_type" : "F-TEID", "ie_value" : "S2b-U PGW F-TEID", "presence" : "C", "instance" : "4", "comment" : "This IE (for user plane) shall be sent on the S2b interface."})
group.append({ "ie_type" : "F-TEID", "ie_value" : "S2a-U PGW F-TEID", "presence" : "C", "instance" : "5", "comment" : "This IE (for user plane) shall be sent on the S2a interface."})
group.append({ "ie_type" : "Bearer QoS", "ie_value" : "Bearer Level QoS", "presence" : "M", "instance" : "0", "comment" : ""})
group.append({ "ie_type" : "PCO", "ie_value" : "Protocol Configuration Options (PCO)", "presence" : "O", "instance" : "0", "comment" : "This IE may be sent on the S5/S8 and S4/S11 interfaces if ePCO is not supported by the UE or the network. This bearer level IE takes precedence over the PCO IE in the message body if they both exist."})
group.append({ "ie_type" : "ePCO", "ie_value" : "Extended Protocol Configuration Options (ePCO)", "presence" : "O", "instance" : "0", "comment" : "This IE may be sent on the S5/S8 and S11 interfaces if the UE and the network support ePCO."})
group_list["Bearer Context"] = { "group" : group }
group.append({ "ie_type" : "EPC Timer", "ie_value" : "Period of Validity", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.2 for the description and use of this parameter.This IE should be set to 0 if the Overload Reduction Metric is null. This IE shall be ignored by the receiver if the Overload Reduction Metric is null."})
group_list["Overload Control Information"] = { "group" : group }
group.append({ "ie_type" : "EBI", "ie_value" : "EPS Bearer ID", "presence" : "M", "instance" : "0", "comment" : ""})
group.append({ "ie_type" : "Cause", "ie_value" : "Cause", "presence" : "M", "instance" : "0", "comment" : "This IE shall indicate if the bearer handling was successful, and if not, it gives information on the reason."})
group.append({ "ie_type" : "F-TEID", "ie_value" : "S4-U SGSN F-TEID", "presence" : "C", "instance" : "6", "comment" : "This IE shall be sent on the S4 interface if the S4-U interface is used. See NOTE1."})
group.append({ "ie_type" : "F-TEID", "ie_value" : "S4-U SGW F-TEID", "presence" : "C", "instance" : "7", "comment" : "This IE shall be sent on the S4 interface. It shall be used to correlate the bearers with those in the Create Bearer Request. See NOTE1."})
group.append({ "ie_type" : "F-TEID", "ie_value" : "S2b-U ePDG F-TEID", "presence" : "C", "instance" : "8", "comment" : "This IE shall be sent on the S2b interface."})
group.append({ "ie_type" : "F-TEID", "ie_value" : "S2b-U PGW F-TEID", "presence" : "C", "instance" : "9", "comment" : "This IE shall be sent on the S2b interface. It shall be used to correlate the bearers with those in the Create Bearer Request."})
group.append({ "ie_type" : "F-TEID", "ie_value" : "S2a-U TWAN F-TEID", "presence" : "C", "instance" : "10", "comment" : "This IE shall be sent on the S2a interface."})
group.append({ "ie_type" : "F-TEID", "ie_value" : "S2a-U PGW F-TEID", "presence" : "C", "instance" : "11", "comment" : "This IE shall be sent on the S2a interface. It shall be used to correlate the bearers with those in the Create Bearer Request."})
group.append({ "ie_type" : "RAN/NAS Cause", "ie_value" : "RAN/NAS Cause", "presence" : "CO", "instance" : "0", "comment" : "If the bearer creation failed, the MME shall include this IE on the S11 interface to indicate the RAN cause and/or the NAS cause of the bearer creation failure, if available and if this information is permitted to be sent to the PGW operator according to MME operators policy. If both a RAN cause and a NAS cause are generated, then several IEs with the same type and instance value shall be included to represent a list of causes.The SGW shall include this IE on the S5/S8 interface if it receives it from the MME."})
group_list["Bearer Context"] = { "group" : group }
group.append({ "ie_type" : "Charging ID", "ie_value" : "Charging ID", "presence" : "C", "instance" : "0", "comment" : "This IE shall be present on the S5/S8 interface if this message is triggered due to one of the following procedures:TAU/RAU/HO with SGW relocationTAU/RAU/HO from Gn/Gp SGSN to MME/S4-SGSN   "})
group.append({ "ie_type" : "Bearer Flags", "ie_value" : "Bearer Flags", "presence" : "CO", "instance" : "0", "comment" : "Applicable flags are:PPC (Prohibit Payload Compression): This flag shall be sent on the S5/S8 and the S4 interfaces at S4-SGSN relocation."})
group_list["Bearer Context"] = { "group" : group }
group.append({ "ie_type" : "APCO", "ie_value" : "Additional Protocol Configuration Options (APCO)", "presence" : "CO", "instance" : "0", "comment" : "The PGW shall include the Additional Prococol Configuration Options (APCO) IE on the S2b interface, including the list of available P-CSCF addresses, as part of the P-CSCF restoration extension procedure for the untrusted WLAN access, as specified in 3GPP TS 23.380 [61]."})
group_list["Bearer Context"] = { "group" : group }
type_list["PDN Connection"] = { "type" : "109" }
group = []
group_list["PDN Connection"] = { "group" : group }
group.append({ "ie_type" : "F-Container", "ie_value" : "BSS Container", "presence" : "CO", "instance" : "0", "comment" : "The MME/S4 SGSN shall include the Packet Flow ID, Radio Priority, SAPI, PS Handover XID parameters in the TAU/RAU/Handover procedure, if available. See Figure 8.48-2. The Container Type shall be set to 2."})
group.append({ "ie_type" : "TI", "ie_value" : "Transaction Identifier", "presence" : "C", "instance" : "0", "comment" : "This IE shall be sent over S3/S10/S16 if the UE supports A/Gb and/or Iu mode."})
group_list["Bearer Context"] = { "group" : group }
type_list["SCEF PDN Connection"] = { "type" : "195" }
group = []
group.append({ "ie_type" : "APN", "ie_value" : "APN", "presence" : "M", "instance" : "0", "comment" : ""})
group.append({ "ie_type" : "EBI", "ie_value" : "Default EPS Bearer ID", "presence" : "M", "instance" : "0", "comment" : "This IE shall identify the default bearer of the SCEF PDN Connection."})
group.append({ "ie_type" : "Node Identifier", "ie_value" : "SCEF ID", "presence" : "M", "instance" : "0", "comment" : "This IE shall include the SCEF Identifier and the SCEF Realm for the APN."})
group_list["SCEF PDN Connection"] = { "group" : group }
group.append({ "ie_type" : "Packet Flow ID", "ie_value" : "Packet Flow ID", "presence" : "C", "instance" : "0", "comment" : "This IE shall be included if the message is used for PS handover and Inter RAT handover to/from A/Gb mode procedures."})
group.append({ "ie_type" : "F-TEID", "ie_value" : "eNodeB F-TEID for DL data forwarding", "presence" : "C", "instance" : "0", "comment" : "This IE shall be included for the message sent from the target MME, if the DL Transport Layer Address and DL GTP TEID are included in the SAE Bearers Admitted List of the S1AP: HANDOVER REQUEST ACKNOWLEDGE and direct forwarding or indirect forwarding without SGW change is applied."})
group.append({ "ie_type" : "F-TEID", "ie_value" : "eNodeB F-TEID for UL data forwarding", "presence" : "O", "instance" : "1", "comment" : "This IE may be included for the message sent from the target MME during the intra-EUTRAN HO, if the UL Transport Layer Address and UL GTP TEID are included in the SAE Bearers Admitted List of the S1AP: HANDOVER REQUEST ACKNOWLEDGE and direct forwarding or indirect forwarding without SGW change is applied."})
group.append({ "ie_type" : "F-TEID", "ie_value" : "SGW F-TEID for DL data forwarding", "presence" : "C", "instance" : "2", "comment" : "This SGW F-TEID shall be included when indirect data forwarding with SGW change is applied."})
group.append({ "ie_type" : "F-TEID", "ie_value" : "RNC F-TEID for DL data forwarding", "presence" : "C", "instance" : "3", "comment" : "This RNC F-TEID shall be included in the message sent from SGSN, if the target system decides using RNC F-TEID for data forwarding."})
group.append({ "ie_type" : "F-TEID", "ie_value" : "SGSN F-TEID for DL data forwarding", "presence" : "C", "instance" : "4", "comment" : "This SGSN F-TEID shall be included in the message sent from SGSN, if the target system decides using SGSN F-TEID for data forwarding."})
group.append({ "ie_type" : "F-TEID", "ie_value" : "SGW F-TEID for UL data forwarding", "presence" : "O", "instance" : "5", "comment" : "If available this SGW F-TEID may be included when indirect data forwarding with SGW change is applied, during the intra-EUTRAN HO."})
group_list["Bearer Context"] = { "group" : group }
group.append({ "ie_type" : "Bearer TFT", "ie_value" : "TFT", "presence" : "C", "instance" : "0", "comment" : "This IE shall be present if a TFT is defined for this bearer."})
group.append({ "ie_type" : "Bearer QoS", "ie_value" : "Bearer Level QoS", "presence" : "M", "instance" : "0", "comment" : ""})
group.append({ "ie_type" : "F-Container", "ie_value" : "BSS Container", "presence" : "CO", "instance" : "0", "comment" : "The MME/S4 SGSN shall include the Packet Flow ID, Radio Priority, SAPI, PS Handover XID parameters in the TAU/RAU/Handover procedure, if available. The Container Type shall be set to 2."})
group.append({ "ie_type" : "TI", "ie_value" : "Transaction Identifier", "presence" : "C", "instance" : "0", "comment" : "This IE shall be sent over S3/S10/S16 if the UE supports A/Gb and/or Iu mode."})
group_list["Bearer Context"] = { "group" : group }
