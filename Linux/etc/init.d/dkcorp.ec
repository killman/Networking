#!/sbin/runscript
# Copyright 2012 dkcorp.ec, Inc.
#Enterprise Linux system administration


depend() {
	need net
}

start() {
	ebegin "Starting Platform dkcorp.ec"
	/etc/dkcorp.ec/isp -d > /dev/null
	eend $?
}

stop() {
	ebegin "Stopping Platform dkcorp.ec"
	/etc/dkcorp.ec/isp -k > /dev/null
	eend $?
}

