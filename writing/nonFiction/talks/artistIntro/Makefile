

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
  0a.html \
  0b.html \
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


all: ${HTML_FILES}

clean: 
	rm ${HTML_FILES} 
