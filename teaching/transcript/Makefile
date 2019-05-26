
transcriptBody.pdf: transcriptBody.ps
	ps2pdf transcriptBody.ps

transcriptBody.ps:  transcriptBody.dvi
	dvips transcriptBody.dvi

transcriptBody.dvi: transcriptBody.tex
	latex transcriptBody.tex

