

# 
# Generic:
#
# Map all .shtml into .html object files
#
# $@   represents the name.html file
# $<   represents the name.shtml file
#
%.html: %.shtml head.html foot.html navBar.html orderList.txt
	perl shtmlToHtml.pl orderList.txt $< head.html


# 
# Generic:
#
# Map all .wshtml into WHITE BACKGROUND .html object files
#
# $@   represents the name.html file
# $<   represents the name.wshtml file
#
%.html: %.wshtml head_white.html foot.html navBar.html orderList.txt
	perl shtmlToHtml.pl orderList.txt $< head_white.html


graph%.png: graph%.gnuplot graph%.data graphSetup.gnuplot
	gnuplot $<




ORDER_LIST := $(shell cat orderList.txt)

HTML_FILES = $(addsuffix .html, ${ORDER_LIST})

all: ${HTML_FILES} inflationGraph.eps cpiGraph.eps #graph1.png graph2.png graph3.png graph4.png graph5.png

clean: 
	rm ${HTML_FILES} 



inflationGraph.eps: inflation.gnuplot cpiAllData.txt graphSetup.gnuplot
	gnuplot inflation.gnuplot

cpiGraph.eps: cpi.gnuplot cpiMonthly.txt graphSetup.gnuplot
	gnuplot cpi.gnuplot
