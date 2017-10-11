

# 
# Generic:
#
# Map all .cpp C++ and C files into .o object files
#
# $@   represents the name.o file
# $<   represents the name.cpp file
#
%.eps: %.sk
	sk2ps $< $@
%.dvi: %.tex
	latex $<
%.ps: %.dvi
	dvips -Ppdf -t letter -o $@ $<
%.pdf: %.ps
	ps2pdf $<


all: fullSail1.pdf fullSail2.pdf fullSail3.pdf fullSail3b.pdf fullSail4.pdf fullSail5.pdf fullSail6.pdf fullSail7.pdf fullSail8.pdf fullSail9.pdf fullSail10.pdf fullSail11.pdf fullSail12.pdf fullSail13.pdf fullSail14.pdf fullSail15.pdf fullSail16.pdf fullSail17.pdf fullSail18.pdf fullSail19.pdf fullSail20.pdf fullSail21.pdf fullSail22.pdf fullSail23.pdf fullSail24.pdf fullSail25.pdf fullSail26.pdf fullSail27.pdf fullSail28.pdf fullSail29.pdf fullSail30.pdf fullSail31.pdf fullSail32.pdf fullSail33.pdf fullSail34.pdf fullSail35.pdf fullSail36.pdf fullSail37.pdf fullSail38.pdf fullSail39.pdf fullSail40.pdf fullSail41.pdf fullSail42.pdf fullSail43.pdf fullSail44.pdf fullSail45.pdf

clean: 
	rm *.pdf *.ps *.dvi *.log *.aux
