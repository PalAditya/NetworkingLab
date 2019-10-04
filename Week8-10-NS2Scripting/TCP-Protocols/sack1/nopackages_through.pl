#!/usr/sup/gnu/bin/perl

#################################################################################
# File		: nopackages_throgh.pl
# Authour	: Ingela Anderton (ingela@docs.uu.se)
# Created	: 1998-04-06
# Last modified	: Time-stamp: <1998-04-22 14:23:08 ingela>
# ------------------------------------------------------------------------------
# Description	: Derives how many unique packages arrived between node <arg1> 
#                 and node <arg2> for the flow <arg3>.
# ################################################################################

$high = -1;
while (<stdin>) {
    # Checks the tracefile for transactions we are interested in, in order to derive 
    # how many unique packages arrived at the receiving end for the client we are studing. 
    next unless (/^r [0-9]+[\.0-9]* 3 4 tcp 1000 [^ ]+ 2 [0-9]+\.[0-9]+ [0-9]+\.[0-9]+ ([0-9]+) [0-9]+/);

    $seen[$1] = 1;

    for ($i = $high; $seen[$i]; $i++) {
	$newhigh = $i;
    }
}

print "$newhigh \n";
















