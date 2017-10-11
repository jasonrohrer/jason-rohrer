
MG_PATH = ../../../../../cpp

PLATFORM_PATH = ${MG_PATH}/minorGems/io/file/linux/PathLinux.cpp

STRING_UTILS = ${MG_PATH}/minorGems/util/stringUtils.cpp


all: genPicture code.pdf

random.o: random.h random.cpp
	g++ -g -c random.cpp -o random.o

genPicture: genPicture.cpp random.o
	g++ -g -o genPicture -I${MG_PATH} genPicture.cpp random.o ${PLATFORM_PATH} ${STRING_UTILS}



code.dvi: code.tex
	latex code.tex

# using T1 Times font, which doesn't work well with -P pdf
# fix it with the G0 switch

code.ps: code.dvi
	dvips -P pdf -G0 -o code.ps code.dvi


code.pdf: code.ps
	ps2pdf code.ps