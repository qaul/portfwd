#! /bin/sh
#
# $1 = turns
# $2 = delay

trigger_dns() {
nc -n 127.0.0.1 2000 </dev/null >/dev/null
}

t=30
[ -n "$1" ] && [ $1 -gt 0 ] && t=$1

d=0
[ -n "$2" ] && [ $2 -gt 0 ] && d=$2

while [ $t -gt 0 ]; do
	echo "turns to go: $t"
	echo "sleeping $d seconds"
	sleep $d
	trigger_dns
	t=$(($t - 1))
done
