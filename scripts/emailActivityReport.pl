#!/usr/bin/perl -w 

my $numArgs = $#ARGV + 1;

if( $numArgs != 4 ) {
    usage();
    }

$path = $ARGV[0];
$user = $ARGV[1];
$email = $ARGV[2];
$serverTag = $ARGV[3];



$sendmailPath = "/usr/lib/sendmail";

$modList = `find $path -mtime 0`;

$accessList = `last -n 5 $user `;


if( $modList ne "" or $accessList ne "" ) {


	open( SENDMAIL, "| $sendmailPath -t" ) or die; 
	
	print SENDMAIL "Subject: Server Activity Report for $serverTag\n"; 
	print SENDMAIL "To: $email\n"; 
	
	if( $modList ne "" ) {
		print SENDMAIL "File modification list:\n\n$modList\n\n";
	}
	else {
		print SENDMAIL "No file modifications\n\n";
	}

	if( $accessList ne "" ) {
		print SENDMAIL "Account access list:\n\n$accessList\n\n";
	}
	else {
		print SENDMAIL "No account accesses\n\n";
	}

	close( SENDMAIL );

}


sub usage {
    print "Usage:\n";
    print "  emailActivityReport.pl path username email_address server_tag\n";
    print "Example:\n";
    print "  emailActivityReport.pl public_html brd55 bob\@test.com MyServer\n";
    die;
    }
