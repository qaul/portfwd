#! /usr/bin/perl -w
#
# $Id: sample-director.pl,v 1.5 2002/05/08 03:50:03 evertonm Exp $
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
    system("echo `date` [$$] $line >> /tmp/sample-director-`date +%Y%m%d`.log");
}

sub response {
    my $line = $_[0];

    &log("OUT: $line");

    print $line, "\n";
}

sub catch_term {
    my $signame = shift;
    &log('Received SIGTERM - exiting');
    exit(0);
}
$SIG{TERM} = \&catch_term;

&log('Start');

#
# Infinite loop
#
while (<STDIN>) {
    chomp;

    &log(" IN: $_");

    #
    # Identify incoming connection
    # 
    # Client from "source_address:source_port" is
    # opening communication to "local_address:local_port"
    # with "protocol".
    #    
    my ($protocol, $source_address, $source_port, $local_address, $local_port) = split;

    #
    # Reject connections other than from localhost
    #
    if ($source_address ne '127.0.0.1') {
	&response("reject");
	next;
    }

    #
    # Forward connections to localhost:$remote_port_number
    #
    &response("forward localhost $remote_port_number");
}

&log("Can't read from stdin - exiting");

