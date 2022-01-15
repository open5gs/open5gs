#!/bin/bash

version=0.9.1

display_help() {
    echo "open5gs-dbctl: Open5GS Database Configuration Tool ($version)"
	echo "FLAGS: --db_uri=mongodb://localhost"
    echo "COMMANDS:" >&2
    echo "   add {imsi key opc}: adds a user to the database with default values"
	echo "   add {imsi ip key opc}: adds a user to the database with default values and a IPv4 address for the UE"
	echo "   addT1 {imsi key opc}: adds a user to the database with 3 differents apns"
	echo "   addT1 {imsi ip key opc}: adds a user to the database with 3 differents apns and the same IPv4 address for the each apn"
    echo "   remove {imsi}: removes a user from the database"
    echo "   reset: WIPES OUT the database and restores it to an empty default"
    echo "   static_ip {imsi ip4}: adds a static IP assignment to an already-existing user"
    echo "   static_ip6 {imsi ip6}: adds a static IPv6 assignment to an already-existing user"
    echo "   type {imsi type}: changes the PDN-Type of the first PDN: 0 = IPv4, 1 = IPv6, 2 = IPv4v6, 3 = v4 OR v6"
    echo "   help: displays this message and exits"
    echo "   default values are as follows: APN \"internet\", dl_bw/ul_bw 1 Gbps, PGW address is 127.0.0.3, IPv4 only"
}

while test $# -gt 0; do
  case "$1" in
    --db_uri*)
      DB_URI=`echo $1 | sed -e 's/^[^=]*=//g'`
      shift
      ;;
    *)
      break
      ;;
  esac
done

DB_URI="${DB_URI:-mongodb://localhost/open5gs}"

if [ "$#" -lt 1 ]; then
	display_help
	exit 1
fi

if [ "$1" = "help" ]; then
	display_help
	exit 1
fi

