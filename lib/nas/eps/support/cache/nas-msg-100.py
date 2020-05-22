ies = []
ies.append({ "iei" : "", "value" : "Paging identity", "type" : "Paging identity", "reference" : "9.9.3.25A", "presence" : "M", "format" : "V", "length" : "1"})
ies.append({ "iei" : "60", "value" : "CLI", "type" : "CLI", "reference" : "9.9.3.38", "presence" : "O", "format" : "TLV", "length" : "3-14"})
ies.append({ "iei" : "61", "value" : "SS Code", "type" : "SS Code", "reference" : "9.9.3.39", "presence" : "O", "format" : "TV", "length" : "2"})
ies.append({ "iei" : "62", "value" : "LCS indicator", "type" : "LCS indicator", "reference" : "9.9.3.40", "presence" : "O", "format" : "TV", "length" : "2"})
ies.append({ "iei" : "63", "value" : "LCS client identity", "type" : "LCS client identity", "reference" : "9.9.3.41", "presence" : "O", "format" : "TLV", "length" : "3-257"})
msg_list[key]["ies"] = ies
