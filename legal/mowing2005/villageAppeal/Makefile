TEX_FILES = doubleJeopardyQuotes.tex motionInvalidNotice.tex motionInvalidNotice_affidavit.tex motionInvalidNotice_letter.tex motionInvalidAffidavitOfFact.tex motionInvalidNotice_coverLetter.tex

DVI_FILES = ${TEX_FILES:.tex=.dvi}

PS_FILES = ${TEX_FILES:.tex=.ps}

PDF_FILES = ${TEX_FILES:.tex=.pdf}


all: dvi ps pdf


dvi: ${DVI_FILES}

ps: ${PS_FILES}

pdf: ${PDF_FILES}



# 
# Generic mappings:
#
.SUFFIXES: .tex .dvi .ps .pdf

# Map all .tex  files into .dvi  files
#
# $@   represents the name.dvi file
# $<   represents the name.tex file
#
# dvi file depends on .tex file
.tex.dvi:
	latex $<

# Map .dvi to .ps files
.dvi.ps:
	dvips -t letter -o $@ $<

# Map .ps files to .pdf files
.ps.pdf:
	ps2pdf $<