if [ "$1" = "add" ]; then
	if [ "$#" -eq 4 ]; then
		IMSI=$2 
		KI=$3
		OPC=$4

		mongo --eval "db.subscribers.update( { \"imsi\" : \"$IMSI\" }, 
			{ \$setOnInsert: 
				{ 
					\"imsi\" : \"$IMSI\", 
					\"subscribed_rau_tau_timer\" : NumberInt(12), 
					\"network_access_mode\" : NumberInt(0), 
					\"subscriber_status\" : NumberInt(0), 
					\"access_restriction_data\" : NumberInt(32), 
					\"slice\" : 
					[{ 
						\"sst\" : NumberInt(1), 
						\"default_indicator\" : true, 
						\"_id\" : new ObjectId(), 
						\"session\" : 
						[{ 
							\"name\" : \"internet\", 
							\"type\" : NumberInt(3), 
							\"_id\" : new ObjectId(), 
							\"pcc_rule\" : [], 
							\"ambr\" : 
							{ 
								\"uplink\" : { \"value\": NumberInt(1), \"unit\" : NumberInt(3) }, 
								\"downlink\" : { \"value\": NumberInt(1), \"unit\" : NumberInt(3) }, 
							}, 
							\"qos\" : 
							{ 
								\"index\" : NumberInt(9), 
								\"arp\" : 
								{ 
									\"priority_level\" : NumberInt(8), 
									\"pre_emption_capability\" : NumberInt(1), 
									\"pre_emption_vulnerability\" : NumberInt(1), 
								}, 
							}, 
						}], 
					}], 
					\"ambr\" : 
					{ 
						\"uplink\" : { \"value\": NumberInt(1), \"unit\" : NumberInt(3),}, 
						\"downlink\" : { \"value\": NumberInt(1), \"unit\" : NumberInt(3) }, 
					},  
					\"security\" : 
					{ 
						\"k\" : \"$KI\", 
						\"amf\" : \"8000\", 
						\"op\" : null, 
						\"opc\" : \"$OPC\" 
					}, 
					\"__v\" : 0
				},  
			},
			upsert=true);" $DB_URI
		exit $?
	fi

	if [ "$#" -eq 5 ]; then
		IMSI=$2 
		IP=$3
		KI=$4
		OPC=$5

		mongo --eval "db.subscribers.update( { \"imsi\" : \"$IMSI\" }, 
			{ \$setOnInsert: 
				{ 
					\"imsi\" : \"$IMSI\", 
					\"subscribed_rau_tau_timer\" : NumberInt(12), 
					\"network_access_mode\" : NumberInt(0), 
					\"subscriber_status\" : NumberInt(0), 
					\"access_restriction_data\" : NumberInt(32), 
					\"slice\" : 
					[{ 
						\"sst\" : NumberInt(1), 
						\"default_indicator\" : true, 
						\"_id\" : new ObjectId(), 
						\"session\" : 
						[{ 
							\"name\" : \"internet\", 
							\"type\" : NumberInt(3), 
							\"_id\" : new ObjectId(), 
							\"pcc_rule\" : [],
							\"ue\" : 
							{
								\"addr\" : \"$IP\",
							},
							\"ambr\" : 
							{ 
								\"uplink\" : { \"value\": NumberInt(1), \"unit\" : NumberInt(3) }, 
								\"downlink\" : { \"value\": NumberInt(1), \"unit\" : NumberInt(3) }, 
							}, 
							\"qos\" : 
							{ 
								\"index\" : NumberInt(9), 
								\"arp\" : 
								{ 
									\"priority_level\" : NumberInt(8), 
									\"pre_emption_capability\" : NumberInt(1), 
									\"pre_emption_vulnerability\" : NumberInt(1), 
								}, 
							}, 
						}], 
					}], 
					\"ambr\" : 
					{ 
						\"uplink\" : { \"value\": NumberInt(1), \"unit\" : NumberInt(3),}, 
						\"downlink\" : { \"value\": NumberInt(1), \"unit\" : NumberInt(3) }, 
					},  
					\"security\" : 
					{ 
						\"k\" : \"$KI\", 
						\"amf\" : \"8000\", 
						\"op\" : null, 
						\"opc\" : \"$OPC\" 
					}, 
					\"__v\" : 0
				},  
			},
			upsert=true);" $DB_URI
		exit $?
	fi

	echo "open5gs-dbctl: incorrect number of args, format is \"open5gs-dbctl add imsi key opc\""
	exit 1
fi

