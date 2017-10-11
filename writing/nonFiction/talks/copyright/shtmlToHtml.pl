
my $fileName = $ARGV[0];


# read entire file with <  > operator
undef $/;

open( SHTML_FILE, "$fileName" );    
$shtmlContents = <SHTML_FILE>;
close( SHTML_FILE );

open( HEAD_FILE, "head.html" );    
$headContents = <HEAD_FILE>;
close( HEAD_FILE );

open( FOOT_FILE, "foot".".html" );    
$footContents = <FOOT_FILE>;
close( FOOT_FILE );


$shtmlContents =~ s/<!--#include virtual="head.html" -->/$headContents/;
$shtmlContents =~ s/<!--#include virtual="foot.html" -->/$footContents/;

#replace navigation links as well
$shtmlContents =~ s/.shtml/.html/g;


$fileName =~ s/shtml/html/;

open( WRITE_FILE, ">$fileName" );    
print WRITE_FILE $shtmlContents;
close( WRITE_FILE );
