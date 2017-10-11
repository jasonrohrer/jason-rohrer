

# 
# Generic:
#
# Map all .shtml into .html object files
#
# $@   represents the name.html file
# $<   represents the name.shtml file
#
%.html: %.shtml head.html foot.html navBar.html orderList.txt
	perl shtmlToHtml.pl orderList.txt $<


graph%.png: graph%.gnuplot graph%.data graphSetup.gnuplot
	gnuplot $<


ORDER_LIST := $(shell cat orderList.txt)

HTML_FILES = $(addsuffix .html, ${ORDER_LIST})

all: ${HTML_FILES} graph1.png graph2.png graph3.png graph4.png graph5.png

clean: 
	rm ${HTML_FILES} 