if [ "$1" = "addT1" ]; then
	if [ "$#" -eq 4 ]; then
		IMSI=$2 
		KI=$3
		OPC=$4

		mongo --eval "db.subscribers.update( { \"imsi\" : \"$IMSI\" }, 
			{ \$setOnInsert: 
				{ 
					\"imsi\" : \"$IMSI\", 
					\"subscribed_rau_tau_timer\" : NumberInt(12), 
					\"network_access_mode\" : NumberInt(0), 
					\"subscriber_status\" : NumberInt(0), 
					\"access_restriction_data\" : NumberInt(32), 
					\"slice\" : 
					[{ 
						\"sst\" : NumberInt(1), 
						\"default_indicator\" : true, 
						\"_id\" : new ObjectId(), 
						\"session\" : 
						[{ 
							\"name\" : \"internet\", 
							\"type\" : NumberInt(3), 
							\"_id\" : new ObjectId(), 
							\"pcc_rule\" : [], 
							\"ambr\" : 
							{ 
								\"uplink\" : { \"value\": NumberInt(1), \"unit\" : NumberInt(3) }, 
								\"downlink\" : { \"value\": NumberInt(1), \"unit\" : NumberInt(3) }, 
							}, 
							\"qos\" : 
							{ 
								\"index\" : NumberInt(9), 
								\"arp\" : 
								{ 
									\"priority_level\" : NumberInt(8), 
									\"pre_emption_capability\" : NumberInt(1), 
									\"pre_emption_vulnerability\" : NumberInt(1), 
								}, 
							}, 
						},{ 
							\"name\" : \"internet1\", 
							\"type\" : NumberInt(3), 
							\"_id\" : new ObjectId(), 
							\"pcc_rule\" : [], 
							\"ambr\" : 
							{ 
								\"uplink\" : { \"value\": NumberInt(1), \"unit\" : NumberInt(3) }, 
								\"downlink\" : { \"value\": NumberInt(1), \"unit\" : NumberInt(3) }, 
							}, 
							\"qos\" : 
							{ 
								\"index\" : NumberInt(9), 
								\"arp\" : 
								{ 
									\"priority_level\" : NumberInt(8), 
									\"pre_emption_capability\" : NumberInt(1), 
									\"pre_emption_vulnerability\" : NumberInt(1), 
								}, 
							}, 
						},{ 
							\"name\" : \"internet2\", 
							\"type\" : NumberInt(3), 
							\"_id\" : new ObjectId(), 
							\"pcc_rule\" : [], 
							\"ambr\" : 
							{ 
								\"uplink\" : { \"value\": NumberInt(1), \"unit\" : NumberInt(3) }, 
								\"downlink\" : { \"value\": NumberInt(1), \"unit\" : NumberInt(3) }, 
							}, 
							\"qos\" : 
							{ 
								\"index\" : NumberInt(9), 
								\"arp\" : 
								{ 
									\"priority_level\" : NumberInt(8), 
									\"pre_emption_capability\" : NumberInt(1), 
									\"pre_emption_vulnerability\" : NumberInt(1), 
								}, 
							}, 
						}], 
					}], 
					\"ambr\" : 
					{ 
						\"uplink\" : { \"value\": NumberInt(1), \"unit\" : NumberInt(3),}, 
						\"downlink\" : { \"value\": NumberInt(1), \"unit\" : NumberInt(3) }, 
					},  
					\"security\" : 
					{ 
						\"k\" : \"$KI\", 
						\"amf\" : \"8000\", 
						\"op\" : null, 
						\"opc\" : \"$OPC\" 
					}, 
					\"__v\" : 0
				},  
			},
			upsert=true);" $DB_URI
		exit $?
	fi

	if [ "$#" -eq 5 ]; then
		IMSI=$2 
		IP=$3
		KI=$4
		OPC=$5

		mongo --eval "db.subscribers.update( { \"imsi\" : \"$IMSI\" }, 
			{ \$setOnInsert: 
				{ 
					\"imsi\" : \"$IMSI\", 
					\"subscribed_rau_tau_timer\" : NumberInt(12), 
					\"network_access_mode\" : NumberInt(0), 
					\"subscriber_status\" : NumberInt(0), 
					\"access_restriction_data\" : NumberInt(32), 
					\"slice\" : 
					[{ 
						\"sst\" : NumberInt(1), 
						\"default_indicator\" : true, 
						\"_id\" : new ObjectId(), 
						\"session\" : 
						[{ 
							\"name\" : \"internet\", 
							\"type\" : NumberInt(3), 
							\"_id\" : new ObjectId(), 
							\"pcc_rule\" : [],
							\"ue\" : 
							{
								\"addr\" : \"$IP\",
							},
							\"ambr\" : 
							{ 
								\"uplink\" : { \"value\": NumberInt(1), \"unit\" : NumberInt(3) }, 
								\"downlink\" : { \"value\": NumberInt(1), \"unit\" : NumberInt(3) }, 
							}, 
							\"qos\" : 
							{ 
								\"index\" : NumberInt(9), 
								\"arp\" : 
								{ 
									\"priority_level\" : NumberInt(8), 
									\"pre_emption_capability\" : NumberInt(1), 
									\"pre_emption_vulnerability\" : NumberInt(1), 
								}, 
							}, 
						},{ 
							\"name\" : \"internet1\", 
							\"type\" : NumberInt(3), 
							\"_id\" : new ObjectId(), 
							\"pcc_rule\" : [],
							\"ue\" : 
							{
								\"addr\" : \"$IP\",
							},
							\"ambr\" : 
							{ 
								\"uplink\" : { \"value\": NumberInt(1), \"unit\" : NumberInt(3) }, 
								\"downlink\" : { \"value\": NumberInt(1), \"unit\" : NumberInt(3) }, 
							}, 
							\"qos\" : 
							{ 
								\"index\" : NumberInt(9), 
								\"arp\" : 
								{ 
									\"priority_level\" : NumberInt(8), 
									\"pre_emption_capability\" : NumberInt(1), 
									\"pre_emption_vulnerability\" : NumberInt(1), 
								}, 
							}, 
						},{ 
							\"name\" : \"internet2\", 
							\"type\" : NumberInt(3), 
							\"_id\" : new ObjectId(), 
							\"pcc_rule\" : [],
							\"ue\" : 
							{
								\"addr\" : \"$IP\",
							},
							\"ambr\" : 
							{ 
								\"uplink\" : { \"value\": NumberInt(1), \"unit\" : NumberInt(3) }, 
								\"downlink\" : { \"value\": NumberInt(1), \"unit\" : NumberInt(3) }, 
							}, 
							\"qos\" : 
							{ 
								\"index\" : NumberInt(9), 
								\"arp\" : 
								{ 
									\"priority_level\" : NumberInt(8), 
									\"pre_emption_capability\" : NumberInt(1), 
									\"pre_emption_vulnerability\" : NumberInt(1), 
								}, 
							}, 
						}], 
					}], 
					\"ambr\" : 
					{ 
						\"uplink\" : { \"value\": NumberInt(1), \"unit\" : NumberInt(3),}, 
						\"downlink\" : { \"value\": NumberInt(1), \"unit\" : NumberInt(3) }, 
					},  
					\"security\" : 
					{ 
						\"k\" : \"$KI\", 
						\"amf\" : \"8000\", 
						\"op\" : null, 
						\"opc\" : \"$OPC\" 
					}, 
					\"__v\" : 0
				},  
			},
			upsert=true);" $DB_URI
		exit $?
	fi

	echo "open5gs-dbctl: incorrect number of args, format is \"open5gs-dbctl add imsi key opc\""
	exit 1
