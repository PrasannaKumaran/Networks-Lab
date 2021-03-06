set ns [new Simulator]
$ns color 0 blue
$ns color 1 red
set nf [open dv.nam w]
$ns namtrace-all $nf
set tr [open dv.tr w]
$ns trace-all $tr
proc finish {} {
 global nf ns tr
 $ns flush-trace
 close $tr
 exec nam dv.nam &
 exit 0
 }
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]
set n5 [$ns node]
set n6 [$ns node]
set n7 [$ns node]
set n8 [$ns node]
set n9 [$ns node]
set n10 [$ns node]
set n11 [$ns node]
$ns duplex-link $n0 $n8 1Mb 10ms DropTail
$ns duplex-link $n1 $n10 1Mb 10ms DropTail
$ns duplex-link $n0 $n9 1Mb 10ms DropTail
$ns duplex-link $n9 $n11 1Mb 10ms DropTail
$ns duplex-link $n10 $n11 1Mb 10ms DropTail
$ns duplex-link $n11 $n5 1Mb 10ms DropTail
$ns duplex-link $n7 $n6 1Mb 10ms DropTail

$ns duplex-link-op $n0 $n8 orient right
$ns duplex-link-op $n1 $n10 orient right
$ns duplex-link-op $n0 $n9 orient left
$ns duplex-link-op $n9 $n11 orient left-up
$ns duplex-link-op $n10 $n11 orient left-down
$ns duplex-link-op $n11 $n5 orient right-up
$ns duplex-link-op $n7 $n6 orient left

set udp [new Agent/UDP]
$ns attach-agent $n0 $udp
set udp1 [new Agent/UDP]
$ns attach-agent $n1 $udp1
$udp set class_ 0
$udp1 set class_ 1
set cbr [new Application/Traffic/CBR]
$cbr attach-agent $udp
set cbr1 [new Application/Traffic/CBR]
$cbr1 attach-agent $udp1
set null [new Agent/Null]
$ns attach-agent $n5 $null
$ns attach-agent $n6 $null
$ns connect $udp $null
$ns connect $udp1 $null
$udp set fid_ 2
$udp set window_ 8000
$udp set packetSize_ 552
$udp1 set fid_ 2
$udp1 set window_ 8000
$udp1 set packetSize_ 552
$ns rtproto DV
$ns rtmodel-at 1.0 down $n11 $n5
$ns rtmodel-at 1.0 down $n7 $n6
$ns at 0.0 "$cbr start"
$ns at 0.0 "$cbr1 start"
$ns at 5.0 "finish"
$ns run

