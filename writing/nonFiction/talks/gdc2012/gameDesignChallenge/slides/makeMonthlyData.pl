#!/usr/bin/perl -w

my $numArgs = $#ARGV + 1;

if( $numArgs != 1 ) {
    usage();
    }

open( CPI_FILE, $ARGV[0] ) or usage();


while( <CPI_FILE> ) {
    chomp;
	
	$line = $_;

	$line =~ s/^\s+//;
	$line =~ s/\s+$//;

    #print "LINE: $line\n";
	
	my @lineParts = split( /\s+/, $line );
	
	$year = $lineParts[0];

	for( $i=1; $i<13; $i++ ) {
		if( $#lineParts >= $i ) {
			$fraction = ( $i - 0.5 ) / 12;
			
			( my $zero, my $decimal ) = split( /\./, $fraction );

			$value = $lineParts[$i];
			
			print "$year.$decimal $value\n"
			}
	}

}

sub usage {
    print "Usage:\n";
    print "  makeMonthlyData.pl cpiDataFile\n";
    print "Example:\n";
    print "  makeMonthlyData.pl cpiAllData.txt\n";
    die;
    }