fi

if [ "$1" = "remove" ]; then
	if [ "$#" -ne 2 ]; then
		echo "open5gs-dbctl: incorrect number of args, format is \"open5gs-dbctl remove imsi\""
		exit 1
	fi

	IMSI=$2 
	mongo --eval "db.subscribers.remove({\"imsi\": \"$IMSI\"});" $DB_URI
	exit $?
fi

if [ "$1" = "reset" ]; then
	if [ "$#" -ne 1 ]; then
		echo "open5gs-dbctl: incorrect number of args, format is \"open5gs-dbctl reset\""
		exit 1
	fi

	mongo --eval "db.subscribers.remove({});" $DB_URI
	exit $?
fi

if [ "$1" = "static_ip" ]; then
	if [ "$#" -ne 3 ]; then
		echo "open5gs-dbctl: incorrect number of args, format is \"open5gs-dbctl static_ip imsi ip\""
		exit 1
	fi
	IMSI=$2 
	IP=$3

	mongo --eval "db.subscribers.update({\"imsi\": \"$IMSI\"},{\$set: { \"slice.0.session.0.ue.addr\": \"$IP\" }});" $DB_URI
	exit $?
fi

if [ "$1" = "static_ip6" ]; then
	if [ "$#" -ne 3 ]; then
		echo "open5gs-dbctl: incorrect number of args, format is \"open5gs-dbctl static_ip6 imsi ip\""
		exit 1
	fi
	IMSI=$2 
	IP=$3

	mongo --eval "db.subscribers.update({\"imsi\": \"$IMSI\"},{\$set: { \"slice.0.session.0.ue.addr6\": \"$IP\" }});" $DB_URI
	exit $?
fi

if [ "$1" = "type" ]; then
	if [ "$#" -ne 3 ]; then
		echo "open5gs-dbctl: incorrect number of args, format is \"open5gs-dbctl type imsi type\""
		exit 1
	fi
	IMSI=$2 
	TYPE=$3

	mongo --eval "db.subscribers.update({\"imsi\": \"$IMSI\"},{\$set: { \"slice.0.session.0.type\": NumberInt($TYPE) }});" $DB_URI
	exit $?
fi

display_help
