#! /usr/bin/perl -w
#
# $Id: sample-director.pl,v 1.2 2002/05/06 03:02:40 evertonm Exp $
#
# sample-director.pl remote_port_number

#
# As example, we've chosen to receive the remote
# port number as command line argument. 
#
my $remote_port_number = $ARGV[0];
$remote_port_number = 2000 unless defined($remote_port_number);

#
# Enable flushing
#
$| = 1;

sub log {
    my $line = $_[0];
    system("echo $line >> /tmp/sample-director-$$.log");
}

sub reponse {
    my $line = $_[0];

    &log("OUT: $_");

    print $line, "\n";
}

#
# Infinite loop
#
while (<>) {
    chomp;

    &log("IN: $_");

    #
    # Identify source of incoming connection
    #    
    my ($protocol, $source_address, $source_port) = split;

    #
    # Reject connections other than from localhost
    #
    if ($source_address ne '127.0.0.1') {
	&response("reject");
	next;
    }

    #
    # Forward connections to localhost:2000
    #
    &response("forward localhost 2000");
}
