#!/bin/bash

read -p "Enter the trace file: " file
# file="test.tr"
max=$(awk 'END {print $12}' $file)
echo "Max packets = "$max
read -p "How many packets do you want to consider?" max
cbr=0
tcp=0
ack=0
cbr_count=0
tcp_count=0
ack_count=0

for i in $(seq 0 $max)
do
	type=$(awk '$12=='"$i"' {print $0}' $file | awk 'END {print $5}' )
	delay=$(awk '$12=='"$i"' {print $0}' $file | awk '(NR=="1"){print $2} END {print $2}' | paste -sd- - | bc -l )

	if [[ $type == "cbr" ]]; then
		cbr=$(echo "$cbr-($delay)" | bc -l &)
		echo "cbr="$cbr" count="$cbr_count
		cbr_count=$((cbr_count+1))
	elif [[ $type == "tcp" ]]; then
		tcp=$(echo "$tcp-($delay)" | bc -l &)
		echo "tcp="$tcp" count="$tcp_count
		tcp_count=$((tcp_count+1))
	elif [[ $type == "ack" ]]; then
		ack=$(echo "$ack-($delay)" | bc -l &)
		echo "ack="$ack" count="$ack_count
		ack_count=$((ack_count+1))
	fi
done

cbr_throughput=0
tcp_throughput=0
ack_throughput=0

if [[ $tcp_count>0 ]]; then
	tcp_throughput=$( echo "$tcp_count*1040/$tcp" | bc -l )
fi
if [[ $cbr_count>0 ]]; then
	cbr_throughput=$( echo "$cbr_count*1000/$cbr" | bc -l )
fi
if [[ $ack_count>0 ]]; then
	ack_throughput=$( echo "$ack_count*40/$ack" | bc -l )
fi

total_delay=$( echo "$cbr+$tcp+$ack" | bc -l )
total_packets=$( echo "$cbr_count+$tcp_count+$ack_count" | bc -l )
avg_delay=$( echo "$total_delay/$total_packets" | bc -l )
avg_throughput=$( echo "$cbr_throughput+$ack_throughput+$tcp_throughput" | bc -l)
failed_packets=$(awk '$1=="d" {print $1}' $file | wc -l)
pdr=$( echo "($total_packets-$failed_packets)*100/$total_packets" | bc -l )

echo "Avg. Delay = "$avg_delay" msec"
echo "Throughput = "$avg_throughput" bytes per sec"
echo "Packet Delivery Rate = "$pdr" percent"