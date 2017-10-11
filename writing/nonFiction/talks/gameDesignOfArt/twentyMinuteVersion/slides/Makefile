

# 
# Generic:
#
# Map all .cpp C++ and C files into .o object files
#
# $@   represents the name.o file
# $<   represents the name.cpp file
#
%.html: %.shtml head.html foot.html
	perl shtmlToHtml.pl $<

HTML_FILES = \
  index.html \
  1.html \
  2.html \
  6.html \
  8.html \
  11.html \
  17.html \
  18.html \

all: ${HTML_FILES}

clean: 
	rm ${HTML_FILES} 
