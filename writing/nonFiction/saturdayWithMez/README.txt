How to build Saturday With Mez from the source files:

Required software:

-- Sketch (now called Skencil)  http://www.skencil.org/
-- LaTeX


Steps:

1.  Download all of the PNG files for the various pages and place them in the
    same folder as these source files.

2.  Generate an EPS file for each page.  At the command line, type:
    sketch --run-script convertToPS.py

3.  Build the full book PDF.  Run the following script:
    makeFullBook



Editing the text:

If you want to edit the text on the story pages, look in the file 
text_data.txt

Then, to re-generate the .sk files for each page, run the Perl script:
perl assemble.pl


If you want to edit the text on the back cover, edit the file
back_cover_text.tex

Then run the following script:
makeBackCoverText



After changing any of the text, you must perform steps 1-3 again to generate
the EPS files and the full book PDF




Jason Rohrer
Potsdam, NY
May 2006
