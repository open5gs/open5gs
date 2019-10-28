#!/bin/bash

version=0.9.0

display_help() {
    echo "COMMANDS:" >&2
    echo "   add {imsi key opc}: adds a user to the network "
    # echo "   static_ip {imsi, ip}: adds a static_ip field to the user"
    echo "   remove {imsi}: removes a user from the network"
    echo "   reset: WIPES OUT the database and restores it to an empty default"
    echo "   help: displays this message and exits"
    # ANY OTHER VALUES?!?!?!
}

echo "nextepc_conf: NextEPC Database Configuration Tool ($version)"

if [ "$#" -lt 1 ]; then
	display_help
	exit 1
fi

if [ "$1" = "help" ]; then
	display_help
	exit 1
fi

if [ "$1" = "add" ]; then
	if [ "$#" -ne 4 ]; then
		echo "nextepc_conf: incorrect number of args, format is \"nextepc_conf add imsi key opc\""
		exit 1
	fi
	$IMSI = $2 
	$KI = $3
	$OPC = $4
	# $IP = $5

	mongo --eval "db.subscribers.insert({ \"imsi\" : \"$IMSI\", \"pdn\" : [ { \"apn\" : \"internet\", \"_id\" : ObjectId(\"5daf349363b34e2c75fee3ee\"), \"pcc_rule\" : [ ], \"pgw\" : { \"addr\" : \"127.0.0.3\" }, \"ambr\" : { \"downlink\" : NumberLong(1024000), \"uplink\" : NumberLong(1024000) }, \"qos\" : { \"qci\" : 9, \"arp\" : { \"priority_level\" : 8, \"pre_emption_vulnerability\" : 1, \"pre_emption_capability\" : 1 } }, \"type\" : 2 } ], \"ambr\" : { \"downlink\" : NumberLong(1024000), \"uplink\" : NumberLong(1024000) }, \"subscribed_rau_tau_timer\" : 12, \"network_access_mode\" : 2, \"subscriber_status\" : 0, \"access_restriction_data\" : 32, \"security\" : { \"k\" : \"$KI\", \"amf\" : \"8000\", \"op\" : null, \"opc\" : \"$OPC\" }, \"__v\" : 0 });" nextepc
	# mongo --eval "db.subscribers.insert({ \"imsi\" : \"$IMSI\", \"pdn\" : [ { \"apn\" : \"internet\", \"_id\" : ObjectId(\"5daf349363b34e2c75fee3ee\"), \"pcc_rule\" : [ ], \"pgw\" : { \"addr\" : \"127.0.0.3\" }, \"ambr\" : { \"downlink\" : NumberLong(1024000), \"uplink\" : NumberLong(1024000) }, \"qos\" : { \"qci\" : 9, \"arp\" : { \"priority_level\" : 8, \"pre_emption_vulnerability\" : 1, \"pre_emption_capability\" : 1 } }, \"type\" : 2, \"static_ip\" : \"$IP\" } ], \"ambr\" : { \"downlink\" : NumberLong(1024000), \"uplink\" : NumberLong(1024000) }, \"subscribed_rau_tau_timer\" : 12, \"network_access_mode\" : 2, \"subscriber_status\" : 0, \"access_restriction_data\" : 32, \"security\" : { \"k\" : \"$KI\", \"amf\" : \"8000\", \"op\" : null, \"opc\" : \"$OPC\" }, \"__v\" : 0 });" nextepc
	exit 0
fi

# if [ "$1" = "static_ip" ]; then
# 	if [ "$#" -ne 3 ]; then
# 		echo "nextepc_conf: incorrect number of args, format is \"nextepc_conf static_ip imsi ip\""
# 		exit 1
# 	fi
# 	$IMSI = $2 
# 	$IP = $3

# 	mongo --eval "db.subscribers.insert({ \"imsi\" : \"$IMSI\", \"pdn\" : [ { \"apn\" : \"internet\", \"_id\" : ObjectId(\"5daf349363b34e2c75fee3ee\"), \"pcc_rule\" : [ ], \"pgw\" : { \"addr\" : \"127.0.0.3\" }, \"ambr\" : { \"downlink\" : NumberLong(1024000), \"uplink\" : NumberLong(1024000) }, \"qos\" : { \"qci\" : 9, \"arp\" : { \"priority_level\" : 8, \"pre_emption_vulnerability\" : 1, \"pre_emption_capability\" : 1 } }, \"type\" : 2, \"static_ip\" : \"$IP\" } ], \"ambr\" : { \"downlink\" : NumberLong(1024000), \"uplink\" : NumberLong(1024000) }, \"subscribed_rau_tau_timer\" : 12, \"network_access_mode\" : 2, \"subscriber_status\" : 0, \"access_restriction_data\" : 32, \"security\" : { \"k\" : \"$KI\", \"amf\" : \"8000\", \"op\" : null, \"opc\" : \"$OPC\" }, \"__v\" : 0 });" nextepc
# 	exit 0
# fi

if [ "$1" = "remove" ]; then
	if [ "$#" -ne 2 ]; then
		echo "nextepc_conf: incorrect number of args, format is \"nextepc_conf remove imsi\""
		$IMSI = $2 
		mongo --eval "db.subscribers.remove({\"imsi\": \"$IMSI\"});" nextepc
		exit 1
	fi
	exit 0
fi

if [ "$1" = "reset" ]; then
	echo "nextepc_conf: reset database not yet implemented."
	exit 0
fi

display_help
