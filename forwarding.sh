#!/bin/sh

iptables -F
iptables -t nat -F


iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
iptables -t nat -A PREROUTING -i eth1 -p tcp --dport 80 -s 63.93.9.0/26 -j DNAT --to-destination 63.93.9.1:6393
#iptables -t nat -A PREROUTING -i eth1 -p tcp --dport 80 -s 63.93.9.0/24 -j DNAT --to-destination 63.93.9.1:3128
#iptables -t nat -A PREROUTING -i eth1 -p tcp -s 63.93.9.0/24 --dport 443 -j DNAT --to-destination 63.93.9.1:3128
