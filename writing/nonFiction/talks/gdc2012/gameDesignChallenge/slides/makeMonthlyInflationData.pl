#!/usr/bin/perl -w

my $numArgs = $#ARGV + 1;

if( $numArgs != 1 ) {
    usage();
    }

open( CPI_FILE, $ARGV[0] ) or usage();


my $lastCPI = 0;

while( <CPI_FILE> ) {
    chomp;
	
	$line = $_;

	$line =~ s/^\s+//;
	$line =~ s/\s+$//;

    #print "LINE: $line\n";
	
	( my $time, my $cpi ) = split( /\s+/, $line );
	
	if( $lastCPI > 0 ) {
		# valid comparison point

		$monthInflation = $cpi / $lastCPI;

		$yearInflation = $monthInflation ** 12;

		$yearInflation -= 1;

		$yearInflation *= 100;

		print "$time $yearInflation\n";
	}
 

	$lastCPI = $cpi;
}

sub usage {
    print "Usage:\n";
    print "  makeMonthlyInflationData.pl cpiMonthlyDataFile\n";
    print "Example:\n";
    print "  makeMonthlyInflationData.pl cpiMonthly.txt\n";
    die;
    }
