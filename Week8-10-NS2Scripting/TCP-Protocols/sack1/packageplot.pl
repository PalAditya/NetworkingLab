#!/usr/sup/gnu/bin/perl

#################################################################################
# File          : packageplot.pl
# Authour	: Ingela Anderton (ingela@docs.uu.se)
# Created       : 1998-03-31
# Last modified : Time-stamp: <1998-04-22 14:23:32 ingela>
# ------------------------------------------------------------------------------
# Description   : This script is a quick hack for extracting
#		  interesting information from the tracefiles form the
#		  ns-simulations run with the following scripts:
#		  tcp_one_drop.tcl, tcp_two_drops.tcl,
#		  tcp_three_drops.tcl and tcp_four_drops.tcl and can not
#		  be applayed to a arbitary ns-tracefile.

# Usage	        : cat <tracefile> | ./packageplot.pl <tcp
#		  implementation> ex: > cat out.tr | ./packageplot.pl
#	          reno It generates three gnuplot input files that
#		  should be ploted together. The names of these files
#		  are [sent | dropped | ack].<input argument> The input
#		  argument should be the tcp implementation used in the
#		  simulation script.  Ex gnuplot command line: gnuplot>
#		  plot [0:6] "sent.reno" with points,"dropped.reno",
#		  "ack.reno" with dots 
#################################################################################

$name = shift(@ARGV); 

open  OUTFILE, ">sent.$name"  or die "Couldn't open sent: $!\n";
open  OUTFILE1, ">dropped.$name"  or die  "Couldn't open dropped: $!\n";
open  OUTFILE2,  ">ack.$name"  or die  "Couldn't open ack: $!\n";

while (<stdin>) {
    # Package sent from node 0 to node 3.
    if(/^\+ ([0-9]+[\.0-9]*) 0 3 tcp [0-9]+ [^ ]+ 2 [0-9]+\.[0-9]+ [0-9]+\.[0-9]+ ([0-9]+) [0-9]+/) 
    {
	$time = $1;
	$packagenr = $2 % 60; 
	print OUTFILE "$time $packagenr\n";
	    
    }
    # Package belonging to flow 2 has been dropped from queue on node 3 on its way to node 4.
    elsif(/^d ([0-9]+[\.0-9]*) 3 4 tcp [0-9]+ [^ ]+ 2 [0-9]+\.[0-9]+ [0-9]+\.[0-9]+ ([0-9]+) [0-9]+/) 
    {
	$time = $1;
	$packagenr = $2 % 60;
	print OUTFILE1 "$time $packagenr\n";  
    }
    # Ack came in for flow 2.
    elsif(/^r ([0-9]+[\.0-9]*) 3 0 ack [0-9]+ [^ ]+ 2 [0-9]+\.[0-9]+ [0-9]+\.[0-9]+ ([0-9]+) [0-9]+/) 
    {
	$time = $1;
	$packagenr = $2 % 60;
	print OUTFILE2 "$time $packagenr\n";
    }    
    else 
    {
	# do nothing
    }
}


