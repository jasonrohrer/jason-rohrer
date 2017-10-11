

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






ORDER_LIST := $(shell cat orderList.txt)

HTML_FILES = $(addsuffix .html, ${ORDER_LIST})

all: ${HTML_FILES}

clean: 
	rm ${HTML_FILES}

