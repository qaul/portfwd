#! /usr/bin/perl -w
#
# $Id: sample-director.pl,v 1.1 2002/05/05 08:55:52 evertonm Exp $
#
# sample-director.pl default_remote_host default_remote_port

$| = 1;

my ($default_remote_host, $default_remote_port) = @ARGV;

while (<>) {
	chomp;

	my ($source_address, $source_port) = split;

	if ($source_address eq '127.0.0.1') {
		print "forward $default_remote_host $default_remote_port\n";
		next;
	}

	print "reject\n";
}
