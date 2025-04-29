---
title: Open5GS with 5G Sharp Orchestrator
---

This tutorial is a guide to configure the tool 5G Sharp Orchestrator with Open5GS. 

5G Sharp Orchestrator is a tool that serves as a comprehensive wrapper for configuring, deploying and monitoring 5G open-source network components, simplifying the orchestration process. 

For more information on 5G Sharp Orchestrator please visit: https://github.com/Ethon-Shield/5g-sharp-orchestrator

![Basic Execution Example](https://raw.githubusercontent.com/Ethon-Shield/5g-sharp-orchestrator/9eddaf0c19d8333708271f1e3d6348735e32ae90/images/basic_execution_example.png)


# Usage

To correctly execute the 5G Sharp Orchestrator with Open5GS you will need to:

**0. Install prerequisites**
**1. Configure the project**
**2. Check the configuration is valid**
**3. Execute the tool**

This tutorial has been described with the following setup:
- Physical machine: 
  - i7 processor
  - Ubuntu 22.04 LTS
  - User "test"
  - Connectivity - Offline
  - Open5GS cloned and compiled in /home/test/open5gs
  - srsRAN cloned and compiled in /home/test/srsRAN_Project
- Network parameters:
  - MCC: 001
  - MNC: 01
  - TAC: 1
- SDR device:
  - Ettus B200 mini

## 0. Install prerequisites

To be able to use the 5G Sharp Orchestrator tool, you will need to meet the following prerequisites. 

<details>
<summary><strong>Software prerequisites</strong></summary>

This project needs certain binaries to be installed.

- The project has been successfully tested with **Ubuntu 22.04** 
- UHD built from source (https://files.ettus.com/manual/page_build_guide.html)
- docker **version 19.03.0 or higher** (https://docs.docker.com/engine/install/ubuntu/)
```
# User going to be used should be in the docker group
sudo usermod -aG docker <user>
newgrp docker
```
- docker-compose **version 1.29.2 or higher** (https://docs.docker.com/compose/install/linux/)
- tmux **version 3.5a** (https://github.com/tmux/tmux/releases)

```bash
wget https://github.com/tmux/tmux/releases/download/3.5a/tmux-3.5a.tar.gz
tar -zxf tmux-3.5a.tar.gz
cd tmux-3.5a
./configure
make
sudo make install
```

- wireshark, tshark, expect

```bash
sudo add-apt-repository ppa:wireshark-dev/stable
sudo apt update
sudo apt install wireshark tshark expect
```

</details>
<details>
<summary><strong>Install Open5GS Core</strong></summary>

Official documentation: https://open5gs.org/open5gs/docs/guide/02-building-open5gs-from-sources/

To work with Open5GS core, it is recommended to build the project from source.

**1. Getting MongoDB**

- Install GNU Privacy Guard tool:

```
sudo apt update
sudo apt install gnupg
```

- Import the public key used by the package management system:

```
curl -fsSL https://pgp.mongodb.com/server-7.0.asc | sudo gpg -o /usr/share/keyrings/mongodb-server-7.0.gpg --dearmor

echo "deb [ arch=amd64,arm64 signed-by=/usr/share/keyrings/mongodb-server-7.0.gpg ] https://repo.mongodb.org/apt/ubuntu jammy/mongodb-org/7.0 multiverse" | sudo tee /etc/apt/sources.list.d/mongodb-org-7.0.list
```

- Install the MongoDB packages:
```
sudo apt update
sudo apt install -y mongodb-org
```

**NOTE**: MongoDB service needs to be running in order to execute the project. This can be done using the command ``sudo systemctl start mongod ``

**2. Building Open5GS**

Install the required dependencies:

```
sudo apt install python3-pip python3-setuptools python3-wheel ninja-build build-essential flex bison git cmake libsctp-dev libgnutls28-dev libgcrypt-dev libssl-dev libidn11-dev libmongoc-dev libbson-dev libyaml-dev libnghttp2-dev libmicrohttpd-dev libcurl4-gnutls-dev libnghttp2-dev libtins-dev libtalloc-dev meson
```
Clone the official project repository:
```
git clone https://github.com/open5gs/open5gs
```
Compile with <i>meson</i> and build with <i>ninja</i>:
```
cd open5gs/ meson build --prefix=`pwd`/install
ninja -C build
```
Check wether the compilation is correct:
```
 ./build/tests/registration/registration
```
Run all test programs:
```
cd build
sudo meson test -v
```
Perform the installation process:
```
cd build
ninja install
cd ../
```
</details>

<details>
<summary><strong>Install srsRAN gNB</strong></summary>

Official documentation: https://docs.srsran.com/projects/project/en/latest/user_manuals/source/installation.html

**1. Installing build tools and dependencies**

The required build tools and dependencies for the project can be installed in Ubuntu using the following command:
```
sudo apt-get install cmake make gcc g++ pkg-config libfftw3-dev libmbedtls-dev libsctp-dev libyaml-cpp-dev libgtest-devg
```

**2. Clone and build**

Clone the project repository:
```
git clone https://github.com/srsRAN/srsRAN_Project.git
```

Build the code-base:
```
cd srsRAN_Project
mkdir build
cd build
cmake ../
make -j $(nproc)
make test -j $(nproc)
sudo make install
```
</details>

<details>
<summary><strong>Hardware prerequisites</strong></summary>


- USRP devices.
- USB 3.0 port to connect the SDR.
- It is recommended to at least have an i7 processor.

</details>


## 1. Configuring the project

Once the prerequisites have been fullfilled, the next step is to setup the 5G Sharp Orchestrator. First of all, clone the repository:

```
cd ~/
git clone https://github.com/Ethon-Shield/5g-sharp-orchestrator.git
```

To configure the 5G Sharp Orchestrator with Open5GS you will need to modify the corresponding parameters in the file `./conf/sharp-orchestrator.src` .  In this file you will be able to set different network parameters such as the MCC, MNC or TAC, encryption and integrity alogrithms, as well as defining the file paths of the corresponding binaries. 

The following variables are set based on the example setup described at the start of the tutorial. 

**SHARP ORCHESTRATOR**:
- **SHARP_ORCHESTRATOR_IP_ADDRESS**=127.0.0.1
- **SHARP_ORCHESTRATOR_USERNAME**="test"
- **SHARP_ORCHESTRATOR_WORKING_DIR**="/home/test/5g-sharp-orhcestrator/"

**GENERAL**:

- **MCC**=001
- **MNC**=01
- **TAC**=1
- **DEBUG**="false"
- **DNS_IP_ADDRESS**="8.8.8.8"
- **DEPLOY_NRCORE_ONLY**="false"
- **BACKUP_DIR_PREFIX**=""

**NRCORE**:

- **NRCORE_TECH**=OPEN5GS
- **INT_ALGO_PRIORITY_LIST**=NIA1,NIA2,NIA0
- **CIPH_ALGO_PRIORITY_LIST**=NEA1,NEA2,NEA0
- **NRCORE_OPEN5GS_WD**="/home/test/open5gs"

**GNB**:

- **GNB_TECH**=SRS
- **GNB_IP_ADDRESS**=127.0.0.1
- **GNB_USERNAME**="test"
- **GNB_WORKING_DIR**="/home/test/Desktop/gnb"
- **ARFCN**="641280"
- **SRS_GNB_BIN**="/home/test/srsRAN_Project/build/apps/gnb/gnb"
- **BAND**="78"
- **CHANNEL_BW**="20"
- **SCS**="15"

### SUDOERS file 

It will be necessary to modify /etc/.sudoers file (via visudo for example) in order to be able to execute certain commands that need root privileges:

```bash
test ALL=(ALL) NOPASSWD: \
/usr/bin/kill -9 *, \
/usr/bin/chown -R * /tmp, \
/usr/bin/chown -R * /logs, \
/usr/sbin/ip tuntap add name ogstun mode tun, \
/usr/sbin/ip addr del 10.45.0.1/16 dev ogstun, \
/usr/sbin/ip addr add 10.45.0.1/16 dev ogstun, \
/usr/sbin/ip addr del 2001\:db8\:cafe\:\:1/48 dev ogstun, \
/usr/sbin/ip addr add 2001\:db8\:cafe\:\:1/48 dev ogstun, \
/usr/sbin/ip link set ogstun up, \
/usr/sbin/sysctl net.ipv4.conf.all.forwarding=1, \ 
/usr/sbin/iptables -P FORWARD ACCEPT, \
/usr/sbin/iptables -S, \
/usr/sbin/ip route del *, \
/usr/sbin/ip route add *, \
/usr/bin/unbuffer *, \
/usr/bin/kill -9 * 
```

### SSH KEYS

Furthermore, although in this example setup we are only using one physical machine, the nodes will talk to each other via ssh. Consequently, it is important to store the public ssh key of the corresponding user. 

1. Generate ssh key pair: `ssh-key-gen -t rsa`
2. Copy public pair into remote machine (in this example setup - localhost): `ssh-copy-id -i ~/.ssh/id_rsa.pub test@127.0.0.1`


## 2. Checking the configuation is valid

Once the project has been configured accordingly, check if it has been correctly configured executing the next script: `./bin/check_conf.sh`. All checks should have a **YES**.

<details>
    <summary><strong> Example output </strong></summary>

```
##################################
Checking basic variables
##################################
BASIC variables are OK

##################################
Checking necessary binaries
##################################
tshark ... YES
expect ... YES
tmux ... YES
uhd_find_devices ... YES
uhd_usrp_probe ... YES
open5gs-nrfd ... YES
open5gs-scpd ... YES
open5gs-amfd ... YES
open5gs-smfd ... YES
open5gs-upfd ... YES
open5gs-ausfd ... YES
open5gs-udmd ... YES
open5gs-pcfd ... YES
open5gs-nssfd ... YES
open5gs-bsfd ... YES
open5gs-udrd ... YES

##################################
Checking CORE IP address, username and working dir
##################################

CORE IP ADDRESS ... YES
CORE USERNAME ... YES
CORE WORKING DIR ... YES

Checking if open5gs services are active 

MongoDB is running ... YES

##################################
Checking SSH connections
##################################

CORE --> GNB ... YES
GNB --> CORE ... YES

##################################
Checking necessary repositories & binaries
##################################

OPEN5GS CORE
        /home/test/open5gs/ directory ... YES 
     NOTES:
       - /home/test/open5gs/ in TAG v2.7.2-65-g6d80d4322
gNB
     SRS gnb bin ... YES 

##################################
Checking sudoers file
##################################

NOPASSWD in ORCH & CORE for test for command /usr/bin/kill -9 \* ... YES
NOPASSWD in ORCH & CORE for test for command /usr/bin/chown -R \* /tmp ... YES
NOPASSWD in ORCH & CORE for test for command /usr/bin/chown -R \* /logs ... YES
NOPASSWD in ORCH & CORE for test for command /usr/sbin/ip tuntap add name ogstun mode tun ... YES
NOPASSWD in ORCH & CORE for test for command /usr/sbin/ip addr del 10.45.0.1/16 dev ogstun ... YES
NOPASSWD in ORCH & CORE for test for command /usr/sbin/ip addr add 10.45.0.1/16 dev ogstun ... YES
NOPASSWD in ORCH & CORE for test for command /usr/sbin/ip addr del 2001\:db8\:cafe\:\:1/48 dev ogstun ... YES
NOPASSWD in ORCH & CORE for test for command /usr/sbin/ip addr add 2001\:db8\:cafe\:\:1/48 dev ogstun ... YES
NOPASSWD in ORCH & CORE for test for command /usr/sbin/ip link set ogstun up ... YES
NOPASSWD in ORCH & CORE for test for command /usr/sbin/sysctl net.ipv4.conf.all.forwarding\=1 ... YES
NOPASSWD in ORCH & CORE for test for command /usr/sbin/iptables -P FORWARD ACCEPT ... YES
NOPASSWD in ORCH & CORE for test for command /usr/sbin/iptables -S ... YES
NOPASSWD in GNB for test for command /usr/sbin/ip route del \* in GNB ... YES
NOPASSWD in GNB for test for command /usr/sbin/ip route add \* in GNB ... YES
NOPASSWD in GNB for test for command /usr/bin/unbuffer \* in GNB ... YES
NOPASSWD in GNB for test for command /usr/bin/kill -9 \* in GNB ... YES

##################################
Checking sharp-orchestrator.src parameters
##################################

MCC 001 ... YES
MNC 01 ... YES
DEBUG false ... YES
DEPLOY_NRCORE_ONLY false ... YES
DNS_IP_ADDRESS 8.8.8.8 ... YES
AMF_IP_ADDRESS 127.0.0.100 ... YES
SHARP_ORCHESTRATOR_IP_ADDRESS 127.0.0.1 ... YES
NRCORE_IP_ADDRESS 127.0.0.1 ... YES
GNB_IP_ADDRESS 127.0.0.1 ... YES

##################################
Checking gNB directory
##################################

GNB directory ... YES
```

</details>

## 3. Executing the tool

After checking the configuration file and setting every paramater correctly, you will be able to start the orchestration process. 

##### STARTING THE ORCHESTRATOR APP

To start the orchestrator app execute the following command:
```bash
./tui.sh
```

You will have an interactive menu to perform all the needed actions.

![Main Menu](https://raw.githubusercontent.com/Ethon-Shield/5g-sharp-orchestrator/9eddaf0c19d8333708271f1e3d6348735e32ae90/images/MAINMENU.png)

##### STARTING THE NETWORK

Choose the "Start network" option in the interactive menu --> "1"

##### STOPPING THE NETWORK

Choose the "Stop network" option in the interactive menu --> "2"

##### UPDATE GNB PARAMETERS ON REAL-TIME

Some parameters in the gNB can be updated on real-time. This is done to prevent the undeployment of the core network and slowing the whole process. 

The current supported parameters for the srs gNB to be updated are:
- ARFCN 
- BAND 
- CHANNEL_BW 
- SCS

Choose the "Change network parameters" option in the interactive menu --> "3"

This will show a sub menu with the different options:
- Setting the different parameters
- Updating them 

![Submenu Update Parameters](https://raw.githubusercontent.com/Ethon-Shield/5g-sharp-orchestrator/9eddaf0c19d8333708271f1e3d6348735e32ae90/images/SUBMENU_update_parameters.png)

Before updating any parameter, you need to set the value. For that, choose the corresponding "set" option and the value to set will be asked.
The new set value will appear in the menu, as it is shown in the images below

![Submenu Update Parameters Set ARFCN](https://raw.githubusercontent.com/Ethon-Shield/5g-sharp-orchestrator/9eddaf0c19d8333708271f1e3d6348735e32ae90/images/SUBMENU_update_parameters_set_arfcn.png)


![Submenu Update Parameters ARFCN Set](https://raw.githubusercontent.com/Ethon-Shield/5g-sharp-orchestrator/9eddaf0c19d8333708271f1e3d6348735e32ae90/images/SUBMENU_update_parameters_arfcn_setted.png)


After setting all the values you want to update, choose the "Update parameters" option in the interactive menu for the changes to take place.
This action will stop the running gNB instance and start a new one with the updated parameters. 
Any error in the gNB execution due to invalid parameters selection will be prompted in the log screen.


##### BACKING UP FILES DURING EXECUTION

Stopping the network will automatically perform a backup. This options is used if you want to perform a backup during the execution without stopping the network. 
The backup will be saved in the sharp-orchestrator/backups directory.

Making a backup will save the following files: 
```
├── logs -> DIR that includes all the logs from the network nodes
├── pcaps -> DIR that includes all the pcaps from the network nodes
├── conf -> DIR that includes the latest configuration files of the network nodes
└── sharp_orchestrator.log -> Orchestration log
```

Choose the "Make backup" option in the interactive menu --> "4"

##### MANAGE SUBSCRIBERS IN DATABASE

To be able to connect subscribers to the deployed network they need to be provisioned in the corresponding databases. 

Choose the "Edit Database" option in the interactive menu --> "5"

This will show a sub menu with the different options:
- Setting the different parameters (IMSI, KI, OPC)
- Adding a subscriber to the database - needed parameters: IMSI, KI, OPC
- Updating a subscriber in the database - needed parameters: IMSI, KI, OPC
- Removing a subscriber from the database - needed parameters: IMSI

![Submenu Edit Database](https://raw.githubusercontent.com/Ethon-Shield/5g-sharp-orchestrator/9eddaf0c19d8333708271f1e3d6348735e32ae90/images/SUBMENU_edit_database.png)


Before performing an action to a subscriber, you need to add the required information. For that set the different parameters.

The next image shows an example of setting the IMSI:

![Submenu Edit Database Set IMSI](https://raw.githubusercontent.com/Ethon-Shield/5g-sharp-orchestrator/9eddaf0c19d8333708271f1e3d6348735e32ae90/images/SUBMENU_edit_database_set_imsi.png)


After setting the IMSI, you would be able to remove it from the database, by chossing the option "Remove subscriber from the database" --> 6

![Submenu Edit Database Remove Subscriber](https://raw.githubusercontent.com/Ethon-Shield/5g-sharp-orchestrator/9eddaf0c19d8333708271f1e3d6348735e32ae90/images/SUBMENU_edit_database_remove_subscriber.png)


</details>

# More configuration options

- If you only want to deploy the Open5GS nrcore network without the radio node, set the DEPLOY_NRCORE_ONLY variable to "true"


# Viewing PCAP files

- Go to Analyze --> Enabled Protocols --> check the box mac_nr and mac_nr_udp
- Sometimes, when debugging the PCAP files, the traffic is encrypted using EEA0. To view this traffic, in Wireshark go to Edit -> Preferences -> Protocols -> NAS-5GS -> check box "Try to detect and decode 5G-EA0 ciphered message"


