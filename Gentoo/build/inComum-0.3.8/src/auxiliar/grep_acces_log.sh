#!/bin/bash
sudo grep --color $1 /var/log/squid/access.log
