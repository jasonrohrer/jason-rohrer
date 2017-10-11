

# 
# Generic:
#
# Map all .cpp C++ and C files into .o object files
#
# $@   represents the name.o file
# $<   represents the name.cpp file
#
%.html: %.shtml head.html head_white.html foot.html
	perl shtmlToHtml.pl $<

HTML_FILES = \
  index.html \
  1.html \
  2.html \
  3.html \
  4.html \
  5.html \
  6.html \
  7.html \
  8.html \
  9.html \
  10.html \
  11.html \
  12.html \
  13.html \
  14.html \
  15.html \
  16.html \
  17.html \
  18.html \
  19.html \
  20.html \
  21.html \

all: ${HTML_FILES}

clean: 
	rm ${HTML_FILES} 
