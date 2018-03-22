What's NextEPC
================

NextEPC is a C-language Open Source implementation of the 3GPP Evolved Packet Core, i.e. the core network of an LTE network.

NextEPC provides the MME (Mobility Management Engine), which terminates the S1 interfaces from the eNodeBs cells in the cellular network, and interfaces via S11 to the SGW as well as via S6a to the HSS.

NextEPC provides the SGW (Serving Gateway) which is situated between the MME and PGW.  It implements the S11 interface to the MME, and the S5 interface to the PGW.

NextEPC provides the PGW or PDN-GW (Packet Data Network Gateway) element of the EPC, i.e. the gateway between the EPC and the external packet data network, such as the public Internet.  It implements the S5 interface towards the S-GW, the SGi interface towards the Internet, and the S7 interface towards the PCRF.

NextEPC provides the HSS (Home Subscriber Server) element of the EPC, i.e. the central database of mobile network subscribers, with their IMSI, MSISDN, cryptographic key materials, service subscription information, etc.  It implements the S6a interface towards the MME using the DIAMETER protocol.

NextEPC contains the PCRF (Policy and Charging Rules Function), which controls the service quality (QoS) of individual connections and how to account/charge related traffic.  It implements the Gx interface towards the PGW using the DIAMETER protocol.

Installation 
============

* ### MME, SGW, PGW, HSS, and PCRF

The NextEPC package is available on the recent versions of Ubuntu.

```bash
sudo add-apt-repository ppa:acetcom/nextepc
sudo apt-get update
sudo apt-get -y install nextepc
```
This will create a virtual network interface named as *pgwtun*. It is automatically removed by uninstalling NextEPC.

```markdown
ifconfig pgwtun
pgwtun    Link encap:UNSPEC  HWaddr 00-00-00-00-00-00-00-00-00-00-00-00-00-00-00-00  
          inet addr:45.45.0.1  P-t-P:45.45.0.1  Mask:255.255.0.0
          inet6 addr: fe80::50f6:182c:5aa3:16bb/64 Scope:Link
          inet6 addr: cafe::1/64 Scope:Global
          ...
```

The NextEPC service is registered in `systemd` environment, and is started automatically during the installation phase. The service names are *nextepc-mmed*, *nextepc-sgwd*, *nextepc-pgwd*, *nextepc-hssd*, and *nextepc-pcrfd*. You can use the `systemctl` command to control specific services.

```bash
sudo systemctl status nextepc-mmed (Check the service status)
sudo systemctl stop nextepc-mmed (Stop the service)
sudo systemctl disable nextepc-mmed (Will not be started after rebooting)
sudo systemctl enable nextepc-mmed (Will be started after rebooting)
sudo systemctl start nextepc-mmed (Start the service)
sudo systemctl restart nextepc-mmed (Stop and start)
```


* ### Web User Interface

```bash
sudo apt-get -y install curl
curl -sL https://deb.nodesource.com/setup_8.x | sudo -E bash -
curl -sL http://nextepc.org/static/webui/install | sudo -E bash -
```

The service name is *nextepc-webui*, and it will be running on _http://localhost:3000_.

```bash
sudo systemctl start nextepc-webui
```


* ### Uninstall NextEPC

```bash
curl -sL http://nextepc.org/static/webui/uninstall | sudo -E bash -
sudo apt-get purge nextepc*
```

You may need to remove manually /var/log/nextepc unless it is empty.
```bash
sudo rm -Rf /var/log/nextepc
```


## Documentation

If you don't understand something about NextEPC, the [http://nextepc.org](http://nextepc.org/) is a great place to look for answers.


## Support

Problem with NextEPC can be filed as [issues](https://github.com/acetcom/nextepc/issues) in this repository. And also, we've created slack workspace named _nextepc.slack.com_. Use [this link](https://join.slack.com/t/nextepc/shared_invite/enQtMzMyMTIwMzY2MTAyLTJjYzQyYWFhYWZmNTBmNmFmY2EzMzJlY2VhYWMzYzY1NTE1YWYzZjFiMDM4ZDIxNDRhZTRmY2M2YmI4NzdmZjI) to get started.


## License

NextEPC source files are made available under the terms of the GNU Affero General Public License (GNU AGPLv3). See [this link](http://nextepc.org/docs/nextepc/4-license/) for details.

