my $numArgs = $#ARGV + 1;

if( $numArgs != 2 ) {
    usage();
    }


my $listFileName = $ARGV[0];

my $fileName = $ARGV[1];


# read entire file with <  > operator
undef $/;


open( LIST_FILE, "$listFileName" );    
$listContents = <LIST_FILE>;
close( LIST_FILE );

open( SHTML_FILE, "$fileName" );    
$shtmlContents = <SHTML_FILE>;
close( SHTML_FILE );

open( HEAD_FILE, "head.html" );    
$headContents = <HEAD_FILE>;
close( HEAD_FILE );

open( FOOT_FILE, "foot".".html" );    
$footContents = <FOOT_FILE>;
close( FOOT_FILE );


open( NAV_BAR_FILE, "navBar".".html" );    
$navBarContents = <NAV_BAR_FILE>;
close( NAV_BAR_FILE );



$fileNumber = $fileName;

$fileNumber =~ s/.shtml//;


@listElements = split( /\s+/, $listContents );





# for nav bar links
$prevNumber;
$nextNumber;

for( $i=0; $i<=$#listElements; $i++ ) {

	if( $listElements[$i] eq $fileNumber ) {
		
		if( $i == 0 ) {
			# wrap back to last
			$prevNumber = $listElements[ $#listElements ];
		}
		else { 
			$prevNumber = $listElements[ $i - 1 ];
		}

		if( $i == $#listElements ) {
			# wrap back to first
			$nextNumber = $listElements[ 0 ];
		}
		else { 
			$nextNumber = $listElements[ $i + 1 ];
		}
	}
}


$navBarContents =~ s/#PREV/$prevNumber/;
$navBarContents =~ s/#NEXT/$nextNumber/;



$fileName =~ s/shtml/html/;

open( WRITE_FILE, ">$fileName" );    
print WRITE_FILE $headContents;
print WRITE_FILE $navBarContents;
print WRITE_FILE $shtmlContents;
print WRITE_FILE $footContents;
close( WRITE_FILE );




sub usage {
    print "Usage:\n";
    print "  shtmlToHtml.pl order_file_name shtml_file_to_process\n";
    print "Example:\n";
    print "  shtmlToHtml.pl orderList.txt 0.shtml\n";
    die;
    }
