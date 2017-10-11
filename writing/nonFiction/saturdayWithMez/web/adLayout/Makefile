

all: dvi ps pdf

dvi: both.dvi postCard.dvi
ps: bothAds.ps postCard.ps postCard_4up.ps
pdf: bothAds.pdf postCard.pdf postCard_4up.pdf



both.dvi: both.tex fullpagead.eps halfpagead.eps 
	latex both.tex

bothAds.ps: both.dvi
	dvips -t letter -o bothAds.ps both.dvi

bothAds.pdf: bothAds.ps
	ps2pdf bothAds.ps



postCard.dvi: postCard.tex postCard_front.eps postCard_back.eps 
	latex postCard.tex

postCard.ps: postCard.dvi
	dvips -t letter -o postCard.ps postCard.dvi

postCard.pdf: postCard.ps
	ps2pdf postCard.ps



postCard_4up.ps: postCard.ps
	cat postCard.ps | psselect -p1,1,1,1,2,2,2,2 -q | psnup -pletter -4 -q > postCard_4up.ps


postCard_4up.pdf: postCard_4up.ps
	ps2pdf postCard_4up.ps