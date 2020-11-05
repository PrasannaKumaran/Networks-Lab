BEGIN {
	recvdSize = 0
	txsize=0
	drpSize=0
	startTime = 0
	stopTime = 0
	thru=0
}
{
	event = $1
	time = $2
	node_id = $3
	pkt_size = $6
	level = $5
	# Store start time
	if (level == "rtProtoLS" && (event == "+" || event == "s") ) 
	{
		if (time < startTime) 
		{
			startTime = time
		}
		txsize++;
	}# Update total received packets size and store packets arrival time
	if (level == "rtProtoLS" && event == "r" ) 
	{
		if (time > stopTime) 
		{
			stopTime = time
		}
		recvdSize++
	}
	
	#if (level == "rtProtoDV" && event == "d" ) 
	#{
	#drpSize++
	#d=%.2f
	#}
}
END {
printf("Average Throughput[kbps] = %.2f\t\ts=%.2f\t\tr=%.2fStartTime=%.2f\tStopTime=%.2f\n",(recvdSize/(stopTime-startTime+1)),txsize,recvdSize,startTime,stopTime)
}

