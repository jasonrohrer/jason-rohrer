##
# This script merges the text data in text_data.txt with the template
# files one_line_template.sk or two_line_template.sk, depending on how
# long the text is for each page in text_data.txt.
##

open( FILE, "text_data.txt" ) or die;

my @lineList = <FILE>;

foreach $line ( @lineList ) {
    
    chomp( $line );

    ( my $number, $text ) = split( /:/, $line );
    
    my $templateType = "";
    if( $number % 2 == 1 ) {
        # use odd page templates
        $templateType = "_odd";
    }

    print "\nProcessing $number\n";

    my $first_line = "";
    my $second_line = "";
    my $template_file_name = "";

    # FIXME:
    # Split long line roughly in half---it will look better.
    if( length( $text ) > 42 ) {
        
        my $halfLength = length( $text ) / 2;

        my @words = split( /\s+/, $text );
        
        $first_line = "";

        # first line should be just longer than half the length
        while( length( $first_line ) < $halfLength ) {
            my $nextWord = shift( @words );
            $first_line = $first_line . $nextWord . " ";
        }

        $second_line = join( " ", @words );
        
        # we ignored double-spaces after periods above.
        # add them back
        $first_line =~ s/\. /\.  /;
        $second_line =~ s/\. /\.  /;

        # remove trailing spaces
        $first_line =~ s/\s+$//;
        $second_line =~ s/\s+$//;

        print $first_line;
        print "\n";
        print $second_line;
        print "\n";

        $template_file_name = "two_line_template$templateType.sk";
    }
    else {
        $first_line = $text;
        print $first_line;
        print "\n";
        $template_file_name = "one_line_template$templateType.sk";
    }


    open( TEMPLATE_FILE, "$template_file_name" ) or die;
    my @templateLines = <TEMPLATE_FILE>;
    my $template = join( "", @templateLines );
    close( TEMPLATE_FILE );

    $template =~ s/\#NUMBER\#/$number/;
    $template =~ s/\#LINE_ONE\#/$first_line/;
    $template =~ s/\#LINE_TWO\#/$second_line/;

    
    open( OUT_FILE, ">$number.sk" ) or die;
    
    print OUT_FILE $template;
        
    close OUT_FILE;
}

close( FILE );

