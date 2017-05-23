#!/bin/bash
for i in `seq 1 4`;
do
	a=`echo "grep $1 /var/log/mixpanel/*.log" | ssh live@arb-query-c06-r0${i} 'bash -s'  | sed  -E 's/(.*)\spid.*/\1/g'`
	echo live@arb-query-c06-r0${i}  $a 
done  
for j in `seq 1 4`;
do
	for i in `seq 1 2`;
	do
		a=`echo "grep $1 /var/log/mixpanel/*.log" | ssh live@arb-data-c06-s0${j}-r0${i} 'bash -s' | sed  -E 's/(.*)\spid.*/\1/g'`
		echo live@arb-data-c06-s0${j}-r0${i} $a	
	done  
done  
