
opendir( SLIDE_DIR, "microtalkSlides" ); 


while( defined ( $file = readdir SLIDE_DIR ) ) { 

	if( $file =~ m/jrohrer(\d+)/ ) {

		$number = $1;

		print "Making .shtml slide for:  microtalkSlides/$file\n";


		$slideFileName = "microtalk$number.shtml";

		# delete old one, if any
		unlink( $slideFileName );
		
		
		open( WRITE_FILE, ">$slideFileName" );

		print WRITE_FILE 
			"<br><img src='microtalkSlides/$file' width=750 height=422>";
		close( WRITE_FILE );
	}
} 

closedir( SLIDE_DIR );


use File::Copy;


copy( "microtalk01.shtml", "microtalk01b.shtml" );
