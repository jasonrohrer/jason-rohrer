#!/usr/bin/perl -w 



my $numArgs = $#ARGV + 1;

if( $numArgs < 2 ) {
    usage();
    }

open( LIST_FILE, $ARGV[0] ) or usage();

print "opening $ARGV[0] for reading\n";


open( OUTPUT_FILE, ">$ARGV[1]" ) or usage();

print "opening $ARGV[1] for writing\n";


print OUTPUT_FILE 
"\\documentclass[12pt]{article}

\\usepackage{times,fullpage,epsfig}


\\pagestyle{empty}

\\begin{document}

{\\large
	 ";


while( <LIST_FILE> ) {
    chomp;

	if( $_ =~ /^(\d+)\)/ ) {
		# a line that starts like this:   17)
        # numeral saved in $1

        # auto-convert to latex-style quotations
        $quoteReplacement = "``";
        while( $_ =~ s/\"/$quoteReplacement/ ) {
            if( $quoteReplacement eq "``" ) {
                $quoteReplacement = "''";
                }
            else {
                $quoteReplacement = "``";
                }
            }

        my $paddedNumber = sprintf( "%02d", $1 );

		print OUTPUT_FILE "\\noindent" . $_;
        print OUTPUT_FILE "\n\n\\vspace{0.060in}\\hrule\n\n\\vspace{0.060in}";
	    }
    }

print OUTPUT_FILE 
"}

\\end{document}";


sub usage {
    print "Usage:\n";
    print "  essayToLatexCuePages.pl essay_file output_file\n";
    print "Example:\n";
    print "  essayToLatexCuePages.pl essay.txt essay.tex\n";
    die;
    }
