#!/bin/sh
#_______________________________________________________________________________
#    _||  _ _  _ _   _  _
#   (_||<(_(_)| |_).(/_(_ Enterprise Linux system administration | www.dkcorp.ec
#_______________|_______________________________________________________________
#           File:  firewall.sh
#    Description:  Firewall proxy ISP
#   Requirements:  Gentoo Base System release 2.0.3
#         Author:  killman
#          Email:  killman@dkcorp.ec 
#        Version:  1.0
#        Created:  vie ene 13 21:46:42 ECT 2012 
#______________________________________________________________________________

IPT="/sbin/iptables"
NETIFACE="eth0"
LANIFACE="eth1"
LOOPBACK="lo"
IPLB="127.0.0.1"
IPLN="192.168.1.0/26"
PORTCACHE="3128"
echo "Applying..."

$IPT -F
$IPT -t nat -F


$IPT -A INPUT -i $LOOPBACK -j ACCEPT
$IPT -A INPUT -s $IPLB -j ACCEPT
$IPT -A INPUT -i $NETIFACE -m state --state ESTABLISHED,RELATED -j ACCEPT
$IPT -t nat -A POSTROUTING -o $NETIFACE -j MASQUERADE
$IPT -P FORWARD ACCEPT
$IPT -t nat -A PREROUTING -s $IPLN -d 0/0 -p tcp -m tcp --dport 80 -j REDIRECT --to-port $PORTCACHE
#$IPT -t nat -A PREROUTING -i $LANIFACE -p tcp --dport 80 -s $IPLN -j DNAT --to-destination 192.168.1.1:$PORTCACHE
$IPT -A INPUT -i $NETIFACE -j DROP

