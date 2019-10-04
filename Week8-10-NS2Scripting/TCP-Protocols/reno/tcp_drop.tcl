################################################################################
# File: tcp_drops.tcl
# Authour: Ingela Anderton (ingela@docs.uu.se)
# Created : 1998-04-07
# Last modified: Time-stamp: <1998-04-07 16:42:57 ingela>
# Description:
################################################################################

# Create a referense to the simulator class.
set ns [new Simulator]

# Definition of colors used to color diffrent flows.   
$ns color 0 green
$ns color 1 yellow
$ns color 2 blue
$ns color 3 red

# Open files to write the traces to.
set outputfile [open out.tr w]
$ns trace-all $outputfile
set namfile [open out.nam w]
$ns namtrace-all $namfile

# Create the following topology:             
#          node 0
#            \ 
#     8Mb,0ms \ 
#	       \ 
#       8Mb,0ms \       0.8Mb,100ms
# node 1 ------node 3 --------- node 4
#               /
#              /
#     8Mb,0ms /
#            /
#          node 2
#
# Create the nodes 
set node0 [$ns node]
set node1 [$ns node]
set node2 [$ns node]
set node3 [$ns node]
set node4 [$ns node]

# Create the links going in both directions. 
# Syntax: duplex-link <node0> <node1> <bandwidth> <delay> <queue_type>
$ns duplex-link $node0 $node3 8Mb 0.1ms DropTail
$ns duplex-link $node2 $node3 8Mb 0.1ms DropTail
$ns duplex-link $node1 $node3 8Mb 0.1ms DropTail
$ns duplex-link $node3 $node4 0.8Mb 100ms DropTail

# Create layout for nam 
$ns duplex-link-op $node0 $node3 orient right-down
$ns duplex-link-op $node2 $node3 orient right-up
$ns duplex-link-op $node3 $node1 orient left
$ns duplex-link-op $node3 $node4 orient right

# Show the queues between these nodes 
$ns duplex-link-op $node3 $node4 queuePos 0.5

# Set a queue limit 
$ns queue-limit $node3 $node4 8

# Create TCP agents

# ------ These are the agents of the conection studied in this simulation --- #
# ------ the other agents just help to create the scenario.  ---------------- # 

set tcp [new Agent/TCP/Reno]		
$tcp set fid_ 2		       ;# Belongs to flow two and will be colored blue
$tcp set window_ 28

set sink [new Agent/TCPSink]    
$ns attach-agent $node0 $tcp
$ns attach-agent $node4 $sink
$ns connect $tcp $sink

# --------------------------------------------------------------------------- #

set tcp1 [new Agent/TCP]		
$tcp1 set fid_ 3	        ;# Belongs to flow three and will be colored red
$tcp1 set window_ 8
 
set sink1 [new Agent/TCPSink]    
$ns attach-agent $node1 $tcp1

$ns attach-agent $node4 $sink1
$ns connect $tcp1 $sink1

set tcp2 [new Agent/TCP]		
$tcp2 set fid_ 1	        ;# Belongs to flow one and will be colored yellow
$tcp2 set window_ 8

set sink2 [new Agent/TCPSink]    
$ns attach-agent $node2 $tcp2
$ns attach-agent $node4 $sink2
$ns connect $tcp2 $sink2

proc drop_1 {} {
    global tcp tcp1 tcp2 ns
    
    # Create FTP source "applications" and associate them with the TCP senders.  
    set source [new Source/FTP]	
    $source attach $tcp		
    
    set source1 [new Source/FTP]	
    $source1 attach $tcp1		
    $source1 set maxpkts_ 12

    set source2 [new Source/FTP]	
    $source2 attach $tcp2		
    $source2 set maxpkts_ 10
    
    $ns at 1.0 "$source start"	;# Start the source after 1.0 s
    $ns at 1.2 "$source1 start"	;# Start the source after 1.2 s
    $ns at 1.2 "$source2 start"	;# Start the source after 1.2 s
    $ns at 6.0 "finish"         ;# Finish the simulation after 6.0 s

    $ns run
}

proc drop_2 {} {
    global tcp tcp1 tcp2 ns
    
    # Create FTP source "applications" and associate them with the TCP senders.  
    set source [new Source/FTP]	
    $source attach $tcp		
    
    set source1 [new Source/FTP]	
    $source1 attach $tcp1		
    $source1 set maxpkts_ 12
    
    set source2 [new Source/FTP]	
    $source2 attach $tcp2		
    $source2 set maxpkts_ 11
    
    $ns at 1.0 "$source start"	;# Start the source after 1.0 s
    $ns at 1.2 "$source1 start"	;# Start the source after 1.2 s
    $ns at 1.2 "$source2 start"	;# Start the source after 1.2 s
    $ns at 6.0 "finish"         ;# Finish the simulation after 6.0 s
    
    $ns run
}

proc drop_3 {} {
    global tcp tcp1 tcp2 ns
    
    # Create FTP source "applications" and associate them with the TCP senders.  
    set source [new Source/FTP]	
    $source attach $tcp		
    
    set source1 [new Source/FTP]	
    $source1 attach $tcp1		
    $source1 set maxpkts_ 12

    set source2 [new Source/FTP]	
    $source2 attach $tcp2		
    $source2 set maxpkts_ 12
    
    $ns at 1.0 "$source start"	;# Start the source after 1.0 s
    $ns at 1.2 "$source1 start"	;# Start the source after 1.2 s
    $ns at 1.18 "$source2 start";# Start the source after 1.18 s
    $ns at 6.0 "finish"         ;# Finish the simulation after 6.0 s
    
    $ns run
}

proc drop_4 {} {
    global tcp tcp1 tcp2 ns
    
    # Create FTP source "applications" and associate them with the TCP senders.  
    set source [new Source/FTP]	
    $source attach $tcp		
    
    set source1 [new Source/FTP]	
    $source1 attach $tcp1		
    $source1 set maxpkts_ 12
    
    set source2 [new Source/FTP]	
    $source2 attach $tcp2		
    $source2 set maxpkts_ 13
    
    $ns at 1.0 "$source start"	;# Start the source after 1.0 s
    $ns at 1.2 "$source1 start"	;# Start the source after 1.2 s
    $ns at 1.2 "$source2 start"	;# Start the source after 1.2 s
    $ns at 6.0 "finish"         ;# Finish the simulation after 6.0 s

    $ns run
}

# Procedure finish: Writes trace data to the tracefile(s) and starts nam with the 
# nam-tracefile.
proc finish {} {
    global ns namfile outputfile
    $ns flush-trace
    close $namfile
    close $outputfile
    
    puts "running nam..."
    exec nam out.nam &
    exit 0
}

# Check that the number of arguments is correct
if { $argc != 1 } {
    puts stderr {usage: ns tcp_drop n where n  belongs to {1, 2, 3, 4}}
    exit 1
}

# Check that the argument is in the correct range
if { "[info procs drop_$argv]" != "drop_$argv" } {
    puts stderr "tcp_drop.tcl: no such test: drop_$argv"
    exit 1
}

# Run the simulation with <$argv> packet drops.
drop_$argv

