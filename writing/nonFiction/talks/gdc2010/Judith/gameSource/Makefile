

all: judith

clean:
	rm judith *.o


include Makefile.mac
#include Makefile.linux


SOURCE = main.cpp q3dmath.cpp quickcg.cpp

DEPENDENCY_FILE = Makefile.dependencies


${DEPENDENCY_FILE}: ${SOURCE}
	rm -f ${DEPENDENCY_FILE}
	${COMPILE} -MM ${SOURCE} >> ${DEPENDENCY_FILE}

include ${DEPENDENCY_FILE}


OBJECTS = ${SOURCE:.cpp=.o}

COMPILE = g++ -c -I${FMOD_API}/inc/



# 
# Generic:
#
# Map all .cpp C++ and C files into .o object files
#
# $@   represents the name.o file
# $<   represents the name.cpp file
#
.cpp.o:
	${COMPILE} -o $@ $<
.c.o:
	${COMPILE} -o $@ $<



judith: ${OBJECTS}
	g++ -o judith ${OBJECTS} ${LINK_LIBS}
	${POST_LINK}