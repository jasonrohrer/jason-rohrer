
opendir( SLIDE_DIR, "kl2" ); 


while( defined ( $file = readdir SLIDE_DIR ) ) { 

	if( $file =~ m/KaneLynch(\d+)/ ) {

		$number = $1;

		


		@subDigits = ( "a", "b", "c" );

		$numSubs = $#subDigits + 1;

		print "Making $numSubs .shtml slides for:  kl2/$file\n";

		foreach $subD ( @subDigits ) {


			$slideFileName = "KaneLynch$number$subD.shtml";

			# delete old one, if any
			unlink( $slideFileName );
			
			
			open( WRITE_FILE, ">$slideFileName" );
			
			print WRITE_FILE 
				"<br><img src='kl2/$file' width=683 height=374>";
			close( WRITE_FILE );
		}
	}
} 

closedir( SLIDE_DIR );


# delete two unused slides

unlink( "KaneLynch1b.shtml" );
unlink( "KaneLynch1c.shtml" );
