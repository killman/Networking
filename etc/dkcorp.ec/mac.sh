#!/bin/sh
#_______________________________________________________________________________
#    _||  _ _  _ _   _  _
#   (_||<(_(_)| |_).(/_(_ Enterprise Linux system administration | www.dkcorp.ec
#_______________|_______________________________________________________________
#           File:  isp
#    Description:  Mac filtering
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
IPLN="63.93.9.1"

#------------------------------------MAC filtering--------------------------
$IPT -A INPUT -s 63.93.9.1 -j ACCEPT
$IPT -A INPUT -s 63.93.9.2 -m mac --mac-source 1C:AF:F7:85:37:09 -j ACCEPT
$IPT -A INPUT -s 63.93.9.3 -m mac --mac-source 00:1C:C0:B9:FA:30 -j ACCEPT
#$ipt -t nat -A PREROUTING -i eth1 -p tcp --dport 80 -s 63.93.9.5 -j DNAT --to-destination 63.93.9.1:80
#$ipt -t nat -A PREROUTING -i eth1 -p tcp --dport 443 -s 63.93.9.5 -j DNAT --to-destination 63.93.9.1:80
$IPT -A INPUT -s 63.93.9.4 -m mac --mac-source 00:15:6D:4E:0A:18 -j ACCEPT
$IPT -A INPUT -s 63.93.9.5 -j ACCEPT
$IPT -A INPUT -s 63.93.9.0/255.255.255.192 -j DROP
$IPT -A INPUT -i $NETIFACE -j DROP

