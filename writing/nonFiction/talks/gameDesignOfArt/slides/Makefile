

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
  0.html \
  0a.html \
  1.html \
  2.html \
  3.html \
  4.html \
  5.html \
  6.html \
  6b.html \
  7.html \
  8.html \
  8a.html \
  9.html \
  10.html \
  10a.html \
  10b.html \
  10c.html \
  11.html \
  12.html \
  13.html \
  14.html \
  15.html \
  16.html \
  17.html \
  18.html \

all: ${HTML_FILES}

clean: 
	rm ${HTML_FILES} 
